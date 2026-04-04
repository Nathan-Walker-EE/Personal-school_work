// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/wait_cancel.c $
// $Rev: 44920 $
// $Date: 2019-05-16 20:56:10 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// #defines *******************************************************************

#define	_15_SEC		(15L * 1000L)



// variables *******************************************************************

static	const gsc_wait_t	wait_src[]	=
{
	// flags		main							...			timeout_ms
	{ 0,			GSC_WAIT_MAIN_DMA0,				0, 0, 0,	_15_SEC	},
	{ 0,			GSC_WAIT_MAIN_DMA1,				0, 0, 0,	_15_SEC	},
	{ 0,			GSC_WAIT_MAIN_GSC,				0, 0, 0,	_15_SEC	},

	// flags ...	gsc								...			timeout_ms
	{ 0, 0,			AO16_WAIT_GSC_INIT_DONE,		0, 0,		_15_SEC	},
	{ 0, 0,			AO16_WAIT_GSC_AUTO_CAL_DONE,	0, 0,		_15_SEC	},
	{ 0, 0,			AO16_WAIT_GSC_BUF_EMPTY,		0, 0,		_15_SEC	},
	{ 0, 0,			AO16_WAIT_GSC_BUF_1Q_FULL,		0, 0,		_15_SEC	},
	{ 0, 0,			AO16_WAIT_GSC_BUF_3Q_FULL,		0, 0,		_15_SEC	},
	{ 0, 0,			AO16_WAIT_GSC_BURST_TRIG_READY,	0, 0,		_15_SEC	},
	{ 0, 0,			AO16_WAIT_GSC_LOAD_READY,		0, 0,		_15_SEC	},
	{ 0, 0,			AO16_WAIT_GSC_LOAD_READY_END,	0, 0,		_15_SEC	},

	// flags ...	io											timeout_ms
	{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_DONE,			 			_15_SEC	},
	{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_ERROR,			 			_15_SEC	},
	{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_TIMEOUT,					_15_SEC	},
	{ 0, 0, 0, 0,	AO16_WAIT_IO_TX_ABORT,						_15_SEC	},

	// terminate list
	{ 1	}
};

static	gsc_wait_t	wait_dst[SIZEOF_ARRAY(wait_src)];
static	int			wait_fd;
static	os_thread_t	wait_thread[SIZEOF_ARRAY(wait_src)];



//*****************************************************************************
static int _service_test(int fd)
{
	// There are no settings bits to test here.
	return(0);
}



//*****************************************************************************
static int _wait_thread(void* arg)
{
	int	i		= (int) (long) arg;
	int	status;

	status		= ao16_ioctl(wait_fd, AO16_IOCTL_WAIT_EVENT, &wait_dst[i]);

	if (status)
		wait_dst[i].flags	= 0xFFFFFFFF;

	return(0);
}



//*****************************************************************************
static int _create_thread_1(int index)
{
	char		buf[64];
	int			errs	= 0;
	int			i;
	int			status;
	gsc_wait_t	wait;

	wait_dst[index]	= wait_src[index];
	sprintf(buf, "wait cancel %d", index);
	errs	+= os_thread_create(	&wait_thread[index],
									buf,
									_wait_thread,
									(void*) (long) index);

	// Wait for the thread to become waiting.

	if (errs == 0)
	{
		wait	= wait_dst[index];

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

			if (wait.count == 1)
				break;

			usleep(100000);
		}

		if ((errs == 0) && (wait.count != 1))
		{
			printf(	"FAIL <---  (%d. count: expect 1, got %ld)\n",
					__LINE__,
					(long) wait.count);
			errs++;
		}
	}

	return(errs);
}



//*****************************************************************************
static int _create_thread_2(int index)
{
	char		buf[64];
	int			errs	= 0;
	int			i;
	int			status;
	gsc_wait_t	wait;

	wait_dst[index]	= wait_src[0];
	sprintf(buf, "wait cancel %d", index);
	errs	+= os_thread_create(	&wait_thread[index],
									buf,
									_wait_thread,
									(void*) (long) index);

	// Wait for the thread to become waiting.

	if (errs == 0)
	{
		wait	= wait_dst[index];

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

		if ((errs == 0) && (wait.count != index + 1))
		{
			printf(	"FAIL <---  (%d. count: expect %d, got %ld)\n",
					__LINE__,
					index + 1,
					(long) wait.count);
			errs++;
		}
	}

	return(errs);
}



//*****************************************************************************
static int _delete_thread_1(int index)
{
	int			errs	= 0;
	int			status;
	gsc_wait_t	wait;

	if (wait_dst[index].flags != 0xFFFFFFFF)
	{
		wait	= wait_dst[index];

		status	= ao16_ioctl(wait_fd, AO16_IOCTL_WAIT_CANCEL, &wait);

		if (status)
		{
			printf(	"FAIL <---  (%d. calcel request %d: errno %d)\n",
					__LINE__,
					index,
					errno);
			errs++;
		}

		if ((errs == 0) && (wait.count != 1))
		{
			printf(	"FAIL <---  (%d. count: expect 1, got %ld)\n",
					__LINE__,
					(long) wait.count);
			errs++;
		}

		errs	+= os_thread_destroy(&wait_thread[index]);
	}

	return(errs);
}



//*****************************************************************************
static int _delete_thread_2(int count)
{
	int			errs	= 0;
	int			i;
	int			status;
	gsc_wait_t	wait;

	wait	= wait_dst[0];
	status	= ao16_ioctl(wait_fd, AO16_IOCTL_WAIT_CANCEL, &wait);

	if (status)
	{
		printf(	"FAIL <---  (%d. calcel request: errno %d)\n",
				__LINE__,
				errno);
		errs++;
	}

	if ((errs == 0) && (wait.count != count))
	{
		printf(	"FAIL <---  (%d. count: expect %d, got %ld)\n",
				__LINE__,
				count,
				(long) wait.count);
		errs++;
	}

	for (i = 0; i < count; i++)
	{
		if (wait_dst[i].flags != 0xFFFFFFFF)
			errs	+= os_thread_destroy(&wait_thread[i]);
	}

	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	int		errs	= 0;
	int		i;

	errs	+= ao16_initialize(fd, -1, 0);

	wait_fd	= fd;
	memset(&wait_dst, 0, sizeof(wait_dst));
	memset(&wait_thread, 0, sizeof(wait_thread));

	// Create threads awaiting each of the source crtiteria.

	for (i = 0; (errs == 0) && (wait_src[i].flags == 0) ; i++)
		errs	+= _create_thread_1(i);

	// Cancel each wait and delete the thread.

	for (i = 0; wait_src[i].flags == 0 ; i++)
		errs	+= _delete_thread_1(i);

	// Create multiple threads awaiting the same criteria.

	for (i = 0; (errs == 0) && (wait_src[i].flags == 0) ; i++)
		errs	+= _create_thread_2(i);

	// Cancel each wait and delete the thread.
	errs	+= _delete_thread_2(SIZEOF_ARRAY(wait_src) - 1);

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	wait_cancel_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_WAIT_CANCEL.
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

int wait_cancel_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_WAIT_CANCEL");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}


