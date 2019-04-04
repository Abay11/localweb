#ifndef SERVERSIDE_H
#define SERVERSIDE_H

#include <QObject>

class ServerSide : public QObject
{
 Q_OBJECT
public:
 explicit ServerSide(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SERVERSIDE_H