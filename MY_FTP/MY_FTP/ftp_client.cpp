#include "ftp_client.h"

#include "ftp_common.h"

#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QDataStream>


FtpClient::FtpClient(const QString& address, quint16 port)
	: serverAddress(address)
	, socket(new QTcpSocket)
{
	this->port = port;

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
	qDebug() << "FTPClient: Connecting to server...";
	qDebug() << "FTPClient: port" << port;
	socket->connectToHost(serverAddress, port);
	socket->waitForConnected(10000);
}

void FtpClient::disconnect()
{
	socket->close();
}

void FtpClient::upload(const QString& path, const QString& filename)
{
	isFinished = false;

	QFile file(path + filename);

	if(file.open(QFile::ReadOnly))
	{
		if(!sendRequest(MY_FTP::UPLOAD, filename))
		{
			qWarning() << "FTPClient: An error has occurred: a request didn't send";
			return;
		}

		qDebug() << "FTPClient: Uploading a file";

		QFileInfo finfo(file);

		expectedSize = finfo.size();

		socket->write(reinterpret_cast<char*>(&expectedSize), sizeof(qint64));

		QByteArray buffer;

		qint64 sentBytes = 0;

		while(sentBytes < expectedSize)
		{
			buffer = file.read(MY_FTP::BUFFER_SIZE);
			socket->write(buffer);
			socket->flush();
			sentBytes += buffer.size();
		}

		file.close();

		emit uploadingIsFinished();

		qDebug() << "FTPClient: File uploading has finished";

		expectedSize = 0;
	}
	else
	{
		qWarning() << "FTPClient: File openning error has occurred";
	}
}

qint8 FtpClient::download(const QString& path, const QString& filename)
{
	isFinished = false;

	QDir dir(path);

	if(!dir.exists())
	{
		if(!dir.mkpath("./"))
		{
			qWarning() << "FTPClient: Couldn't create a download directory";

			return MY_FTP::FAIL;
		}
	}

	QFile file(path + filename);

	if(file.open(QFile::WriteOnly))
	{
		if(!sendRequest(MY_FTP::DOWNLOAD, filename))
		{
			qWarning() << "FTPClient: An error has occurred: a request didn't send";
			return MY_FTP::FAIL;
		}

		qDebug() << "FTPClient: Downloading a file";

		qint8 status;

		while(socket->bytesAvailable() < static_cast<qint64>(sizeof(qint8)))
			socket->waitForReadyRead();

		socket->read(reinterpret_cast<char*>(&status), sizeof(qint8));

		if(status == MY_FTP::FILE_NOT_FOUND)
		{
			qDebug() << "FTPClient: STATUS: FILE NOT FOUND";

			file.remove();

			return status;
		}
		else
		{
			qDebug() << "FTPClient: STATUS: OK";
		}

		if(!expectedSize)
		{
			while(socket->bytesAvailable() < static_cast<qint64>(sizeof(qint64)))
			{
				socket->waitForReadyRead();
			}

			socket->read(reinterpret_cast<char*>(&expectedSize), sizeof(qint64));
		}

		qint64 receivedSize = 0;
		qint64 leaveSize = 0;
		QByteArray buffer;

		while(receivedSize < expectedSize)
		{
			leaveSize = expectedSize - receivedSize;

			if(socket->bytesAvailable() < 1)
				socket->waitForReadyRead();

			buffer = socket->read(MY_FTP::BUFFER_SIZE < leaveSize ? MY_FTP::BUFFER_SIZE : leaveSize);
			receivedSize += (buffer.size());

			qDebug() << "FTPClient: Already received: " << receivedSize;
			file.write(buffer);
		}

		file.close();
		expectedSize = 0;

		qDebug() << "FTPClient: The file received";

		return MY_FTP::OK;
	}
	else
	{
		qWarning() << "FTPClient: Couldn't open a file to write";
		return MY_FTP::FAIL;
	}
}

bool FtpClient::sendRequest(qint8 request, const QString& filename)
{
	if(socket->state() != QTcpSocket::SocketState::ConnectedState)
	{
		connectToServer();
	}

	if(socket->state() != QTcpSocket::SocketState::ConnectedState)
	{
		qWarning() << "FTPClient: ERROR: Time response is elapsed";
		return false;
	}

	QByteArray arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_11);

	out << request << filename;

	socket->write(arrBlock);
	socket->flush();

	return true;
}

void FtpClient::slotConnected()
{
	qDebug() << "FTPClient: Connected to the server";
}


void FtpClient::slotError(QAbstractSocket::SocketError)
{
	qWarning() << "FTPClient: Connection error has occured!";
	exit(-1);
}
