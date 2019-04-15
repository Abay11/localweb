#ifndef SERVERSIDE_H
#define SERVERSIDE_H

#include <QObject>
#include <QUdpSocket>
#include <QVector>
#include <QPair>

#include <QDebug>

static const QString DTAG("AudioModule::ServerSide:");

class ServerSide : public QObject
{
 Q_OBJECT

public:
 explicit ServerSide(quint16 listeningPort, QObject *parent = nullptr);

 bool startListening();

 void stopListening();

 void setClients(QVector<QPair<QHostAddress, quint16>>& clients);

private:
 quint16 listeningPort;

 QUdpSocket* socket;

 QVector<QPair<QHostAddress, quint16>> connectedClients;

 void sendIndividual(QByteArray& data, const QHostAddress&, quint16);

 void sendBroadcast(QByteArray& data);

signals:

private slots:
 void slotReadyRead();

public slots:
};

#endif // SERVERSIDE_H
