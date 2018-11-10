#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include <QTime>
#include <QMap>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QStringListModel>
#include <QMessageBox>

#include "../clientinfo.h"

class ClientService : public QObject
{
 Q_OBJECT

private:
 QWidget *pparent;

 quint16 mnNextBlockSize=0;
 QTcpSocket *psocket;
 QString *pserverAddress, *pserverPort;

 QStringListModel *ponline, *poffline;
 QMap<QString, ClientInfo*> clients;

 void save();
 void restore();

public:
 ClientService(QWidget *parent=nullptr);

private slots:
 void slotConnected();
 void slotReadyRead();
 void slotError(QAbstractSocket::SocketError);

public slots:
 void slotConnectToServer();
 void slotDisconnectFromServer();
 void slotSentToServer(QString);
 void slotSetAddress(QString addr, QString port);
};

#endif // CLIENTSERVICE_H
