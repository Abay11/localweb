#include <QCoreApplication>

#include <QDebug>
#include <iostream>

#include "iohandler.h"
#include "connectionhandler.h"

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

 IOHandler voiceIO;

 ConnectionHandler connectionHandler(5000);

 QObject::connect(&voiceIO, SIGNAL(readData(QByteArray&)), &connectionHandler, SLOT(slotWriteDataTo(QByteArray&)));

 voiceIO.startRecording();

	qDebug() << "MAIN FINISHED";

 return a.exec();
}
