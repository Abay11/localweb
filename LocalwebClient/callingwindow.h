#ifndef CALLINGWINDOW_H
#define CALLINGWINDOW_H

#include <QWidget>

class QTimer;

namespace Ui
{
	class CallingWindow;
}

class CallingWindow : public QWidget
{
	Q_OBJECT

	const QString DTAG = "CallingWindow:";

public:
	explicit CallingWindow(QWidget* parent = nullptr);
	~CallingWindow();

	enum class STATES
	{
		INCOMING,
		OUTGOING,
		SPEAKING,
		REJECTING,
		NOTREACH,
		STOPPING
	};

	void setState(const QString& nick, STATES state);

	QString getCurrentNick();

private:
	Ui::CallingWindow* ui;

	QTimer* timer;

	enum class IMAGES
	{
		ACCEPTCALL,
		REJECTCALL,
		CONNECTED,
		INCOMING,
		OUTGOING
	};

	void setImage(IMAGES image);

private slots:
	void slotTurnMicro();

	void slotTurnSpeakers();

	void slotPutDown();

	void slotCallAccepted();

	void slotCallRejected();

signals:
	void turnMicroClicked();

	void turnSpeakersClicked();

	void putDownClicked(QString);

	void callAccepted(QString);

	void callRejected(QString);
};

#endif // CALLINGWINDOW_H
