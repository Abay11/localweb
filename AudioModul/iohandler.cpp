#include "iohandler.h"


IOHandler::IOHandler()
{
 QAudioFormat format;
 format.setSampleRate(8000); // 22050
 format.setSampleSize(8);    // 16
 format.setChannelCount(1);
 format.setCodec("audio/pcm");
 format.setByteOrder(QAudioFormat::LittleEndian);
 format.setSampleType(QAudioFormat::SignedInt);

 QAudioDeviceInfo inputInfo = QAudioDeviceInfo::defaultInputDevice();
 if (!inputInfo.isFormatSupported(format))
 {
		 qWarning() << "Default format not supported, trying to use the nearest.";
		 format = inputInfo.nearestFormat(format);
 }

 audioInput = new QAudioInput(format, this);

 buffer = new QBuffer(this);
 buffer->open(QIODevice::ReadWrite);

 connect(buffer, SIGNAL(readyRead()), SLOT(slotReadAudioInput()));
 audioInput->start(buffer);

// inputDev->open(QIODevice::ReadWrite);

 qDebug() << "Start recording...";

}

void IOHandler::slotReadAudioInput()
{
 QByteArray data = inputDev->readAll();

 qDebug() << "Read inputDev:" << data.size();
}
