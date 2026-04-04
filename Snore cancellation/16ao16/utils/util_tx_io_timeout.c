// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_tx_io_timeout.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_tx_io_timeout
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_TX_IO_TIMEOUT service.
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

int	ao16_tx_io_timeout(int fd, int index, int verbose, s32 set, s32* get)
{
	char		buf[128];
	int			errs	= 0;
	const char*	ptr;
	int			ret;

	if (verbose)
		gsc_label_index("Tx I/O Timeout", index);

	ret		= ao16_ioctl(fd, AO16_IOCTL_TX_IO_TIMEOUT, &set);
	errs	+= ret ? 1 : 0;

	if (verbose)
	{
		if (set == -1)
		{
			ptr	= "This service is not supported by this board.";
		}
		else if (set == AO16_IO_TIMEOUT_INFINITE)
		{
			ptr	= "infinite/never timeout";
		}
		else
		{
			sprintf(buf, "%ld second%s", (long) set, (set == 1) ? "" : "s");
			ptr	= buf;
		}

		printf("%s  (%s)\n", errs ? "FAIL <---" : "PASS", ptr);
	}

	if (get)
		get[0]	= set;

	return(errs);
}


