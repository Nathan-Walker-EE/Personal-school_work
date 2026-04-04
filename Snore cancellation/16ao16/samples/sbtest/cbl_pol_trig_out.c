// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/cbl_pol_trig_out.c $
// $Rev: 42931 $
// $Date: 2018-05-31 13:03:40 -0500 (Thu, 31 May 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd, s32 invert)
{
	static const service_data_t	list_1[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CBL_POL_TRIG_OUT,
			/* arg		*/	AO16_CBL_POL_NORM,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x080000,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CBL_POL_TRIG_OUT,
			/* arg		*/	AO16_CBL_POL_INV,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x080000,
			/* value	*/	0x080000
		},

		{ SERVICE_END_LIST }
	};

	static const s32	list_2[3]	=
	{
		AO16_CBL_POL_NORM,
		AO16_CBL_POL_INV,
		-1
	};

	int	err;
	int	errs	= 0;
	int	i;
	int	ret;
	s32	val;

	errs	+= ao16_initialize(fd, -1, 0);

	if (invert)
	{
		errs	+= service_ioctl_set_reg_list(fd, list_1);
		errs	+= service_ioctl_reg_get_list(fd, list_1);

		errs	+= service_ioctl_set_reg_list(fd, list_1);
		errs	+= service_ioctl_reg_get_list(fd, list_1);
	}
	else
	{
		for (i = 0; i <= 2; i++)
		{
			val		= list_2[i];
			ret		= ao16_ioctl(fd, AO16_IOCTL_CBL_POL_TRIG_OUT, &val);
			err		= ret ? 1 : 0;
			errs	+= err;

			if ((err == 0) && (val != AO16_CBL_POL_NORM))
			{
				printf(	"FAIL <---  (Unexpected return value: %ld)\n",
						(long) val);
				errs++;
			}
		}
	}

	errs	+= ao16_initialize(fd, -1, 0);

	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	// Testing this requires additional equipment.
	return(0);
}



/******************************************************************************
*
*	Function:	cbl_pol_trig_out_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_CBL_POL_TRIG_OUT.
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

int cbl_pol_trig_out_test(int fd)
{
	int	errs	= 0;
	s32	invert	= 0;

	gsc_label("AO16_IOCTL_CBL_POL_TRIG_OUT");
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_CABLE_INVERT_4, &invert);
	errs	+= _service_test(fd, invert);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



