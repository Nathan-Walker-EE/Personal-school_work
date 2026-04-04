// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_auto_calibrate.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_auto_calibrate
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_AUTO_CALIBRATE service.
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
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int	ao16_auto_calibrate(int fd, int index, int verbose)
{
	int	errs	= 0;
	int	ret;

	if (verbose)
		gsc_label_index("Auto-Calibrate", index);

	ret		= ao16_ioctl(fd, AO16_IOCTL_AUTO_CALIBRATE, NULL);
	errs	+= ret ? 1 : 0;

	if (verbose)
		printf("%s\n", errs ? "FAIL <---" : "PASS");

	return(errs);
}


