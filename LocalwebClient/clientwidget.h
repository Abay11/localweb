#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include "../mylogger.h"
#include "../clientinfo.h"
#include "../popup.h"
#include "listdock.h"
#include "convertionwidget.h"
#include "clientservice.h"

#include <QApplication>
#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTime>
#include <QMap>
#include <QSplitter>
#include <QListWidgetItem>
#include <QListWidget>
#include <QDockWidget>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QSystemTrayIcon>

#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>


class ClientWidget : public QMainWindow
{
 Q_OBJECT

private:
 QString maddress;
 QString *pserverAddress;
 QString *pserverPort;

 QLabel *plblAddress;
 QLineEdit *pleAddress;
 QLabel *plblPort;
 QLineEdit *plePort;
 QPushButton *pcmdConnect;
 QPushButton *pcmdDisconnect;

 QHBoxLayout *phlay;
 QHBoxLayout *pconvertionLay;
 QVBoxLayout *pvlay;

 PopUp *popup;

 QSystemTrayIcon *ptray;

 ListDock *plistdock;

 ConvertionWidget *pgeneralConvertion;

 ClientService *pservice;

public:
 ClientWidget(ClientService *pservice, QWidget *parent = nullptr);
 ~ClientWidget();
 void turnStateOn();
 void turnStateOff();

 void setUI();

private slots:
 void slotConnectClicked();
 void slotQuit();
 void slotSocketError();
 void slotShowNotification(QString msg);

public slots:
 void slotAddressEdited();
 void slotPortEdited();
 void slotSetAddress(QString addr, QString port);
 void slotOpenConversion();
 void slotConnected();
 void slotDisconnected();
};

#endif // CLIENTWIDGET_H
