#include "stackedwindows.h"

StackedWindows::StackedWindows(QWidget *parent):QMainWindow(parent)
,pstackWidgets(new QStackedWidget(this))
{
 plogger=new MyLogger;
 preg=new Registration(plogger, this);
 pwidget=new ClientWidget(plogger, this);

 pstackWidgets->addWidget(pwidget);
 pstackWidgets->addWidget(preg);

 if(QFile::exists("date.bin"))
	{
	pstackWidgets->setCurrentIndex(0);
	delete preg;
	}
 else
	{
	 pstackWidgets->setCurrentIndex(1);
	 connect(preg, SIGNAL(registered(int)),
				 this,
				 SLOT(setCurrentIndex(int)));
	 connect(preg->cmdExit(), SIGNAL(clicked()), SLOT(close()));
	}

 QMenu *pfileMenu=new QMenu("Файл", this);
 pfileMenu->addAction("Скрыть", this, SLOT(hide()));
 pfileMenu->addAction("Выход", this, SLOT(close()));
 menuBar()->addMenu(pfileMenu);

 QMenu *psetMenu=new QMenu("Настройки", this);
 psetMenu->addAction("Настроить адреса");
 psetMenu->addAction("Изменить информацию");
 menuBar()->addMenu(psetMenu);

 menuBar()->addMenu(new QMenu("Помощь", this));
 menuBar()->addMenu(new QMenu("О приложении", this));

 setCentralWidget(pstackWidgets);
}
