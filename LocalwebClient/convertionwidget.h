#ifndef CONVERTIONWIDGET_H
#define CONVERTIONWIDGET_H

#include <QWidget>
#include <QVariant>

#include "../clientinfo.h"
#include "../common.h"

class QPushButton;
class QTextEdit;

class ConvertionWidget: public QWidget
{
	Q_OBJECT
private:
	QPushButton* cmdSend;
	QPushButton* cmdSendFile;
	QPushButton* cmdClr;

	QPushButton* cmdAudioCall;
	QPushButton* cmdVideoCall;

	QTextEdit* pdisplay, *pmsgField;

	bool socketIsOpen = false;
	QString convertionName;

public:
	ConvertionWidget(QString name,
		QWidget* parent = nullptr);

	void setSocketState(bool);

	void setSentEnabled(bool);

	void appendMsg(const QString& msg);


private slots:
	void slotSend();
	void slotSendFile();
	void slotClrDisplay();
	void slotMsgChanged();

	void slotAudioCall();

	void slotVideoCall();

public slots:
	void slotAppendMessageToDisplay(QString msg, const QTime& actionTime, QString from = nullptr);

signals:
	void sentClicked(DATATYPE type, QString stringData, QVariant variantData = 0);

	void audioCallClicked(QString nick);

	void videoCallClicked(QString nick);
};

#endif // CONVERTIONWIDGET_H
