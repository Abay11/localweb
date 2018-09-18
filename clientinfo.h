#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>

enum DATATYPE{REGISTRATION=0, CONNECT, MESSAGE};

class ClientSimpleInfo
{
private:
 QString mnickname;
 DATATYPE mtype;
public:
 ClientSimpleInfo(QString, DATATYPE);
 const QString& nickname() const;
 DATATYPE type() const;
};

class ClientRegInfo:public ClientSimpleInfo
{
 QString maddress;
 int mport;
 QString mfullName;
public:
 ClientRegInfo(QString
												,DATATYPE
												,QString
												,int
												,QString
												);

 const QString& address();
 int port();
 const QString& fullName();
};

#endif // CLIENTINFO_H
