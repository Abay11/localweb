#ifndef CLIENTSIDE_H
#define CLIENTSIDE_H

#include <QObject>
#include <QHostAddress>

class Camera;
class VideoWidget;
class ConnectionHandler;
class QVideoFrame;

class ClientSide : public QObject
{
	Q_OBJECT

public:
	explicit ClientSide(quint16 serverPort,
		const QHostAddress& serverAddress = QHostAddress("127.0.0.1"),
		QObject* parent = nullptr);

	void startCamera();

	void stopCamera();

private slots:

private:

	quint16 serverPort;
	QHostAddress serverAddress;
	Camera* camera;
	VideoWidget* video_widget;
	ConnectionHandler* connHandler;
};

#endif // CLIENTSIDE_H
