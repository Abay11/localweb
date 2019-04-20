#include "clientside.h"

ClientSide::ClientSide(quint16 serverPort, const QHostAddress &serverAddress, QObject *parent)
 :QObject (parent)
 ,serverPort(serverPort)
 ,serverAddress(serverAddress)
 ,voiceIO(new IOHandler)
 ,connectionHandler(new ConnectionHandler(serverPort, serverAddress))
{
 connectionHandler->startListen();

 //writing voice
 connect(voiceIO, SIGNAL(readData(QByteArray&)), connectionHandler, SLOT(slotWriteDataTo(QByteArray&)));

 //receiving voice
 connect(connectionHandler, SIGNAL(dataArrived(QByteArray&)), voiceIO, SLOT(slotWriteAudioOutput(QByteArray&)));
}

ClientSide::~ClientSide()
{
 connectionHandler->stopListen();
}

void ClientSide::setDestination(const QHostAddress &addr, quint16 port)
{
 connectionHandler->setDestination(addr, port);
}

void ClientSide::setBroadcast()
{
 connectionHandler->setDestination(QHostAddress("255.255.255.255"), 0);
}

void ClientSide::turnOffMicrophone()
{
 voiceIO->stopRecording();
}

void ClientSide::turnOnMicrophone()
{
 voiceIO->startRecording();
}

quint16 ClientSide::getPort()
{
 return connectionHandler->getPort();
}
