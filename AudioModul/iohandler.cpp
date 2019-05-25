#include "iohandler.h"

#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>

IOHandler::IOHandler()
{
	QAudioFormat format;
	format.setSampleRate(8000); // 22050
	format.setSampleSize(8);    // 16
	format.setChannelCount(1);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);

	QAudioDeviceInfo inputInfo = QAudioDeviceInfo::defaultInputDevice();
	QAudioFormat inputFormat = format;

	if(!inputInfo.isFormatSupported(inputFormat))
	{
		qWarning() << "Default input format not supported, trying to use the nearest.";

		inputFormat = inputInfo.nearestFormat(inputFormat);
	}

	audioInput = new QAudioInput(inputFormat, this);

	QAudioDeviceInfo outputInfo = QAudioDeviceInfo::defaultOutputDevice();

	if(!outputInfo.isFormatSupported(format))
	{
		qWarning() << "ERROR: Default output format not supported!";
	}

	audioOutput = new QAudioOutput(format, this);
}

void IOHandler::turnOnInput()
{
	inputDev = audioInput->start();

	connect(inputDev, SIGNAL(readyRead()), SLOT(slotReadAudioInput()));
}

void IOHandler::turnOffInput()
{
	audioInput->stop();
}

void IOHandler::turnOnOutput()
{
	outputDev = audioOutput->start();
}

void IOHandler::turnOffOutput()
{
	audioOutput->stop();
}

void IOHandler::slotReadAudioInput()
{
	QByteArray data = inputDev->readAll();

	emit readData(data);

	qDebug() << "Read inputDev:" << data.size();
}

void IOHandler::slotWriteAudioOutput(QByteArray& data)
{
	qDebug() << "Size of writing data to output:" << data.size();

	if(audioOutput->state() != QAudio::State::StoppedState)
		outputDev->write(data);
}
