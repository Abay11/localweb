#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include <QTime>
#include <QMap>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QStringListModel>
#include <QListWidget>
#include <QMessageBox>
#include <QFile>
#include <QLabel>

#include "../clientinfo.h"

class ClientService : public QObject
{
 Q_OBJECT

private:
 QWidget *pparent;
 QString nick;
 QString name;

 bool registrationResult=false;

 quint16 mnNextBlockSize=0;
 QTcpSocket *psocket;
 QString *pserverAddress, *pserverPort;

 QStringListModel *ponlineModel, *pofflineModel;
 QMap<QString, ClientInfo*> clients;

 void saveDataAndProperties();
 void restoreDataAndProperties();
 void addAllUsersToOfflineModel();
 void removeOnlinesFromOfflines(QStringList onlines);
 void throwOnlinesToOfflines();

public:
 ClientService(QWidget *parent=nullptr);
 ~ClientService();
 bool socketIsOpen();
 QStringListModel *onlineModel();
 QStringListModel *offlineModel();

 void setNickAndName(QString nick, QString name);

private slots:
 void slotConnected();
 void slotReadyRead();
 void slotError(QAbstractSocket::SocketError);

public slots:
 void slotConnectToServer();
 void slotDisconnectFromServer();
 void slotSentToServer(DATATYPE, QString="", QVariant=0);
 void slotSetAddress(QString addr, QString port);


signals:
 void connected();
 void disconnected();
 void returnRegistrationResult(bool);
};

#endif // CLIENTSERVICE_H
