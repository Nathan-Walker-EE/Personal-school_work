// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/driver/gsc_irq.c $
// $Rev: 44474 $
// $Date: 2019-02-22 16:49:53 -0600 (Fri, 22 Feb 2019) $

// OS & Device Independent: Device Driver: source file

#include "main.h"



// #defines *******************************************************************

#ifdef DEV_SUPPORTS_IRQ
#if (GSC_DEVS_PER_BOARD > 1)
	#define	GSC_WAIT_RESUME_IRQ_MAIN(dev,flags)			_wait_resume_irq_main(dev,flags)
	#define	GSC_WAIT_RESUME_IRQ_MAIN_DMA(dev,dma,flags)	_wait_resume_irq_main_dma(dev,dma,flags)
#else
	#define	GSC_WAIT_RESUME_IRQ_MAIN(dev,flags)			gsc_wait_resume_irq_main(dev,flags)
	#define	GSC_WAIT_RESUME_IRQ_MAIN_DMA(dev,dma,flags)	gsc_wait_resume_irq_main(dev,flags)
#endif

#ifndef	GSC_DMA0_INT_EXTERN
#define	GSC_DMA0_INT_EXTERN(dev)
#endif

#ifndef	GSC_DMA1_INT_EXTERN
#define	GSC_DMA1_INT_EXTERN(dev)
#endif
#endif

#ifndef	OS_ISR_INTCSR_READ
#define	OS_ISR_INTCSR_READ(d)			os_reg_mem_rx_u32(NULL, (d)->vaddr.plx_intcsr_32);
#endif



//*****************************************************************************
#ifdef DEV_SUPPORTS_IRQ
#ifndef	PRINTF_ISR
static void PRINTF_ISR(const char* format, ...)
{
}
#endif
#endif



//*****************************************************************************
#ifdef DEV_SUPPORTS_IRQ
#if (GSC_DEVS_PER_BOARD > 1)
static void _wait_resume_irq_main(dev_data_t* dev, u32 flags)
{
	int	i;

	for (i = 0; i < GSC_DEVS_PER_BOARD; i++)
		gsc_wait_resume_irq_main(&dev->channel[i], flags);
}
#endif
#endif



//*****************************************************************************
#ifdef DEV_SUPPORTS_IRQ
#if (GSC_DEVS_PER_BOARD > 1)
static void _wait_resume_irq_main_dma(dev_data_t* dev, gsc_dma_ch_t* dma, u32 flags)
{
	GSC_ALT_STRUCT_T*	alt		= NULL;
	int					i;
	int					j;
	u32					type	= GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_SPURIOUS;

	for (i = 0; i < GSC_DEVS_PER_BOARD; i++)
	{
		alt	= &dev->channel[i];

		for (j = 0; j < DEV_IO_STREAM_QTY; j++)
		{
			if (alt->io.io_streams[j])
			{
				if (alt->io.io_streams[j]->dma_channel == dma)
				{
					type	= flags;
					i		= GSC_DEVS_PER_BOARD;
					break;
				}
			}
		}
	}

	gsc_wait_resume_irq_main(alt, type);
}
#endif
#endif



/******************************************************************************
*
*	Function:	gsc_irq_access_lock
*
*	Purpose:
*
*		Apply a locking mechanism to prevent simultaneous access to the
*		device's IRQ substructure.
*
*	Arguments:
*
*		dev		The data for the device of interest.
*
*	Returned:
*
*		None.
*
******************************************************************************/

void gsc_irq_access_lock(dev_data_t* dev)
{
	os_spinlock_lock(&dev->spinlock);
}



/******************************************************************************
*
*	Function:	gsc_irq_access_unlock
*
*	Purpose:
*
*		Remove the locking mechanism that prevented simultaneous access to the
*		device's IRQ substructure.
*
*	Arguments:
*
*		dev		The data for the device of interest.
*
*	Returned:
*
*		None.
*
******************************************************************************/

void gsc_irq_access_unlock(dev_data_t* dev)
{
	os_spinlock_unlock(&dev->spinlock);
}



/******************************************************************************
*
*	Function:	gsc_irq_close
*
*	Purpose:
*
*		Perform IRQ actions appropriate for closing a device.
*
*	Arguments:
*
*		dev		The device of interest.
*
*		index	The index of the device channel being referenced.
*
*	Returned:
*
*		None.
*
******************************************************************************/

#ifdef DEV_SUPPORTS_IRQ
void gsc_irq_close(dev_data_t* dev, int index)
{
	u32	map;

	if ((index >= 0) && (index < GSC_DEVS_PER_BOARD))
	{
		// Adjust the usage map.
		gsc_irq_access_lock(dev);
		map	= dev->irq.usage_map;
		map	&= ~ ((u32) 0x1 << index);
		dev->irq.usage_map	= map;

		if (map == 0)
		{
			// Reset the device interrupts.
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, 0);

			if (dev->irq.acquired)
			{
				// We're the last user so we now release the interrupt. We have
				// to release the lock before we release the interrupt, as the
				// release call causes the ISR to be called.
				gsc_irq_access_unlock(dev);
				os_irq_release(dev);
				gsc_irq_access_lock(dev);
				dev->irq.acquired	= 0;
			}
		}

		gsc_irq_access_unlock(dev);
	}
}
#endif



/******************************************************************************
*
*	Function:	gsc_irq_create
*
*	Purpose:
*
*		Perform a one time initialization of the structure.
*
*	Arguments:
*
*		dev		The device of interest.
*
*	Returned:
*
*		0		All went well.
*		< 0		There was a problem and this is the error status.
*
******************************************************************************/

#ifdef DEV_SUPPORTS_IRQ
int gsc_irq_create(dev_data_t* dev)
{
	int	ret;

	memset(&dev->irq, 0, sizeof(dev->irq));
	os_sem_create(&dev->irq.sem);

	dev->vaddr.plx_intcsr_32	= PLX_VADDR(dev, 0x68);

	os_reg_mem_tx_u32(dev, dev->vaddr.plx_intcsr_32, 0);

	// What type device is this?
	dev->irq.did	= os_reg_pci_rx_u16(dev, 1, 0x02);

	switch (dev->irq.did)
	{
		default:
		case 0x906E:

			dev->irq.isr_mask	= GSC_INTCSR_PCI_INT_ENABLE			// D8
								| GSC_INTCSR_PCI_DOOR_INT_ENABLE	// D9
								| GSC_INTCSR_ABORT_INT_ENABLE		// D10
								| GSC_INTCSR_LOCAL_INT_ENABLE		// D11
								| GSC_INTCSR_PCI_DOOR_INT_ACTIVE	// D13
								| GSC_INTCSR_ABORT_INT_ACTIVE		// D14
								| GSC_INTCSR_LOCAL_INT_ACTIVE		// D15
								| GSC_INTCSR_LOC_DOOR_INT_ENABLE	// D17
								| GSC_INTCSR_LOC_DOOR_INT_ACTIVE	// D20
								| GSC_INTCSR_BIST_INT_ACTIVE;		// D23
			break;

		case 0x9080:

			dev->irq.isr_mask	= GSC_INTCSR_MAILBOX_INT_ENABLE		// D3
								| GSC_INTCSR_PCI_INT_ENABLE			// D8
								| GSC_INTCSR_PCI_DOOR_INT_ENABLE	// D9
								| GSC_INTCSR_ABORT_INT_ENABLE		// D10
								| GSC_INTCSR_LOCAL_INT_ENABLE		// D11
								| GSC_INTCSR_PCI_DOOR_INT_ACTIVE	// D13
								| GSC_INTCSR_ABORT_INT_ACTIVE		// D14
								| GSC_INTCSR_LOCAL_INT_ACTIVE		// D15
								| GSC_INTCSR_LOC_DOOR_INT_ENABLE	// D17
								| GSC_INTCSR_DMA_0_INT_ENABLE		// D18
								| GSC_INTCSR_DMA_1_INT_ENABLE		// D19
								| GSC_INTCSR_LOC_DOOR_INT_ACTIVE	// D20
								| GSC_INTCSR_DMA_0_INT_ACTIVE		// D21
								| GSC_INTCSR_DMA_1_INT_ACTIVE		// D22
								| GSC_INTCSR_BIST_INT_ACTIVE		// D23
								| GSC_INTCSR_MAILBOX_INT_ACTIVE;	// D28-D31
			break;

		case 0x9056:
		case 0x9656:

			dev->irq.isr_mask	= GSC_INTCSR_MAILBOX_INT_ENABLE		// D3
								| GSC_INTCSR_POWER_MAN_INT_ENABLE	// D4
								| GSC_INTCSR_POWER_MAN_INT_ACTIVE	// D5
								| GSC_INTCSR_LOCAL_PE_INT_ENABLE	// D6
								| GSC_INTCSR_LOCAL_PE_INT_ACTIVE	// D7
								| GSC_INTCSR_PCI_INT_ENABLE			// D8
								| GSC_INTCSR_PCI_DOOR_INT_ENABLE	// D9
								| GSC_INTCSR_ABORT_INT_ENABLE		// D10
								| GSC_INTCSR_LOCAL_INT_ENABLE		// D11
								| GSC_INTCSR_PCI_DOOR_INT_ACTIVE	// D13
								| GSC_INTCSR_ABORT_INT_ACTIVE		// D14
								| GSC_INTCSR_LOCAL_INT_ACTIVE		// D15
								| GSC_INTCSR_LOC_DOOR_INT_ENABLE	// D17
								| GSC_INTCSR_DMA_0_INT_ENABLE		// D18
								| GSC_INTCSR_DMA_1_INT_ENABLE		// D19
								| GSC_INTCSR_LOC_DOOR_INT_ACTIVE	// D20
								| GSC_INTCSR_DMA_0_INT_ACTIVE		// D21
								| GSC_INTCSR_DMA_1_INT_ACTIVE		// D22
								| GSC_INTCSR_BIST_INT_ACTIVE		// D23
								| GSC_INTCSR_MAILBOX_INT_ACTIVE;	// D28-D31
			break;
	}

	ret	= os_irq_create(dev);
	return(ret);
}
#endif



/******************************************************************************
*
*	Function:	gsc_irq_destroy
*
*	Purpose:
*
*		Perform a one time tear down of the structure.
*
*	Arguments:
*
*		dev		The device of interest.
*
*	Returned:
*
*		None.
*
******************************************************************************/

#ifdef DEV_SUPPORTS_IRQ
void gsc_irq_destroy(dev_data_t* dev)
{
	if (dev->vaddr.plx_intcsr_32)
		os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, 0);

	os_irq_destroy(dev);
	os_sem_destroy(&dev->irq.sem);
	memset(&dev->irq, 0, sizeof(dev->irq));
}
#endif



/******************************************************************************
*
*	Function:	gsc_irq_open
*
*	Purpose:
*
*		Perform IRQ actions appropriate for opening a device.
*
*	Arguments:
*
*		dev		The device of interest.
*
*		index	The index of the device channel being referenced.
*
*	Returned:
*
*		0		All went well.
*		< 0		There was a problem and this is the error status.
*
******************************************************************************/

#ifdef DEV_SUPPORTS_IRQ
int gsc_irq_open(dev_data_t* dev, int index)
{
	u32	map;
	int	ret	= 0;
	u32	val;

	if ((index < 0) || (index >= GSC_DEVS_PER_BOARD))
	{
		ret	= -EINVAL;
	}
	else
	{
		// Adjust the usage map.
		gsc_irq_access_lock(dev);
		map	= dev->irq.usage_map;
		dev->irq.usage_map	|= 0x1 << index;

		if (map == 0)
		{
			// We're the first user so we now acquire the interrupt. We have
			// to release the lock before we acquire the interrupt, as the
			// acquire call may cause the ISR to be called.
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, 0);
			gsc_irq_access_unlock(dev);
			ret	= os_irq_acquire(dev);
			gsc_irq_access_lock(dev);

			if (ret == 0)
			{
				dev->irq.acquired	= 1;
				// Initialize the device interrupts.
				val	= GSC_INTCSR_PCI_INT_ENABLE		// D8
					| GSC_INTCSR_LOCAL_INT_ENABLE	// D11
					| GSC_INTCSR_ABORT_INT_ENABLE;	// D10
				os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, val);
			}
		}

		gsc_irq_access_unlock(dev);
	}

	return(ret);
}
#endif



//*****************************************************************************
#ifdef DEV_SUPPORTS_IRQ
static void _isr_abort_service(dev_data_t* dev, u32 intcsr)
{
	u16		mod		= 0;
	int		offset;
	VADDR_T	va;
	u16		val;

	// Report the PCI Abort address.
	va		= PLX_VADDR(dev, 0x104);
	val		= os_reg_mem_rx_u32(NULL, va);
	PRINTF_ISR(	"%s: Abort occurred: PCI address 0x%lX\n",
				DEV_NAME,
				(long) val);

	// Clasify the Abort, and clear it.
	offset	= 0x06;
	val		= os_reg_pci_rx_u16(dev, 0, offset);

	if (val & D11)
	{
		mod	|= D11;
		PRINTF_ISR("%s: Target Abort signaled\n", DEV_NAME);
	}

	if (val & D12)
	{
		mod	|= D12;
		PRINTF_ISR("%s: Target Abort received\n", DEV_NAME);
	}

	if (val & D13)
	{
		mod	|= D13;
		PRINTF_ISR("%s: Master Abort received\n", DEV_NAME);
	}

	if (mod)
		os_reg_pci_tx_u16(dev, 0, offset, mod);

	// Report the party involved in the Abort.

	if ((intcsr & D24) == 0)
		PRINTF_ISR("%s: Abort from Direct Master activity\n", DEV_NAME);

	if ((intcsr & D25) == 0)
	{
		dev->dma.channel[0].error	= 1;
		PRINTF_ISR("%s: Abort from DMA Channel 0 activity\n", DEV_NAME);
	}

	if ((intcsr & D26) == 0)
	{
		dev->dma.channel[1].error	= 1;
		PRINTF_ISR("%s: Abort from DMA Channel 1 activity\n", DEV_NAME);
	}

	if ((intcsr & D27) == 0)
		PRINTF_ISR("%s: Target Abort from 256 Master Retrys\n", DEV_NAME);
}
#endif



/******************************************************************************
*
*	Function:	gsc_irq_isr_common
*
*	Purpose:
*
*		Detect and service interrupts.
*
*	Arguments:
*
*		dev_id	The structure for the device with the interrupt.
*
*		flags	See the GSC_IRQ_ISR_FLAG_xxx definitions in gsc_main.h.
*
*	Returned:
*
*		0		The interrupt was NOT ours.
*		1		The interrupt was ours.
*
******************************************************************************/

#ifdef DEV_SUPPORTS_IRQ
int gsc_irq_isr_common(void* dev_id, u32 flags)
{
	dev_data_t*		dev	= (void*) dev_id;
	gsc_dma_ch_t*	dma;
	u32				intcsr;		// PLX Interrupt Control/Status Register
	int				is_ours;	// Is this one or our interrupts?

	if (flags & GSC_IRQ_ISR_FLAG_LOCK)
		gsc_irq_access_lock(dev);

	for (;;)
	{
		intcsr	= OS_ISR_INTCSR_READ(dev);
		intcsr	&= dev->irq.isr_mask;

		// PCI ****************************************************************

		if ((intcsr & GSC_INTCSR_PCI_INT_ENABLE) == 0)	// D8
		{
			// We don't have interrupts enabled. This isn't ours.
			is_ours	= 0;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_OTHER);
			break;
		}

		// DMA 0 **************************************************************

		if ((intcsr & GSC_INTCSR_DMA_0_INT_ENABLE) &&	// D18
			(intcsr & GSC_INTCSR_DMA_0_INT_ACTIVE))		// D21
		{
			// This is a DMA0 interrupt.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Clear the DMA DONE interrupt.
			dma	= &dev->dma.channel[0];
			os_reg_mem_tx_u8(NULL, dma->vaddr.csr_8, GSC_DMA_CSR_CLEAR);

			// Allow any external processing.
			GSC_DMA0_INT_EXTERN(dev);

			// Disable the DMA 0 interrupt.
			intcsr	&= ~GSC_INTCSR_DMA_0_INT_ENABLE;
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, intcsr);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN_DMA(dev, dma, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_DMA0);
			break;
		}

		// DMA 1 **************************************************************

		if ((intcsr & GSC_INTCSR_DMA_1_INT_ENABLE) &&	// D19
			(intcsr & GSC_INTCSR_DMA_1_INT_ACTIVE))		// D22
		{
			// This is a DMA1 interrupt.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Clear the DMA DONE interrupt.
			dma	= &dev->dma.channel[1];
			os_reg_mem_tx_u8(NULL, dma->vaddr.csr_8, GSC_DMA_CSR_CLEAR);

			// Allow any external processing.
			GSC_DMA1_INT_EXTERN(dev);

			// Disable the DMA 1 interrupt.
			intcsr	&= ~GSC_INTCSR_DMA_1_INT_ENABLE;
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, intcsr);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN_DMA(dev, dma, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_DMA1);
			break;
		}

		// LOCAL **************************************************************

		if ((intcsr & GSC_INTCSR_LOCAL_INT_ENABLE) &&	// D11
			(intcsr & GSC_INTCSR_LOCAL_INT_ACTIVE))		// D15
		{
			// This is a LOCAL interrupt.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Let device specific code process local interrupts.
			dev_irq_isr_local_handler(dev);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_GSC);
			break;
		}

		// MAIL BOX ***********************************************************

		if ((intcsr & GSC_INTCSR_MAILBOX_INT_ENABLE) &&		// D3
			(intcsr & GSC_INTCSR_MAILBOX_INT_ACTIVE))		// D28-D31
		{
			// This is an unexpected MAIL BOX interrupt.
			// We should never receive this interrupt.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Disable the MAIL BOX interrupt.
			intcsr	&= ~GSC_INTCSR_MAILBOX_INT_ENABLE;
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, intcsr);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_SPURIOUS);
			break;
		}

		// PCI DOORBELL *******************************************************

		if ((intcsr & GSC_INTCSR_PCI_DOOR_INT_ENABLE) &&	// D9
			(intcsr & GSC_INTCSR_PCI_DOOR_INT_ACTIVE))		// D13
		{
			// This is an unexpected PCI DOORBELL interrupt.
			// We should never receive this interrupt.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Disable the PCI DOORBELL interrupt.
			intcsr	&= ~GSC_INTCSR_PCI_DOOR_INT_ENABLE;
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, intcsr);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_SPURIOUS);
			break;
		}

		// ABORT **************************************************************

		if ((intcsr & GSC_INTCSR_ABORT_INT_ENABLE) &&	// D10
			(intcsr & GSC_INTCSR_ABORT_INT_ACTIVE))		// D14
		{
			// This is an unexpected ABORT interrupt.
			// We hope to never receive any of these.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Handle the abort condition.
			_isr_abort_service(dev, intcsr);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_SPURIOUS);
			break;
		}

		// LOCAL DOORBELL *****************************************************

		if ((intcsr & GSC_INTCSR_LOC_DOOR_INT_ENABLE) &&	// D17
			(intcsr & GSC_INTCSR_LOC_DOOR_INT_ACTIVE))		// D20
		{
			// This is an unexpected Local DOORBELL interrupt.
			// We should never receive this interrupt.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Disable the Local DOORBELL interrupt.
			intcsr	&= ~GSC_INTCSR_LOC_DOOR_INT_ENABLE;
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, intcsr);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_SPURIOUS);
			break;
		}

		// BIST ***************************************************************

		if (intcsr & GSC_INTCSR_BIST_INT_ACTIVE)	// D23
		{
			// This is an unexpected BIST interrupt.
			// We should never receive this interrupt.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Service the interrupt.
			os_reg_pci_tx_u8(dev, 0, 0x0F, 0);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_SPURIOUS);
			break;
		}

		// LOCAL PARITY ERROR *************************************************

		if (intcsr & GSC_INTCSR_LOCAL_PE_INT_ACTIVE)	// D7
		{
			// This looks like a local parity error.
			// This can occur even if the interrupt is disabled.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Service the interrupt - write 1 to clear
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, intcsr);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_SPURIOUS);
			break;
		}

		// POWER MANAGEMENT ***************************************************

		if ((intcsr & GSC_INTCSR_POWER_MAN_INT_ENABLE) &&	// D4
			(intcsr & GSC_INTCSR_POWER_MAN_INT_ACTIVE))		// D5
		{
			// This looks like a power management interrupt.
			is_ours	= 1;

			if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
				break;

			// Service the interrupt - write 1 to clear
			os_reg_mem_tx_u32(NULL, dev->vaddr.plx_intcsr_32, intcsr);

			// Resume any blocked threads.
			GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_SPURIOUS);
			break;
		}

		// This is not one of our interrupts.
		is_ours	= 0;

		if (flags & GSC_IRQ_ISR_FLAG_DETECT_ONLY)
			break;

		// Resume any blocked threads.
		GSC_WAIT_RESUME_IRQ_MAIN(dev, GSC_WAIT_MAIN_PCI | GSC_WAIT_MAIN_OTHER);
		break;
	}

	if (flags & GSC_IRQ_ISR_FLAG_LOCK)
		gsc_irq_access_unlock(dev);

	return(is_ours);
}
#endif



/******************************************************************************
*
*	Function:	gsc_irq_local_disable
*
*	Purpose:
*
*		Disable local interrupts. This is for non-ISR use only.
*
*	Arguments:
*
*		dev		The data for the device of interest.
*
*	Returned:
*
*		0		All went well.
*		< 0		There was a problem and this is the error status.
*
******************************************************************************/

#ifdef DEV_SUPPORTS_IRQ
int gsc_irq_local_disable(dev_data_t* dev)
{
	u32	mask	= GSC_INTCSR_LOCAL_INT_ENABLE;

	os_reg_mem_mx_u32(dev, dev->vaddr.plx_intcsr_32, 0, mask);
	return(0);
}
#endif



/******************************************************************************
*
*	Function:	gsc_irq_local_enable
*
*	Purpose:
*
*		Enable local interrupts. This is for non-ISR use only.
*
*	Arguments:
*
*		dev		The data for the device of interest.
*
*	Returned:
*
*		0		All went well.
*		< 0		There was a problem and this is the error status.
*
******************************************************************************/

#ifdef DEV_SUPPORTS_IRQ
int gsc_irq_local_enable(dev_data_t* dev)
{
	u32	mask	= GSC_INTCSR_LOCAL_INT_ENABLE;

	os_reg_mem_mx_u32(dev, dev->vaddr.plx_intcsr_32, mask, mask);
	return(0);
}
#endif


