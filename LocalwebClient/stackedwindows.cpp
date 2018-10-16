#include "stackedwindows.h"

StackedWindows::StackedWindows(QWidget *parent):QMainWindow(parent)
,pstackWidgets(new QStackedWidget(this))
{
 plogger=new MyLogger;
 pwidget=new ClientWidget(plogger, this);
 preg=new Registration(plogger, this);

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
	}

 setCentralWidget(pstackWidgets);

 resize(1000, 600);
}
