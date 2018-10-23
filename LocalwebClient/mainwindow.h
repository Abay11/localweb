#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

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

class MainWindow:public QMainWindow
{
private:
 QStackedWidget *pwidgetsStack;

 MyLogger *plogger;
 ClientWidget* pclientWidget;
 Registration* preg;

public:
 MainWindow(QWidget *parent=nullptr);
};

#endif // STACKEDWIDGET_H
