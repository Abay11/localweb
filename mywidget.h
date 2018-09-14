#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>

class MyWidget : public QWidget
{
 Q_OBJECT
private:
 QLabel* plblPort;
 QLineEdit* plePort;
 QTextEdit* pInfo;
 QPushButton* pcmdOn;
 QPushButton* pcmdOff;
 QHBoxLayout* phlay;
 QVBoxLayout* pvlay;

public:
 MyWidget(QWidget* parent=nullptr);
 ~MyWidget();
};

#endif // MYWIDGET_H
