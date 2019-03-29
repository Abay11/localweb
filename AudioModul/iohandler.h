#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>

#include <QDebug>

class IOHandler : public QObject
{
 Q_OBJECT

 QAudioInput* microphone;

 QAudioOutput* speakers;

 QAudioFormat* format;

 QBuffer* inputBuffer;

 QBuffer* outputBuffer;

public:
 IOHandler(QObject* parent = nullptr);

signals:
 void readyToSend();

public slots:
 void readyToRead();
};

#endif // IOHANDLER_H
