// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_tx_io_mode.c $
// $Rev: 43842 $
// $Date: 2018-10-23 14:12:30 -0500 (Tue, 23 Oct 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_tx_io_mode
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_TX_IO_MODE service.
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

int	ao16_tx_io_mode(int fd, int index, int verbose, s32 set, s32* get)
{
	char	buf[128];
	int		errs	= 0;
	int		ret;

	if (verbose)
		gsc_label_index("Tx I/O Mode", index);

	ret		= ao16_ioctl(fd, AO16_IOCTL_TX_IO_MODE, &set);
	errs	+= ret ? 1 : 0;

	switch (set)
	{
		default:

			sprintf(buf, "INVALID: %ld", (long) set);
			break;

		case GSC_IO_MODE_PIO:

			strcpy(buf, "PIO");
			break;

		case GSC_IO_MODE_BMDMA:

			strcpy(buf, "Block Mode DMA");
			break;

		case GSC_IO_MODE_DMDMA:

			strcpy(buf, "Demand Mode DMA");
			break;
	}

	if (verbose)
		printf("%s  (%s)\n", errs ? "FAIL <---" : "PASS", buf);

	if (get)
		get[0]	= set;

	return(errs);
}


