#include "clientside.h"

ClientSide::ClientSide(quint16 serverPort, const QHostAddress &serverAddress, QObject *parent)
 :QObject (parent)
 ,serverPort(serverPort)
 ,serverAddress(serverAddress)
 ,voiceIO(new IOHandler)
 ,connectionHandler(new ConnectionHandler(serverPort, serverAddress))
{
 //writing voice
 connect(voiceIO, SIGNAL(readData(QByteArray&)), connectionHandler, SLOT(slotWriteDataTo(QByteArray&)));

 //receiving voice
 connect(connectionHandler, SIGNAL(dataArrived(QByteArray&)), voiceIO, SLOT(slotWriteAudioOutput(QByteArray&)));
}

void ClientSide::start()
{
 qDebug() << "ClientSide: started";

 connectionHandler->startListen();

 voiceIO->startRecording();
}

void ClientSide::stop()
{
 connectionHandler->stopListen();

 voiceIO->stopRecording();
}
