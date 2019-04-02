#include "connectionhandler.h"

ConnectionHandler::ConnectionHandler(quint16 port, const QHostAddress& host, QObject *parent)
 :QObject (parent)
 ,socket(new QUdpSocket)
 ,host(host)
 ,port(port)
{
 connect(socket, SIGNAL(readyRead()), SLOT(slotReadDataFrom()));
}

void ConnectionHandler::startListen()
{
 if(!socket->bind(QHostAddress::Any, port))
	qWarning() << "Couldn't bind a port";

 if(!socket->open(QAbstractSocket::ReadWrite))
	qWarning() << "Couldn't open a port to read and write";
}

void ConnectionHandler::slotReadDataFrom()
{
 auto datagramSize = socket->pendingDatagramSize();

 QByteArray compressed;
 compressed.resize(static_cast<int>(datagramSize));

 socket->readDatagram(compressed.data(), datagramSize);

 auto data = qUncompress(compressed);

 emit dataArrived(data);
}

void ConnectionHandler::slotWriteDataTo(QByteArray& data)
{
 auto compressed = qCompress(data, 5);

 qint64 wrote_bytes = socket->writeDatagram(compressed, host, port);

 qDebug() << "Connection handler: Received data to write(compressed size:" <<compressed.size() << "wrote: " << wrote_bytes;
}
