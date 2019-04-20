#include "callingwindow.h"
#include "ui_callingwindow.h"

CallingWindow::CallingWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CallingWindow)
{
 ui->setupUi(this);
}

CallingWindow::~CallingWindow()
{
 delete ui;
}
