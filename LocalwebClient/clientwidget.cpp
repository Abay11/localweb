#include "clientwidget.h"

ClientWidget::ClientWidget(MyLogger *logger,
													 QWidget *parent)
 : QMainWindow (parent)
 ,mnNextBlockSize(0)
 ,pserverAddress(new QString)
 ,pserverPort(new QString)
 ,plblAddress(new QLabel("IP адрес сервера"))
 ,pleAddress(new QLineEdit)
 ,plblPort(new QLabel("Номер порта сервера"))
 ,plePort(new QLineEdit)
 ,pcmdConnect(new QPushButton("Подключиться"))
 ,pcmdDisconnect(new QPushButton("Отсоединиться"))
 ,pcmdSend(new QPushButton("Отправить"))
 ,pcmdClear(new QPushButton("Очистить"))
 ,pinfo(new QTextEdit)
 ,pmsgField(new QTextEdit)
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
 ,psplitter(new QSplitter(this))
 ,ponlineList(new QListWidget(this))
 ,pofflineList(new QListWidget(this))
 ,pserverSocket(new QTcpSocket)
 ,plogger(logger)
 ,ponline(new QDockWidget("Доступные", this))
 ,poffline(new QDockWidget("Недоступные", this))
 ,popup(new PopUp(this))
 ,plistdock(new ListDock(this))
{
 readBase();


// pleAddress->setText(pleAddress->text());
 pmsgField->setPlaceholderText("Введите сообщение...");
 pcmdDisconnect->setEnabled(false);
 pcmdSend->setEnabled(false);
 pinfo->setReadOnly(true);

 ponline->setWidget(ponlineList);
 poffline->setWidget(pofflineList);
 ponline->hide();
 poffline->hide();

 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdConnect);
 phlay->addWidget(pcmdDisconnect);
 pvlay->addLayout(phlay);
 pvlay->addWidget(pinfo);
 pvlay->addWidget(pmsgField);
 QHBoxLayout *ptemplayout1=new QHBoxLayout;
 ptemplayout1->addWidget(pcmdSend);
 ptemplayout1->addWidget(pcmdClear);
 pvlay->addLayout(ptemplayout1);

 ponlineList->setContextMenuPolicy(Qt::CustomContextMenu);
 connect(ponlineList, SIGNAL(customContextMenuRequested(const QPoint &)),
				 SLOT(slotSetListContextMenu(const QPoint &)));

 connect(pmsgField, SIGNAL(textChanged()), SLOT(slotMsgChanged()));
 connect(pcmdConnect, SIGNAL(clicked()),
				 SLOT(slotConnectToServer()));
 connect(pcmdDisconnect, SIGNAL(clicked()),
				 SLOT(slotDisconnectFromServer()));
 connect(pcmdSend, SIGNAL(clicked())
				 ,SLOT(slotSendToServer()));
 connect(pcmdClear, SIGNAL(clicked()),
				 SLOT(slotClearMsg()));
 connect(pserverSocket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(pserverSocket, SIGNAL(error(QAbstractSocket::SocketError))
				 ,SLOT(slotError(QAbstractSocket::SocketError)));
 connect(pserverSocket, SIGNAL(readyRead())
				 ,SLOT(slotReadyRead()));

 connect(pleAddress, SIGNAL(textChanged(QString)), SLOT(slotAddressChanged(QString)));
 connect(plePort, SIGNAL(textChanged(QString)), SLOT(slotPortChanged(QString)));

//	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, ponline);
//	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, poffline);
 addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, plistdock);

	QWidget *pcentral=new QWidget(this);
	pcentral->setLayout(pvlay);
	setCentralWidget(pcentral);
	setUI();
	resize(640, 480);
}

void ClientWidget::slotConnectToServer()
{
 pserverSocket->connectToHost(pleAddress->text(),
												static_cast<quint16>(plePort->text()
																						 .toInt()));
 pcmdDisconnect->setEnabled(true);
 pcmdConnect->setEnabled(false);
}

void ClientWidget::slotConnected()
{
 pinfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Соединение с сервером установлено.");
 qInfo()<<"Соединение с сервером установлено.";

 //запрос серверу чтобы тот прислал список клиентов
 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)<<static_cast<int>(DATATYPE::CONNECT)
	 <<usernick
	<<clients.size();
 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

 pserverSocket->write(arrBlock);
 qInfo()<<"Отправлен запрос на получение списка";

}

void ClientWidget::slotMsgChanged()
{
 pcmdSend->setEnabled(!pmsgField->toPlainText().isEmpty()
											&&pserverSocket->isOpen());
}

void ClientWidget::slotClearMsg()
{
 pinfo->clear();
}

void ClientWidget::slotSwitchBetweenWidgets()
{
 qDebug()<<"reloadBase called";
 //reload base
 readBase();
 QApplication::setQuitOnLastWindowClosed(false);
 ptray->show();

}

void ClientWidget::slotAddressChanged(QString newAddress)
{
 *pserverAddress=newAddress;
}

void ClientWidget::slotPortChanged(QString newPort)
{
 *pserverPort=newPort;
}

void ClientWidget::slotSetAddress(QString addr, QString port)
{
 pleAddress->setText(addr);
 plePort->setText(port);
 qDebug()<<"receive address info: "<<*pserverAddress<<" "<<*pserverPort;
}

void ClientWidget::slotOpenConversion()
{

}

void ClientWidget::slotDisconnectFromServer()
{
 pserverSocket->close();
 pinfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Отсоединение от сервера.");
 pcmdConnect->setEnabled(true);
 pcmdDisconnect->setEnabled(false);

 ponlineList->clear();
 pofflineList->clear();
 ponlineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/online.png"), QString("Вы: %1").arg(usernick)));

//перекидываем доступных пользователей к недоступным после отсоединения
 for(auto it=clients.begin(), end=clients.end();
		 it!=end; ++it)
	if(it.key()!=usernick)
	 pofflineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/offline.png"), it.key()));

 onlines.clear();

	qInfo()<<"Соединение с сервером разорвано.";
}

void ClientWidget::slotReadyRead()
{
 QDataStream in(pserverSocket) ;
 in.setVersion(QDataStream::Qt_5_11);
 forever
 {
	if(!mnNextBlockSize)
	 {
		if(static_cast<size_t>(pserverSocket->bytesAvailable())<sizeof(quint16))
		 break;

		in>>mnNextBlockSize;
	 }

	if(pserverSocket->bytesAvailable()<mnNextBlockSize)
	 break;

	QTime time;
	QString msg;

	DATATYPE type;
	in>>type;
	qDebug()<<"новое событие ";

	switch (type) {
	 case DATATYPE::CONNECT:
	 {
		 qDebug()<<"Receiving client's list";
		 //получение списка пользователей от сервера

		 int serverBaseSize;
		 in>>serverBaseSize;
		 //если база у сервера не совпадает с нашим,
		 //получаем актуальную базу
		 if(serverBaseSize!=clients.size())
			{
			 qDebug()<<"короче тут у нас база не сошлась, поэтому ждем новый";
			in>>clients;
			}

		 //нужно получить количество доступных
		 int count=0;
		 in>>count;

		 for(int i=0; i<count; ++i)
			{
			 QString nick;
			 in>>nick;
			 onlines.push_back(nick);
			}

		 //кидаем полученный список онлайнов в список доступных
		 for(auto i : onlines)
			 if(i!=usernick)
				ponlineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/online.png"),i));


		 //очищаем список недоступных
		 //и начинаем заного добавлять туда
		 //только не имеющихся в полученном списке-онлайн
			 pofflineList->clear();
			 for(auto iter=clients.begin(),
					 end=clients.end(); iter!=end; ++iter)
				if(usernick!=iter.key()
					 && !onlines.contains(iter.key()))
				 pofflineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/offline.png"), iter.key()));

		 break;
	 }
	 case DATATYPE::DISCONNECTION:
		{
		 qDebug()<<"кто-то отсоединился";
		 QString disconnected;
		 in>>disconnected;
		 qDebug()<<disconnected<<" отсоединился";
		 onlines.removeOne(disconnected);

//		 ponlineList->findItems(disconnected, Qt::MatchExactly).first();
		 for(auto i=0, s=ponlineList->count();
				 i<s; ++i)
			{
			 if(ponlineList->item(i)->text()==disconnected)
				{
				 delete ponlineList->takeItem(i);
				 break;
				}
			}

		 pofflineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/offline.png"),disconnected));
		 pofflineList->sortItems();

		 QString msg=disconnected;
		 msg+=" вышел";

		 if(parentWidget()->parentWidget()->isMinimized()
				|| parentWidget()->parentWidget()->isHidden())
			ptray->showMessage("Новое событие", msg, QSystemTrayIcon::Information, 3000);
		 else
			popup->showNotify(std::move(msg), mapToGlobal(pos()));

		 break;
		}
	 case DATATYPE::NOTIFYING:
		{
		 qDebug()<<"есть новый подсоединившийся";
		 QString connected;
		 in>>connected;
		 //убираем из недоступных подсоединившегося
		 for(auto i=0;
				 i<pofflineList->count(); ++i)
			{
			 if(pofflineList->item(i)->text()==connected)
				{
				delete pofflineList->takeItem(i);
				 break;
				}
			}

		 //и добавляем к доступным
		 ponlineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/online.png"), connected));
		 ponlineList->sortItems();

		 QString msg=connected;
		 msg+=" доступен";

		 if(parentWidget()->parentWidget()->isMinimized()
				|| parentWidget()->parentWidget()->isHidden())
			ptray->showMessage("Новое событие", msg, QSystemTrayIcon::Information, 3000);
		 else
			popup->showNotify(std::move(msg), mapToGlobal(pos()));


		 break;
		}
	 case DATATYPE::MESSAGE:
	 {
		 //новое сообщение
		 qDebug()<<"новое  сообщение";
		 in>>time>>msg;
		 msg.prepend(time.toString("[hh:mm:ss] "));
//		 msg=time.toString("[hh:mm:ss] ")+"Новое сообщение: "+msg;
		 pinfo->append(msg);

		 if(parentWidget()->parentWidget()->isMinimized()
				|| parentWidget()->parentWidget()->isHidden())
			ptray->showMessage("Новое событие", "Новое сообщение", QSystemTrayIcon::Information, 3000);
		 else
			popup->showNotify("***Новое сообщение***", mapToGlobal(pos()));

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

void ClientWidget::slotError(QAbstractSocket::SocketError nerr)
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
 QMessageBox::critical(this, "Ошибка подключения к серверу",
											 info);
 pcmdDisconnect->setEnabled(false);
 pcmdConnect->setEnabled(true);

	qCritical()<<"Ошибка соединения с сервером: "<<pserverSocket->errorString();
	pserverSocket->close();
}

void ClientWidget::slotSendToServer()
{
 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)<<static_cast<int>(DATATYPE::MESSAGE)<<QTime::currentTime();

 //когда общая рассылка, другой клиент не сможет узнать,
 //кто прислал мсг, т.к. получит от сервера, поэтому отправителю
 //нужно приписать, что это он.
 //Если это будет Р2Р, возможно это будет не обяз. Нужно еще раз обдумать...
 //а пока так.
 QString msg=pmsgField->toPlainText();
 msg.prepend(usernick+": ");

 out<<msg;

 //вывод у себя
 pinfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] Вы: ")
							 + pmsgField->toPlainText());
 pmsgField->clear();

 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

 pserverSocket->write(arrBlock);
}

ClientWidget::~ClientWidget()
{
 saveBase();
 delete plogger;
}

void ClientWidget::readBase()
{
 QFile file("data.bin");
 if(file.open(QFile::ReadOnly))
	{
	 QDataStream in(&file);
	 //at first read current user info
	 in>>usernick>>username;

	 //then read server address and port
	 //and update only if they not null (after registration they may be null)
	 QString addr, port;
	 in>>addr>>port;
	 if(addr!="")
		{
		 *pserverAddress=addr;
		pleAddress->setText(*pserverAddress);
		}
	 if(port!="")
		{
		 *pserverPort=port;
		 plePort->setText(port);
		}

	 //then other clients
	 in>>clients;
	 file.close();

		ponlineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/online.png"), QString("Вы: %1").arg(usernick)));

	 for(auto iter=clients.begin(), end=clients.end(); iter!=end; ++iter)
		if(usernick!=iter.key())
			pofflineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/offline.png"), iter.key()));
	}
 //сразу после регистрации не создается файл сохранения
 //поэтому, чтобы лишний раз не выбрасывать ошибку
 //сперва проверяется существует ли вообще сохранения
 //и только если оно существует, и не удалось ее прочитать
 //кидать ошибку
 else if(QFile::exists(file.fileName()))
	qWarning()<<"Error restoring clients base";
}

void ClientWidget::saveBase()
{
 QFile file("data.bin");
 if(file.open(QFile::WriteOnly))
	{
	 QDataStream out(&file);
	 //at first save current user info
	 out<<usernick<<username;

	 //then save server address and port
	 out<<*pserverAddress<<*pserverPort;
	 qDebug()<<"saving address info: "<<*pserverAddress<<" "<<*pserverPort;

	 //then if clientbase is not empty save it too
	 if(!clients.isEmpty())
		out<<clients;

	 file.close();
	}
 else
	qCritical()<<"Error saving clients base";
}

void ClientWidget::setUI()
{
 QApplication::setQuitOnLastWindowClosed(false);
 QApplication::setWindowIcon(QIcon(":/Res/Icons/client.png"));

 QAction *paconnect=new QAction("Подключиться", this);
 paconnect->setIcon(QIcon(":/Res/Icons/connect.png"));
 connect(paconnect, SIGNAL(triggered()),
				 SLOT(slotConnectToServer()));

 QAction *padisconnect=new QAction("Отключиться", this);
 padisconnect->setIcon(QIcon(":/Res/Icons/disconnect.png"));
 connect(padisconnect, SIGNAL(triggered()),
				 SLOT(slotDisconnectFromServer()));

 QAction *pashow=new QAction("Показать", this);
 pashow->setIcon(QIcon(":/Res/Icons/show.png"));
 connect(pashow, SIGNAL(triggered()),
				parentWidget(), SLOT(show()));

 QAction *pahide=new QAction("Скрыть", this);
 pahide->setIcon(QIcon(":/Res/Icons/hide.png"));
 connect(pahide, SIGNAL(triggered()),
				parentWidget(), SLOT(hide()));

 QAction *paexit=new QAction("Выход", this);
 paexit->setIcon(QIcon(":/Res/Icons/exit.png"));
 connect(paexit, SIGNAL(triggered()), SLOT(quit()));

 QToolBar *toolbar=new QToolBar(this);
 toolbar->addAction(paconnect);
 toolbar->addAction(padisconnect);
 toolbar->addAction(pashow);
 toolbar->addAction(pahide);
 toolbar->addAction(paexit);


 QMenu *pfileMenu=new QMenu("Меню", this);
 pfileMenu->addAction(paconnect);
 pfileMenu->addAction(padisconnect);
 pfileMenu->addAction(pashow);
 pfileMenu->addAction(pahide);
 pfileMenu->addAction(paexit);

 QMenu *psettingMenu=new QMenu("Настройки", this);
 psettingMenu->addAction("Настроить адреса");
 psettingMenu->addAction("Изменить информацию");

 QMenuBar *pmenuBar=new QMenuBar(this);
 pmenuBar->addMenu(pfileMenu);
 pmenuBar->addMenu(psettingMenu);
 pmenuBar->addMenu(new QMenu("Помощь", this));
 pmenuBar->addMenu(new QMenu("О приложении", this));

 ptray=new QSystemTrayIcon(this);
 ptray->setIcon(QIcon(":/Res/Icons/client.png"));
 ptray->setContextMenu(pfileMenu);
 ptray->show();

 addToolBar(toolbar);
 setMenuBar(pmenuBar);
}

void ClientWidget::hideSystemtray(bool value)
{
 value?
 ptray->hide()
	: ptray->show();
}

void ClientWidget::slotSetListContextMenu(const QPoint &pos)
{
 QPoint gpos=ponlineList->mapToGlobal(pos);

 QMenu menu;
 menu.addAction("Открыть переписку");
 menu.addAction("Аудиозвонок");
 menu.addAction("Видеозвонок");
 menu.addAction("Показать информацию");
 menu.exec(gpos);
}


void ClientWidget::quit()
{
 QApplication::quit();
}
