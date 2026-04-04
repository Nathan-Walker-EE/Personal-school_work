// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/driver/gsc_dma.c $
// $Rev: 44692 $
// $Date: 2019-04-22 12:47:34 -0500 (Mon, 22 Apr 2019) $

// OS & Device Independent: Device Driver: source file

#include "main.h"



// #defines *******************************************************************

#ifndef	DEV_IO_AUTO_START
	#define	DEV_IO_AUTO_START(d,i)
#endif

#ifndef	DEV_DMA_CHAN_SETUP
	#define	DEV_DMA_CHAN_SETUP(a,io,dma,dpr)		0
#endif

#ifndef	GSC_DMA_CHANNEL_RESET_EXTERN
	#define	GSC_DMA_CHANNEL_RESET_EXTERN(dev, dma)
#endif

#if (GSC_RX_IO_DEBUG || GSC_TX_IO_DEBUG)
	#define	DBPRINTF(a)			printf a
#else
	#define	DBPRINTF(a)
#endif



// variables ******************************************************************

#if OS_DMA_ACQUIRE_METRICS

long long	dma_met_chan_get_count;
long long	dma_met_chan_get_byte_sum;
long long	dma_met_chan_get_ns_sum;
long long	dma_met_chan_get_retry_sum;

#endif



//*****************************************************************************
static int _dma_abort(dev_data_t* dev, gsc_dma_ch_t* dma)
{
	u8 	csr;
	int	ret	= 0;	// No active DMA aborted.

	if ((dma) && (dma->in_use))
	{
		csr	= os_reg_mem_rx_u8(NULL, dma->vaddr.csr_8);

		if ((csr & GSC_DMA_CSR_DONE) == 0)
		{
			// Force DMA termination.
			// If the DMA has, by chance, already ended, then this abort
			// request will apply to the next DMA request on this channel.
			os_reg_mem_tx_u8(NULL, dma->vaddr.csr_8, GSC_DMA_CSR_ABORT);
			os_time_us_delay(10);		// Wait for completion.
			ret	= 1;	// An active DMA was aborted.
		}
	}

	return(ret);
}



//*****************************************************************************
static int _dma_start(GSC_ALT_STRUCT_T* alt, void* arg)
{
	gsc_dma_ch_t*	dma;
	dev_io_t*		io;
	unsigned long	ul;

	io	= arg;
	dma	= io->dma_channel;

	// Start the DMA transfer.
	os_reg_mem_tx_u8(NULL, dma->vaddr.csr_8, 0);		// Disabe
	ul	= GSC_DMA_CSR_ENABLE;
	os_reg_mem_tx_u8(NULL, dma->vaddr.csr_8, (u8) ul);	// Enable
	ul	= GSC_DMA_CSR_ENABLE | GSC_DMA_CSR_START;
	os_reg_mem_tx_u8(NULL, dma->vaddr.csr_8, (u8) ul);	// Start

	// Initiate any Auto Start activity.
	DEV_IO_AUTO_START(alt, io);

	return(0);
}



/******************************************************************************
*
*	Function:	_dma_channel_get
*
*	Purpose:
*
*		Get a PLX DMA channel for the specified operation.
*
*	Arguments:
*
*		dev		The device data structure.
*
*		io		The I/O structure for the operation to perform.
*
*		ability	The required read/write ability.
*
*	Returned:
*
*		NULL	We couldn't get the channel.
*		else	This is the pointer to the DMA channel to use.
*
******************************************************************************/

static gsc_dma_ch_t* _dma_channel_get(
	dev_data_t*	dev,
	dev_io_t*	io,
	u32			ability)
{
	gsc_dma_ch_t*	dma;
	int				i;

	for (;;)	// Use a loop for convenience.
	{
		if (io->dma_channel)
		{
			dma	= io->dma_channel;
			break;
		}

		i	= os_sem_lock(&dev->dma.sem);

		if (i)
		{
			dma	= NULL;
			break;
		}

		if ((dev->dma.channel[0].in_use == 0) &&
			(dev->dma.channel[0].flags & ability))
		{
			dma			= &dev->dma.channel[0];
			dma->in_use	= 1;
			os_sem_unlock(&dev->dma.sem);
			break;
		}

		if ((dev->dma.channel[1].in_use == 0) &&
			(dev->dma.channel[1].flags & ability))
		{
			dma			= &dev->dma.channel[1];
			dma->in_use	= 1;
			os_sem_unlock(&dev->dma.sem);
			break;
		}

		dma	= NULL;
		os_sem_unlock(&dev->dma.sem);
		break;
	}

	return(dma);
}



/******************************************************************************
*
*	Function:	_dma_channel_reset
*
*	Purpose:
*
*		Reset the given DMA channel.
*
*	Arguments:
*
*		dev		The device data structure.
*
*		dma		The DMA channel to reset.
*
*	Returned:
*
*		None.
*
******************************************************************************/

static void _dma_channel_reset(dev_data_t* dev, gsc_dma_ch_t* dma)
{
	int	i;

	i			= os_sem_lock(&dev->dma.sem);
	dma->in_use	= 0;

	if (dma->vaddr.csr_8)
	{
		_dma_abort(dev, dma);
		os_reg_mem_tx_u8(NULL, dma->vaddr.csr_8, GSC_DMA_CSR_DISABLE);
		os_reg_mem_tx_u8(NULL, dma->vaddr.csr_8, GSC_DMA_CSR_CLEAR);

		// Allow external processing.
		GSC_DMA_CHANNEL_RESET_EXTERN(dev, dma);
	}

	if (i == 0)
		os_sem_unlock(&dev->dma.sem);
}



/******************************************************************************
*
*	Function:	gsc_dma_close
*
*	Purpose:
*
*		Cleanup the DMA code for the device due to the device being closed.
*
*	Arguments:
*
*		dev		The data for the device of interest.
*
*		index	The index of the device channel being referenced.
*
*	Returned:
*
*		None.
*
******************************************************************************/

void gsc_dma_close(dev_data_t* dev, int index)
{
	u32	map;

	if ((index >= 0) && (index < GSC_DEVS_PER_BOARD))
	{
		// Adjust the usage map.
		gsc_irq_access_lock(dev);
		map	= dev->dma.usage_map;
		map	&= ~ ((u32) 0x1 << index);
		dev->dma.usage_map	= map;

		if (map == 0)
		{
			// We're the last user so we now release the DMA engines.
			_dma_channel_reset(dev, &dev->dma.channel[0]);
			_dma_channel_reset(dev, &dev->dma.channel[1]);
		}

		gsc_irq_access_unlock(dev);
	}
}



/******************************************************************************
*
*	Function:	gsc_dma_create
*
*	Purpose:
*
*		Perform a one time initialization of the DMA portion of the given
*		structure.
*
*	Arguments:
*
*		dev		The data for the device of interest.
*
*		ch0_flg	The DMA Channel 0 capability flags.
*
*		ch1_flg	The DMA Channel 1 capability flags.
*
*	Returned:
*
*		0		All went well.
*		< 0		There was a problem and this is the error status.
*
******************************************************************************/

int gsc_dma_create(dev_data_t* dev, u32 ch0_flg, u32 ch1_flg)
{
	gsc_dma_ch_t*	dma;
	VADDR_T			vaddr;

	os_sem_create(&dev->dma.sem);
	os_sem_create(&dev->dma.wait.sem);
	os_sem_lock(&dev->dma.wait.sem);
	dev->dma.wait.count	= 0;

	dev->vaddr.plx_dmaarb_32	= PLX_VADDR(dev, 0xAC);
	dev->vaddr.plx_dmathr_32	= PLX_VADDR(dev, 0xB0);

	os_reg_mem_tx_u32(dev, dev->vaddr.plx_dmaarb_32, 0);
	os_reg_mem_tx_u32(dev, dev->vaddr.plx_dmathr_32, 0);

	dma					= &dev->dma.channel[0];
	dma->index			= 0;
	dma->flags			= ch0_flg;
	dma->intcsr_enable	= GSC_INTCSR_DMA_0_INT_ENABLE;
	dma->vaddr.mode_32	= PLX_VADDR(dev, 0x80);
	dma->vaddr.padr_32	= PLX_VADDR(dev, 0x84);
	dma->vaddr.ladr_32	= PLX_VADDR(dev, 0x88);
	dma->vaddr.siz_32	= PLX_VADDR(dev, 0x8C);
	dma->vaddr.dpr_32	= PLX_VADDR(dev, 0x90);
	dma->vaddr.csr_8	= PLX_VADDR(dev, 0xA8);
	_dma_channel_reset(dev, dma);

	dma					= &dev->dma.channel[1];
	dma->index			= 1;
	dma->flags			= ch1_flg;
	dma->intcsr_enable	= GSC_INTCSR_DMA_1_INT_ENABLE;
	dma->vaddr.mode_32	= PLX_VADDR(dev, 0x94);
	dma->vaddr.padr_32	= PLX_VADDR(dev, 0x98);
	dma->vaddr.ladr_32	= PLX_VADDR(dev, 0x9C);
	dma->vaddr.siz_32	= PLX_VADDR(dev, 0xA0);
	dma->vaddr.dpr_32	= PLX_VADDR(dev, 0xA4);
	dma->vaddr.csr_8	= PLX_VADDR(dev, 0xA9);
	_dma_channel_reset(dev, dma);

	// Change the DMA Read command from Memory Read Line to Memory Read Multiple
	// This IS for the 9056, the 9080 and the 9656.
	// This is NOT for the 9030 or the 9060ES.
	// This is the Control Register (CNTRL). We can't use the macro name as
	// we don't know which device headers are included.
	// Memory Read 0110b (6h) MR
	// Memory Read Line 1110b (Eh) MRL
	// Memory Read Multiple 1100b (Ch) MRM (multiple lines)
	vaddr	= PLX_VADDR(dev, 0x6C);
	os_reg_mem_mx_u32(NULL, vaddr, 0xC, 0xF);

	return(0);
}



/******************************************************************************
*
*	Function:	gsc_dma_destroy
*
*	Purpose:
*
*		Perform a one time clean up of the DMA portion of the given structure.
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

void gsc_dma_destroy(dev_data_t* dev)
{
	_dma_channel_reset(dev, &dev->dma.channel[0]);
	_dma_channel_reset(dev, &dev->dma.channel[1]);
	os_sem_destroy(&dev->dma.sem);
	os_sem_destroy(&dev->dma.wait.sem);
	memset(&dev->dma, 0, sizeof(dev->dma));
}



/******************************************************************************
*
*	Function:	gsc_dma_open
*
*	Purpose:
*
*		Perform the steps needed to prepare the DMA code for operation due to
*		the device being opened.
*
*	Arguments:
*
*		dev		The data for the device of interest.
*
*		index	The index of the device channel being referenced.
*
*	Returned:
*
*		0		All went well.
*		< 0		There was a problem and this is the error status.
*
******************************************************************************/

int gsc_dma_open(dev_data_t* dev, int index)
{
	u32	map;
	int	ret	= 0;

	if ((index < 0) || (index >= GSC_DEVS_PER_BOARD))
	{
		ret	= -EINVAL;
	}
	else
	{
		// Adjust the usage map.
		gsc_irq_access_lock(dev);
		map	= dev->dma.usage_map;
		dev->dma.usage_map	|= 0x1 << index;

		if (map == 0)
		{
			// We're the first user so we now setup the DMA engines.
			_dma_channel_reset(dev, &dev->dma.channel[0]);
			_dma_channel_reset(dev, &dev->dma.channel[1]);
		}

		gsc_irq_access_unlock(dev);
	}

	return(ret);
}



//*****************************************************************************
static int _wait_for_dma_channel(dev_data_t* dev)
{
	int	ret;

	ret	= os_sem_lock(&dev->dma.sem);

	if (ret == 0)
	{
		dev->dma.wait.count++;
		os_sem_unlock(&dev->dma.sem);

		ret	= os_sem_lock(&dev->dma.wait.sem);

		if (ret)
		{
			// There was a problem.
			os_sem_lock(&dev->dma.sem);
			dev->dma.wait.count--;
			os_sem_unlock(&dev->dma.sem);
		}
	}

	return(ret);
}



//*****************************************************************************
static void _resume_awaiting_threads(dev_data_t* dev)
{
	int	count;
	int	ret;

	ret		= os_sem_lock(&dev->dma.sem);
	count	= dev->dma.wait.count;

	if (count)
		dev->dma.wait.count--;

	if (ret == 0)
		os_sem_unlock(&dev->dma.sem);

	if (count)
		os_sem_unlock(&dev->dma.wait.sem);
}



/******************************************************************************
*
*	Function:	gsc_dma_perform
*
*	Purpose:
*
*		Perform a DMA transfer per the given arguments.
*
*		NOTES:
*
*		With interrupts it is possible for the requested interrupt to occure
*		before the current task goes to sleep. This is possible, for example,
*		because the DMA transfer could complete before the task gets to sleep.
*		So, to make sure things work in these cases, we take manual steps to
*		insure that the current task can be awoken by the interrupt before the
*		task actually sleeps and before the interrupt is enabled.
*
*	Arguments:
*
*		alt		The structure for the device to access.
*
*		io		The I/O structure for the operation to perform.
*
*		st_end	Timeout at this point in time (in system ticks).
*
*		ability	The required read/write ability.
*
*		mode	The value for the DMAMODE register.
*
*		dpr		The necessary bits for the DMADPR register.
*
*		buff	The data transfer buffer.
*
*		bytes	The number of bytes to transfer.
*
*	Returned:
*
*		>= 0	The number of bytes transferred.
*		< 0		There was a problem and this is the error status.
*
******************************************************************************/

long gsc_dma_perform(
	GSC_ALT_STRUCT_T*	alt,
	dev_io_t*			io,
	os_time_tick_t		st_end,	// The "system tick" time that we timeout.
	unsigned int		ability,
	u32					mode,
	u32					dpr,
	const os_mem_t*		mem,
	long				bytes)
{
	u8					csr;
	dev_data_t*			dev		= GSC_ALT_DEV_GET(alt);
	gsc_dma_ch_t*		dma;
	int					i;
	long				qty		= 0;
	os_time_tick_t		ticks;
	long				timeout	= 0;
	u32					ul;
	gsc_wait_t			wt;
	os_sem_t			sem;

	#if OS_DMA_ACQUIRE_METRICS

	struct timespec		begin;
	struct timespec		end;
	long long			ns;
	int					retries	= 0;

	#endif

	DBPRINTF(("DMA: %ld bytes\n", (long) bytes));

	os_sem_create(&sem);	// dummy required for wait operations.

	#if OS_DMA_ACQUIRE_METRICS

	getnstimeofday(&begin);

	#endif

	for (;;)	// Use a loop for convenience.
	{
		dma	= _dma_channel_get(dev, io, ability);

		if (dma == NULL)
		{
			#if OS_DMA_ACQUIRE_METRICS

			retries++;

			#endif

			qty	= _wait_for_dma_channel(dev);

			if (qty)
				break;

			continue;
		}


		#if OS_DMA_ACQUIRE_METRICS

		getnstimeofday(&end);
		ns	= ((s64) end.tv_sec * 1000000000LL + end.tv_nsec)
			- ((s64) begin.tv_sec * 1000000000LL + begin.tv_nsec);

		i	= os_sem_lock(&dev->dma.sem);

		dma_met_chan_get_count++;
		dma_met_chan_get_byte_sum		+= bytes;
		dma_met_chan_get_ns_sum			+= ns;
		dma_met_chan_get_retry_sum		+= retries;

		if (i == 0)
			os_sem_unlock(&dev->dma.sem);

		#endif

		#ifdef DEV_DMA_CHAN_SETUP

		{
			int	ret;

			ret	= DEV_DMA_CHAN_SETUP(alt, io, dma, dpr);

			if (ret < 0)
			{
				qty	= ret;
				break;
			}
		}

		#endif

		io->dma_channel	= dma;
		dma->error		= 0;

		// Enable the DMA interrupt.
		os_reg_mem_mx_u32(dev, dev->vaddr.plx_intcsr_32, dma->intcsr_enable, dma->intcsr_enable);

		// DMAMODEx
		os_reg_mem_tx_u32(dev, dma->vaddr.mode_32, mode);

		// DMAPADRx
		ul	= (u32) mem->adrs;
		os_reg_mem_tx_u32(dev, dma->vaddr.padr_32, ul);

		// DMALADRx
		os_reg_mem_tx_u32(dev, dma->vaddr.ladr_32, io->io_reg_offset);

		// DMASIZx
		os_reg_mem_tx_u32(dev, dma->vaddr.siz_32, (u32) bytes);

		// DMADPRx
		os_reg_mem_tx_u32(dev, dma->vaddr.dpr_32, dpr);

		//	Wait for completion.

		if (io->non_blocking)
		{
			// Allow at least one second for the DMA.
			timeout	= 1000;
		}
		else if (st_end)
		{
			timeout	= os_time_tick_timedout(st_end);

			if (timeout)
			{
				// We've already timed out.
				qty		= -ETIMEDOUT;
			}
			else
			{
				// This will almost always be the case.
				ticks	= os_time_tick_get();
				ticks	= st_end - ticks;		// Works even during rollover period.
				timeout	= os_time_ticks_to_ms(ticks);

				if (timeout < 10)
					timeout	= 10;
			}
		}
		else
		{
			timeout	= 0;	// never timeout
		}

		if (qty >= 0)
		{
			memset(&wt, 0, sizeof(wt));
			wt.flags		= GSC_WAIT_FLAG_INTERNAL;
			wt.main			= dma->index ? GSC_WAIT_MAIN_DMA1 : GSC_WAIT_MAIN_DMA0;
			wt.timeout_ms	= (u32) timeout;
			gsc_wait_event(alt, &wt, _dma_start, io, &sem);
			// The return value from gsc_wait_event() is ignored as it only validates the arguments.
		}

		// Clean up.

		// Check the results.
		csr	= os_reg_mem_rx_u8(dev, dma->vaddr.csr_8);

		// Disable the DONE interrupt.
		os_reg_mem_mx_u32(dev, dma->vaddr.mode_32, 0, GSC_DMA_MODE_INTERRUPT_WHEN_DONE);


		if ((csr & GSC_DMA_CSR_DONE) == 0)
		{
			//	If it isn't done then the operation timed out.
			//	Unfortunately the PLX PCI9080 doesn't tell us
			//	how many bytes were transferred. So, instead
			//	of reporting the number of transferred bytes,
			//	or zero, we return an error status.

			// Force DMA termination.
			// If the DMA has, by chance, already ended, then this abort
			// request will apply to the next DMA request on this channel.
			os_reg_mem_tx_u8(dev, dma->vaddr.csr_8, GSC_DMA_CSR_ABORT);
			os_time_us_delay(10);		// Wait for completion.
			qty	= -ETIMEDOUT;
		}
		else if (qty >= 0)
		{
			qty	= bytes;
		}

		// Disable the DMA interrupt, just in case.
		os_reg_mem_mx_u32(dev, dev->vaddr.plx_intcsr_32, 0, dma->intcsr_enable);

		// Clear the DMA.
		os_reg_mem_tx_u8(dev, dma->vaddr.csr_8, GSC_DMA_CSR_CLEAR);

		if (dma->flags & GSC_DMA_SEL_DYNAMIC)
		{
			i				= os_sem_lock(&dev->dma.sem);
			io->dma_channel	= NULL;
			dma->in_use		= 0;

			if (i == 0)
				os_sem_unlock(&dev->dma.sem);
		}

		// Notify any threads waiting on an available DMA channel.
		_resume_awaiting_threads(dev);
		break;
	}

	os_sem_destroy(&sem);
	return(qty);
}



/******************************************************************************
*
*	Function:	gsc_dma_abort_active_xfer
*
*	Purpose:
*
*		Abort the I/O channel's DMA transfer if it is in progress.
*
*	Arguments:
*
*		dev		The device data structure.
*
*		io		The I/O structure for the operation to access.
*
*	Returned:
*
*		1		An active DMA was aborted.
*		0		A DMA was not in progress.
*
******************************************************************************/

int gsc_dma_abort_active_xfer(dev_data_t* dev, dev_io_t* io)
{
	gsc_dma_ch_t*	dma;
	int				i;
	int				ret;

	i	= os_sem_lock(&dev->dma.sem);
	dma	= io->dma_channel;
	ret	= _dma_abort(dev, dma);

	if (i == 0)
		os_sem_unlock(&dev->dma.sem);

	return(ret);
}


