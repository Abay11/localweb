#include <QApplication>

#include "mainwindow.h"

#include "convertionwidget.h"

int main(int argc, char *argv[])
{
 QApplication a(argc, argv);
// MainWindow w;
 ConvertionWidget w("General");
 w.show();

 return a.exec();
}
