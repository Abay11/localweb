#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>

class QDataStream;

enum class DATATYPE : char
{
 REGISTRATION = 0,
 DELETION,
 DISCONNECTION,
 NOTIFYING,
 CONNECT,
 MESSAGE,
 FILE,
 GETACTUALDATA,
 CALLINGREQUEST,
 CALLINGRESPONSE,
 STOPCALLING,
 VIDEOCALL,
 STOPVIDEOCALL
};

class ClientInfo
{
private:
	QString mfullName;

	QString maddress;

	QString mport;

	quint16 audioPort_;

	quint16 video_port;

	bool mstatus = false;

public:
	ClientInfo(QString fullname = QString(),
   QString addr = QString(),
   QString mport = QString(),
   bool status = false);

	virtual ~ClientInfo();

	const QString& fullName()const;

	QString& fullName();

	const QString& address()const;

	QString& address();

	const QString& port() const;

	QString& port();

	const bool& status() const;

	bool& status();

	quint16 audioPort() const;

	quint16& audioPort();

	quint16 videoPort() const;

	quint16& videoPort();
};

QDataStream& operator>>(QDataStream& out, ClientInfo*& cinfo);

QDataStream& operator<<(QDataStream& out, const ClientInfo* const& cinfo);

QDataStream& operator<<(QDataStream& out, DATATYPE& type);

QDataStream& operator>>(QDataStream& out, DATATYPE& type);

#endif // CLIENTINFO_H
