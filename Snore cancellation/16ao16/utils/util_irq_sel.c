// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_irq_sel.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_irq_sel
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_IRQ_SEL service.
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

int	ao16_irq_sel(int fd, int index, int verbose, s32 set, s32* get)
{
	char	buf[128];
	int		errs	= 0;
	int		ret;

	if (verbose)
		gsc_label_index("IRQ Selection", index);

	ret		= ao16_ioctl(fd, AO16_IOCTL_IRQ_SEL, &set);
	errs	+= ret ? 1 : 0;

	switch (set)
	{
		default:

			sprintf(buf, "INVALID: %ld", (long) set);
			break;

		case AO16_IRQ_INIT_DONE:

			strcpy(buf, "Initialization Done");
			break;

		case AO16_IRQ_AUTO_CAL_DONE:

			strcpy(buf, "Auto-Calibration Done");
			break;

		case AO16_IRQ_BUF_EMPTY:

			strcpy(buf, "Buffer Empty");
			break;

		case AO16_IRQ_BUF_1Q_FULL:

			strcpy(buf, "Buffer <= 1/4 Full");
			break;

		case AO16_IRQ_BUF_3Q_FULL:

			strcpy(buf, "Buffer > 1/4 Full");
			break;

		case AO16_IRQ_BURST_TRIG_READY:

			strcpy(buf, "Burst Trigger Ready");
			break;

		case AO16_IRQ_LOAD_READY:

			strcpy(buf, "Load Ready");
			break;

		case AO16_IRQ_LOAD_READY_END:

			strcpy(buf, "Load Ready End");
			break;
	}

	if (verbose)
		printf("%s  (%s)\n", errs ? "FAIL <---" : "PASS", buf);

	if (get)
		get[0]	= set;

	return(errs);
}


