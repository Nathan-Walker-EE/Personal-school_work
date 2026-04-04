// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/include/16ao16_dsl.h $
// $Rev: 44908 $
// $Date: 2019-05-16 20:35:01 -0500 (Thu, 16 May 2019) $

// 16AO16: Document Source Library: header file

#ifndef	__16AO16_DSL_H__
#define	__16AO16_DSL_H__

#include <stdlib.h>

#include "16ao16_api.h"



// prototypes *****************************************************************

int	ao16_close_dsl(int fd);
int	ao16_init_dsl(void);
int	ao16_ioctl_dsl(int fd, int request, void *arg);
int	ao16_open_dsl(int index, int share, int* fd);
int ao16_read_dsl(int fd, void* dst, size_t bytes);
int	ao16_write_dsl(int fd, const void* src, size_t bytes);



#endif
