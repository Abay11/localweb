#ifndef COMMONUTILITYFUNCS_H
#define COMMONUTILITYFUNCS_H

#include <QString>

class QTime;

const qint64 BUFFER_SIZE = 1024;

const quint16 AUDIO_VIDEO_SERVER_IP = 16016;

namespace MY_NETWORK
{
	static QString DOWNLOAD_PATH = "./Downloads/";
}

QString formatTimeToString(const QTime& time);

#endif // COMMONUTILITYFUNCS_H
