#ifndef SERVERNETWORKSERVICE_H
#define SERVERNETWORKSERVICE_H

#include "clientinfo.h"

#include <QObject>
#include <QString>
#include <QTime>
#include <QFile>

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#define CLIENTBASE QMap<QString, ClientInfo *>

class ServerNetworkService : public QObject
{
 Q_OBJECT
private:
 quint16 nextBlockSize=0;
 quint16 port;
 QTcpServer *ptcpServer;

 CLIENTBASE *clientbase;
 QMap<QTcpSocket *, QString> *socketsAndNicksOfOnlines;

public:
 explicit ServerNetworkService(quint16 port, QObject *parent = nullptr);
 quint16 serverPort();
 quint16 expectedPort();
 bool saveData(QString filename="data.bin");
 bool restoreData(QString filename="data.bin");
 void sendToClient(QTcpSocket *to, DATATYPE type, QVariant data, void *paddition=nullptr);
 bool addToBase(const QString &nick, const QString &name,
								const QString &address, const QString &port);
 QMap<QString, ClientInfo *> *getClientBase() const;
signals:
// void receivedMessage(QString msg);
public slots:
 bool slotStartServer();
 void slotStopServer();
 void slotNewConnection();
 void slotDisconnection();
 void slotReadClient();
};

#endif // SERVERNETWORKSERVICE_H
