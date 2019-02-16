#include "ftp_client.h"

FtpClient::FtpClient(const QString &address)
 :serverAddress(address)
{}

FtpClient::~FtpClient()
{}

void FtpClient::connectToServer()
{
 qDebug()<<"---Connecting to server...---";
 socket->connectToHost(serverAddress, port);
}

void FtpClient::disconnect()
{
 socket->close();
}

void FtpClient::upload(const QString &path, const QString &filename)
{
 qDebug()<<"---Uploading a file...---";

 while(socket->state() != QTcpSocket::SocketState::ConnectedState)
	{
	 QThread::sleep(5000);
	}

 QFile file(path + filename);

 if(file.open(QFile::ReadOnly))
	{
	 QByteArray arrBlock;
	 QDataStream out(&arrBlock, QIODevice::WriteOnly);
	 out.setVersion(QDataStream::Qt_5_11);

	 QFileInfo finfo(file);

	 expectedSize=static_cast<quint64>(finfo.size());

	 out<<expectedSize<<filename;
	 socket->write(arrBlock);

	 QByteArray buffer;
	 buffer = file.read(100000000);
	 socket->write(buffer);

	 file.close();

	 qDebug()<<"File uploading has finished";
	}
 else {
	 qWarning()<<"FTP_CLIENT: File openning error has occurred";
	}

 qDebug()<<"---The file uploaded---";
}

void FtpClient::slotDownload()
{

}

void FtpClient::slotError(QAbstractSocket::SocketError)
{
 qWarning()<<"FTP_CLIENT: Connection error has occured!";
}
