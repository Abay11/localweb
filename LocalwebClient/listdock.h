#ifndef CLIENTSLISTCONTAINER_H
#define CLIENTSLISTCONTAINER_H

#include <QDockWidget>

class QListView;
class QStringListModel;

class ListDock: public QDockWidget
{
	Q_OBJECT

	QListView* ponline;

	QListView* poffline;

public:
	ListDock(QWidget* parent = nullptr);

	QListView* onlines();

	QListView* offlines();

	void setOnlineModel(QStringListModel* pmodel);

	void setOfflineModel(QStringListModel* pmodel);

	void setupListviewContextMenu();

private slots:
	void slotShowContextMenu(const QPoint& pos);

	void slotOpenConvertion();

	void slotMakeCall();

signals:
	void openConvertion(QString nick);

	void makeCall(QString nick);
};

#endif // CLIENTSLISTCONTAINER_H
