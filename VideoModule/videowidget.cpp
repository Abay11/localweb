#include "videowidget.h"

#include <QPainter>
#include <QVideoSurfaceFormat>
#include <QPaintEvent>
#include <QDataStream>

#include <private/qvideoframe_p.h>

VideoWidget::VideoWidget(QWidget* parent)
	: QLabel(parent)
{
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::slotDraw(const QByteArray& data)
{
	//TODO
	//optimize
	QDataStream input_stream(data);
	input_stream.setVersion(QDataStream::Qt_5_11);

	QImage img;
	input_stream >> img;

	if(size != img.size())
	{
		size = img.size();
		resize(size);
	}

	setPixmap(QPixmap::fromImage(img));
}
