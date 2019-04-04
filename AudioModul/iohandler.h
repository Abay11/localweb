#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <QAudioInput>
#include <QAudioOutput>
#include <QDebug>
#include <QBuffer>

class IOHandler : public QObject
{
 Q_OBJECT

private:
 QAudioInput* audioInput;

 QAudioOutput* audioOutput;

 QIODevice* inputDev;

 QIODevice* outputDev;

 QBuffer* buffer;

public:
 IOHandler();

 void startRecording();

 void stopRecording();

signals:
 void readData(QByteArray&);

 private slots:
 void slotReadAudioInput();

 public slots:
 void slotWriteAudioOutput(QByteArray& data);
};

#endif // IOHANDLER_H
