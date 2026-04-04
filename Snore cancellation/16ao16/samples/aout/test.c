// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/aout/test.c $
// $Rev: 44923 $
// $Date: 2019-05-16 21:10:15 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// variables ******************************************************************

static	u32		_data[128 * 1024];
static	long	_loops;	// Number of times to send a block of data.
static	long	_qty;	// Number of samples to send per block.



/******************************************************************************
*
*	Function:	_init_data
*
*	Purpose:
*
*		initialize our transmit data buffer.
*
*	Arguments:
*
*		chans	The number of channels we're using.
*
*		rate	The data output clock rate.
*
*		seconds	Generate data for this period of time.
*
*	Returned:
*
*		>= 0	The number of errors seen.
*
******************************************************************************/

static void _init_data(int chans, long rate, int seconds)
{
	#define	PIE		(3.1415962)

	u32*		dest;
	int			i;
	long		period_len;	// The length of the pattern in samples.
	long		period_tot;	// Pattern samples total for all channels.
	long		period;		// Pattern length in x units.
	int			samp;
	float		samp_dat;	// Total sample size of the data buffer.
	float		samp_sec;	// Samples Per Second for all active channels
	float		samp_tot;	// Samples Total for all active channels
	const char*	units;

	gsc_label("Initializing Data");
	memset(_data, 0, sizeof(_data));
	samp_sec	= ((float) rate) * chans;
	samp_tot	= ((float) rate) * chans * seconds;
	samp_dat	= sizeof(_data) / 4;

	period_len	= (rate <= 100) ? rate				//      10-      100 ->    10-   100
				: (rate <= 1000) ? (rate / 2)		//     101-    1,000 ->    50-   500
				: (rate <= 10000) ? (rate / 5)		//   1,001-   10,000 ->   200- 2,000
				: (rate <= 100000) ? (rate / 10)	//  10,001-  100,000 -> 1,000-10,000
				: (rate / 100);						// 100,001-1,000,000 -> 1,000-10,000
	period_tot	= period_len * chans;
	period		= (long) (1000000000. / rate * period_len);
	units		= "ns";

	if ((period % 1000) == 0)
	{
		period	/= 1000;
		units	= "us";
	}

	if ((period % 1000) == 0)
	{
		period	/= 1000;
		units	= "ms";
	}

	if (samp_tot <= SIZEOF_ARRAY(_data))
	{
		// We can get the entire image in the buffer.
		_loops	= 1;
		_qty	= (long) samp_tot;
	}
	else if (samp_sec <= SIZEOF_ARRAY(_data))
	{
		// We can get one second of the image in the buffer.
		_loops	= seconds;
		_qty	= (long) samp_sec;
	}
	else if (period_tot <= SIZEOF_ARRAY(_data))
	{
		// We can get one period of the image in the buffer.
		_loops	= seconds * (samp_sec / period_tot);
		_qty	= period_tot;
	}
	else
	{
		// We can get only a fraction of a period of the image in the buffer.
		_qty	= (samp_dat / chans) * chans;
		_loops	= (samp_tot + _qty - 1) / _qty;
	}

	// Initialize the data buffer.
	// The data is encoded in OFFSET BINARY format.
	dest	= _data;;

	for (i = 0, samp = 0; i < _qty; i += chans, samp++)
	{
		samp	= (samp < period_len) ? samp : 0;

		// 1st channel: sawtooth wave
		*dest++	= (u16) (0xFFFF - (0xFFFF * samp / (period_len - 1)));

		// 2nd channel: ramp wave
		if (chans >= 2)
			*dest++	= (u16) (0xFFFF * samp / (period_len - 1));

		// 3rd channel: square wave
		if (chans >= 3)
			*dest++	= (samp < (period_len / 2)) ? 0xFFFF : 0;

		// 4th channel: sine wave
		if (chans >= 4)
			*dest++	= ((long) 0x7FFF) * (1 + sin((2 * PIE / period_len) * samp));
	}

	printf("DONE  (pattern length: %ld samples, %ld %s)\n", period_len, period, units);
}



/******************************************************************************
*
*	Function:	_io_write
*
*	Purpose:
*
*		Write the data to the device.
*
*	Arguments:
*
*		fd		This is the handle used to access the device.
*
*		chans	The number of channels being used.
*
*		errs	This is the number of errors seen so far. We don't do the
*				work if errors have been see.
*
*	Returned:
*
*		>= 0	The number of errors seen.
*
******************************************************************************/

static int _io_write(int fd, int chans, int errs)
{
	struct timeb	begin;
	struct timeb	end;
	s32				fifo;
	long			loop;
	long long		ms;
	char*			ptr;
	double			rate;
	int				send;
	int				sent;
	long long		total;

	gsc_label("Writing Data");

	for (;;)	// A convenience loop.
	{
		if (errs)
		{
			errs	= 0;
			printf("SKIPPED   (due to errors)\n");
			break;
		}

		// Prefill the output buffer
		errs	= ao16_query(fd, -1, 0, AO16_QUERY_FIFO_SIZE, &fifo);
		fifo	*= 4;
		total	= 0;

		for (;;)
		{
			sent	= ao16_write(fd, &_data, _qty * 4);

			if (sent < 0)
				errs++;

			if (errs)
				break;

			total	+= sent;

			if (total >= fifo)
				break;
		}

		// Write the data to the device.
		total	= 0;
		ftime(&begin);

		for (loop = 0; loop < _loops; loop++)
		{
			if (errs)
				break;

			send	= _qty * 4;
			ptr		= (char*) &_data;

			for (;;)
			{
				sent	= ao16_write(fd, ptr, send);

				if (sent < 0)
					errs++;

				if (sent <= 0)
					break;

				total	+= sent;
				ptr		+= sent;
				send	-= sent;

				if (send == 0)
					break;
			}

			if (sent <= 0)
				break;
		}

		// Compute the results.
		ftime(&end);

		if (errs)
		{
			errs	= 1;
			printf("FAIL <---  (write error, ");
		}
		else if (sent == 0)
		{
			errs	= 1;
			printf("FAIL <---  (I/O timeout, ");
		}
		else
		{
			printf("PASS  (");
		}

		// Samples
		gsc_label_long_comma(total / 4);
		printf(" Samples, ");

		// Period
		ms	= ((long long) end.time * 1000 + end.millitm)
			- ((long long) begin.time * 1000 + begin.millitm);
		gsc_label_float_comma((double) ms / 1000, -1, 3);
		printf(" Seconds, ");

		// Rate
		ms		= (ms <= 0) ? 1 : ms;
		rate	= ((double) total * 1000000 / 4) / ms / 1000;
		gsc_label_float_comma(rate, -1, 3);
		printf(" S/S)\n");
		break;
	}

	return(errs);
}



/******************************************************************************
*
*	Function:	analog_out
*
*	Purpose:
*
*		Ganerate Analog Output.
*
*	Arguments:
*
*		fd		The file descriptor for the board to access.
*
*		seconds	Generate data for this period of time.
*
*	Returned:
*
*		>= 0	The number of errors seen.
*
******************************************************************************/

int analog_out(int fd, int seconds)
{
	int	errs	= 0;
	s32	rate;

	gsc_label("Analog Output");

	errs	+= ao16_fsamp_report		(fd, -1, 0, 1, &rate);

	if (errs == 0)
	{
		printf("\n");
		gsc_label_level_inc();

		errs	+= ao16_channel_sel			(fd, -1, 1, 0xF, NULL);
		errs	+= ao16_fsamp_report_all	(fd, -1, 1, NULL);
		_init_data(4, rate, seconds);
		errs	+= ao16_buffer_over_frame	(fd, -1, 1, AO16_BUFFER_OVER_FRAME_CLR, NULL);
		errs	+= ao16_buffer_over_data	(fd, -1, 1, AO16_BUFFER_OVER_DATA_CLR, NULL);
		errs	+= _io_write(fd, 4, errs);
		errs	+= ao16_buffer_over_frame	(fd, -1, 1, AO16_BUFFER_OVER_FRAME_CHK, NULL);
		errs	+= ao16_buffer_over_data	(fd, -1, 1, AO16_BUFFER_OVER_DATA_CHK, NULL);

		gsc_label_level_dec();
	}

	return(errs);
}


