// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/write.c $
// $Rev: 45045 $
// $Date: 2019-06-04 16:05:38 -0500 (Tue, 04 Jun 2019) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_write_util
*
*	Purpose:
*
*		Implement a visual wrapper around the device write call.
*
*	Arguments:
*
*		fd		Use this handle to access the device.
*
*		index	The index of the board to access. Ignore if < 0.
*
*		verbose	0: errors only, !0: all output
*
*		src		The source for the data to write.
*
*		bytes	The voume of data to write.
*
*	Returned:
*
*		>= 0	The volume of data written.
*		< 0		The negative of the number of errors seen.
*
******************************************************************************/

int ao16_write_util(int fd, int index, int verbose, const void* src, size_t bytes)
{
	int	ret;

	if (verbose)
		gsc_label_index("Writing", index);

	ret	= ao16_write(fd, src, bytes);

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


