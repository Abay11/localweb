#include "clientservice.h"

void ClientService::saveDataAndProperties()
{
 QFile file(defaultSavingFile);
 if(file.open(QFile::WriteOnly))
	{
	 QDataStream out(&file);
	 out<<nick<<name<<*pserverAddress<<*pserverPort<<*clients;
	 file.close();
	}
 else
	qCritical()<<"Error saving clients base";
}

void ClientService::restoreDataAndProperties()
{
 QFile file(defaultSavingFile);
 if(file.open(QFile::ReadOnly))
	{
	 QDataStream in(&file);
	 in>>nick>>name>>*pserverAddress>>*pserverPort>>*clients;
	 file.close();

	 addAllUsersToOfflineModel();
	 return;
	}
 else if(QFile::exists(file.fileName()))
	qWarning()<<"Error restoring clients base";

 *pserverAddress=defaultServerAddress;
 *pserverPort=defaultServerPort;
}

void ClientService::addAllUsersToOfflineModel()
{
 QStringList offlines;
 QStringList nicksInBase=clients->keys();
 if(!nick.isEmpty())
	{
	 QString usernick="Вы: "+nick;
	 offlines.prepend(usernick);
	 nicksInBase.removeOne(nick);
	}

 offlines.append(nicksInBase);

 ponlineModel->setStringList(QStringList());
 pofflineModel->setStringList(offlines);
}

void ClientService::addNewOnlineToModel(QString nick)
{
 int nIndex=ponlineModel->rowCount();
 ponlineModel->insertRow(nIndex);
 ponlineModel->setData(ponlineModel->index(nIndex), nick);
}

ClientService::ClientService(QWidget *prnt)
 :QObject (prnt)
 ,psocket(new QTcpSocket)
 ,clients(new CLIENTBASE)
 ,ponlineModel(new QStringListModel)
 ,pofflineModel(new QStringListModel)
 ,pserverAddress(new QString)
 ,pserverPort(new QString)
{
 restoreDataAndProperties();

 connect(psocket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(psocket, SIGNAL(connected()), SIGNAL(connected()));
 connect(psocket, SIGNAL(disconnected()), SLOT(slotDisconnected()));
 connect(psocket, SIGNAL(disconnected()), SIGNAL(disconnected()));
 connect(psocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
 connect(psocket, SIGNAL(error(QAbstractSocket::SocketError)),
				 this, SLOT(slotError(QAbstractSocket::SocketError)));
}

ClientService::~ClientService()
{
 saveDataAndProperties();
}

void ClientService::appendOwnerUserToOnlines()
{
 //this method may be usefull only on registration
 ponlineModel->insertRow(0);
 auto firstIndex=ponlineModel->index(0);
 ponlineModel->setData(firstIndex, "Вы: " + nick);
}

bool ClientService::socketIsOpen()
{
 return psocket->isOpen();
}

QStringListModel* ClientService::onlineModel()
{
 return ponlineModel;
}

QStringListModel *ClientService::offlineModel()
{
 return pofflineModel;
}

QString ClientService::serverAddress()
{
 return *pserverAddress;
}

QString ClientService::serverPort()
{
 return *pserverPort;
}

quint16 ClientService::clientPort()
{
 return psocket->localPort();
}

void ClientService::setAddress(QString addr)
{
 *pserverAddress=addr;
}

void ClientService::setPort(QString port)
{
 *pserverPort=port;
}

void ClientService::setNickAndName(QString nick, QString name)
{
 this->nick=nick;
 this->name=name;
}

bool ClientService::isConnected()
{
 return psocket->state()==QTcpSocket::ConnectedState;
}

void ClientService::slotConnected()
{
 QTime actionTime=QTime::currentTime();
 qInfo()<<formatTimeToString(actionTime)<<" Соединение с сервером установлено.";

 //говорим серверу что мы только что подключились и нам нужно сверить базу
 slotSendToServer(DATATYPE::CONNECT);
 emit newMessageForNotification("Соединение с сервером установлено!");
 emit newMessageForDisplay("Соединение с сервером установлено!", actionTime);

 qInfo()<<actionTime<<"Отправлен запрос на получение списка";
}

void ClientService::slotDisconnected()
{
 QTime actionTime=QTime::currentTime();
	qInfo()<<formatTimeToString(actionTime)<<" Соединение с сервером разорвано.";
	emit newMessageForNotification("Соединение с сервером разорвано!");
	emit newMessageForDisplay("Соединение с сервером разорвано!", actionTime);
}

void ClientService::slotReadyRead()
{
 qDebug()<<"read server event";

 QDataStream in(psocket) ;
 in.setVersion(QDataStream::Qt_5_11);

 forever
 {
	if(!mnNextBlockSize)
	 {
		if(static_cast<size_t>(psocket->bytesAvailable())<sizeof(quint16))
		 break;

		in>>mnNextBlockSize;
	 }

	if(psocket->bytesAvailable()<mnNextBlockSize)
	 break;

	QTime time;
	QString msg;

	DATATYPE type;
	in>>type;

	switch (type) {
	 case DATATYPE::REGISTRATION:
		in>>time>>registrationResult;

		emit returnRegistrationResult(registrationResult);

		qInfo()<<time.toString("[hh:mm:ss] ")
					<<"Registration attempt. Result: "<<registrationResult;
		break;

	 case DATATYPE::CONNECT:
	 {
		 qDebug()<<"Receiving list of clients";

		 //если база сервера не совпадает с нашим,
		 //ждем актуальную базу
		 int serverBaseSize;
		 in>>serverBaseSize;
		 bool hasClientActualBase=serverBaseSize==clients->size();
		 if(!hasClientActualBase)
			{
			 //receive and update base
			 in>>*clients;
			 qDebug()<<"база не сошлась, получена новая с размером:"<<clients->size();
			 addAllUsersToOfflineModel();
			}
		 else
			qDebug()<<"база сошлась, синхрон не нужен";

		 QList<QString> onlines;
		 in>>onlines;
		 removeOnlinesFromOfflines(onlines);
		 break;
	 }
	 case DATATYPE::DISCONNECTION:
		{
		 qDebug()<<"кто-то отсоединился";
		 QString disconnected;
		 in>>disconnected;
		 throwDisconnectedToOfflines(disconnected);

		 QString msg=disconnected;
		 msg+=" вышел";

		 emit(newMessageForNotification(msg));
		 emit(newMessageForDisplay(msg));
		 break;
		}
	 case DATATYPE::NOTIFYING:
		{
		 QString connectedUserNick;
		 in>>connectedUserNick;
		 qDebug()<<"есть новый подсоединившийся:"<<connectedUserNick;
		 addNewOnlineToModel(connectedUserNick);
		 removeConnectedFromOfflines(connectedUserNick);

		 QString msg=connectedUserNick + " стал доступен";
		 emit(newMessageForNotification(msg));
		 emit(newMessageForDisplay(msg));
		 break;
		}
	 case DATATYPE::MESSAGE:
	 {
		 qDebug()<<"новое  сообщение";
		 in>>time>>msg;
		 receivedMessage=msg;
		 emit(newMessageForDisplay(msg, time));
		 emit(newMessageForNotification("***Новое сообщение***"));
		 break;
	 }
	 default:
		//что-то пошло не так, клиент не может получить иную команду
		 qDebug()<<"что-то пошло не так я полагаю";
		 qDebug()<<"но что?!?!";
		 qDebug()<<"но это уже не моя забота";

		 qCritical()<<"Неизвестная ошибка при получении сообщения.";
	 }

	mnNextBlockSize=0;
 }
}

void ClientService::slotError(QAbstractSocket::SocketError nerr)
{
 QString info;
 switch(nerr)
	{
	case QAbstractSocket::HostNotFoundError:
	 info.append("Удаленный сервер не найден. "
							 "Удостоверьте, что введен правильный адрес и порт сервера.");
	 break;
	case QAbstractSocket::RemoteHostClosedError:
	 info.append("Удаленный сервер закрыл соединение. "
							 "Обратитесь к администратору сети.");
	 break;
	case QAbstractSocket::ConnectionRefusedError:
	 info.append("Удаленный сервер отказал в доступе. "
							 "Обратитесь к администратору сети.");
	 break;
	default:
	 info.append("Произошла критическая ошибка. "
							 "Обратитесь к администратору сети.");
	}

 emit socketError("Ошибка соединения", info);
 qCritical()<<"Ошибка соединения с сервером: "<<psocket->errorString();
 throwOnlinesToOfflines();
 psocket->close();
}

void ClientService::slotConnectToServer()
{
 psocket->connectToHost(*pserverAddress,
												static_cast<quint16>(pserverPort->toInt()));
}

void ClientService::slotDisconnectFromServer()
{
 qDebug()<<"Disconnected from server";
 throwOnlinesToOfflines();
 psocket->close();
}

void ClientService::slotSendToServer(DATATYPE type, QString msg, QVariant additionData)
{
 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);

 QTime curTime=QTime::currentTime();
 out<<quint16(0)<<type<<curTime;

 switch(type)
	{
	case DATATYPE::MESSAGE:
	 {
	 qDebug()<<"Client send message";
 /*//когда общая рассылка, другой клиент не сможет узнать,
 //кто прислал мсг, т.к. получит от сервера, поэтому отправителю
 //нужно приписать, что это он.
 //Если это будет Р2Р, возможно это будет не обяз. Нужно еще раз обдумать...
 //а пока так.*/
	 emit newMessageForDisplay("Вы: "+msg);
	 msg.prepend(nick+": ");
	 out<<msg;
	 break;
	 }

	case DATATYPE::REGISTRATION:
	 {
		qDebug()<<"Sending to server registration request";
		QString *nick=&msg;
		QString name=additionData.toString();
		out<<*nick<<name;
		break;
	 }

	case DATATYPE::CONNECT:
	 {
	 qDebug()<<"Sending to server size of base:"<<clients->size();
	 out<<nick<<clients->size();
	 break;
	 }

	default: break;
	}

 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

 psocket->write(arrBlock);
}

void ClientService::slotSetAddress(QString addr, QString port)
{
 *pserverAddress=addr;
 *pserverPort=port;
}

void ClientService::removeOnlinesFromOfflines(QStringList onlines)
{
 QStringList offlines=pofflineModel->stringList();

 if(!nick.isEmpty())
	{
	 if(!offlines.empty())
		offlines.pop_front();

	 if(onlines.removeOne(nick))
		onlines.prepend("Вы: "+nick);
	}

 for(auto o:onlines)
	if(offlines.contains(o))
	 offlines.removeOne(o);

 ponlineModel->setStringList(onlines);
 pofflineModel->setStringList(offlines);
}

void ClientService::removeConnectedFromOfflines(QString connected)
{
 int nrow = pofflineModel->stringList().indexOf(connected);
 if(nrow!=-1)
	 pofflineModel->removeRow(nrow);
}

void ClientService::throwOnlinesToOfflines()
{
 QStringList onlines=ponlineModel->stringList();
 QStringList offlines=pofflineModel->stringList();

 if(!onlines.empty())
	{
	 onlines.pop_front();
	 offlines.append(onlines);
	 offlines.prepend("Вы: "+nick);
	}

 ponlineModel->setStringList(QStringList());
 pofflineModel->setStringList(offlines);
}

void ClientService::throwDisconnectedToOfflines(QString disconnected)
{
 int nrow = ponlineModel->stringList().indexOf(disconnected);
 if(nrow!=-1)
	{
	 ponlineModel->removeRow(nrow);
	 int i=pofflineModel->rowCount();
	 pofflineModel->insertRow(pofflineModel->rowCount());
	 pofflineModel->setData(pofflineModel->index(i), disconnected);
	}
}
