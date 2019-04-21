#include "convertionwidget.h"

ConvertionWidget::ConvertionWidget(QString name,
																	 QWidget *parent)
 :QWidget(parent)
 ,cmdSend(new QPushButton("Отправить"))
 ,cmdSendFile(new QPushButton("Отправить файл"))
 ,cmdClr(new QPushButton("Очистить"))
 ,cmdMakeCall(new QPushButton("Позвонить"))
 ,pdisplay(new QTextEdit)
 ,pmsgField(new QTextEdit)
{
 convertionName=name;

 cmdSend->setEnabled(false);
 cmdSendFile->setEnabled(false);
 pmsgField->setPlaceholderText("Введите сообщение...");
 pdisplay->setReadOnly(true);

 QVBoxLayout *pvlay=new QVBoxLayout;
 pvlay->addWidget(new QLabel(name));
 pvlay->addWidget(pdisplay, 6);
 pvlay->addWidget(pmsgField, 1);

 QHBoxLayout *phlay=new QHBoxLayout;
 phlay->addWidget(cmdMakeCall);
 phlay->addWidget(cmdSend);
 phlay->addWidget(cmdSendFile);
 phlay->addWidget(cmdClr);
 pvlay->addLayout(phlay);

 setLayout(pvlay);

 connect(cmdSend, SIGNAL(clicked()), SLOT(slotSend()));
 connect(cmdSendFile, SIGNAL(clicked()), SLOT(slotSendFile()));
 connect(cmdClr, SIGNAL(clicked()), SLOT(slotClrDisplay()));

 connect(pmsgField, SIGNAL(textChanged()), SLOT(slotMsgChanged()));

 connect(cmdMakeCall, SIGNAL(clicked()), SLOT(slotMakeCall()));
}

void ConvertionWidget::setSocketState(bool isOpen)
{
 socketIsOpen=isOpen;

 cmdSendFile->setEnabled(isOpen);
 cmdMakeCall->setEnabled(isOpen);
}

void ConvertionWidget::setSentEnabled(bool value)
{
 cmdSend->setEnabled(value);
}

void ConvertionWidget::slotSend()
{
 QString msg=pmsgField->toPlainText();
 slotAppendMessageToDisplay("Вы: " + msg, QTime::currentTime());

 QString to=convertionName;
 emit sentClicked(DATATYPE::MESSAGE, msg, to);
 pmsgField->clear();
 cmdSend->setEnabled(false);
}

void ConvertionWidget::slotSendFile()
{
 qDebug()<<"ConvertionWidget: Send file button clicked";

 //invoke file selection dialog
 QString fullFileName=nullptr;
 fullFileName=QFileDialog::getOpenFileName(parentWidget());

 if(fullFileName!=nullptr)
	{
	 QFile *file=new QFile(fullFileName);
	 QFileInfo *fileInfo=new QFileInfo(*file);

	 if(fileInfo->isFile() && fileInfo->isReadable())
		{
		 QString to=convertionName;

		 slotAppendMessageToDisplay("Вы: Отправка файла " + fileInfo->fileName(), QTime::currentTime());

		 emit sentClicked(DATATYPE::FILE, fullFileName, to);
		}
	 else
		{
		 qWarning()<<"ConvertionWidget: Selected file is not a file or can't be read";
		}

	 delete file;
	 delete fileInfo;
	}
}

void ConvertionWidget::slotClrDisplay()
{
 pmsgField->clear();
}

void ConvertionWidget::slotMsgChanged()
{
 if(pmsgField->toPlainText()!="" && socketIsOpen)
	cmdSend->setEnabled(true);
}

void ConvertionWidget::slotMakeCall()
{
 emit makeCallClicked(convertionName);
}

void ConvertionWidget::slotAppendMessageToDisplay(QString msg, const QTime &actionTime, QString from)
{
 QString str = formatTimeToString(actionTime);

 if(from != nullptr)
	str += " " + from + ":";

 str += " " + msg;

 pdisplay->append(str);
}
