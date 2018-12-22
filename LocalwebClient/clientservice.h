#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include <QTime>
#include <QMap>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QStringListModel>
#include <QListWidget>
#include <QFile>
#include <QLabel>

#include "../clientinfo.h"
#include "../commonutilityfuncs.h"

#define CLIENTBASE QMap<QString, ClientInfo*>

static QString defaultServerAddress="127.0.0.1";
static QString defaultServerPort="9999";

class ClientService : public QObject
{
 Q_OBJECT
protected:
 QTcpSocket *psocket;
 bool registrationResult=false;
 CLIENTBASE clients;
 QStringListModel *ponlineModel;
 QStringListModel *pofflineModel;

 void addAllUsersToOfflineModel();
 void addNewOnlineToModel(QString nick);
 void removeOnlinesFromOfflines(QStringList onlines);
 void throwOnlinesToOfflines();
 void throwDisconnectedToOfflines(QString disconnected);

private:
 QString nick;
 QString name;
 QString *pserverAddress;
 QString *pserverPort;

 quint16 mnNextBlockSize=0;

 void saveDataAndProperties();
 void restoreDataAndProperties();

public:
 ClientService(QWidget *parent=nullptr);
 ~ClientService();
 bool socketIsOpen();
 QStringListModel *onlineModel();
 QStringListModel *offlineModel();
 QString serverAddress();
 QString serverPort();
 quint16 clientPort();
 void setAddress(QString addr);
 void setPort(QString port);

 QString receivedMessage;

 void setNickAndName(QString nick, QString name);
 bool isConnected();

protected slots:
 void slotConnected();
 void slotDisconnected();
 void slotReadyRead();
 void slotError(QAbstractSocket::SocketError);

public slots:
 void slotConnectToServer();
 void slotDisconnectFromServer();
 void slotSendToServer(DATATYPE, QString="", QVariant=0);
 void slotSetAddress(QString addr, QString port);


signals:
 void connected();
 void disconnected();
 void returnRegistrationResult(bool);
 void socketError(QString title, QString info);
 void newMessageForDisplay(QString msg, const QTime &time=QTime::currentTime());
 void newMessageForNotification(QString msg);
};

#endif // CLIENTSERVICE_H
