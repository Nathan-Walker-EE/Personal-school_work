// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/initialize.c $
// $Rev: 42931 $
// $Date: 2018-05-31 13:03:40 -0500 (Thu, 31 May 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd)
{
	static const service_data_t	list[]	=
	{
		{				// Perform the initialization.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_INITIALIZE,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x1000,
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
	static const service_data_t	list[]	=
	{
		{				// Adjust the voltage range.
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x30000
		},
		{				// Make sure the value is correct.
			/* service	*/	SERVICE_REG_TEST,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x30000
		},
		{				// Perform the initialization and chech the results.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_INITIALIZE,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x00000
		},

		{ SERVICE_END_LIST }
	};

	int errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list);

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	initialize_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_INITIALIZE.
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

int initialize_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_INITIALIZE");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



