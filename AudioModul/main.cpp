#include <QCoreApplication>

#include <QDebug>
#include <iostream>
#include <string.h>

#include "clientside.h"
#include "serverside.h"

int main(int argc, char *argv[]) /*expected parameters: program port mode*/
{
 QCoreApplication a(argc, argv);

 IOHandler voiceIO;

 qDebug() << "args: " << argv[1] << argv[2];

 if(argc > 1)
	{
	 if(argv[1] == QString("s"))
		{
		 QString listening_port("16000");

		 if(argc > 2)
			listening_port = argv[2];

		 ServerSide* server = new ServerSide(static_cast<quint16>(listening_port.toInt()));

		 server->startListening();
		}
	 else
		{
		 QString server_port("15999");

		 if(argc > 2)
			{
			server_port = argv[2];
			}

		 ClientSide* client = new ClientSide(static_cast<quint16>(server_port.toInt()));
//		 client->setBroadcast();
		 client->setDestination(QHostAddress("195.195.195.195"), 55555);

		 client->start();
		}
	}



	qDebug() << "MAIN FINISHED";

 return a.exec();
}
