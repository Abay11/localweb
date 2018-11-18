#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QFile>
#include <QObject>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QStyle>
#include <QDockWidget>

#include "clientwidget.h"
#include "registrationwidget.h"
#include "../mylogger.h"
#include "clientservice.h"

class MainWindow : public QMainWindow
{
 Q_OBJECT
private:
 QStackedWidget *pstackedWidgets;

 MyLogger *plogger;
 ClientWidget* pclientWidget;
 Registration* preg;
 ClientService *pservice;

 void setupAndShowRegistrationWidget();

public:
 explicit MainWindow(QWidget *parent = nullptr);

public slots:
	void slotShowClientWidget();
};

#endif // MAINWINDOW_H
