#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "mylogger.h"
#include "clientinfo.h"

#include <QMap>

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
#include <QNetworkInterface>



class MyWidget : public QWidget
{
 Q_OBJECT
private:
 QLabel* plblAddress;
 QLabel* plblPort;
 QLineEdit* pleAddress;
 QLineEdit* plePort;
 QTextEdit* pInfo;
 QPushButton* pcmdOn;
 QPushButton* pcmdOff;
 QHBoxLayout* phlay;
 QVBoxLayout* pvlay;
 QValidator* pvalidator;

 QTcpServer* pserver;

 MyLogger* logger;

 QList<QTcpSocket*> clients;
 QString m_address;
 quint16 m_nextBlockSize;

 QMap<QString, ClientInfo*> clientbase;

public:
 MyWidget(QWidget* parent=nullptr);
 ~MyWidget();

 void sendToClient(QTcpSocket*, const QString&);
 void updateClientBase(ClientInfo&);
 void addNewUser(ClientInfo&);
 void removeUser(QString);

private slots:
 void slotStartServer();
 void slotStopServer();
 void slotNewConnection();
 void slotReadClient();
};

#endif // MYWIDGET_H
