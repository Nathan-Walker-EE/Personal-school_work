// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/aout/main.c $
// $Rev: 44913 $
// $Date: 2019-05-16 20:50:37 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// #defines *******************************************************************

#define	DEFAULT_PERIOD	2
#define	DEFAULT_RATE	100000L



// variables ******************************************************************

static	s32	_continuous		= 0;
static	s32	_def_index		= 0;
static	s32	_io_mode		= GSC_IO_MODE_PIO;
static	s32	_minute_limit	= 0;
static	s32	_period			= DEFAULT_PERIOD;
static	s32	_rate			= DEFAULT_RATE;
static	s32	_test_limit		= 0;



//*****************************************************************************
static int _parse_args(int argc, char** argv)
{
	static const gsc_arg_item_t	list[]	=
	{
		//	type				var				arg			values					desc
		{	GSC_ARG_S32_FLAG,	&_io_mode,		"-bmdma",	{GSC_IO_MODE_BMDMA },	"Use Block Mode DMA mode for write requests."	},
		{	GSC_ARG_S32_FLAG,	&_continuous,	"-c",		{ -1 },					"Continue testing until an error occurs."		},
		{	GSC_ARG_S32_FLAG,	&_continuous,	"-C",		{ +1 },					"Continue testing even if errors occur."		},
		{	GSC_ARG_S32_FLAG,	&_io_mode,		"-dmdma",	{ GSC_IO_MODE_DMDMA },	"Use Demand Mode DMA mode for write requests."	},
		{	GSC_ARG_S32_MIN,	&_minute_limit,	"-m",		{ 1 },					"Continue testing for up to # minutes."			},
		{	GSC_ARG_S32_MIN,	&_test_limit,	"-n",		{ 1 },					"Continue testing for up to # iterations."		},
		{	GSC_ARG_S32_FLAG,	&_io_mode,		"-pio",		{ GSC_IO_MODE_PIO },	"Use PIO mode for write requests."				},
		{	GSC_ARG_S32_MIN,	&_period,		"-p",		{ 1 },					"Select the data output period in seconds."		},
		{	GSC_ARG_S32_RANGE,	&_rate,			"-r",		{ 10, 1000000L },		"Select the output data rate."					},
		{	GSC_ARG_DEV_INDEX,	&_def_index,	"index",	{ 0 },					"The zero based index of the device to access."	}
	};

	static const gsc_arg_set_t	set	=
	{
		/* name	*/	"aout",
		/* desc	*/	"Analog Output Sample Application",
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
	s32	dmdma	= 0;
	int	errs	= 0;
	int	verbose	= 1;

	errs	+= ao16_config_ao(fd, -1, 1, _rate);
	errs	+= ao16_query(fd, -1, 1, AO16_QUERY_DMDMA, &dmdma);

	if ((dmdma == 0) && (_io_mode == GSC_IO_MODE_DMDMA))
	{
		errs	+= ao16_tx_io_mode	(fd, -1, verbose, GSC_IO_MODE_BMDMA, NULL);
		gsc_label(NULL);
		printf("ALTERATION  (using BMDMA as DMDMA is not supported.)\n");
	}
	else
	{
		errs	+= ao16_tx_io_mode	(fd, -1, verbose, _io_mode, NULL);
	}

	errs	+= analog_out(fd, _period);

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
*		EXIT_SUCCESS	No errors were encounterred.
*		EXIT_FAILURE	One or more problems were encounterred.
*
******************************************************************************/

int main(int argc, char** argv)
{
	int		errs	= 0;
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
		gsc_label_init(26);
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



