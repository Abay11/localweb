#ifndef CALLINGWINDOW_H
#define CALLINGWINDOW_H

#include <QWidget>

namespace Ui {
 class CallingWindow;
}

class CallingWindow : public QWidget
{
 Q_OBJECT

public:
 explicit CallingWindow(QWidget *parent = nullptr);
 ~CallingWindow();

private:
 Ui::CallingWindow *ui;
};

#endif // CALLINGWINDOW_H
