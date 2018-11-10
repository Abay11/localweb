#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
 :QMainWindow(parent)
,pwidgetsStack(new QStackedWidget(this))
{
// plogger=new MyLogger;
 pclientWidget=new ClientWidget(this);

 pwidgetsStack->addWidget(pclientWidget);

 if(QFile::exists("data.bin"))
	{
	 pwidgetsStack->setCurrentIndex(0);
	}
 else
	{
	 preg=new Registration(this);
	 pwidgetsStack->addWidget(preg);
	 pwidgetsStack->setCurrentIndex(1);
	 connect(preg, SIGNAL(registered(int)),
				 pwidgetsStack,
				 SLOT(setCurrentIndex(int)));
	 connect(pwidgetsStack, SIGNAL(currentChanged(int)),
					 pclientWidget, SLOT(slotSwitchBetweenWidgets())); //необходимо чтобы заново вычитать базу

	 connect(preg, SIGNAL(addressChanged(QString, QString)),
					 pclientWidget, SLOT(slotSetAddress(QString, QString)));

	 pclientWidget->hideSystemtray(true);
	}

 setCentralWidget(pwidgetsStack);

 resize(1000, 600);
}
