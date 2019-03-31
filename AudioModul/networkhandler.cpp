#include "networkhandler.h"

NetworkHandler::NetworkHandler(const QString& host, quint16 port)
 :socket(new QUdpSocket)
 ,host(host)
 ,port(port)
{
 connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

void NetworkHandler::startListen()
{
 socket->bind(QHostAddress::Any, port);

 if(!socket->open(QIODevice::ReadWrite))
	qDebug() << "SOCKET IS NOT OPEN";
}

void NetworkHandler::stopListen()
{
 socket->close();
}

void NetworkHandler::slotWriteData(QByteArray data)
{
 qDebug() << "Socket: Data to write with size:" << data.size();
// QByteArray compressed = qCompress(data);

// socket->writeDatagram(compressed, host, port);
 socket->writeDatagram(data, host, port);
}

void NetworkHandler::slotReadyRead()
{

 QByteArray incomeData = socket->readAll();

 qDebug() << "Socket: Data to read with size:" << incomeData.size();

// auto decompressed = qUncompress(incomeData);

 emit readyRead(incomeData);
}
