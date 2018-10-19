#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QIntValidator>

class SettingsWidget : public QDialog
{
 Q_OBJECT
private:
 QLineEdit *pleAddress;
 QLineEdit *plePort;
 QPushButton *pcmdOk;
 QPushButton *pcmdCancel;

 QFormLayout *pflay;

 QString mcurrentAddress;
 QString mcurrentPort;

public:
 SettingsWidget(QWidget *parent=nullptr);
 ~SettingsWidget();

 void setCurrentAddress(QString address, QString port);

public slots:
 void slotOK();
 void slotCancel();

signals:
 void addressChanged(QString, QString);
};
#endif // SETTINGSWIDGET_H
