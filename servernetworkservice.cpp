#include "servernetworkservice.h"

ServerNetworkService::ServerNetworkService(quint16 port, QObject *parent)
 :QObject(parent)
{
 this->port=port;
 ptcpServer=new QTcpServer(this);

 clientbase=new QMap<QString, ClientInfo *>;
 socketsAndNicksOfOnlines=new QMap<QTcpSocket *, QString>;

 connect(ptcpServer, SIGNAL(newConnection()),
				 SLOT(slotNewConnection()));
}

quint16 ServerNetworkService::serverPort()
{
 return ptcpServer->serverPort();
}

quint16 ServerNetworkService::expectedPort()
{
 return port;
}

bool ServerNetworkService::saveData(QString filename)
{
 QFile file(filename);
 if(file.open(QFile::WriteOnly))
	{
	 QDataStream out(&file);
	 out<<port<<*clientbase;
	 file.close();

	 return true;
	}
	else
	return false;
}

bool ServerNetworkService::restoreData(QString filename)
{
 QFile file(filename);
 if(file.open(QFile::ReadOnly))
	{
	 QDataStream in(&file);
	 in>>port>>*clientbase;

	 file.close();
	 return true;
	}
 else
	return false;
}

void ServerNetworkService::sendToClient()
{

}

bool ServerNetworkService::addToBase(const QString &nick,
																		 const QString &name,
																		 const QString &addr,
																		 const QString &port)
{
 QString lowerNick=nick.toLower();
 if(clientbase->find(lowerNick)!=clientbase->end())
	return false;

 clientbase->insert(lowerNick,
										new ClientInfo(name, addr,
																	 port, true));
 return true;
}

QMap<QString, ClientInfo *> *ServerNetworkService::getClientBase() const
{
 return clientbase;
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
		 qDebug()<<"Received message";

		 in>>msg;
		 break;
	 }
	 default:
		qWarning()<<"Unknown datatype";
		break;
	 }
 }
}
