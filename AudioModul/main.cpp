#include <QCoreApplication>

#include <QDebug>
#include <iostream>

#include "iohandler.h"
#include "connectionhandler.h"

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

 IOHandler voiceIO;

 QString port_str("15999");

 if(argc > 1)
	QString port_str = argv[1];

 ConnectionHandler connectionHandler(static_cast<quint16>(port_str.toInt()));

 //writing voice
 QObject::connect(&voiceIO, SIGNAL(readData(QByteArray&)), &connectionHandler, SLOT(slotWriteDataTo(QByteArray&)));

 //receiving voice
 QObject::connect(&connectionHandler, SIGNAL(dataArrived(QByteArray&)), &voiceIO, SLOT(slotWriteAudioOutput(QByteArray&)));

 connectionHandler.startListen();

 voiceIO.startRecording();

	qDebug() << "MAIN FINISHED";

 return a.exec();
}
