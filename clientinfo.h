#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>

enum DATATYPE{REGISTRATION=0, CONNECT, MESSAGE};

class ClientSimpleInfo
{
private:
 QString maddress;
 int mport;
 DATATYPE mtype;

public:
 ClientSimpleInfo(QString, int, DATATYPE);
 virtual ~ClientSimpleInfo();

 const QString& address();
 int port();
 DATATYPE type() const;
};

class ClientRegInfo:public ClientSimpleInfo
{
 QString mnickname;
 QString mfullName;

public:
 ClientRegInfo(QString
							 ,int
							 ,DATATYPE
							 ,QString
							 ,QString
							 );

virtual ~ClientRegInfo();

 const QString& nickname() const;

 const QString& fullName();
};

#endif // CLIENTINFO_H
