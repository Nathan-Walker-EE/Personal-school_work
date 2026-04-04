// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/api/write.c $
// $Rev: 44909 $
// $Date: 2019-05-16 20:39:02 -0500 (Thu, 16 May 2019) $

// 16AO16: API Library: source file

#include "main.h"



//*****************************************************************************
// Return Values:
//	>=0	= success, the number of bytes transferred
//	<0  = failure, value is -errno or -GetLastError (Windows only)
int ao16_write(int fd, const void* src, size_t bytes)
{
	int	ret;

	if (bytes > GSC_IO_SIZE_QTY_MASK)
		bytes	= GSC_IO_SIZE_QTY_MASK & ~0x3;

	bytes	|= STREAM_ID_TX;
	ret		= gsc_api_write(fd, src, bytes);
	return(ret);
}


