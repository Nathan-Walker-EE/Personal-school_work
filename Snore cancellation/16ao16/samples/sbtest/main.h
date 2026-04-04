// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/main.h $
// $Rev: 44920 $
// $Date: 2019-05-16 20:56:10 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: header file

#ifndef	__MAIN_H__
#define	__MAIN_H__

#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "16ao16_main.h"



// #defines *******************************************************************

#define	SIZEOF_ARRAY(a)			(sizeof((a))/sizeof((a)[0]))



// data types *****************************************************************

typedef enum
{
	SERVICE_END_LIST,	// Ends list of service_data_t structures.
	SERVICE_NORMAL,
	SERVICE_REG_MOD,
	SERVICE_REG_READ,
	SERVICE_REG_WRITE,
	SERVICE_REG_SHOW,
	SERVICE_REG_TEST,
	SERVICE_SLEEP,
	SERVICE_SLEEP_MS,
	SERVICE_IOCTL_GET,
	SERVICE_IOCTL_SET
} service_t;

typedef struct
{
	service_t		service;
	unsigned long	cmd;	// IOCTL code
	s32				arg;	// The IOCTL data argument.
	u32				reg;	// The register to access. Use -1 to ignore.
	u32				mask;	// The register bits of interest.
	u32				value;	// The value expected for the bits of interest.
} service_data_t;



// prototypes *****************************************************************

int	auto_calibrate_test(int fd);
int	auto_cal_sts_test(int fd);

int	buffer_clear_test(int fd);
int	buffer_mode_test(int fd);
int	buffer_over_data_test(int fd);
int	buffer_over_frame_test(int fd);
int	buffer_size_test(int fd);
int	buffer_status_test(int fd);
int	burst_enable_test(int fd);
int	burst_ready_test(int fd);
int	burst_trig_src_test(int fd);
int	burst_trigger_test(int fd);

int	cbl_iso_clock_io_test(int fd);		// AO16_IOCTL_CBL_ISO_CLOCK_IO
int	cbl_iso_dac_clk_out_test(int fd);	// AO16_IOCTL_CBL_ISO_DAC_CLK_OUT
int	cbl_iso_trig_out_test(int fd);		// AO16_IOCTL_CBL_ISO_TRIG_OUT
int	cbl_pol_clock_io_test(int fd);		// AO16_IOCTL_CBL_POL_CLOCK_IO
int	cbl_pol_dac_clk_out_test(int fd);	// AO16_IOCTL_CBL_POL_DAC_CLK_OUT
int	cbl_pol_trig_in_test(int fd);		// AO16_IOCTL_CBL_POL_TRIG_IN
int	cbl_pol_trig_out_test(int fd);		// AO16_IOCTL_CBL_POL_TRIG_OUT
int	channel_sel_test(int fd);
int	clock_enable_test(int fd);
int	clock_ready_test(int fd);
int	clock_ref_src_test(int fd);
int	clock_src_test(int fd);
int	clock_sw_test(int fd);

int	data_format_test(int fd);

int	ground_sense_test(int fd);

int	initialize_test(int fd);
int	irq_sel_test(int fd);

int	load_ready_test(int fd);
int	load_request_test(int fd);

int	nclk_test(int fd);
int	nrate_test(int fd);

int	output_mode_test(int fd);

int	query_test(int fd);

int	range_test(int fd);
int	reg_mod_test(int fd);
int	reg_read_test(int fd);
int	reg_write_test(int fd);

int	service_ioctl_reg_get_list(int fd, const service_data_t* list);
int	service_ioctl_set_get_list(int fd, const service_data_t* list);
int	service_ioctl_set_reg_list(int fd, const service_data_t* list);

int	tx_io_abort_test(int fd);			// AO16_IOCTL_TX_IO_ABORT
int	tx_io_mode_test(int fd);
int	tx_io_over_data_test(int fd);
int	tx_io_over_frame_test(int fd);
int	tx_io_timeout_test(int fd);

int	wait_cancel_test(int fd);			// AO16_IOCTL_WAIT_CANCEL
int	wait_event_test(int fd);			// AO16_IOCTL_WAIT_EVENT
int	wait_status_test(int fd);			// AO16_IOCTL_WAIT_STATUS

int	xcvr_test(int fd);



#endif
