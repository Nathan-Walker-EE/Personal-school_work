// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/api/gsc_open.c $
// $Rev: 42862 $
// $Date: 2018-05-18 12:34:12 -0500 (Fri, 18 May 2018) $

// OS & Device Independent: API Library: source file

#include "main.h"



//*****************************************************************************
// Return Values:
//	0	= success, fd is valid and >= 0
//	<0  = failure, value is -errno or -GetLastError (Windows only), fd is -1
//	>0	= never returned
int gsc_api_open(int index, int share, int* fd, const char* base_name)
{
	int	ret;

	if (fd == NULL)
	{
		ret	= -EINVAL;
	}
	else
	{
		ret		= os_api_open(index, share, base_name);
		fd[0]	= (ret < 0) ? -1 : ret;
		ret		= (ret < 0) ? ret : 0;
	}

	return(ret);
}


