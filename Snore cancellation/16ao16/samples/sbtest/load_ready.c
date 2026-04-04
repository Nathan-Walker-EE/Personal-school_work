// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/load_ready.c $
// $Rev: 42931 $
// $Date: 2018-05-31 13:03:40 -0500 (Thu, 31 May 2018) $

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
	static const service_data_t	list[]	=
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

	s32	_1_yes;
	s32	_2_no;
	int	errs	= 0;
	int	ret;

	errs	+= ao16_initialize(fd, -1, 0);

	ret		= ao16_ioctl(fd, AO16_IOCTL_LOAD_READY,&_1_yes);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list);

	ret		= ao16_ioctl(fd, AO16_IOCTL_LOAD_READY,&_2_no);
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

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	load_ready_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_LOAD_READY.
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

int load_ready_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_LOAD_READY");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



