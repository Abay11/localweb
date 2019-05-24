#include "connectionhandler.h"

#include <QUdpSocket>
#include <QTimer>

ConnectionHandler::ConnectionHandler(quint16 server_port, const QHostAddress& host, QObject* parent)
	: QObject(parent)
	, socket(new QUdpSocket)
	, serverhost(host)
	, server_port(server_port)
{
	connect(socket, SIGNAL(readyRead()), SLOT(slotReadDataFrom()));
}

ConnectionHandler::~ConnectionHandler()
{

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
		qWarning() << DTAG << "Couldn't bind a port";
	else
		qDebug() << DTAG << "binding port" << binding_port;

	if(!socket->open(QAbstractSocket::ReadWrite))
		qWarning() << DTAG  << "Couldn't open a port to read and write";
	else
	{
		qDebug() << DTAG << "Start listen";
	}
}

void ConnectionHandler::stopListen()
{
	socket->close();

	qDebug() << DTAG << "Stop listen";
}

void ConnectionHandler::setDestination(const QHostAddress& host, quint16 port)
{
	dest_host = host;
	dest_port = port;
}

quint16 ConnectionHandler::getPort()
{
	return binding_port;
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

void ConnectionHandler::slotWriteData(const QByteArray& data)
{
	qDebug() << DTAG << "Writing data";

	auto compressed = qCompress(data, 10);
	qDebug() << DTAG << "size of writing data:" << data.size();
	qDebug() << DTAG << "size of writing coperssed:" << compressed.size();



	QString address = dest_host.toString();

	QString preheader;
	preheader += static_cast<char>(address.length());
	preheader += address;

	if(address != "BROADCAST")
	{
		QString port = QString::number(dest_port);
		char port_length = static_cast<char>(port.length());
		preheader += port_length;
		preheader += port;
	}

	compressed.insert(0, preheader);

	auto wrote = socket->writeDatagram(compressed, serverhost, server_port);

	if(wrote == -1)
	{
		qWarning() << DTAG << "error writing bytes";
	}

	qDebug() << DTAG << serverhost << server_port << "wrote bytes:" << wrote;
}
