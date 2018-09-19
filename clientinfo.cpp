#include "clientinfo.h"

ClientSimpleInfo::ClientSimpleInfo(QString nick
																	 ,QString address
																	 ,int port
																	 ,DATATYPE type)
 :mnickname(nick)
 ,maddress(address)
 ,mport(port)
 ,mtype(type){}

ClientSimpleInfo::~ClientSimpleInfo(){}

const QString& ClientSimpleInfo::nickname()const{return mnickname;}

const QString& ClientSimpleInfo::address()const{return maddress;}
QString& ClientSimpleInfo::address(){return maddress;}

int ClientSimpleInfo::port()const{return mport;}
int& ClientSimpleInfo::port(){return mport;}

DATATYPE ClientSimpleInfo::type()const{return mtype;}


////////////////////Client Registration Information////////////////////


ClientRegInfo::ClientRegInfo(QString nick
														 ,QString address
														 ,int port
														 ,DATATYPE type
														 ,QString fullName
														 )
 :ClientSimpleInfo(nick, address, port, type)
 ,mfullName(fullName){}

ClientRegInfo::~ClientRegInfo(){}


const QString& ClientRegInfo::fullName(){return mfullName;}
