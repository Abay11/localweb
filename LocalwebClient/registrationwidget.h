#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QFormLayout;

class MyLogger;
class ClientService;

class Registration: public QWidget
{
	Q_OBJECT
private:
	QLineEdit* pleNick, *pleName;
	QPushButton* pcmdRegister, *pcmdExit, *pcmdSettings, *pcmdHelp;
	QHBoxLayout* phlay;
	QVBoxLayout* pvlay;
	QGridLayout* pglay;
	QFormLayout* pflay;

	ClientService* pservice;

public:
	Registration(ClientService* pservice, QWidget* parent = nullptr);

private slots:
	void slotSentRequest();
	void slotProcessResult(bool registrationResult);
	void slotExit();
	void slotSettings();
	void slotAddressChanged(QString, QString);

public slots:

signals:
	void registrationFinished();
};

#endif // REGISTRATION_H
