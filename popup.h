#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMediaPlayer>

class PopUp:public QWidget
{
 Q_OBJECT
private:
 QLabel *ptext;
 QMediaPlayer *pplayer;

public:
 PopUp(QWidget *pwgt);
 void  paintEvent(QPaintEvent *pe) override;
 void showNotify(QString &&msg, QPoint &&parentPos);

private slots:
 void hide();
};

#endif // POPUP_H
