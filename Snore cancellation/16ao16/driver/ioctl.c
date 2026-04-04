// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/ioctl.c $
// $Rev: 44907 $
// $Date: 2019-05-16 20:31:04 -0500 (Thu, 16 May 2019) $

// 16AO16: Device Driver: source file

#include "main.h"



// #defines *******************************************************************

#define	INIT_START	0x8000



// data types *****************************************************************

typedef struct
{
	s32	arg;
	u32	val;
} _arg_val_t;



//*****************************************************************************
static void _bcr_mod(dev_data_t* dev, u32 value, u32 mask)
{
	os_reg_mem_mx_u32(dev, dev->vaddr.gsc_bcr_32, value, mask);
}



//*****************************************************************************
static int _query(dev_data_t* dev, s32* arg)
{
	switch (arg[0])
	{
		default:							arg[0]	= AO16_IOCTL_QUERY_ERROR;		break;
		case AO16_QUERY_AUTO_CAL_MS:		arg[0]	= dev->cache.auto_cal_ms;		break;
		case AO16_QUERY_CABLE_INVERT_4:		arg[0]	= dev->cache.cable_invert_4;	break;
		case AO16_QUERY_CABLE_PASSIVE_3:	arg[0]	= dev->cache.cable_passive_3;	break;
		case AO16_QUERY_CHANNEL_MASK:		arg[0]	= dev->cache.channel_mask;		break;
		case AO16_QUERY_CHANNEL_MAX:		arg[0]	= dev->cache.channels_max;		break;
		case AO16_QUERY_CHANNEL_QTY:		arg[0]	= dev->cache.channel_qty;		break;
		case AO16_QUERY_COUNT:				arg[0]	= AO16_QUERY_LAST;				break;
		case AO16_QUERY_DEVICE_TYPE:		arg[0]	= dev->board_type;				break;
		case AO16_QUERY_DIFFERENTIAL:		arg[0]	= dev->cache.differential;		break;
		case AO16_QUERY_DMDMA:				arg[0]	= dev->cache.dmdma;				break;
		case AO16_QUERY_FIFO_SIZE:			arg[0]	= dev->cache.fifo_size;			break;
		case AO16_QUERY_FILTER:				arg[0]	= dev->cache.filter;			break;
		case AO16_QUERY_FREF_DEFAULT:		arg[0]	= dev->cache.fref_default;		break;
		case AO16_QUERY_FSAMP_MAX:			arg[0]	= dev->cache.fsamp_max;			break;
		case AO16_QUERY_FSAMP_MIN:			arg[0]	= dev->cache.fsamp_min;			break;
		case AO16_QUERY_INIT_MS:			arg[0]	= dev->cache.initialize_ms;		break;
		case AO16_QUERY_LAST:				arg[0]	= AO16_QUERY_LAST;				break;
		case AO16_QUERY_NCLK_MASK:			arg[0]	= dev->cache.nclk_mask;			break;
		case AO16_QUERY_NCLK_MAX:			arg[0]	= dev->cache.nclk_max;			break;
		case AO16_QUERY_NCLK_MIN:			arg[0]	= dev->cache.nclk_min;			break;
		case AO16_QUERY_NRATE_MASK:			arg[0]	= dev->cache.nrate_mask;		break;
		case AO16_QUERY_NRATE_MAX:			arg[0]	= dev->cache.nrate_max;			break;
		case AO16_QUERY_NRATE_MIN:			arg[0]	= dev->cache.nrate_min;			break;
		case AO16_QUERY_VOLT_RANGE:			arg[0]	= dev->cache.volt_range;		break;
	}

	return(0);
}



//*****************************************************************************
int initialize_ioctl(dev_data_t* dev, void* arg)
{
	int		i;
	long	ms;
	u32		reg;
	int		ret;

	// Manually wait for completion to insure that initialization completes
	// even if we get a signal, such as ^c.
	os_reg_mem_tx_u32(dev, dev->vaddr.gsc_bcr_32, INIT_START);
	ms	= dev->cache.initialize_ms + 5000;
	ret	= gsc_poll_u32(dev, ms, dev->vaddr.gsc_bcr_32, INIT_START, 0);
	reg	= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bcr_32);

	if (ret)
	{
		printk(	"%s: INITIALIZATION TIMED OUT (%ld ms)\n",
				dev->model,
				(long) ms);
	}

	if (reg & INIT_START)
	{
		ret	= -EIO;
		printk(	"%s: INITIALIZATION DID NOT COMPLETE (%ld ms)\n",
				dev->model,
				(long) ms);
	}

	// Initialize the software settings.

	for (i = 0; i < DEV_IO_STREAM_QTY; i++)
	{
		if (dev->io.io_streams[i])
		{
			if (dev->io.io_streams[i]->dev_io_sw_init)
			{
				(dev->io.io_streams[i]->dev_io_sw_init)(dev, dev->io.io_streams[i]);
			}
		}
	}

	return(ret);
}



//*****************************************************************************
static int _auto_cal_start(dev_data_t* dev, void* arg)
{
	#define	AUTO_CAL_START	0x2000

	// Initiate auto-calibration.
	os_reg_mem_mx_u32(NULL, dev->vaddr.gsc_bcr_32, AUTO_CAL_START, AUTO_CAL_START);
	return(0);
}



//*****************************************************************************
static int _auto_calibrate(dev_data_t* dev, void* arg)
{
	#define	BCR_IRQ_AUTO_CAL_DONE	0x0100
	#define	BCR_IRQ_MASK			0x0700
	#define	BCR_IRQ_ACTIVE			0x0800
	#define	AUTO_CAL_FAIL			0x4000

	long		ms;
	u32			reg;
	int			ret;
	os_sem_t	sem;
	gsc_wait_t	wt;

	// Safely select the Auto-Cal Done interrupt.
	_bcr_mod(dev, BCR_IRQ_AUTO_CAL_DONE, BCR_IRQ_MASK);
	_bcr_mod(dev, 0, BCR_IRQ_ACTIVE);

	// Wait for the local interrupt.
	os_sem_create(&sem);	// dummy, required for wait operations.
	ms				= dev->cache.auto_cal_ms + 5000;
	memset(&wt, 0, sizeof(wt));
	wt.flags		= GSC_WAIT_FLAG_INTERNAL;
	wt.gsc			= AO16_WAIT_GSC_AUTO_CAL_DONE;
	wt.timeout_ms	= ms;
	ret				= gsc_wait_event(dev, &wt, _auto_cal_start, NULL, &sem);
	os_sem_destroy(&sem);

	if (wt.flags & GSC_WAIT_FLAG_TIMEOUT)
	{
		ret	= ret ? ret : -ETIMEDOUT;
		printk(	"%s: AUTO-CALIBRATION TIMED OUT (%ld ms).\n",
				dev->model,
				(long) ms);
	}

	// Check the completion status.
	reg	= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bcr_32);

	if (ret)
	{
	}
	else if (reg & AUTO_CAL_START)
	{
		ret	= ret ? ret : -EIO;
		printk(	"%s: AUTO-CALIBRATION DID NOT COMPLETE (%ld ms)\n",
				dev->model,
				(long) ms);
	}
	else if (reg & AUTO_CAL_FAIL)
	{
		ret	= ret ? ret : -EIO;
		printk(	"%s: AUTO-CALIBRATION FAILED (%ld ms)\n",
				dev->model,
				(long) ms);
	}

	return(ret);
}



//*****************************************************************************
static int _auto_cal_sts(dev_data_t* dev, s32* arg)
{
	u32	reg;
	s32	value;

	reg	= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bcr_32);

	if (reg & AUTO_CAL_START)
		value	= AO16_AUTO_CAL_STS_ACTIVE;
	else if (reg & AUTO_CAL_FAIL)
		value	= AO16_AUTO_CAL_STS_FAIL;
	else
		value	= AO16_AUTO_CAL_STS_PASS;

	arg[0]	= value;
	return(0);
}



//*****************************************************************************
static int _tx_io_mode(dev_data_t* dev, s32* arg)
{
	static const s32	list[]	=
	{
		GSC_IO_MODE_BMDMA,
		GSC_IO_MODE_DMDMA,
		GSC_IO_MODE_PIO,
		-1
	};

	int	ret;

	if (dev->cache.dmdma)
		ret	= gsc_s32_list_var(arg, list, &dev->io.tx.io_mode);
	else
		ret	= gsc_s32_list_var(arg, list + 1, &dev->io.tx.io_mode);

	return(ret);
}



//*****************************************************************************
static int _tx_io_over_data(dev_data_t* dev, s32* arg)
{
	static const s32	options[]	=
	{
		AO16_TX_IO_OVER_DATA_IGNORE,
		AO16_TX_IO_OVER_DATA_CHECK,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_var(arg, options, &dev->io.tx.overflow_data);
	return(ret);
}



//*****************************************************************************
static int _tx_io_over_frame(dev_data_t* dev, s32* arg)
{
	static const s32	options[]	=
	{
		AO16_TX_IO_OVER_FRAME_IGNORE,
		AO16_TX_IO_OVER_FRAME_CHECK,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_var(arg, options, &dev->io.tx.overflow_frame);
	return(ret);
}



//*****************************************************************************
static int _tx_io_timeout(dev_data_t* dev, s32* arg)
{
	int	ret;

	ret	= gsc_s32_range_var(
			arg,
			AO16_IO_TIMEOUT_MIN,
			AO16_IO_TIMEOUT_INFINITE,
			&dev->io.tx.timeout_s);
	return(ret);
}



//*****************************************************************************
static int _buffer_clear(dev_data_t* dev, void* arg)
{
	u32	mask;
	int	ret;

	// Clear the buffer.
	mask	= 0x800;
	os_reg_mem_mx_u32(dev, dev->vaddr.gsc_bor_32, mask, mask);
	ret	= gsc_poll_u32(dev, dev->io.tx.timeout_s * 1000, dev->vaddr.gsc_bor_32, mask, 0);

	// Clear the overrun flags.
	mask	= 0x30000;
	os_reg_mem_mx_u32(dev, dev->vaddr.gsc_bor_32, 0, mask);
	return(ret);
}



//*****************************************************************************
static int _buffer_mode(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_BUFFER_MODE_OPEN,
		AO16_BUFFER_MODE_CIRC,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bor_32, 8, 8);
	return(ret);
}



//*****************************************************************************
static int _buffer_over_data(dev_data_t* dev, s32* arg)
{
	static const s32	options[]	=
	{
		AO16_BUFFER_OVER_DATA_NO,
		AO16_BUFFER_OVER_DATA_YES,
		-1	// terminate list
	};

	int	ret;

	if ((arg[0] == AO16_BUFFER_OVER_DATA_CLR) ||
		(arg[0] == AO16_BUFFER_OVER_DATA_CHK))
	{
		ret	= gsc_s32_list_reg(dev, arg, options,dev->vaddr.gsc_bor_32, 16, 16);
	}
	else
	{
		ret	= -EINVAL;
	}

	return(ret);
}



//*****************************************************************************
static int _buffer_over_frame(dev_data_t* dev, s32* arg)
{
	static const s32	options[]	=
	{
		AO16_BUFFER_OVER_FRAME_NO,
		AO16_BUFFER_OVER_FRAME_YES,
		-1	// terminate list
	};

	int	ret;

	if ((arg[0] == AO16_BUFFER_OVER_FRAME_CLR) ||
		(arg[0] == AO16_BUFFER_OVER_FRAME_CHK))
	{
		ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bor_32, 17, 17);
	}
	else
	{
		ret	= -EINVAL;
	}

	return(ret);
}



//*****************************************************************************
static int _buffer_size(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_BUFFER_SIZE_8,
		AO16_BUFFER_SIZE_16,
		AO16_BUFFER_SIZE_32,
		AO16_BUFFER_SIZE_64,
		AO16_BUFFER_SIZE_128,
		AO16_BUFFER_SIZE_256,
		AO16_BUFFER_SIZE_512,
		AO16_BUFFER_SIZE_1K,
		AO16_BUFFER_SIZE_2K,
		AO16_BUFFER_SIZE_4K,
		AO16_BUFFER_SIZE_8K,
		AO16_BUFFER_SIZE_16K,
		AO16_BUFFER_SIZE_32K,
		AO16_BUFFER_SIZE_64K,
		AO16_BUFFER_SIZE_128K,
		AO16_BUFFER_SIZE_256K,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bor_32, 3, 0);
	return(ret);
}



//*****************************************************************************
static int _buffer_status(dev_data_t* dev, u32* arg)
{
	u32	bor;

	bor	= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bor_32);

	if (bor & BUFFER_FULL)
		arg[0]	= AO16_BUFFER_STATUS_FULL;
	else if (bor & BUFFER_FULL_3Q)
		arg[0]	= AO16_BUFFER_STATUS_3Q_FULL;
	else if (bor & BUFFER_EMPTY)
		arg[0]	= AO16_BUFFER_STATUS_EMPTY;
	else if (bor & BUFFER_FULL_1Q)
		arg[0]	= AO16_BUFFER_STATUS_1Q_FULL;
	else
		arg[0]	= AO16_BUFFER_STATUS_MEDIUM;

	return(0);
}



//*****************************************************************************
static int _burst_enable(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_BURST_ENABLE_NO,
		AO16_BURST_ENABLE_YES,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bcr_32, 0, 0);
	return(ret);
}



//*****************************************************************************
static int _burst_ready(dev_data_t* dev, s32* arg)
{
	static const s32	options[]	=
	{
		AO16_BURST_READY_NO,
		AO16_BURST_READY_YES,
		-1	// terminate list
	};

	int	ret;

	arg[0]	= -1;
	ret		= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bcr_32, 1, 1);
	return(ret);
}



//*****************************************************************************
static int _burst_trig_src(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_BURST_TRIG_SRC_SW,
		AO16_BURST_TRIG_SRC_EXT,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bcr_32, 6, 6);
	return(ret);
}



//*****************************************************************************
static int _burst_trigger(dev_data_t* dev, void* arg)
{
	_bcr_mod(dev, 0x4, 0x4);
	return(0);
}



//*****************************************************************************
static int _channel_sel(dev_data_t* dev, void* arg)
{
	int	ret;

	ret	= gsc_s32_range_reg(
			dev,
			arg,
			0,
			dev->cache.channel_mask,
			dev->vaddr.gsc_csr_32,
			15, 0);
	return(ret);
}



//*****************************************************************************
static int _clock_enable(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_CLOCK_ENABLE_NO,
		AO16_CLOCK_ENABLE_YES,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bor_32, 5, 5);
	return(ret);
}



//*****************************************************************************
static int _clock_ready(dev_data_t* dev, s32* arg)
{
	static const s32	options[]	=
	{
		AO16_CLOCK_READY_NO,
		AO16_CLOCK_READY_YES,
		-1	// terminate list
	};

	int	ret;

	arg[0]	= -1;
	ret		= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bor_32, 6, 6);
	return(ret);
}



//*****************************************************************************
static int _clock_ref_src(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_CLOCK_REF_SRC_PRI,
		AO16_CLOCK_REF_SRC_ALT,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_acr_32, 9, 9);
	return(ret);
}



//*****************************************************************************
static int _clock_src(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_CLOCK_SRC_INT,
		AO16_CLOCK_SRC_EXT_SW,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bor_32, 4, 4);
	return(ret);
}



//*****************************************************************************
static int _clock_sw(dev_data_t* dev, void* arg)
{
	os_reg_mem_mx_u32(dev, dev->vaddr.gsc_bor_32, 0x80, 0x80);
	return(0);
}



//*****************************************************************************
static int _data_format(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_DATA_FORMAT_2S_COMP,
		AO16_DATA_FORMAT_OFF_BIN,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bcr_32, 4, 4);
	return(ret);
}



//*****************************************************************************
static int _ground_sense(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_GROUND_SENSE_DISABLE,
		AO16_GROUND_SENSE_REMOTE,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bcr_32, 3, 3);
	return(ret);
}



//*****************************************************************************
static int _irq_sel(dev_data_t* dev, s32* arg)
{
	u32	reg;
	int	ret	= 0;

	if (arg[0] == -1)
	{
		// Retrieve the current setting.
		reg		= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bcr_32);
		arg[0]	= GSC_FIELD_DECODE(reg, 10, 8);
	}
	else
	{
		// Validate the option value passed in.

		switch (arg[0])
		{
			default:

				ret	= -EINVAL;
				break;

			case AO16_IRQ_INIT_DONE:
			case AO16_IRQ_AUTO_CAL_DONE:
			case AO16_IRQ_BUF_EMPTY:
			case AO16_IRQ_BUF_1Q_FULL:
			case AO16_IRQ_BUF_3Q_FULL:
			case AO16_IRQ_BURST_TRIG_READY:
			case AO16_IRQ_LOAD_READY:
			case AO16_IRQ_LOAD_READY_END:

				break;
		}

		if (ret == 0)
		{
			// Clear the status bit and apply the new setting.
			_bcr_mod(dev, arg[0] << 8, 0xF00);
		}
	}

	return(ret);
}



//*****************************************************************************
static int _load_ready(dev_data_t* dev, s32* arg)
{
	static const s32	options[]	=
	{
		AO16_LOAD_READY_NO,
		AO16_LOAD_READY_YES,
		-1	// terminate list
	};

	int	ret;

	arg[0]	= -1;
	ret		= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bor_32, 10, 10);
	return(ret);
}



//*****************************************************************************
static int _load_request(dev_data_t* dev, void* arg)
{
	os_reg_mem_mx_u32(dev, dev->vaddr.gsc_bor_32, 0x200, 0x200);
	return(0);
}



//*****************************************************************************
static int _nclk(dev_data_t* dev, void* arg)
{
	int	ret;

	ret	= gsc_s32_range_reg(
			dev,
			arg,
			dev->cache.nclk_min,
			dev->cache.nclk_max,
			dev->vaddr.gsc_acr_32,
			8, 0);
	return(ret);
}



//*****************************************************************************
static int _nrate(dev_data_t* dev, void* arg)
{
	int	ret;

	ret	= gsc_s32_range_reg(
			dev,
			arg,
			dev->cache.nrate_min,
			dev->cache.nrate_max,
			dev->vaddr.gsc_srr_32,
			17, 0);
	return(ret);
}



//*****************************************************************************
static int _output_mode(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_OUTPUT_MODE_SEQ,
		AO16_OUTPUT_MODE_SIM,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bcr_32, 7, 7);
	return(ret);
}



//*****************************************************************************
static int _output_range_list(dev_data_t* dev, s32* arg, const _arg_val_t* options)
{
	int	i;
	int	ret;
	u32	v;

	if (arg[0] == -1)
	{
		ret	= -EINVAL;
		v	= os_reg_mem_rx_u32(dev, dev->vaddr.gsc_bcr_32);
		v	&= (D16 | D17);
		v	>>= 16;

		for (i = 0; options[i].arg >= 0; i++)
		{
			if (options[i].val == v)
			{
				arg[0]	= options[i].arg;
				ret		= 0;
				break;
			}
		}
	}
	else
	{
		ret	= -EINVAL;

		for (i = 0; options[i].arg >= 0; i++)
		{
			if (options[i].arg == arg[0])
			{
				ret	= 0;
				_bcr_mod(dev, options[i].val << 16, D16 | D17);
				break;
			}
		}
	}

	return(ret);
}



//*****************************************************************************
static int _output_range(dev_data_t* dev, s32* arg)
{
	static _arg_val_t	low[]	=
	{
		{ AO16_RANGE_1_25,	0	},
		{ AO16_RANGE_2_5,	1	},
		{ AO16_RANGE_5,		2	},
		{ AO16_RANGE_10,	3	},
		{ -1, -1 }	// terminate list
	};

	static _arg_val_t	high[]	=
	{
		{ AO16_RANGE_5,		1	},
		{ AO16_RANGE_10,	2	},
		{ AO16_RANGE_20,	3	},
		{ -1, -1 }	// terminate list
	};

	int	ret;

	if (dev->cache.volt_range == AO16_VOLT_RANGE_LOW)
		ret	= _output_range_list(dev, arg, low);
	else
		ret	= _output_range_list(dev, arg, high);

	return(ret);
}



//*****************************************************************************
static int _xcvr_type(dev_data_t* dev, void* arg)
{
	static const s32	options[]	=
	{
		AO16_XCVR_TYPE_TTL,
		AO16_XCVR_TYPE_LVDS,
		-1	// terminate list
	};

	int	ret;

	ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bcr_32, 5, 5);
	return(ret);
}



//*****************************************************************************
static int _tx_io_abort(dev_data_t* dev, s32* arg)
{
	arg[0]	= gsc_write_abort_active_xfer(dev, &dev->io.tx);
	return(0);
}



//*****************************************************************************
static int _cbl_pol_select(dev_data_t* dev, s32* arg, int bit)
{
	static const s32	options[]	=
	{
		AO16_CBL_POL_NORM,
		AO16_CBL_POL_INV,
		-1	// terminate list
	};

	int	ret;

	if (dev->cache.cable_invert_4)
	{
		ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bcr_32, bit, bit);
	}
	else
	{
		switch (arg[0])
		{
			default:

				ret	= -EINVAL;
				break;

			case -1:
			case AO16_CBL_POL_INV:
			case AO16_CBL_POL_NORM:

				ret		= 0;
				arg[0]	= AO16_CBL_POL_NORM;
				break;
		}
	}

	return(ret);
}



//*****************************************************************************
static int _cbl_pol_trig_in(dev_data_t* dev, s32* arg)
{
	int	ret;

	ret	= _cbl_pol_select(dev, arg, 18);
	return(ret);
}



//*****************************************************************************
static int _cbl_pol_trig_out(dev_data_t* dev, s32* arg)
{
	int	ret;

	ret	= _cbl_pol_select(dev, arg, 19);
	return(ret);
}



//*****************************************************************************
static int _cbl_pol_dac_clk_in(dev_data_t* dev, s32* arg)
{
	int	ret;

	ret	= _cbl_pol_select(dev, arg, 20);
	return(ret);
}



//*****************************************************************************
static int _cbl_pol_clock_io(dev_data_t* dev, s32* arg)
{
	int	ret;

	ret	= _cbl_pol_select(dev, arg, 21);
	return(ret);
}



//*****************************************************************************
static int _cbl_iso_select(dev_data_t* dev, s32* arg, int bit)
{
	static const s32	options[]	=
	{
		AO16_CBL_ISO_NORM,
		AO16_CBL_ISO_OUT_0,
		-1	// terminate list
	};

	int	ret;

	if (dev->cache.cable_passive_3)
	{
		ret	= gsc_s32_list_reg(dev, arg, options, dev->vaddr.gsc_bor_32, bit, bit);
	}
	else
	{
		switch (arg[0])
		{
			default:

				ret	= -EINVAL;
				break;

			case -1:
			case AO16_CBL_ISO_NORM:
			case AO16_CBL_ISO_OUT_0:

				ret		= 0;
				arg[0]	= AO16_CBL_ISO_NORM;
				break;
		}
	}

	return(ret);
}



//*****************************************************************************
static int _cbl_iso_trig_out(dev_data_t* dev, s32* arg)
{
	int	ret;

	ret	= _cbl_iso_select(dev, arg, 18);
	return(ret);
}



//*****************************************************************************
static int _cbl_iso_dac_clk_in(dev_data_t* dev, s32* arg)
{
	int	ret;

	ret	= _cbl_iso_select(dev, arg, 19);
	return(ret);
}



//*****************************************************************************
static int _cbl_iso_clock_io(dev_data_t* dev, s32* arg)
{
	int	ret;

	ret	= _cbl_iso_select(dev, arg, 20);
	return(ret);
}



// variables *******************************************************************

const gsc_ioctl_t	dev_ioctl_list[]	=
{
	{ AO16_IOCTL_REG_READ,				(void*) gsc_reg_read_ioctl		},
	{ AO16_IOCTL_REG_WRITE,				(void*) gsc_reg_write_ioctl		},
	{ AO16_IOCTL_REG_MOD,				(void*) gsc_reg_mod_ioctl		},
	{ AO16_IOCTL_QUERY,					(void*) _query					},
	{ AO16_IOCTL_INITIALIZE,			(void*) initialize_ioctl		},
	{ AO16_IOCTL_AUTO_CALIBRATE,		(void*) _auto_calibrate			},
	{ AO16_IOCTL_AUTO_CAL_STS,			(void*) _auto_cal_sts			},
	{ AO16_IOCTL_TX_IO_MODE,			(void*) _tx_io_mode				},
	{ AO16_IOCTL_TX_IO_OVER_DATA,		(void*) _tx_io_over_data		},
	{ AO16_IOCTL_TX_IO_OVER_FRAME,		(void*) _tx_io_over_frame		},
	{ AO16_IOCTL_TX_IO_TIMEOUT,			(void*) _tx_io_timeout			},
	{ AO16_IOCTL_BUFFER_CLEAR,			(void*) _buffer_clear			},
	{ AO16_IOCTL_BUFFER_MODE,			(void*) _buffer_mode			},
	{ AO16_IOCTL_BUFFER_OVER_DATA,		(void*) _buffer_over_data		},
	{ AO16_IOCTL_BUFFER_OVER_FRAME,		(void*) _buffer_over_frame		},
	{ AO16_IOCTL_BUFFER_SIZE,			(void*) _buffer_size			},
	{ AO16_IOCTL_BUFFER_STATUS,			(void*) _buffer_status			},
	{ AO16_IOCTL_BURST_ENABLE,			(void*) _burst_enable			},
	{ AO16_IOCTL_BURST_READY,			(void*) _burst_ready			},
	{ AO16_IOCTL_BURST_TRIG_SRC,		(void*) _burst_trig_src			},
	{ AO16_IOCTL_BURST_TRIGGER,			(void*) _burst_trigger			},
	{ AO16_IOCTL_CHANNEL_SEL,			(void*) _channel_sel			},
	{ AO16_IOCTL_CLOCK_ENABLE,			(void*) _clock_enable			},
	{ AO16_IOCTL_CLOCK_READY,			(void*) _clock_ready			},
	{ AO16_IOCTL_CLOCK_REF_SRC,			(void*) _clock_ref_src			},
	{ AO16_IOCTL_CLOCK_SRC,				(void*) _clock_src				},
	{ AO16_IOCTL_CLOCK_SW,				(void*) _clock_sw				},
	{ AO16_IOCTL_DATA_FORMAT,			(void*) _data_format			},
	{ AO16_IOCTL_GROUND_SENSE,			(void*) _ground_sense			},
	{ AO16_IOCTL_IRQ_SEL,				(void*) _irq_sel				},
	{ AO16_IOCTL_LOAD_READY,			(void*) _load_ready				},
	{ AO16_IOCTL_LOAD_REQUEST,			(void*) _load_request			},
	{ AO16_IOCTL_NCLK,					(void*) _nclk					},
	{ AO16_IOCTL_NRATE,					(void*) _nrate					},
	{ AO16_IOCTL_OUTPUT_MODE,			(void*) _output_mode			},
	{ AO16_IOCTL_RANGE,					(void*) _output_range			},
	{ AO16_IOCTL_XCVR_TYPE,				(void*) _xcvr_type				},
	{ AO16_IOCTL_TX_IO_ABORT,			(void*) _tx_io_abort			},
	{ AO16_IOCTL_WAIT_EVENT,			(void*) gsc_wait_event_ioctl	},
	{ AO16_IOCTL_WAIT_CANCEL,			(void*) gsc_wait_cancel_ioctl	},
	{ AO16_IOCTL_WAIT_STATUS,			(void*) gsc_wait_status_ioctl	},
	{ AO16_IOCTL_CBL_POL_TRIG_IN,		(void*) _cbl_pol_trig_in		},
	{ AO16_IOCTL_CBL_POL_TRIG_OUT,		(void*) _cbl_pol_trig_out		},
	{ AO16_IOCTL_CBL_POL_DAC_CLK_OUT,	(void*) _cbl_pol_dac_clk_in		},
	{ AO16_IOCTL_CBL_POL_CLOCK_IO,		(void*) _cbl_pol_clock_io		},
	{ AO16_IOCTL_CBL_ISO_TRIG_OUT,		(void*) _cbl_iso_trig_out		},
	{ AO16_IOCTL_CBL_ISO_DAC_CLK_OUT,	(void*) _cbl_iso_dac_clk_in		},
	{ AO16_IOCTL_CBL_ISO_CLOCK_IO,		(void*) _cbl_iso_clock_io		},
	{ -1, NULL }
};



/******************************************************************************
*
*	Function:	buffer_size
*
*	Purpose:
*
*		Return the buffer size in samples.
*
*	Arguments:
*
*		dev		The partial data for the device of interest.
*
*		size	The buffer size is returned here.
*
*	Returned:
*
*		0		All went well.
*		< 0		An appropriate error code.
*
******************************************************************************/

int buffer_size(dev_data_t* dev, u32* size)
{
	s32	arg	= -1;
	int	ret	= 0;

	ret	= _buffer_size(dev, &arg);

	switch (arg)
	{
		default:
		case AO16_BUFFER_SIZE_8:	size[0]	=		    8;	break;
		case AO16_BUFFER_SIZE_16:	size[0]	=		   16;	break;
		case AO16_BUFFER_SIZE_32:	size[0]	=		   32;	break;
		case AO16_BUFFER_SIZE_64:	size[0]	=		   64;	break;
		case AO16_BUFFER_SIZE_128:	size[0]	=		  128;	break;
		case AO16_BUFFER_SIZE_256:	size[0]	=		  256;	break;
		case AO16_BUFFER_SIZE_512:	size[0]	=		  512;	break;
		case AO16_BUFFER_SIZE_1K:	size[0]	= 1024L *   1;	break;
		case AO16_BUFFER_SIZE_2K:	size[0]	= 1024L *   2;	break;
		case AO16_BUFFER_SIZE_4K:	size[0]	= 1024L *   4;	break;
		case AO16_BUFFER_SIZE_8K:	size[0]	= 1024L *   8;	break;
		case AO16_BUFFER_SIZE_16K:	size[0]	= 1024L *  16;	break;
		case AO16_BUFFER_SIZE_32K:	size[0]	= 1024L *  32;	break;
		case AO16_BUFFER_SIZE_64K:	size[0]	= 1024L *  64;	break;
		case AO16_BUFFER_SIZE_128K:	size[0]	= 1024L * 128;	break;
		case AO16_BUFFER_SIZE_256K:	size[0]	= 1024L * 256;	break;
	};

	return(ret);
}


