#include "serverside.h"

ServerSide::ServerSide(quint16 nport, QObject* parent) : QObject(parent)
	, listeningPort(nport)
	, socket(new QUdpSocket)
{
	connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

bool ServerSide::startListening()
{
	bool isListening = socket->bind(QHostAddress::Any, listeningPort);

	if(!isListening)qWarning() << DTAG << "Couldn't bind to a port!";
	else
	{
		qDebug() << DTAG << "Starting the audio module server...";
	}

	return isListening;
}

void ServerSide::stopListening()
{
	socket->close();

	qDebug() << DTAG << "Server is stopped!";
}

void ServerSide::setClients(QVector<QPair<QHostAddress, quint16> >& clients)
{
	connectedClients = clients;
}

void ServerSide::sendIndividual(QByteArray& data, const QHostAddress& address, quint16 port)
{
	socket->writeDatagram(data, address, port);
}

void ServerSide::sendBroadcast(QByteArray& data)
{
	for(auto c : connectedClients)
		sendIndividual(data, c.first, c.second);
}

void ServerSide::slotReadyRead()
{

	auto dataSize = socket->pendingDatagramSize();

	QByteArray receivedData;
	receivedData.resize(static_cast<int>(dataSize));

	QHostAddress from_address;
	quint16 from_port;
	socket->readDatagram(receivedData.data(), dataSize, &from_address, &from_port);

	//extract destination info
	uint address_size = static_cast<uint>(receivedData.at(0) + 1);
	receivedData.remove(0, 1);

	char address_buffer[address_size];
	qstrncpy(address_buffer, receivedData.data(), address_size);
	receivedData.remove(0, static_cast<int>(address_size - uint(1)));

	uint port_size = static_cast<uint>(receivedData.at(0) + 1);
	receivedData.remove(0, 1);

	char port_buffer[port_size];
	qstrncpy(port_buffer, receivedData.data(), port_size);
	receivedData.remove(0, static_cast<int>(port_size - uint(1)));

	QString dest_address(address_buffer);
	quint16 dest_port = static_cast<quint16>(QString(port_buffer).toInt());

	if(dest_address == "255.255.255.255")
	{
		for(auto c : connectedClients)
		{
			sendBroadcast(receivedData);
		}
	}
	else
	{
		//send to receiver
		sendIndividual(receivedData, QHostAddress(dest_address), dest_port);
	}
}
