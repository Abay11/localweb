#include "ftp_client.h"

FtpClient::FtpClient(const QString &address)
 :serverAddress(address)
{}

FtpClient::~FtpClient()
{}

void FtpClient::connectToServer()
{
 socket->connectToHost(serverAddress, port);
}

void FtpClient::disconnect()
{
 socket->close();
}

void FtpClient::upload(const QString &path, const QString &filename)
{
 QFile file(path + filename);

 if(file.open(QFile::ReadOnly))
	{
	 QDataStream out(socket);
	 out.setVersion(QDataStream::Qt_5_11);

	 QFileInfo finfo(file);

	 expectedSize=static_cast<quint64>(finfo.size());

	 QByteArray buffer;
	 buffer = file.read(100000000);
	 socket->write(buffer);

	 file.close();
	}
 else {
	 qWarning()<<"FTP_CLIENT: File openning error has occurred";
	}
}

void FtpClient::slotDownload()
{

}

void FtpClient::slotError(QAbstractSocket::SocketError)
{
 qWarning()<<"FTP_CLIENT: Connection error has occured!";
}
