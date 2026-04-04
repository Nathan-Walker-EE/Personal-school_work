// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/load_request.c $
// $Rev: 42947 $
// $Date: 2018-06-01 11:16:33 -0500 (Fri, 01 Jun 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd)
{
	// There are no persistent bits to test with this service.
	return(0);
}



//*****************************************************************************
static int _function_test(int fd)
{
	static const service_data_t	list_1[]	=
	{
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

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_2[]	=
	{
		{					// Select circular mode.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_MODE,
			/* arg		*/	AO16_BUFFER_MODE_CIRC,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_3[]	=
	{
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
		{					// Allow a little settling time.
			/* service	*/	SERVICE_SLEEP_MS,
			/* cmd		*/	0,
			/* arg		*/	10,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	s32	_1_yes;
	s32	_2_no;
	s32	_3_0;
	s32	_4_no;
	int	errs	= 0;
	int	ret;

	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list_1);
	ret		= ao16_ioctl(fd, AO16_IOCTL_LOAD_READY,&_1_yes);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_2);
	ret		= ao16_ioctl(fd, AO16_IOCTL_LOAD_READY,&_2_no);
	errs	+= ret ? 1 : 0;

	_3_0	= ao16_ioctl(fd, AO16_IOCTL_LOAD_REQUEST, NULL);

	// The buffer should empty almost immediately and return to circular mode.
	errs	+= service_ioctl_set_reg_list(fd, list_3);
	ret		= ao16_ioctl(fd, AO16_IOCTL_LOAD_READY, &_4_no);
	errs	+= ret ? 1 : 0;

	if (errs)
	{
	}
	else if (_1_yes != AO16_LOAD_READY_YES)
	{
		errs++;
		printf("FAIL <--- (Test 1 results are not YES.)\n");
	}
	else if (_2_no != AO16_LOAD_READY_NO)
	{
		errs++;
		printf("FAIL <--- (Test 2 results are not NO.)\n");
	}
	else if (_3_0 != 0)
	{
		errs++;
		printf("FAIL <--- (Test 3 did not fail.)\n");
	}
	else if (_4_no != AO16_LOAD_READY_NO)
	{
		errs++;
		printf("FAIL <--- (Test 4 results are not NO.)\n");
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	load_request_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_LOAD_REQUEST.
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

int load_request_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_LOAD_REQUEST");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



