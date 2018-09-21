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
 ,m_nextBlockSize(0)
{
 plePort->setValidator(pvalidator);

 pcmdOff->setEnabled(false);

 pInfo->setText(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
								+"Сервер не запущен.");
 pInfo->setReadOnly(true);

 foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
	 if (address.protocol() == QAbstractSocket::IPv4Protocol
			 && address != QHostAddress(QHostAddress::LocalHost))
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

MyWidget::~MyWidget()
{
 delete logger;
}

void sendToClient(QTcpSocket* psocket,
									const QString& msg)
{
 QByteArray byteArr;
 QDataStream out(&byteArr, QIODevice::WriteOnly);
 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)<<QTime::currentTime()<<msg;
 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(byteArr.size())-sizeof (quint16));

 psocket->write(byteArr);
}

void MyWidget::updateClientBase(ClientInfo& pinfo)
{
 ClientInfo* temp=dynamic_cast<ClientInfo*>(&pinfo);
 if(temp) //means need to register a new user
	{
	 addNewUser(*temp);
	 return;
	}

 //else we just need only update client's ip and port
 auto it=clientbase.find(pinfo.nickname());
 (*it)->address()=pinfo.address();
 (*it)->port()=pinfo.port();
}

void MyWidget::addNewUser(ClientInfo& pclient)
{
 ClientInfo* temp =new ClientInfo(
		dynamic_cast<ClientInfo&>(pclient));

 clientbase.insert(temp->nickname(), std::move(temp));
}

void MyWidget::removeUser(QString nickname)
{
 clientbase.remove(nickname);
}

void MyWidget::slotStartServer()
{
 QString nPort=plePort->text();
 int pos=0;
 if(pvalidator->validate(nPort, pos)!=QValidator::Acceptable)
	{
	 pInfo->setTextColor(Qt::red);
	 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
								 +"Некорректный адрес порта.");
	 pInfo->setTextColor(Qt::black);
	 pcmdOff->setEnabled(false);
	 return;
	}

 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Сервер запущен.");

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
 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Сервер остановлен.");
 pcmdOn->setEnabled(true);
 pcmdOff->setEnabled(false);

 plePort->setEnabled(true);
 pserver->close();
 qInfo()<<"Сервер остановлен.";
}

void MyWidget::slotNewConnection()
{
	QTcpSocket* pclient=pserver->nextPendingConnection();
	connect(pclient, SIGNAL(disconnected), SLOT(deleteLater()));
	connect(pclient, SIGNAL(readyRead()), SLOT(slotReadClient()));
}

void MyWidget::slotReadClient()
{
 QTcpSocket* pclient=static_cast<QTcpSocket*>(sender());
 QDataStream in(pclient);
 in.setVersion(QDataStream::Qt_5_11);

 forever
 {
	if(!m_nextBlockSize)
	 {
		if(pclient->bytesAvailable()<static_cast<qint64>(sizeof(quint16)))
		 break;

		in>>m_nextBlockSize;
	 }


	if(pclient->bytesAvailable()<m_nextBlockSize)
	 break;

	QString str;
	QTime time;

	DATATYPE type;
	in>>type;

	switch (type) {
	 case DATATYPE::MESSAGE:
	 {
		 in>>time>>str;
		 str=time.toString("[hh:mm:ss] ")+"Сообщение от пользователя: "+str;
		 pInfo->append(str);
		 break;
	 }
	 default:
		qDebug()<<"INCORRECT DATATYPE";
	 }

	m_nextBlockSize=0;
 }
}
