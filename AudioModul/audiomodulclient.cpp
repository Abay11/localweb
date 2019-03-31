#include "audiomodulclient.h"

AudioModulClient::AudioModulClient(QString address, quint16 port, int type)
 :connection(new NetworkHandler(address, port))
 ,iodev(new IOHandler)
 ,type(type)
{
 QObject::connect(iodev, SIGNAL(readySend(QByteArray)), connection, SLOT(slotWriteData(QByteArray)));

 QObject::connect(connection, SIGNAL(readyRead(QByteArray)), iodev, SLOT(slotReadyRead(QByteArray)));
}

AudioModulClient::~AudioModulClient()
{
 delete connection;

 delete iodev;
}

void AudioModulClient::start()
{
 qInfo() << "Launch...";

 connection->startListen();

 iodev->startRecording(type);
}

void AudioModulClient::stop()
{
 qInfo() << "Stop...";

 connection->stopListen();

 iodev->stopRecording();
}
