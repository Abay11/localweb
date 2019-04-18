#ifndef CLIENTSIDE_H
#define CLIENTSIDE_H

#include <QObject>
#include <QHostAddress>

#include "iohandler.h"
#include "connectionhandler.h"

class ClientSide : public QObject
{
 Q_OBJECT
public:
 explicit ClientSide(quint16 serverPort, const QHostAddress& serverAddress=QHostAddress("localhost"), QObject *parent = nullptr);

 ~ClientSide();

 void setDestination(const QHostAddress& addr, quint16 port);

 void setBroadcast();

 void turnOffMicrophone();

 void turnOnMicrophone();

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
