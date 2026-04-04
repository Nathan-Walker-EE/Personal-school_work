// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/include/16ao16_utils.h $
// $Rev: 45050 $
// $Date: 2019-06-04 16:37:58 -0500 (Tue, 04 Jun 2019) $

// 16AO16: Utilities: header file

#ifndef	__16AO16_UTILS_H__
#define	__16AO16_UTILS_H__

#include "16ao16_api.h"
#include "gsc_utils.h"



// prototypes *****************************************************************

// API utility services

int	ao16_close_util			(int fd, int index, int verbose);
int ao16_init_util			(int verbose);	// Must be first call to API Library.
int ao16_ioctl_util			(int fd, int index, int verbose, int request, void* arg);
int	ao16_open_util			(int device, int share, int index, int verbose, int* fd);
int ao16_read_util			(int fd, int index, int verbose, void* dst, size_t bytes);
int	ao16_write_util			(int fd, int index, int verbose, const void* src, size_t bytes);

// IOCTL utility services

int	ao16_auto_cal_sts		(int fd, int index, int verbose, s32* get);						// AO16_IOCTL_AUTO_CAL_STS
int	ao16_auto_calibrate		(int fd, int index, int verbose);								// AO16_IOCTL_AUTO_CALIBRATE
int	ao16_buffer_clear		(int fd, int index, int verbose);								// AO16_IOCTL_BUFFER_CLEAR
int	ao16_buffer_mode		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_BUFFER_MODE
int	ao16_buffer_over_data	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_BUFFER_OVER_DATA
int	ao16_buffer_over_frame	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_BUFFER_OVER_FRAME
int	ao16_buffer_size		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_BUFFER_SIZE
int	ao16_buffer_status		(int fd, int index, int verbose, s32* get);						// AO16_IOCTL_BUFFER_STATUS
int	ao16_burst_enable		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_BURST_ENABLE
int	ao16_burst_ready		(int fd, int index, int verbose, s32* get);						// AO16_IOCTL_BURST_READY
int	ao16_burst_trig_src		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_BURST_TRIG_SRC
int	ao16_burst_trigger		(int fd, int index, int verbose);								// AO16_IOCTL_BURST_TRIGGER
int	ao16_cbl_iso_clock_io	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CBL_ISO_CLOCK_IO
int	ao16_cbl_iso_dac_clk_out(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CBL_ISO_DAC_CLK_OUT
int	ao16_cbl_iso_trig_out	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CBL_ISO_TRIG_OUT
int	ao16_cbl_pol_clock_io	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CBL_POL_CLOCK_IO
int	ao16_cbl_pol_dac_clk_out(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CBL_POL_DAC_CLK_OUT
int	ao16_cbl_pol_trig_in	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CBL_POL_TRIG_IN
int	ao16_cbl_pol_trig_out	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CBL_POL_TRIG_OUT
int	ao16_channel_sel		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CHANNEL_SEL
int	ao16_clock_enable		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CLOCK_ENABLE
int	ao16_clock_ready		(int fd, int index, int verbose, s32* get);						// AO16_IOCTL_CLOCK_READY
int	ao16_clock_ref_src		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CLOCK_REF_SRC
int	ao16_clock_src			(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_CLOCK_SRC
int	ao16_clock_sw			(int fd, int index, int verbose);								// AO16_IOCTL_CLOCK_SW
int	ao16_data_format		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_DATA_FORMAT
int	ao16_ground_sense		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_GROUND_SENSE
int	ao16_initialize			(int fd, int index, int verbose);								// AO16_IOCTL_INITIALIZE
int	ao16_irq_sel			(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_IRQ_SEL
int	ao16_load_ready			(int fd, int index, int verbose, s32* get);						// AO16_IOCTL_LOAD_READY
int	ao16_load_request		(int fd, int index, int verbose);								// AO16_IOCTL_LOAD_REQUEST
int	ao16_nclk				(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_NCLK
int	ao16_nrate				(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_NRATE
int	ao16_output_mode		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_OUTPUT_MODE
int	ao16_query				(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_QUERY
int	ao16_range				(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_RANGE
int	ao16_reg_mod			(int fd, int index, int verbose, u32 reg, u32 value, u32 mask);	// AO16_IOCTL_REG_MOD
int	ao16_reg_read			(int fd, int index, int verbose, u32 reg, u32* value);			// AO16_IOCTL_REG_READ
int	ao16_reg_write			(int fd, int index, int verbose, u32 reg, u32 value);			// AO16_IOCTL_REG_WRITE
int	ao16_tx_io_abort		(int fd, int index, int verbose, s32* get);						// AO16_IOCTL_TX_IO_ABORT
int	ao16_tx_io_mode			(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_TX_IO_MODE
int	ao16_tx_io_over_data	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_TX_IO_OVER_DATA
int	ao16_tx_io_over_frame	(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_TX_IO_OVER_FRAME
int	ao16_tx_io_timeout		(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_TX_IO_TIMEOUT
int	ao16_wait_cancel		(int fd, int index, int verbose, gsc_wait_t* wait);				// AO16_IOCTL_WAIT_CANCEL
int	ao16_wait_event			(int fd, int index, int verbose, gsc_wait_t* wait);				// AO16_IOCTL_WAIT_EVENT
int	ao16_wait_status		(int fd, int index, int verbose, gsc_wait_t* wait);				// AO16_IOCTL_WAIT_STATUS
int	ao16_xcvr_type			(int fd, int index, int verbose, s32 set, s32* get);			// AO16_IOCTL_XCVR_TYPE

// Auxiliary utility services

int	ao16_config_ao			(int fd, int index, int verbose, s32 fsamp);
int	ao16_fsamp_compute		(int fd, int index, int verbose, s32 fsamp, s32* ref_src, s32* nrate, s32* nclk, double* rate);
int	ao16_fsamp_report		(int fd, int index, int verbose, int chan, s32* fsamp);
int	ao16_fsamp_report_all	(int fd, int index, int verbose, s32* fsamp);
int	ao16_id_device			(int fd, int index, int verbose);

int	ao16_count_boards		(int verbose, s32* get);
int	ao16_reg_list			(int fd, int detail);

const gsc_reg_def_t*		ao16_reg_get_def_id			(u32 reg);
const gsc_reg_def_t*		ao16_reg_get_def_index		(int index);
const char*					ao16_reg_get_desc			(u32 reg);
const char*					ao16_reg_get_name			(u32 reg);



#endif
