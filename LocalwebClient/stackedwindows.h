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
#include "registration.h"
#include "../mylogger.h"

class StackedWindows:public QMainWindow
{
private:
 QStackedWidget *pstackWidgets;

 MyLogger *plogger;
 ClientWidget* pwidget;
 Registration* preg=nullptr;

public:
 StackedWindows(QWidget *parent=nullptr);
};

#endif // STACKEDWIDGET_H
