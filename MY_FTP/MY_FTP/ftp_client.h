#ifndef FTP_CLIENT_H
#define FTP_CLIENT_H

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;

class FtpClient : public QObject
{
	Q_OBJECT

private:
	QString serverAddress;

	quint16 port;

	qint64 expectedSize = 0;

	QTcpSocket* socket;

public:
	FtpClient(const QString& serverAddress, quint16 port);

	virtual ~FtpClient();

	void connectToServer();

	void disconnect();

	void upload(const QString& path, const QString& filename);

	qint8 download(const QString& path, const QString& filename);

	bool isFinished = false;

	bool sendRequest(qint8 request, const QString& filename);

signals:
	void uploadingIsFinished();

public slots:
	void slotConnected();

	void slotError(QAbstractSocket::SocketError);
};

#endif // FTP_CLIENT_H
