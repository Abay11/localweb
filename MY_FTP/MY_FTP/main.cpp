#include <QCoreApplication>

#include "ftp_server.h"
#include "ftp_client.h"

#include <iostream>

#include <QThread>

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

 std::cout<<"THE PROGRAM STARTED"<<std::endl;

 enum MODE{CLIENT, SERVER};

 char mode=CLIENT;
// char mode=SERVER;

 switch(mode)
	{
	case SERVER:
	 {
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
		break;
	 }

	case CLIENT:
	 {
		FtpClient client("localhost", 21021);

		QString path = "/home/adygha/Desktop";
		QString name = "tanks_3.png";

		client.upload(path, name);

		while(!client.isFinished)
		 QThread::sleep(3000);

		break;
	 }
	default: break;
	}

 std::cout<<"THE PROGRAM ENDED"<<std::endl;

 return a.exec();
}
