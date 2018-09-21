#include "clientinfo.h"

QDataStream& operator<<(QDataStream& out, DATATYPE& type)
{
 int i=static_cast<int>(type);
 return out<<i;
}

QDataStream& operator>>(QDataStream& out, DATATYPE& type)
{
 int c;
 out>>c;
 type=DATATYPE(static_cast<char>(c));
 return out;
}

ClientInfo::ClientInfo(QString nick
											 ,QTcpSocket* sock
											 ,QString info)
 :mnickname(nick)
 ,psocket(sock)
 ,mfullName(info)
 {}

ClientInfo::~ClientInfo(){}

const QString& ClientInfo::nickname()const{return mnickname;}

const QString& ClientInfo::address()const{return maddress;}
QString& ClientInfo::address(){return maddress;}

int ClientInfo::port()const{return mport;}
int& ClientInfo::port(){return mport;}

const QString& ClientInfo::fullName(){return mfullName;}
