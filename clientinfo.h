#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>

class ClientInfo
{
 QString maddress;
 int mport;
 QString mname;
public:
 ClientInfo(QString addr, int, QString);
 const QString& address();
 int port();
 const QString& name();
};

#endif // CLIENTINFO_H
