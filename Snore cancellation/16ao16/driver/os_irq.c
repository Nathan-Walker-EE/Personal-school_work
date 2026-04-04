// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/driver/linux/os_irq.c $
// $Rev: 45073 $
// $Date: 2019-06-12 14:13:28 -0500 (Wed, 12 Jun 2019) $

// Linux: Device Driver: source file: This software is covered by the GNU GENERAL PUBLIC LICENSE (GPL).

#include "main.h"



//*****************************************************************************
#ifdef DEV_SUPPORTS_IRQ
int os_irq_acquire(dev_data_t* dev)
{
	int	ret	= 0;

	if (dev->irq.os.allotted == 0)
	{
		ret	= IRQ_REQUEST(dev, os_irq_isr);

		if (ret)
		{
			dev->irq.os.allotted	= 0;
			printf("%s: IRQ_REQUEST() failed: %d\n", DEV_NAME, ret);
		}
		else
		{
			dev->irq.os.allotted	= 1;
		}
	}

	return(ret);
}
#endif



//*****************************************************************************
#ifdef DEV_SUPPORTS_IRQ
int os_irq_create(dev_data_t* dev)
{
	// The ISR is installed during the acquire call, so nothing is done here.
	return(0);
}
#endif



//*****************************************************************************
#ifdef DEV_SUPPORTS_IRQ
void os_irq_destroy(dev_data_t* dev)
{
	// The ISR is removed during the acquire call, so nothing is done here.
}
#endif



//*****************************************************************************
#ifdef DEV_SUPPORTS_IRQ
void os_irq_release(dev_data_t* dev)
{
	if (dev->irq.os.allotted)
	{
		dev->irq.os.allotted	= 0;
		free_irq(dev->pci.pd->irq, dev);
	}
}
#endif


