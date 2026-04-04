// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/open.c $
// $Rev: 44911 $
// $Date: 2019-05-16 20:44:07 -0500 (Thu, 16 May 2019) $

// 16AO16: Utilities: source file

#include "main.h"



/*****************************************************************************
*
*	Function:	ao16_open_util
*
*	Purpose:
*
*		Implement a visual wrapper around the device open call.
*
*	Arguments:
*
*		device	The zero based index of the device to access.
*
*		share	Open the device in shared access mode?
*
*		index	The index of the device to access. Ignore if < 0.
*				This is for display purposes only.
*
*		verbose	Work verbosely?
*
*		fd		The file descriptor is returned here.
*
*	Returned:
*
*		0		All went well. The fd value is valid.
*		> 0		The number of errors seen. The fd value is -1.
*
*****************************************************************************/

int ao16_open_util(int device, int share, int index, int verbose, int* fd)
{
	int	errs;
	int	ret;

	if (verbose)
		gsc_label_index("Open", index);

	ret		= ao16_open(device, share, fd);
	errs	= ret ? 1 : 0;

	if (verbose)
	{
		printf(	"%s  (%s)\n",
				errs ? "FAIL <---" : "PASS",
				share ? "Shared" : "Exclusive");
	}

	return(errs);
}


