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
 pdisplay->setReadOnly(true);

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

 connect(pmsgField, SIGNAL(textChanged()), SLOT(slotMsgChanged()));
}

void ConvertionWidget::setSocketState(bool isOpen)
{
 socketIsOpen=isOpen;
}

void ConvertionWidget::setSentEnabled(bool value)
{
 pcmdSent->setEnabled(value);
}

void ConvertionWidget::slotSentClicked()
{
 QString msg=pmsgField->toPlainText();
 slotAppendMessageToDisplay("Вы: " + msg, QTime::currentTime());
 emit sentClicked(DATATYPE::MESSAGE, msg);
 pmsgField->clear();
 pcmdSent->setEnabled(false);
}

void ConvertionWidget::slotClrDisplay()
{
 pmsgField->clear();
}

void ConvertionWidget::slotMsgChanged()
{
 if(pmsgField->toPlainText()!="" && socketIsOpen)
	pcmdSent->setEnabled(true);
}

void ConvertionWidget::slotAppendMessageToDisplay(QString msg, const QTime &actionTime)
{
 pdisplay->append(formatTimeToString(actionTime) +" " + msg);
}
