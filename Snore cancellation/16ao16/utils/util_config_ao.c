// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_config_ao.c $
// $Rev: 45045 $
// $Date: 2019-06-04 16:05:38 -0500 (Tue, 04 Jun 2019) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_config_ao
*
*	Purpose:
*
*		Configure the basic services.
*
*	Arguments:
*
*		fd		The handle to use to access the driver.
*
*		index	The index of the board to access. Ignore if < 0.
*
*		verbose	Work verbosely?
*
*		fsamp	The desired sample rate.
*
*	Returned:
*
*		>= 0	The number of errors encountered here.
*
******************************************************************************/

int	ao16_config_ao(int fd, int index, int verbose, s32 fsamp)
{
	int	errs	= 0;
	s32	mask;
	s32	nclk;
	s32	nrate;
	s32	ref_src;

	if (verbose)
	{
		gsc_label_index("Output Configuration", index);
		printf("\n");
		gsc_label_level_inc();
	}

	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_CHANNEL_MASK, &mask);

	errs	+= ao16_initialize			(fd, index, verbose);

	errs	+= ao16_tx_io_mode			(fd, index, verbose, GSC_IO_MODE_PIO,				NULL);
	errs	+= ao16_tx_io_over_data		(fd, index, verbose, AO16_TX_IO_OVER_DATA_CHECK,	NULL);
	errs	+= ao16_tx_io_over_frame	(fd, index, verbose, AO16_TX_IO_OVER_FRAME_CHECK,	NULL);
	errs	+= ao16_tx_io_timeout		(fd, index, verbose, AO16_IO_TIMEOUT_DEFAULT,		NULL);

	errs	+= ao16_range				(fd, index, verbose, AO16_RANGE_10,					NULL);
//	errs	+= ao16_auto_calibrate		(fd, index);	// cable must be disconnected for this.

	errs	+= ao16_buffer_mode			(fd, index, verbose, AO16_BUFFER_MODE_OPEN,			NULL);
	errs	+= ao16_buffer_size			(fd, index, verbose, AO16_BUFFER_SIZE_256K,			NULL);
	errs	+= ao16_burst_enable		(fd, index, verbose, AO16_BURST_ENABLE_NO,			NULL);
	errs	+= ao16_burst_trig_src		(fd, index, verbose, AO16_BURST_TRIG_SRC_SW,		NULL);
	errs	+= ao16_channel_sel			(fd, index, verbose, mask,							NULL);
	errs	+= ao16_output_mode			(fd, index, verbose, AO16_OUTPUT_MODE_SIM,			NULL);
	errs	+= ao16_xcvr_type			(fd, index, verbose, AO16_XCVR_TYPE_LVDS,			NULL);
	errs	+= ao16_data_format			(fd, index, verbose, AO16_DATA_FORMAT_OFF_BIN,		NULL);
	errs	+= ao16_cbl_pol_clock_io	(fd, index, verbose, AO16_CBL_POL_NORM,				NULL);
	errs	+= ao16_cbl_pol_dac_clk_out	(fd, index, verbose, AO16_CBL_POL_NORM,				NULL);
	errs	+= ao16_cbl_pol_trig_in		(fd, index, verbose, AO16_CBL_POL_NORM,				NULL);
	errs	+= ao16_cbl_pol_trig_out	(fd, index, verbose, AO16_CBL_POL_NORM,				NULL);
	errs	+= ao16_cbl_iso_clock_io	(fd, index, verbose, AO16_CBL_ISO_NORM,				NULL);
	errs	+= ao16_cbl_iso_dac_clk_out	(fd, index, verbose, AO16_CBL_ISO_NORM,				NULL);
	errs	+= ao16_cbl_iso_trig_out	(fd, index, verbose, AO16_CBL_ISO_NORM,				NULL);
	errs	+= ao16_ground_sense		(fd, index, verbose, AO16_GROUND_SENSE_DISABLE,		NULL);
	errs	+= ao16_fsamp_compute		(fd, index, verbose, fsamp, &ref_src, &nrate, &nclk, NULL);
	errs	+= ao16_clock_src			(fd, index, verbose, AO16_CLOCK_SRC_INT,			NULL);
	errs	+= ao16_clock_ref_src		(fd, index, verbose, ref_src,						NULL);
	errs	+= ao16_nrate				(fd, index, verbose, nrate,							NULL);
	errs	+= ao16_nclk				(fd, index, verbose, nclk,							NULL);
	errs	+= ao16_clock_enable		(fd, index, verbose, AO16_CLOCK_ENABLE_YES,			NULL);

	errs	+= ao16_fsamp_report_all	(fd, index, verbose, NULL);

	errs	+= ao16_buffer_clear		(fd, index, verbose);
	errs	+= ao16_buffer_over_data	(fd, index, verbose, AO16_BUFFER_OVER_DATA_CLR,		NULL);
	errs	+= ao16_buffer_over_frame	(fd, index, verbose, AO16_BUFFER_OVER_FRAME_CLR,	NULL);

	if (verbose)
		gsc_label_level_dec();

	return(errs);
}


