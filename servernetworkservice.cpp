#include "servernetworkservice.h"

ServerNetworkService::ServerNetworkService(quint16 port, QObject *parent)
 :QObject(parent)
{
 this->port=port;
 ptcpServer=new QTcpServer(this);

 connect(ptcpServer, SIGNAL(newConnection()),
				 SLOT(slotNewConnection()));
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

void ServerNetworkService::slotNewConnection()
{
 qDebug()<<"A new pending connection";

 QTcpSocket *pclient=ptcpServer->nextPendingConnection();
 connect(pclient, SIGNAL(disconnected()), SLOT(slotDisconnection()));
 connect(pclient, SIGNAL(readyRead()), SLOT(slotReadClient()));
}

void ServerNetworkService::slotDisconnection()
{
 qDebug()<<"A client disconnected";

 QTcpSocket* pclient=static_cast<QTcpSocket*>(sender());
 delete pclient;
}

void ServerNetworkService::slotReadClient()
{
 qDebug()<<"read client";

 QTcpSocket* pclient=static_cast<QTcpSocket*>(sender());
 QDataStream in(pclient);
 in.setVersion(QDataStream::Qt_5_11);

 forever
 {
	if(!nextBlockSize)
	 {
		if(pclient->bytesAvailable()<static_cast<qint64>(sizeof(quint16)))
		 break;

		in>>nextBlockSize;
	 }

	if(pclient->bytesAvailable()<nextBlockSize)
	 break;

	QString msg;
	QTime time;
	DATATYPE type;
	in>>type>>time;

	switch(type)
	 {
	 case DATATYPE::MESSAGE:
	 {
		 in>>msg;

//		 emit receivedMessage(msg);
		 break;
	 }
	 default:
		qWarning()<<"Unknown datatype type";
		break;
	 }
 }
}
