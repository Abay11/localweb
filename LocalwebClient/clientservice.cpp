#include "clientservice.h"

ClientService::ClientService(QObject *prnt)
 :QObject (prnt)
 ,psocket(new QTcpSocket)
 ,pserverAddress(new QString)
 ,pserverPort(new QString)
 ,ponline(new QStringListModel)
 ,poffline(new QStringListModel)
{
 connect(psocket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(psocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));


}

void ClientService::slotConnected()
{

}

void ClientService::slotReadyRead()
{

}

void ClientService::slotError(QAbstractSocket::SocketError)
{

}

void ClientService::slotConnectToServer()
{
 psocket->connectToHost(*pserverAddress,
												static_cast<quint16>(pserverPort->toInt()));
}

void ClientService::slotDisconnectFromServer()
{
 psocket->close();

// ponline->removeRows(0, ponline->rowCount()-1);
}

void ClientService::slotSendToServer()
{

}

void ClientService::slotSetAddress(QString addr, QString port)
{

}
