#include "camera.h"

#include <QCameraInfo>
#include <QVideoProbe>
#include <QDataStream>
#include <QImageWriter>
#include <QBuffer>

#include <private/qvideoframe_p.h>

using namespace VideoModule;

Camera::Camera(QObject* parent)
	: QObject(parent)
{
	if(!isCameraAvailable())
	{
		qWarning() << "A camera not found!";
		return;
	}

	camera = new QCamera;

	video_probe = new QVideoProbe;
	video_probe->setSource(camera);

	connect(video_probe, SIGNAL(videoFrameProbed(const QVideoFrame&)),
		SLOT(slotFrameProbed(const QVideoFrame&)));

	camera->setCaptureMode(QCamera::CaptureMode::CaptureVideo);
}

Camera::~Camera()
{
}

bool Camera::isCameraAvailable()
{
	if(QCameraInfo::availableCameras().count() > 0)
		return true;
	else
		return false;
}

void Camera::start()
{
	camera->start();
}

void Camera::stop()
{
	camera->stop();
}

void Camera::slotFrameProbed(const QVideoFrame& frame)
{
	qDebug() << DTAG << "A frame probed";
	QVideoFrame copy_frame(frame);
	copy_frame.map(QAbstractVideoBuffer::ReadOnly);
	QImage img = qt_imageFromVideoFrame(copy_frame);
	copy_frame.unmap();

	//TODO:
	//optimize
	QByteArray bytes;

	QByteArray format;
	QBuffer buffer(&bytes);

	QImageWriter writer; // use to compress img's size
	writer.setDevice(&buffer);
	writer.setFormat("jpg");
	writer.setQuality(50);


	if(!writer.write(img))
	{
		qWarning() << DTAG << "Couldn't write img to device using a writer:" << writer.errorString();
	}

	qDebug() << DTAG <<  "size of img" << bytes.size();

	emit newFrame(bytes);
}
