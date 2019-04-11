#include "serverside.h"

ServerSide::ServerSide(quint16 nport, QObject *parent) : QObject(parent)
,listeningPort(nport)
,socket(new QUdpSocket)
{
 connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

void ServerSide::startListening()
{
 if(!socket->bind(QHostAddress::Any, listeningPort))
	qWarning() << DTAG << "Couldn't bind to a port!";
	else
	qDebug() << DTAG << "Server start listening!";
}

void ServerSide::stopListening()
{
 socket->close();

 qDebug() << DTAG << "Server is stopped!";
}

void ServerSide::setClients(QVector<QPair<QHostAddress, quint16> > &clients)
{
 connectedClients = clients;
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

 char dest_address[address_size];
 qstrncpy(dest_address, receivedData.data(), address_size);
 receivedData.remove(0, (address_size-1));

 uint port_size = static_cast<uint>(receivedData.at(0) + 1);
 receivedData.remove(0, 1);

 char dest_port[port_size];
 qstrncpy(dest_port, receivedData.data(), port_size);
 receivedData.remove(0, port_size-1);

 qDebug() << DTAG << "Receiving a datagram from:" << from_address << from_port;
 qDebug() << DTAG << "Dest addr: " << dest_address << "dest port:" << dest_port;
}
