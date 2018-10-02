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
#include <QDockWidget>

#include <QTcpSocket>
#include <QTime>

#include "../clientinfo.h"

class MyLogger;

class Registration:public QWidget
{
 Q_OBJECT
private:
 QLineEdit *pleNick, *pleName;
 QPushButton *pcmdRegister, *pcmdExit;
 QHBoxLayout *phlay;
 QVBoxLayout *pvlay;
 QGridLayout *pglay;
 QFormLayout *pflay;

 MyLogger *plogger;
 QDockWidget *pmonline;
 QDockWidget *pmoffline;

 QTcpSocket *psocket;

 bool mstatus;
public:
 Registration(MyLogger *logger,
							QDockWidget *ponline,
							QDockWidget *poffline,
							QWidget *parent=nullptr);

 QPushButton* cmdExit();
public slots:
 void slotRegister();
 void slotError(QAbstractSocket::SocketError);
signals:
 void registered(int index);

};

#endif // REGISTRATION_H
