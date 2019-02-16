#include <QCoreApplication>

#include "ftp_server.h"
#include "ftp_client.h"

#include <iostream>

#include <QThread>

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

 std::cout<<"THE PROGRAM STARTED"<<std::endl;

 FtpServer *server = new FtpServer;
 if(!server->start(21021))
	qDebug()<<"Error occurred";
 else {
	qDebug()<<"SERVER IS RUNNING";
	}

 while(server->isRunning())
	{
	 QThread::sleep(10000);
	}

 std::cout<<"THE PROGRAM ENDED"<<std::endl;

 return a.exec();
}
