#include "convertionwidget.h"

#include <QDebug>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QFile>
#include <QFileDialog>

ConvertionWidget::ConvertionWidget(QString name,
	QWidget* parent)
	: QWidget(parent)
	, cmdSend(new QPushButton("Отправить сообщение"))
	, cmdSendFile(new QPushButton("Отправить файл"))
	, cmdClr(new QPushButton("Очистить"))
	, cmdAudioCall(new QPushButton("Аудиозвонок"))
	, cmdVideoCall(new QPushButton("Видеозвонок"))
	, pdisplay(new QTextEdit)
	, pmsgField(new QTextEdit)
{
	convertionName = name;

	cmdSend->setEnabled(false);
	cmdSendFile->setEnabled(false);
	pmsgField->setPlaceholderText("Введите сообщение...");
	pdisplay->setReadOnly(true);

	cmdAudioCall->setIcon(QIcon(":/Res/Icons/audiocall.png"));
	cmdVideoCall->setIcon(QIcon(":/Res/Icons/videocall.png"));
	cmdSendFile->setIcon(QIcon(":/Res/Icons/attach_file.png"));
	cmdSend->setIcon(QIcon(":/Res/Icons/send.png"));
	cmdClr->setIcon(QIcon(":/Res/Icons/clear.png"));

	QVBoxLayout* pvlay = new QVBoxLayout;
	pvlay->addWidget(new QLabel(name));
	pvlay->addWidget(pdisplay, 6);
	pvlay->addWidget(pmsgField, 1);

	QHBoxLayout* phlay = new QHBoxLayout;
	phlay->addWidget(cmdAudioCall);
	phlay->addWidget(cmdVideoCall);
	phlay->addWidget(cmdSendFile);
	phlay->addWidget(cmdSend);
	phlay->addWidget(cmdClr);
	pvlay->addLayout(phlay);

	setLayout(pvlay);

	connect(cmdSend, SIGNAL(clicked()), SLOT(slotSend()));
	connect(cmdSendFile, SIGNAL(clicked()), SLOT(slotSendFile()));
	connect(cmdClr, SIGNAL(clicked()), SLOT(slotClrDisplay()));

	connect(pmsgField, SIGNAL(textChanged()), SLOT(slotMsgChanged()));

	connect(cmdAudioCall, SIGNAL(clicked()), SLOT(slotAudioCall()));
	connect(cmdVideoCall, SIGNAL(clicked()), SLOT(slotVideoCall()));
}

void ConvertionWidget::setSocketState(bool isOpen)
{
	socketIsOpen = isOpen;

	cmdSendFile->setEnabled(isOpen);
	cmdAudioCall->setEnabled(isOpen);
	cmdVideoCall->setEnabled(isOpen);
}

void ConvertionWidget::setSentEnabled(bool value)
{
	cmdSend->setEnabled(value);
}

void ConvertionWidget::appendMsg(const QString& msg)
{
	QString str = formatTimeToString(QTime::currentTime());

	str += " " + msg;

	pdisplay->append(str);
}

void ConvertionWidget::slotSend()
{
	QString msg = pmsgField->toPlainText();
	slotAppendMessageToDisplay("Вы: " + msg, QTime::currentTime());

	QString to = convertionName;
	emit sentClicked(DATATYPE::MESSAGE, msg, to);
	pmsgField->clear();
	cmdSend->setEnabled(false);
}

void ConvertionWidget::slotSendFile()
{
	qDebug() << "ConvertionWidget: Send file button clicked";

	//invoke file selection dialog
	QString fullFileName = nullptr;
	fullFileName = QFileDialog::getOpenFileName(parentWidget());

	if(fullFileName != nullptr)
	{
		QFile* file = new QFile(fullFileName);
		QFileInfo* fileInfo = new QFileInfo(*file);

		if(fileInfo->isFile() && fileInfo->isReadable())
		{
			QString to = convertionName;

			slotAppendMessageToDisplay("Вы: Отправка файла " + fileInfo->fileName(), QTime::currentTime());

			emit sentClicked(DATATYPE::FILE, fullFileName, to);
		}
		else
		{
			qWarning() << "ConvertionWidget: Selected file is not a file or can't be read";
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
	if(pmsgField->toPlainText() != "" && socketIsOpen)
		cmdSend->setEnabled(true);
}

void ConvertionWidget::slotAudioCall()
{
	emit audioCallClicked(convertionName);
}

void ConvertionWidget::slotVideoCall()
{
	emit videoCallClicked(convertionName);
}

void ConvertionWidget::slotAppendMessageToDisplay(QString msg, const QTime& actionTime, QString from)
{
	QString str = formatTimeToString(actionTime);

	if(from != nullptr)
		str += " " + from + ":";

	str += " " + msg;

	pdisplay->append(str);
}
