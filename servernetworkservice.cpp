#include "servernetworkservice.h"

ServerNetworkService::ServerNetworkService(QObject *parent)
 :QObject(parent)
{
 ptcpServer=new QTcpServer(this);

 ftp = new FtpServer;

 initAudioServerModule(AUDIOMODULE_IP);

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

void ServerNetworkService::initAudioServerModule(quint16 listeningPort)
{
 audioServerModule = new ServerSide(listeningPort, this);
}

quint16 ServerNetworkService::listeningPort()
{
 return ptcpServer->serverPort();
}

void ServerNetworkService::setPort(quint16 nport)
{
 this->nport=nport;
}

quint16 ServerNetworkService::getPort()
{
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

void ServerNetworkService::sendToClient(QTcpSocket *to, DATATYPE type, QVariant data, void *paddition, QString from, QVariant addition2)
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

		out<<QTime().currentTime()<<registrationResult;

		break;
	 }
	case DATATYPE::DISCONNECTION:
	 {
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
			out<<*clientbase;
		 }

		auto onlines=socketsAndNicksOfOnlines->values();
		onlines.prepend("Общий чат");
		out<<onlines;
		break;
	 }
	case DATATYPE::NOTIFYING:
	 {
		QString nick=data.toString();
		out<<nick;
		break;
	 }
	case DATATYPE::MESSAGE:
	 {
		QString msg=data.toString();
		QTime *time=static_cast<QTime*>(paddition);
		QString to = addition2.toString();

		out<<*time<<msg<<from<<to;
		break;
	 }
	case DATATYPE::FILE:
	 {
		qDebug()<<"ServerNetworkService: Transfer file";
		QTime time=data.toTime();
		QString *to = static_cast<QString *>(paddition);
		QString filename=addition2.toString();

		out<<time<<from<<filename<<*to;
		break;
	 }

	case DATATYPE::GETACTUALDATA:
	 {
		qDebug() << DTAG << "Sending to a client an updated data";

		QString nick = data.toString();
		auto clientinfo = clientbase->find(nick);

		quint16 audioPort = 0;

		if(clientinfo == clientbase->end())
			{
			 qWarning() << DTAG << "Not found a client in the clientbase to respond to a request GETACTUALDATE";
			 //if client is not in the base, we send a value which is equal 0 what means an error
			}
		 else {
			audioPort = clientinfo.value()->audioPort();
			}

		out << nick << audioPort;

		break;
	 }

	case DATATYPE::CALLINGREQUEST:
	 {
		qDebug() << DTAG << "Sending the CALLINGREQUEST";

		QString destination = data.toString();
		out << destination;

		break;
	 }
	case DATATYPE::CALLINGRESPONSE:
	 {
		qDebug() << DTAG << "Sending the CALLING REQUEST";

		QString to = data.toString();

		bool isAccepted = *(static_cast<bool*>(paddition));

		out << to << isAccepted;

		break;
	 }

	default:
	 qWarning()<< DTAG << "Unknown datatype for sending to client";
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
						 QString::number(client->peerPort()));
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
 int nIndex=pmodel->rowCount();
 pmodel->insertRow(nIndex);
 pmodel->setData(pmodel->index(nIndex), nick);
}

void ServerNetworkService::addToOnlines(QTcpSocket *client, const QString &nick)
{
 socketsAndNicksOfOnlines->insert(client, nick);
}

void ServerNetworkService::updateClientInfo(CLIENTBASE::iterator* item, quint16 audioPort)
{
 qDebug() << DTAG << "updating clientinfo audioport: " << audioPort;
 item->value()->audioPort() = audioPort;
}

void ServerNetworkService::notifyOthersNewConnection(const QString &nick)
{
 for(auto iter=socketsAndNicksOfOnlines->begin();
		 iter!=socketsAndNicksOfOnlines->end();
		 ++iter)
	if(iter.value() != nick)
	 sendToClient(iter.key(), DATATYPE::NOTIFYING, nick);
}

void ServerNetworkService::notifyOthersAboutDisconnection(const QString &nick)
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
								 static_cast<quint16>(nport))
	 & ftp->start(MY_FTP::PORT)
	 & audioServerModule->startListening();
}

void ServerNetworkService::slotStopServer()
{
 ptcpServer->close(); //stop only listening

 for(auto c : socketsAndNicksOfOnlines->keys()) //disconnect of all clients
	c->disconnectFromHost();

 ftp->stop();

 audioServerModule->stopListening();

 qInfo()<<"Сервер остановлен.";
}

void ServerNetworkService::slotNewConnection()
{
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
 qDebug()<<"ServerNetworkService: Reading a client";

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
		 QString nick;
		 QString name;
		 quint16 audioPort;
		 in>>nick>>name >> audioPort;
		 nick=nick.toLower();

		 bool registrationResult=addUserIfNickNotBusy(nick,
														name,
														pclient);
		 sendToClient(pclient, DATATYPE::REGISTRATION, registrationResult);

		 if(registrationResult)
			{
			 qInfo()<<"Зарегистрирован новый пользователь " + nick;

			 notifyOthersNewConnection(nick);
			}

		 break;
		}
	 case DATATYPE::CONNECT:
	 {
		 QString nick;
		 int clientBaseSize;
		 quint16 audioPort;
		 in >> nick >> audioPort >> clientBaseSize;

		 qDebug() << DTAG << "connected client's audio port" << audioPort;

		 auto iter=clientbase->find(nick);
		 if(!nick.isEmpty())
			{
			 if(iter!=clientbase->end())
				{
				 notifyOthersNewConnection(nick);

				 updateClientInfo(&iter, audioPort);

				 addToOnlines(pclient, nick);
				}
			 else {
				 qWarning() << DTAG << "A connected client not found in the base";
				}

			}

		 sendToClient(pclient, DATATYPE::CONNECT, clientBaseSize);
		 break;
	 }

	 case DATATYPE::MESSAGE:
	 {
		 qDebug()<<"ServerNetworkService: Receiving a message"<<msg;

		 QString to;
		 in>>msg>>to;

			 //get sender's nick
		 QString from;
		 from=socketsAndNicksOfOnlines->find(pclient).value();

		 if(to=="Общий чат")
			{
			//a broadcast message
			 qDebug()<<"received broadcast a message";

			for(auto it=socketsAndNicksOfOnlines->begin();
					it!=socketsAndNicksOfOnlines->end();
					++it)
			 {
				if(pclient != it.key()) //пропускаем, чтобы клиент не получил сообщение от самого себя
				 sendToClient(it.key(), DATATYPE::MESSAGE, msg, &time, from, to);
			 }
			}
		 else
			{
			 qDebug()<<"received private a message";
			 //a private message
			 QTcpSocket *toSocket=nullptr;
			 for (auto it=socketsAndNicksOfOnlines->begin();
						it!=socketsAndNicksOfOnlines->end();
						++it)
				 if(it.value()==to)
					 toSocket=it.key();

			 if(toSocket)
				sendToClient(toSocket, DATATYPE::MESSAGE, msg, &time, from, to);
			 else
				qDebug()<<"User is not online";
			}
		 break;
	 }
	 case DATATYPE::FILE:
		{
		 qDebug()<<"ServerNetworkService: Receiving a file";

		 QString filename;
		 QString to;

		 in>>to>>filename;

		 //get sender's nick
		 QString from;
		 from=socketsAndNicksOfOnlines->find(pclient).value();

		 if(to=="Общий чат")
			{
			 //sending to all of online user

			for(auto it=socketsAndNicksOfOnlines->begin();
					it!=socketsAndNicksOfOnlines->end();
					++it)
			 {
				if(pclient != it.key()) //проверяем, чтобы клиент не получил сообщение от самого себя
				 sendToClient(it.key(), DATATYPE::FILE, time, &to, from, filename);
			 }
			}
		 else
			{
			 QTcpSocket *toSocket=nullptr;

			 for (auto it=socketsAndNicksOfOnlines->begin();
						it!=socketsAndNicksOfOnlines->end() && !toSocket;
						++it)
				{
				 if(it.value()==to)
					toSocket=it.key();

				 if(toSocket)
					sendToClient(toSocket, DATATYPE::FILE, time, &to, from, filename);
				 else
					qDebug()<<"ServerNetworkService: User is not online for transferring a file";
				}
			}
		 break;
		}

	 case DATATYPE::GETACTUALDATA:
		{
		 qDebug() << DTAG << "Received the GETACTUALDATA request";
		 QString nick;

		 in >> nick;

		 //checking if is the client in the base happens in the sending method
		 sendToClient(pclient, type, nick);

		 break;
		}

	 case DATATYPE::CALLINGREQUEST:
		{
		 qDebug() << DTAG << "Received the CALLINGREQUEST request";
		 QString nick;

		 in >> nick;

		 auto fromIter = socketsAndNicksOfOnlines->find(pclient);
		 if(fromIter == socketsAndNicksOfOnlines->end())
			{
			 qWarning() << DTAG << "Couldn't find in the onlines the requesting client";

			 break;
			}
		 QString from = fromIter.value();

		 QTcpSocket* to = nullptr;
		 for(auto i = socketsAndNicksOfOnlines->begin(); i != socketsAndNicksOfOnlines->end(); ++i)
			{
			 if(i.value() == nick)
				{
				 to = i.key();
				 break;
				}
			}

		 if(to == nullptr)
			{
			 qWarning() << DTAG << "Coulnd't find in the base a destination client";

			 break;
			}

		 //checking if is the client in the base happens in the sending method
		 sendToClient(to, type, from);

		 break;
		}

	 case DATATYPE::CALLINGRESPONSE:
		{
		 qDebug() << DTAG << "Received the CALLINGRESPONSE request";


		 bool isAccepted;

		 QString to;
		 in >> to >> isAccepted;

		 QTcpSocket* destination = nullptr;

		 for(auto it = socketsAndNicksOfOnlines->begin(); it != socketsAndNicksOfOnlines->end(); ++it)
			{
			 if(to == it.value())
				{
				 destination = it.key();
				 break;
				}
			}

		 auto iterFrom = socketsAndNicksOfOnlines->find(pclient);
		 if(iterFrom == socketsAndNicksOfOnlines->end())
			{
			 qWarning() << DTAG << "Something goes wrong: not found the responder in onlines";
			 return;
			}

		 if(destination)
			sendToClient(destination, type, to, &isAccepted);
		 else {
			 qWarning() << "Couldn't found a user to resend the calling response";
			}

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
