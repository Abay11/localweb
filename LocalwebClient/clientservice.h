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

#define CLIENTBASE QMap<QString, ClientInfo*>

class ClientService : public QObject
{
 Q_OBJECT

private:
 QString nick;
 QString name;

 bool registrationResult=false;

 quint16 mnNextBlockSize=0;
 QTcpSocket *psocket;
 QString *pserverAddress, *pserverPort;

 QStringListModel *ponlineModel, *pofflineModel;
 CLIENTBASE clients;

 void saveDataAndProperties();
 void restoreDataAndProperties();
 void addAllUsersToOfflineModel();
 void removeOnlinesFromOfflines(QStringList onlines);
 void throwOnlinesToOfflines();

public:
 ClientService(QWidget *parent=nullptr);
 ~ClientService();
 bool socketIsOpen();
 QStringListModel *onlineModel();
 QStringListModel *offlineModel();
 quint16 clientPort();
 CLIENTBASE* getClientBase();
 QStringList getOnlines();

 QString receivedMessage;


 void setNickAndName(QString nick, QString name);
 bool isConnected();

private slots:
 void slotConnected();
 void slotReadyRead();
 void slotError(QAbstractSocket::SocketError);

public slots:
 void slotConnectToServer();
 void slotDisconnectFromServer();
 void slotSentToServer(DATATYPE, QString="", QVariant=0);
 void slotSetAddress(QString addr, QString port);


signals:
 void debugPurpose();
 void connected();
 void disconnected();
 void returnRegistrationResult(bool);
 void socketError(QString title, QString info);
};

#endif // CLIENTSERVICE_H
