// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/api/gsc_main.h $
// $Rev: 43577 $
// $Date: 2018-10-12 13:30:50 -0500 (Fri, 12 Oct 2018) $

// OS & Device Independent: API Library: header file

#ifndef	__GSC_MAIN_H__
#define	__GSC_MAIN_H__

#include "os_main.h"



// #defines *******************************************************************

int	gsc_api_close(int fd);												// Returned: <0 = failure, 0 = success,>0 = fault
int	gsc_api_init(void);													// Returned: <0 = failure, 0 = success, >0 = fault
int	gsc_api_ioctl(int fd, int cmd, void *arg);							// Returned: <0 = failure, 0 = success, >0 = fault
int	gsc_api_open(int index, int share, int* fd, const char* base_name);	// Returned: <0 = failure, 0 = success
int	gsc_api_read(int fd, void* dst, size_t bytes);						// Returned: <0 = failure, >=0 = success
int	gsc_api_write(int fd, const void *src, size_t bytes);				// Returned: <0 = failure, >=0 = success



#endif
