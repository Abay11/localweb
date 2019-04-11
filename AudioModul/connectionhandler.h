#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QUdpSocket>
#include <QTimer>

class ConnectionHandler : public QObject
{
 Q_OBJECT

 private:
 QUdpSocket* socket;

 QHostAddress serverhost;

 quint16 server_port;

 quint16 binding_port = 5000;

public:
 explicit ConnectionHandler(quint16 server_port, const QHostAddress& serverhost = QHostAddress("127.0.0.1"), QObject* parent=nullptr);

 void startListen();

 void stopListen();

signals:
 void dataArrived(QByteArray&);

private slots:
 void slotReadDataFrom();

public slots:
 void slotWriteDataTo(QByteArray&);



};

#endif // CONNECTIONHANDLER_H
