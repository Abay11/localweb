#include <QtTest>
#include <QCoreApplication>

#include "clientservicefordebug.h"

// add necessary includes here
#include "../LocalwebClient/clientservice.h"
#include "../servernetworkservice.h"

class test : public QObject
{
 Q_OBJECT

public:
 test();
 ~test();

private slots:
 //server
 void test_addToBase();
 void test_saveDataAndRestoreData();
 void test_startServer0();

 //both
 void test_registration();
 void test_notifying();
 void test_clientConnectionToServer();
 void test_sendingAndReadingMessage();

 //client
 void test_addAllUsersToOfflineModel();
 void test_addNewOnlineToModel();
};

test::test()
{

}

test::~test()
{

}

static QString savingFile="serverdata.bin";

void test::test_addToBase()
{
 QFile::remove(savingFile);

 ServerNetworkService *pservice=new ServerNetworkService;
 QTcpSocket *client1 = new QTcpSocket;
 QTcpSocket *client2 = new QTcpSocket;
 QTcpSocket *client3 = new QTcpSocket;

 QCOMPARE(pservice->addUserIfNickNotBusy("nick1","", client1), true);
 QCOMPARE(pservice->addUserIfNickNotBusy("nick2","", client2), true);
 QCOMPARE(pservice->addUserIfNickNotBusy("JOHNY","", client3), true);

 QCOMPARE(pservice->getClientBase()->size(), 3);
 QCOMPARE(pservice->getClientsList().size(), 3);

 QCOMPARE(pservice->addUserIfNickNotBusy("nick1","", client1), false);
 QCOMPARE(pservice->addUserIfNickNotBusy("NICK2","", client2), false);
 QCOMPARE(pservice->addUserIfNickNotBusy("JOHNY","", client3), false);

 QCOMPARE(pservice->getClientBase()->size(), 3);
 QCOMPARE(pservice->getClientsList().size(), 3);

 delete pservice;
 delete  client1;
 delete  client2;
 delete  client3;
}

void test::test_saveDataAndRestoreData()
{
 QFile::remove(savingFile);

 quint16 nport=9009;
 ServerNetworkService *pserviceForSave=new ServerNetworkService;
 pserviceForSave->setPort(nport);
 QCOMPARE(pserviceForSave->getPort(), nport);

 ServerNetworkService *pserviceForRestore=new ServerNetworkService;

 pserviceForSave->addToBase("nick1","name1","127.0.0.1","8888");
 pserviceForSave->addToBase("nick2","name2","127.0.0.1","9999");

 QCOMPARE(pserviceForSave->saveData(savingFile), true);
 QCOMPARE(pserviceForRestore->restoreData(savingFile), true);
 QCOMPARE(pserviceForRestore->getPort(), nport);

 QCOMPARE(pserviceForRestore->getClientBase()->size(), 2);

 auto keys=pserviceForRestore->getClientBase()->keys();
 QCOMPARE(keys.at(0), "nick1");
 QCOMPARE(keys.at(1), "nick2");

 auto cinfo1=pserviceForRestore->getClientBase()->first();
 QCOMPARE(cinfo1->fullName(), "name1");
 QCOMPARE(cinfo1->address(), "127.0.0.1");
 QCOMPARE(cinfo1->port(), "8888");

 auto cinfo2=pserviceForRestore->getClientBase()->last();
 QCOMPARE(cinfo2->fullName(), "name2");
 QCOMPARE(cinfo2->address(), "127.0.0.1");
 QCOMPARE(cinfo2->port(), "9999");


 QCOMPARE(QFile::exists(savingFile), true);

 delete pserviceForSave;
 delete pserviceForRestore;
}

void test::test_startServer0()
{
 QFile::remove(savingFile);
 quint16 nPort=7777;
 ServerNetworkService *pservice0=new ServerNetworkService;
 pservice0->setPort(nPort);

 QCOMPARE(pservice0->slotStartServer(), true);
 QCOMPARE(pservice0->listeningPort(), nPort);

 pservice0->slotStopServer();
 QCOMPARE(pservice0->slotStartServer(), true);


 ServerNetworkService *pservice1=new ServerNetworkService;
 pservice1->setPort(nPort);
 QCOMPARE(pservice1->slotStartServer(), false);


 pservice0->slotStopServer();
 pservice1->slotStopServer();
 delete pservice0;
 delete pservice1;

 QFile::remove(savingFile);
}

void test::test_registration()
{
 QString port="11000";
 ServerNetworkService *pserver=new ServerNetworkService;
 pserver->setPort(quint16(port.toInt()));
 QCOMPARE(pserver->slotStartServer(), true);

 ClientServiceForDebug *pclient0=new ClientServiceForDebug;
 pclient0->slotSetAddress("localhost", port);
 pclient0->setNickAndName("client0", "name0");
 pclient0->slotConnectToServer();

 QEventLoop *loop=new QEventLoop;
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 pclient0->slotSendToServer(DATATYPE::REGISTRATION, "pclient0", "name0");
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->getRegistrationResult(), true);

 ClientServiceForDebug *pclient1=new ClientServiceForDebug;
 pclient1->slotSetAddress("localhost", port);
 pclient1->setNickAndName("client1", "name1");
 pclient1->slotConnectToServer();

 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 pclient1->slotSendToServer(DATATYPE::REGISTRATION, "pclient1", "name1");
 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient1->getRegistrationResult(), true);

 ClientServiceForDebug *pclientClone=new ClientServiceForDebug;
 pclientClone->slotSetAddress("localhost", port);
 pclientClone->setNickAndName("client0", "name0");
 pclientClone->slotConnectToServer();

 QObject::connect(pclientClone, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 pclientClone->slotSendToServer(DATATYPE::REGISTRATION, "pclient0", "name0");
 QObject::connect(pclientClone, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclientClone->getRegistrationResult(), false);

 pclient0->slotDisconnectFromServer();
 pclient1->slotDisconnectFromServer();
 pclientClone->slotDisconnectFromServer();
 pserver->slotStopServer();

 delete pserver;
 delete pclient0;
 delete pclient1;
 delete pclientClone;
 delete loop;
}

void test::test_notifying()
{
 QFile::remove(savingFile);

 QString port="22222";
 ServerNetworkService *pserver=new ServerNetworkService;
 pserver->setPort(quint16(port.toInt()));
 QCOMPARE(pserver->slotStartServer(), true);
 QCOMPARE(pserver->listeningPort(), port.toInt());

 pserver->addToBase("client0", "name0", "localhost", "0");
 pserver->addToBase("client1", "name1", "localhost", "0");

 QEventLoop *loop=new QEventLoop;
 ClientServiceForDebug *pclient0=new ClientServiceForDebug;
 pclient0->setNickAndName("client0", "name0");
 pclient0->slotSetAddress("localhost", port);
 pclient0->slotConnectToServer();
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->getOnlines().size(), 1);

 ClientServiceForDebug *pclient1=new ClientServiceForDebug;
 pclient1->setNickAndName("client1", "name1");
 pclient1->slotSetAddress("localhost", port);
 pclient1->slotConnectToServer();
 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->getOnlines().size(), 2);
 QCOMPARE(pclient1->getOnlines().size(), 2);


 pclient0->slotDisconnectFromServer();
 pclient1->slotDisconnectFromServer();
 pserver->slotStopServer();


 delete pclient0;
 delete pclient1;
 delete pserver;
 delete loop;
}

void test::test_clientConnectionToServer()
{
 QFile::remove(savingFile);

 QString port="9000";
 ServerNetworkService *pserver=new ServerNetworkService;
 pserver->setPort(quint16(port.toInt()));
 QCOMPARE(pserver->slotStartServer(), true);

 QTcpSocket *client0=new QTcpSocket;
 QTcpSocket *client1=new QTcpSocket;

 QCOMPARE(pserver->addUserIfNickNotBusy("client0", "name0", client0), true);
 QCOMPARE(pserver->addUserIfNickNotBusy("client1", "name1", client1), true);
 pserver->removeFromOnlines(client0);
 pserver->removeFromOnlines(client1);

 ClientServiceForDebug *pclient0=new ClientServiceForDebug;
 pclient0->slotSetAddress("localhost", port);
 pclient0->setNickAndName("client0", "name0");
 pclient0->slotConnectToServer();

 QEventLoop *loop=new QEventLoop;
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->isConnected(), true);

 auto clientbase = pclient0->getClientBase();
 QCOMPARE(clientbase->size(), 2);
 QCOMPARE(pclient0->getOnlines().size(), 1);

 ClientServiceForDebug *pclient1=new ClientServiceForDebug;
 pclient1->setNickAndName("client1", "nick1");
 pclient1->slotSetAddress("localhost", port);
 pclient1->slotConnectToServer();

 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient1->isConnected(), true);

 auto clientbase1 = pclient1->getClientBase();
 QCOMPARE(clientbase1->size(), 2);
 QCOMPARE(pclient1->getOnlines().size(), 2);

 pclient0->slotDisconnectFromServer();
 pclient1->slotDisconnectFromServer();
 pserver->slotStopServer();
 delete pclient0;
 delete pclient1;
 delete pserver;
 delete loop;
 delete client0;
 delete client1;
 QFile::remove(savingFile);
}

void test::test_sendingAndReadingMessage()
{
 QFile::remove(savingFile);

 QString port="9000";
 ServerNetworkService *pserver=new ServerNetworkService;
 pserver->setPort(quint16(port.toInt()));
 QCOMPARE(pserver->slotStartServer(), true);
 pserver->addToBase("client0", "name0", "localhost", "0");
 pserver->addToBase("client1", "name1", "localhost", "0");

 QEventLoop *loop=new QEventLoop;

 ClientServiceForDebug *pclient0=new ClientServiceForDebug;
 pclient0->setNickAndName("client0", "name0");
 pclient0->slotSetAddress("localhost", port);
 pclient0->slotConnectToServer();
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 ClientServiceForDebug *pclient1=new ClientServiceForDebug;
 pclient1->setNickAndName("client1", "name1");
 pclient1->slotSetAddress("localhost", port);
 pclient1->slotConnectToServer();
 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->isConnected(), true);
 QCOMPARE(pclient1->isConnected(), true);

 QString sendingMessage="Read me!";

 pclient0->slotSendToServer(DATATYPE::MESSAGE, sendingMessage);
 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 pclient1->slotSendToServer(DATATYPE::MESSAGE, sendingMessage);
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->receivedMessage, "client1: "+sendingMessage);

 pclient0->slotDisconnectFromServer();
 pserver->slotStopServer();
 delete pclient0;
 delete pserver;
 delete loop;
}

void test::test_addAllUsersToOfflineModel()
{
 QFile::remove(savingFile);

 QString port="6565";
 ServerNetworkService *pserver=new ServerNetworkService;
 pserver->setPort(quint16(port.toInt()));
 QCOMPARE(pserver->slotStartServer(), true);

 QTcpSocket *psocket0=new QTcpSocket;
 QTcpSocket *psocket1=new QTcpSocket;
 QTcpSocket *psocket2=new QTcpSocket;

 pserver->addUserIfNickNotBusy("n0", "", psocket0);
 pserver->addUserIfNickNotBusy("n1", "", psocket1);
 pserver->addUserIfNickNotBusy("n2", "", psocket2);
 pserver->removeFromOnlines(psocket0);

 QEventLoop *loop=new QEventLoop;

 ClientServiceForDebug *pclient0=new ClientServiceForDebug;
 pclient0->setNickAndName("n0", "");
 pclient0->slotSetAddress("localhost", port);
 pclient0->slotConnectToServer();
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 pclient0->addAllUsersToOfflineModel();

 auto onlines = pclient0->onlineModel()->stringList();
 auto offlines= pclient0->offlineModel()->stringList();

 QCOMPARE(onlines.size(), 0);
 QCOMPARE(offlines.size(), 3);
 QCOMPARE(offlines.contains("Вы: n0"), true);

 pclient0->slotDisconnectFromServer();
 pserver->slotStopServer();
 delete psocket0;
 delete psocket1;
 delete psocket2;
 delete pclient0;
 delete pserver;
 delete loop;
}

void test::test_addNewOnlineToModel()
{
 ClientServiceForDebug *pclient0=new ClientServiceForDebug;
 pclient0->addNewOnlineToModel("Client1");
 pclient0->addNewOnlineToModel("Client2");
 pclient0->addNewOnlineToModel("Client3");
 pclient0->addNewOnlineToModel("Client4");
 pclient0->addNewOnlineToModel("Client5");
 QCOMPARE(pclient0->getOnlines().size(), 5);
 QCOMPARE(pclient0->getOnlines().first(), "Client1");
 QCOMPARE(pclient0->getOnlines().last(), "Client5");

 delete  pclient0;
}

QTEST_MAIN(test)

#include "tst_test.moc"
