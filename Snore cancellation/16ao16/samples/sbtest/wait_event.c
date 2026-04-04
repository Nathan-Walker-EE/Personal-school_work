// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/wait_event.c $
// $Rev: 44920 $
// $Date: 2019-05-16 20:56:10 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// #defines *******************************************************************

#define	_1_SEC		(1000L)
#define	_15_SEC		(15L * 1000L)
#define	_30_SEC		(30L * 1000L)



// variables *******************************************************************

static	int			io_stat;
static	sem_t		sem_enter;
static	sem_t		sem_exit;
static	gsc_wait_t	wait_dst;
static	int			wait_fd;



//*****************************************************************************
static int _service_test(int fd)
{
	// There are no settings bits to test here.
	return(0);
}



//*****************************************************************************
static int	_wait_main_dma(int fd)
{
	static const service_data_t	list[]	=
	{
		{					// Select DMA mode.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_MODE,
			/* arg		*/	GSC_IO_MODE_BMDMA,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_DATA,
			/* arg		*/	AO16_TX_IO_OVER_DATA_IGNORE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_FRAME,
			/* arg		*/	AO16_TX_IO_OVER_FRAME_IGNORE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	u32	buf[1024];
	int	errs	= 0;
	s32	sts		= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);

	// Now perform a DMA read to trigger the wait.

	if (errs == 0)
	{
		sts		= ao16_write(fd, buf, sizeof(buf));
		errs	+= (sts == sizeof(buf)) ? 0 : 1;
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_main_gsc(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_INIT_DONE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_INITIALIZE,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	= service_ioctl_set_reg_list(fd, list);
	return(errs);
}



//*****************************************************************************
static int	_wait_gsc_init_done(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_INIT_DONE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_INITIALIZE,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	= service_ioctl_set_reg_list(fd, list);
	return(errs);
}



//*****************************************************************************
static int	_wait_gsc_auto_cal_done(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_AUTO_CAL_DONE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_AUTO_CALIBRATE,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_gsc_buf_empty(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_BUF_EMPTY,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_REG_WRITE,
			/* cmd		*/	0,
			/* arg		*/	1024,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_CLEAR,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_gsc_buf_1q_full(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_BUF_1Q_FULL,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_SIZE,
			/* arg		*/	AO16_BUFFER_SIZE_1K,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_REG_WRITE,
			/* cmd		*/	0,
			/* arg		*/	256,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_CLEAR,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_gsc_buf_3q_full(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_BUF_3Q_FULL,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_SIZE,
			/* arg		*/	AO16_BUFFER_SIZE_256,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_REG_WRITE,
			/* cmd		*/	0,
			/* arg		*/	200,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_CLEAR,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_gsc_burst_trig_ready(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_BURST_TRIG_READY,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BURST_ENABLE,
			/* arg		*/	AO16_BURST_ENABLE_YES,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BURST_TRIG_SRC,
			/* arg		*/	AO16_BURST_TRIG_SRC_SW,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_REG_WRITE,
			/* cmd		*/	0,
			/* arg		*/	1024,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Enable the clock.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CLOCK_ENABLE,
			/* arg		*/	AO16_CLOCK_ENABLE_YES,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_gsc_load_ready(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_LOAD_READY,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Enable only a single channel.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CHANNEL_SEL,
			/* arg		*/	0x1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Output data with the EOF flag set.
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0x1FFFF,
			/* value	*/	0x10000
		},
		{					// Output data with the EOF flag set.
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0x1FFFF,
			/* value	*/	0x10000
		},
		{					// Select circular mode.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_MODE,
			/* arg		*/	AO16_BUFFER_MODE_CIRC,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Issue a load request.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_LOAD_REQUEST,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Enable the clock.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CLOCK_ENABLE,
			/* arg		*/	AO16_CLOCK_ENABLE_YES,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Issue a load request.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_LOAD_REQUEST,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
#if 0
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
#endif

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_gsc_load_ready_end(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_LOAD_READY_END,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Enable only a single channel.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CHANNEL_SEL,
			/* arg		*/	0x1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Output data with the EOF flag set.
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0x1FFFF,
			/* value	*/	0x10000
		},
		{					// Output data with the EOF flag set.
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0x1FFFF,
			/* value	*/	0x10000
		},
		{					// Select circular mode.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_MODE,
			/* arg		*/	AO16_BUFFER_MODE_CIRC,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Issue a load request.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_LOAD_REQUEST,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Enable the clock.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CLOCK_ENABLE,
			/* arg		*/	AO16_CLOCK_ENABLE_YES,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Issue a load request.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_LOAD_REQUEST,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_io_tx_done(int fd)
{
	u32	buf[1024];
	int	errs	= 0;
	s32	sts		= 0;

	errs	+= ao16_initialize(fd, -1, 0);

	if (errs == 0)
	{
		sts		= ao16_write(fd, buf, sizeof(buf));
		errs	+= (sts == sizeof(buf)) ? 0 : 1;
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_io_tx_error(int fd)
{
	int	errs	= 0;
	int	ret;

	errs	+= ao16_initialize(fd, -1, 0);

	// Now perform a write to trigger the wait.

	if (errs == 0)
	{
		ret	= ao16_write(fd, NULL, 3);

		if (ret >= 0)
		{
			errs++;
			printf("FAIL <---  (%d. expected < 0, got %d)\n", __LINE__, ret);
		}
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int	_wait_io_tx_timeout(int fd)
{
	static const service_data_t	list[]	=
	{
		{					// Select PIO mode.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_MODE,
			/* arg		*/	GSC_IO_MODE_PIO,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_DATA,
			/* arg		*/	AO16_TX_IO_OVER_DATA_IGNORE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_FRAME,
			/* arg		*/	AO16_TX_IO_OVER_FRAME_IGNORE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_TIMEOUT,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_SIZE,
			/* arg		*/	AO16_BUFFER_SIZE_512,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_REG_WRITE,
			/* cmd		*/	0,
			/* arg		*/	1024,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	u32	buf[1024];
	int	errs	= 0;
	int	ret;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list);

	// Now perform a write to trigger the wait.

	if (errs == 0)
	{
		ret	= ao16_write(fd, buf, sizeof(buf));

		if (ret)
		{
			errs++;
			printf(	"FAIL <---  (%d. expected %d, got %d)\n",
					__LINE__,
					0,
					ret);
		}
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int _write_thread(void* arg)
{
	u32		buf[1024];
	int		fd	= (int) (long) arg;
	long	status;

	status	= ao16_write(fd, buf, sizeof(buf));

	if (status == 0)
	{
		io_stat	= 1;
	}
	else
	{
		io_stat	= 2;
		printf("FAIL <---  (%d. write returned %ld, expected 0)\n",
				__LINE__,
				(long) status);
	}

	return(0);
}



//*****************************************************************************
static int	_wait_io_tx_abort(int fd)
{
	static const service_data_t	list[]	=
	{
		{					// Select PIO mode.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_MODE,
			/* arg		*/	GSC_IO_MODE_PIO,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_DATA,
			/* arg		*/	AO16_TX_IO_OVER_DATA_IGNORE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_TIMEOUT,
			/* arg		*/	10,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_SIZE,
			/* arg		*/	AO16_BUFFER_SIZE_256,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_REG_WRITE,
			/* cmd		*/	0,
			/* arg		*/	1024,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int			errs	= 0;
	int			i;
	int			ret;
	s32			set;
	os_thread_t	thread;

	errs	+= ao16_initialize(fd, -1, 0);

	// Verify that the status is an overflow.
	errs	+= service_ioctl_set_reg_list(fd, list);

	// Now start the write thread so we can do the abort.
	memset(&thread, 0, sizeof(thread));

	if (errs == 0)
	{
		errs	+= os_thread_create(	&thread,
										"Tx",
										_write_thread,
										(void*) (long) fd);
	}

	for (i = 0; (errs == 0) && (i < 100); i++)
	{
		set		= 0;
		ret		= ao16_ioctl(fd, AO16_IOCTL_TX_IO_ABORT, &set);
		errs	+= ret ? 1 : 0;

		if (errs)
			break;

		if (set)
			break;

		usleep(100000L);
	}

	// Wait for the thread to report its status.

	for (i = 0; i < 100; i++)
	{
		if (errs)
			break;

		if (io_stat == 0)
		{
			usleep(100000);
			continue;
		}

		if (io_stat == 1)
			break;

		errs++;
		break;
	}

	errs	+= os_thread_destroy(&thread);
	errs	+= ao16_initialize(fd, -1, 0);
	ret		= errs;
	return(ret);
}



//*****************************************************************************
static int	_wait_timeout(int fd)
{
	sleep(2);
	return(0);
}



//*****************************************************************************
static int	_wait_cancel(int fd)
{
	int			errs	= 0;
	int			ret;
	gsc_wait_t	wait;

	wait.flags			= 0;
	wait.main			= 0xFFFFFFFF;
	wait.gsc			= 0xFFFFFFFF;
	wait.alt			= 0xFFFFFFFF;
	wait.io				= 0xFFFFFFFF;
	wait.timeout_ms		= 0;
	wait.count			= 0;
	ret		= ao16_ioctl(wait_fd, AO16_IOCTL_WAIT_CANCEL, &wait);
	errs	+= ret ? 1 : 0;
	return(errs);
}



// variables *******************************************************************

static const struct
{
	const char*	name;
	int			(*function)(int fd);
	gsc_wait_t	wait;
} wait_src[]	=
{
	// name						function					flags			main							...			timeout_ms
	{ "DMA0/DMA1",				_wait_main_dma,				{ 0,			GSC_WAIT_MAIN_DMA0 |
																			GSC_WAIT_MAIN_DMA1,				0, 0, 0,	_15_SEC	}	},
	{ "GSC",					_wait_main_gsc,				{ 0,			GSC_WAIT_MAIN_GSC,				0, 0, 0,	_15_SEC	}	},

	// name						function					flags ...		gsc								...			timeout_ms
	{ "Initialization Done",	_wait_gsc_init_done,		{ 0, 0,			AO16_WAIT_GSC_INIT_DONE,		0, 0,		_15_SEC	}	},
	{ "Auto-Cal Done",			_wait_gsc_auto_cal_done,	{ 0, 0,			AO16_WAIT_GSC_AUTO_CAL_DONE,	0, 0,		_15_SEC	}	},
	{ "Buffer Empty",			_wait_gsc_buf_empty,		{ 0, 0,			AO16_WAIT_GSC_BUF_EMPTY,		0, 0,		_15_SEC	}	},
	{ "Buffer 1/4 Full",		_wait_gsc_buf_1q_full,		{ 0, 0,			AO16_WAIT_GSC_BUF_1Q_FULL,		0, 0,		_15_SEC	}	},
	{ "Buffer 3/4 Full",		_wait_gsc_buf_3q_full,		{ 0, 0,			AO16_WAIT_GSC_BUF_3Q_FULL,		0, 0,		_15_SEC	}	},
	{ "Burst Trigger Ready",	_wait_gsc_burst_trig_ready,	{ 0, 0,			AO16_WAIT_GSC_BURST_TRIG_READY,	0, 0,		_15_SEC	}	},
	{ "Load Ready",				_wait_gsc_load_ready,		{ 0, 0,			AO16_WAIT_GSC_LOAD_READY,		0, 0,		_15_SEC	}	},
	{ "Load Ready End",			_wait_gsc_load_ready_end,	{ 0, 0,			AO16_WAIT_GSC_LOAD_READY_END,	0, 0,		_15_SEC	}	},

	// name						function					flags ...		io											timeout_ms
	{ "Tx Done",				_wait_io_tx_done,			{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_DONE,						_15_SEC	}	},
	{ "Tx Error",				_wait_io_tx_error,			{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_ERROR,						_15_SEC	}	},
	{ "Tx Timeout",				_wait_io_tx_timeout,		{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_TIMEOUT,					_15_SEC	}	},
	{ "Tx Abort",				_wait_io_tx_abort,			{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_ABORT,						_15_SEC	}	},

	// function					flags ...																				timeout_ms
	{ "Wait Timeout",			_wait_timeout,				{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_DONE,						_1_SEC	}	},
	{ "Wait Cancel",			_wait_cancel,				{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_DONE,						_30_SEC	}	},

	// terminate list
	{ NULL, NULL, { 1 } }
};



//*****************************************************************************
static int _wait_thread(void* arg)
{
	int	status;

	sem_post(&sem_enter);
	status	= ao16_ioctl(wait_fd, AO16_IOCTL_WAIT_EVENT, &wait_dst);

	if (status)
		wait_dst.flags	= 0xFFFFFFFF;

	sem_post(&sem_exit);
	return(0);
}


//*****************************************************************************
static int _process_thread(int index)
{
	char		buf[64];
	int			errs	= 0;
	int			i;
	int			ret;
	int			status;
	os_thread_t	thread;
	gsc_wait_t	wait;

	sem_init(&sem_enter, 0, 0);
	sem_init(&sem_exit, 0, 0);
	memset(&thread, 0, sizeof(thread));
	sprintf(buf, "wait event %d", index);
	wait_dst	= wait_src[index].wait;
	errs		+= os_thread_create(&thread, buf, _wait_thread, NULL);

	// Wait for the thread to become waiting.
	sem_wait(&sem_enter);

	if (errs == 0)
	{
		wait	= wait_src[index].wait;

		for (i = 0; i < 100; i++)
		{
			status		= ao16_ioctl(wait_fd, AO16_IOCTL_WAIT_STATUS, &wait);

			if (status)
			{
				printf(	"FAIL <---  (%d. status request %d: errno %d)\n",
						__LINE__,
						index,
						errno);
				errs++;
				break;
			}

			if (wait.count == (index + 1))
				break;

			usleep(100000);
		}
	}

	// Initiate the respective event.
	status	= wait_src[index].function(wait_fd);

	if (status < 0)
	{
	}
	else
	{
		errs	+= status;

		// Verify that the wait ended as expected.

		if (wait_src[index].wait.flags == 0xFFFFFFFF)
		{
		}
		else
		{
			// Wait for the resumed thread to run.
			sem_wait(&sem_exit);

			for (i = 0;; i++)
			{
				if (wait_dst.flags)
					break;

				if (i >= 100)
				{
					printf(	"FAIL <---  (%d. thread failed to resume)\n",
							__LINE__);
					errs++;
					break;
				}

				usleep(100000);
			}

			if (wait_src[index].wait.timeout_ms == _1_SEC)
			{
				if (wait_dst.flags != GSC_WAIT_FLAG_TIMEOUT)
				{
					printf(	"FAIL <---  (%d. flag: expect 0x%lX, got 0x%lX)\n",
							__LINE__,
							(long) GSC_WAIT_FLAG_TIMEOUT,
							(long) wait_dst.flags);
					errs++;
				}
			}
			else if (wait_src[index].wait.timeout_ms == _15_SEC)
			{
				if (wait_dst.flags != GSC_WAIT_FLAG_DONE)
				{
					printf(	"FAIL <---  (%d. flag: expect 0x%lX, got 0x%lX)\n",
							__LINE__,
							(long) GSC_WAIT_FLAG_DONE,
							(long) wait_dst.flags);
					errs++;
				}
			}
			else if (wait_src[index].wait.timeout_ms == _30_SEC)
			{
				if (wait_dst.flags != GSC_WAIT_FLAG_CANCEL)
				{
					printf(	"FAIL <---  (%d. flag: expect 0x%lX, got 0x%lX)\n",
							__LINE__,
							(long) GSC_WAIT_FLAG_TIMEOUT,
							(long) wait_dst.flags);
					errs++;
				}
			}
			else
			{
				printf("FAIL <---  (%d. INTERNAL ERROR)\n", __LINE__);
				errs++;
			}
		}
	}

	wait.flags			= 0;
	wait.main			= 0xFFFFFFFF;
	wait.gsc			= 0xFFFFFFFF;
	wait.alt			= 0xFFFFFFFF;
	wait.io				= 0xFFFFFFFF;
	wait.timeout_ms		= 0;
	wait.count			= 0;
	ret		= ao16_ioctl(wait_fd, AO16_IOCTL_WAIT_CANCEL, &wait);
	errs	+= ret ? 1 : 0;
	errs	+= os_thread_destroy(&thread);

	if ((status >= 0) && (errs == 0))
		printf("PASS\n");

	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	int	errs	= 0;
	int	i;

	gsc_label_level_inc();

	// Create threads awaiting each of the source crtiteria
	// and verify that it resumes when the criteria is met.

	for (i = 0; wait_src[i].wait.flags == 0; i++)
	{
		gsc_label(wait_src[i].name);
		errs	+= ao16_initialize(fd, -1, 0);

		wait_fd	= fd;
		memset(&wait_dst, 0, sizeof(wait_dst));
		errs	+= _process_thread(i);

		errs	+= ao16_initialize(fd, -1, 0);
	}

	gsc_label_level_dec();
	return(errs);
}



/******************************************************************************
*
*	Function:	wait_event_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_WAIT_EVENT.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int wait_event_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_WAIT_EVENT");
	errs	+= _service_test(fd);

	if (errs == 0)
		printf("PASS\n");

	errs	+= _function_test(fd);

	return(errs);
}


