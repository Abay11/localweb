#include "stackedwindows.h"

StackedWindows::StackedWindows(QWidget *parent)
 :QMainWindow(parent)
,pstackWidgets(new QStackedWidget(this))
{
 plogger=new MyLogger;
 pwidget=new ClientWidget(plogger, this);

 pstackWidgets->addWidget(pwidget);

 if(QFile::exists("data.bin"))
	{
	 pstackWidgets->setCurrentIndex(0);
	}
 else
	{
	 preg=new Registration(plogger, this);
	 pstackWidgets->addWidget(preg);
	 pstackWidgets->setCurrentIndex(1);
	 connect(preg, SIGNAL(registered(int)),
				 pstackWidgets,
				 SLOT(setCurrentIndex(int)));
	 connect(pstackWidgets, SIGNAL(currentChanged(int)),
					 pwidget, SLOT(slotSwitchBetweenWidgets())); //необходимо чтобы заново вычитать базу

	 connect(preg, SIGNAL(addressChanged(QString, QString)),
					 pwidget, SLOT(slotSetAddress(QString, QString)));

	 pwidget->hideSystemtray(true);
	}

 setCentralWidget(pstackWidgets);

 resize(1000, 600);
}
