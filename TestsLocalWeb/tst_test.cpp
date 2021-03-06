#include <QtTest>
#include <QCoreApplication>
#include <QApplication>

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
 void test_sendingAndReadingPublicMessage();
 void test_sendingPrivateMessage();

 //client
 void test_addAllUsersToOfflineModel();
 void test_addNewOnlineToModel();
 void test_removeOnlinesFromOfflines();
 void test_removeConnectedFromOfflines();
 void test_throwOnlinesToOfflines();
 void test_throwDisconnectedToOfflines();

public slots:
};

test::test()
{

}

test::~test()
{

}

static QString savingFile="serverdata.bin";
static QString clientSavingFile="data.bin";

void test::test_addToBase()
{
 QFile::remove(savingFile);
 QFile::remove(clientSavingFile);

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
 QFile::remove(clientSavingFile);

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
 QFile::remove(clientSavingFile);
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
}

void test::test_registration()
{
 QFile::remove(clientSavingFile);
 QFile::remove(savingFile);

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
 QFile::remove(clientSavingFile);
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

 QCOMPARE(pclient0->getOnlines().size(), 2);
 QCOMPARE(pclient0->getOnlines().contains("Общий чат"), true);

 ClientServiceForDebug *pclient1=new ClientServiceForDebug;
 pclient1->setNickAndName("client1", "name1");
 pclient1->slotSetAddress("localhost", port);
 pclient1->slotConnectToServer();
 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->getOnlines().size(), 3);
 QCOMPARE(pclient1->getOnlines().size(), 3);


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
 QFile::remove(clientSavingFile);
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
 QCOMPARE(pclient0->getOnlines().size(), 2);

 ClientServiceForDebug *pclient1=new ClientServiceForDebug;
 pclient1->setNickAndName("client1", "nick1");
 pclient1->slotSetAddress("localhost", port);
 pclient1->slotConnectToServer();

 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient1->isConnected(), true);

 auto clientbase1 = pclient1->getClientBase();
 QCOMPARE(clientbase1->size(), 2);
 QCOMPARE(pclient1->getOnlines().size(), 3);

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

void test::test_sendingAndReadingPublicMessage()
{
 QFile::remove(clientSavingFile);
 QFile::remove(savingFile);

 QString port="9000";
 ServerNetworkService *pserver=new ServerNetworkService;
 pserver->setPort(quint16(port.toInt()));
 QCOMPARE(pserver->slotStartServer(), true);
 pserver->addToBase("client0", "name", "","");
 pserver->addToBase("client1", "name", "","");

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
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->isConnected(), true);
 QCOMPARE(pclient1->isConnected(), true);

 QString sendingMessage="Read me!";
 QString sendingMessage2="Read me! 2";

 QString destination="Общий чат";
 pclient0->slotSendToServer(DATATYPE::MESSAGE, sendingMessage, destination);
 qApp->processEvents();
 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();
 QCOMPARE(pclient1->receivedMessage, sendingMessage);

 pclient1->slotSendToServer(DATATYPE::MESSAGE, sendingMessage2, destination);
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 QCOMPARE(pclient0->receivedMessage, sendingMessage2);

 pclient0->slotDisconnectFromServer();
 pclient1->slotDisconnectFromServer();
 pserver->slotStopServer();
 delete pclient0;
 delete pclient1;
 delete pserver;
 delete loop;
}

void test::test_sendingPrivateMessage()
{
// /*
 QFile::remove(savingFile);
 QFile::remove(clientSavingFile);

 QString port="9000";
 ServerNetworkService *pserver=new ServerNetworkService;
 pserver->setPort(quint16(port.toInt()));
 QCOMPARE(pserver->slotStartServer(), true);
 pserver->addToBase("client0", "name0", "localhost", "0");
 pserver->addToBase("client1", "name1", "localhost", "0");
 pserver->addToBase("client2", "name2", "localhost", "0");

 ClientServiceForDebug *pclient0=new ClientServiceForDebug;
 pclient0->setNickAndName("client0", "name0");
 pclient0->slotSetAddress("localhost", port);
 pclient0->slotConnectToServer();
 QEventLoop *loop=new QEventLoop;
 QObject::connect(pclient0, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 ClientServiceForDebug *pclient1=new ClientServiceForDebug;
 pclient1->setNickAndName("client1", "name1");
 pclient1->slotSetAddress("localhost", port);
 pclient1->slotConnectToServer();
 QObject::connect(pclient1, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 ClientServiceForDebug *pclient2=new ClientServiceForDebug;
 pclient2->setNickAndName("client2", "name2");
 pclient2->slotSetAddress("localhost", port);
 pclient2->slotConnectToServer();
 QObject::connect(pclient2, SIGNAL(debugPurpose()), loop, SLOT(quit()));
 loop->exec();

 qApp->processEvents();
 QCOMPARE(pclient0->isConnected(), true);
 QCOMPARE(pclient1->isConnected(), true);
 QCOMPARE(pclient2->isConnected(), true);

 QString privateMessage="Private from c0 to c2!";

 pclient0->slotSendToServer(DATATYPE::MESSAGE, privateMessage, "client2");
 QEventLoop *waitPrivateMessage=new QEventLoop;
 connect(pclient2, SIGNAL(debugPurpose()), waitPrivateMessage, SLOT(quit()));
 waitPrivateMessage->exec();

	qApp->processEvents();
 QCOMPARE(pclient2->receivedMessage, privateMessage);
 QCOMPARE(pclient1->receivedMessage, "");

 pclient0->slotDisconnectFromServer();
 pclient1->slotDisconnectFromServer();
 pclient2->slotDisconnectFromServer();
 pserver->slotStopServer();
 delete pclient0;
 delete pclient1;
 delete pclient2;
 delete pserver;
 delete loop;
// */
}

void test::test_addAllUsersToOfflineModel()
{
 QFile::remove(savingFile);
 QFile::remove(clientSavingFile);

 QString port="4444";
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
 QFile::remove(clientSavingFile);
 QFile::remove(savingFile);

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

void test::test_removeOnlinesFromOfflines()
{
 QFile::remove(savingFile);

 QStringList offlines={"ownernick", "nick1", "nick2", "nick3", "nick4"};
 QStringList receivedOnlines={"ownernick", "nick1", "nick3"};
 ClientServiceForDebug *pclient=new ClientServiceForDebug;
 pclient->setNickAndName("ownernick", "nomatter");
 pclient->setListToOfflineModel(offlines);

 pclient->removeOnlineFromOfflines(receivedOnlines);
 QCOMPARE(pclient->getOfflines().size(), 2);
 QCOMPARE(pclient->getOnlines().size(), 3);
 QCOMPARE(pclient->getOnlines().first(), "Вы: ownernick");
 QCOMPARE(pclient->getOnlines().last(), "nick3");
 QCOMPARE(pclient->getOfflines().contains("ownernick"), false);
}

void test::test_removeConnectedFromOfflines()
{
 ClientServiceForDebug *pclient=new ClientServiceForDebug;
 QStringList onlines={"nick1", "nick2"};
 QStringList offlines={"nick3", "nick4"};
 pclient->setListToOnlineModel(onlines);
 pclient->setListToOfflineModel(offlines);
 pclient->removeConnectedFromOfflines("nick4");
 QCOMPARE(pclient->getOfflines().size(), 1);
 QCOMPARE(pclient->getOfflines().at(0), "nick3");

 pclient->removeConnectedFromOfflines("nick4");
 QCOMPARE(pclient->getOfflines().size(), 1);
 QCOMPARE(pclient->getOfflines().at(0), "nick3");

 delete pclient;
}

void test::test_throwOnlinesToOfflines()
{
 QFile::remove(savingFile);

 ClientServiceForDebug *pclient=new ClientServiceForDebug;
 pclient->setNickAndName("ownernick", "nomatter");

 QStringList offlines={"nick2", "nick4"};
 QStringList receivedOnlines={"ownernick", "nick1", "nick3"};
 pclient->setListToOfflineModel(offlines);
 pclient->setListToOnlineModel(receivedOnlines);

 pclient->throwOnlinesToOfflines();
 QCOMPARE(pclient->getOnlines().size(), 0);
 QCOMPARE(pclient->getOfflines().size(), 5);
 QCOMPARE(pclient->getOfflines().first(), "Вы: ownernick");
}

void test::test_throwDisconnectedToOfflines()
{
 ClientServiceForDebug *pclient=new ClientServiceForDebug;
 QStringList onlines={"nick1", "nick2"};
 QStringList offlines={"nick3", "nick4"};
 pclient->setListToOnlineModel(onlines);
 pclient->setListToOfflineModel(offlines);
 pclient->throwDisconnectedToOfflines("nick1");
 QCOMPARE(pclient->getOnlines().size(), 1);
 QCOMPARE(pclient->getOfflines().size(), 3);
 QCOMPARE(pclient->getOnlines().first(), "nick2");
 QCOMPARE(pclient->getOfflines().at(0), "nick3");
 QCOMPARE(pclient->getOfflines().at(1), "nick4");
 QCOMPARE(pclient->getOfflines().at(2), "nick1");

 pclient->throwDisconnectedToOfflines("nick1");
 QCOMPARE(pclient->getOnlines().size(), 1);
 QCOMPARE(pclient->getOfflines().size(), 3);
 QCOMPARE(pclient->getOnlines().first(), "nick2");
 QCOMPARE(pclient->getOfflines().at(0), "nick3");
 QCOMPARE(pclient->getOfflines().at(1), "nick4");
 QCOMPARE(pclient->getOfflines().at(2), "nick1");

 delete pclient;
}

QTEST_MAIN(test)

#include "tst_test.moc"
