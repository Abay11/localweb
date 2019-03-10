#ifndef FTP_SERVER_H
#define FTP_SERVER_H

#include "ftp_common.h"
#include "../../common.h"

#include <QObject>
#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDir>
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

 bool uploading(QTcpSocket *client, const QString &path, const QString &filename);

 void downloading(QTcpSocket *client, const QString &path, const QString &filename);

public slots:
 void slotNewConnection();

 void slotReadClient();

 void slotAcceptError(QAbstractSocket::SocketError socketError);
};

#endif // FTP_SERVER_H
