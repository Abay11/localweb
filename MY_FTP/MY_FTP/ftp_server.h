#ifndef FTP_SERVER_H
#define FTP_SERVER_H

#include <QObject>
#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
#include <QDebug>

#include <QTcpServer>
#include <QTcpSocket>

class FtpServer : public QObject
{
 Q_OBJECT

private:
 QTcpServer *server;

 quint16 port;

 qint64 expectedSize=0;

 bool isStateRunning=false;

public:
 FtpServer();

 virtual ~FtpServer();

 bool start(quint16 port);

 void stop();

 bool isRunning(){return isStateRunning;}

public slots:
 void slotNewConnection();

 void slotReadClient();

 void slotAcceptError(QAbstractSocket::SocketError socketError);
};

#endif // FTP_SERVER_H
