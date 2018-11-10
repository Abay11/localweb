#include "clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
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
 ,pgeneralConvertion(new ConvertionWidget("General"))
 ,pservice(new ClientService(this))
{
// pleAddress->setText(pleAddress->text());
// pmsgField->setPlaceholderText("Введите сообщение...");
 pcmdDisconnect->setEnabled(false);

 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdConnect);
 phlay->addWidget(pcmdDisconnect);
 pvlay->addLayout(phlay);

 pconvertionLay->addWidget(pgeneralConvertion);
 pvlay->addLayout(pconvertionLay);

 connect(pcmdConnect, SIGNAL(clicked()),
				SLOT(slotConnectClicked()));
 connect(pcmdDisconnect, SIGNAL(clicked()),
				 pservice, SLOT(slotDisconnectFromServer()));
 connect(pgeneralConvertion, SIGNAL(sentClicked(QString)),
				 pservice, SLOT(slotSentToServer(QString)));

 connect(pleAddress, SIGNAL(textChanged(QString)), SLOT(slotAddressChanged(QString)));
 connect(plePort, SIGNAL(textChanged(QString)), SLOT(slotPortChanged(QString)));

 addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, plistdock);

	QWidget *pcentral=new QWidget(this);
	pcentral->setLayout(pvlay);
	setCentralWidget(pcentral);
	setUI();
	resize(640, 480);
}

void ClientWidget::slotMsgChanged()
{
// pcmdSend->setEnabled(!pmsgField->toPlainText().isEmpty()
//											&&pserverSocket->isOpen());
}

void ClientWidget::slotSwitchBetweenWidgets()
{
 qDebug()<<"reloadBase called";
 //reload base
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

ClientWidget::~ClientWidget()
{
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
 connect(pashow, SIGNAL(triggered()),
				parentWidget(), SLOT(show()));

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

void ClientWidget::hideSystemtray(bool value)
{
 value?
 ptray->hide()
	: ptray->show();
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
