// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/write.c $
// $Rev: 44907 $
// $Date: 2019-05-16 20:31:04 -0500 (Thu, 16 May 2019) $

// 16AO16: Device Driver: source file

#include "main.h"



// #defines *******************************************************************

#define	BOR_FULL	(0x1 << 15)
#define	BOR_3_4		(0x1 << 14)
#define	BOR_1_4		(0x1 << 13)
#define	BOR_EMPTY	(0x1 << 12)



//*****************************************************************************
static void _dev_io_sw_init(dev_data_t* dev, dev_io_t* io)
{
	io->io_mode			= AO16_IO_MODE_DEFAULT;
	io->overflow_data	= AO16_TX_IO_OVER_DATA_DEFAULT;
	io->overflow_frame	= AO16_TX_IO_OVER_FRAME_DEFAULT;
	io->timeout_s		= AO16_IO_TIMEOUT_DEFAULT;
}



//*****************************************************************************
static void _dev_io_close(dev_data_t* dev, dev_io_t* io)
{
	io->dma_channel	= NULL;
}



//*****************************************************************************
static void _dev_io_open(dev_data_t* dev, dev_io_t* io)
{
	_dev_io_sw_init(dev, io);
}



//*****************************************************************************
static int _dev_io_startup(dev_data_t* dev, dev_io_t* io)
{
	u32	bor;
	int	ret	= 0;

	if ((io->overflow_data) || (io->overflow_frame))
	{
		bor	= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bor_32);

		if ((io->overflow_data) && (bor & D16))
			ret	= -EIO;

		if ((io->overflow_frame) && (bor & D17))
			ret	= -EIO;
	}

	return(ret);
}



//*****************************************************************************
static long _dev_pio_available(dev_data_t* dev, dev_io_t* io, size_t bytes)
{
	u32	bor;
	u32	size;

	buffer_size(dev, &size);
	bor	= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bor_32);

	if (bor & BOR_FULL)
		bytes	= 0;
	else if (bor & BOR_3_4)
		bytes	= 1;
	else if (bor & BOR_EMPTY)
		bytes	= size;
	else if (bor & BOR_1_4)
		bytes	= size * 3 / 4;
	else
		bytes	= size / 4;

	bytes	*= 4;
	return(bytes);
}



//*****************************************************************************
static long _dev_pio_xfer(
	dev_data_t*		dev,
	dev_io_t*		io,
	const os_mem_t*	mem,
	size_t			bytes,
	os_time_tick_t	st_end)
{
	long	qty;

	qty	= gsc_write_pio_work_32_bit(dev, io, mem, bytes, st_end);
	return(qty);
}



//*****************************************************************************
static long _dev_bmdma_available(dev_data_t* dev, dev_io_t* io, size_t bytes)
{
	u32	bor;
	u32	samples;
	u32	size;

	buffer_size(dev, &size);
	bor	= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bor_32);

	if (bor & BOR_FULL)
		samples	= 0;
	else if (bor & BOR_3_4)
		samples	= 1;
	else if (bor & BOR_EMPTY)
		samples	= size;
	else if (bor & BOR_1_4)
		samples	= size * 3 / 4;
	else
		samples	= size / 4;

	bytes	= samples * 4;
	return(bytes);
}



//*****************************************************************************
static long _dev_bmdma_xfer(
	dev_data_t*		dev,
	dev_io_t*		io,
	const os_mem_t*	mem,
	size_t			bytes,
	os_time_tick_t	st_end)
{
	u32		dpr;
	u32		mode;
	long	qty;
	long	samples	= bytes / 4;

	if (samples < io->pio_threshold)
	{
		qty	= gsc_write_pio_work_32_bit(dev, io, mem, bytes, st_end);
	}
	else
	{
		mode	= GSC_DMA_MODE_BLOCK_DMA
				| GSC_DMA_MODE_SIZE_32_BITS
				| GSC_DMA_MODE_INPUT_ENABLE
				| GSC_DMA_MODE_BURSTING_LOCAL
				| GSC_DMA_MODE_INTERRUPT_WHEN_DONE
				| GSC_DMA_MODE_LOCAL_ADRESS_CONSTANT
				| GSC_DMA_MODE_PCI_INTERRUPT_ENABLE;

		dpr		= GSC_DMA_DPR_HOST_TO_BOARD
				| GSC_DMA_DPR_END_OF_CHAIN
				| GSC_DMA_DPR_TERMINAL_COUNT_IRQ;

		qty		= gsc_dma_perform(	dev,
									io,
									st_end,
									GSC_DMA_CAP_BMDMA_WRITE,
									mode,
									dpr,
									mem,
									bytes);
	}

	return(qty);
}



//*****************************************************************************
static long _dev_dmdma_available(dev_data_t* dev, dev_io_t* io, size_t bytes)
{
	return(bytes);
}



//*****************************************************************************
static long _dev_dmdma_xfer(
	dev_data_t*		dev,
	dev_io_t*		io,
	const os_mem_t*	mem,
	size_t			bytes,
	os_time_tick_t	st_end)
{
	u32		dpr;
	u32		mode;
	long	qty;
	long	samples	= bytes / 4;

	if (samples < io->pio_threshold)
	{
		qty	= gsc_write_pio_work_32_bit(dev, io, mem, bytes, st_end);
	}
	else
	{
		mode	= GSC_DMA_MODE_DM_DMA
				| GSC_DMA_MODE_SIZE_32_BITS
				| GSC_DMA_MODE_INPUT_ENABLE
				| GSC_DMA_MODE_BURSTING_LOCAL
				| GSC_DMA_MODE_INTERRUPT_WHEN_DONE
				| GSC_DMA_MODE_LOCAL_ADRESS_CONSTANT
				| GSC_DMA_MODE_PCI_INTERRUPT_ENABLE;

		dpr		= GSC_DMA_DPR_HOST_TO_BOARD
				| GSC_DMA_DPR_END_OF_CHAIN
				| GSC_DMA_DPR_TERMINAL_COUNT_IRQ;

		qty		= gsc_dma_perform(	dev,
									io,
									st_end,
									GSC_DMA_CAP_DMDMA_WRITE,
									mode,
									dpr,
									mem,
									bytes);
	}

	return(qty);
}



/******************************************************************************
*
*	Function:	dev_write_create
*
*	Purpose:
*
*		Perform a one-tine setup of the Write Analog Output streaming I/O structure.
*
*	Arguments:
*
*		dev		The data for the device of interest.
*
*		io		The I/O structure for this stream.
*
*	Returned:
*
*		None.
*
******************************************************************************/

int dev_write_create(dev_data_t* dev, dev_io_t* io)
{
	int	ret;

	io->bytes_per_sample	= 4;
	io->io_reg_offset		= GSC_REG_OFFSET(AO16_GSC_ODBR);
	io->io_reg_vaddr		= dev->vaddr.gsc_odbr_32;
	io->pio_threshold		= 32;

	io->dev_io_sw_init		= _dev_io_sw_init;
	io->dev_io_close		= _dev_io_close;
	io->dev_io_open			= _dev_io_open;
	io->dev_io_startup		= _dev_io_startup;
	io->dev_pio_available	= _dev_pio_available;
	io->dev_bmdma_available	= _dev_bmdma_available;
	io->dev_dmdma_available	= _dev_dmdma_available;
	io->dev_pio_xfer		= _dev_pio_xfer;
	io->dev_bmdma_xfer		= _dev_bmdma_xfer;
	io->dev_dmdma_xfer		= _dev_dmdma_xfer;

	io->wait.abort			= AO16_WAIT_IO_TX_ABORT;
	io->wait.done			= AO16_WAIT_IO_TX_DONE;
	io->wait.error			= AO16_WAIT_IO_TX_ERROR;
	io->wait.timeout		= AO16_WAIT_IO_TX_TIMEOUT;

	ret	= gsc_io_create(dev, io, dev->cache.fifo_size * 4);

	return(ret);
}


