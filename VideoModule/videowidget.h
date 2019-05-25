#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QLabel>
#include <QVideoFrame>

class VideoWidget : public QLabel
{
	Q_OBJECT
public:
	VideoWidget(QWidget* parent = nullptr);

	~VideoWidget();

public slots:
	void slotDraw(QByteArray& data);

private:
	QSize size;

	const QString DTAG = "VideoModule::VideoWidget:";

};

#endif // VIDEOWIDGET_H
