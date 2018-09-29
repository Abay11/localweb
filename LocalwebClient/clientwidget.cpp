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
 ,pcmdClear(new QPushButton("Очистить"))
 ,pinfo(new QTextEdit)
 ,pmsgField(new QTextEdit)
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
 ,psplitter(new QSplitter(this))
 ,plist(new QListWidget(this))
 ,pserverSocket(new QTcpSocket)
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
 pcmdSend->setEnabled(false);
 pinfo->setReadOnly(true);

 plist->addItem(new QListWidgetItem("Николай Петренко"));

 psplitter->addWidget(new QWidget(psplitter));
 psplitter->addWidget(new QWidget(psplitter));
 psplitter->setStretchFactor(0, 1);
 psplitter->setStretchFactor(1, 2);

 QVBoxLayout *ptemplayout0 = new QVBoxLayout(psplitter);
 ptemplayout0->addWidget(new QLabel("Пользователи"));
 ptemplayout0->addWidget(plist);

 psplitter->widget(0)->setLayout(ptemplayout0);

 phlay->addWidget(plblAddress);
 phlay->addWidget(pleAddress);
 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdConnect);
 phlay->addWidget(pcmdDisconnect);
 pvlay->addLayout(phlay);
 pvlay->addWidget(pinfo);
 pvlay->addWidget(pmsgField);
 QHBoxLayout *ptemplayout1=new QHBoxLayout;
 ptemplayout1->addWidget(pcmdSend);
 ptemplayout1->addWidget(pcmdClear);
 pvlay->addLayout(ptemplayout1);

 psplitter->widget(1)->setLayout(pvlay);


 connect(pmsgField, SIGNAL(textChanged()), SLOT(slotMsgChanged()));
 connect(pcmdConnect, SIGNAL(clicked()),
				 SLOT(slotConnectToServer()));
 connect(pcmdDisconnect, SIGNAL(clicked()),
				 SLOT(slotDisconnectFromServer()));
 connect(pcmdSend, SIGNAL(clicked())
				 ,SLOT(slotSendToServer()));
 connect(pcmdClear, SIGNAL(clicked()),
				 SLOT(slotClearMsg()));
 connect(pserverSocket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(pserverSocket, SIGNAL(error(QAbstractSocket::SocketError))
				 ,SLOT(slotError(QAbstractSocket::SocketError)));
 connect(pserverSocket, SIGNAL(readyRead())
				 ,SLOT(slotReadyRead()));

 QHBoxLayout *ptemplayout2=new QHBoxLayout;
 ptemplayout2->addWidget(psplitter);
 setLayout(ptemplayout2);
 resize(640, 480);
}

void ClientWidget::slotConnectToServer()
{
 pserverSocket->connectToHost(pleAddress->text(),
												static_cast<quint16>(plePort->text()
																						 .toInt()));
 pcmdDisconnect->setEnabled(true);
 pcmdConnect->setEnabled(false);
}

void ClientWidget::slotConnected()
{
 pinfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Соединение с сервером установлено.");
 qInfo()<<"Соединение с сервером установлено.";

 //запрос серверу чтобы тот прислал список клиентов
 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)<<static_cast<int>(DATATYPE::CONNECT);
 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

 pserverSocket->write(arrBlock);
 pinfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Отправлен запрос на получение списка клиентов.");
 qInfo()<<"Отправлен запрос на получение списка";

}

void ClientWidget::slotMsgChanged()
{
 pcmdSend->setEnabled(!pmsgField->toPlainText().isEmpty()
											&&pserverSocket->isOpen());
}

void ClientWidget::slotClearMsg()
{
 pmsgField->clear();
}

void ClientWidget::slotDisconnectFromServer()
{
 pserverSocket->close();
 pinfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
							 +"Отсоединение от сервера.");
 pcmdConnect->setEnabled(true);
 pcmdDisconnect->setEnabled(false);
	qInfo()<<"Соединение с сервером разорвано.";
}

void ClientWidget::slotReadyRead()
{
 QDataStream in(pserverSocket) ;
 in.setVersion(QDataStream::Qt_5_11);
 forever
 {
	if(!mnNextBlockSize)
	 {
		if(static_cast<size_t>(pserverSocket->bytesAvailable())<sizeof(quint16))
		 break;

		in>>mnNextBlockSize;
	 }

	if(pserverSocket->bytesAvailable()<mnNextBlockSize)
	 break;

	QTime time;
	QString msg;

	DATATYPE type;
	in>>type;

	switch (type) {
	 case DATATYPE::CONNECT:
	 {
		 //получение списка пользователей от сервера
		 in>>clients;

		 qDebug()<<"Received client's list";
//		 for(auto a:clients)
//			qDebug()<<
		 break;
	 }
	 case DATATYPE::MESSAGE:
	 {
		 //новое сообщение
		 in>>time>>msg;
		 msg=time.toString("[hh:mm:ss] ")+"Новое сообщение: "+msg;
		 pinfo->append(msg);
		 break;
	 }
	 default:
		//что-то пошло не так, клиент не может получить иную команду
		qCritical()<<"Неизвестная ошибка при получении сообщения.";
	 }

	mnNextBlockSize=0;
 }
}

void ClientWidget::slotError(QAbstractSocket::SocketError nerr)
{
 QString info;
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
 QMessageBox::critical(this, "Ошибка подключения к серверу",
											 info);
 pcmdDisconnect->setEnabled(false);
 pcmdConnect->setEnabled(true);

	qCritical()<<"Ошибка соединения с сервером: "<<pserverSocket->errorString();
	pserverSocket->close();
}

void ClientWidget::slotSendToServer()
{
 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)<<static_cast<int>(DATATYPE::MESSAGE)<<QTime::currentTime();

 out<<pmsgField->toPlainText();
 pinfo->append(QDateTime::currentDateTime().toString("[hh:MM:ss] Вы: ")
							 + pmsgField->toPlainText());
 pmsgField->clear();

 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

 pserverSocket->write(arrBlock);
}

ClientWidget::~ClientWidget()
{
 delete plogger;
}
