#ifndef CLIENTSIDE_H
#define CLIENTSIDE_H

#include "iohandler.h"

#include <QObject>
#include <QHostAddress>

class IOHandler;
class ConnectionHandler;

class ClientSide : public QObject
{
	Q_OBJECT
public:
	explicit ClientSide(quint16 serverPort, const QHostAddress& serverAddress = QHostAddress("localhost"), QObject* parent = nullptr);

	~ClientSide();

	void setDestination(const QHostAddress& addr, quint16 port);

	void setBroadcast();

	void turnOnMicrophone();

	void turnOffMicrophone();

	void turnOnSpeakers();

	void turnOffSpeakers();

	quint16 getPort();

signals:

public slots:

private:
	quint16 serverPort;

	QHostAddress serverAddress;

	IOHandler* voiceIO;

	ConnectionHandler* connectionHandler;

	const QString DTAG = "AudioModule::ClientSide:";
};

#endif // CLIENTSIDE_H
