// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/open.c $
// $Rev: 42919 $
// $Date: 2018-05-31 12:56:08 -0500 (Thu, 31 May 2018) $

// 16AO16: Device Driver: source file

#include "main.h"



//*****************************************************************************
int dev_open(dev_data_t* dev)
{
	int	ret;

	ret	= dev_io_open(dev);

	if (ret == 0)
		ret	= gsc_dma_open(dev, 0);

	if (ret == 0)
		ret	= gsc_irq_open(dev, 0);

	if (ret == 0)
		initialize_ioctl(dev, NULL);

	if (ret)
		dev_close(dev);

	return(ret);
}


