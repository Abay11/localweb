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

 QIODevice* outputDev;

 QBuffer* buffer;
public:
 IOHandler();

 void startRecording();

signals:
 void readData(QByteArray&);

 private slots:
 void slotReadAudioInput();

 public slots:
 void slotWriteAudioOutput(QByteArray& data);
};

#endif // IOHANDLER_H
