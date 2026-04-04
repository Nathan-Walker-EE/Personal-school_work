// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/main.c $
// $Rev: 44920 $
// $Date: 2019-05-16 20:56:10 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// variables ******************************************************************

static	s32	_continuous		= 0;
static	s32	_def_index		= 0;
static	s32	_minute_limit	= 0;
static	s32	_test_limit		= 0;



//*****************************************************************************
static int _parse_args(int argc, char** argv)
{
	static const gsc_arg_item_t	list[]	=
	{
		//	type				var				arg			values	desc
		{	GSC_ARG_S32_FLAG,	&_continuous,	"-c",		{ -1 },	"Continue testing until an error occurs."		},
		{	GSC_ARG_S32_FLAG,	&_continuous,	"-C",		{ +1 },	"Continue testing even if errors occur."		},
		{	GSC_ARG_S32_MIN,	&_minute_limit,	"-m",		{ 1 },	"Continue testing for up to # minutes."			},
		{	GSC_ARG_S32_MIN,	&_test_limit,	"-n",		{ 1 },	"Continue testing for up to # iterations."		},
		{	GSC_ARG_DEV_INDEX,	&_def_index,	"index",	{ 0 },	"The zero based index of the device to access."	}
	};

	static const gsc_arg_set_t	set	=
	{
		/* name	*/	"sbtest",
		/* desc	*/	"Single Board Test Application",
		/* qty	*/	SIZEOF_ARRAY(list),
		/* list	*/	list
	};

	int	errs;

	errs	= gsc_args_parse(argc, argv, &set);
	return(errs);
}



//*****************************************************************************
static int _perform_tests(int fd)
{
	int	errs	= 0;

	errs	+= reg_read_test(fd);			// AO16_IOCTL_REG_READ
	errs	+= reg_write_test(fd);			// AO16_IOCTL_REG_WRITE
	errs	+= reg_mod_test(fd);			// AO16_IOCTL_REG_MOD
	errs	+= query_test(fd);				// AO16_IOCTL_QUERY
	errs	+= initialize_test(fd);			// AO16_IOCTL_INITIALIZE
	errs	+= auto_calibrate_test(fd);		// AO16_IOCTL_AUTO_CALIBRATE
	errs	+= auto_cal_sts_test(fd);		// AO16_IOCTL_AUTO_CAL_STS

	errs	+= tx_io_abort_test(fd);		// AO16_IOCTL_TX_IO_ABORT
	errs	+= tx_io_mode_test(fd);			// AO16_IOCTL_TX_IO_MODE
	errs	+= tx_io_over_data_test(fd);	// AO16_IOCTL_TX_IO_OVER_DATA
	errs	+= tx_io_over_frame_test(fd);	// AO16_IOCTL_TX_IO_OVER_FRAME
	errs	+= tx_io_timeout_test(fd);		// AO16_IOCTL_TX_IO_TIMEOUT

	errs	+= buffer_clear_test(fd);		// AO16_IOCTL_BUFFER_CLEAR
	errs	+= buffer_mode_test(fd);		// AO16_IOCTL_BUFFER_MODE
	errs	+= buffer_over_data_test(fd);	// AO16_IOCTL_BUFFER_OVER_DATA
	errs	+= buffer_over_frame_test(fd);	// AO16_IOCTL_BUFFER_OVER_FRAME
	errs	+= buffer_size_test(fd);		// AO16_IOCTL_BUFFER_SIZE
	errs	+= buffer_status_test(fd);		// AO16_IOCTL_BUFFER_STATUS
	errs	+= burst_enable_test(fd);		// AO16_IOCTL_BURST_ENABLE
	errs	+= burst_ready_test(fd);		// AO16_IOCTL_BURST_READY
	errs	+= burst_trig_src_test(fd);		// AO16_IOCTL_BURST_TRIG_SRC
	errs	+= burst_trigger_test(fd);		// AO16_IOCTL_BURST_TRIGGER
	errs	+= cbl_iso_clock_io_test(fd);	// AO16_IOCTL_CBL_ISO_CLOCK_IO
	errs	+= cbl_iso_dac_clk_out_test(fd);// AO16_IOCTL_CBL_ISO_DAC_CLK_OUT
	errs	+= cbl_iso_trig_out_test(fd);	// AO16_IOCTL_CBL_ISO_TRIG_OUT
	errs	+= cbl_pol_clock_io_test(fd);	// AO16_IOCTL_CBL_POL_CLOCK_IO
	errs	+= cbl_pol_dac_clk_out_test(fd);// AO16_IOCTL_CBL_POL_DAC_CLK_OUT
	errs	+= cbl_pol_trig_in_test(fd);	// AO16_IOCTL_CBL_POL_TRIG_IN
	errs	+= cbl_pol_trig_out_test(fd);	// AO16_IOCTL_CBL_POL_TRIG_OUT
	errs	+= channel_sel_test(fd);		// AO16_IOCTL_CHANNEL_SEL
	errs	+= clock_enable_test(fd);		// AO16_IOCTL_CLOCK_ENABLE
	errs	+= clock_ready_test(fd);		// AO16_IOCTL_CLOCK_READY
	errs	+= clock_ref_src_test(fd);		// AO16_IOCTL_CLOCK_REF_SRC
	errs	+= clock_src_test(fd);			// AO16_IOCTL_CLOCK_SRC
	errs	+= clock_sw_test(fd);			// AO16_IOCTL_CLOCK_SW
	errs	+= data_format_test(fd);		// AO16_IOCTL_DATA_FORMAT
	errs	+= ground_sense_test(fd);		// AO16_IOCTL_GROUND_SENSE
	errs	+= irq_sel_test(fd);			// AO16_IOCTL_IRQ_SEL
	errs	+= load_ready_test(fd);			// AO16_IOCTL_LOAD_READY
	errs	+= load_request_test(fd);		// AO16_IOCTL_LOAD_REQUEST
	errs	+= nclk_test(fd);				// AO16_IOCTL_NCLK
	errs	+= nrate_test(fd);				// AO16_IOCTL_NRATE
	errs	+= output_mode_test(fd);		// AO16_IOCTL_OUTPUT_MODE
	errs	+= range_test(fd);				// AO16_IOCTL_RANGE
	errs	+= wait_event_test(fd);			// AO16_IOCTL_WAIT_EVENT
	errs	+= wait_cancel_test(fd);		// AO16_IOCTL_WAIT_CANCEL
	errs	+= wait_status_test(fd);		// AO16_IOCTL_WAIT_STATUS
	errs	+= xcvr_test(fd);				// AO16_IOCTL_XCVR_TYPE

	return(errs);
}



//*****************************************************************************
static void _show_access_index(int index)
{
	gsc_label("Accessing Device");
	printf("%d\n", index);
}



//*****************************************************************************
static void _show_time_stamp(void)
{
	const char*	psz;
	struct tm*	stm;
	time_t		tt;

	time(&tt);
	stm	= localtime(&tt);
	psz	= (char*) asctime(stm);
	gsc_label("Time Stamp");
	printf("%s", psz);
}



/******************************************************************************
*
*	Function:	main
*
*	Purpose:
*
*		Control the overall flow of the application.
*
*	Arguments:
*
*		argc			The number of command line arguments.
*
*		argv			The list of command line arguments.
*
*	Returned:
*
*		EXIT_SUCCESS	We tested a device.
*		EXIT_FAILURE	One or more problems were encounterred.
*
******************************************************************************/

int main(int argc, char** argv)
{
	int		errs;
	time_t	exec		= time(NULL);
	long	failures	= 0;
	int		fd			= 0;
	long	hours;
	long	mins;
	time_t	now;
	long	passes		= 0;
	int		qty			= 0;
	int		ret;
	long	secs;
	time_t	t_limit;
	time_t	test;
	long	tests		= 0;

	for (;;)
	{
		gsc_label_init(30);
		test	= time(NULL);
		errs	= _parse_args(argc, argv);

		if (errs)
			break;

		// Introduction

		t_limit	= exec + (_minute_limit * 60);
		os_id_host();
		_show_time_stamp();
		_show_access_index(_def_index);
		errs	+= ao16_init_util(1);
		errs	+= os_id_driver(ao16_open, ao16_read, ao16_close);
		errs	+= ao16_count_boards(1, &qty);

		if ((errs) || (qty <= 0))
			break;

		// Open access to device

		errs	= ao16_open_util(_def_index, 0, -1, 1, &fd);

		if (errs == 0)
		{
			// Device identification

			errs	= ao16_id_device(fd, -1, 1);

			// Perform testing

			errs	+= _perform_tests(fd);

			// Close device access

			errs	+= ao16_close_util(fd, -1, 1);
		}

		// End of processing.

		now	= time(NULL);
		tests++;

		if (errs)
		{
			failures++;
			printf(	"\nRESULTS: FAIL <---  (%d error%s)",
					errs,
					(errs == 1) ? "" : "s");
		}
		else
		{
			passes++;
			printf("\nRESULTS: PASS");
		}

		secs	= (long) now - test;
		hours	= secs / 3600;
		secs	= secs % 3600;
		mins	= secs / 60;
		secs	= secs % 60;
		printf(" (duration %ld:%ld:%02ld)\n", hours, mins, secs);

		secs	= now - exec;
		hours	= secs / 3600;
		secs	= secs % 3600;
		mins	= secs / 60;
		secs	= secs % 60;
		printf(	"SUMMARY: tests %ld, pass %ld, fail %ld"
				" (duration %ld:%ld:%02ld)\n\n",
				tests,
				passes,
				failures,
				hours,
				mins,
				secs);

		if (_continuous == 0)
			break;

		if ((_continuous < 0) && (errs))
			break;

		if ((_minute_limit > 0) && (now >= t_limit))
			break;

		if ((_test_limit > 0) && (tests >= _test_limit))
			break;
	}

	ret	= ((qty <= 0) || (errs) || (failures)) ? EXIT_FAILURE : EXIT_SUCCESS;
	return(ret);
}



