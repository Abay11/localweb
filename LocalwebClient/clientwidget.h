#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include "../mylogger.h"
#include "../clientinfo.h"

#include <QWidget>
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

#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>


class ClientWidget : public QWidget
{
 Q_OBJECT

private:
 QString maddress;
 QString mserverAddress;
 QString mserverPort;
 quint16 mnNextBlockSize;

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

public:
 ClientWidget(MyLogger *logger,
							QDockWidget *&ponline,
							QDockWidget *&poffline,
							QWidget *parent = nullptr);
 ~ClientWidget();
 void readBase();
 void saveBase();

signals:

public slots:
 void slotConnectToServer();
 void slotDisconnectFromServer();
 void slotReadyRead();
 void slotError(QAbstractSocket::SocketError);
 void slotSendToServer();
 void slotConnected();
 void slotMsgChanged();
 void slotClearMsg();
};

#endif // CLIENTWIDGET_H
