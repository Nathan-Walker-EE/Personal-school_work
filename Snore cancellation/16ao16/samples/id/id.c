// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/id/id.c $
// $Rev: 42928 $
// $Date: 2018-05-31 13:02:59 -0500 (Thu, 31 May 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



//*****************************************************************************
static void _local_label_string(const char* str)
{
	int	len;

	printf("%s", str);
	len	= 7 - (int) strlen(str);

	for (; len > 0; len--)
		printf(" ");
}



//*****************************************************************************
static void _local_label_long(long l)
{
	char	buf[32];

	sprintf(buf, "%ld", l);
	_local_label_string(buf);
}



//*****************************************************************************
static int _auto_calibrate(int fd)
{
	int	errs	= 0;
	s32	ms;

	gsc_label("Auto-Calibrate Period");

	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_AUTO_CAL_MS, &ms);

	if (errs)
	{
	}
	else if (ms <= 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. ms response error: %ld)\n",
				__LINE__,
				(long) ms);
	}
	else
	{
		gsc_label_long_comma(ms);
		printf(" ms maximum\n");
	}

	return(errs);
}



//*****************************************************************************
static int _buffer_modes(int fd)
{
	gsc_label("Buffer Modes");
	printf("Circular, Open\n");
	return(0);
}



//*****************************************************************************
static int _channel_qty(int fd)
{
	s32	channel_qty;
	int	errs	= 0;

	gsc_label("D/A Channels");
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_CHANNEL_QTY, &channel_qty);

	if (errs)
	{
	}
	else if (channel_qty <= 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. qty response error: %ld)\n",
				__LINE__,
				(long) channel_qty);
	}
	else
	{
		_local_label_long(channel_qty);
		printf("(16-bit resolution)\n");
	}

	return(errs);
}



//*****************************************************************************
static int _cable_options(int fd)
{
	s32	invert	= 0;
	int	errs	= 0;
	s32	passive	= 0;

	gsc_label("Invert Cable Signals");
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_CABLE_INVERT_4, &invert);
	printf("%s", invert ? "Yes" : "No");
	printf("  (Clock I/O, DAC Clock Out, Trigger In, Trigger Out)\n");

	gsc_label("Passive Cable Signals");
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_CABLE_PASSIVE_3, &passive);
	printf("%s", passive ? "Yes" : "No");
	printf("  (Clock I/O, DAC Clock Out, Trigger Out)\n");

	return(errs);
}



//*****************************************************************************
static int _clocking_modes(int fd)
{
	gsc_label("Clocking Modes");
	printf("Sequential, Simultaneous\n");
	return(0);
}



//*****************************************************************************
static int _clocking_sources(int fd)
{
	gsc_label("Clocking Sources");
	printf("Internal, External, Software\n");
	return(0);
}



//*****************************************************************************
static int _fifo_size(int fd)
{
	int	errs	= 0;
	s32	size;			// FIFO Size in samples

	gsc_label("FIFO Size");

	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_FIFO_SIZE, &size);

	if (errs)
	{
	}
	else if (size <= 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. size response error: %ld)\n",
				__LINE__,
				(long) size);
	}
	else
	{
		gsc_label_long_comma(size);
		printf(" Samples Deep");
		printf("  (%ldK)\n", (long) size / 1024);
	}

	return(errs);
}



//*****************************************************************************
static int _fsamp_range(int fd)
{
	int	errs	= 0;
	s32	max;			// Sample Rate Maximum
	s32	min;			// Sample Rate Minimum

	gsc_label("Sample Rates");

	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_FSAMP_MAX, &max);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_FSAMP_MIN, &min);

	if (errs)
	{
	}
	else if (max < 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. max response error: %ld)\n",
				__LINE__,
				(long) max);
	}
	else if (min < 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. min response error: %ld)\n",
				__LINE__,
				(long) min);
	}
	else if (min >= max)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. range error: %ld-%ld)\n",
				__LINE__,
				(long) min,
				(long) max);
	}
	else
	{
		gsc_label_long_comma(min);
		printf(" to ");
		gsc_label_long_comma(max);
		printf(" S/S/C\n");
	}

	return(errs);
}



//*****************************************************************************
static int _initialize(int fd)
{
	int	errs	= 0;
	s32	ms;

	gsc_label("Initialize Period");

	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_INIT_MS, &ms);

	if (errs)
	{
	}
	else if (ms <= 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. ms response error: %ld)\n",
				__LINE__,
				(long) ms);
	}
	else
	{
		gsc_label_long_comma(ms);
		printf(" ms maximum\n");
	}

	return(errs);
}



//*****************************************************************************
static int _io_modes(int fd)
{
	s32	dmdma	= 0;
	int	errs	= 0;

	gsc_label("I/O Modes");
	errs	= ao16_query(fd, -1, 0, AO16_QUERY_DMDMA, &dmdma);

	if (dmdma)
		printf("PIO, DMA, DMDMA\n");
	else
		printf("PIO, DMA (DMDMA not supported)\n");

	return(errs);
}



//*****************************************************************************
static int _master_clock(int fd)
{
	gsc_label("Master Clock");
	gsc_label_long_comma(45000000L);
	printf(" Hz, Alternate ");
	gsc_label_long_comma(16000000L);
	printf(" Hz to ");
	gsc_label_long_comma(32000000L);
	printf(" Hz\n");
	return(0);
}



//*****************************************************************************
static int _nrate_range(int fd)
{
	int	errs	= 0;
	s32	mask;
	s32	max;
	s32	min;

	gsc_label("Nrate Range");

	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_NRATE_MASK, &mask);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_NRATE_MAX, &max);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_NRATE_MIN, &min);

	if (errs)
	{
	}
	else if (max < 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. max response error: %ld)\n",
				__LINE__,
				(long) max);
	}
	else if (min < 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. min response error: %ld)\n",
				__LINE__,
				(long) min);
	}
	else if (mask < 0)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. mask response error: %ld)\n",
				__LINE__,
				(long) mask);
	}
	else if (min >= max)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. range error: %ld-%ld)\n",
				__LINE__,
				(long) min,
				(long) max);
	}
	else if (max > mask)
	{
		errs	= 1;
		printf(	"FAIL <---  (%d. mask range error: mask 0x%lX, max %ld)\n",
				__LINE__,
				(long) mask,
				(long) max);
	}
	else
	{
		gsc_label_long_comma(min);
		printf(" to ");
		gsc_label_long_comma(max);
		printf("  (mask 0x%lX)\n", (long) mask);
	}

	return(errs);
}



//*****************************************************************************
static int _output_modes(int fd)
{
	gsc_label("Output Modes");
	printf("Differential, Single Ended\n");
	return(0);
}



//*****************************************************************************
static int _rate_generators(int fd)
{
	gsc_label("Rate Generators");
	printf("1\n");
	return(0);
}



//*****************************************************************************
static int _sampling_modes(int fd)
{
	gsc_label("Sampling Modes");
	printf("Continuous, Burst, Single Shot\n");
	return(0);
}



//*****************************************************************************
static int _voltage_ranges(int fd)
{
	int	errs	= 0;
	s32	range	= 0;

	gsc_label("Voltage Ranges");
	errs	= ao16_query(fd, -1, 0, AO16_QUERY_VOLT_RANGE, &range);

	if (range == AO16_VOLT_RANGE_LOW)
	{
		printf("+-10V, +-5V, +-2.5V, +- 1.25V\n");
	}
	else if (range == AO16_VOLT_RANGE_HIGH)
	{
		printf("+-20V, +-10V, +-5V\n");
	}
	else
	{
		errs++;
		printf("<---  (ERROR: invalid range: %ld)\n", (long) range);
	}

	return(errs);
}



//*****************************************************************************
static int _xcvr_options(int fd)
{
	gsc_label("Transceiver Options");
	printf("LVDS, TTL\n");
	return(0);
}



//*****************************************************************************
static int _id_board_for(int fd)
{
	gsc_reg_def_t	list[2]	= { { NULL }, { NULL } };
	gsc_reg_def_t*	ptr;

	int	errs	= 0;

	printf("\n");
	ptr	= (gsc_reg_def_t*) ao16_reg_get_def_id(AO16_GSC_FOR);

	if (ptr)
	{
		list[0]	= ptr[0];
		errs	= gsc_reg_list(fd, list, 1, ao16_reg_read);
	}
	else
	{
		errs++;
		printf(	"FAIL <---  (%d. ao16_reg_get_def_id, AO16_GSC_FOR)\n",
				__LINE__);
	}

	return(errs);
}



//*****************************************************************************
static int _register_map(int fd)
{
	int	errs	= 0;

	printf("\n");
	errs	= ao16_reg_list(fd, 0);
	return(errs);
}



/******************************************************************************
*
*	Function:	id_device
*
*		Identify the device and its features.
*
*	Arguments:
*
*		fd		The handle to the board to access.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int id_device(int fd)
{
	int	errs	= 0;

	gsc_label("Board Features");
	printf("\n");

	gsc_label_level_inc();

	errs	+= _output_modes(fd);
	errs	+= _voltage_ranges(fd);
	errs	+= _channel_qty(fd);
	errs	+= _rate_generators(fd);
	errs	+= _master_clock(fd);
	errs	+= _clocking_sources(fd);
	errs	+= _clocking_modes(fd);
	errs	+= _cable_options(fd);
	errs	+= _nrate_range(fd);
	errs	+= _fsamp_range(fd);
	errs	+= _sampling_modes(fd);
	errs	+= _fifo_size(fd);
	errs	+= _buffer_modes(fd);
	errs	+= _io_modes(fd);
	errs	+= _xcvr_options(fd);
	errs	+= _initialize(fd);
	errs	+= _auto_calibrate(fd);

	gsc_label_level_dec();

	errs	+= _id_board_for(fd);
	errs	+= _register_map(fd);

	return(errs);
}


