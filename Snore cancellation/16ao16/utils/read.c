// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/read.c $
// $Rev: 44911 $
// $Date: 2019-05-16 20:44:07 -0500 (Thu, 16 May 2019) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_read_util
*
*	Purpose:
*
*		Implement a visual wrapper around the device read call.
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
*		dst		The data read is placed here.
*
*		bytes	The voume of data to read.
*
*	Returned:
*
*		>= 0	The volume of data read.
*		< 0		The negative of the number of errors seen.
*
******************************************************************************/

int ao16_read_util(int fd, int index, int verbose, void* dst, size_t bytes)
{
	int	ret;

	if (verbose)
		gsc_label_index("Reading", index);

	ret	= ao16_read(fd, dst, bytes);

	if (verbose == 0)
	{
	}
	else if (ret >= 0)
	{
		printf("PASS  (%d bytes)\n", ret);
	}
	else
	{
		printf("FAIL <---  (requested %ld bytes, ret = %ld)\n", (long) bytes, (long) ret);
	}

	return(ret);
}


