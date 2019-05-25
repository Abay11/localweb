#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <QObject>

class QAudioInput;
class QAudioOutput;
class QIODevice;
class QBuffer;

class IOHandler : public QObject
{
	Q_OBJECT

private:
	const QString DTAG = "AudioModule::IOHandler:";

	QAudioInput* audioInput;

	QAudioOutput* audioOutput;

	QIODevice* inputDev = nullptr;

	QIODevice* outputDev = nullptr;

	QBuffer* buffer;

public:
	IOHandler();

	void turnOnInput();

	void turnOffInput();

	void turnOnOutput();

	void turnOffOutput();

signals:
	void readData(QByteArray&);

private slots:
	void slotReadAudioInput();

public slots:
	void slotWriteAudioOutput(QByteArray& data);
};

#endif // IOHANDLER_H
