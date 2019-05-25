#include "popup.h"

#include <QLabel>
#include <QMediaPlayer>
#include <QHBoxLayout>
#include <QPainter>
#include <QTimer>

PopUp::PopUp(QWidget* pwgt)
	: QWidget(pwgt, Qt::Popup)
	, ptext(new QLabel(pwgt))
	, pplayer(new QMediaPlayer)
{
	setAttribute(Qt::WA_ShowWithoutActivating);
	setAttribute(Qt::WA_TranslucentBackground);

	ptext->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	QHBoxLayout* hlay = new QHBoxLayout(this);
	hlay->addWidget(ptext);
	setLayout(hlay);

	ptext->setStyleSheet("QLabel{ color:white;"
		"margin-top: 6px;"
		"margin-bottom: 6px;"
		"margin-left: 10px;"
		"margin-right: 10px;"
		"font-size: 14px; }");

	pplayer->setMedia(QUrl("qrc:/Sounds/definite.mp3"));
}

void PopUp::paintEvent(QPaintEvent* pe)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QRect roundedRect;
	roundedRect.setX(rect().x());
	roundedRect.setY(rect().y());
	roundedRect.setWidth(rect().width());
	roundedRect.setHeight(rect().height());

	painter.setBrush(QBrush(QColor(0, 0, 0, 130)));
	painter.setPen(Qt::NoPen);

	painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopUp::showNotify(QString&& msg, QPoint&& parentPos)
{
	ptext->setText(msg);
	QWidget::show();
	move(parentPos.x() + parentWidget()->width() - width() - 10,
		parentPos.y() + parentWidget()->height() - height() - 10);

	pplayer->setPosition(0);
	pplayer->play();

	QTimer::singleShot(3000, this, SLOT(hide()));
}

void PopUp::hide()
{
	QWidget::hide();
}
