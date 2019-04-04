#include "serverside.h"

void ServerSide::startListening()
{
 if(!socket->bind(listeningPort))
	qWarning() << DTAG + "Couldn't bind to a port!";
}

void ServerSide::insertClient(const QHostAddress& address,  quint16 port)
{
 connectedClients.push_back(qMakePair(address, port));
}

void ServerSide::sendIndividual(QByteArray &data, QHostAddress& address, quint16 port)
{
	socket->writeDatagram(data, address, port);
}

void ServerSide::sendBroadcast(QByteArray& data)
{
 for(auto c : connectedClients)
	sendIndividual(data, c.first, c.second);
}

ServerSide::ServerSide(quint16 nport, QObject *parent) : QObject(parent)
,listeningPort(nport)
,socket(new QUdpSocket(this))
{
 connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

void ServerSide::slotReadyRead()
{
 auto dataSize = socket->pendingDatagramSize();

 QByteArray receivedData;
 receivedData.resize(static_cast<int>(dataSize));

 QHostAddress from_address;
 quint16 from_port;
 socket->readDatagram(receivedData.data(), dataSize, &from_address, &from_port);
}
