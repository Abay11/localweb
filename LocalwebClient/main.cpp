#include <QApplication>

#include "mainwindow.h"
#include "convertionwidget.h"

int main(int argc, char *argv[])
{
 QApplication a(argc, argv);
 MainWindow w;
 w.show();

 return a.exec();
}
