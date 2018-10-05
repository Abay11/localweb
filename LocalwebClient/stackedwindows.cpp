#include "stackedwindows.h"

StackedWindows::StackedWindows(QWidget *parent):QMainWindow(parent)
,pstackWidgets(new QStackedWidget(this))
,ponline(new QDockWidget("Доступные", this))
,poffline(new QDockWidget("Недоступные", this))
{
 plogger=new MyLogger;
 pwidget=new ClientWidget(plogger, ponline, poffline, this);
 preg=new Registration(plogger, ponline, poffline, this);

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
	 connect(pstackWidgets, SIGNAL(currentChanged(int)),
					 pwidget, SLOT(slotReloadBase())); //необходимо чтобы заново вычитать базу
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
