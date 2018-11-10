#ifndef CONVERTIONWIDGET_H
#define CONVERTIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QLabel>

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

private slots:
 void slotSentClicked();
 void slotClrDisplay();
 void slotMsgChanged();

public slots:
 void slotSocketIsOpen(bool);

signals:
 void sentClicked(QString msg);
};

#endif // CONVERTIONWIDGET_H
