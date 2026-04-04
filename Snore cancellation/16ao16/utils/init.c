// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/init.c $
// $Rev: 44911 $
// $Date: 2019-05-16 20:44:07 -0500 (Thu, 16 May 2019) $

// 16AO16: Utilities: source file

#include "main.h"



/*****************************************************************************
*
*	Function:	ao16_init_util
*
*	Purpose:
*
*		Implement a visual wrapper around the API init call.
*
*	Arguments:
*
*		verbose	Work verbosely?
*
*	Returned:
*
*		>= 0	The number of errors seen.
*
*****************************************************************************/

int ao16_init_util(int verbose)
{
	int	errs;
	int	ret;

	if (verbose)
		gsc_label("API Initialize");

	ret		= ao16_init();
	errs	= ret ? 1 : 0;

	if (verbose)
		printf("%s\n", errs ? "FAIL <---" : "PASS");

	return(errs);
}



