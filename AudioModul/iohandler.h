#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <QAudioInput>
#include <QDebug>
#include <QBuffer>

class IOHandler : public QObject
{
 Q_OBJECT

private:
 QAudioInput* audioInput;

 QIODevice* inputDev;

 QBuffer* buffer;
public:
 IOHandler();

 void startRecording();

signals:

 public slots:
 void slotReadAudioInput();
};

#endif // IOHANDLER_H
