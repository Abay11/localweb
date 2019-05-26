#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;

class MyLogger;
class ClientWidget;
class Registration;
class ClientService;

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	QStackedWidget* pstackedWidgets;
	MyLogger* plogger;
	ClientWidget* pclientWidget;
	Registration* preg;
	ClientService* pservice;

	void setupAndShowRegistrationWidget();

public:
	explicit MainWindow(QWidget* parent = nullptr);
	void changeEvent(QEvent* e) override;

public slots:
	void slotShowClientWidget();
	void slotShowSocketError(QString title, QString info);
	void slotRestoreWindow();
};


#endif // MAINWINDOW_H
