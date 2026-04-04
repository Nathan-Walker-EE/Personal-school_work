// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/close.c $
// $Rev: 42902 $
// $Date: 2018-05-30 13:25:37 -0500 (Wed, 30 May 2018) $

// 16AO16: Device Driver: source file

#include "main.h"



//*****************************************************************************
int dev_close(dev_data_t* dev)
{
	initialize_ioctl(dev, NULL);
	gsc_wait_close(dev);
	dev_io_close(dev);
	gsc_dma_close(dev, 0);
	gsc_irq_close(dev, 0);
	return(0);
}



