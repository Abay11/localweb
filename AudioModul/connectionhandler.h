#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>
#include <QHostAddress>

class QUdpSocket;

class ConnectionHandler : public QObject
{
	Q_OBJECT

private:
	QUdpSocket* socket;

	QHostAddress serverhost;

	QHostAddress dest_host;

	quint16 server_port;

	quint16 dest_port;

	quint16 binding_port = 5000;

	const QString DTAG = "AudioModule::ConnectionHandler:";

public:
	explicit ConnectionHandler(quint16 server_port, const QHostAddress& serverhost = QHostAddress("127.0.0.1"), QObject* parent = nullptr);

	void startListen();

	void stopListen();

	void setDestination(const QHostAddress& host, quint16 port);

	quint16 getPort();

signals:
	void dataArrived(QByteArray&);

private slots:
	void slotReadDataFrom();

public slots:
	void slotWriteDataTo(QByteArray&);
};

#endif // CONNECTIONHANDLER_H
