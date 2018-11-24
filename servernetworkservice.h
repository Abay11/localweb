#ifndef SERVERNETWORKSERVICE_H
#define SERVERNETWORKSERVICE_H

#include <QObject>
#include <QString>

#include <QTcpServer>


class ServerNetworkService : public QObject
{
private:
 quint16 port;
 QTcpServer *ptcpServer;

public:
 ServerNetworkService(quint16 port, QObject *parent=nullptr);

 quint16 serverPort();

public slots:
 bool slotStartServer();
 void slotStopServer();
// void slotNewConnection();
// void slotDisconnection();

};

#endif // SERVERNETWORKSERVICE_H
