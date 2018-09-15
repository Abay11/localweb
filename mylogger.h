#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <iostream>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class MyLogger
{
private:
 static QFile plogFile;
 static QTextStream out;
public:
 MyLogger();
 ~MyLogger();
 static void myMessageOutputHandler(QtMsgType,const QMessageLogContext&,const QString&);
};



#endif // MYLOGGER_H
