#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>
#include <QDataStream>
#include <QTcpSocket>

enum class DATATYPE:char{
 REGISTRATION=0,
 DELETION,
 DISCONNECTION,
 NOTIFYING,
 CONNECT,
 MESSAGE
};

class ClientInfo
{
private:
 QString mfullName;
 QString maddress;
 QString mport;
 bool mstatus=false;

public:
 ClientInfo(QString fullname=QString(),
						QString addr=QString(),
						QString mport=QString(),
						bool status=false);
 virtual ~ClientInfo();

 const QString& nickname() const;
 QString& nickname();

 const QString& fullName()const;
 QString& fullName();
 const QString& address()const;
 QString& address();
 const QString& port() const;
 QString& port();
 const bool& status() const;
 bool& status();
};

QDataStream& operator>>(QDataStream& out, ClientInfo *&cinfo);
QDataStream& operator<<(QDataStream& out, const ClientInfo *const&cinfo);

QDataStream& operator<<(QDataStream& out, DATATYPE& type);
QDataStream& operator>>(QDataStream& out, DATATYPE& type);

#endif // CLIENTINFO_H
