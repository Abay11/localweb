#include "ftp_server.h"

FtpServer::FtpServer() : server(new QTcpServer)
{
 qDebug()<<"---Starting the FTP server...---";

 connect(server, SIGNAL(newConnection()), SLOT(slotNewConnection()));

 connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
				 SLOT(slotAcceptError(QAbstractSocket::SocketError)));
}

FtpServer::~FtpServer()
{
 qDebug()<<"---Stopping the FTP server...---";

 delete server;
}

bool FtpServer::start(quint16 port)
{
 this->port=port;

 isStateRunning=true;

 return server->listen(QHostAddress::Any, port);
}

void FtpServer::stop()
{
 server->close();

 isStateRunning=false;
}

void FtpServer::slotNewConnection()
{
 qDebug()<<"New connection";

 QTcpSocket *newClient = server->nextPendingConnection();
 if(!newClient) qWarning() << "FTP_SERVER: Pending connection is NULL!";

 connect(newClient, SIGNAL(readyRead()), SLOT(slotReadClient()));
}

void FtpServer::slotReadClient()
{
 qDebug()<<"---Start receiving a file---";

 QTcpSocket *client = static_cast<QTcpSocket*>(sender());
 QDataStream in(client);
 in.setVersion(QDataStream::Qt_5_11);

 forever
 {
	if(!expectedSize)
	 {
		if(client->bytesAvailable() < static_cast<qint64>(sizeof(qint64)))
			break;

		in>>expectedSize;
	 }

	while(client->bytesAvailable() < static_cast<qint64>(sizeof(QString)))
	 break;

	QString filename;
	in>>filename;

	QFile file(filename);
	if(file.open(QFile::WriteOnly))
	 {
		quint64 receivedSize=0;
		QByteArray buffer;
		while(receivedSize < expectedSize)
		 {
			in>>buffer;
			receivedSize += static_cast<quint64>(buffer.size());
			file.write(buffer);
		 }

		file.close();
		expectedSize=0;
		break;
	 }

	qDebug() << "File received";
 }
}

void FtpServer::slotAcceptError(QAbstractSocket::SocketError socketError)
{
 if(socketError == QAbstractSocket::SocketAccessError)
	qWarning()<<"A socket accepting error:"<<server->errorString();
 else {
	 qWarning()<<"Unknown error type occurred when accepted socket";
	}
}
