#include "callingwindow.h"
#include "ui_callingwindow.h"

CallingWindow::CallingWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CallingWindow)
{
 ui->setupUi(this);

 connect(ui->cmdTurnMicro, SIGNAL(clicked()), SLOT(slotTurnMicro()));
 connect(ui->cmdTurnSpeakers, SIGNAL(clicked()), SLOT(slotTurnSpeakers()));
}

CallingWindow::~CallingWindow()
{
 delete ui;
}

void CallingWindow::setState(const QString& nick, CallingWindow::STATES state)
{
 QLabel* nickname = ui->nickname;

 QLabel* status = ui->status;

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

		break;
	 }

	case STATES::REJECTING:
	 {
		status->setText("Клиент отклонил вызов");

		setImage(IMAGES::REJECTCALL);

		timer->setHidden(true);

		acceptingWidget->setVisible(false);

		speakingWidget->setVisible(false);

		QTimer::singleShot(3000, this, SLOT(close()));

		break;
	 }

	case STATES::NOTREACH:
	 {
		status->setText("Не удалось дозвониться");

		setImage(IMAGES::REJECTCALL);

		timer->setHidden(true);

		acceptingWidget->setVisible(false);

		speakingWidget->setVisible(false);

		QTimer::singleShot(3000, this, SLOT(close()));

		break;
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

void CallingWindow::slotTurnMicro()
{
 QPushButton* btn = ui->cmdTurnMicro;

 static bool isTurnOn = true;

 isTurnOn = !isTurnOn;

 if(isTurnOn)
	{
	 btn->setText("Выключить микрофон");
	 btn->setPalette(QPalette(QColor("#8d6c9f")));
	}
 else
	{
	 btn->setText("Включить микрофон");
	 btn->setPalette(QPalette(QColor("#81d6bb")));
	}

 emit turnMicro();
}

void CallingWindow::slotTurnSpeakers()
{
 QPushButton* btn = ui->cmdTurnSpeakers;

 static bool isTurnOn = true;

 isTurnOn = !isTurnOn;

 if(isTurnOn)
	{
	 btn->setText("Выключить динамики");
	 btn->setPalette(QPalette(QColor("#8d6c9f")));
	}
 else
	{
	 btn->setText("Включить динамики");
	 btn->setPalette(QPalette(QColor("#81d6bb")));
	}

 emit turnSpeakers();
}
