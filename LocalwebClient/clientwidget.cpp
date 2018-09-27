#include "clientwidget.h"

ClientWidget::ClientWidget(MyLogger *logger, QWidget *parent)
 : QWidget(parent)
 ,mnNextBlockSize(0)
 ,plblAddress(new QLabel("IP адрес сервера"))
 ,pleAddress(new QLineEdit("127.0.0.1"))
 ,plblPort(new QLabel("Номер порта сервера"))
 ,plePort(new QLineEdit("7165"))
 ,pcmdConnect(new QPushButton("Подключиться"))
 ,pcmdDisconnect(new QPushButton("Отсоединиться"))
 ,pcmdSend(new QPushButton("Отправить"))
 ,pcmdRegistration(new QPushButton("Зарегаться"))
 ,pInfo(new QTextEdit)
 ,pmsgField(new QTextEdit)
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
 ,psocket(new QTcpSocket)
 ,plogger(logger)
{
// foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
//	{
//	 if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
//		maddress=address.toString();
//	}

 pleAddress->setText(pleAddress->text());
 pmsgField->setPlaceholderText("Введите сообщение...");
 pcmdDisconnect->setEnabled(false);

 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdConnect);
 phlay->addWidget(pcmdDisconnect);
 pvlay->addLayout(phlay);
 pvlay->addWidget(pInfo);
 pvlay->addWidget(pmsgField);
 pvlay->addWidget(pcmdSend);
 pvlay->addWidget(pcmdRegistration);

 connect(pcmdConnect, SIGNAL(clicked()),
				 SLOT(slotConnectToServer()));
 connect(pcmdDisconnect, SIGNAL(clicked()),
				 SLOT(slotDisconnectFromServer()));
 connect(pcmdSend, SIGNAL(clicked())
				 ,SLOT(slotSendToServer()));
 connect(psocket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(psocket, SIGNAL(error(QAbstractSocket::SocketError))
				 ,SLOT(slotError(QAbstractSocket::SocketError)));
 connect(psocket, SIGNAL(readyRead())
				 ,SLOT(slotReadyRead()));
 connect(pcmdRegistration, SIGNAL(clicked())
				 ,SLOT(slotRegistration()));


 setLayout(pvlay);
 resize(640, 480);
}

void ClientWidget::slotConnectToServer()
{
 psocket->connectToHost(pleAddress->text(),
												static_cast<quint16>(plePort->text()
																						 .toInt()));
 pcmdDisconnect->setEnabled(true);
 pcmdConnect->setEnabled(false);

 qDebug()<<"Sizeof QTcpSocket "<<sizeof(psocket)<< " "<<sizeof(*psocket);
 qDebug()<<"Sizeof QString "<<sizeof(QString);

}

void ClientWidget::slotConnected()
{
 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Соединение с сервером установлено.");
 qInfo()<<"Соединение с сервером установлено.";

 qDebug()<<psocket->localPort();
 qDebug()<<psocket->localAddress();
}

void ClientWidget::slotDisconnectFromServer()
{
 psocket->close();
 pInfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Отсоединение от сервера.");
 pcmdConnect->setEnabled(true);
 pcmdDisconnect->setEnabled(false);
	qInfo()<<"Соединение с сервером разорвано.";
}

void ClientWidget::slotReadyRead()
{
 QDataStream in(psocket) ;
 in.setVersion(QDataStream::Qt_5_11);
 forever
 {
	if(!mnNextBlockSize)
	 {
		if(static_cast<size_t>(psocket->bytesAvailable())<sizeof(quint16))
		 break;

		in>>mnNextBlockSize;
	 }

	if(psocket->bytesAvailable()<mnNextBlockSize)
	 break;

	QTime time;
	QString msg;

	in>>time>>msg;

	pInfo->append(time.toString()+" "+msg);
	mnNextBlockSize=0;
 }
}

void ClientWidget::slotError(QAbstractSocket::SocketError nerr)
{
 QString info("Произошла ошибка при подключении к серверу: ");
 switch(nerr)
	{
	case QAbstractSocket::HostNotFoundError:
	 info.append("Удаленный сервер не найден. "
							 "Удостоверьте, что введен правильный адрес и порт сервера.");
	 break;
	case QAbstractSocket::RemoteHostClosedError:
	 info.append("Удаленный сервер закрыл соединение. "
							 "Обратитесь к администратору сети.");
	 break;
	case QAbstractSocket::ConnectionRefusedError:
	 info.append("Удаленный сервер отказал в доступе. "
							 "Обратитесь к администратору сети.");
	 break;
	default:
	 info.append("Произошла критическая ошибка. "
							 "Обратитесь к администратору сети.");
	}
 pInfo->append(info);
 pcmdDisconnect->setEnabled(false);
 pcmdConnect->setEnabled(true);

	qCritical()<<"Ошибка соединения с сервером: "<<psocket->errorString();
}

void ClientWidget::slotSendToServer()
{
 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)<<static_cast<int>(DATATYPE::MESSAGE)<<QTime::currentTime();

 out<<pmsgField->toPlainText();
 pInfo->append(QDateTime::currentDateTime().toString("[hh:MM:ss] Вы: ")
							 + pmsgField->toPlainText());
 pmsgField->clear();

 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

 psocket->write(arrBlock);
}

void ClientWidget::slotRegistration()
{
 qDebug()<<"slotRegistratoin()";

 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);
 out<<quint16(0)<<static_cast<int>(DATATYPE::REGISTRATION);
 out<<QString("testuser");
 out<<QString("My fullname");
 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

 psocket->write(arrBlock);
}

ClientWidget::~ClientWidget()
{
 delete plogger;
}
