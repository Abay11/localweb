#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>
#include <QDataStream>
#include <QTcpSocket>

enum class DATATYPE:char{
 REGISTRATION=0,
 CONNECT,
 MESSAGE
};

QDataStream& operator<<(QDataStream& out, DATATYPE& type);
QDataStream& operator>>(QDataStream& out, DATATYPE& type);

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
 const QString& address()const;
 QString& address();
 int port() const;
 int& port();
 DATATYPE type() const;
 const QString& fullName();
};

#endif // CLIENTINFO_H
