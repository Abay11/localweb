#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "../LocalwebClient/clientservice.h"
#include "../mywidget.h"
#include "../servernetworkservice.h"
#include "simpleclient.h"

#include <QObject>
#include <unistd.h>

class test : public QObject
{
 Q_OBJECT

private slots:
 void test_addToBase();
 void test_saveDataAndRestoreData();
 void test_startServer0();
 void test_startServer1();
 void test_startServer2();
// void test_readingMessage();
};

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
 ServerNetworkService *pservice=new ServerNetworkService(nPort);
 QCOMPARE(pservice->slotStartServer(), true);
 QCOMPARE(pservice->serverPort(), nPort);
 pservice->slotStopServer();

 delete pservice;
}

void test::test_startServer1()
{
 quint16 nPort=7777;
 ServerNetworkService *pservice0=new ServerNetworkService(nPort);
 QCOMPARE(pservice0->slotStartServer(), true);

 ServerNetworkService *pservice1=new ServerNetworkService(nPort);
 QCOMPARE(pservice1->slotStartServer(), false);

 pservice0->slotStopServer();
 pservice1->slotStopServer();
}

void test::test_startServer2()
{
 quint16 nPort=8888;
 ServerNetworkService *pservice=new ServerNetworkService(nPort);
 pservice->slotStartServer();
 pservice->slotStopServer();

 QCOMPARE(pservice->slotStartServer(), true);
 pservice->slotStopServer();
}

//void test::test_readingMessage()
//{
// QString sendingText="Read me!";
// quint16 nPort=9985;

// ServerNetworkService *pservice=new ServerNetworkService(nPort);
// if(!pservice->slotStartServer())
//	QFAIL("Can't open the port");

// SimpleClient *client=new SimpleClient(nPort);
// client->sendMessage(sendingText);

//// QCOMPARE(pservice->receivedMsg, sendingText);
// pservice->slotStopServer();
//}

QTEST_APPLESS_MAIN(test)

#include "tst_test.moc"
