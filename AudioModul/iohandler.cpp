#include "iohandler.h"


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
 if (!inputInfo.isFormatSupported(format))
 {
		 qWarning() << "Default format not supported, trying to use the nearest.";
		 format = inputInfo.nearestFormat(format);
 }

 audioInput = new QAudioInput(format, this);

 buffer = new QBuffer(this);
 buffer->open(QIODevice::ReadWrite);

}

void IOHandler::startRecording()
{
 qDebug() << "Start recording...";

 inputDev = audioInput->start();

 connect(inputDev, SIGNAL(readyRead()), SLOT(slotReadAudioInput()));
}

void IOHandler::slotReadAudioInput()
{
 QByteArray data = inputDev->readAll();

 emit readData(data);

 qDebug() << "Read inputDev:" << data.size();
}

void IOHandler::slotWriteAudioOutput(QByteArray &data)
{
 qDebug() << "Size of writing data to output:" << data.size();
 outputDev->write(data);
}
