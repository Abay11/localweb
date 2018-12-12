#include "mywidget.h"
#include "servernetworkservice.h"

#include <QApplication>


int main(int argc, char *argv[])
{
 QApplication a(argc, argv);

 ServerNetworkService *pservice=new ServerNetworkService;

 MyWidget w;
 w.setModelToView(pservice->getModel());

 QObject::connect(&w, SIGNAL(startClicked()),
									pservice, SLOT(slotStartServer()));
 QObject::connect(&w, SIGNAL(startClicked()), pservice, SLOT(slotStartServer()));
 QObject::connect(&w, SIGNAL(stopClicked()), pservice, SLOT(slotStopServer()));

 w.show();

 return a.exec();
}
