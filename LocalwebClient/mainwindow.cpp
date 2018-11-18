#include "mainwindow.h"

void MainWindow::setupAndShowRegistrationWidget()
{
 preg=new Registration(pservice, this);
 pstackedWidgets->addWidget(preg);
 pstackedWidgets->setCurrentIndex(0);
 connect(preg, SIGNAL(registrationFinished()),
				 SLOT(slotShowClientWidget()));
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
//,plogger(new MyLogger)
,pstackedWidgets(new QStackedWidget(this))
,pservice(new ClientService(this))
{
 if(!QFile::exists("data.bin"))
	setupAndShowRegistrationWidget();
 else
	slotShowClientWidget();

 setCentralWidget(pstackedWidgets);
 resize(1000, 600);
}

void MainWindow::slotShowClientWidget()
{
 pclientWidget=new ClientWidget(pservice, this);
 pstackedWidgets->addWidget(pclientWidget);
 pstackedWidgets->setCurrentIndex(pstackedWidgets->count()-1);
}
