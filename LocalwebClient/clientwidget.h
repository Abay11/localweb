#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>


#include <QTcpSocket>
#include <QNetworkInterface>

class ClientWidget : public QWidget
{
 Q_OBJECT

private:
 QString m_address;

 QLabel* plblAddress;
 QLineEdit* pleAddress;
 QLabel* plblPort;
 QLineEdit* plePort;
 QPushButton* pcmdConnect;
 QPushButton* pcmdDisconnect;
 QTextEdit* pInfo;

 QHBoxLayout* phlay;
 QVBoxLayout* pvlay;

public:
 ClientWidget(QWidget *parent = 0);
 ~ClientWidget();
};

#endif // CLIENTWIDGET_H
