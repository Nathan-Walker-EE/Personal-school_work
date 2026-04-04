// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/linux/os_util_time.c $
// $Rev: 42867 $
// $Date: 2018-05-18 12:43:42 -0500 (Fri, 18 May 2018) $

// Linux: Utility: source file

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "gsc_utils.h"




//*****************************************************************************
size_t os_time_delta_ms(void)
{
	static	int				started	= 0;
	static	struct timeval	start;

	size_t					delta;
	size_t					sec;
	struct timeval			tv;
	size_t					us;

	if (started == 0)
	{
		gettimeofday(&start, NULL);
		started	= 1;
	}

	gettimeofday(&tv, NULL);
	sec		= tv.tv_sec - start.tv_sec;
	us		= 1000000L + tv.tv_usec - start.tv_usec;
	delta	= (sec * 1000) + (us / 1000);
	return(delta);
}


