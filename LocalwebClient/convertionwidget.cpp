#include "convertionwidget.h"

ConvertionWidget::ConvertionWidget(QString name,
																	 QWidget *parent)
 :QWidget(parent)
 ,pcmdSent(new QPushButton("Отправить"))
 ,pcmdClr(new QPushButton("Очистить"))
 ,pdisplay(new QTextEdit)
 ,pmsgField(new QTextEdit)
{
 pcmdSent->setEnabled(false);
 pmsgField->setPlaceholderText("Введите сообщение...");

 QVBoxLayout *pvlay=new QVBoxLayout;
 pvlay->addWidget(new QLabel(name));
 pvlay->addWidget(pdisplay, 6);
 pvlay->addWidget(pmsgField, 1);

 QHBoxLayout *phlay=new QHBoxLayout;
 phlay->addWidget(pcmdSent);
 phlay->addWidget(pcmdClr);
 pvlay->addLayout(phlay);

 setLayout(pvlay);

 connect(pcmdSent, SIGNAL(clicked()), SLOT(slotSentClicked()));
 connect(pcmdClr, SIGNAL(clicked()), SLOT(slotClrDisplay()));
}

void ConvertionWidget::slotSentClicked()
{
 emit sentClicked(pmsgField->toPlainText());
}

void ConvertionWidget::slotClrDisplay()
{
 pmsgField->clear();
}

void ConvertionWidget::slotMsgChanged()
{
 if(socketIsOpen)
	pcmdSent->setEnabled(true);

}

void ConvertionWidget::slotSocketIsOpen(bool value)
{
 socketIsOpen=value;
}
