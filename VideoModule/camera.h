#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

class QCamera;
class QVideoFrame;
class QVideoProbe;

namespace VideoModule
{

	class Camera : public QObject
	{
		Q_OBJECT

	public:
		Camera(QObject* parent = nullptr);
		~Camera();
		bool isCameraAvailable();

		bool isAvailable();

		void start();

		void stop();

	private:
		void initCamera();


	signals:
		void newFrame(const QByteArray&);

	private slots:
		void slotFrameProbed(const QVideoFrame& frame);

	private:
		QCamera* camera;
		QVideoProbe* video_probe;

		const QString DTAG = "VideoModule::Camera: ";
	};
}

#endif // CAMERA_H
