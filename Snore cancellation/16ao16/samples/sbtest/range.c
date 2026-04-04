// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/range.c $
// $Rev: 42931 $
// $Date: 2018-05-31 13:03:40 -0500 (Thu, 31 May 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd, s32 range)
{
	static const service_data_t	list_low[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_RANGE,
			/* arg		*/	AO16_RANGE_1_25,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x00000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_RANGE,
			/* arg		*/	AO16_RANGE_2_5,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x10000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_RANGE,
			/* arg		*/	AO16_RANGE_5,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x20000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_RANGE,
			/* arg		*/	AO16_RANGE_10,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x30000
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_high[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_RANGE,
			/* arg		*/	AO16_RANGE_5,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x10000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_RANGE,
			/* arg		*/	AO16_RANGE_10,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x20000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_RANGE,
			/* arg		*/	AO16_RANGE_20,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x30000,
			/* value	*/	0x30000
		},

		{ SERVICE_END_LIST }
	};

	int						errs	= 0;
	const service_data_t*	list;

	errs	+= ao16_initialize(fd, -1, 0);

	if (range == AO16_VOLT_RANGE_LOW)
		list	= list_low;
	else
		list	= list_high;

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
	// This requires additional equipment to test.
	return(0);
}



/******************************************************************************
*
*	Function:	range_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_RANGE.
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

int range_test(int fd)
{
	int	errs	= 0;
	s32	range	= 0;

	gsc_label("AO16_IOCTL_RANGE");
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_VOLT_RANGE, &range);
	errs	+= _service_test(fd, range);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



