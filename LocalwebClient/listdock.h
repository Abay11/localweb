#ifndef CLIENTSLISTCONTAINER_H
#define CLIENTSLISTCONTAINER_H

#include <QDockWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListView>

#include <QStringListModel>

class ListDock:public QDockWidget
{
 QListView *ponline;
 QListView *poffline;

public:
 ListDock(QWidget *parent=nullptr);

 QListView *onlines();
 QListView *offlines();

 void setOnlineModel(QStringListModel *pmodel);
 void setOfflineModel(QStringListModel *pmodel);
};

#endif // CLIENTSLISTCONTAINER_H
