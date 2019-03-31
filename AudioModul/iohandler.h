#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QCoreApplication>

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

 void startRecording(int type);

 void stopRecording();

signals:
 void readySend(QByteArray data);

private slots:
 void readInput();

public slots:
 void slotReadyRead(QByteArray data);
};

#endif // IOHANDLER_H
