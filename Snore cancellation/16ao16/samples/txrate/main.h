// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/txrate/main.h $
// $Rev: 44921 $
// $Date: 2019-05-16 20:56:31 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: header file

#ifndef	__MAIN_H__
#define	__MAIN_H__

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

#include "16ao16_main.h"



// #defines *******************************************************************

#define	SIZEOF_ARRAY(a)				(sizeof((a)) / sizeof((a)[0]))



// prototypes *****************************************************************

int	tx_rate_test(int fd, long mb);



#endif
