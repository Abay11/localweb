#include "camera.h"

#include <videowidget.h>

#include <QCameraInfo>
#include <QVideoSurfaceFormat>

#include <private/qvideoframe_p.h>

Camera::Camera(QObject* parent)
	: QObject(parent)
{
	if(!isCameraAvailable())
	{
		qWarning() << "A camera not found!";
		return;
	}

	camera = new QCamera;

	viewfinder = new QCameraViewfinder();
	viewfinder->show();

	QVideoProbe* probe = new QVideoProbe;
	probe->setSource(camera);

	connect(probe, SIGNAL(videoFrameProbed(const QVideoFrame&)),
		SLOT(frameProbed(const QVideoFrame&)));

	camera->setViewfinder(viewfinder);
	camera->setCaptureMode(QCamera::CaptureMode::CaptureVideo);

	camera->start();

	video_widget = new VideoWidget;
	video_widget->show();
}

Camera::~Camera()
{
}

void Camera::frameProbed(const QVideoFrame& frame)
{
	video_widget->draw(frame);
}

bool Camera::isCameraAvailable()
{
	if(QCameraInfo::availableCameras().count() > 0)
		return true;
	else
		return false;
}
