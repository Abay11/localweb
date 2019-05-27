#include "vclientside.h"

#include "camera.h"
#include "videowidget.h"
#include "vconnectionhandler.h"

using namespace VideoModule;

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
	connect(connHandler, SIGNAL(dataArrived(QByteArray&)),
		video_widget, SLOT(slotDraw(QByteArray&)));

	connHandler->startListen();
}

void ClientSide::startCamera()
{
	if(camera->isCameraAvailable())
		camera->start();
}

void ClientSide::stopCamera()
{
	camera->stop();
}

QWidget* ClientSide::getVideoWidget()
{
	return video_widget;
}

void ClientSide::setDestination(const QHostAddress& host, quint16 port)
{
	connHandler->setDestination(host, port);
}

quint16 ClientSide::getPort()
{
	return connHandler->getPort();
}
