#include "clientwidget.h"

void ClientWidget::initConvertions()
{
 generalConvertion=new ConvertionWidget("Общий чат");

 convertionWidgets->insert("Общий чат", generalConvertion);

 currentCunvertion=generalConvertion;

 connect(generalConvertion, SIGNAL(sentClicked(DATATYPE, QString, QVariant)),
				 pservice, SLOT(slotSendToServer(DATATYPE, QString, QVariant)));

 connect(generalConvertion, SIGNAL(makeCallClicked(QString)),
				 this, SLOT(slotMakeCall(QString)));
}

ClientWidget::ClientWidget(ClientService *service, QWidget *parent)
 : QMainWindow (parent)
 ,pserverAddress(new QString)
 ,pserverPort(new QString)
 ,plblAddress(new QLabel("IP адрес сервера"))
 ,pleAddress(new QLineEdit)
 ,plblPort(new QLabel("Номер порта сервера"))
 ,plePort(new QLineEdit)
 ,pcmdConnect(new QPushButton("Подключиться"))
 ,pcmdDisconnect(new QPushButton("Отсоединиться"))
 ,phlay(new QHBoxLayout)
 ,pconvertionLay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
 ,popup(new PopUp(this))
 ,plistdock(new ListDock(this))
 ,pservice(service)
 ,convertionWidgets(new QMap<QString, ConvertionWidget *>)
{
 plistdock->setOnlineModel(pservice->onlineModel());
 plistdock->setOfflineModel(pservice->offlineModel());

 pleAddress->setInputMask("000.000.000.000");
 pleAddress->setText(pservice->serverAddress());
 plePort->setInputMask("00000");
 plePort->setText(pservice->serverPort());

 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdConnect);
 phlay->addWidget(pcmdDisconnect);
 pvlay->addLayout(phlay);
 initConvertions();
 pconvertionLay->addWidget(generalConvertion);
 pvlay->addLayout(pconvertionLay);

 connect(pcmdConnect, SIGNAL(clicked()),
				SLOT(slotConnectClicked()));

 connect(pcmdDisconnect, SIGNAL(clicked()),
				 pservice, SLOT(slotDisconnectFromServer()));

 connect(pservice, SIGNAL(connected()), SLOT(slotConnected()));

 connect(pservice, SIGNAL(disconnected()), SLOT(slotDisconnected()));

 connect(pservice, SIGNAL(socketError(QString, QString)), this, SLOT(slotSocketError()));

 connect(pservice, SIGNAL(newMessageToForwarding(QString, QString, const QTime &)),
				 SLOT(slotForwardToDestination(QString, QString, const QTime &)));

 connect(pservice, SIGNAL(newMessageForDisplay(QString, const QTime &, QString)),
				 generalConvertion, SLOT(slotAppendMessageToDisplay(QString, const QTime &, QString)));

 connect(pservice, SIGNAL(newMessageForNotification(QString)), this, SLOT(slotShowNotification(QString)));

 connect(pleAddress, SIGNAL(editingFinished()), SLOT(slotAddressEdited()));

 connect(plePort, SIGNAL(editingFinished()), SLOT(slotPortEdited()));

 connect(plistdock, SIGNAL(openConvertion(QString)),
				 SLOT(slotSwitchConversions(QString)));

 connect(plistdock, SIGNAL(makeCall(QString)), this, SLOT(slotMakeCall(QString)));

 addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, plistdock);

 if(pservice->isConnected())
	{
	 turnStateOn();
	}
 else
	{
	 turnStateOff();
	}

	QWidget *pcentral=new QWidget(this);
	pcentral->setLayout(pvlay);
	setCentralWidget(pcentral);
	setUI();
	resize(640, 480);
}

void ClientWidget::slotSetAddress(QString addr, QString port)
{
 pleAddress->setText(addr);
 plePort->setText(port);
 qDebug()<<"receive address info: "<<*pserverAddress<<" "<<*pserverPort;
}

void ClientWidget::slotSwitchConversions(QString convertionName)
{
 auto it=convertionWidgets->find(convertionName);
 if(it==convertionWidgets->end())
	{
	 auto insertingConvertion=new ConvertionWidget(convertionName, this);
	 it=convertionWidgets->insert(convertionName,
														 insertingConvertion);
	 connect(insertingConvertion, SIGNAL(sentClicked(DATATYPE, QString, QVariant)),
					 pservice, SLOT(slotSendToServer(DATATYPE, QString, QVariant)));

	 connect(insertingConvertion, SIGNAL(makeCallClicked(QString)), this, SLOT(makeCall(QString)));
	}
 auto oldWidget = pconvertionLay->itemAt(0)->widget();
 if(oldWidget)
	{
	 pconvertionLay->removeWidget(oldWidget);
	 oldWidget->hide();
	}
 currentCunvertion=*it;
 currentCunvertion->setSocketState(pservice->isConnected());
 currentCunvertion->show();
 pconvertionLay->addWidget(currentCunvertion);
}

void ClientWidget::slotForwardToDestination(QString msg, QString from, const QTime &time)
{
 auto it=convertionWidgets->find(from);

 if(it==convertionWidgets->end())
	{
	 it=convertionWidgets->insert(from, new ConvertionWidget(from, this));

	 connect(*it, SIGNAL(sentClicked(DATATYPE, QString, QVariant)),
					 pservice, SLOT(slotSendToServer(DATATYPE, QString, QVariant)));
	}

 (*it)->slotAppendMessageToDisplay(msg, time);
}

void ClientWidget::slotMakeCall(QString nick)
{
 static CallingWindow* callingWindow = new CallingWindow;

 callingWindow->setState(nick, CallingWindow::STATES::OUTGOING);

 callingWindow->show();

 if(pservice->makeCall(nick) != 0) // return nonzero if user not found
	{
	 qWarning() << DTAG << "Not found a user to calling. Closing a windows";

	 callingWindow->setState(nick, CallingWindow::STATES::NOTREACH);
	}
}

void ClientWidget::slotConnected()
{
 turnStateOn();
}

void ClientWidget::slotDisconnected()
{
 turnStateOff();
}

void ClientWidget::slotSocketError()
{
 turnStateOff();
}

void ClientWidget::slotShowNotification(QString msg)
{
 ptray->showMessage("Localweb", msg,
										QSystemTrayIcon::Information, 3000);
}

ClientWidget::~ClientWidget()
{
}

void ClientWidget::turnStateOn()
{
 pleAddress->setEnabled(false);
 plePort->setEnabled(false);
 pcmdConnect->setEnabled(false);
 pcmdDisconnect->setEnabled(true);

 bool isConnected=true;
 currentCunvertion->setSocketState(isConnected);
}

void ClientWidget::turnStateOff()
{
 pleAddress->setEnabled(true);
 plePort->setEnabled(true);
 pcmdConnect->setEnabled(true);
 pcmdDisconnect->setEnabled(false);

 bool isConnected=false;
 currentCunvertion->setSocketState(isConnected);
 currentCunvertion->setSentEnabled(false);
}


void ClientWidget::setUI()
{
 QApplication::setQuitOnLastWindowClosed(false);
 QApplication::setWindowIcon(QIcon(":/Res/Icons/client.png"));

 QAction *paconnect=new QAction("Подключиться", this);
 paconnect->setIcon(QIcon(":/Res/Icons/connect.png"));
 connect(paconnect, SIGNAL(triggered()),
				 pservice, SLOT(slotConnectToServer()));

 QAction *padisconnect=new QAction("Отключиться", this);
 padisconnect->setIcon(QIcon(":/Res/Icons/disconnect.png"));
 connect(padisconnect, SIGNAL(triggered()),
				 pservice, SLOT(slotDisconnectFromServer()));

 QAction *pashow=new QAction("Показать", this);
 pashow->setIcon(QIcon(":/Res/Icons/show.png"));
 auto parentWidget_=parentWidget();
	 connect(pashow, SIGNAL(triggered()),
					 parentWidget_, SLOT(show()));
	 connect(pashow, SIGNAL(triggered()),
					 parentWidget_, SLOT(slotRestoreWindow()));

 QAction *pahide=new QAction("Скрыть", this);
 pahide->setIcon(QIcon(":/Res/Icons/hide.png"));
	connect(pahide, SIGNAL(triggered()),
				parentWidget(), SLOT(hide()));

 QAction *paexit=new QAction("Выход", this);
 paexit->setIcon(QIcon(":/Res/Icons/exit.png"));
 connect(paexit, SIGNAL(triggered()), SLOT(slotQuit()));

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

void ClientWidget::slotConnectClicked()
{
 if(pleAddress->text()=="" || plePort->text()=="")
	{
	 qWarning()<<"Поле адреса или порта не могут быть пустыми!";
	 return;
	}

 pservice->slotSetAddress(pleAddress->text(), plePort->text());
 pservice->slotConnectToServer();
}

void ClientWidget::slotQuit()
{
 QApplication::quit();
}

void ClientWidget::slotAddressEdited()
{
 pservice->setAddress(pleAddress->text());
}

void ClientWidget::slotPortEdited()
{
 pservice->setPort((plePort->text()));
}
