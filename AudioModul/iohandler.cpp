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

	connect(inputBuffer, SIGNAL(readRead()), SLOT(readInput()));
}

void IOHandler::startRecording()
{
 microphone->start(inputBuffer);

 speakers->start(outputBuffer);
}

void IOHandler::readInput()
{
 QByteArray data = inputBuffer->readAll();

 emit readyToSend(data);
}

void IOHandler::readyToRead(QByteArray& data)
{
 outputBuffer->write(data);
}
