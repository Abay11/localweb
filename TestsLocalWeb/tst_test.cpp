#include <QtTest>

// add necessary includes here
#include "../LocalwebClient/clientservice.h"
#include "../mywidget.h"
#include "../servernetworkservice.h"

class test : public QObject
{
 Q_OBJECT

private slots:
 void test_startServer0();
 void test_startServer1();
 void test_startServer2();
};

void test::test_startServer0()
{
 quint16 nPort=7777;
 ServerNetworkService *pservice=new ServerNetworkService(nPort);
 QCOMPARE(pservice->slotStartServer(), true);
 QCOMPARE(pservice->serverPort(), nPort);
 pservice->slotStopServer();
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

QTEST_APPLESS_MAIN(test)

#include "tst_test.moc"
