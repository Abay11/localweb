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
 QPushButton *pcmdSend,*pcmdClr;

 QTextEdit *pdisplay,*pmsgField;

public:
 ConvertionWidget(QString name,
									QWidget *parent=nullptr);

private slots:
 void slotClrDisplay();

signals:
 void sendClicked();
};

#endif // CONVERTIONWIDGET_H
