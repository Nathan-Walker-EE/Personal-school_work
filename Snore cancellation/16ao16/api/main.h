// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/api/main.h $
// $Rev: 44909 $
// $Date: 2019-05-16 20:39:02 -0500 (Thu, 16 May 2019) $

// 16AO16: API Library: header file

#ifndef	__MAIN_H__
#define	__MAIN_H__

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#include "16ao16_api.h"
#include "gsc_main.h"



// #defines *******************************************************************

#define	GSC_DEVS_PER_BOARD					1	// from ../driver/main.h

#define	STREAM_ID_RX						(0 << GSC_IO_SIZE_FLASG_SHIFT)
#define	STREAM_ID_TX						(0 << GSC_IO_SIZE_FLASG_SHIFT)



#endif
