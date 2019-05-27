#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QMainWindow>

class PopUp;
class ListDock;
class ClientService;
class ConvertionWidget;
class CallingWindow;

class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QSystemTrayIcon;

class ClientWidget : public QMainWindow
{
	Q_OBJECT

private:
	QString maddress;
	QString* pserverAddress;
	QString* pserverPort;

	QLabel* plblAddress;
	QLineEdit* pleAddress;
	QLabel* plblPort;
	QLineEdit* plePort;
	QPushButton* pcmdConnect;
	QPushButton* pcmdDisconnect;

	QHBoxLayout* phlay;
	QHBoxLayout* pconvertionLay;
	QVBoxLayout* pvlay;

	PopUp* popup;

	QSystemTrayIcon* ptray;

	ListDock* plistdock;

	ClientService* pservice;

	QMap<QString, ConvertionWidget*>* convertionWidgets;
	ConvertionWidget* generalConvertion = nullptr;
	ConvertionWidget* currentCunvertion = nullptr;

public:
	ClientWidget(ClientService* pservice, QWidget* parent = nullptr);
	~ClientWidget();
	void turnStateOn();
	void turnStateOff();

	void setUI();

private:
	void initConvertions();

	CallingWindow* initCallingWindow();

private slots:
	void slotConnectClicked();
	void slotQuit();
	void slotSocketError();
	void slotShowNotification(QString msg);
	void slotSwitchConversions(QString convertionName);
	void slotForwardToDestination(QString msg, QString from, const QTime&);

	void slotMakeCall(QString nick);

	void slotReceiveCall(QString nick);

	void slotCompanonFinisheCall();

	void slotMakeVideoCall(QString to);

public slots:
	void slotAddressEdited();
	void slotPortEdited();
	void slotSetAddress(QString addr, QString port);
	void slotConnected();
	void slotDisconnected();
};

#endif // CLIENTWIDGET_H
