// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/irq.c $
// $Rev: 44907 $
// $Date: 2019-05-16 20:31:04 -0500 (Thu, 16 May 2019) $

// 16AO16: Device Driver: source file

#include "main.h"



//*****************************************************************************
void dev_irq_isr_local_handler(dev_data_t* dev)
{
	#define	BCR_IRQ_REQUEST	0x800

	u32	bcr;
	u32	irq;

	bcr	= os_reg_mem_rx_u32(NULL, dev->vaddr.gsc_bcr_32);

	if (bcr & BCR_IRQ_REQUEST)
	{
		// Service the interrupt.
		bcr	^= BCR_IRQ_REQUEST;
		os_reg_mem_tx_u32(NULL, dev->vaddr.gsc_bcr_32, bcr);

		// Resume any waiting threads.
		irq	= GSC_FIELD_DECODE(bcr, 10, 8);

		switch (irq)
		{
			default:

				gsc_wait_resume_irq_main(dev, GSC_WAIT_MAIN_SPURIOUS);
				break;

			case AO16_IRQ_INIT_DONE:

				gsc_wait_resume_irq_gsc(dev, AO16_WAIT_GSC_INIT_DONE);
				break;

			case AO16_IRQ_AUTO_CAL_DONE:

				gsc_wait_resume_irq_gsc(dev, AO16_WAIT_GSC_AUTO_CAL_DONE);
				break;

			case AO16_IRQ_BUF_EMPTY:

				gsc_wait_resume_irq_gsc(dev, AO16_WAIT_GSC_BUF_EMPTY);
				break;

			case AO16_IRQ_BUF_1Q_FULL:

				gsc_wait_resume_irq_gsc(dev, AO16_WAIT_GSC_BUF_1Q_FULL);
				break;

			case AO16_IRQ_BUF_3Q_FULL:

				gsc_wait_resume_irq_gsc(dev, AO16_WAIT_GSC_BUF_3Q_FULL);
				break;

			case AO16_IRQ_BURST_TRIG_READY:

				gsc_wait_resume_irq_gsc(dev, AO16_WAIT_GSC_BURST_TRIG_READY);
				break;

			case AO16_IRQ_LOAD_READY:

				gsc_wait_resume_irq_gsc(dev, AO16_WAIT_GSC_LOAD_READY);
				break;

			case AO16_IRQ_LOAD_READY_END:

				gsc_wait_resume_irq_gsc(dev, AO16_WAIT_GSC_LOAD_READY_END);
				break;
		}
	}
	else
	{
		// We don't know the source of the interrupt.
		gsc_wait_resume_irq_main(dev, GSC_WAIT_MAIN_SPURIOUS);
	}
}



//*****************************************************************************
int dev_irq_create(dev_data_t* dev)
{
	int	ret;

	os_reg_mem_tx_u32(dev, dev->vaddr.gsc_bcr_32, 0);
	ret	= gsc_irq_create(dev);
	return(ret);
}



//*****************************************************************************
void dev_irq_destroy(dev_data_t* dev)
{
	if (dev->vaddr.gsc_bcr_32)
		os_reg_mem_tx_u32(dev, dev->vaddr.gsc_bcr_32, 0);

	gsc_irq_destroy(dev);
}


