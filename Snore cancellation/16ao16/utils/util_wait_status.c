// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_wait_status.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_wait_status
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_WAIT_STATUS service.
*
*	Arguments:
*
*		fd		Use this handle to access the device.
*
*		index	The index of the device to access. Ignore if < 0.
*				This is for display purposes only.
*
*		verbose	Work verbosely?
*
*		wait	This is the crieteria to use.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int ao16_wait_status(int fd, int index, int verbose, gsc_wait_t* wait)
{
	int	errs	= 0;
	int	ret;

	if (verbose)
		gsc_label_index("Wait Status", index);

	if (wait)
	{
		ret		= ao16_ioctl(fd, AO16_IOCTL_WAIT_STATUS, wait);
		errs	+= ret ? 1 : 0;

		if (verbose)
		{
			printf(	"%s  (%ld matching wait%s)\n",
					errs ? "FAIL <---" : "PASS",
					(long) wait->count,
					(wait->count == 1) ? "" : "s");
		}
	}
	else
	{
		errs	= 1;

		if (verbose)
			printf(	"FAIL <---  (NULL pointer)\n");
	}

	return(errs);
}


