#include "settingswidget.h"

#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QRegExpValidator>
#include <QLabel>
#include <QDebug>

SettingsWidget::SettingsWidget(QWidget* prnt)
	: QDialog(prnt)
	, pleAddress(new QLineEdit)
	, plePort(new QLineEdit)
	, pcmdOk(new QPushButton("OK"))
	, pcmdCancel(new QPushButton("Отмена"))
	, pflay(new QFormLayout)
{

	//установка формата и проверка ввода
	pleAddress->setInputMask("000.000.000.000");
	pleAddress->setValidator(
		new QRegExpValidator(QRegExp("")));
	plePort->setInputMask("00000");
	plePort->setValidator(new QIntValidator(1000, 65535));

	QHBoxLayout* phlay = new QHBoxLayout;
	phlay->addWidget(pcmdOk);
	phlay->addWidget(pcmdCancel);

	pflay->addRow(new QLabel("Адрес сервера: "), pleAddress);
	pflay->addRow(new QLabel("Порт сервера: "), plePort);
	pflay->addRow(phlay);

	setLayout(pflay);
	setAttribute(Qt::WA_DeleteOnClose);

	connect(pcmdOk, SIGNAL(clicked()),
		SLOT(slotOK()));
	connect(pcmdCancel, SIGNAL(clicked()),
		SLOT(slotCancel()));
}

SettingsWidget::~SettingsWidget()
{
	qDebug() << "~SettingsWidget";
}

void SettingsWidget::setCurrentAddress(QString addr, QString port)
{
	mcurrentAddress = addr;
	mcurrentPort = port;

	pleAddress->setText(mcurrentAddress);
	plePort->setText(mcurrentPort);

	pflay->addRow(new QLabel(
			QString("Текущий адрес и порт: %1:%2")
			.arg(mcurrentAddress)
			.arg(mcurrentPort)));
}

void SettingsWidget::slotOK()
{
	emit addressChanged(pleAddress->text(), plePort->text());
	done(QDialog::Accepted);

}

void SettingsWidget::slotCancel()
{
	done(QDialog::Rejected);
}
