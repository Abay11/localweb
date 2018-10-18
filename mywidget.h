#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "mylogger.h"
#include "clientinfo.h"

#include <QMap>
#include <QListWidget>
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

#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>



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
 QValidator* pvalidator;
 QDockWidget *pdock;
 QListWidget *plist;

 QTcpServer* pserver;

 MyLogger* logger;

 QString m_address;
 quint16 m_nextBlockSize;


 QMap<QString, ClientInfo*> clientbase;

 //for clients identification by socket
 //also it can serve as a list of online clients
 QMap<QTcpSocket*, QString> binder;

 QSystemTrayIcon *ptray;
 QAction *paonOff;

public:
 MyWidget(QWidget* parent=nullptr);
 ~MyWidget();

 void sendToClient(QTcpSocket*
									 ,const QString&
									 ,DATATYPE type=DATATYPE::MESSAGE
									 ,const QTime &curTime=QTime::currentTime());
 void updateClientBase(ClientInfo&);
 void addNewUser(ClientInfo&);
 void removeUser(QString);
 void saveBase();
 void readBase();

 void setMenuAndIcons();

private slots:
 void slotStartServer();
 void slotStopServer();
 void slotNewConnection();
 void slotDisconnection();
 void slotReadClient();

 void slotOnOff();
 void slotQuit();
};

#endif // MYWIDGET_H
