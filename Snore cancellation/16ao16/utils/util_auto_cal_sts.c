// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_auto_cal_sts.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_auto_cal_sts
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_AUTO_CAL_STS service.
*
*	Arguments:
*
*		fd		Use this handle to access the device.
*
*		index	The index of the device to access. Ignore if < 0.
*				This is for display purposes only.
*
*		verbose	Work verbosely?
*
*		get		The current setting is recorded here, if not NULL.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int	ao16_auto_cal_sts(int fd, int index, int verbose, s32* get)
{
	char	buf[128];
	int		errs	= 0;
	int		ret;
	s32		set;

	if (verbose)
		gsc_label_index("Auto-Cal Status", index);

	ret		= ao16_ioctl(fd, AO16_IOCTL_AUTO_CAL_STS, &set);
	errs	+= ret ? 1 : 0;

	switch (set)
	{
		default:

			sprintf(buf, "INVALID: %ld", (long) set);
			break;

		case AO16_AUTO_CAL_STS_ACTIVE:

			strcpy(buf, "Active");
			break;

		case AO16_AUTO_CAL_STS_FAIL:

			strcpy(buf, "Fail");
			break;

		case AO16_AUTO_CAL_STS_PASS:

			strcpy(buf, "Pass");
			break;
	}

	if (verbose)
		printf("%s  (%s)\n", errs ? "FAIL <---" : "PASS", buf);

	if (get)
		get[0]	= set;

	return(errs);
}


