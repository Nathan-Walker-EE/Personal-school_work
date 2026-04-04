// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/fsamp/main.c $
// $Rev: 44915 $
// $Date: 2019-05-16 20:51:54 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// variables ******************************************************************

static	s32	_continuous		= 0;
static	s32	_def_index		= 0;
static	s32	_fsamp			= 10000;
static	s32	_minute_limit	= 0;
static	s32	_range			= 0;
static	s32	_save			= 0;
static	s32	_test_limit		= 0;



//*****************************************************************************
static int _parse_args(int argc, char** argv)
{
	static const gsc_arg_item_t	list[]	=
	{
		//	type				var				arg			values	desc
		{	GSC_ARG_S32_FLAG,	&_continuous,	"-c",		{ -1 },	"Continue testing until an error occurs."				},
		{	GSC_ARG_S32_FLAG,	&_continuous,	"-C",		{ +1 },	"Continue testing even if errors occur."				},
		{	GSC_ARG_S32_MIN,	&_minute_limit,	"-m",		{ 1 },	"Continue testing for up to # minutes."					},
		{	GSC_ARG_S32_MIN,	&_test_limit,	"-n",		{ 1 },	"Continue testing for up to # iterations."				},
		{	GSC_ARG_S32_FLAG,	&_range,		"-r",		{ 1 },	"Compute the rate for all supported sample rates."		},
		{	GSC_ARG_S32_MIN,	&_fsamp,		"-s",		{ 1 },	"The desired sample rate."								},
		{	GSC_ARG_S32_FLAG,	&_save,			"-S",		{ 1 },	"Save the rate scan data to fsamp.txt (requires -r)."	},
		{	GSC_ARG_DEV_INDEX,	&_def_index,	"index",	{ 0 },	"The zero based index of the device to access."			}
	};

	static const gsc_arg_set_t	set	=
	{
		/* name	*/	"fsamp",
		/* desc	*/	"Compute the configuration for a desired sample rate.",
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
	int	errs	= 0;

	errs	+= fsamp_compute(fd, _fsamp, _range, _save);

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
		gsc_label_init(21);
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



