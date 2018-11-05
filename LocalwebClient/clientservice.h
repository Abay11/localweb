#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include <QTime>
#include <QMap>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QStringListModel>

class ClientService : public QObject
{
 Q_OBJECT

private:
 quint16 mnNextBlockSize=0;
 QTcpSocket *psocket;
 QString *pserverAddress, *pserverPort;

 QStringListModel *ponline, *poffline;

public:
 ClientService(QObject *parent=nullptr);

private slots:
 void slotConnected();
 void slotReadyRead();
 void slotError(QAbstractSocket::SocketError);

public slots:
 void slotConnectToServer();
 void slotDisconnectFromServer();
 void slotSendToServer();
 void slotSetAddress(QString addr, QString port);
};

#endif // CLIENTSERVICE_H
