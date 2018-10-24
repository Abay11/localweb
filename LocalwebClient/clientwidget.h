#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include "../mylogger.h"
#include "../clientinfo.h"
#include "../popup.h"
#include "listdock.h"

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

 quint16 mnNextBlockSize;

 QString maddress;
 QString *pserverAddress;
 QString *pserverPort;

 QLabel *plblAddress;
 QLineEdit *pleAddress;
 QLabel *plblPort;
 QLineEdit *plePort;
 QPushButton *pcmdConnect;
 QPushButton *pcmdDisconnect;
 QPushButton *pcmdSend;
 QPushButton *pcmdClear;
 QTextEdit *pinfo;
 QTextEdit *pmsgField;

 QHBoxLayout *phlay;
 QVBoxLayout *pvlay;
 QSplitter *psplitter;
 QListWidget *ponlineList;
 QListWidget *pofflineList;

 QTcpSocket *pserverSocket;

 MyLogger *plogger;
 QString usernick;
 QString username;
 QMap<QString, ClientInfo*> clients;
 QVector<QString> onlines;
 QDockWidget *ponline;
 QDockWidget *poffline;

 PopUp *popup;

 QApplication *papp;

 QSystemTrayIcon *ptray;

 ListDock *plistdock;

public:
 ClientWidget(MyLogger *logger,
							QWidget *parent = nullptr);
 ~ClientWidget();
 void readBase();
 void saveBase();

 void setUI();
 void hideSystemtray(bool);

private slots:
 void slotSetListContextMenu(const QPoint &pos);
 void quit();

public slots:
 void slotConnectToServer();
 void slotDisconnectFromServer();
 void slotReadyRead();
 void slotError(QAbstractSocket::SocketError);
 void slotSendToServer();
 void slotConnected();
 void slotMsgChanged();
 void slotClearMsg();
 void slotSwitchBetweenWidgets();

 void slotAddressChanged(QString);
 void slotPortChanged(QString);

 void slotSetAddress(QString addr, QString port);

 void slotOpenConversion();
};

#endif // CLIENTWIDGET_H
