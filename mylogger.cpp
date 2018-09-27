#include "mylogger.h"

QFile MyLogger::plogFile("log.txt");
QTextStream MyLogger::out(&plogFile);

MyLogger::MyLogger()
{
 plogFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
 out.setCodec("UTF-8");
 qInstallMessageHandler(myMessageOutputHandler);
}

MyLogger::~MyLogger()
{
 if(plogFile.isOpen())
	{
	plogFile.close();
	}
}

void MyLogger::myMessageOutputHandler(QtMsgType type
														,const QMessageLogContext& c
														,const QString& msg
														)
{
 QByteArray localMsg=msg.toUtf8();
 switch(type)
	{
	case QtDebugMsg:
	 std::cout<<QString("Debug: %1 (%2:%3, %4)\n")
				.arg(localMsg.constData())
				.arg(c.file)
				.arg(c.line)
				.arg(c.function).toStdString()
					 <<std::endl;
	 break;

	case QtInfoMsg:
	 out<<QDateTime::currentDateTime().toString("[dd/MM/yy hh:mm:ss] ")
		 <<QString("Info: %1\n")
				.arg(localMsg.constData());
	 break;

	case QtWarningMsg:
	 out<<QDateTime::currentDateTime().toString("[dd/MM/yy hh:mm:ss] ")
		 <<QString("Warning: %1\n")
				.arg(localMsg.constData());
	 break;

	case QtCriticalMsg:
	 out<<QDateTime::currentDateTime().toString("[dd/MM/yy hh:mm:ss] ")
		 <<QString("Critical: %1\n")
				.arg(localMsg.constData());
	 break;

	case QtFatalMsg:
	 out<<QDateTime::currentDateTime().toString("[dd/MM/yy hh:mm:ss] ")
		 <<QString("Fatal: %1\n")
				.arg(localMsg.constData());
	 break;
	}
 out.flush();
}
