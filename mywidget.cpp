#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
 :QWidget(parent)
 ,plblPort(new QLabel("Номер порта:"))
 ,plePort(new QLineEdit("7165"))
 ,pInfo(new QTextEdit)
 ,pcmdOn(new QPushButton("Пуск"))
 ,pcmdOff(new QPushButton("Стоп"))
 ,phlay(new QHBoxLayout)
 ,pvlay(new QVBoxLayout)
{
 pInfo->setText(QDateTime::currentDateTime().toString("[hh:mm:ss] ")+"Сервер не запущен.");
 pInfo->setReadOnly(true);

 phlay->addWidget(plblPort);
 phlay->addWidget(plePort);
 phlay->addWidget(pcmdOn);
 phlay->addWidget(pcmdOff);

 pvlay->addLayout(phlay);
 pvlay->addWidget(pInfo);

 setLayout(pvlay);
}


MyWidget::~MyWidget()
{

}
