#include "networkhandler.h"

NetworkHandler::NetworkHandler(const QString& host, quint16 port)
 :socket(new QUdpSocket)
 ,host(host)
 ,port(port)
{
 connect(socket, SIGNAL(slotReadyRead()), SLOT(slotReadyRead()));
}

void NetworkHandler::startListen()
{
 socket->bind(QHostAddress::Any, port);
}

void NetworkHandler::slotWriteData(QByteArray &data)
{
 QByteArray compressed = qCompress(data);

 socket->writeDatagram(compressed, host, port);
}

void NetworkHandler::slotReadyRead()
{
 QByteArray incomeData = socket->readAll();

 auto decompressed = qUncompress(incomeData);

 emit readyRead(decompressed);
}
