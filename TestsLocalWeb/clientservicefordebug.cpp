#include "clientservicefordebug.h"

ClientServiceForDebug::ClientServiceForDebug(QWidget *parent)
 :ClientService (parent)
{
 connect(psocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

bool ClientServiceForDebug::getRegistrationResult()
{
 return registrationResult;
}

const CLIENTBASE *ClientServiceForDebug::getClientBase()
{
 return &clients;
}

void ClientServiceForDebug::slotReadyRead()
{
 QTimer *timer=new QTimer;
 timer->setInterval(25);
 QThread *thread=new QThread;
 timer->moveToThread(thread);
 thread->start();

 connect(thread, SIGNAL(started()), timer, SLOT(start()));
 connect(timer, SIGNAL(timeout()), thread, SLOT(quit()));
 connect(timer, SIGNAL(timeout()), this, SIGNAL(debugPurpose()));

// timer->deleteLater();
// thread->deleteLater();
}
