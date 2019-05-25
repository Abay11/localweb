#include <QApplication>

#include "clientside.h"

#include <QWidget>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	ClientSide* client = new ClientSide(16000);

	client->setDestination(QHostAddress("127.0.0.1"),
		6001);

	client->startCamera();

	client->getVideoWidget()->show();

	return a.exec();
}
