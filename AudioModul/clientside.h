#ifndef CLIENTSIDE_H
#define CLIENTSIDE_H

#include <QObject>

class ClientSide : public QObject
{
 Q_OBJECT
public:
 explicit ClientSide(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CLIENTSIDE_H