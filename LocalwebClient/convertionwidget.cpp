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
 pvlay->addWidget(pdisplay);
 pvlay->addWidget(pmsgField);

 QHBoxLayout *phlay=new QHBoxLayout;
 phlay->addWidget(pcmdSend);
 phlay->addWidget(pcmdClr);
 pvlay->addLayout(phlay);

 connect(pcmdSend, SIGNAL(clicked()), SLOT(sendClicked()));
 connect(pcmdClr, SIGNAL(clicked()), SLOT(slotClearDisplay()));
}

void ConvertionWidget::slotClrDispay()
{
 pmsgField->clear();
}
