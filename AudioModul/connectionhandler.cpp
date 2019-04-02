#include "connectionhandler.h"

ConnectionHandler::ConnectionHandler(quint16 port, const QHostAddress& host, QObject *parent)
 :QObject (parent)
 ,socket(new QUdpSocket)
 ,host(host)
 ,port(port)
{
}

void ConnectionHandler::startListen()
{
 if(!socket->bind(QHostAddress::Any, port))
	qWarning() << "Couldn't bind a port";
}

void ConnectionHandler::slotReadDataFrom()
{

}

void ConnectionHandler::slotWriteDataTo(QByteArray& data)
{
 auto compressed = qCompress(data);

 qint64 wrote_bytes = socket->writeDatagram(compressed, host, port);

 qDebug() << "Connection handler: Received data to write(compressed size:" <<compressed.size() << "wrote: " << wrote_bytes;
}
