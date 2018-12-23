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

 setFeatures(QDockWidget::NoDockWidgetFeatures);
 setupListviewContextMenu();
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

void ListDock::setupListviewContextMenu()
{
 ponline->setContextMenuPolicy(Qt::CustomContextMenu);
 connect(ponline, SIGNAL(customContextMenuRequested(const QPoint &)),
				 SLOT(slotShowContextMenu(const QPoint &)));
}

void ListDock::slotShowContextMenu(const QPoint &pos)
{
 qDebug()<<"context menu requested";
 static QMenu *menu=nullptr;
 if(menu==nullptr)
	{
	 menu=new QMenu(this);
	 menu->addAction("Открыть беседу");
	 menu->addAction("Аудиозвонок");
	 menu->addAction("Видеозвонок");
	 menu->addAction("Показать информацию");
	}
 auto model=ponline->model();
 if(model->rowCount()>0)
	{
	 auto index=ponline->indexAt(pos);
	 qDebug()<<"Item data: "<<model->data(index);
	 QPoint gpos=ponline->mapToGlobal(pos);
	 menu->exec(gpos);
	}
}
