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
 void test_addToBase();
 void test_saveDataAndRestoreData();
 void test_startServer0();
 void test_registration();
 void test_notifying();
 void test_clientConnectionToServer();
 void test_sendingAndReadingMessage();
};

test::test()
{

}

test::~test()
{

}

void test::test_addToBase()
{
 ServerNetworkService *pservice=new ServerNetworkService(0);
	QCOMPARE(pservice->addToBase("nick1","","",""), true);
	QCOMPARE(pservice->addToBase("nick2","","",""), true);
	QCOMPARE(pservice->addToBase("JOHNY","","",""), true);
	QCOMPARE(pservice->addToBase("test00","name","ip","port"), true);
	QCOMPARE(pservice->addToBase("test0","name","ip","port"), true);
	QCOMPARE(pservice->addToBase("test","name","ip","port"), true);

	QCOMPARE(pservice->addToBase("nick1","","",""), false);
	QCOMPARE(pservice->addToBase("NICK1","","",""), false);
	QCOMPARE(pservice->addToBase("JOHNY","","",""), false);
	QCOMPARE(pservice->addToBase("test00","name","ip","port"), false);
	QCOMPARE(pservice->addToBase("test00","","",""), false);

	delete pservice;
}

void test::test_saveDataAndRestoreData()
{
 QString filename="clientdata.bin";
 quint16 nport=9009;
 ServerNetworkService *pserviceForSave=new ServerNetworkService(nport);
 ServerNetworkService *pserviceForRestore=new ServerNetworkService(0);

 pserviceForSave->addToBase("nick1","name1","127.0.0.1","8888");
 pserviceForSave->addToBase("nick2","name2","127.0.0.1","9999");
 QCOMPARE(pserviceForSave->saveData(filename), true);

 QCOMPARE(pserviceForRestore->restoreData(filename), true);
 QCOMPARE(pserviceForRestore->expectedPort(), nport);

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


 QFile file(filename);
 QCOMPARE(file.exists(), true);
 file.remove();

 delete pserviceForSave;
 delete pserviceForRestore;
}

void test::test_startServer0()
{
 quint16 nPort=7777;
 ServerNetworkService *pservice0=new ServerNetworkService(nPort);
 QCOMPARE(pservice0->slotStartServer(), true);
 QCOMPARE(pservice0->serverPort(), nPort);

 pservice0->slotStopServer();
 QCOMPARE(pservice0->slotStartServer(), true);


 ServerNetworkService *pservice1=new ServerNetworkService(nPort);
 QCOMPARE(pservice1->slotStartServer(), false);


 pservice0->slotStopServer();
 pservice1->slotStopServer();
 delete pservice0;
 delete pservice1;
}

void test::test_registration()
{
 QString port="11000";
 ServerNetworkService *pserver=new ServerNetworkService(11000);
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
 QString port="22222";
 ServerNetworkService *pserver=new ServerNetworkService(22222);
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
 QString port="9000";
 ServerNetworkService *pserver=new ServerNetworkService(9000);
 QCOMPARE(pserver->slotStartServer(), true);

 QCOMPARE(pserver->addToBase("client0", "name0", "", ""), true);
 QCOMPARE(pserver->addToBase("client1", "name1", "", ""), true);


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
}

void test::test_sendingAndReadingMessage()
{
 QString port="9000";
 ServerNetworkService *pserver=new ServerNetworkService(9000);
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

QTEST_MAIN(test)

#include "tst_test.moc"
