#include "commonutilityfuncs.h"


QString formatTimeToString(const QTime &time)
{
	return time.toString("[hh:mm:ss]");
}
