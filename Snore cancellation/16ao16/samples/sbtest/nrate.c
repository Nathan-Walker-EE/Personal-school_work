// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/nrate.c $
// $Rev: 44920 $
// $Date: 2019-05-16 20:56:10 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x3FFFF,
			/* reg		*/	AO16_GSC_SRR,
			/* mask		*/	0x3FFFF,
			/* value	*/	0x3FFFF
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x30000,
			/* reg		*/	AO16_GSC_SRR,
			/* mask		*/	0x3FFFF,
			/* value	*/	0x30000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x0F000,
			/* reg		*/	AO16_GSC_SRR,
			/* mask		*/	0x3FFFF,
			/* value	*/	0x0F000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x00F00,
			/* reg		*/	AO16_GSC_SRR,
			/* mask		*/	0x3FFFF,
			/* value	*/	0x00F00
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x000F0,
			/* reg		*/	AO16_GSC_SRR,
			/* mask		*/	0x3FFFF,
			/* value	*/	0x000F0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x0064,
			/* reg		*/	AO16_GSC_SRR,
			/* mask		*/	0x3FFFF,
			/* value	*/	0x0064
		},

		{ SERVICE_END_LIST }
	};

	int errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= service_ioctl_reg_get_list(fd, list);

	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= service_ioctl_reg_get_list(fd, list);

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	static const service_data_t	list_1[]	=
	{
		{					// Enable one channel only.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CHANNEL_SEL,
			/* arg		*/	0x1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Select PIO mode.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_MODE,
			/* arg		*/	GSC_IO_MODE_PIO,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Select simultaneous outputs.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_OUTPUT_MODE,
			/* arg		*/	AO16_OUTPUT_MODE_SIM,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Set Nrate high for the lowest possible Fsamp.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x3FFFF,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_2[]	=
	{
		{					// Set Nrate lower for the higher Fref.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x1FEFD,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_3[]	=
	{
		{					// Set Nrate higher for the lower Fref.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_NRATE,
			/* arg		*/	0x3FFFF,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	clock_enable[]	=
	{
		{					// Enable the clock.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CLOCK_ENABLE,
			/* arg		*/	AO16_CLOCK_ENABLE_YES,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	clock_disable[]	=
	{
		{					// Clear the buffer and flags.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_CLEAR,
			/* arg		*/	0,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{					// Disable the clock.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CLOCK_ENABLE,
			/* arg		*/	AO16_CLOCK_ENABLE_NO,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	#define	SAMPLES	602

	s32	_1_ne;
	s32	_2_e;
	s32	_3_ne;
	s32	_4_e;
	u32	buf[SAMPLES];
	int	errs	= 0;
	int	ret;
	int	sent;

	// The procedure is to select a sample rate and duration so we can look at
	// the time it takes for the buffer to empty.

	memset(buf, 0, sizeof(buf));
	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list_1);

	// Here, it should take about 1.75 seconds for the buffer to empty.
	errs	+= service_ioctl_set_reg_list(fd, list_2);
	errs	+= service_ioctl_set_reg_list(fd, clock_disable);
	sent	= ao16_write(fd, buf, sizeof(buf));

	if (sent != sizeof(buf))
	{
		errs++;
		printf(	"ERROR: write: send %ld, expected %ld)\n",
				(long) sent,
				(long) SAMPLES);
	}

	errs	+= service_ioctl_set_reg_list(fd, clock_enable);
	sleep(1);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_1_ne);
	errs	+= ret ? 1 : 0;
	sleep(1);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_2_e);
	errs	+= ret ? 1 : 0;
	errs	+= service_ioctl_set_reg_list(fd, clock_disable);

	// Here, it should take about 3.5 seconds for the buffer to empty.
	errs	+= service_ioctl_set_reg_list(fd, list_3);
	errs	+= service_ioctl_set_reg_list(fd, clock_disable);
	sent	= ao16_write(fd, buf, sizeof(buf));

	if (sent != sizeof(buf))
	{
		errs++;
		printf(	"ERROR: write: send %ld, expected %ld)\n",
				(long) sent,
				(long) SAMPLES);
	}

	errs	+= service_ioctl_set_reg_list(fd, clock_enable);
	sleep(3);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_3_ne);
	errs	+= ret ? 1 : 0;
	sleep(1);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_4_e);
	errs	+= ret ? 1 : 0;
	errs	+= service_ioctl_set_reg_list(fd, clock_disable);

	// Now verify all of the results.

	if (errs)
	{
	}
	else if (_1_ne == AO16_BUFFER_STATUS_EMPTY)
	{
		errs++;
		printf("FAIL <--- (Test 1 results are EMPTY.)\n");
	}
	else if (_2_e != AO16_BUFFER_STATUS_EMPTY)
	{
		errs++;
		printf("FAIL <--- (Test 2 results are not EMPTY.)\n");
	}
	else if (_3_ne == AO16_BUFFER_STATUS_EMPTY)
	{
		errs++;
		printf("FAIL <--- (Test 3 results are EMPTY.)\n");
	}
	else if (_4_e != AO16_BUFFER_STATUS_EMPTY)
	{
		errs++;
		printf("FAIL <--- (Test 4 results are not EMPTY.)\n");
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	nrate_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_NRATE.
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

int nrate_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_NRATE");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



