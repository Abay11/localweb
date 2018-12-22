#include "registrationwidget.h"

#include <QSizePolicy>

Registration::Registration(ClientService *pclientService, QWidget *parent)
 :QWidget (parent)
 ,pleNick(new QLineEdit)
 ,pleName(new QLineEdit)
 ,pcmdRegister(new QPushButton("Ок"))
 ,pcmdExit(new QPushButton("Выход"))
 ,pcmdSettings(new QPushButton("Настройки подключения"))
 ,pcmdHelp(new QPushButton("Помощь"))
 ,phlay(new QHBoxLayout)
 ,pflay(new QFormLayout)
 ,pservice(pclientService)
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

 connect(pcmdRegister, SIGNAL(clicked()), SLOT(slotSentRequest()));
 connect(pcmdExit, SIGNAL(clicked()), SLOT(slotExit()));
 connect(pcmdSettings, SIGNAL(clicked()), SLOT(slotSettings()));
 connect(pservice, SIGNAL(returnRegistrationResult(bool)),
				 SLOT(slotProcessResult(bool)));

 QApplication::setQuitOnLastWindowClosed(true);
}


void Registration::slotSentRequest()
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

 //тут проверка для того чтобы мы не пытались заново подключиться к серверу,
 //при активном соединении.
 //такое будет возможно, нпрм, когда после первого отказа, мы будем пробоваться
 //зарегаться еще(отказ возможен если подобранный ник уже используется)
 if(!pservice->socketIsOpen())
	{
	 pservice->slotConnectToServer();
	 //нужно подождать пока установится соединение,
	 //поэтому блокируемся до нужного сигнала
	 QEventLoop loop;
	 connect(pservice, SIGNAL(connected()), &loop, SLOT(quit()));
	 connect(pservice, SIGNAL(socketError(QString, QString)), &loop, SLOT(quit()));
	 loop.exec();
	}

 if(!pservice->isConnected())
	//device not open (after error)
	return;

 pservice->slotSendToServer(DATATYPE::REGISTRATION, pleNick->text(), QVariant(pleName->text()));
}

void Registration::slotProcessResult(bool registrationResult)
{
 qDebug()<<"Processing Registration Result slot";

		if(!registrationResult)
		 {
		 QMessageBox::warning(this, "Ошибка регистрации",
													"Псевдоним уже используется другим пользователем. "
													"Вам нужно использовать другой");
		 return;
		 }

		if(registrationResult)
		 {
			pservice->setNickAndName(pleNick->text(), pleName->text());
			pservice->appendOwnerUserToOnlines();
		 }

		emit(registrationFinished());
		QMessageBox::information(this, "Регистрация прошла успешно",
														 "Вы успешно зарегистрировались в сети");
	 deleteLater();
}

void Registration::slotExit()
{
 QApplication::quit();
}

void Registration::slotSettings()
{
 SettingsWidget *sd=new SettingsWidget;
 sd->setCurrentAddress(pservice->serverAddress(), pservice->serverPort());
 connect(sd, SIGNAL(addressChanged(QString, QString)),
				 SLOT(slotAddressChanged(QString, QString)));
 sd->show();
}

void Registration::slotAddressChanged(QString address, QString port)
{
 pservice->setAddress(address);
 pservice->setPort(port);
}
