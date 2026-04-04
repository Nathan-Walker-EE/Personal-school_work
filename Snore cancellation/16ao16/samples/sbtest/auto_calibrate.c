// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/auto_calibrate.c $
// $Rev: 42931 $
// $Date: 2018-05-31 13:03:40 -0500 (Thu, 31 May 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_AUTO_CALIBRATE,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x6000,
			/* value	*/	0x0000
		},

		{ SERVICE_END_LIST }
	};

	int errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list);

	errs	+= service_ioctl_set_reg_list(fd, list);

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	// This requires additional equipment.
	return(0);
}



/******************************************************************************
*
*	Function:	auto_calibrate_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_AUTO_CALIBRATE.
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

int auto_calibrate_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_AUTO_CALIBRATE");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



