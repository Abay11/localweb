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

protected:
 QTcpSocket *psocket;
 bool registrationResult=false;
 CLIENTBASE clients;

private:
 QString nick;
 QString name;
 QString *pserverAddress;
 QString *pserverPort;

 quint16 mnNextBlockSize=0;

 QStringListModel *ponlineModel;
 QStringListModel *pofflineModel;

 void saveDataAndProperties();
 void restoreDataAndProperties();
 void addAllUsersToOfflineModel();
 void addNewOnlineToModel(QString nick);
 void removeOnlinesFromOfflines(QStringList onlines);
 void throwOnlinesToOfflines();

public:
 ClientService(QWidget *parent=nullptr);
 ~ClientService();
 bool socketIsOpen();
 QStringListModel *onlineModel();
 QStringListModel *offlineModel();
 quint16 clientPort();
 QStringList getOnlines();

 QString receivedMessage;

 void setNickAndName(QString nick, QString name);
 bool isConnected();

protected slots:
 void slotConnected();
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
};

#endif // CLIENTSERVICE_H
