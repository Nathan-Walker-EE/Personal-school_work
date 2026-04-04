// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/buffer_status.c $
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
		{					// Set the buffer size.
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_SIZE,
			/* arg		*/	AO16_BUFFER_SIZE_8,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_2[]	=
	{
		// 1 of 8 data values

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
		// 2 of 8 data values

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

	static const service_data_t	list_4[]	=
	{
		// 3 of 8 data values

		{					// Put data in the buffer.
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0x0000,
			/* value	*/	0x0000
		},

		// 4 of 8 data values

		{					// Put data in the buffer.
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0x0000,
			/* value	*/	0x0000
		},

		// 5 of 8 data values

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

	static const service_data_t	list_5[]	=
	{
		// 6 of 8 data values

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

	static const service_data_t	list_6[]	=
	{
		// 7 of 8 data values

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

	static const service_data_t	list_7[]	=
	{
		// 8 of 8 data values

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

	s32	_1_e;
	s32	_2_1;
	s32	_3_m;
	s32	_4_m;
	s32	_5_3;
	s32	_6_3;
	s32	_7_f;
	int	errs	= 0;
	int	ret;

	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list_1);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_1_e);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_2);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_2_1);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_3);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_3_m);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_4);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_4_m);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_5);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_5_3);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_6);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_6_3);
	errs	+= ret ? 1 : 0;

	errs	+= service_ioctl_set_reg_list(fd, list_7);
	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_STATUS,&_7_f);
	errs	+= ret ? 1 : 0;

	if (errs)
	{
	}
	else if (_1_e != AO16_BUFFER_STATUS_EMPTY)
	{
		errs++;
		printf("FAIL <--- (List 1 results are not EMPTY.)\n");
	}
	else if (_2_1 != AO16_BUFFER_STATUS_1Q_FULL)
	{
		errs++;
		printf("FAIL <--- (List 2 results are not 1Q FULL.)\n");
	}
	else if (_3_m != AO16_BUFFER_STATUS_MEDIUM)
	{
		errs++;
		printf("FAIL <--- (List 3 results are not MEDIUM.)\n");
	}
	else if (_4_m != AO16_BUFFER_STATUS_MEDIUM)
	{
		errs++;
		printf("FAIL <--- (List 4 results are not MEDIUM.)\n");
	}
	else if (_5_3 != AO16_BUFFER_STATUS_3Q_FULL)
	{
		errs++;
		printf("FAIL <--- (List 5 results are not 3Q FULL.)\n");
	}
	else if (_6_3 != AO16_BUFFER_STATUS_3Q_FULL)
	{
		errs++;
		printf("FAIL <--- (List 6 results are not 3Q FULL.)\n");
	}
	else if (_7_f != AO16_BUFFER_STATUS_FULL)
	{
		errs++;
		printf("FAIL <--- (List 7 results are not FULL.)\n");
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	buffer_status_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_BUFFER_STATUS.
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

int buffer_status_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_BUFFER_STATUS");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



