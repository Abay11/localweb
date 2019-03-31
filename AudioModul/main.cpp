#include <QCoreApplication>

#include "audiomodulclient.h"
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

 quint16 port;
 std::cout << "Port: ";
 std::cin >> port;

 int type;
 std::cout << "\nTYPE(0 - listen only, 1 - recording): ";
 std::cin >> type;

 AudioModulClient client("localhost", port, type);

 client.start();

	qDebug() << "MAIN FINISHED";

 return a.exec();
}
