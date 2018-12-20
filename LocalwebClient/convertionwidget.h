#ifndef CONVERTIONWIDGET_H
#define CONVERTIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QTime>

#include "../clientinfo.h"
#include "../commonutilityfuncs.h"

class ConvertionWidget:public QWidget
{
 Q_OBJECT
private:
 QPushButton *pcmdSent,*pcmdClr;

 QTextEdit *pdisplay,*pmsgField;

 bool socketIsOpen=false;

public:
 ConvertionWidget(QString name,
									QWidget *parent=nullptr);

 void setSocketState(bool);
 void setSentEnabled(bool);


private slots:
 void slotSentClicked();
 void slotClrDisplay();
 void slotMsgChanged();

public slots:
 void slotAppendMessageToDisplay(QString msg, const QTime &actionTime);

signals:
 void sentClicked(DATATYPE type, QString msg);
};

#endif // CONVERTIONWIDGET_H
