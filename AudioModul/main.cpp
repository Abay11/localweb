#include <QCoreApplication>

#include <QDebug>
#include <iostream>

#include "clientside.h"

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

 IOHandler voiceIO;

 QString port_str("15999");

 if(argc > 1)
	QString port_str = argv[1];

 ClientSide client(port_str.toInt());

 client.start();

	qDebug() << "MAIN FINISHED";

 return a.exec();
}
