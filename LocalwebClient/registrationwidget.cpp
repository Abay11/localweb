#include "registrationwidget.h"

#include <QSizePolicy>

Registration::Registration(ClientService *pserv, QWidget *parent)
 :QWidget (parent)
 ,pleNick(new QLineEdit)
 ,pleName(new QLineEdit)
 ,pcmdRegister(new QPushButton("Ок"))
 ,pcmdExit(new QPushButton("Выход"))
 ,pcmdSettings(new QPushButton("Настройки подключения"))
 ,pcmdHelp(new QPushButton("Помощь"))
 ,phlay(new QHBoxLayout)
 ,pflay(new QFormLayout)
 ,psocket(new QTcpSocket)
 ,pservice(pserv)
{
 pcmdSettings->setToolTip("Задать настройки");

 phlay->addWidget(pcmdRegister);
 phlay->addWidget(pcmdSettings);
 phlay->addWidget(pcmdExit);
 phlay->addWidget(pcmdHelp);
 phlay->setAlignment(Qt::AlignRight);

 pflay->addRow(new QLabel("Введите псевдоним"), pleNick);
 pflay->addRow(new QLabel("Введите Ваше имя"), pleName);
 pflay->addRow(phlay);
 setLayout(pflay);
 setWindowTitle("Регистрация");

 connect(pcmdRegister, SIGNAL(clicked()), SLOT(slotRegister()));
 connect(pcmdExit, SIGNAL(clicked()), SLOT(slotExit()));
 connect(pcmdSettings, SIGNAL(clicked()), SLOT(slotSettings()));

 QApplication::setQuitOnLastWindowClosed(true);
}

void Registration::slotRegister()
{
 if(pleNick->text().isEmpty())
	{
	 QMessageBox::warning(this, "Заполните данные",
												"Поле \"Псевдоним\" не может быть пустым");
	 return;
	}

 if(pleName->text().isEmpty())
	{
	 QMessageBox::warning(this, "Заполните данные",
												"Поле \"Имя\" не может быть пустым");
	 return;
	}

 if(!pservice->socketIsOpen())
	{
	 pservice->slotSetAddress(maddress, mport);
	 pservice->slotConnectToServer();
	}

// psocket=new QTcpSocket;
// psocket->connectToHost(maddress,
//												static_cast<quint16>(mport.toInt()));

 pservice->slotSentToServer(DATATYPE::REGISTRATION, pleNick->text(), QVariant(pleName->text()));

// if(psocket->waitForConnected())
//	{
//	 QByteArray arrBlock;
//	 QDataStream stream(&arrBlock, QIODevice::WriteOnly);
//	 stream.setVersion(QDataStream::Qt_5_11);

//	 stream<<quint16(0)<<static_cast<int>(DATATYPE::REGISTRATION);
//	 stream<<pleNick->text();
//	 stream.device()->seek(0);
//	 stream<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

//	 psocket->write(arrBlock);
//	 psocket->waitForBytesWritten(-1);
//	 psocket->waitForReadyRead(-1);

//	 stream.setDevice(psocket);
//	 quint16 blockSize=0;
//	 forever
//	 {
//		if(!blockSize)
//		 {
//			if(static_cast<size_t>(psocket->bytesAvailable())
//				 <sizeof(quint16))
//			 continue;

//			stream>>blockSize;
//		 }

//		if(psocket->bytesAvailable()!=blockSize)
//		 {
//		 continue;
//		 }

//		DATATYPE type;
//		QString res;
//		QTime time;
//		stream>>type>>time>>res;

//		if(type!=DATATYPE::REGISTRATION)
//		 qCritical()<<"Ожидалась регистрация, вышло что-то другое";

//		qInfo()<<time.toString("[hh:mm:ss] ")
//					<<"Registration attempt. Result: "<<res;

		if(!pservice->getRegistrationResult())
		 {
		 QMessageBox::warning(this, "Ошибка регистрации",
													"Псевдоним уже используется другим пользователем. "
													"Вам нужно использовать другой");
		 return;
		 }

//		break;
//	 }

	 QFile file("data.bin");
	 file.open(QIODevice::WriteOnly);
	 QDataStream fileout(&file);
	 fileout<<pleNick->text()<<pleName->text();
	 file.close();
	 emit(registered(0));

	 psocket->close();

	 QMessageBox::information(this, "Регистрация прошла успешно",
														"Вы успешно зарегистрировались в сети. "
														"Теперь можете подключиться к серверу");

	 deleteLater();
//	}
// else
//	{
//	 psocket->close();
//	 slotError(psocket->error());
//	}
}

/*void Registration::slotError(QAbstractSocket::SocketError nerr)
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
*/

void Registration::slotExit()
{
 QApplication::quit();
}

void Registration::slotSettings()
{
 SettingsWidget *sd=new SettingsWidget;
 sd->setCurrentAddress(maddress, mport);
 sd->show();
 connect(sd, SIGNAL(addressChanged(QString, QString)),
				 SLOT(slotAddressChanged(QString, QString)));
}

void Registration::slotAddressChanged(QString addr, QString port)
{
 maddress=addr;
 mport=port;
 emit addressChanged(addr, port);
}
