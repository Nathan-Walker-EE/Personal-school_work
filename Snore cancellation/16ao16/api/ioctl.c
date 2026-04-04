// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/api/ioctl.c $
// $Rev: 42898 $
// $Date: 2018-05-30 13:18:14 -0500 (Wed, 30 May 2018) $

// 16AO16: API Library: source file

#include "main.h"



//*****************************************************************************
// Return Values:
//	0	= success
//	<0  = failure, value is -errno or -GetLastError (Windows only)
//	>0	= never returned
int ao16_ioctl(int fd, int request, void *arg)
{
	int	ret;

	ret	= gsc_api_ioctl(fd, request, arg);
	return(ret);
}


