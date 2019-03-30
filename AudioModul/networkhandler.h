#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QUdpSocket>

class NetworkHandler : public QObject
{
 Q_OBJECT

 QUdpSocket* socket;

 QHostAddress host;

 quint16 port;

public:
 NetworkHandler(const QString& host, quint16 port);

 void startListen();

signals:
 void readyRead(QByteArray& data);

public slots:
 void slotWriteData(QByteArray& data);

 void slotReadyRead();
};

#endif // NETWORKHANDLER_H
