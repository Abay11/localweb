#include "callingwindow.h"
#include "ui_callingwindow.h"

CallingWindow::CallingWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CallingWindow)
{
 ui->setupUi(this);
}

CallingWindow::~CallingWindow()
{
 delete ui;
}

void CallingWindow::setState(const QString& nick, CallingWindow::STATES state)
{
 QLabel* nickname = ui->nickname;

 QLabel* status = ui->status;

 QLabel* picture = ui->picture;

 QLabel* timer =ui->timer;

 QWidget* acceptingWidget = ui->acceptingControl;

 QWidget* speakingWidget = ui->speakingControl;

 nickname->setText(nick);

 switch (state)
	{
	case STATES::INCOMING:
	 {
		status->setText("ВХОДЯЩИЙ ВЫЗОВ");

		setImage(IMAGES::INCOMING);

		timer->setHidden(true);

		acceptingWidget->setVisible(true);

		speakingWidget->setVisible(false);

		break;
	 }

	case STATES::OUTGOING:
	 {
		status->setText("ИСХОДЯЩИЙ ВЫЗОВ");

		setImage(IMAGES::OUTGOING);

		timer->setHidden(true);

		acceptingWidget->setVisible(false);

		speakingWidget->setVisible(true);

		break;
	 }

	case STATES::SPEAKING:
	 {
		status->setText("ИДЕТ БЕСЕДА");

		setImage(IMAGES::CONNECTED);

		timer->setHidden(false);

		acceptingWidget->setVisible(false);

		speakingWidget->setVisible(true);
	 }

	}
}

void CallingWindow::setImage(CallingWindow::IMAGES image)
{
 QPixmap pixmap;
 bool isPixmapLoaded = false;

 switch (image) {
	case IMAGES::INCOMING:
	 {
		isPixmapLoaded = pixmap.load(":/Res/Icons/incomingCall.png");
		break;
	 }

	case IMAGES::OUTGOING:
	 {
		isPixmapLoaded = pixmap.load(":/Res/Icons/outgoingCall.png");
		break;
	 }

	case IMAGES::CONNECTED:
	 {
		isPixmapLoaded = pixmap.load(":/Res/Icons/connectedCall.png");
		break;
	 }

	case IMAGES::ACCEPTCALL:
	 {
		isPixmapLoaded = pixmap.load(":/Res/Icons/acceptCall.png");
		break;
	 }

	case IMAGES::REJECTCALL:
	 {
		isPixmapLoaded = pixmap.load(":/Res/Icons/rejectCall.png");
		break;
	 }
	}

 if(isPixmapLoaded)
	ui->picture->setPixmap(pixmap);
 else
	qWarning() << DTAG << "Couldn't load a pixmap";
}
