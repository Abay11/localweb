#include "clientinfo.h"

ClientSimpleInfo::ClientSimpleInfo(QString address
																	 ,int port
																	 ,DATATYPE type)
 :maddress(address)
 ,mport(port)
 ,mtype(type){}

ClientSimpleInfo::~ClientSimpleInfo(){}

const QString& ClientSimpleInfo::address(){return maddress;}

int ClientSimpleInfo::port(){return mport;}

DATATYPE ClientSimpleInfo::type()const{return mtype;}


////////////////////Client Registration Information////////////////////


ClientRegInfo::ClientRegInfo(QString address
														 ,int port
														 ,DATATYPE type
														 ,QString nick
														 ,QString fullName
														 )
 :ClientSimpleInfo(address, port, type)
 ,mnickname(nick)
 ,mfullName(fullName){}

ClientRegInfo::~ClientRegInfo(){}

const QString& ClientRegInfo::nickname()const{return mnickname;}

const QString& ClientRegInfo::fullName(){return mfullName;}
