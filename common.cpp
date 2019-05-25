#include "common.h"

#include <QTime>

QString formatTimeToString(const QTime& time)
{
	return time.toString("[hh:mm:ss]");
}
