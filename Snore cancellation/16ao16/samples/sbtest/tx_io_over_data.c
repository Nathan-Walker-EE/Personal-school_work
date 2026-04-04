// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/tx_io_over_data.c $
// $Rev: 42947 $
// $Date: 2018-06-01 11:16:33 -0500 (Fri, 01 Jun 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static int _service_test(int fd)
{
	static const service_data_t	list[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_DATA,
			/* arg		*/	AO16_TX_IO_OVER_DATA_IGNORE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_DATA,
			/* arg		*/	AO16_TX_IO_OVER_DATA_CHECK,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	int errs	= 0;

	errs	+= ao16_initialize(fd, -1, 0);

	errs	+= service_ioctl_set_reg_list(fd, list);

	errs	+= service_ioctl_set_reg_list(fd, list);

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



//*****************************************************************************
static int _function_test(int fd)
{
	static const service_data_t	list_1[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_MODE,
			/* arg		*/	AO16_BUFFER_MODE_OPEN,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BURST_ENABLE,
			/* arg		*/	AO16_BURST_ENABLE_NO,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_CLOCK_ENABLE,
			/* arg		*/	AO16_CLOCK_ENABLE_NO,
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
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_TIMEOUT,
			/* arg		*/	1,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_BUFFER_SIZE,
			/* arg		*/	AO16_BUFFER_SIZE_8,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_2[]	=
	{
		{				// Cause a data overflow
			/* service	*/	SERVICE_REG_MOD,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_ODBR,
			/* mask		*/	0xFFFF,
			/* value	*/	0xFFFF
		},
		{				// Verify that we got an overflow.
			/* service	*/	SERVICE_REG_TEST,
			/* cmd		*/	0,
			/* arg		*/	0,
			/* reg		*/	AO16_GSC_BOR,
			/* mask		*/	0x10000,
			/* value	*/	0x10000
		},
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_DATA,
			/* arg		*/	AO16_TX_IO_OVER_DATA_IGNORE,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	static const service_data_t	list_3[]	=
	{
		{
			/* service	*/	SERVICE_NORMAL,
			/* cmd		*/	AO16_IOCTL_TX_IO_OVER_DATA,
			/* arg		*/	AO16_TX_IO_OVER_DATA_CHECK,
			/* reg		*/	-1,
			/* mask		*/	0,
			/* value	*/	0
		},

		{ SERVICE_END_LIST }
	};

	u32		data[1];
	int		errs	= 0;
	int		i;
	long	l;

	errs	+= ao16_initialize(fd, -1, 0);
	errs	+= service_ioctl_set_reg_list(fd, list_1);

	// Fill the output FIFO.
	data[0]	= 0;

	for (l = 0; l < 8; l++)
	{
		i	= ao16_write(fd, &data, 4);

		if (i != 4)
		{
			errs++;
			printf(	"FAIL <---  (%d. ao16_write(): returned %d, expected 4)\n",
					__LINE__,
					i);
		}
	}

	// Verify that IGNORE works on a data overflow.
	errs	+= service_ioctl_set_reg_list(fd, list_2);

	if (errs == 0)
	{
		i	= ao16_write(fd, &data, 4);

		if (i)
		{
			errs++;
			printf(	"FAIL <---  (%d. ao16_write(): returned %d, expected 0)\n",
					__LINE__,
					i);
		}
	}

	// Verify that CHECK works on a data overflow.
	errs	+= service_ioctl_set_get_list(fd, list_3);

	if (errs == 0)
	{
		i	= ao16_write(fd, &data, 4);

		if (i >= 0)
		{
			errs++;
			printf(	"FAIL <---  (%d. ao16_write(): returned %d, expected an error)\n",
					__LINE__,
					i);
		}
	}

	errs	+= ao16_initialize(fd, -1, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	tx_io_over_data_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_TX_IO_OVER_DATA.
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

int tx_io_over_data_test(int fd)
{
	int	errs	= 0;

	gsc_label("AO16_IOCTL_TX_IO_OVER_DATA");
	errs	+= _service_test(fd);
	errs	+= _function_test(fd);

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



