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

void FtpServer::uploading()
{

}

void FtpServer::downloading()
{

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

 qDebug() << "INIT SIZE" << INIT_MSG_SIZE;

 if(client->bytesAvailable() < INIT_MSG_SIZE)
	{
	 qDebug() << "Available:" << client->bytesAvailable();
	 return;
	}

 qint8 code;
 QString filename;

 in >> code;
 in >> filename;

 qDebug() << "Expected code:" << code;
 qDebug() << "Filename:" << filename;

 if(code == DOWNLOAD)
	{

	}
 else if(code == UPLOAD)
	{
	 forever
	 {
		if(!expectedSize)
		 {
			if(client->bytesAvailable() < static_cast<qint64>(sizeof(qint64)))
			 break;

			in>>expectedSize;
			qDebug() << "Expected: " << expectedSize;
		 }

		QString filename;
		in>>filename;
		qDebug()<<"Receiving filename:"<<filename;

		QFile file(filename);
		if(file.open(QFile::WriteOnly))
		 {
			qint64 receivedSize=0;
			qint64 leaveSize=0;
			QByteArray buffer;
			while(receivedSize < expectedSize)
			 {
				leaveSize = expectedSize - receivedSize;

				client->waitForReadyRead();

				buffer = client->read(BUFFER_SIZE < leaveSize ? BUFFER_SIZE : leaveSize);
				receivedSize += (buffer.size());

				qDebug()<<"Already received: "<<receivedSize;
				file.write(buffer);
			 }

			file.close();
			expectedSize=0;

			qDebug() << "File received";
			return;
		 }
	 }

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
