#include "registration.h"

#include <QSizePolicy>

Registration::Registration(MyLogger *logger,
													 QDockWidget *ponline,
													 QDockWidget *poffline,
													 QWidget *parent)
 :QWidget (parent)
 ,pleNick(new QLineEdit)
 ,pleName(new QLineEdit)
 ,pcmdRegister(new QPushButton("Ок"))
 ,pcmdExit(new QPushButton("Выход"))
 ,phlay(new QHBoxLayout)
 ,pflay(new QFormLayout)
 ,plogger(logger)
 ,pmonline(ponline)
 ,pmoffline(poffline)
 ,psocket(new QTcpSocket)
{
 pcmdRegister->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
 pcmdExit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


 pflay->addRow(new QLabel("Псевдоним"), pleNick);
 pflay->addRow(new QLabel("Имя"), pleName);
 phlay->addWidget(pcmdRegister);
 phlay->addWidget(pcmdExit);
 pflay->addRow(phlay);
 pflay->setAlignment(Qt::AlignVCenter);

 setLayout(pflay);
 setWindowTitle("Регистрация");

 connect(pcmdRegister, SIGNAL(clicked()), SLOT(slotRegister()));
}

QPushButton *Registration::cmdExit()
{
 return pcmdExit;
}

void Registration::slotRegister()
{
 psocket=new QTcpSocket;
 psocket->connectToHost("localhost", 7165);
 if(psocket->waitForConnected())
	{
	 QByteArray arrBlock;
	 QDataStream stream(&arrBlock, QIODevice::WriteOnly);
	 stream.setVersion(QDataStream::Qt_5_11);

	 stream<<quint16(0)<<static_cast<int>(DATATYPE::REGISTRATION);
	 stream<<pleNick->text();
	 stream.device()->seek(0);
	 stream<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

	 psocket->write(arrBlock);
	 psocket->waitForBytesWritten(-1);
	 psocket->waitForReadyRead(-1);

	 stream.setDevice(psocket);
	 quint16 blockSize=0;
	 forever
	 {
		if(!blockSize)
		 {
			if(static_cast<size_t>(psocket->bytesAvailable())
				 <sizeof(quint16))
			 continue;

			stream>>blockSize;
		 }

		if(psocket->bytesAvailable()!=blockSize)
		 {
		 continue;
		 }

		QString res;
		QTime time;
		stream>>time>>res;

		qInfo()<<time.toString("[hh:mm:ss] ")
					<<"Registration attempt. Result: "<<res;

		if(res=="false")
		 {
		 QMessageBox::warning(this, "Ошибка регистрации",
													"Псевдоним уже используется другим пользователем. "
													"Вам нужно использовать другой");
		 return;
		 }

		break;
	 }

	 QFile file("data.bin");
	 file.open(QIODevice::WriteOnly);
	 QDataStream fileout(&file);
	 fileout<<pleNick->text()<<pleName->text();
	 file.close();
	 emit(registered(0));
	 pmonline->setVisible(true);
	 pmoffline->setVisible(true);

	 psocket->close();

	 QMessageBox::information(this, "Регистрация прошла успешно",
														"Вы успешно зарегистрировались в сети. "
														"Теперь можете подключиться к серверу");
	}
 else
	{
	 psocket->close();
	 slotError(psocket->error());
	}
}

void Registration::slotError(QAbstractSocket::SocketError nerr)
{
 QString info("Произошла следующая ошибка при подключении к серверу: ");
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

 QMessageBox::critical(this, "Ошибка соединения к серверу", info);
 qCritical()<<"Ошибка соединения с сервером: "<<psocket->errorString();
}
