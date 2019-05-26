#ifndef VCONNECTIONHANDLER_H
#define VCONNECTIONHANDLER_H

#include <QObject>
#include <QHostAddress>

class QUdpSocket;

namespace VideoModule
{
	class ConnectionHandler : public QObject
	{
		Q_OBJECT
	public:
		explicit ConnectionHandler(quint16 server_port,
			const QHostAddress& host = QHostAddress("127.0.0.1"),
			QObject* parent = nullptr);

		~ConnectionHandler();

		void startListen();

		void stopListen();

		void setDestination(const QHostAddress& host, quint16 port);

		quint16 getPort();

	signals:
		void dataArrived(QByteArray&);

	private slots:
		void slotReadDataFrom();

	public slots:
		void slotWriteData(const QByteArray&);

	private:
		QUdpSocket* socket;

		QHostAddress serverhost;

		QHostAddress dest_host;

		quint16 server_port;

		quint16 dest_port;

		quint16 binding_port = 6000;

		const QString DTAG = "VideoModule::ConnectionHandler:";
	};
}

#endif // VCONNECTIONHANDLER_H
