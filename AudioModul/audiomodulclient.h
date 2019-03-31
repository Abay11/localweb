#ifndef AUDIOMODULCLIENT_H
#define AUDIOMODULCLIENT_H

#include <networkhandler.h>
#include <iohandler.h>

class AudioModulClient : public QObject
{
 Q_OBJECT

 NetworkHandler* connection;

 IOHandler* iodev;

 int type;
public:
 AudioModulClient(QString address, quint16 port, int type);

 ~AudioModulClient();

 void start();

 void stop();

};

#endif // AUDIOMODULCLIENT_H
