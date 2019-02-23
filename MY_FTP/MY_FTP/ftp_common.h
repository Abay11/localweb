#ifndef COMMON_H
#define COMMON_H

#include <QString>


const qint8 DOWNLOAD = 1;
const qint8 UPLOAD = 2;

int const BUFFER_SIZE = 5242880; // 5MB

qint64 const INIT_MSG_SIZE = sizeof(char) + sizeof(QString);

#endif // COMMON_H
