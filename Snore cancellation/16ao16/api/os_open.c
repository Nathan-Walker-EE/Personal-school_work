// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/api/linux/os_open.c $
// $Rev: 43573 $
// $Date: 2018-10-12 13:28:08 -0500 (Fri, 12 Oct 2018) $

// Linux: API Library: source file

#include "main.h"



// #defines *******************************************************************

#ifndef	OS_DEV_NAME_SEPARATOR
#define	OS_DEV_NAME_SEPARATOR		"."
#endif



//*****************************************************************************
// Return Values:
//	>=0 = success
//	<0  = failure, value is -errno
int os_api_open(int index, int share, const char* base_name)
{
	int		fd;
	int		flags;
	char	name[128];

	if (os_api_global.init == 0)
	{
		fd	= -EPROTO;
	}
	else if (index == -1)
	{
		sprintf(name, "/proc/%s", base_name);
		fd	= open(name, S_IRUSR);

		if (fd < 0)
			fd	= -errno;
		else
			fd	= FD_ENCODE(fd);
	}
	else
	{
		sprintf(name, "/dev/%s%s%d", base_name, OS_DEV_NAME_SEPARATOR, index);
		flags	= O_RDWR | (share ? O_APPEND : 0);
		fd		= open(name, flags);

		if (fd < 0)
			fd	= -errno;
		else
			fd	= FD_ENCODE(fd);
	}

	return(fd);
}


