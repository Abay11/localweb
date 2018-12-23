#include "clientwidget.h"

void ClientWidget::initConvertions()
{
 pgeneralConvertion=new ConvertionWidget("Общий чат");
 convertionWidgets->insert("Общий чат", pgeneralConvertion);
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
 ,convertionWidgets(new QMap<QString, QWidget *>)
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
 pconvertionLay->addWidget(pgeneralConvertion);
 pvlay->addLayout(pconvertionLay);

 connect(pcmdConnect, SIGNAL(clicked()),
				SLOT(slotConnectClicked()));
 connect(pcmdDisconnect, SIGNAL(clicked()),
				 pservice, SLOT(slotDisconnectFromServer()));
 connect(pgeneralConvertion, SIGNAL(sentClicked(DATATYPE, QString)),
				 pservice, SLOT(slotSendToServer(DATATYPE, QString)));
 connect(pservice, SIGNAL(connected()), SLOT(slotConnected()));
 connect(pservice, SIGNAL(disconnected()), SLOT(slotDisconnected()));
 connect(pservice, SIGNAL(socketError(QString, QString)), this, SLOT(slotSocketError()));
 connect(pservice, SIGNAL(newMessageForDisplay(QString, const QTime &)), pgeneralConvertion, SLOT(slotAppendMessageToDisplay(QString, const QTime &)));
 connect(pservice, SIGNAL(newMessageForNotification(QString)), this, SLOT(slotShowNotification(QString)));
 connect(pleAddress, SIGNAL(editingFinished()), SLOT(slotAddressEdited()));
 connect(plePort, SIGNAL(editingFinished()), SLOT(slotPortEdited()));

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

void ClientWidget::slotOpenConversion()
{

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
 pgeneralConvertion->setSocketState(isConnected);
}

void ClientWidget::turnStateOff()
{
 pleAddress->setEnabled(true);
 plePort->setEnabled(true);
 pcmdConnect->setEnabled(true);
 pcmdDisconnect->setEnabled(false);

 bool isConnected=false;
 pgeneralConvertion->setSocketState(isConnected);
 pgeneralConvertion->setSentEnabled(false);
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
