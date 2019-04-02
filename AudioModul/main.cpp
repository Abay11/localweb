#include <QCoreApplication>

#include <QDebug>
#include <iostream>

#include "iohandler.h"

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

 IOHandler handler;

	qDebug() << "MAIN FINISHED";

 return a.exec();
}
