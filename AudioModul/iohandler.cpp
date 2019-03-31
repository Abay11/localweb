#include "iohandler.h"

IOHandler::IOHandler(QObject* parent)
 :QObject (parent)
{
 QAudioFormat defaultFormat;
	// Set up the format, eg.
	defaultFormat.setSampleRate(8000);
	defaultFormat.setChannelCount(1);
	defaultFormat.setSampleSize(8);
	defaultFormat.setCodec("audio/pcm");
	defaultFormat.setByteOrder(QAudioFormat::LittleEndian);
	defaultFormat.setSampleType(QAudioFormat::UnSignedInt);

	QAudioDeviceInfo inputInfo(QAudioDeviceInfo::defaultOutputDevice());

	QAudioFormat inputFormat = defaultFormat;

	if (!inputInfo.isFormatSupported(inputFormat)) {
			qWarning() << "Default format not supported, trying to use the nearest.";

			inputFormat = inputInfo.nearestFormat(inputFormat);
	}

	microphone = new QAudioInput(inputFormat, this);

	QAudioFormat outputFormat = defaultFormat;

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	if (!info.isFormatSupported(outputFormat)) {
			qWarning() << "Raw audio format not supported by backend, cannot play audio.";
	}

	speakers = new QAudioOutput(outputFormat, this);

	inputBuffer = new QBuffer;
	connect(inputBuffer, SIGNAL(readyRead()), SLOT(readInput()));

	outputBuffer = new QBuffer;
}

void IOHandler::startRecording(int type)
{
 qInfo() << "Start recording...";

 if(type == 1)
	{
 inputBuffer->open(QIODevice::ReadWrite);

 microphone->start(inputBuffer);
	}

 outputBuffer->open(QIODevice::ReadWrite);

 speakers->start(outputBuffer);
}

void IOHandler::stopRecording()
{
 qInfo() << "Stop recording";

 microphone->stop();

 speakers->stop();
}

void IOHandler::readInput()
{
 qApp->processEvents();

 QByteArray data = inputBuffer->readAll();

 qDebug() << "IOHandler: Input buffer size:" << data.size();

 emit readySend(data);
}

void IOHandler::slotReadyRead(QByteArray data)
{
 outputBuffer->write(data);
}
