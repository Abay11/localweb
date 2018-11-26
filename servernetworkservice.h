#ifndef SERVERNETWORKSERVICE_H
#define SERVERNETWORKSERVICE_H

#include "clientinfo.h"

#include <QObject>
#include <QString>
#include <QTime>

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class ServerNetworkService : public QObject
{
 Q_OBJECT
private:
 quint16 nextBlockSize=0;
 quint16 port;
 QTcpServer *ptcpServer;

public:
 explicit ServerNetworkService(quint16 port, QObject *parent = nullptr);
 quint16 serverPort();

signals:
 void receivedMessage(QString msg);
public slots:
 bool slotStartServer();
 void slotStopServer();
 void slotNewConnection();
 void slotDisconnection();
 void slotReadClient();
};

#endif // SERVERNETWORKSERVICE_H
