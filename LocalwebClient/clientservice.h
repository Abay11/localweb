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
 QString mnick="", mname;

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
 void slotSentToServer(DATATYPE, QString="");
 void slotSetAddress(QString addr, QString port);

signals:
 void connected();
 void disconnected();
};

#endif // CLIENTSERVICE_H
