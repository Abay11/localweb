#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <QDebug>

class QFile;
class QTextStream;
class QMessageLogContext;
class QString;


class MyLogger
{
private:
	static QFile plogFile;
	static QTextStream out;
public:
	MyLogger();
	~MyLogger();
	static void myMessageOutputHandler(QtMsgType, const QMessageLogContext&, const QString&);
};



#endif // MYLOGGER_H
