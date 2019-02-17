#include "ftp_client.h"

FtpClient::FtpClient(const QString &address, quint16 port)
 :serverAddress(address)
 ,port(port)
 ,socket(new QTcpSocket)
{
 connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(socket, SIGNAL(readyRead()), SLOT(slotDownload()));
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

 if(socket->state() != QTcpSocket::SocketState::ConnectedState)
	{
	 connectToServer();
	}

 if(socket->state() != QTcpSocket::SocketState::ConnectedState)
	{
	 qWarning()<<"FTP_CLIENT: ERROR: Time response is elapsed";
	 return;
	}

 qDebug()<<"---Uploading a file...---";

 QFile file(path + filename);

 if(file.open(QFile::ReadOnly))
	{
	 QByteArray arrBlock;
	 QDataStream out(&arrBlock, QIODevice::WriteOnly);
	 out.setVersion(QDataStream::Qt_5_11);

	 QFileInfo finfo(file);

	 expectedSize=static_cast<quint64>(finfo.size());

	 out<<expectedSize<<filename;
	 qint64 writedBytes;
	 writedBytes = socket->write(arrBlock);
	 qDebug() << "Writed bytes: " << writedBytes;

	 QByteArray buffer;
	 buffer = file.read(100000000);
	 socket->write(buffer);

	 file.close();

	 emit uploadingIsFinished();

	 qDebug()<<"File uploading has finished";
	}
 else {
	 qWarning()<<"FTP_CLIENT: File openning error has occurred";
	}

 qDebug()<<"---The file uploaded---";
}

void FtpClient::slotConnected()
{
 qDebug()<<"---Connecting to the server is done---";
}

void FtpClient::slotDownload()
{

}

void FtpClient::slotError(QAbstractSocket::SocketError)
{
 qWarning()<<"FTP_CLIENT: Connection error has occured!";
}
