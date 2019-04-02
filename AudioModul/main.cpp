#include <QCoreApplication>

#include <QDebug>
#include <iostream>

#include "iohandler.h"

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

 IOHandler handler;
 handler.startRecording();

	qDebug() << "MAIN FINISHED";

 return a.exec();
}
