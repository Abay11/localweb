#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include "../clientinfo.h"
#include "../common.h"
#include "../MY_FTP/MY_FTP/ftp_client.h"
#include "../AudioModul/clientside.h"

#include <QTime>
#include <QMap>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QStringListModel>
#include <QListWidget>
#include <QFile>
#include <QFileInfo>
#include <QLabel>

#define CLIENTBASE QMap<QString, ClientInfo*>

static QString defaultServerAddress="127.0.0.1";

static QString defaultServerPort="9999";

static QString defaultSavingFile="data.bin";

static const QString DTAG = "ClientService:";

class ClientService : public QObject
{
 Q_OBJECT

protected:
 QTcpSocket *psocket;

 bool registrationResult=false;

 CLIENTBASE *clients;

 QStringListModel *ponlineModel;

 QStringListModel *pofflineModel;

 FtpClient *ftp;

 ClientSide* audioModule;

 void addAllUsersToOfflineModel();

 void addNewOnlineToModel(QString nick);

 void removeOnlinesFromOfflines(QStringList onlines);

 void removeConnectedFromOfflines(QString connected);

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

 void appendOwnerUserToOnlines();

 bool socketIsOpen();

 QStringListModel *onlineModel();

 QStringListModel *offlineModel();

 QString serverAddress();

 QString serverPort();

 quint16 clientPort();

 void setAddress(QString addr);

 void setPort(QString port);

 QString receivedMessage=nullptr;

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

 void slotSendToServer(DATATYPE, QString=nullptr, QVariant=0);

 void slotSetAddress(QString addr, QString port);

 void slotMakeCall(QString nick);

signals:
 void connected();

 void disconnected();

 void returnRegistrationResult(bool);

 void socketError(QString title, QString info);

 void newMessageForDisplay(QString msg, const QTime &time=QTime::currentTime(), QString from=nullptr);

 void newMessageToForwarding(QString msg, QString from, const QTime &time=QTime::currentTime());

 void newMessageForNotification(QString msg);

 void clientInfoUpdated();
};

#endif // CLIENTSERVICE_H
