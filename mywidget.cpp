#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
 :QMainWindow(parent)
 ,plblAddress(new QLabel("IP адрес:"))
 ,plblPort(new QLabel("Номер порта:"))
 ,pleAddress(new QLineEdit)
 ,plePort(new QLineEdit)
 ,pInfo(new QTextEdit)
 ,pcmdOn(new QPushButton("Запуск"))
 ,pcmdOff(new QPushButton("Остановка"))
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
 ,portValidator(new QIntValidator(1000, 65535, plePort))
 ,pdock(new QDockWidget("Пользователи", this))
 ,pview(new QListView(this))
// ,logger(new MyLogger)
 ,ptray(new QSystemTrayIcon)
 ,onOff(new QAction(this))
 ,pservice(new ServerNetworkService(this))
{

 pInfo->setText(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
								+"Сервер не запущен.");
 pInfo->setReadOnly(true);

 pleAddress->setInputMask("000.000.000.000");
 //set local host address
 pleAddress->setText(pservice->getAddress());
 pleAddress->setReadOnly(true);
 plePort->setValidator(portValidator);
 plePort->setInputMask("00000");
 plePort->setText(QString::number(pservice->getPort()));

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
 connect(pcmdOff, SIGNAL(clicked()), pservice, SLOT(slotStopServer()));
 connect(onOff, SIGNAL(triggered()), SLOT(slotOnOffHandler()));
 connect(plePort, SIGNAL(textChanged(QString)), SLOT(slotPortChanged(QString)));

 setModelToView(pservice->getModel());
 addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, pdock);
 pdock->setWidget(pview);

 setMenuAndIcons();

 setCentralWidget(new QWidget(this));
 centralWidget()->setLayout(pvlay);
 resize(800, 480);
}

MyWidget::~MyWidget()
{
 // delete logger;
}

void MyWidget::turnStateOn()
{
 onOff->setText("Остановка");
 onOff->setIcon(QIcon(":/Res/Icons/off.png"));
 pcmdOn->setEnabled(false);
 pcmdOff->setEnabled(true);
 plePort->setEnabled(false);
}

void MyWidget::turnStateOff()
{
 onOff->setText("Запуск");
 onOff->setIcon(QIcon(":/Res/Icons/on.png"));
 pcmdOn->setEnabled(true);
 pcmdOff->setEnabled(false);
 plePort->setEnabled(true);
}

void MyWidget::setModelToView(QStringListModel *pmodel)
{
 pview->setModel(pmodel);
}

void MyWidget::setMenuAndIcons()
{
 setAttribute(Qt::WA_QuitOnClose, false);

 QMenu *pmenu=new QMenu(this);
 pmenu->setTitle("Меню");

 turnStateOff();

 QAction *pashow=new QAction(QIcon(":/Res/Icons/show.png"), "Показать");
 connect(pashow, SIGNAL(triggered()), SLOT(show()));
 QAction *pahide=new QAction(QIcon(":/Res/Icons/hide.png"), "Скрыть");
 connect(pahide, SIGNAL(triggered()), SLOT(hide()));
 QAction *paexit=new QAction(QIcon(":/Res/Icons/exit.png"),"Выйти");
 connect(paexit, SIGNAL(triggered()), SLOT(slotQuit()));

 pmenu->addAction(onOff);
 pmenu->addAction(pashow);
 pmenu->addAction(pahide);
 pmenu->addAction(paexit);

 QMenuBar *pmenuBar=new QMenuBar(this);
 pmenuBar->addMenu(pmenu);

 QIcon *picon=new QIcon(":/Res/Icons/connect.png");
 ptray->show();
 ptray->setContextMenu(pmenu);

 QToolBar *ptoolbar=new QToolBar("Title");
 ptoolbar->addAction(onOff);
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
 if(!pservice->slotStartServer())
	{
	 qCritical()<<"Невозможно запустить сервер: "; //<<pserver->errorString();
	 return;
	}

 turnStateOn();

 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Сервер запущен.");

 ptray->showMessage("Новое событие", "Сервер запущен", QSystemTrayIcon::Information, 3000);

 qInfo()<<QString("Сервер запущен. IP адрес: %1. Порт: %2")
					 .arg(pservice->getAddress()).arg(pservice->getPort());
}

void MyWidget::slotStopServer()
{
 turnStateOff();

 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Сервер остановлен.");

 ptray->showMessage("Новое событие", "Сервер остановлен", QSystemTrayIcon::Information, 3000);
 qInfo()<<"Сервер остановлен.";
}

void MyWidget::slotOnOffHandler()
{
 if(onOff->text()=="Запуск")
	slotStartServer();
 else
	slotStopServer();
}

void MyWidget::slotQuit()
{
 QApplication::quit();
}

void MyWidget::slotPortChanged(QString newPort)
{
 qDebug()<<"port text changed";
 int pos;
 if(portValidator->validate(newPort, pos)==QValidator::Acceptable)
	pservice->setPort(quint16(newPort.toInt()));
}
