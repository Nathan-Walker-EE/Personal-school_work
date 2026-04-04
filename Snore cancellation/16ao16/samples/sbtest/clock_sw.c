// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/clock_sw.c $
// $Rev: 42913 $
// $Date: 2018-05-30 13:41:15 -0500 (Wed, 30 May 2018) $

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
	// This requires external equipment to test.
	return(0);
}



/******************************************************************************
*
*	Function:	clock_sw_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_CLOCK_SW.
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

int clock_sw_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_CLOCK_SW");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



