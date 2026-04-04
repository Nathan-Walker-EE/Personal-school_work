// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/include/16ao16_api.h $
// $Rev: 44908 $
// $Date: 2019-05-16 20:35:01 -0500 (Thu, 16 May 2019) $

// 16AO16: API Library: header file

#ifndef	__16AO16_API_H__
#define	__16AO16_API_H__

#include <stdlib.h>

#include "16ao16.h"



// prototypes *****************************************************************

int	ao16_close(int fd);
int	ao16_init(void);
int	ao16_ioctl(int fd, int request, void *arg);
int	ao16_open(int device, int share, int* fd);
int	ao16_read(int fd, void* dst, size_t bytes);
int	ao16_write(int fd, const void* src, size_t bytes);



#endif
