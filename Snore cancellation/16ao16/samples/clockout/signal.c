// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/clockout/signal.c $
// $Rev: 42925 $
// $Date: 2018-05-31 13:02:16 -0500 (Thu, 31 May 2018) $

// 16AO16: Sample Application: source file

#include "main.h"




/*****************************************************************************
*
*	Function:	clock_out
*
*		Post the internal clock to the cable interface for the specified
(		number of seconds.
*
*	Arguments:
*
*		fd		The handle to the board to access.
*
*		seconds	Operate for this number of seconds.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
*****************************************************************************/

int clock_out(int fd, int seconds)
{
	int	errs	= 0;
	int	verbose	= 1;

	gsc_label("Clock Out");
	printf("\n");
	gsc_label_level_inc();

	errs	+= ao16_clock_enable(fd, -1, verbose, AO16_CLOCK_ENABLE_YES, NULL);

	gsc_label("Waiting");
	sleep(seconds);
	printf("Done\n");

	errs	+= ao16_clock_enable(fd, -1, verbose, AO16_CLOCK_ENABLE_NO, NULL);

	gsc_label_level_dec();
	return(errs);
}


