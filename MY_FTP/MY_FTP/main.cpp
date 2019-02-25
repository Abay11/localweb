#include <QCoreApplication>

#include "ftp_server.h"
#include "ftp_client.h"

#include <iostream>

#include <QThread>
#include <QEventLoop>

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

		QEventLoop loop;
		loop.exec();

		break;
	 }

	case CLIENT:
	 {
		FtpClient client("localhost", 21021);

		QString path = "/home/adygha/Desktop/";
		QString dest = "/home/adygha/Desktop/LocalWeb_downloads/";
		QString name = "tanks_3.png";

//		client.sendRequest(DOWNLOAD, name);
//		client.upload(path, name);
		client.download(dest, name);

		QEventLoop loop;
		QObject::connect(&client, SIGNAL(uploadingIsFinished()), &loop, SLOT(quit()));

		break;
	 }
	default: break;
	}

 std::cout<<"THE PROGRAM ENDED"<<std::endl;
//*/
}
