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

 void startRecording();

 void stopRecording();

signals:
 void readyToSend(QByteArray& data);

private slots:
 void readInput();

public slots:
 void readyToRead(QByteArray& data);
};

#endif // IOHANDLER_H
