#include "servernetworkservice.h"

ServerNetworkService::ServerNetworkService(quint16 port, QObject *parent)
 :QObject (parent)
{
 this->port=port;
 ptcpServer=new QTcpServer(this);
}

quint16 ServerNetworkService::serverPort()
{
 return ptcpServer->serverPort();
}

bool ServerNetworkService::slotStartServer()
{
 return ptcpServer->listen(QHostAddress::Any,
								 static_cast<quint16>(port));
}

void ServerNetworkService::slotStopServer()
{
 ptcpServer->close();
}
