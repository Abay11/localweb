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

 void start();

 void stop();

signals:

public slots:

private:
 quint16 serverPort;
 QHostAddress serverAddress;

 IOHandler* voiceIO;

 ConnectionHandler* connectionHandler;
};

#endif // CLIENTSIDE_H
