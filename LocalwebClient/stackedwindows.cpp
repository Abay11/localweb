#include "stackedwindows.h"

StackedWindows::StackedWindows(QWidget *parent):QMainWindow(parent)
,pstackWidgets(new QStackedWidget(this))
,ponline(new QDockWidget("В сети", this))
,poffline(new QDockWidget("Вне сети", this))
{
 plogger=new MyLogger;
 preg=new Registration(plogger, ponline, poffline, this);
 pwidget=new ClientWidget(plogger, ponline, poffline, this);

 pstackWidgets->addWidget(pwidget);
 pstackWidgets->addWidget(preg);

 if(QFile::exists("data.bin"))
	{
	pstackWidgets->setCurrentIndex(0);
	delete preg;
	}
 else
	{
	 pstackWidgets->setCurrentIndex(1);
	 connect(preg, SIGNAL(registered(int)),
				 pstackWidgets,
				 SLOT(setCurrentIndex(int)));
	 connect(preg->cmdExit(), SIGNAL(clicked()), SLOT(close()));
	 ponline->setVisible(false);
	 poffline->setVisible(false);
	}


 pactConnect=new QAction("Подключиться", this);
 pactConnect->setIcon(QIcon(":/Icons/connect.png"));
 connect(pactConnect, SIGNAL(triggered()),
				 pwidget, SLOT(slotConnectToServer()));

 pactDisconnect=new QAction("Отключиться", this);
 pactDisconnect->setIcon(QIcon(":/Icons/disconnect.ico"));
 connect(pactDisconnect, SIGNAL(triggered()),
				 pwidget, SLOT(slotDisconnectFromServer()));

 QToolBar *toolbar=new QToolBar(this);
 toolbar->addAction(pactConnect);
 toolbar->addAction(pactDisconnect);

 addToolBar(toolbar);

 QMenu *pfileMenu=new QMenu("Меню", this);
 pfileMenu->addAction(pactConnect);
 pfileMenu->addAction(pactDisconnect);

 pactHide=new QAction("Скрыть", this);
 pactHide->setIcon(QIcon(":/Icons/hide.png"));
 connect(pactHide, SIGNAL(triggered()), SLOT(hide()));
 pfileMenu->addAction(pactHide);

 pfileMenu->addAction("Выход", this, SLOT(close()));
 menuBar()->addMenu(pfileMenu);

 QMenu *psetMenu=new QMenu("Настройки", this);
 psetMenu->addAction("Настроить адреса");
 psetMenu->addAction("Изменить информацию");
 menuBar()->addMenu(psetMenu);

 menuBar()->addMenu(new QMenu("Помощь", this));
 menuBar()->addMenu(new QMenu("О приложении", this));

 addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, ponline);
 addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, poffline);
 setCentralWidget(pstackWidgets);

 resize(1000, 600);
}
