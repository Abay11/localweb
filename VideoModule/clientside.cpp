#include "clientside.h"

#include "camera.h"
#include "videowidget.h"
#include "connectionhandler.h"

ClientSide::ClientSide(quint16 serverPort,
	const QHostAddress& serverAddress,
	QObject* parent)
	: QObject(parent)
	, serverPort(serverPort)
	, serverAddress(serverAddress)
	, camera(new Camera(this))
	, video_widget(new VideoWidget)
	, connHandler(new ConnectionHandler(serverPort, serverAddress, this))
{
	//writing frames
	connect(camera, SIGNAL(newFrame(const QByteArray&)),
		connHandler, SLOT(slotWriteData(const QByteArray&)));

	//reading frames
	connect(connHandler, SIGNAL(dataArrived(const QByteArray&)),
		video_widget, SLOT(slotDraw(const QByteArray&)));

	connHandler->startListen();
}

void ClientSide::startCamera()
{
	camera->start();
}

void ClientSide::stopCamera()
{
	camera->stop();
}
