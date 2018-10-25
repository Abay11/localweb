#include "convertionwidget.h"

ConvertionWidget::ConvertionWidget(QString name,
																	 QWidget *parent)
 :QWidget(parent)
 ,pcmdSend(new QPushButton("Отправить"))
 ,pcmdClr(new QPushButton("Очистить"))
 ,pdisplay(new QTextEdit)
 ,pmsgField(new QTextEdit)
{
 pmsgField->setPlaceholderText("Введите сообщение...");

 QVBoxLayout *pvlay=new QVBoxLayout;
 pvlay->addWidget(new QLabel(name));
 pvlay->addWidget(pdisplay, 6);
 pvlay->addWidget(pmsgField, 1);

 QHBoxLayout *phlay=new QHBoxLayout;
 phlay->addWidget(pcmdSend);
 phlay->addWidget(pcmdClr);
 pvlay->addLayout(phlay);

 setLayout(pvlay);

 connect(pcmdSend, SIGNAL(clicked()), SIGNAL(sendClicked()));
 connect(pcmdClr, SIGNAL(clicked()), SLOT(slotClrDisplay()));
}

void ConvertionWidget::slotClrDisplay()
{
 pmsgField->clear();
}
