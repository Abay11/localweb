#ifndef CLIENTSIDE_H
#define CLIENTSIDE_H

#include <QObject>
#include <QHostAddress>

#include "camera.h"
#include "videowidget.h"
#include "vconnectionhandler.h"

//class VideoWidget;
//class ConnectionHandler;

class QVideoFrame;
class QLabel;

namespace VideoModule
{
	class ClientSide : public QObject
	{
		Q_OBJECT

	public:
		explicit ClientSide(quint16 serverPort,
			const QHostAddress& serverAddress = QHostAddress("127.0.0.1"),
			QObject* parent = nullptr);

		void startCamera();

		void stopCamera();

		QWidget* getVideoWidget();

		void setDestination(const QHostAddress& host, quint16 port);

	private slots:

	private:

		quint16 serverPort;
		QHostAddress serverAddress;
		VideoModule::Camera* camera;
		VideoWidget* video_widget;
		ConnectionHandler* connHandler;
	};
}

#endif // CLIENTSIDE_H
