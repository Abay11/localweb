#include "clientinfo.h"

ClientInfo::ClientInfo(QString address, int port, QString name)
 :maddress(address)
 ,mport(port)
 ,mname(name)
{

}

const QString& ClientInfo::address(){return maddress;}

int ClientInfo::port(){return mport;}

const QString& ClientInfo::name(){return mname;}
