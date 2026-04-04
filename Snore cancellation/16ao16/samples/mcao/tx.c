// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/mcao/tx.c $
// $Rev: 44918 $
// $Date: 2019-05-16 20:53:55 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static void _wait_til_done(tx_data_t* tx)
{
	// Wait until the processing thread indicates that it is done.

	for (;;)
	{
		if (tx->done)
			break;

		gsc_time_sleep_ms(1);
	}
}



//*****************************************************************************
static void _wait_til_started(tx_data_t* tx)
{
	// Wait until the processing thread indicates that it has started.

	for (;;)
	{
		if ((tx->started) || (tx->stop))
			break;

		gsc_time_sleep_ms(1);
	}
}



//*****************************************************************************
static void _wait_for_start(tx_data_t* tx)
{
	// Wait until the controlling thread tells us to start working.

	for (;;)
	{
		if ((tx->start) || (tx->stop))
			break;

		gsc_time_sleep_ms(1);
	}
}



//*****************************************************************************
static void _tx_process(tx_data_t* tx)
{
	gsc_buf_man_t	bm;
	int				bytes;
	int				eof;
	int				errs	= 0;
	int				sent;

	memset(&bm, 0, sizeof(gsc_buf_man_t));

	for (;;)
	{
		// Request an data buffer so we can process it.
		errs	= gsc_buf_man_request_data(&bm);

		if (tx->stop)
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
			tx->errs	= 1;
			strcpy(tx->err_buf, "Tx data request: failed");
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		if (bm.buffer == NULL)
		{
			bm.count	= 0;
			bm.eof		= 0;
			tx->errs	= 1;
			strcpy(tx->err_buf, "Tx data request: NULL buffer");
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		if (bm.size == 0)
		{
			bm.count	= 0;
			bm.eof		= 0;
			tx->errs	= 1;
			strcpy(tx->err_buf, "Tx data request: zero sized buffer");
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		if (bm.count == 0)
		{
			bm.eof		= 0;
			tx->errs	= 1;
			strcpy(tx->err_buf, "Tx data request: empty buffer");
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		if (bm.count % 4)
		{
			bm.count	= 0;
			bm.eof		= 0;
			tx->errs	= 1;
			sprintf(tx->err_buf,
					"Tx data request: ill filled buffer: %ld bytes",
					(long) bm.count);
			gsc_buf_man_release_buffer(&bm);
			break;
		}

		// Do something with the data.
		bytes	= bm.count;

		// Write until all data is sent.

		for (;;)
		{
			sent	= ao16_write(tx->fd, bm.buffer, bytes);

			if (sent < 0)
			{
				tx->errs	= 1;
				sprintf(tx->err_buf, "write failure: errno %d", errno);
				break;
			}

			tx->total_bytes	+= sent;

			if (sent == bytes)
			{
				bm.offset	= 0;
				bm.count	= 0;
				break;
			}

			bm.offset		+= sent;
			bm.count		-= sent;
			bytes			-= sent;
		}

		eof			= bm.eof;
		bm.eof		= 0;
		bm.count	= 0;
		errs		= gsc_buf_man_release_buffer(&bm);

		if ((tx->errs == 0) && (errs))
		{
			tx->errs	= 1;
			strcpy(tx->err_buf, "Tx buffer release failure");
		}

		if ((eof) || (tx->errs) || (tx->stop))
			break;
	}
}



//*****************************************************************************
static void _prefill_buffer(tx_data_t* tx)
{
	#define	_FILL	(256 * 1024 + 100)

	int			errs	= 0;
	int			i;
	gsc_reg_t	reg;
	s32			set;
	int			sts;

	for (;;)
	{
		// Temporarily disable output.
		set		= AO16_CLOCK_ENABLE_NO;
		sts		= ao16_ioctl(tx->fd, AO16_IOCTL_CLOCK_ENABLE, &set);
		errs	+= sts ? 1 : 0;

		// Fill the buffer until the status is reported as full.
		reg.reg		= AO16_GSC_ODBR;
		reg.value	= 0x00008000;

		for (i = 0; i < _FILL; i++)
		{
			sts		= ao16_ioctl(tx->fd, AO16_IOCTL_REG_WRITE, &reg);
			errs	+= sts ? 1 : 0;
		}

		// Clear any overflow errors.
		set		= AO16_BUFFER_OVER_DATA_CLR;
		sts		= ao16_ioctl(tx->fd, AO16_IOCTL_BUFFER_OVER_DATA, &set);
		errs	+= sts ? 1 : 0;

		set		= AO16_BUFFER_OVER_FRAME_CLR;
		sts		= ao16_ioctl(tx->fd, AO16_IOCTL_BUFFER_OVER_FRAME, &set);
		errs	+= sts ? 1 : 0;

		// Reenable output.
		set		= AO16_CLOCK_ENABLE_YES;
		sts		= ao16_ioctl(tx->fd, AO16_IOCTL_CLOCK_ENABLE, &set);
		errs	+= sts ? 1 : 0;

		break;
	}

	if (errs)
	{
		tx->errs	= 1;
		strcpy(tx->err_buf, "Tx buffer prefill: failed");
	}
}



//*****************************************************************************
static int _tx_thread(void* arg)
{
	tx_data_t*	tx	= arg;

	// Tell the controlling code that we're started.
	tx->started	= 1;

	// Prefill the output buffer so the measured output rate is more accurate.
	_prefill_buffer(tx);

	// Wait till we're told to start.
	_wait_for_start(tx);

	// Perform the expected activity.
	tx->total_ms	= gsc_time_delta_ms();
	_tx_process(tx);
	tx->total_ms	= gsc_time_delta_ms() - tx->total_ms;

	// Tell the controlling code that we're done.
	tx->done	= 1;
	return(0);
}



//*****************************************************************************
int tx_startup(tx_data_t* tx)
{
	int			errs	= 0;
	const char*	ptr		= NULL;

	gsc_label("Tx Startup");
	ptr		= "writing to device";
	errs	= os_thread_create(&tx->thread, "Tx Thread", _tx_thread, tx);

	if (errs)
	{
		printf("FAIL <---  (%s)\n", ptr);
	}
	else
	{
		_wait_til_started(tx);
		printf("PASS  (%s)\n", ptr);
	}

	return(errs);
}



//*****************************************************************************
int tx_stop(tx_data_t* tx)
{
	int			errs	= 0;
	long long	ms;
	double		rate;
	long long	total;

	// STOP ===============================================
	gsc_label("Tx Stop");
	tx->stop	= 1;

	if (tx->started)
		_wait_til_done(tx);

	os_thread_destroy(&tx->thread);
	printf("Done\n");

	gsc_label_level_inc();

	// STATUS =============================================
	gsc_label("Status");

	if (tx->errs == 0)
	{
		errs	= 0;
		printf("PASS\n");
	}
	else
	{
		errs	= 1;
		printf("FAIL <---");

		if (tx->err_buf[0])
			printf(" (%s)", tx->err_buf);

		printf("\n");
	}

	// THROUGHPUT =========================================
	gsc_label("Throughput");

	// Samples
	total	= tx->total_bytes;
	gsc_label_long_comma(total / 4);
	printf(" Samples, ");

	// Period
	ms	= tx->total_ms;
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


