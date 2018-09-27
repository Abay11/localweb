#include <QApplication>

#include "stackedwindows.h"


int main(int argc, char *argv[])
{
 QApplication a(argc, argv);
 StackedWindows w;
 w.show();
 return a.exec();
}
