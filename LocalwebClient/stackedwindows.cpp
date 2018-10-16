#include "stackedwindows.h"

StackedWindows::StackedWindows(QWidget *parent)
 :QMainWindow(parent)
,pstackWidgets(new QStackedWidget(this))
{
 plogger=new MyLogger;
 pwidget=new ClientWidget(plogger, this);

 pstackWidgets->addWidget(pwidget);

 if(QFile::exists("data.bin"))
	{
	 pstackWidgets->setCurrentIndex(0);

	 //если мы вернулись из второй ветки, удаляем лишнее окно
	 if(preg)
		delete preg;
	}
 else
	{
	 preg=new Registration(plogger, this);
	 pstackWidgets->addWidget(preg);
	 pstackWidgets->setCurrentIndex(1);
	 connect(preg, SIGNAL(registered(int)),
				 this,
				 SLOT(setCurrentIndex(int)));
	 connect(preg->cmdExit(), SIGNAL(clicked()), SLOT(close()));
	 connect(this, SIGNAL(currentChanged(int)),
					 pwidget, SLOT(slotReloadBase())); //необходимо чтобы заново вычитать базу
	}

 setCentralWidget(pstackWidgets);

 resize(1000, 600);
}
