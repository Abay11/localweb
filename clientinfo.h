#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>

enum DATATYPE{REGISTRATION=0, CONNECT, MESSAGE};

class ClientSimpleInfo
{
private:
 QString mnickname;
 QString maddress;
 int mport;
 DATATYPE mtype;

public:
 ClientSimpleInfo(QString, QString, int, DATATYPE);
 virtual ~ClientSimpleInfo();

 const QString& nickname() const;
 const QString& address()const;
 QString& address();
 int port() const;
 int& port();
 DATATYPE type() const;
};

class ClientRegInfo:public ClientSimpleInfo
{
 QString mfullName;

public:
 ClientRegInfo(QString
							 ,QString
							 ,int
							 ,DATATYPE
							 ,QString
							 );

virtual ~ClientRegInfo();

 const QString& fullName();
};

#endif // CLIENTINFO_H
