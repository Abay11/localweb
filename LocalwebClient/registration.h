#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

#include <QTcpSocket>
#include <QTime>

#include "../clientinfo.h"

class MyLogger;

class Registration:public QWidget
{
 Q_OBJECT
private:
 QLabel *pnick, *pname;
 QLineEdit *pleNick, *pleName;
 QPushButton *pcmdRegister, *pcmdExit;
 QHBoxLayout *phlay;
 QVBoxLayout *pvlay;
 QGridLayout *pglay;

 MyLogger *plogger;

 QTcpSocket *psocket;

 bool mstatus;
public:
 Registration(MyLogger *logger, QWidget *parent=nullptr);

 QPushButton* cmdExit();
public slots:
 void slotRegister();
 void slotError(QAbstractSocket::SocketError);
signals:
 void registered(int index);

};

#endif // REGISTRATION_H
