#include <QCoreApplication>

#include "audiomodulclient.h"
#include <QDebug>

int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);

	qDebug() << "MAIN FINISHED";

 return a.exec();
}
