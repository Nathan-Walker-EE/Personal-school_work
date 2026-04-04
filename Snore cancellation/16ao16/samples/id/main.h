// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/id/main.h $
// $Rev: 44917 $
// $Date: 2019-05-16 20:52:55 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: header file

#ifndef	__MAIN_H__
#define	__MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "16ao16_main.h"



// #defines *******************************************************************

#define	SIZEOF_ARRAY(a)				(sizeof((a)) / sizeof((a)[0]))



// prototypes *****************************************************************

int	id_device(int fd);



#endif
