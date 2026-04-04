// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/xcvr.c $
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
			/* cmd		*/	AO16_IOCTL_XCVR_TYPE,
			/* arg		*/	AO16_XCVR_TYPE_TTL,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x20,
			/* value	*/	0x00
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_XCVR_TYPE,
			/* arg		*/	AO16_XCVR_TYPE_LVDS,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x20,
			/* value	*/	0x20
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
	// This requires external equipment to test.
	return(0);
}



/******************************************************************************
*
*	Function:	xcvr_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_XCVR_TYPE.
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

int xcvr_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_XCVR_TYPE");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



