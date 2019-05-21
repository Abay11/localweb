#ifndef CAMERA_H
#define CAMERA_H

#include <QLabel>
#include <QWidget>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QVideoFrame>
#include <QVideoProbe>
#include <QTimer>
#include <QVideoWidget>

#include <QMediaPlayer>

class VideoWidget;

class Camera : public QObject
{
	Q_OBJECT

public:
	Camera(QObject* parent = 0);
	~Camera();

private slots:
	void frameProbed(const QVideoFrame&);


private:
	bool isCameraAvailable();

	VideoWidget* video_widget;

	QCamera* camera;
	QCameraImageCapture* imageCapture;
	QCameraViewfinder* viewfinder;
	QLabel* label;
};

#endif // CAMERA_H
