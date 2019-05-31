#include "videowidget.h"
#include "camera.h"

#include <QBuffer>
#include <QImageReader>
#include <QDebug>

#include <private/qvideoframe_p.h>

using namespace VideoModule;

VideoWidget::VideoWidget(QWidget* parent)
	: QLabel(parent)
{
	resize(400, 400);
}

void VideoWidget::setCameraInstance(Camera* cam)
{
	this->cam = cam;
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::slotDraw(QByteArray& data)
{
	//TODO
	//optimize

	QBuffer buff(&data);
	buff.open(QIODevice::ReadOnly);
	QImage img;

	QImageReader reader;
	reader.setDevice(&buff);
	reader.setFormat("jpg");
	reader.read(&img);

	if(size != img.size())
	{
		size = img.size();
		resize(size);
	}

	setPixmap(QPixmap::fromImage(img));
}

void VideoWidget::closeEvent(QCloseEvent* event)
{
	QWidget::closeEvent(event);

	if(cam)
		cam->stop();

	emit videoWidgetClosed(caller_nick);
}

void VideoWidget::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);

	if(!cam->start())
	{
		qWarning() << DTAG << "A camera not found";
	}
}
