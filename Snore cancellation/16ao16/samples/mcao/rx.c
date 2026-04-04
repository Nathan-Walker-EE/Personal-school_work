// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/mcao/rx.c $
// $Rev: 44918 $
// $Date: 2019-05-16 20:53:55 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// #defines *******************************************************************

#define	PI		(3.14159265358979323846264338327950288419716939937510)



//*****************************************************************************
static void _wait_til_done(rx_data_t* rx)
{
	// Wait until the processing thread indicates that it is done.

	for (;;)
	{
		if (rx->done)
			break;

		gsc_time_sleep_ms(1);
	}
}



//*****************************************************************************
static void _wait_til_started(rx_data_t* rx)
{
	// Wait until the processing thread indicates that it has started.

	for (;;)
	{
		if ((rx->started) || (rx->stop))
			break;

		gsc_time_sleep_ms(1);
	}
}



//*****************************************************************************
static void _wait_for_start(rx_data_t* rx)
{
	// Wait until the controlling thread tells us to start working.

	for (;;)
	{
		if ((rx->start) || (rx->stop))
			break;

		gsc_time_sleep_ms(1);
	}
}



//*****************************************************************************
static void _square_init(int chan, settings_t* settings, u32* dst)
{
	int	i;
	int	j;
	int	thresh;
	int	samps_per_cycle;

	samps_per_cycle	= settings->chan_samps / settings->chan_cycles;
	thresh			= samps_per_cycle / 2;

	for (i = 0; i < settings->chan_samps; i++)
	{
		j = i % samps_per_cycle;

		if (j < thresh)
			dst[0]	= 0xFFFF;
		else
			dst[0]	= 0x0000;

		dst	+= settings->ao_chan_qty;
	}
}



//*****************************************************************************
static void _sine_init(int chan, settings_t* settings, u32* dst)
{
	int	denom;
	int	i;
	int	samps_per_cycle;

	samps_per_cycle	= settings->chan_samps / settings->chan_cycles;
	denom			= samps_per_cycle - 1;

	for (i = 0; i < settings->chan_samps; i++)
	{
		dst[0]	= (u32) (sin(PI * 2 * i / denom) * 32767 + 32768);
		dst		+= settings->ao_chan_qty;
	}
}



//*****************************************************************************
static void _saw_init(int chan, settings_t* settings, u32* dst)
{
	double	b;
	int		i;
	int		j;
	double	m;
	int		samps_per_cycle;

	samps_per_cycle	= settings->chan_samps / settings->chan_cycles;
	m				= (double) 0xFFFF / (1 - samps_per_cycle);
	b				= (double) 0xFFFF;


	for (i = 0; i < settings->chan_samps; i++)
	{
		j		= i % samps_per_cycle;
		dst[0]	= (u32) (m * j + b);
		dst		+= settings->ao_chan_qty;
	}
}



//*****************************************************************************
static void _ramp_init(int chan, settings_t* settings, u32* dst)
{
	int		i;
	int		j;
	double	m;
	int		samps_per_cycle;

	samps_per_cycle	= settings->chan_samps / settings->chan_cycles;
	m				= (double) 0xFFFF / (samps_per_cycle - 1);


	for (i = 0; i < settings->chan_samps; i++)
	{
		j		= i % samps_per_cycle;
		dst[0]	= (u32) (m * j);
		dst		+= settings->ao_chan_qty;
	}
}



//*****************************************************************************
static void _rx_process(rx_data_t* rx)
{
	gsc_buf_man_t	bm;
	int				chan;
	u32*			dst;
	int				errs	= 0;

	for (;;)
	{
		// Request an empty buffer so we can fill it.
		memset(&bm, 0, sizeof(gsc_buf_man_t));
		errs	= gsc_buf_man_request_empty(&bm);

		if (rx->stop)
		{
			bm.count	= 0;
			bm.eof		= 0;
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		// Perform validation.

		if (errs)
		{
			bm.count	= 0;
			bm.eof		= 0;
			rx->errs	= 1;
			sprintf(rx->err_buf,
				"%d. Rx: gsc_buf_man_request_empty(): failed",
					__LINE__);
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		if (bm.buffer == NULL)
		{
			bm.count	= 0;
			bm.eof		= 0;
			rx->errs	= 1;
			sprintf(rx->err_buf,
					"%d. Rx: gsc_buf_man_request_empty(): NULL buffer",
					__LINE__);
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		if (bm.size == 0)
		{
			bm.count	= 0;
			bm.eof		= 0;
			rx->errs	= 1;
			sprintf(rx->err_buf,
					"%d. Rx: gsc_buf_man_request_empty(): zero sized buffer",
					__LINE__);
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		if (bm.size != rx->settings->buf_size)
		{
			bm.count	= 0;
			bm.eof		= 0;
			rx->errs	= 1;
			sprintf(rx->err_buf,
					"%d. Rx: gsc_buf_man_request_empty(): buffer size: got %ld, expected %ld",
					__LINE__,
					(long) bm.size,
					(long) rx->settings->buf_size);
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		// Perform the work to fill the buffer.
		bm.offset		= 0;
		bm.count		= bm.size;
		bm.eof			= 0;
		dst				= bm.buffer;
		rx->total_bytes	+= bm.count;

		for (chan = 0; chan < 16; chan++)
		{
			if (rx->chan_data[chan].enable == 0)
				continue;

			switch (rx->chan_data[chan].waveform)
			{
				default:
				case WF_RAMP:	_ramp_init(chan, rx->settings, dst);	break;
				case WF_SAW:	_saw_init(chan, rx->settings, dst);	break;
				case WF_SINE:	_sine_init(chan, rx->settings, dst);	break;
				case WF_SQUARE:	_square_init(chan, rx->settings, dst);	break;
			}

			dst++;
		}

		// Release the buffer back to the Buffer Manager.
		errs	= gsc_buf_man_release_buffer(&bm);

		if (errs)
		{
			rx->errs	= 1;
			sprintf(rx->err_buf,
					"%d. Rx gsc_buf_man_release_buffer() failed",
					__LINE__);
			break;
		}
	}
}



//*****************************************************************************
static int _rx_thread(void* arg)
{
	rx_data_t*	rx	= arg;

	// Tell the controlling code that we're started.
	rx->started	= 1;

	// Wait till we're told to start.
	_wait_for_start(rx);

	// Perform the expected activity.
	rx->total_ms	= gsc_time_delta_ms();
	_rx_process(rx);
	rx->total_ms	= gsc_time_delta_ms() - rx->total_ms;

	// Tell the controlling code that we're done.
	rx->done	= 1;
	return(0);
}


//*****************************************************************************
int rx_startup(rx_data_t* rx)
{
	int			errs	= 0;
	const char*	ptr		= NULL;

	gsc_label("Rx Startup");
	ptr		= "generating pattern";

	if (errs == 0)
		errs	= os_thread_create(&rx->thread, "Rx Thread", _rx_thread, rx);

	if (errs)
	{
		printf("FAIL <---  (%s)\n", ptr);
	}
	else
	{
		_wait_til_started(rx);
		printf("PASS  (%s)\n", ptr);
	}

	return(errs);
}



//*****************************************************************************
int rx_stop(rx_data_t* rx)
{
	int			errs	= 0;
	long long	ms;
	double		rate;
	long long	total;

	// STOP ===============================================
	gsc_label("Rx Stop");
	rx->stop	= 1;

	if (rx->started)
		_wait_til_done(rx);

	os_thread_destroy(&rx->thread);
	printf("Done\n");

	gsc_label_level_inc();

	// STATUS =============================================
	gsc_label("Status");

	if (rx->errs == 0)
	{
		errs	= 0;
		printf("PASS\n");
	}
	else
	{
		errs	= 1;
		printf("FAIL <---");

		if (rx->err_buf[0])
			printf(" (%s)", rx->err_buf);

		printf("\n");
	}

	// THROUGHPUT =========================================
	gsc_label("Throughput");

	// Samples
	total	= rx->total_bytes;
	gsc_label_long_comma(total / 4);
	printf(" Samples, ");

	// Period
	ms	= rx->total_ms;
	gsc_label_float_comma((double) ms / 1000, -1, 3);
	printf(" Seconds, ");

	// Rate
	ms		= (ms <= 0) ? 1 : ms;
	rate	= ((double) total * 1000000 / 4) / ms / 1000;
	gsc_label_float_comma(rate, -1, 3);
	printf(" S/S)\n");

	gsc_label_level_dec();
	return(errs);
}


