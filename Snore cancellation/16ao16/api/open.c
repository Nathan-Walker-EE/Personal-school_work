// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/api/open.c $
// $Rev: 42898 $
// $Date: 2018-05-30 13:18:14 -0500 (Wed, 30 May 2018) $

// 16AO16: API Library: source file

#include "main.h"



//*****************************************************************************
// Return Values:
//	0	= success, fd is valid and >= 0
//	<0  = failure, value is -errno or -GetLastError (Windows only), fd is -1
//	>0	= never returned
int ao16_open(int device, int share, int* fd)
{
	int	ret;

	ret	= gsc_api_open(device, share, fd, AO16_BASE_NAME);
	return(ret);
}


