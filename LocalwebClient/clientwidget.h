#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include "../mylogger.h"
#include "../clientinfo.h"
#include "../popup.h"
#include "listdock.h"
#include "convertionwidget.h"
#include "clientservice.h"
#include "callingwindow.h"

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

 ClientService *pservice;

 QMap<QString, ConvertionWidget*> *convertionWidgets;
 ConvertionWidget *generalConvertion=nullptr;
 ConvertionWidget *currentCunvertion=nullptr;

public:
 ClientWidget(ClientService *pservice, QWidget *parent = nullptr);
 ~ClientWidget();
 void turnStateOn();
 void turnStateOff();

 void setUI();

private:
 void initConvertions();

private slots:
 void slotConnectClicked();
 void slotQuit();
 void slotSocketError();
 void slotShowNotification(QString msg);
 void slotSwitchConversions(QString convertionName);
 void slotForwardToDestination(QString msg, QString from, const QTime &);

 void slotMakeCall(QString nick);

 void slotReceiveCall(QString nick);

 CallingWindow* initCallingWindow();

public slots:
 void slotAddressEdited();
 void slotPortEdited();
 void slotSetAddress(QString addr, QString port);
 void slotConnected();
 void slotDisconnected();
};

#endif // CLIENTWIDGET_H
