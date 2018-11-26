#include "clientservice.h"

void ClientService::saveDataAndProperties()
{
 QFile file("data.bin");
 if(file.open(QFile::WriteOnly))
	{
	 QDataStream out(&file);
	 out<<nick<<name<<*pserverAddress<<*pserverPort;

	 if(!clients.isEmpty())
		out<<clients;

	 file.close();
	}
 else
	qCritical()<<"Error saving clients base";
}

void ClientService::restoreDataAndProperties()
{
 QFile file("data.bin");
 if(file.open(QFile::ReadOnly))
	{
	 QDataStream in(&file);
	 in>>nick>>name;

	 in>>*pserverAddress>>*pserverPort;

	 in>>clients;
	 file.close();

	 addAllUsersToOfflineModel();
	}
 else if(QFile::exists(file.fileName()))
	qWarning()<<"Error restoring clients base";
}

void ClientService::addAllUsersToOfflineModel()
{
 QStringList offlines;
 if(!clients.isEmpty())
	offlines.append(clients.keys());

 QString usernick="Вы: "+nick;
 offlines.prepend(usernick);

 ponlineModel->setStringList(QStringList());
 pofflineModel->setStringList(offlines);
}

ClientService::ClientService(QWidget *prnt)
 :QObject (prnt)
 ,pparent(prnt)
 ,psocket(new QTcpSocket)
 ,pserverAddress(new QString)
 ,pserverPort(new QString)
 ,ponlineModel(new QStringListModel)
 ,pofflineModel(new QStringListModel)
{
 restoreDataAndProperties();

 connect(psocket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(psocket, SIGNAL(connected()), SIGNAL(connected()));
 connect(psocket, SIGNAL(disconnected()), SIGNAL(disconnected()));
 connect(psocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
 connect(psocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
}

ClientService::~ClientService()
{
 saveDataAndProperties();
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

void ClientService::setNickAndName(QString nick, QString name)
{
 this->nick=nick;
 this->name=name;
}

void ClientService::slotConnected()
{
	qInfo()<<"Соединение с сервером установлено.";

	//говорим серверу что мы только что подключились и нам нужно сверить базу
	slotSentToServer(DATATYPE::CONNECT);

	qInfo()<<"Отправлен запрос на получение списка";
}

void ClientService::slotReadyRead()
{
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
	qDebug()<<"новое событие ";

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
		 if(serverBaseSize!=clients.size())
			{
			 qDebug()<<"база не сошлась, ждем новый";
			 in>>clients;
			}

		 QList<QString> onlines;
		 in>>onlines;
		 qDebug()<<"Приняли как онлайн: ";
		 for(auto s : onlines)
			qDebug()<<s;

		 removeOnlinesFromOfflines(onlines);

		 break;
	 }
	 case DATATYPE::DISCONNECTION:
		{
		 qDebug()<<"кто-то отсоединился";
		 QString disconnected;
		 in>>disconnected;
		 qDebug()<<disconnected<<" отсоединился";
//		 onlines.removeOne(disconnected);

//		 ponlineList->findItems(disconnected, Qt::MatchExactly).first();
//		 for(auto i=0, s=ponlineList->count();
//				 i<s; ++i)
//			{
//			 if(ponlineList->item(i)->text()==disconnected)
//				{
//				 delete ponlineList->takeItem(i);
//				 break;
//				}
//			}

//		 pofflineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/offline.png"),disconnected));
//		 pofflineList->sortItems();

		 QString msg=disconnected;
		 msg+=" вышел";

//		 if(parentWidget()->parentWidget()->isMinimized()
//				|| parentWidget()->parentWidget()->isHidden())
//			ptray->showMessage("Новое событие", msg, QSystemTrayIcon::Information, 3000);
//		 else
//			popup->showNotify(std::move(msg), mapToGlobal(pos()));

		 break;
		}
	 case DATATYPE::NOTIFYING:
		{
		 qDebug()<<"есть новый подсоединившийся";
		 QString connected;
		 in>>connected;
		 //убираем из недоступных подсоединившегося
//		 for(auto i=0;
//				 i<pofflineList->count(); ++i)
//			{
//			 if(pofflineList->item(i)->text()==connected)
//				{
//				delete pofflineList->takeItem(i);
//				 break;
//				}
//			}

		 //и добавляем к доступным
//		 ponlineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/online.png"), connected));
//		 ponlineList->sortItems();

		 QString msg=connected;
		 msg+=" доступен";

//		 if(parentWidget()->parentWidget()->isMinimized()
//				|| parentWidget()->parentWidget()->isHidden())
//			ptray->showMessage("Новое событие", msg, QSystemTrayIcon::Information, 3000);
//		 else
//			popup->showNotify(std::move(msg), mapToGlobal(pos()));


		 break;
		}
	 case DATATYPE::MESSAGE:
	 {
		 //новое сообщение
		 qDebug()<<"новое  сообщение";
		 in>>time>>msg;
		 msg.prepend(time.toString("[hh:mm:ss] "));
//		 msg=time.toString("[hh:mm:ss] ")+"Новое сообщение: "+msg;
//		 pinfo->append(msg);

//		 if(parentWidget()->parentWidget()->isMinimized()
//				|| parentWidget()->parentWidget()->isHidden())
//			ptray->showMessage("Новое событие", "Новое сообщение", QSystemTrayIcon::Information, 3000);
//		 else
//			popup->showNotify("***Новое сообщение***", mapToGlobal(pos()));

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

	QMessageBox::critical(pparent,
											 "Ошибка подключения к серверу", info);

	qCritical()<<"Ошибка соединения с сервером: "<<psocket->errorString();
	psocket->close();
}

void ClientService::slotConnectToServer()
{
 psocket->connectToHost(*pserverAddress,
												static_cast<quint16>(pserverPort->toInt()));
}

void ClientService::slotDisconnectFromServer()
{
 psocket->close();
 throwOnlinesToOfflines();

 emit disconnected();
}

void ClientService::slotSentToServer(DATATYPE type, QString msg, QVariant additionData)
{
 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)<<type<<QTime::currentTime();


 switch(type)
	{
	case DATATYPE::REGISTRATION:
	 out<<msg<<additionData.toString();
	 break;

	case DATATYPE::MESSAGE:
 /*//когда общая рассылка, другой клиент не сможет узнать,
 //кто прислал мсг, т.к. получит от сервера, поэтому отправителю
 //нужно приписать, что это он.
 //Если это будет Р2Р, возможно это будет не обяз. Нужно еще раз обдумать...
 //а пока так.*/
	 msg.prepend(nick+": ");
	 out<<msg;
	 qDebug()<<"msg send";
	 break;

	case DATATYPE::CONNECT:
	 out<<nick<<clients.size();
	 break;
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

 if(!offlines.empty())
	offlines.pop_front();

 onlines.removeOne(nick);
 onlines.prepend("Вы: "+nick);

 for(auto o:onlines)
	if(offlines.contains(o))
	 offlines.removeOne(o);

 ponlineModel->setStringList(onlines);
 pofflineModel->setStringList(offlines);
}

void ClientService::throwOnlinesToOfflines()
{
 QStringList onlines= ponlineModel->stringList();
 QStringList offlines=pofflineModel->stringList();

 onlines.pop_front();
 offlines.append(onlines);
 offlines.prepend("Вы: nick");

 ponlineModel->setStringList(QStringList());
 pofflineModel->setStringList(offlines);
}
