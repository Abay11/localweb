#include "clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
 : QWidget(parent)

 ,plblAddress(new QLabel("IP адрес"))
 ,pleAddress(new QLineEdit)
 ,plblPort(new QLabel("Номер порта"))
 ,plePort(new QLineEdit("7166"))
 ,pcmdConnect(new QPushButton("Подключиться"))
 ,pcmdDisconnect(new QPushButton("Отсоединиться"))
 ,pInfo(new QTextEdit)
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
{
 foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
	 if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
		m_address=address.toString();
	}
 pleAddress->setText(m_address);

 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdConnect);
 phlay->addWidget(pcmdDisconnect);
 pvlay->addLayout(phlay);
 pvlay->addWidget(pInfo);

 setLayout(pvlay);
 resize(640, 480);
}

ClientWidget::~ClientWidget()
{

}
