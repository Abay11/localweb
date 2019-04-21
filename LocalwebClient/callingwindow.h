#ifndef CALLINGWINDOW_H
#define CALLINGWINDOW_H

#include <QWidget>
#include <QBitmap>
#include <QTimer>
#include <QDebug>

namespace Ui {
 class CallingWindow;
}

class CallingWindow : public QWidget
{
 Q_OBJECT

 const QString DTAG = "CallingWindow:";

public:
 explicit CallingWindow(QWidget *parent = nullptr);
 ~CallingWindow();

 enum class STATES
 {
	INCOMING,
	OUTGOING,
	SPEAKING,
	REJECTING,
	NOTREACH,
 };

 void setState(const QString& nick, STATES state);

private:
 Ui::CallingWindow *ui;

 enum class IMAGES{
	ACCEPTCALL,
	REJECTCALL,
	CONNECTED,
	INCOMING,
	OUTGOING
 };

 void setImage(IMAGES image);

private slots:
 void slotTurnMicro();

signals:
 void turnMicro();

};

#endif // CALLINGWINDOW_H
