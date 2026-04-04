// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/buffer_over_frame.c $
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
	static const service_data_t	list_1[]	=
	{
		{					// Put the buffer in circular mode.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_MODE,
			/* arg		*/	AO16_BUFFER_MODE_CIRC,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_2[]	=
	{
		// Induce a frame overrun

		{					// Put data in the buffer.
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0x0000,
			/* value	*/	0x0000
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_3[]	=
	{
		// Clear the status.

		{					// Clear the status.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_OVER_FRAME,
			/* arg		*/	AO16_BUFFER_OVER_FRAME_CLR,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	s32	_1_no;
	s32	_2_yes;
	s32	_3_no;
	int	errs	= 0;
	int	ret;

	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list_1);
	_1_no	= AO16_BUFFER_OVER_FRAME_CHK;
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_OVER_FRAME,&_1_no);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_2);
	_2_yes	= AO16_BUFFER_OVER_FRAME_CHK;
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_OVER_FRAME,&_2_yes);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_3);
	_3_no	= AO16_BUFFER_OVER_FRAME_CHK;
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_OVER_FRAME,&_3_no);
	errs	+= ret ? 1 : 0;

	if (errs)
	{
	}
	else if (_1_no != AO16_BUFFER_OVER_FRAME_NO)
	{
		errs++;
		printf("FAIL <--- (List 1 results are not NO.)\n");
	}
	else if (_2_yes != AO16_BUFFER_OVER_FRAME_YES)
	{
		errs++;
		printf("FAIL <--- (List 2 results are not YES.)\n");
	}
	else if (_3_no != AO16_BUFFER_OVER_FRAME_NO)
	{
		errs++;
		printf("FAIL <--- (List 3 results are not NO.)\n");
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	buffer_over_frame_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_BUFFER_OVER_FRAME.
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

int buffer_over_frame_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_BUFFER_OVER_FRAME");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



