// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/irq_sel.c $
// $Rev: 42931 $
// $Date: 2018-05-31 13:03:40 -0500 (Thu, 31 May 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_INIT_DONE,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x700,
			/* value	*/	0x000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_AUTO_CAL_DONE,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x700,
			/* value	*/	0x100
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_BUF_EMPTY,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x700,
			/* value	*/	0x200
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_BUF_1Q_FULL,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x700,
			/* value	*/	0x300
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_BUF_3Q_FULL,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x700,
			/* value	*/	0x400
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_BURST_TRIG_READY,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x700,
			/* value	*/	0x500
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_LOAD_READY,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x700,
			/* value	*/	0x600
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_IRQ_SEL,
			/* arg		*/	AO16_IRQ_LOAD_READY_END,
			/* reg		*/	AO16_GSC_BCR,
			/* mask		*/	0x700,
			/* value	*/	0x700
		},

		{ SERVICE_END_LIST }
	};

	int errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= service_ioctl_reg_get_list(fd, list);

	errs	+= service_ioctl_set_reg_list(fd, list);
	errs	+= service_ioctl_reg_get_list(fd, list);

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	// This is tested via the Wait Event tests.
	return(0);
}



/******************************************************************************
*
*	Function:	irq_sel_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_IRQ_SEL.
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

int irq_sel_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_IRQ_SEL");
	errs	+= _service_test(fd);

	if (errs == 0)
		printf("PASS\n");

	gsc_label_level_inc();
	errs	+= _function_test(fd);
	gsc_label_level_dec();

	return(errs);
}



