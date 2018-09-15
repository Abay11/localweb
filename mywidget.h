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
#include <QIntValidator>

#include <QTcpServer>
#include <QTcpSocket>

#include "mylogger.h"

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
 QValidator* pvalidator;

 QTcpServer* pserver;

 MyLogger* logger;

public:
 MyWidget(QWidget* parent=nullptr);
 ~MyWidget();

private slots:
 void slotStartServer();
 void slotStopServer();
};

#endif // MYWIDGET_H
