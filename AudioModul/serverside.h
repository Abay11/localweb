#ifndef SERVERSIDE_H
#define SERVERSIDE_H

#include <QObject>
#include <QUdpSocket>
#include <QVector>
#include <QPair>

#include <QDebug>

static QString DTAG("AudioModule::ServerSide:");

class ServerSide : public QObject
{
 Q_OBJECT
private:
 quint16 listeningPort;

 QUdpSocket* socket;

 QVector<QPair<QHostAddress, quint16>> connectedClients;

 void startListening();

 void insertClient(const QHostAddress& host, quint16 port);

 void sendIndividual(QByteArray& data, QHostAddress&, quint16);

 void sendBroadcast(QByteArray& data);

public:
 explicit ServerSide(quint16 listeningPort, QObject *parent = nullptr);

signals:

private slots:
 void slotReadyRead();

public slots:
};

#endif // SERVERSIDE_H
