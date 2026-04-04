// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/mcao/test.c $
// $Rev: 44925 $
// $Date: 2019-05-16 21:10:43 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// variables ******************************************************************

static	chan_data_t		_chan_data[16];
static	rx_data_t		_rx_data;
static	tx_data_t		_tx_data;



//*****************************************************************************
static int _channel_hardware_check(int fd, settings_t* settings)
{
	u32		bit;
	char	buf[32];
	int		bypassed	= 0;	// number of channels bypassed
	int		errs;
	int		i;

	gsc_label("Channel Hardware Check");
	errs	= ao16_query(fd, -1, 0, AO16_QUERY_CHANNEL_QTY, &settings->chan_qty);

	if (errs)
	{
		printf("FAIL <---  (%d. ao16_query() failure)\n", __LINE__);
	}
	else
	{
		for (i = 0; i < 16; i++)
		{
			bit	= 0x1 << i;

			if ((settings->ao_channels & bit) == 0)
				continue;

			if (i < settings->chan_qty)
				continue;

			if (bypassed == 0)
			{
				printf("\n");
				gsc_label_level_inc();
			}

			bypassed++;
			settings->ao_channels	&= ~bit;
			sprintf(buf, "Channel %d", i);
			gsc_label(buf);
			printf("BYPASS  (Channel not insllaed on this device.)\n");
		}

		if (bypassed)
			gsc_label_level_dec();
		else
			printf("PASS\n");
	}

	return(errs);
}



//*****************************************************************************
static int _channel_waveform_check(settings_t* settings, chan_data_t* chan_data)
{
	u32		bit;
	char	buf[32];
	int		bypassed	= 0;	// number of channels bypassed
	int		i;

	gsc_label("Channel Waveform Check");

	for (i = 0; i < 16; i++)
	{
		bit	= 0x1 << i;
		chan_data[i].chan_index	= i;

		if ((settings->ao_channels & bit) == 0)
		{
			chan_data[i].enable	= 0;
			continue;
		}

		if (settings->wave_square & bit)
		{
			chan_data[i].enable		= 1;
			chan_data[i].waveform	= WF_SQUARE;
			continue;
		}

		if (settings->wave_sin & bit)
		{
			chan_data[i].enable		= 1;
			chan_data[i].waveform	= WF_SINE;
			continue;
		}

		if (settings->wave_saw & bit)
		{
			chan_data[i].enable		= 1;
			chan_data[i].waveform	= WF_SAW;
			continue;
		}

		if (settings->wave_ramp & bit)
		{
			chan_data[i].enable		= 1;
			chan_data[i].waveform	= WF_RAMP;
			continue;
		}

		if (bypassed == 0)
		{
			printf("\n");
			gsc_label_level_inc();
		}

		bypassed++;
		settings->ao_channels	&= ~bit;
		chan_data[i].enable		= 0;
		sprintf(buf, "Channel %d", i);
		gsc_label(buf);
		printf("BYPASS  (Channel not assigned a pattern.)\n");
	}

	if (bypassed)
		gsc_label_level_dec();
	else
		printf("PASS\n");

	return(0);
}



//*****************************************************************************
static int _channel_waveform_report(settings_t* settings, chan_data_t* list)
{
	char	buf[32];
	int		errs	= 0;
	int		i;

	gsc_label("Channel Waveforms");
	printf("\n");
	gsc_label_level_inc();

	settings->ao_chan_qty	= 0;

	for (i = 0; i < 16; i++)
	{
		if (list[i].enable == 0)
			continue;

		sprintf(buf, "Channel %d", i);
		gsc_label(buf);

		switch (list[i].waveform)
		{
			default:

				errs++;
				printf("FAIL <---  (%d. INTERNAL ERROR, channel disabled)\n", __LINE__);
				list[i].enable	= 0;
				continue;

			case WF_RAMP:	printf("Ramp wave (/|)\n");			break;
			case WF_SAW:	printf("Sawtooth wave (|\\)\n");	break;
			case WF_SINE:	printf("Sine wave\n");				break;
			case WF_SQUARE:	printf("Square wave\n");			break;
		}

		settings->ao_chan_qty++;
	}

	gsc_label_level_dec();
	return(errs);
}



//*****************************************************************************
static int _io_mode_check(int fd, settings_t* settings)
{
	s32	dmdma	= 0;
	int	errs	= 0;

	for (;;)
	{
		gsc_label("I/O Mode Check");

		if (settings->io_mode == GSC_IO_MODE_DMDMA)
		{
			errs	= ao16_query(fd, -1, 0, AO16_QUERY_DMDMA, &dmdma);

			if (errs)
			{
				printf("FAIL <---  (%d. ao16_query() failure)\n", __LINE__);
				break;
			}

			if (dmdma == 0)
			{
				settings->io_mode	= GSC_IO_MODE_BMDMA;
				printf("ALTERATION  (using BMDMA as DMDMA is not supported.)\n");
				break;
			}
		}

		printf("PASS\n");
		break;
	}

	return(errs);
}



//*****************************************************************************
static int _pre_test_check(settings_t* settings, int errs)
{
	const char*	title	= "ABORTING TEST";

	if (errs)
	{
		errs	= 0;
		gsc_label(title);
		printf("FAIL <--- (Setup Errors)\n");
	}
	else if (settings->ao_channels == 0)
	{
		errs	= 1;
		gsc_label(title);
		printf("FAIL <--- (No channels selected.)\n");
	}
	else
	{
		errs	= 0;
	}

	return(errs);
}



//*****************************************************************************
static void _show_entry(const char* label, s32 value, const char* suffix)
{
	gsc_label(label);
	gsc_label_long_comma(value);

	if (suffix)
		printf(" %s", suffix);

	printf("\n");
}



//*****************************************************************************
static int _waveform_calculations(settings_t* settings)
{
	#define	SAMPLES_PER_CYCLE_MIN	20

	s32	cycles_per_channel;
	s32	samples_per_buffer;
	s32	samples_per_channel;
	s32	samples_per_cycle;
	s32	tmp;

	gsc_label("Waveform Calculations");
	printf("\n");
	gsc_label_level_inc();

	_show_entry("Channels", settings->ao_chan_qty, NULL);

	settings->buf_size		= 1024 * 1024;
	_show_entry("Buffer Size", settings->buf_size, "Bytes");

	samples_per_buffer	= settings->buf_size / 4;
	_show_entry("Buffer Size", samples_per_buffer, "Samples");

	samples_per_channel	= samples_per_buffer / settings->ao_chan_qty;
	_show_entry("Samples/Channel", samples_per_channel, "Samples");

	_show_entry("Fsamp", settings->fsamp, "S/S");

	_show_entry("Waveform Rate", settings->wave_cps, "C/S");

	samples_per_cycle	= settings->fsamp / settings->wave_cps;
	_show_entry("Samples/Cycle", samples_per_cycle, "Samples");

	if (samples_per_cycle < SAMPLES_PER_CYCLE_MIN)
	{
		samples_per_cycle	= SAMPLES_PER_CYCLE_MIN;
		_show_entry("Samples/Cycle", samples_per_cycle, "Samples (Minimum)");

		settings->wave_cps	= settings->fsamp / samples_per_cycle;
		_show_entry("Waveform Rate", settings->wave_cps, "C/S (Adjusted)");
	}

	if (samples_per_cycle > samples_per_channel)
	{
		samples_per_cycle	= samples_per_channel;
		_show_entry("Samples/Cycle", samples_per_cycle, "Samples (Adjusted)");
	}

	cycles_per_channel	= samples_per_channel / samples_per_cycle;
	_show_entry("Cycles/Channel", cycles_per_channel, "Cycles");

	if (cycles_per_channel > settings->wave_cps)
	{
		cycles_per_channel	= settings->wave_cps;
		_show_entry("Cycles/Channel", cycles_per_channel, "Cycles (Adjusted)");
	}

	tmp	= cycles_per_channel * samples_per_cycle;

	if (tmp < samples_per_channel)
	{
		samples_per_channel	= tmp;
		_show_entry("Samples/Channel", samples_per_channel, "Samples (Adjusted)");
	}

	tmp	= samples_per_channel * settings->ao_chan_qty;

	if (tmp < samples_per_buffer)
	{
		samples_per_buffer	= tmp;
		_show_entry("Buffer Size", samples_per_buffer, "Samples (Adjusted)");
	}

	tmp	= samples_per_buffer * 4;

	if (tmp < settings->buf_size)
	{
		settings->buf_size	= tmp;
		_show_entry("Buffer Size", settings->buf_size, "Bytes (Adjusted)");
	}

	gsc_label_level_dec();

	settings->chan_samps	= samples_per_channel;
	settings->chan_cycles	= cycles_per_channel;
	return(0);
}



//*****************************************************************************
static int _buffers_init(settings_t* settings)
{
	int			errs	= 0;
	const char*	factor	= "";
	size_t		qty		= 8;
	size_t		size	= settings->buf_size;

	for (;;)
	{
		gsc_label("Initializing Buffers");

		errs	+= gsc_buf_man_init();

		if (errs)
		{
			printf("FAIL <---  (gsc_buf_man_init() failed)\n");
			break;
		}

		errs	+= gsc_buf_man_setup(qty, size);

		if (errs)
			printf("FAIL <---  ");
		else
			printf("PASS  ");

		printf("(");
		gsc_label_long_comma(qty);
		printf(" buffers, ");

		if ((size % 1024) == 0)
		{
			size	/= 1024;
			factor	= "K";
		}

		if ((size % 1024) == 0)
		{
			size	/= 1024;
			factor	= "M";
		}

		gsc_label_long_comma(size);
		printf("%sB each)\n", factor);
		break;
	}

	return(errs);
}



//*****************************************************************************
static int _start_operation(int errs)
{
	gsc_label("Start Working");

	if (errs)
	{
		printf("ABORTED DUE TO SETUP ERRORS <---\n");
		errs	= 0;
	}
	else
	{
		printf("Done\n");
		_tx_data.start	= 1;
		_rx_data.start	= 1;
	}

	return(0);
}



//*****************************************************************************
static int _wait_for_completion(int seconds)
{
	struct timeval	t1;
	struct timeval	t2;

	gsc_label("Waiting for completion");

	// Wait for the elapsed time period to pass, if both threads started.

	if ((_rx_data.started) && (_tx_data.started))
	{
		printf("%d second%s ... ", seconds, (seconds == 1) ? "" : "s");
		fflush(stdout);
		gettimeofday(&t1, NULL);
		t1.tv_sec	+= seconds;

		for (;;)
		{
			gsc_time_sleep_ms(100);
			gettimeofday(&t2, NULL);

			if (t2.tv_sec > t1.tv_sec)
				break;

			if ((t2.tv_sec == t1.tv_sec) &&  (t2.tv_usec >= t1.tv_usec))
				break;
		}

		printf("Done\n");
	}
	else
	{
		printf("SKIPPED   (There were thread start errors.)\n");
	}

	return(0);
}



//*****************************************************************************
static int _buffers_reset(void)
{
	int	errs	= 0;

	gsc_label("Resetting Buffers");

	errs	= gsc_buf_man_free_all();

	if (errs)
		printf("FAIL <---  (buf man free all failed)\n");
	else
		printf("PASS\n");

	return(errs);
}



//*****************************************************************************
int analog_out(int fd, settings_t* settings)
{
	int	errs		= 0;
	int	verbose		= 1;

	gsc_label("Analog Output");
	printf("\n");
	gsc_label_level_inc();

	memset(_chan_data, 0, sizeof(_chan_data));
	errs	+= ao16_config_ao(fd, -1, verbose, settings->fsamp);
	errs	+= _channel_hardware_check(fd, settings);
	errs	+= _channel_waveform_check(settings, _chan_data);
	errs	+= ao16_channel_sel(fd, -1, verbose, settings->ao_channels, NULL);
	errs	+= _io_mode_check(fd, settings);
	errs	+= ao16_tx_io_mode(fd, -1, verbose, settings->io_mode, NULL);
	errs	+= ao16_fsamp_report_all(fd, -1, verbose, NULL);

	errs	+= _pre_test_check(settings, errs);

	if (errs == 0)
	{
		errs	+= _channel_waveform_report(settings, _chan_data);
		errs	+= _waveform_calculations(settings);
		errs	+= _buffers_init(settings);

	}

	if (errs)
	{
		gsc_label("ABORTING");
		printf("Discontinueing due to errors.\n");
	}
	else
	{
		memset(&_rx_data, 0, sizeof(_rx_data));
		_rx_data.chan_data	= _chan_data;
		_rx_data.settings	= settings;
		errs				+= rx_startup(&_rx_data);

		memset(&_tx_data, 0, sizeof(_tx_data));
		_tx_data.fd		= fd;
		errs			+= tx_startup(&_tx_data);

		errs	+= _start_operation(errs);
		errs	+= _wait_for_completion(settings->output_secs);
		errs	+= rx_stop(&_rx_data);
		errs	+= tx_stop(&_tx_data);
		errs	+= _buffers_reset();

		if (settings->show_stats)
			gsc_buf_man_stats();
	}

	gsc_label_level_dec();
	return(errs);
}


