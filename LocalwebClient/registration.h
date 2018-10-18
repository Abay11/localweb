#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QLayoutItem>
#include <QApplication>

#include <QTcpSocket>
#include <QTime>

#include "../clientinfo.h"
#include "../settingsdialog.h"

class MyLogger;

class Registration:public QWidget
{
 Q_OBJECT
private:
 QLineEdit *pleNick, *pleName;
 QPushButton *pcmdRegister, *pcmdExit, *pcmdSettings, *pcmdHelp;
 QHBoxLayout *phlay;
 QVBoxLayout *pvlay;
 QGridLayout *pglay;
 QFormLayout *pflay;

 MyLogger *plogger;

 QTcpSocket *psocket;

 bool mstatus;
public:
 Registration(MyLogger *logger,
							QWidget *parent=nullptr);

public slots:
 void slotRegister();
 void slotError(QAbstractSocket::SocketError);
 void slotExit();
 void slotSettings();

signals:
 void registered(int index);

};

#endif // REGISTRATION_H
