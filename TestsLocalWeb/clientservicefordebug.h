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

private slots:
 void slotReadyRead();

 signals:
 void debugPurpose();

};

#endif // CLIENTNETWORKSERVICEIMPLFORDEBUG_H
