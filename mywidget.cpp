#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
 :QWidget(parent)
 ,plblPort(new QLabel("Номер порта:"))
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
//	 m_p
	}

}

void MyWidget::slotStopServer()
{
 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")+"Сервер остановлен.");
 pcmdOn->setEnabled(true);
 pcmdOff->setEnabled(false);

 qInfo()<<"Сервер остановлен.";
}

MyWidget::~MyWidget()
{
 delete logger;
}
