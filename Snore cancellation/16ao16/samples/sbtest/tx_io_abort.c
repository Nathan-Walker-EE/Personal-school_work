// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/tx_io_abort.c $
// $Rev: 42947 $
// $Date: 2018-06-01 11:16:33 -0500 (Fri, 01 Jun 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



// variables *******************************************************************

static	sem_t	sem_enter;
static	sem_t	sem_exit;



//*****************************************************************************
static int _service_test(int fd)
{
	// There are no persistent bits to test with this service.
	return(0);
}



//*****************************************************************************
static int _abort_none_test(int fd)
{
	int	errs;
	s32	get;
	int	ret;

	ret		= ao16_ioctl(fd, AO16_IOCTL_TX_IO_ABORT, &get);
	errs	= ret ? 1 : 0;

	if ((errs == 0) && (get))
	{
		errs++;
		printf(	"FAIL <---  (%d. got %ld, expected %ld)\n",
				__LINE__,
				(long) get,
				(long) 0);
	}

	return(errs);
}



//*****************************************************************************
static int _tx_thread(void* arg)
{
	char	buffer[1024];
	int		fd		= (int) (long) arg;

	sem_post(&sem_enter);
	ao16_write(fd, buffer, sizeof(buffer));
	sem_post(&sem_exit);

	return(0);
}



//*****************************************************************************
static int _abort_one_test(int fd)
{
	static const service_data_t	list[]	=
	{
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
			/* cmd		*/	AO16_IOCTL_CLOCK_ENABLE,
			/* arg		*/	AO16_CLOCK_ENABLE_NO,
			/* reg		*/	-1,
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
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_SIZE,
			/* arg		*/	AO16_BUFFER_SIZE_8,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int			errs	= 0;
	s32			get;
	int			i;
	int			ret;
	os_thread_t	thread;

	errs	+= service_ioctl_set_reg_list(fd, list);
	sem_init(&sem_enter, 0, 0);
	sem_init(&sem_exit, 0, 0);
	memset(&thread, 0, sizeof(thread));
	errs	+= os_thread_create(
				&thread,
				"Tx I/O Abort",
				_tx_thread,
				(void*) (long) fd);

	// Wait for the thread to execute.
	sem_wait(&sem_enter);

	if (errs == 0)
	{
		for (i = 0; i < 20; )
		{
			ret		= ao16_ioctl(fd, AO16_IOCTL_TX_IO_ABORT, &get);
			errs	+= ret ? 1 : 0;

			if ((errs) || (get))
				break;

			sleep(1);
		}

		if (errs)
		{
		}
		else if (get == 0)
		{
			errs++;
			printf("FAIL <---  (%d. abort not detected)\n", __LINE__);
		}
	}

	// Wait for the thread to finish.
	sem_wait(&sem_exit);

	// Finish up.
	errs	+= os_thread_destroy(&thread);
	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	int errs	= 0;

	errs	+= _abort_none_test(fd);
	errs	+= _abort_one_test(fd);
	return(errs);
}



/******************************************************************************
*
*	Function:	tx_io_abort_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_TX_IO_ABORT.
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

int tx_io_abort_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_TX_IO_ABORT");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}


