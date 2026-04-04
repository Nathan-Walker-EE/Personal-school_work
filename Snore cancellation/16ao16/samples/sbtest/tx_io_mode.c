// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/tx_io_mode.c $
// $Rev: 43852 $
// $Date: 2018-10-23 14:45:26 -0500 (Tue, 23 Oct 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_MODE,
			/* arg		*/	GSC_IO_MODE_DMDMA,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_MODE,
			/* arg		*/	GSC_IO_MODE_BMDMA,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_MODE,
			/* arg		*/	GSC_IO_MODE_PIO,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int	delta;
	s32	dmdma	= 0;
	int	errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_DMDMA, &dmdma);
	delta	= dmdma ? 0 : 1;

	errs	+= service_ioctl_set_reg_list(fd, list + delta);

	errs	+= service_ioctl_set_reg_list(fd, list + delta);

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	// There aren't simple functional tests we can perform to verify this.
	return(0);
}



/******************************************************************************
*
*	Function:	tx_io_mode_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_TX_IO_MODE.
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

int tx_io_mode_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_TX_IO_MODE");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



