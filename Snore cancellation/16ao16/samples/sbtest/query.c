// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/query.c $
// $Rev: 42931 $
// $Date: 2018-05-31 13:03:40 -0500 (Thu, 31 May 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



/******************************************************************************
*
*	Function:	query_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_QUERY.
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

int query_test(int fd)
{
	s32	data;
	s32	count;
	int	errs	= 0;
	int	i;

	gsc_label("AO16_IOCTL_QUERY");
	errs	= ao16_query(fd, -1, 0, AO16_QUERY_COUNT, &count);

	if (count != AO16_QUERY_LAST)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. expected %ld, got %ld, rebuild application)\n",
				__LINE__,
				(long) AO16_QUERY_LAST,
				(long) count);
	}
	else
	{
		for (i = 0; (errs == 0) && (i <= AO16_QUERY_LAST); i++)
			errs	= ao16_query(fd, -1, 0, i, &data);

		if (errs == 0)
			printf("PASS\n");
	}

	return(errs);
}



