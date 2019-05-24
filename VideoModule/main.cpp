#include <QApplication>

#include "clientside.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	ClientSide* client = new ClientSide(16000);
	client->startCamera();

	return a.exec();
}
