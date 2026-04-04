// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/api/linux/os_main.h $
// $Rev: 43573 $
// $Date: 2018-10-12 13:28:08 -0500 (Fri, 12 Oct 2018) $

// API Library: Linux: header file

#ifndef	__OS_MAIN_H__
#define	__OS_MAIN_H__

#include <sys/stat.h>



// #defines *******************************************************************

// This is done just to insure the API interface is used.
#define	FD_ENCODE(fd)				((fd) + 0x77777)
#define	FD_DECODE(fd)				((fd) - 0x77777)



// data types *****************************************************************

typedef struct
{
	int	init;	 // Have we been initialized?
} os_api_global_t;



// variables ******************************************************************

extern	os_api_global_t	os_api_global;



// prototypes *****************************************************************

int	os_api_close(int fd);										// Returned: 0 = success, <0 = failure, >0 = fault
int	os_api_init(void);											// Returned: 0 = success, <0 = failure, >0 = fault
int	os_api_ioctl(int fd, int cmd, void *arg);					// Returned: 0 = success, <0 = failure, >0 = fault
int	os_api_open(int index, int share, const char* base_name);	// Returned: >=0 = success, <0 = failure
int	os_api_read(int fd, void* dst, size_t bytes);				// Returned: >=0 = success, <0 = failure
int	os_api_write(int fd, const void* src, size_t bytes);		// Returned: >=0 = success, <0 = failure



#endif
