#include "servernetworkservice.h"

ServerNetworkService::ServerNetworkService(QObject *parent)
 :QObject(parent)
{
 ptcpServer=new QTcpServer(this);

 foreach (const QHostAddress &addr, QNetworkInterface::allAddresses()) {
	 if (addr.protocol() == QAbstractSocket::IPv4Protocol
			 && addr!= QHostAddress(QHostAddress::LocalHost))
		address=addr.toString();
	}

 pmodel=new QStringListModel(this);

 clientbase=new CLIENTBASE;
 socketsAndNicksOfOnlines=new QMap<QTcpSocket *, QString>;

 if(!restoreData()) qWarning()<<"Не удалось восстановить данные";
 setDataFromBaseToModel();

 connect(ptcpServer, SIGNAL(newConnection()),
				 SLOT(slotNewConnection()));
 connect(ptcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)),
				 SLOT(slotAcceptError(QAbstractSocket::SocketError)));
}

ServerNetworkService::~ServerNetworkService()
{
 if(!saveData()) qWarning()<<"Не удалось сохранить данные";
}

quint16 ServerNetworkService::listeningPort()
{
 return ptcpServer->serverPort();
}

void ServerNetworkService::setPort(quint16 nport)
{
 qDebug()<<"Received new port";
 this->nport=nport;
}

quint16 ServerNetworkService::getPort()
{
 qDebug()<<"getPort() called "<<nport;
 return nport;
}

QString ServerNetworkService::getAddress()
{
 return address;
}

bool ServerNetworkService::saveData(QString filename)
{
 qDebug()<<"saving data";
 QFile file(filename);
 if(file.open(QFile::WriteOnly))
	{
	 QDataStream out(&file);
	 out<<nport<<*clientbase;
	 file.close();
	 return true;
	}
	else return false;
}

bool ServerNetworkService::restoreData(QString filename)
{
 QFile file(filename);
 if(file.open(QFile::ReadOnly))
	{
	 QDataStream in(&file);
	 in>>nport>>*clientbase;
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
	case DATATYPE::REGISTRATION:
	 {
		bool registrationResult=data.toBool();
		qDebug()<<"Sending to a client registration result:"<<registrationResult;

		out<<QTime().currentTime()<<registrationResult;

		break;
	 }
	case DATATYPE::DISCONNECTION:
	 {
		qDebug()<<"sending clients about client disconnection";
		QString disconnected=data.toString();
		out<<disconnected;
		break;
	 }
	case DATATYPE::CONNECT:
	 {
		int clientBaseSize=data.toInt();
		out<<clientbase->size();
		if(clientbase->size() != clientBaseSize)
		 {
			qDebug()<<"Server sent a base with size:"<<clientbase->size();
			out<<*clientbase;
		 }

		auto onlines=socketsAndNicksOfOnlines->values();
		onlines.prepend("Общий чат");
		out<<onlines;
		break;
	 }
	case DATATYPE::NOTIFYING:
	 {
		qDebug()<<"Notifying users";
		QString nick=data.toString();
		out<<nick;
		break;
	 }
	case DATATYPE::MESSAGE:
	 {
		QString msg=data.toString();
		QTime *time=static_cast<QTime*>(paddition);
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

bool ServerNetworkService::addUserIfNickNotBusy(const QString &nick,
																								const QString &name,
																								QTcpSocket *client)
{
 QString lowerNick=nick.toLower();
 bool isNickFree=clientbase->find(lowerNick)==clientbase->end();

 if(isNickFree)
	{
	 addToBase(lowerNick, name,
						 client->localAddress().toString(),
						 QString::number(client->localPort()));
	 addToModel(nick);
	 addToOnlines(client, nick);
	}

 return isNickFree;
}

void ServerNetworkService::addToBase(const QString &nick,
																		 const QString &name,
																		 const QString &addr,
																		 const QString &port)
{
 clientbase->insert(nick,
										new ClientInfo(name, addr,
																	 port, true));
}

void ServerNetworkService::addToModel(const QString &nick)
{
 qDebug()<<"Adding to model";
 int nIndex=pmodel->rowCount();
 pmodel->insertRow(nIndex);
 pmodel->setData(pmodel->index(nIndex), nick);
}

void ServerNetworkService::addToOnlines(QTcpSocket *client, const QString &nick)
{
 socketsAndNicksOfOnlines->insert(client, nick);
}

void ServerNetworkService::notifyOthersAboutDisconnection(QString nick)
{
 for (auto i = socketsAndNicksOfOnlines->begin();
			i!=socketsAndNicksOfOnlines->end();
			++i)
	 sendToClient(i.key(), DATATYPE::DISCONNECTION, nick);
}

void ServerNetworkService::removeFromOnlines(QTcpSocket *client)
{
 socketsAndNicksOfOnlines->remove(client);
}

void ServerNetworkService::setDataFromBaseToModel()
{
 auto list = clientbase->keys();
 pmodel->setStringList(list);
}

QMap<QString, ClientInfo *> *ServerNetworkService::getClientBase() const
{
 return clientbase;
}

QStringListModel *ServerNetworkService::getModel()
{
 return pmodel;
}

QStringList ServerNetworkService::getClientsList()
{
 return pmodel->stringList();
}

bool ServerNetworkService::slotStartServer()
{
 return ptcpServer->listen(QHostAddress::Any,
								 static_cast<quint16>(nport));
}

void ServerNetworkService::slotStopServer()
{
 ptcpServer->close(); //stop only listening

 for(auto c : socketsAndNicksOfOnlines->keys()) //disconnect of all clients
	c->disconnectFromHost();

 qInfo()<<"Сервер остановлен.";
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

 QTcpSocket *client=static_cast<QTcpSocket*>(sender());
 QString disconnected=socketsAndNicksOfOnlines->value(client);
 removeFromOnlines(client);
 notifyOthersAboutDisconnection(disconnected);
 client->deleteLater();
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
	 case DATATYPE::REGISTRATION:
		{
		 qDebug()<<"A user is trying to register";

		 QString nick;
		 QString name;
		 in>>nick>>name;
		 nick=nick.toLower();

		 bool registrationResult=addUserIfNickNotBusy(nick,
														name,
														pclient);
		 sendToClient(pclient, DATATYPE::REGISTRATION, registrationResult);
		 qInfo()<<"Зарегистрирован новый пользователь " + nick;
		 break;
		}
	 case DATATYPE::MESSAGE:
	 {
		 in>>msg;
		 qDebug()<<"Received message"<<msg;
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

		 auto iter=clientbase->find(nick);
		 if(!nick.isEmpty())
			{
			 if(iter!=clientbase->end())
				{
			 //		  notifying other users
				 for(auto iter=socketsAndNicksOfOnlines->begin();
						 iter!=socketsAndNicksOfOnlines->end();
						 ++iter)
					{
					 sendToClient(iter.key(), DATATYPE::NOTIFYING, nick);
					}
				}

			 addToOnlines(pclient, nick);
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

void ServerNetworkService::slotAcceptError(
	QAbstractSocket::SocketError socketError)
{
 if(socketError == QAbstractSocket::SocketAccessError)
	qWarning()<<"A socket accepting error:"
					 <<ptcpServer->errorString();
 else {
	 qWarning()<<"Unknown error type occurred when accepted socket";
	}
}
