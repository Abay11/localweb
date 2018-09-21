#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>
#include <QDataStream>
#include <QTcpSocket>

enum class DATATYPE:char{
 REGISTRATION=0,
 DELETION,
 CONNECT,
 MESSAGE
};



class ClientInfo
{
private:
 QString mnickname;
 QTcpSocket* psocket;
 QString mfullName;
 QString maddress;
 int mport;

public:
 ClientInfo(QString nickname, QTcpSocket* psocket, QString info);
 virtual ~ClientInfo();

 const QString& nickname() const;
 QString& nickname();

 QTcpSocket** socket();

 const QString& address()const;
 QString& address();
 int port() const;
 int& port();
 DATATYPE type() const;
 const QString& fullName()const;
 QString& fullName();
};

QDataStream& operator<<(QDataStream& out, const ClientInfo& cinfo);
QDataStream& operator>>(QDataStream& out, ClientInfo& cinfo);

QDataStream& operator<<(QDataStream& out, DATATYPE& type);
QDataStream& operator>>(QDataStream& out, DATATYPE& type);

#endif // CLIENTINFO_H
