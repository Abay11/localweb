#include "videowidget.h"

#include <QPainter>
#include <QVideoSurfaceFormat>
#include <QPaintEvent>

#include <private/qvideoframe_p.h>

VideoWidget::VideoWidget(QWidget* parent)
	: QLabel(parent)
{
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::draw(const QVideoFrame& frame)
{
	if(size != frame.size())
	{
		size = frame.size();
		resize(size);
	}

	QVideoFrame copy_frame(frame);
	copy_frame.map(QAbstractVideoBuffer::ReadOnly);
	QImage temp_img = qt_imageFromVideoFrame(copy_frame);
	copy_frame.unmap();

	setPixmap(QPixmap::fromImage(temp_img));
}
