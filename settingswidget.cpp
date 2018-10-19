#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *prnt)
 :QDialog(prnt)
 ,pleAddress(new QLineEdit)
 ,plePort(new QLineEdit)
 ,pcmdOk(new QPushButton("OK"))
 ,pcmdCancel(new QPushButton("Отмена"))
 ,pflay(new QFormLayout)
{

 QHBoxLayout *phlay=new QHBoxLayout;
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
 qDebug()<<"~SettingsWidget";
}

void SettingsWidget::setCurrentAddress(QString addr, QString port )
{
 mcurrentAddress=addr;
 mcurrentPort=port;

 pflay->addRow(new QLabel(
								QString("Текущий адрес и порт: %1:%2")
								.arg(mcurrentAddress)
								.arg(mcurrentPort)));
}

void SettingsWidget::slotOK()
{
 qDebug()<<"OOOOKKKKKK";
 emit addressChanged(pleAddress->text(), plePort->text());
 done(QDialog::Accepted);
}

void SettingsWidget::slotCancel()
{
 done(QDialog::Rejected);
 qDebug()<<"CANCEEEEEEEEEEEL";
}
