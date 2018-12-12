#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "mylogger.h"
#include "clientinfo.h"
#include "servernetworkservice.h"

#include <QMap>
#include <QListView>
#include <QStringListModel>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QIntValidator>
#include <QDockWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QApplication>

class MyWidget : public QMainWindow
{
 Q_OBJECT
private:
 QLabel* plblAddress;
 QLabel* plblPort;
 QLineEdit* pleAddress;
 QLineEdit* plePort;
 QTextEdit* pInfo;
 QPushButton* pcmdOn;
 QPushButton* pcmdOff;
 QHBoxLayout* phlay;
 QVBoxLayout* pvlay;
 QValidator* portValidator;
 QDockWidget *pdock;
 QListView *pview;

 MyLogger* logger;
 QSystemTrayIcon *ptray;
 QAction *onOff;

 ServerNetworkService *pservice;
public:
 MyWidget(QWidget* parent=nullptr);
 ~MyWidget();

 void turnStateOn();
 void turnStateOff();
 void setModelToView(QStringListModel *pmodel);
 void setMenuAndIcons();

signals:
 void startClicked();
 void stopClicked();

private slots:
 void slotStartServer();
 void slotStopServer();
 void slotOnOffHandler();

 void slotQuit();

 void slotPortChanged(QString);
};

#endif // MYWIDGET_H
