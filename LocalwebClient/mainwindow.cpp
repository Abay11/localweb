#include "mainwindow.h"

#include "clientwidget.h"
#include "registrationwidget.h"
//#include "../mylogger.h"
#include "clientservice.h"

#include <QWindowStateChangeEvent>
#include <QStackedWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
	//,plogger(new MyLogger)
	, pstackedWidgets(new QStackedWidget(this))
	, pservice(new ClientService(this))
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

void MainWindow::changeEvent(QEvent* e)
{
	if(e->type() == QEvent::WindowStateChange)
	{
		QWindowStateChangeEvent* se = static_cast<QWindowStateChangeEvent*>(e);

		if(!(se->oldState() & Qt::WindowMinimized) && windowState() & Qt::WindowMinimized)
			hide();
	}
}

void MainWindow::setupAndShowRegistrationWidget()
{
	preg = new Registration(pservice, this);
	pstackedWidgets->addWidget(preg);
	pstackedWidgets->setCurrentIndex(0);
	connect(preg, SIGNAL(registrationFinished()),
		SLOT(slotShowClientWidget()));
}
void MainWindow::slotShowClientWidget()
{
	pclientWidget = new ClientWidget(pservice, this);
	pstackedWidgets->addWidget(pclientWidget);
	pstackedWidgets->setCurrentIndex(pstackedWidgets->count() - 1);
}

void MainWindow::slotRestoreWindow()
{
	setWindowState(windowState() & (~Qt::WindowMinimized | Qt::WindowActive));
}

void MainWindow::slotShowSocketError(QString title, QString info)
{
	QMessageBox::critical(this, title, info);
}
