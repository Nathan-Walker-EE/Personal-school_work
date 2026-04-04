// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/ioctl.c $
// $Rev: 44911 $
// $Date: 2019-05-16 20:44:07 -0500 (Thu, 16 May 2019) $

// 16AO16: Utilities: source file

#include "main.h"



/*****************************************************************************
*
*	Function:	ao16_ioctl_util
*
*	Purpose:
*
*		Implement a visual wrapper around the device IOCTL call.
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
*		request	The IOCTL service to request.
*
*		arg		The argument for the IOCTL service.
*
*	Returned:
*
*		>= 0	The number of errors seen.
*
*****************************************************************************/

int ao16_ioctl_util(int fd, int index, int verbose, int request, void *arg)
{
	int	errs;
	int	ret;

	if (verbose)
		gsc_label_index("IOCTL", index);

	ret		= ao16_ioctl(fd, request, arg);
	errs	= ret ? 1 : 0;

	if (verbose)
	{
		printf(	"%s  (#%d)\n",
				errs ? "FAIL <---" : "PASS",
				(int) OS_IOCTL_INDEX_DECODE(request));
	}

	return(errs);
}


