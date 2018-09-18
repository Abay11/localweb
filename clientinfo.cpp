#include "clientinfo.h"

ClientSimpleInfo::ClientSimpleInfo(QString nick, DATATYPE type)
 :mnickname(nick)
 ,mtype(type){}

const QString& ClientSimpleInfo::nickname()const{return mnickname;}

DATATYPE ClientSimpleInfo::type()const{return mtype;}

ClientRegInfo::ClientRegInfo(QString nick
														 ,DATATYPE type
														 ,QString address
														 ,int port
														 ,QString fullName
														 )
 :ClientSimpleInfo(nick, type)
 ,maddress(address)
 ,mport(port)
 ,mfullName(fullName)
{}

const QString& ClientRegInfo::address(){return maddress;}

int ClientRegInfo::port(){return mport;}

const QString& ClientRegInfo::fullName(){return mfullName;}
