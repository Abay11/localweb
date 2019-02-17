#ifndef FTP_CLIENT_H
#define FTP_CLIENT_H

#include "ftp_server.h"

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QThread>

class FtpClient : public QObject
{
 Q_OBJECT

private:
 QString serverAddress;

 quint16 port;

 quint64 expectedSize=0;

 QTcpSocket *socket;

public:
 FtpClient(const QString &serverAddress, quint16 port);

 virtual ~FtpClient();

 void connectToServer();

 void disconnect();

 void upload(const QString &path, const QString &filename);

 bool isFinished=false;

signals:
 void uploadingIsFinished();

public slots:
 void slotConnected();
 void slotDownload();
 void slotError(QAbstractSocket::SocketError);
};

#endif // FTP_CLIENT_H
