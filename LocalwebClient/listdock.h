#ifndef CLIENTSLISTCONTAINER_H
#define CLIENTSLISTCONTAINER_H

#include <QDockWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListView>
#include <QDebug>
#include <QStringListModel>
#include <QMenu>

class ListDock:public QDockWidget
{
 Q_OBJECT
 QListView *ponline;
 QListView *poffline;

public:
 ListDock(QWidget *parent=nullptr);
 QListView *onlines();
 QListView *offlines();
 void setOnlineModel(QStringListModel *pmodel);
 void setOfflineModel(QStringListModel *pmodel);
 void setupListviewContextMenu();
private slots:
 void slotShowContextMenu(const QPoint &pos);
 void slotOpenConvertion();
signals:
 void openConvertion(QString nick);
};

#endif // CLIENTSLISTCONTAINER_H
