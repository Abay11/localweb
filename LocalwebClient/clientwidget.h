#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include "../mylogger.h"
#include "../clientinfo.h"

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTime>

#include <QTcpSocket>
#include <QNetworkInterface>


class ClientWidget : public QWidget
{
 Q_OBJECT

private:
 QString maddress;
 QString mserverAddress;
 QString mserverPort;
 quint16 mnNextBlockSize;

 QLabel* plblAddress;
 QLineEdit* pleAddress;
 QLabel* plblPort;
 QLineEdit* plePort;
 QPushButton* pcmdConnect;
 QPushButton* pcmdDisconnect;
 QPushButton* pcmdSend;
 QPushButton* pcmdRegistration;
 QTextEdit* pInfo;
 QTextEdit* pmsgField;

 QHBoxLayout* phlay;
 QVBoxLayout* pvlay;

 QTcpSocket* psocket;

 MyLogger* logger;
 ClientInfo* pclientInfo;

public:
 ClientWidget(QWidget *parent = nullptr);
 ~ClientWidget();

public slots:
 void slotConnectToServer();
 void slotDisconnectFromServer();
 void slotReadyRead();
 void slotError(QAbstractSocket::SocketError);
 void slotSendToServer();
 void slotRegistration();
 void slotConnected();
};

#endif // CLIENTWIDGET_H
