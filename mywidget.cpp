#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
 :QWidget(parent)
 ,plblAddress(new QLabel("IP адрес:"))
 ,plblPort(new QLabel("Номер порта:"))
 ,pleAddress(new QLineEdit)
 ,plePort(new QLineEdit("7165"))
 ,pInfo(new QTextEdit)
 ,pcmdOn(new QPushButton("Пуск"))
 ,pcmdOff(new QPushButton("Стоп"))
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
 ,pvalidator(new QIntValidator(1000, 65535, plePort))
 ,pserver(new QTcpServer(this))
 ,logger(new MyLogger)
{
 plePort->setValidator(pvalidator);

 pcmdOff->setEnabled(false);

 pInfo->setText(QDateTime::currentDateTime().toString("[hh:mm:ss] ")+"Сервер не запущен.");
 pInfo->setReadOnly(true);

 foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
	 if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
		m_address=address.toString();
	}

 pleAddress->setText(m_address);
 pleAddress->setReadOnly(true);
 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdOn);
 phlay->addWidget(pcmdOff);

 pvlay->addLayout(phlay);
 pvlay->addWidget(pInfo);

 connect(pcmdOn, SIGNAL(clicked()), SLOT(slotStartServer()));
 connect(pcmdOff, SIGNAL(clicked()), SLOT(slotStopServer()));

 setLayout(pvlay);
 resize(640, 480);
}

void MyWidget::slotStartServer()
{
 QString nPort=plePort->text();
 int pos=0;
 if(pvalidator->validate(nPort, pos)!=QValidator::Acceptable)
	{
	 pInfo->setTextColor(Qt::red);
	 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")+"Некорректный адрес порта.");
	 pInfo->setTextColor(Qt::black);
	 pcmdOff->setEnabled(false);
	 return;
	}

 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")+"Сервер запущен.");

 pcmdOff->setEnabled(true);
 pcmdOn->setEnabled(false);

 if(!pserver->listen(QHostAddress::Any,
								 static_cast<quint16>(plePort->text().toInt())))
	{
	 qCritical()<<"Невозможно запустить сервер: "<<pserver->errorString();
	 pserver->close();
	 return;
	}

 plePort->setEnabled(false);


 connect(pserver, SIGNAL(newConnection()),
				 SLOT(slotNewConnection()));

 qInfo()<<QString("Сервер запущен. IP адрес: %1. Порт: %2")
					 .arg(m_address).arg(pserver->serverPort());
}

void MyWidget::slotStopServer()
{
 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")+"Сервер остановлен.");
 pcmdOn->setEnabled(true);
 pcmdOff->setEnabled(false);

 plePort->setEnabled(true);
 pserver->close();
 qInfo()<<"Сервер остановлен.";
}

void MyWidget::slotNewConnection()
{

}

void MyWidget::slotReadClient()
{

}

MyWidget::~MyWidget()
{
 delete logger;
}
