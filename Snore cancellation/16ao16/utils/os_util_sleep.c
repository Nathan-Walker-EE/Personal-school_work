// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/linux/os_util_sleep.c $
// $Rev: 42867 $
// $Date: 2018-05-18 12:43:42 -0500 (Fri, 18 May 2018) $

// Linux: Utility: source file

#include <unistd.h>

#include "gsc_utils.h"



//*****************************************************************************
void os_sleep_ms(int ms)
{
	for (; ms > 100; ms -= 100)
		usleep(100000);

	for (; ms > 10; ms -= 10)
		usleep(10000);

	if (ms > 0)
		usleep((long) ms * 1000);
	else
		usleep(1000);
}


