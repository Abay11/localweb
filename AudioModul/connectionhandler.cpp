#include "connectionhandler.h"

ConnectionHandler::ConnectionHandler(quint16 port, const QHostAddress& host, QObject *parent)
 :QObject (parent)
 ,socket(new QUdpSocket)
 ,serverhost(host)
 ,server_port(port)
{
 connect(socket, SIGNAL(readyRead()), SLOT(slotReadDataFrom()));
}

void ConnectionHandler::startListen()
{
 QTimer timeout;
 timeout.start(3000);

 bool isBound = false;
 while(!(isBound = socket->bind(QHostAddress::Any, binding_port))
			 && timeout.remainingTime() > 0)
	{
	 ++binding_port;
	}

 if(!isBound)
	qWarning() << "Couldn't bind a port";
 else
	qDebug() << "binding port" << binding_port;

 socket->state();

 if(!socket->open(QAbstractSocket::ReadWrite))
	qWarning() << "Couldn't open a port to read and write";
}

void ConnectionHandler::stopListen()
{
 socket->close();
}

void ConnectionHandler::slotReadDataFrom()
{
 qDebug() << "ConnectionHandler: Received data to read";

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

 qint64 wrote_bytes = socket->writeDatagram(compressed, serverhost, server_port);

 qDebug() << "Connection handler: Received data to write(compressed size:" <<compressed.size() << "wrote: " << wrote_bytes;
}
