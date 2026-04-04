// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/mcao/main.h $
// $Rev: 44918 $
// $Date: 2019-05-16 20:53:55 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: header file

#ifndef	__MAIN_H__
#define	__MAIN_H__

#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "16ao16_main.h"



// #defines *******************************************************************

#define	WF_RAMP				0
#define	WF_SAW				1
#define	WF_SINE				2
#define	WF_SQUARE			3

#define	SIZEOF_ARRAY(a)		(sizeof((a)) / sizeof((a)[0]))



// data types *****************************************************************

typedef struct
{
	int	enable;			// Is the channel enabled?
	int	waveform;		// The channel's assigned waveform.
	int	chan_index;		// The channel index number (0...15).
} chan_data_t;

typedef struct
{
	s32				ao_channels;	// Mask of channels selected for use.
	s32				ao_chan_qty;	// Number of channels selected for use.
	s32				buf_size;		// Size of data buffers.
	s32				chan_cycles;	// The number of output waveform cycles to generate per channel.
	s32				chan_qty;		// Number of installed output channels.
	s32				chan_samps;		// The number of output samples to generate per channel.
	s32				fsamp;			// The desired output sample rate.
	s32				io_mode;
	s32				output_secs;	// Generate cable output for this number of seconds
	s32				show_stats;		// Show statistics when done?
	s32				wave_cps;		// waveform cycles per second
	s32				wave_ramp;		// Generate a ramp wave (/|) on this mask of channels.
	s32				wave_saw;		// Generate a saw tooth wave (|\) on this mask of channels.
	s32				wave_sin;		// Generate a sine wave on this mask of channels.
	s32				wave_square;	// Generate a square wave on this mask of channels.

} settings_t;

typedef struct
{
	// These are for thread status.
	os_thread_t		thread;
	int				started;	// Was the thread successfully started?

	// These are for the designated effort.
	int				start;		// Start working on the designated effort.
	int				stop;		// Stop working on the designated effort.
	int				done;		// Work on the designated effort is done.

	// These are for the work to be done.
	int				errs;
	char			err_buf[128];
	long long		total_bytes;
	long			total_ms;
	settings_t*		settings;
	chan_data_t*	chan_data;

} rx_data_t;

typedef struct
{
	// These are for thread status.
	os_thread_t		thread;
	int				started;	// Was the thread successfully started?

	// These are for the designated effort.
	int				start;		// Start working on the designated effort.
	int				stop;		// Stop working on the designated effort.
	int				done;		// Work on the designated effort is done.

	// These are for the work to be done.
	int				errs;
	char			err_buf[128];
	int				fd;
	long long		total_bytes;
	long			total_ms;

} tx_data_t;



// prototypes *****************************************************************

int		analog_out(int fd, settings_t* settings);

int		rx_startup(rx_data_t* rx);
int		rx_stop(rx_data_t* rx);

int		tx_startup(tx_data_t* tx);
int		tx_stop(tx_data_t* tx);



#endif
