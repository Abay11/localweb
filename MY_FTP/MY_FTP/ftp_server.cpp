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

bool FtpServer::uploading(QTcpSocket *client, const QString &filename)
{
		if(!expectedSize)
		 {
			if(client->bytesAvailable() < static_cast<qint64>(sizeof(qint64))) return false;

			client->read(reinterpret_cast<char *>(&expectedSize), sizeof(qint64));
			qDebug() << "Expected: " << expectedSize;
		 }

		QFile file(filename);
		if(file.open(QFile::WriteOnly))
		 {
			qint64 receivedSize=0;
			qint64 leaveSize=0;
			QByteArray buffer;
			while(receivedSize < expectedSize)
			 {
				leaveSize = expectedSize - receivedSize;

				if(client->bytesAvailable() < 1)
				 client->waitForReadyRead();

				buffer = client->read(BUFFER_SIZE < leaveSize ? BUFFER_SIZE : leaveSize);
				receivedSize += (buffer.size());

				qDebug()<<"Already received: "<<receivedSize;
				file.write(buffer);
			 }

			file.close();
			expectedSize=0;

			qDebug() << "File received";
			return true;
		 }
		else
		 {
			qWarning() << "Error: couldn't open a file to write";
			return true;
		 }
}

void FtpServer::downloading(QTcpSocket *client, const QString &filename)
{
 QFile file(filename);

 if(file.open(QFile::ReadOnly))
	{
	 client->write(reinterpret_cast<const char *>(&OK), sizeof (qint8));

	 qDebug() << "Start sending a file";

	 QFileInfo finfo(file);
	 expectedSize=finfo.size();

	 client->write(reinterpret_cast<char *>(&expectedSize), sizeof(qint64));
	 qDebug() << "Sending file size to client:" << expectedSize;

	 qint64 sentBytes = 0;
	 QByteArray buffer;
	 while(sentBytes < expectedSize)
		{
		 buffer = file.read(800000);
		 qDebug() << "Wrote bytes" << client->write(buffer);
		 qDebug() << "Is flushed" << client->flush();
		 sentBytes += buffer.size();

		 qDebug()<<"Already sent bytes:"<<sentBytes;
		}

	 file.close();

	 expectedSize = 0;

	 qDebug() << "The download of file is finished";
	}
 else
	{
	 client->write(reinterpret_cast<const char *>(&FILE_NOT_FOUND), sizeof (qint8));
	 qWarning() << "A file not found";
	}
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

 if(client->bytesAvailable() < INIT_MSG_SIZE)
	 return;

 qint8 code;
 QString filename;

 in >> code;
 in >> filename;

 if(code == DOWNLOAD)
	{
	 qDebug() << "DOWNLOADING MODE";
	 downloading(client, filename);
	}
 else if(code == UPLOAD)
	{
	 qDebug() << "UPLOADING MODE";
	 while(!uploading(client, filename)){client->waitForReadyRead();}
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
