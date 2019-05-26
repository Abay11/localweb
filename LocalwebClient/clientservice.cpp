#include "clientservice.h"

#include "../MY_FTP/MY_FTP/ftp_common.h"
#include "../MY_FTP/MY_FTP/ftp_client.h"
#include "../AudioModul/clientside.h"
#include "../common.h"

#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>

void ClientService::saveDataAndProperties()
{
	QFile file(defaultSavingFile);

	if(file.open(QFile::WriteOnly))
	{
		QDataStream out(&file);
		out << nick << name << *pserverAddress << *pserverPort << *clients;
		file.close();
	}
	else
		qCritical() << "Error saving clients base";
}

void ClientService::restoreDataAndProperties()
{
	QFile file(defaultSavingFile);

	if(file.open(QFile::ReadOnly))
	{
		QDataStream in(&file);
		in >> nick >> name >> *pserverAddress >> *pserverPort >> *clients;
		file.close();

		addAllUsersToOfflineModel();
		return;
	}
	else if(QFile::exists(file.fileName()))
		qWarning() << "Error restoring clients base";

	*pserverAddress = defaultServerAddress;
	*pserverPort = defaultServerPort;
}

void ClientService::addAllUsersToOfflineModel()
{
	QStringList offlines;
	QStringList nicksInBase = clients->keys();

	if(!nick.isEmpty())
	{
		QString usernick = "Вы: " + nick;
		offlines.prepend(usernick);
		nicksInBase.removeOne(nick);
	}

	offlines.append(nicksInBase);

	ponlineModel->setStringList(QStringList());
	pofflineModel->setStringList(offlines);
}

void ClientService::addNewOnlineToModel(QString nick)
{
	int nIndex = ponlineModel->rowCount();
	ponlineModel->insertRow(nIndex);
	ponlineModel->setData(ponlineModel->index(nIndex), nick);
}

ClientService::ClientService(QWidget* prnt)
	: QObject(prnt)
	, psocket(new QTcpSocket)
	, clients(new CLIENTBASE)
	, ponlineModel(new QStringListModel)
	, pofflineModel(new QStringListModel)
	, pserverAddress(new QString)
	, pserverPort(new QString)
{
	restoreDataAndProperties();

	ftp = new FtpClient(*pserverAddress, MY_FTP::PORT);

	audioModule = new AudioModule::ClientSide(AUDIOMODULE_IP, QHostAddress(*pserverAddress));

	connect(psocket, SIGNAL(connected()), SLOT(slotConnected()));
	connect(psocket, SIGNAL(connected()), SIGNAL(connected()));
	connect(psocket, SIGNAL(disconnected()), SLOT(slotDisconnected()));
	connect(psocket, SIGNAL(disconnected()), SIGNAL(disconnected()));
	connect(psocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
	connect(psocket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(slotError(QAbstractSocket::SocketError)));
}

ClientService::~ClientService()
{
	saveDataAndProperties();
}

void ClientService::appendOwnerUserToOnlines()
{
	//this method may be usefull only on registration
	ponlineModel->insertRow(0);
	auto firstIndex = ponlineModel->index(0);
	ponlineModel->setData(firstIndex, "Вы: " + nick);
}

bool ClientService::socketIsOpen()
{
	return psocket->isOpen();
}

QStringListModel* ClientService::onlineModel()
{
	return ponlineModel;
}

QStringListModel* ClientService::offlineModel()
{
	return pofflineModel;
}

QString ClientService::serverAddress()
{
	return *pserverAddress;
}

QString ClientService::serverPort()
{
	return *pserverPort;
}

quint16 ClientService::clientPort()
{
	return psocket->localPort();
}

void ClientService::setAddress(QString addr)
{
	*pserverAddress = addr;
}

void ClientService::setPort(QString port)
{
	*pserverPort = port;
}

void ClientService::setNickAndName(QString nick, QString name)
{
	this->nick = nick;
	this->name = name;
}

bool ClientService::isConnected()
{
	return psocket->state() == QTcpSocket::ConnectedState;
}

int ClientService::makeCall(QString nick)
{
	qDebug() << "Make a call to" << nick;

	auto iter = clients->find(nick);

	if(iter == clients->end())
	{
		qWarning() << DTAG << "Not found such nick to make a call";

		return -1;
	}

	slotSendToServer(DATATYPE::GETACTUALDATA, nick);

	QEventLoop* loop = new QEventLoop;
	//block until we get a client's info;
	connect(this, SIGNAL(clientInfoUpdated()), loop, SLOT(quit()));
	loop->exec();

	loop->deleteLater();

	qDebug() << DTAG << "Making a call to a client" << nick << "with address and port" << iter.value()->address()
		<< iter.value()->audioPort();

	QHostAddress destAddress(iter.value()->address());
	quint16 destPort = iter.value()->audioPort();

	audioModule->setDestination(destAddress, destPort);

	QEventLoop* loop2 = new QEventLoop;
	connect(this, SIGNAL(privateCallingResponseReceived()), loop2, SLOT(quit()));

	QTimer::singleShot(4000, loop2, SLOT(quit()));

	// if client won't response, break the loop with a 15ses timer
	slotSendToServer(DATATYPE::CALLINGREQUEST, nick);

	loop2->exec();

	loop2->deleteLater();

	int res = 0;

	if(callingResponseStatus == true)
	{
		//the other client accept call
		qDebug() << DTAG << "The client accept a call";

		audioModule->turnOnSpeakers();
		audioModule->turnOnMicrophone();

		res = 0;
	}
	else
	{
		qInfo() << DTAG << "Не удалось дозвониться";

		res = -1;
	}

	callingResponseStatus = false;

	return res;
}

void ClientService::slotConnected()
{
	QTime actionTime = QTime::currentTime();
	qInfo() << formatTimeToString(actionTime) << " Соединение с сервером установлено.";

	//говорим серверу что мы только что подключились и нам нужно сверить базу
	slotSendToServer(DATATYPE::CONNECT);
	emit newMessageForNotification("Соединение с сервером установлено!");
	emit newMessageForDisplay("Соединение с сервером установлено!", actionTime);

	qInfo() << actionTime << "Отправлен запрос на получение списка";
}

void ClientService::slotDisconnected()
{
	QTime actionTime = QTime::currentTime();
	qInfo() << formatTimeToString(actionTime) << " Соединение с сервером разорвано.";
	emit newMessageForNotification("Соединение с сервером разорвано!");
	emit newMessageForDisplay("Соединение с сервером разорвано!", actionTime);
}

void ClientService::slotReadyRead()
{
	qDebug() << "read server event";

	QDataStream in(psocket) ;
	in.setVersion(QDataStream::Qt_5_11);

	forever
	{
		if(!mnNextBlockSize)
		{
			if(static_cast<size_t>(psocket->bytesAvailable()) < sizeof(quint16))
				break;

			in >> mnNextBlockSize;
		}

		if(psocket->bytesAvailable() < mnNextBlockSize)
			break;

		QTime time;
		QString msg;

		DATATYPE type;
		in >> type;

		switch(type)
		{
			case DATATYPE::REGISTRATION:
			{
				in >> time >> registrationResult;

				emit returnRegistrationResult(registrationResult);

				qInfo() << time.toString("[hh:mm:ss] ")
					<< "Registration attempt. Result: " << registrationResult;
				break;
			}

			case DATATYPE::CONNECT:
			{
				qDebug() << "Receiving list of clients";

				//если база сервера не совпадает с нашим,
				//ждем актуальную базу
				int serverBaseSize;
				in >> serverBaseSize;
				bool hasClientActualBase = serverBaseSize == clients->size();

				if(!hasClientActualBase)
				{
					//receive and update base
					in >> *clients;
					qDebug() << "база не сошлась, получена новая с размером:" << clients->size();
					addAllUsersToOfflineModel();
				}
				else
					qDebug() << "база сошлась, синхрон не нужен";

				QList<QString> onlines;
				in >> onlines;
				removeOnlinesFromOfflines(onlines);
				break;
			}

			case DATATYPE::DISCONNECTION:
			{
				qDebug() << "кто-то отсоединился";
				QString disconnected;
				in >> disconnected;
				throwDisconnectedToOfflines(disconnected);

				QString msg = disconnected;
				msg += " вышел";

				emit(newMessageForNotification(msg));
				emit(newMessageForDisplay(msg));
				break;
			}

			case DATATYPE::NOTIFYING:
			{
				QString connectedUserNick;
				in >> connectedUserNick;
				qDebug() << "есть новый подсоединившийся:" << connectedUserNick;
				addNewOnlineToModel(connectedUserNick);
				removeConnectedFromOfflines(connectedUserNick);

				QString msg = connectedUserNick + " стал доступен";
				emit(newMessageForNotification(msg));
				emit(newMessageForDisplay(msg));
				break;
			}

			case DATATYPE::MESSAGE:
			{
				qDebug() << "новое сообщение";
				QString from;
				QString to;

				in >> time >> msg >> from >> to;

				receivedMessage = msg;
				emit(newMessageForNotification("***Новое сообщение***"));

				if(to == "Общий чат")
					emit(newMessageForDisplay(msg, time, from));
				else
				{
					emit newMessageToForwarding(msg, from, time);
				}

				break;
			}

			case DATATYPE::FILE:
			{
				qDebug() << "ClientService: receiving a file";
				QString from;
				QString filename;
				QString to;

				in >> time >> from >> filename >> to;

				emit(newMessageForNotification("***Прием файла***"));

				ftp->download(MY_NETWORK::DOWNLOAD_PATH, filename);

				msg = "Прием файла " + filename;

				if(to == "Общий чат")
					emit(newMessageForDisplay(msg, time, from));
				else
				{
					emit newMessageToForwarding(msg, from, time);

					qDebug() << "from:" << from << "filename:" << filename;
				}

				break;
			}

			case DATATYPE::GETACTUALDATA:
			{
				qDebug() << DTAG << "Receving updated client info";

				QString nick;
				quint16 audioPort;
				in >> nick >> audioPort;

				auto it = clients->find(nick);

				if(it == clients->end())
				{
					qWarning() << DTAG << "A client not found such nick to update data";
				}
				else if(audioPort == 0)
				{
					qWarning() << DTAG << "The server not found such nick to update data";
				}
				else
				{
					it.value()->audioPort() = audioPort;
				}

				//need emit even a client was not found to interupt a waiting event loop
				emit clientInfoUpdated();

				break;
			}

			case DATATYPE::CALLINGREQUEST:
			{
				qDebug() << DTAG << "Received calling request";

				QString from;
				in >> from;

				emit callingRequest(from);

				break;
			}

			case DATATYPE::CALLINGRESPONSE:
			{
				qDebug() << DTAG << "Received calling response";

				QString from;

				bool isAccepted;

				in >> from >> isAccepted;

				callingResponseStatus = isAccepted;

				emit privateCallingResponseReceived();

				break;
			}

			case DATATYPE::STOPCALLING:
			{
				qDebug() << DTAG << "Received STOPCALLING";

				emit receivedCallStopped();

				audioModule->turnOffSpeakers();
				audioModule->turnOffMicrophone();

				break;
			}

			default:
				//что-то пошло не так, клиент не может получить иную команду
				qCritical() << "Неизвестная ошибка при получении сообщения.";
		}

		mnNextBlockSize = 0;
	}
}

void ClientService::slotError(QAbstractSocket::SocketError nerr)
{
	QString info;

	switch(nerr)
	{
		case QAbstractSocket::HostNotFoundError:
			info.append("Удаленный сервер не найден. "
				"Удостоверьте, что введен правильный адрес и порт сервера.");
			break;

		case QAbstractSocket::RemoteHostClosedError:
			info.append("Удаленный сервер закрыл соединение. "
				"Обратитесь к администратору сети.");
			break;

		case QAbstractSocket::ConnectionRefusedError:
			info.append("Удаленный сервер отказал в доступе. "
				"Обратитесь к администратору сети.");
			break;

		default:
			info.append("Произошла критическая ошибка. "
				"Обратитесь к администратору сети.");
	}

	emit socketError("Ошибка соединения", info);
	qCritical() << "Ошибка соединения с сервером: " << psocket->errorString();
	throwOnlinesToOfflines();
	psocket->close();
}

void ClientService::slotConnectToServer()
{
	psocket->connectToHost(*pserverAddress,
		static_cast<quint16>(pserverPort->toInt()));
}

void ClientService::slotDisconnectFromServer()
{
	qDebug() << "Disconnected from server";
	throwOnlinesToOfflines();
	psocket->close();
}

void ClientService::slotSendToServer(DATATYPE type, QString msg, QVariant firstAddition)
{
	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);

	out.setVersion(QDataStream::Qt_5_11);

	QTime curTime = QTime::currentTime();
	out << quint16(0) << type << curTime;

	switch(type)
	{
		case DATATYPE::MESSAGE:
		{
			qDebug() << "Client send message";
			QString to = firstAddition.toString();
			out << msg << to;
			break;
		}

		case DATATYPE::FILE:
		{
			qDebug() << "ClientService: Sending a file";

			QString fullFileName = msg;
			QFile file(fullFileName);

			if(file.open(QFile::ReadOnly))
			{
				QString to = firstAddition.toString();

				QFileInfo fileInfo(file);

				out << to << fileInfo.fileName();
				ftp->upload(fileInfo.path() + "/", fileInfo.fileName());
			}
			else
			{
				qWarning() << "ClientService: Couldn't open a file to read";
			}

			break;
		}

		case DATATYPE::REGISTRATION:
		{
			qDebug() << "Sending to server registration request";
			QString* nick = &msg;
			QString name = firstAddition.toString();
			out << *nick << name << audioModule->getPort();
			break;
		}

		case DATATYPE::CONNECT:
		{
			qDebug() << "Sending to server size of base:" << clients->size();
			out << nick << audioModule->getPort() << clients->size();

			break;
		}

		case DATATYPE::GETACTUALDATA:
		{
			qDebug() << DTAG << "Sendting to the server a request to update client's info";

			QString& updatingClient = msg;

			out << updatingClient;

			break;
		}

		case DATATYPE::CALLINGREQUEST:
		{
			qDebug() << DTAG << "Sendting to the server the calling request";

			QString& callingUser = msg;

			out << callingUser;

			break;
		}

		case DATATYPE::CALLINGRESPONSE:
		{
			qDebug() << DTAG << "Sendting to the server the calling RESPONSE";

			QString& callingUser = msg;

			bool isCallAccepted = firstAddition.toBool();

			out << callingUser << isCallAccepted;

			break;
		}

		case DATATYPE::STOPCALLING:
		{
			qDebug() << DTAG << "Sending STOPCALLING";

			QString to = msg;

			out << to;

			break;
		}

		default:
			break;
	}

	out.device()->seek(0);
	out << quint16(static_cast<size_t>(arrBlock.size()) - sizeof(quint16));

	psocket->write(arrBlock);
}

void ClientService::slotSetAddress(QString addr, QString port)
{
	*pserverAddress = addr;
	*pserverPort = port;
}

void ClientService::slotCallAccepted(QString to)
{
	bool isCallAccepted = true;

	slotSendToServer(DATATYPE::GETACTUALDATA, to);

	QEventLoop* loop = new QEventLoop;
	//block until we get a client's info;
	connect(this, SIGNAL(clientInfoUpdated()), loop, SLOT(quit()));
	loop->exec();
	loop->deleteLater();

	auto iter = clients->find(to);

	if(iter == clients->end())
	{
		qWarning() << DTAG << "Not found such nick to accept a call";

		return;
	}

	QHostAddress destAddress(iter.value()->address());
	quint16 destPort = iter.value()->audioPort();

	audioModule->setDestination(destAddress, destPort);

	slotSendToServer(DATATYPE::CALLINGRESPONSE, to, isCallAccepted);

	audioModule->turnOnSpeakers();

	audioModule->turnOnMicrophone();
}

void ClientService::slotCallRejected(QString to)
{
	bool isCallAccepted = false;

	slotSendToServer(DATATYPE::CALLINGRESPONSE, to, isCallAccepted);
}

void ClientService::slotCallPutDowned(QString to)
{
	audioModule->turnOffSpeakers();

	audioModule->turnOffMicrophone();

	slotSendToServer(DATATYPE::STOPCALLING, to);
}

void ClientService::slotTurnMicro()
{
	qDebug() << DTAG << "turn micro";

	static bool isTurn = true;

	if(isTurn)
		audioModule->turnOffMicrophone();
	else
		audioModule->turnOnMicrophone();

	isTurn = !isTurn;
}

void ClientService::slotTurnSpeakers()
{
	qDebug() << DTAG << "turn speakers";

	static bool isTurn = true;

	if(isTurn)
		audioModule->turnOffSpeakers();
	else
		audioModule->turnOnSpeakers();

	isTurn = !isTurn;
}

void ClientService::removeOnlinesFromOfflines(QStringList onlines)
{
	QStringList offlines = pofflineModel->stringList();

	if(!nick.isEmpty())
	{
		if(!offlines.empty())
			offlines.pop_front();

		if(onlines.removeOne(nick))
			onlines.prepend("Вы: " + nick);
	}

	for(auto o : onlines)
		if(offlines.contains(o))
			offlines.removeOne(o);

	ponlineModel->setStringList(onlines);
	pofflineModel->setStringList(offlines);
}

void ClientService::removeConnectedFromOfflines(QString connected)
{
	int nrow = pofflineModel->stringList().indexOf(connected);

	if(nrow != -1)
		pofflineModel->removeRow(nrow);
}

void ClientService::throwOnlinesToOfflines()
{
	QStringList onlines = ponlineModel->stringList();
	QStringList offlines = pofflineModel->stringList();

	if(!onlines.empty())
	{
		onlines.pop_front();
		offlines.append(onlines);
		offlines.prepend("Вы: " + nick);
	}

	ponlineModel->setStringList(QStringList());
	pofflineModel->setStringList(offlines);
}

void ClientService::throwDisconnectedToOfflines(QString disconnected)
{
	int nrow = ponlineModel->stringList().indexOf(disconnected);

	if(nrow != -1)
	{
		ponlineModel->removeRow(nrow);
		int i = pofflineModel->rowCount();
		pofflineModel->insertRow(pofflineModel->rowCount());
		pofflineModel->setData(pofflineModel->index(i), disconnected);
	}
}
