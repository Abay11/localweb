#ifndef SERVERNETWORKSERVICE_H
#define SERVERNETWORKSERVICE_H

#include "clientinfo.h"

#include <QObject>
#include <QString>
#include <QStringListModel>
#include <QTime>
#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkInterface>

#ifndef CLIENTBASE
#define CLIENTBASE QMap<QString, ClientInfo *>
#endif

class ServerNetworkService : public QObject
{
 Q_OBJECT
private:
 quint16 nextBlockSize=0;
 quint16 nport;
 QString address;
 QTcpServer *ptcpServer;
 QStringListModel *pmodel;

 CLIENTBASE *clientbase;
 QMap<QTcpSocket *, QString> *socketsAndNicksOfOnlines;

public:
 explicit ServerNetworkService(QObject *parent = nullptr);
 ~ServerNetworkService();
 quint16 listeningPort();
 void setPort(quint16 nport);
 quint16 getPort();
 QString getAddress();
 bool saveData(QString filename="serverdata.bin");
 bool restoreData(QString filename="serverdata.bin");
 void sendToClient(QTcpSocket *to, DATATYPE type, QVariant data, void *paddition=nullptr, QString from=nullptr, QVariant addition2=0);
 bool addUserIfNickNotBusy(const QString &nick, const QString &name, QTcpSocket *client);
 void addToBase(const QString &nick, const QString &name,
								const QString &address, const QString &nport);
 void addToModel(const QString &nick);
 void addToOnlines(QTcpSocket *client, const QString &nick);
 void notifyOthersAboutDisconnection(QString nick);
 void removeFromOnlines(QTcpSocket *client);
 void setDataFromBaseToModel();
 CLIENTBASE *getClientBase() const;
 QStringListModel *getModel();
 QStringList getClientsList();

signals:

public slots:
 bool slotStartServer();
 void slotStopServer();
 void slotNewConnection();
 void slotDisconnection();
 void slotReadClient();

 void slotAcceptError(QAbstractSocket::SocketError socketError);
};

#endif // SERVERNETWORKSERVICE_H
