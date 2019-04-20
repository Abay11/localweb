#ifndef CALLINGWINDOW_H
#define CALLINGWINDOW_H

#include <QWidget>
#include <QBitmap>
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
};

#endif // CALLINGWINDOW_H
