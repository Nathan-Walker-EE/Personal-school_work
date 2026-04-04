// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/close.c $
// $Rev: 44911 $
// $Date: 2019-05-16 20:44:07 -0500 (Thu, 16 May 2019) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_close_util
*
*	Purpose:
*
*		Implement a visual wrapper around the device close call.
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
*		>= 0	The number of errors seen.
*
******************************************************************************/

int ao16_close_util(int fd, int index, int verbose)
{
	int	errs;
	int	ret;

	if (verbose)
		gsc_label_index("Close", index);

	ret		= ao16_close(fd);
	errs	= ret ? 1 : 0;

	if (verbose)
		printf("%s\n", errs ? "FAIL <---" : "PASS");

	return(errs);
}


