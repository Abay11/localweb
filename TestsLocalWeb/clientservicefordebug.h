#ifndef CLIENTNETWORKSERVICEIMPLFORDEBUG_H
#define CLIENTNETWORKSERVICEIMPLFORDEBUG_H

#include "../LocalwebClient/clientservice.h"

#include <QTimer>
#include <QThread>

class ClientServiceForDebug : public ClientService
{
 Q_OBJECT
public:
 ClientServiceForDebug(QWidget *parent=nullptr);
 bool getRegistrationResult();
 const CLIENTBASE *getClientBase();
 void addAllUsersToOfflineModel();
 void addNewOnlineToModel(QString nick);
 void removeOnlineFromOfflines(QStringList onlines);
 void setListToOnlineModel(QStringList onlines);
 void setListToOfflineModel(QStringList offlines);
 void throwOnlinesToOfflines();
 void throwDisconnectedToOfflines(QString disconnected);
 QStringList getOnlines();
 QStringList getOfflines();

private slots:
 void slotReadyRead();

 signals:
 void debugPurpose();

};

#endif // CLIENTNETWORKSERVICEIMPLFORDEBUG_H
