#ifndef COMMON_H
#define COMMON_H

#include <QString>


const qint8 DOWNLOAD = 1;
const qint8 UPLOAD = 2;

const qint8 OK = 1;
const qint8 FILE_NOT_FOUND = 2;
const qint8 FAIL = 2;

int const BUFFER_SIZE = 5242880; // 5MB

qint64 const INIT_MSG_SIZE = sizeof(char) + sizeof(QString);

#endif // COMMON_H
