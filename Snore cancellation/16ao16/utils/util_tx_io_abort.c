// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_tx_io_abort.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_tx_io_abort
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_TX_IO_ABORT service.
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
*		get		The results are reported here. This may be NULL.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int ao16_tx_io_abort(int fd, int index, int verbose, s32* get)
{
	char		buf[128];
	int			errs	= 0;
	const char*	ptr;
	int			ret;
	s32			set;

	if (verbose)
		gsc_label_index("Tx I/O Abort", index);

	ret		= ao16_ioctl(fd, AO16_IOCTL_TX_IO_ABORT, &set);
	errs	+= ret ? 1 : 0;

	switch (set)
	{
		default:

			errs++;
			ptr	= buf;
			sprintf(buf, "Unrecognized option: 0x%lX", (long) set);
			break;

		case AO16_IO_ABORT_NO:

			ptr	= "No (Not Aborted)";
			break;

		case AO16_IO_ABORT_YES:

			ptr	= "Yes (Aborted)";
			break;
	}

	if (verbose)
		printf("%s  (%s)\n", errs ? "FAIL <---" : "PASS", ptr);

	if (get)
		get[0]	= set;

	return(errs);
}


