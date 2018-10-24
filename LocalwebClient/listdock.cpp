#include "listdock.h"

ListDock::ListDock(QWidget *parent)
 :QDockWidget(parent)
 ,ponline(new QListView(this))
 ,poffline(new QListView(this))
{
 QVBoxLayout *pvlay=new QVBoxLayout;


 pvlay->addWidget(new QLabel("<B>Доступные</B>"));
 pvlay->addWidget(ponline);
 pvlay->addWidget(new QLabel("<B>Недоступные</B>"));
 pvlay->addWidget(poffline);

 QWidget *pwidget=new QWidget(this);
 pwidget->setLayout(pvlay);

 setWidget(pwidget);
}

QListView *ListDock::onlines()
{
 return ponline;
}

QListView *ListDock::offlines()
{
 return poffline;
}

void ListDock::setOnlineModel(QStringListModel *pmodel)
{
 ponline->setModel(pmodel);
}

void ListDock::setOfflineModel(QStringListModel *pmodel)
{
 poffline->setModel(pmodel);
}
