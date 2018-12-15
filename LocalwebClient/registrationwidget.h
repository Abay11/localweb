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
#include "../settingswidget.h"
#include "clientservice.h"

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

 QTcpSocket *psocket;
 ClientService *pservice;

 QString address="127.0.0.1";
 QString port="7777";
public:
 Registration(ClientService *pservice, QWidget *parent=nullptr);

private slots:
 void slotSentRequest();
 void slotProcessResult(bool registrationResult);
 void slotExit();
 void slotSettings();
 void slotAddressChanged(QString, QString);

public slots:

signals:
 void registrationFinished();
 void addressChanged(QString address, QString port);
 void nickAndNameChanged(QString nick, QString name);
};

#endif // REGISTRATION_H
