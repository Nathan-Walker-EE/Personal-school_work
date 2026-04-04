// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/device.c $
// $Rev: 44907 $
// $Date: 2019-05-16 20:31:04 -0500 (Thu, 16 May 2019) $

// 16AO16: Device Driver: source file

#include "main.h"



// variables *******************************************************************

const gsc_dev_id_t	dev_id_list[]	=
{
	// model	Vendor	Device	SubVen	SubDev	type
	{ "16AO16",	0x10B5, 0x9056, 0x10B5, 0x3120,	GSC_DEV_TYPE_16AO16	},
	{ NULL }
};



//*****************************************************************************
static void _cable_compute(dev_data_t* dev)
{
	if ((dev->cache.gsc_for_32 & 0xFFF) >= 0x009)
		dev->cache.cable_invert_4	= 1;
	else
		dev->cache.cable_invert_4	= 0;

	if ((dev->cache.gsc_for_32 & 0xFFF) >= 0x011)
		dev->cache.cable_passive_3	= 1;
	else
		dev->cache.cable_passive_3	= 0;
}



//*****************************************************************************
static void _channels_compute(dev_data_t* dev)
{
	dev->cache.channels_max	= 16;

	switch ((dev->cache.gsc_for_32 >> 16) & 0x3)
	{
		default:
		case 1:		dev->cache.channel_mask	= 0xFF;
					dev->cache.channel_qty	= 8;
					break;

		case 2:		dev->cache.channel_mask	= 0xFFF;
					dev->cache.channel_qty	= 12;
					break;

		case 3:		dev->cache.channel_mask	= 0xFFFF;
					dev->cache.channel_qty	= 16;
					break;
	}
}



//*****************************************************************************
static void _dmdma_compute(dev_data_t* dev)
{
	if ((dev->cache.gsc_for_32 & 0xFFF) >= 0x007)
		dev->cache.dmdma	= 1;
	else
		dev->cache.dmdma	= 0;
}



//*****************************************************************************
static void _filters_compute(dev_data_t* dev)
{
	switch ((dev->cache.gsc_for_32 >> 18) & 0x3)
	{
		default:
		case 0:		dev->cache.filter	= 0;
					break;

		case 1:		dev->cache.filter	= 10L * 1000L;
					break;

		case 2:		dev->cache.filter	= 100L * 1000L;
					break;
	}
}



//*****************************************************************************
static void _fref_default_compute(dev_data_t* dev)
{
	if (dev->cache.gsc_for_32 & D21)
		dev->cache.fref_default		= 49152000L;
	else
		dev->cache.fref_default		= 45000000L;
}



//*****************************************************************************
static void _volt_range_compute(dev_data_t* dev)
{
	if (dev->cache.gsc_for_32 & D23)
		dev->cache.volt_range	= AO16_VOLT_RANGE_HIGH;
	else
		dev->cache.volt_range	= AO16_VOLT_RANGE_LOW;
}



/******************************************************************************
*
*	Function:	dev_device_create
*
*	Purpose:
*
*		Do everything needed to setup and use the given device.
*
*	Arguments:
*
*		dev		The structure to initialize.
*
*	Returned:
*
*		0		All is well.
*		< 0		An appropriate error status.
*
******************************************************************************/

int dev_device_create(dev_data_t* dev)
{
	static const gsc_bar_maps_t	bar_map	=
	{
		{
			// mem	io	rw
			{ 1,	0,	GSC_REG_TYPE_ACCESS_RO },	// BAR 0: PLX registers, memory mapped
			{ 0,	0,	GSC_REG_TYPE_ACCESS_RO },	// BAR 1: PLX registers, I/O mapped
			{ 1,	0,	GSC_REG_TYPE_ACCESS_RW },	// BAR 2: GSC registers, memory mapped
			{ 0,	0,	GSC_REG_TYPE_ACCESS_RO },	// BAR 3: unused
			{ 0,	0,	GSC_REG_TYPE_ACCESS_RO },	// BAR 4: unused
			{ 0,	0,	GSC_REG_TYPE_ACCESS_RO }	// BAR 5: unused
		}
	};

	u32	dma;
	int	ret;

	for (;;)	// A convenience loop.
	{
		// Verify some macro contents.
		ret	= gsc_macro_test_base_name(AO16_BASE_NAME);
		if (ret)	break;

		ret	= gsc_macro_test_model();
		if (ret)	break;

		// PCI setup.
		ret	= os_pci_dev_enable(&dev->pci);
		if (ret)	break;

		ret	= os_pci_master_set(&dev->pci);
		if (ret)	break;

		// Control ISR access to the device and data structure.
		ret	= os_spinlock_create(&dev->spinlock);
		if (ret)	break;

		// Control access to the device and data structure.
		ret	= os_sem_create(&dev->sem);
		if (ret)	break;

		// Access the BAR regions.
		ret	= gsc_bar_create(dev, &dev->bar, &bar_map);
		if (ret)	break;

		// Firmware access.
		dev->vaddr.gsc_acr_32		= GSC_VADDR(dev, GSC_REG_OFFSET(AO16_GSC_ACR));
		dev->vaddr.gsc_bcr_32		= GSC_VADDR(dev, GSC_REG_OFFSET(AO16_GSC_BCR));
		dev->vaddr.gsc_bor_32		= GSC_VADDR(dev, GSC_REG_OFFSET(AO16_GSC_BOR));
		dev->vaddr.gsc_csr_32		= GSC_VADDR(dev, GSC_REG_OFFSET(AO16_GSC_CSR));
		dev->vaddr.gsc_for_32		= GSC_VADDR(dev, GSC_REG_OFFSET(AO16_GSC_FOR));
		dev->vaddr.gsc_odbr_32		= GSC_VADDR(dev, GSC_REG_OFFSET(AO16_GSC_ODBR));
		dev->vaddr.gsc_srr_32		= GSC_VADDR(dev, GSC_REG_OFFSET(AO16_GSC_SRR));

		// Data cache initialization.
		dev->cache.gsc_for_32		= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_for_32);

		dev->cache.auto_cal_ms		= 5000;
		dev->cache.differential		= (dev->cache.gsc_for_32 & 0x100000) ? 1 : 0;
		dev->cache.fifo_size		= _256K;
		dev->cache.fsamp_max		= 450000L;
		dev->cache.initialize_ms	= 3;
		dev->cache.nclk_mask		= 0x1FF;
		dev->cache.nclk_max			= 0x1FF;
		dev->cache.nclk_min			= 0;
		dev->cache.nrate_mask		= 0x3FFFF;
		dev->cache.nrate_max		= 0x3FFFF;

		_fref_default_compute(dev);
		_cable_compute(dev);
		_channels_compute(dev);
		_dmdma_compute(dev);
		_filters_compute(dev);
		_volt_range_compute(dev);

		dev->cache.fsamp_min		= dev->cache.fref_default / dev->cache.nrate_max;
		dev->cache.nrate_min		= dev->cache.fref_default / dev->cache.fsamp_max;

		// Initialize additional resources.
		ret	= dev_irq_create(dev);
		if (ret)	break;

		ret	= dev_io_create(dev);
		if (ret)	break;

		dma	= GSC_DMA_SEL_STATIC
			| GSC_DMA_CAP_BMDMA_WRITE
			| GSC_DMA_CAP_DMDMA_WRITE;
		ret	= gsc_dma_create(dev, dma, dma);
		break;
	}

	return(ret);
}



/******************************************************************************
*
*	Function:	dev_device_destroy
*
*	Purpose:
*
*		Do everything needed to release the referenced device.
*
*	Arguments:
*
*		dev		The partial data for the device of interest.
*
*	Returned:
*
*		None.
*
******************************************************************************/

void dev_device_destroy(dev_data_t* dev)
{
	if (dev)
	{
		gsc_dma_destroy(dev);
		dev_io_destroy(dev);
		dev_irq_destroy(dev);
		gsc_bar_destroy(&dev->bar);
		os_sem_destroy(&dev->sem);
		os_spinlock_destroy(&dev->spinlock);
		os_pci_master_clear(&dev->pci);
		os_pci_dev_disable(&dev->pci);
	}
}


