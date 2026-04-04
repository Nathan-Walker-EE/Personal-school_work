// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_nrate.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_nrate
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_NRATE service.
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
*		set		The setting to apply.
*
*		get		The current setting is recorded here, if not NULL.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int ao16_nrate(int fd, int index, int verbose, s32 set, s32* get)
{
	int	errs	= 0;
	int	ret;

	if (verbose)
		gsc_label_index("Nrate", index);

	ret		= ao16_ioctl(fd, AO16_IOCTL_NRATE, &set);
	errs	+= ret ? 1 : 0;

	if (verbose)
	{
		printf("%s  (", errs ? "FAIL <---" : "PASS");
		gsc_label_long_comma(set);
		printf(")\n");
	}

	if (get)
		get[0]	= set;

	return(errs);
}


