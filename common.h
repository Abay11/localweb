#ifndef COMMONUTILITYFUNCS_H
#define COMMONUTILITYFUNCS_H

#include <QTime>

const qint64 BUFFER_SIZE=1024;

namespace MY_NETWORK
{
  static QString DOWNLOAD_PATH = "./Downloads/";
}

QString formatTimeToString(const QTime &time);

#endif // COMMONUTILITYFUNCS_H
