#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QUdpSocket>
//#include <

class ConnectionHandler : public QObject
{
 Q_OBJECT

 private:
 QUdpSocket* socket;

 QHostAddress host;

 quint16 port;

public:
 explicit ConnectionHandler(quint16 port, const QHostAddress& host = QHostAddress("localhost"), QObject* parent=nullptr);

 void startListen();

signals:
 void dataArrived(QByteArray&);

private slots:
 void slotReadDataFrom();

public slots:
 void slotWriteDataTo(QByteArray&);



};

#endif // CONNECTIONHANDLER_H
