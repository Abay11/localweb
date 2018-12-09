#include "servernetworkservice.h"

ServerNetworkService::ServerNetworkService(quint16 port, QObject *parent)
 :QObject(parent)
{
 this->port=port;
 ptcpServer=new QTcpServer(this);

 clientbase=new CLIENTBASE;
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

void ServerNetworkService::sendToClient(QTcpSocket *to, DATATYPE type, QVariant data, void *paddition)
{
 QByteArray byteArr;
 QDataStream out(&byteArr, QIODevice::WriteOnly);
 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)
	 <<static_cast<int>(type);

 switch(type)
	{
	case DATATYPE::CONNECT:
	 {
		int clientBaseSize=data.toInt();
		out<<clientbase->size();
		if(clientbase->size() != clientBaseSize)
		 {
			qDebug()<<"Server sent a base with size:"<<clientbase->size();
			out<<*clientbase;
		 }

		out<<socketsAndNicksOfOnlines->values();
		break;
	 }
	case DATATYPE::MESSAGE:
	 {
		QString msg=data.toString();
		QTime *time=(QTime*)paddition;
		out<<*time;
		out<<msg;
		break;
	 }
	default:
	 qWarning()<<"Unknown datatype for sending to client";
	 break;
	}

 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(byteArr.size())-sizeof (quint16));
 to->write(byteArr);
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
		 for(auto it=socketsAndNicksOfOnlines->begin();
				 it!=socketsAndNicksOfOnlines->end();
				 ++it)
			{
			 if(pclient != it.key()) //пропускаем, чтобы клиент не получил сообщение от самого себя
				sendToClient(it.key(), DATATYPE::MESSAGE, msg, &time);
			}
		 break;
	 }
	 case DATATYPE::CONNECT:
	 {
		 QString nick;
		 int clientBaseSize;
		 in>>nick>>clientBaseSize;

		 auto client=clientbase->find(nick);
		 if(!nick.isEmpty() && client!=clientbase->end())
			{
			 socketsAndNicksOfOnlines->insert(pclient, nick);
//		  notifying other users
			}

		 sendToClient(pclient, DATATYPE::CONNECT, clientBaseSize);
		 break;
	 }
	 default:
		qWarning()<<"Unknown datatype";
		break;
	 }

	nextBlockSize=0;
 }
}
