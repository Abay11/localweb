#include "clientinfo.h"

ClientInfo::ClientInfo(QString fullname
											 ,QString address,
											 QString port,
											 bool status)
 :mfullName(fullname)
 ,maddress(address)
 ,mport(port)
 ,mstatus(status)
 {}

ClientInfo::~ClientInfo(){}

const QString& ClientInfo::fullName()const{return mfullName;}
QString& ClientInfo::fullName(){return mfullName;}

const QString& ClientInfo::address()const{return maddress;}
QString& ClientInfo::address(){return maddress;}

const QString& ClientInfo::port()const{return mport;}
QString& ClientInfo::port(){return mport;}

const bool &ClientInfo::status() const
{
 return mstatus;
}

bool &ClientInfo::status()
{
 return mstatus;
}

quint16 ClientInfo::audioPort() const
{
 return audioPort_;
}

quint16 &ClientInfo::audioPort()
{
 return audioPort_;
}

QDataStream &operator>>(QDataStream &out, ClientInfo *&cinfo)
{
 cinfo=new ClientInfo;
 return out>>cinfo->fullName()>>cinfo->address()>>cinfo->port()>>cinfo->status();
}

QDataStream &operator<<(QDataStream &out, const ClientInfo *const&cinfo)
{
 return out<<cinfo->fullName()<<cinfo->address()<<cinfo->port()<<cinfo->status();
}

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
