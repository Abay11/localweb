#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
//,plogger(new MyLogger)
,pstackedWidgets(new QStackedWidget(this))
,pservice(new ClientService(this))
{
 if(!QFile::exists("data.bin")) //user isn't registrated
	setupAndShowRegistrationWidget();
 else
	slotShowClientWidget();

 connect(pservice, SIGNAL(socketError(QString, QString)),
				 SLOT(slotShowSocketError(QString, QString)));

 setCentralWidget(pstackedWidgets);
 resize(1000, 600);
}

void MainWindow::setupAndShowRegistrationWidget()
{
 preg=new Registration(pservice, this);
 pstackedWidgets->addWidget(preg);
 pstackedWidgets->setCurrentIndex(0);
 connect(preg, SIGNAL(registrationFinished()),
				 SLOT(slotShowClientWidget()));
}
void MainWindow::slotShowClientWidget()
{
 pclientWidget=new ClientWidget(pservice, this);
 pstackedWidgets->addWidget(pclientWidget);
 pstackedWidgets->setCurrentIndex(pstackedWidgets->count()-1);
}
void MainWindow::slotShowSocketError(QString title, QString info)
{
 QMessageBox::critical(this, title, info);
}
