#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
 :QMainWindow(parent)
 ,plblAddress(new QLabel("IP адрес:"))
 ,plblPort(new QLabel("Номер порта:"))
 ,pleAddress(new QLineEdit)
 ,plePort(new QLineEdit("7165"))
 ,pInfo(new QTextEdit)
 ,pcmdOn(new QPushButton("Пуск"))
 ,pcmdOff(new QPushButton("Стоп"))
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
 ,pvalidator(new QIntValidator(1000, 65535, plePort))
 ,pdock(new QDockWidget("Пользователи", this))
 ,plist(new QListWidget())
 ,pserver(new QTcpServer(this))
 ,logger(new MyLogger)
 ,m_nextBlockSize(0)
{
 plePort->setValidator(pvalidator);

 pcmdOff->setEnabled(false);

 pInfo->setText(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
								+"Сервер не запущен.");
 pInfo->setReadOnly(true);

 foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
	 if (address.protocol() == QAbstractSocket::IPv4Protocol
			 && address != QHostAddress(QHostAddress::LocalHost))
		m_address=address.toString();
	}

 pleAddress->setText(m_address);
 pleAddress->setReadOnly(true);
 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdOn);
 phlay->addWidget(pcmdOff);

 pvlay->addLayout(phlay);
 pvlay->addWidget(pInfo);

 connect(pcmdOn, SIGNAL(clicked()), SLOT(slotStartServer()));
 connect(pcmdOff, SIGNAL(clicked()), SLOT(slotStopServer()));

 addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, pdock);
 readBase();
 pdock->setWidget(plist);

 setCentralWidget(new QWidget(this));
 centralWidget()->setLayout(pvlay);
 resize(800, 480);
}

MyWidget::~MyWidget()
{
 delete logger;
 saveBase();
}

void MyWidget::sendToClient(
	QTcpSocket* psocket,
	const QString& msg,
	const QTime &curTime)
{
 QByteArray byteArr;
 QDataStream out(&byteArr, QIODevice::WriteOnly);
 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)
	 <<static_cast<int>(DATATYPE::MESSAGE)
	 <<curTime<<msg;
 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(byteArr.size())-sizeof (quint16));

 psocket->write(byteArr);
}

void MyWidget::updateClientBase(ClientInfo& )
{
// ClientInfo* temp=dynamic_cast<ClientInfo*>(&pinfo);
// if(temp) //means need to register a new user
//	{
//	 addNewUser(*temp);
//	 return;
//	}

// //else we just need only update client's ip and port
// auto it=clientbase.find(pinfo.nickname());
// (*it)->address()=pinfo.address();
// (*it)->port()=pinfo.port();
}

void MyWidget::addNewUser(ClientInfo& )
{
// ClientInfo* temp =new ClientInfo(
//		dynamic_cast<ClientInfo&>(pclient));

// clientbase.insert(temp->nickname(), std::move(temp));
}

void MyWidget::removeUser(QString nickname)
{
 clientbase.remove(nickname);
}

void MyWidget::saveBase()
{
qDebug()<<"File saving";
 if(clientbase.isEmpty())
	return;

 QFile file("clientbase.bin");
 if(file.open(QFile::WriteOnly))
	{
	 QDataStream out(&file);
	 out<<clientbase;
	 file.close();
	}
 else
	qCritical()<<"Ошибка сохранения базы";
}

void MyWidget::readBase()
{
 QFile file("clientbase.bin");
 if(file.open(QFile::ReadOnly))
	{
	 QDataStream in(&file);
	 in>>clientbase;

	 for(auto iter=clientbase.begin(), end=clientbase.end();
			 iter!=end;++iter)
		plist->addItem(iter.key());

	 file.close();
	}
 else
	qCritical()<<"Error restoring clients base";
}

void MyWidget::slotStartServer()
{
 QString nPort=plePort->text();
 int pos=0;
 if(pvalidator->validate(nPort, pos)!=QValidator::Acceptable)
	{
	 pInfo->setTextColor(Qt::red);
	 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
								 +"Некорректный адрес порта.");
	 pInfo->setTextColor(Qt::black);
	 pcmdOff->setEnabled(false);
	 return;
	}

 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Сервер запущен.");

 pcmdOff->setEnabled(true);
 pcmdOn->setEnabled(false);

 if(!pserver->listen(QHostAddress::Any,
								 static_cast<quint16>(plePort->text().toInt())))
	{
	 qCritical()<<"Невозможно запустить сервер: "<<pserver->errorString();
	 pserver->close();
	 return;
	}

 plePort->setEnabled(false);


 connect(pserver, SIGNAL(newConnection()),
				 SLOT(slotNewConnection()));

 qInfo()<<QString("Сервер запущен. IP адрес: %1. Порт: %2")
					 .arg(m_address).arg(pserver->serverPort());
}

void MyWidget::slotStopServer()
{
 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Сервер остановлен.");
 pcmdOn->setEnabled(true);
 pcmdOff->setEnabled(false);

 plePort->setEnabled(true);
 pserver->close();
 qInfo()<<"Сервер остановлен.";
}

void MyWidget::slotNewConnection()
{
	QTcpSocket* pclient=pserver->nextPendingConnection();
	connect(pclient, SIGNAL(disconnected()), SLOT(slotDisconnection()));
	connect(pclient, SIGNAL(readyRead()), SLOT(slotReadClient()));
}

void MyWidget::slotDisconnection()
{
 QTcpSocket* pclient
	 =static_cast<QTcpSocket*>(sender());

 QString disconnected=*binder.find(pclient);
 binder.remove(pclient);

 QByteArray byteArray;
 QDataStream out(&byteArray, QIODevice::WriteOnly);
 out.setVersion(QDataStream::Qt_5_11);
 out<<quint16(0)<<static_cast<int>(DATATYPE::DISCONNECTION)
	 <<disconnected;
 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(byteArray.size())-sizeof(quint16));

 for(auto iter=binder.begin(), end=binder.end();
		 iter!=end;
		 ++iter)
	{
	 iter.key()->write(byteArray);
	}

 pInfo->append(QTime::currentTime().toString("[hh:mm:ss] ")
							 +disconnected+" отсоединился.");
 pclient->deleteLater();
}

void MyWidget::slotReadClient()
{
 QTcpSocket* pclient=static_cast<QTcpSocket*>(sender());
 QDataStream in(pclient);
 in.setVersion(QDataStream::Qt_5_11);

 forever
 {
	if(!m_nextBlockSize)
	 {
		if(pclient->bytesAvailable()<static_cast<qint64>(sizeof(quint16)))
		 break;

		in>>m_nextBlockSize;
	 }


	if(pclient->bytesAvailable()<m_nextBlockSize)
	 break;

	QString str;
	QTime time;

	DATATYPE type;
	in>>type;

	switch (type) {
	 case DATATYPE::REGISTRATION:
	 {
		 qDebug()<<"User registration";
		 QString nick;
		 QString fullname;
		 in>>nick>>fullname;
		 nick=nick.toLower();

		 auto pos = clientbase.find(nick);
		 QString answer="false";
		 if(pos==clientbase.end())
			{
			answer="true";
		 clientbase.insert(nick,
											 new ClientInfo(fullname,
																			pclient->localAddress().toString()
																			,pclient->localAddress().toString()
																			,true));
		 binder.insert(pclient, nick);
		 plist->addItem(nick);
		 plist->sortItems();
		 pInfo->append(QTime::currentTime().toString("[hh:mm:ss] ")
									 + "Зарегистрирован новый пользователь " + nick);
		 qInfo()<<"Зарегистрирован новый пользователь " + nick;
			}


		 sendToClient(pclient, answer);
		 break;
	 }
	 case DATATYPE::DELETION:
	 {
		 QString nick;
		 in>>nick;
		 clientbase.remove(nick);
		 //also need to remove from binder;
		 break;
	 }
	 case DATATYPE::CONNECT:
	 {
		 QString nick;
		 int clientBaseSize;
		 in>>nick>>clientBaseSize;

		 auto client=clientbase.find(nick);
		 if(client!=clientbase.end())
			{
			 client.value()->status()=true; //set to client "online"
			 //рассылка онлайн-пользователям о новом подключении
			 for(auto iter=binder.begin(), end=binder.end();
					 iter!=end;
					 ++iter)
				{
				 QByteArray byteArray;
				 QDataStream out(&byteArray, QIODevice::WriteOnly);
				 out.setVersion(QDataStream::Qt_5_11);
				 out<<quint16(0)<<static_cast<int>(DATATYPE::NOTIFYING)
					 <<nick;
				 out.device()->seek(0);
				 out<<quint16(static_cast<size_t>(byteArray.size())-sizeof(quint16));
				 iter.key()->write(byteArray);
				}

			 pInfo->append(QTime::currentTime().toString("[hh:mm:ss] ")
							 +nick+" присоединился.");
			 binder.insert(pclient, nick);
			}


		 //send to client clientsbase
		 QByteArray arrBlock;
		 QDataStream out(&arrBlock, QIODevice::WriteOnly);
		 out<<quint16(0)<<static_cast<int>(DATATYPE::CONNECT);
		 qDebug()<<"Ща отправлю клиенту базу";

		 out<<clientbase.size();
		 if(clientBaseSize!=clientbase.size())
			{
			out<<clientbase;
			qDebug()<<"размеры не сошлись, отправка всей базы";
			}

		 //тут скинем колво доступных
		 //и клиент уже будет знать сколько принимать
		 qDebug()<<"размер binder: "<<binder.size();
		 out<<binder.size();

		 for(auto nick:binder)
			{
//			 находим в базе инфу для текущего ника и отправляем
			 out<<nick;
//			 in<<nick;
//			 in<<*clientbase.find(nick);
			}
		 out.device()->seek(0);
		 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));
		 pclient->write(arrBlock);

		 break;
	 }
	 case DATATYPE::MESSAGE:
	 {
		 in>>time>>str;

		 for(auto iter=binder.begin(), end=binder.end();
				 iter!=end; ++iter)
			 if(iter.key()!=pclient)
				 sendToClient(iter.key(), str, time);

		 break;
	 }
	 default:
		qCritical()<<"Ошибка при получении типа сообщения";
		break;
	 }

	m_nextBlockSize=0;
 }
}
