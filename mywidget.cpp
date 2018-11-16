#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
 :QMainWindow(parent)
 ,pport(new QString("6175"))
 ,plblAddress(new QLabel("IP адрес:"))
 ,plblPort(new QLabel("Номер порта:"))
 ,pleAddress(new QLineEdit)
 ,plePort(new QLineEdit(*pport))
 ,pInfo(new QTextEdit)
 ,pcmdOn(new QPushButton("Запуск"))
 ,pcmdOff(new QPushButton("Остановка"))
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
 ,pvalidator(new QIntValidator(1000, 65535, plePort))
 ,pdock(new QDockWidget("Пользователи", this))
 ,plist(new QListWidget())
 ,pserver(new QTcpServer(this))
 ,logger(new MyLogger)
 ,mnextBlockSize(0)
 ,ptray(new QSystemTrayIcon)
{
 plePort->setValidator(pvalidator);

 pcmdOff->setEnabled(false);

 pInfo->setText(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
								+"Сервер не запущен.");
 pInfo->setReadOnly(true);

 foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
	 if (address.protocol() == QAbstractSocket::IPv4Protocol
			 && address != QHostAddress(QHostAddress::LocalHost))
		maddress=address.toString();
	}

 pleAddress->setInputMask("000.000.000.000");
 pleAddress->setText(maddress);
 pleAddress->setReadOnly(true);
 plePort->setInputMask("00000");

 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdOn);
 phlay->addWidget(pcmdOff);

 pvlay->addLayout(phlay);
 pvlay->addWidget(pInfo);

 connect(pcmdOn, SIGNAL(clicked()), SLOT(slotStartServer()));
 connect(pcmdOn, SIGNAL(clicked()), SLOT(slotOnOff()));
 connect(pcmdOff, SIGNAL(clicked()), SLOT(slotStopServer()));
 connect(pcmdOff, SIGNAL(clicked()), SLOT(slotOnOff()));

 connect(plePort, SIGNAL(textChanged(QString)), SLOT(slotPortChanged(QString)));

 addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, pdock);
 readBase();
 pdock->setWidget(plist);

 setMenuAndIcons();

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
	QTcpSocket* psocket
	,QVariant data
	,DATATYPE type
	,const QTime &curTime)
{
 QByteArray byteArr;
 QDataStream out(&byteArr, QIODevice::WriteOnly);
 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)
	 <<static_cast<int>(type)
	 <<curTime;

 switch(type)
	{
	case DATATYPE::REGISTRATION:
	 qDebug()<<"Returning registration result "<<data.toBool();
	 out<<data.toBool();
		break;

	default:
	 out<<data.toString();
	}

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

 QFile file("clientbase.bin");
 if(file.open(QFile::WriteOnly))
	{
	 QDataStream out(&file);
	 out<<*pport<<clientbase;
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

	 //read port value
	 QString port;
	 in>>port;
	 qDebug()<<port;
	 if(port!="")
		{
		*pport=port;
		 plePort->setText(port);
		}

	 in>>clientbase;

	 for(auto iter=clientbase.begin(), end=clientbase.end();
			 iter!=end;++iter)
		plist->addItem(iter.key());

	 file.close();
	}
 else if(QFile::exists(file.fileName()))
	qCritical()<<"Error restoring clients base";
}

void MyWidget::setMenuAndIcons()
{
 setAttribute(Qt::WA_QuitOnClose, false);

 QMenu *pmenu=new QMenu(this);
 pmenu->setTitle("Меню");

 paonOff=new QAction(QIcon(":/Res/Icons/on.png"), "Запуск");
 connect(paonOff, SIGNAL(triggered()), SLOT(slotOnOff()));
 QAction *pashow=new QAction(QIcon(":/Res/Icons/show.png"), "Показать");
 connect(pashow, SIGNAL(triggered()), SLOT(show()));
 QAction *pahide=new QAction(QIcon(":/Res/Icons/hide.png"), "Скрыть");
 connect(pahide, SIGNAL(triggered()), SLOT(hide()));
 QAction *paexit=new QAction(QIcon(":/Res/Icons/exit.png"),"Выйти");
 connect(paexit, SIGNAL(triggered()), SLOT(slotQuit()));

 pmenu->addAction(paonOff);
 pmenu->addAction(pashow);
 pmenu->addAction(pahide);
 pmenu->addAction(paexit);

 QMenuBar *pmenuBar=new QMenuBar(this);
 pmenuBar->addMenu(pmenu);

 QIcon *picon=new QIcon(":/Res/Icons/connect.png");
 ptray->show();
 ptray->setContextMenu(pmenu);

 QToolBar *ptoolbar=new QToolBar("Title");
 ptoolbar->addAction(paonOff);
 ptoolbar->addAction(pashow);
 ptoolbar->addAction(pahide);
 ptoolbar->addAction(paexit);

 ptray->setIcon(*picon);
 setMenuBar(pmenuBar);
 setWindowIcon(*picon);
 addToolBar(ptoolbar);
}

void MyWidget::slotStartServer()
{

 if(pserver->isListening())
	return;

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


 if(!pserver->listen(QHostAddress::Any,
								 static_cast<quint16>(plePort->text().toInt())))
	{
	 qCritical()<<"Невозможно запустить сервер: "<<pserver->errorString();
	 pserver->close();
	 return;
	}

 pcmdOff->setEnabled(true);
 pcmdOn->setEnabled(false);
 plePort->setEnabled(false);

 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Сервер запущен.");

 connect(pserver, SIGNAL(newConnection()),
				 SLOT(slotNewConnection()));

 ptray->showMessage("Новое событие", "Сервер запущен", QSystemTrayIcon::Information, 3000);

 qInfo()<<QString("Сервер запущен. IP адрес: %1. Порт: %2")
					 .arg(maddress).arg(pserver->serverPort());
}

void MyWidget::slotStopServer()
{
 if(!pserver->isListening())
	return;

 pcmdOn->setEnabled(true);
 pcmdOff->setEnabled(false);

 plePort->setEnabled(true);
 pserver->close();

 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Сервер остановлен.");

 ptray->showMessage("Новое событие", "Сервер остановлен", QSystemTrayIcon::Information, 3000);
 qInfo()<<"Сервер остановлен.";
}

void MyWidget::slotNewConnection()
{
	QTcpSocket* pclient=pserver->nextPendingConnection();
	connect(pclient, SIGNAL(disconnected()), SLOT(slotDisconnection()));
	connect(pclient, SIGNAL(readyRead()), SLOT(slotReadClient()));
	qDebug()<<"New connection";
}

void MyWidget::slotDisconnection()
{
 QTcpSocket* pclient
	 =static_cast<QTcpSocket*>(sender());

 QString disconnected=binder.find(pclient).value();
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
 qDebug()<<"read to client";
 QTcpSocket* pclient=static_cast<QTcpSocket*>(sender());
 QDataStream in(pclient);
 in.setVersion(QDataStream::Qt_5_11);

 forever
 {
	if(!mnextBlockSize)
	 {
		if(pclient->bytesAvailable()<static_cast<qint64>(sizeof(quint16)))
		 break;

		in>>mnextBlockSize;
	 }


	if(pclient->bytesAvailable()<mnextBlockSize)
	 break;

	QString msg;
	QTime time;

	DATATYPE type;
	in>>type>>time;

	switch (type) {
	 case DATATYPE::REGISTRATION:
	 {
		 qDebug()<<"User registration";
		 QString nick;
		 QString fullname;
		 in>>nick>>fullname;
		 nick=nick.toLower();

		 auto pos = clientbase.find(nick);
		 bool isRegistrationSuccess=pos==clientbase.end();
		 if(isRegistrationSuccess)
			{
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


		 sendToClient(pclient, QVariant(isRegistrationSuccess), DATATYPE::REGISTRATION);
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
		 qDebug()<<"Ща кину клиенту размер базу";

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
		 in>>msg;

		 for(auto iter=binder.begin(), end=binder.end();
				 iter!=end; ++iter)
			 if(iter.key()!=pclient)
				 sendToClient(iter.key(), QVariant(msg), DATATYPE::MESSAGE, time);

		 qDebug()<<"msg: "<<msg;

		 break;
	 }
	 default:
		qCritical()<<"Ошибка при получении типа сообщения";
		break;
	 }

	mnextBlockSize=0;
 }
}

void MyWidget::slotOnOff()
{
 if(paonOff->text()=="Запуск")
	{
	 paonOff->setText("Остановка");
	 paonOff->setIcon(QIcon(":/Res/Icons/off.png"));
	 if(!pserver->isListening())
		slotStartServer();
	}
 else
	{
	 paonOff->setText("Запуск");
	 paonOff->setIcon(QIcon(":/Res/Icons/on.png"));
	 if(pserver->isListening())
		slotStopServer();
	}
}

void MyWidget::slotQuit()
{
 QApplication::quit();
}

void MyWidget::slotPortChanged(QString newPort)
{
 *pport=newPort;
}
