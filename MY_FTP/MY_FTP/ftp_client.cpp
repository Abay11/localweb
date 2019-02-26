#include "ftp_client.h"

FtpClient::FtpClient(const QString &address, quint16 port)
 :serverAddress(address)
 ,port(port)
 ,socket(new QTcpSocket)
{
 connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
				 this, SLOT(slotError(QAbstractSocket::SocketError)));
}

FtpClient::~FtpClient()
{
 delete socket;
}

void FtpClient::connectToServer()
{
 qDebug()<<"---Connecting to server...---";
 socket->connectToHost(serverAddress, port);
 socket->waitForConnected(10000);
}

void FtpClient::disconnect()
{
 socket->close();
}

void FtpClient::upload(const QString &path, const QString &filename)
{
 isFinished=false;

 QFile file(path + filename);

 if(file.open(QFile::ReadOnly))
	{
	 if(!sendRequest(UPLOAD, filename))
		{
		 qWarning() << "An error has occurred: a request didn't send";
		 return;
		}

	 qDebug()<<"---Uploading a file...---";

	 QFileInfo finfo(file);

	 expectedSize=finfo.size();

	 socket->write(reinterpret_cast<char *>(&expectedSize), sizeof(qint64));
	 qDebug() << "upload:: Expected" << expectedSize;

	 QByteArray buffer;

	 qint64 sentBytes = 0;
	 while(sentBytes < expectedSize)
		{
		 buffer = file.read(800000);
		 socket->write(buffer);
		 socket->flush();
		 sentBytes += buffer.size();
		}
	 file.close();

	 emit uploadingIsFinished();

	 qDebug()<<"File uploading has finished";
	 qDebug()<<"Sent bytes:"<<sentBytes;

	 expectedSize = 0;
	}
 else {
	 qWarning()<<"FTP_CLIENT: File openning error has occurred";
	}
}

qint8 FtpClient::download(const QString &path, const QString &filename)
{
 isFinished = false;

 QFile file(path + filename);

 if(file.open(QFile::WriteOnly))
	{
	 if(!sendRequest(DOWNLOAD, filename))
		{
		 qWarning() << "An error has occurred: a request didn't send";
		 return FAIL;
		}

	 qDebug()<<"---Downloading a file...---";

	 qint8 status;
	 while(socket->bytesAvailable() < static_cast<qint64>(sizeof(qint8)))
		socket->waitForReadyRead();

	 socket->read(reinterpret_cast<char*>(&status), sizeof(qint8));

	 if(status == FILE_NOT_FOUND)
		{
		 qDebug() << "STATUS: FILE NOT FOUND";

		 file.remove();

		 return status;
		}
	 else
		{
		 qDebug() << "STATUS: OK";
		}

	 if(!expectedSize)
		{
		 while(socket->bytesAvailable() < static_cast<qint64>(sizeof(qint64))){socket->waitForReadyRead();}

		 socket->read(reinterpret_cast<char *>(&expectedSize), sizeof(qint64));
		 qDebug() << "Expected:" << expectedSize;
		}

	 qint64 receivedSize=0;
	 qint64 leaveSize=0;
	 QByteArray buffer;
	 while(receivedSize < expectedSize)
		{
		 leaveSize = expectedSize - receivedSize;

		 if(socket->bytesAvailable() < 1)
			socket->waitForReadyRead();

		 buffer = socket->read(BUFFER_SIZE < leaveSize ? BUFFER_SIZE : leaveSize);
		 receivedSize += (buffer.size());

		 qDebug()<<"Already received: "<<receivedSize;
		 file.write(buffer);
		}

	 file.close();
	 expectedSize = 0;

	 qDebug()<<"---The file received---";

	 return OK;
	}
 else
	{
	 qWarning() << "Couldn't open a file to write";
	 return FAIL;
	}
}

bool FtpClient::sendRequest(qint8 request, const QString &filename)
{
 if(socket->state() != QTcpSocket::SocketState::ConnectedState)
	{
	 connectToServer();
	}

 if(socket->state() != QTcpSocket::SocketState::ConnectedState)
	{
	 qWarning()<<"FTP_CLIENT: ERROR: Time response is elapsed";
	 return false;
	}

 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);
 out.setVersion(QDataStream::Qt_5_11);

 out << request<< filename;

 socket->write(arrBlock);
 socket->flush();

 return true;
}

void FtpClient::slotConnected()
{
 qDebug()<<"---Connecting to the server is done---";
}


void FtpClient::slotError(QAbstractSocket::SocketError)
{
 qWarning()<<"FTP_CLIENT: Connection error has occured!";
 exit(-1);
}
