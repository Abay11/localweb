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

void ClientServiceForDebug::addAllUsersToOfflineModel()
{
 ClientService::addAllUsersToOfflineModel();
}

void ClientServiceForDebug::addNewOnlineToModel(QString nick)
{
 ClientService::addNewOnlineToModel(nick);
}

void ClientServiceForDebug::removeOnlineFromOfflines(QStringList onlines)
{
 ClientService::removeOnlinesFromOfflines(onlines);
}

void ClientServiceForDebug::setListToOnlineModel(QStringList onlines)
{
 ponlineModel->setStringList(onlines);
}

void ClientServiceForDebug::setListToOfflineModel(QStringList offlines)
{
 pofflineModel->setStringList(offlines);
}

void ClientServiceForDebug::throwOnlinesToOfflines()
{
 ClientService::throwOnlinesToOfflines();
}

QStringList ClientServiceForDebug::getOnlines()
{
 return ponlineModel->stringList();
}

QStringList ClientServiceForDebug::getOfflines()
{
 return pofflineModel->stringList();
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
