// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/regs/main.c $
// $Rev: 43849 $
// $Date: 2018-10-23 14:17:34 -0500 (Tue, 23 Oct 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



// variables ******************************************************************

static	s32	_def_index		= 0;



//*****************************************************************************
static int _parse_args(int argc, char** argv)
{
	static const gsc_arg_item_t	list[]	=
	{
		//	type				var				arg			values	desc
		{	GSC_ARG_DEV_INDEX,	&_def_index,	"index",	{ 0 },	"The zero based index of the device to access."	}
	};

	static const gsc_arg_set_t	set	=
	{
		/* name	*/	"regs",
		/* desc	*/	"Register Access Application",
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
	main_menu(fd);

	return(0);
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



/*****************************************************************************
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
*		EXIT_SUCCESS	We tested a device.
*		EXIT_FAILURE	One or more problems were encounterred.
*
*****************************************************************************/

int main(int argc, char** argv)
{
	int		errs	= 0;
	int		fd		= 0;
	int		qty		= 0;
	int		ret;

	for (;;)
	{
		gsc_label_init(21);
		errs	= _parse_args(argc, argv);

		if (errs)
			break;

		// Introduction

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
			// Perform testing

			errs	= _perform_tests(fd);

			// Close device access

			errs	+= ao16_close_util(fd, -1, 1);
		}

		// End of processing.

		break;
	}

	ret	= ((qty <= 0) || (errs)) ? EXIT_FAILURE : EXIT_SUCCESS;
	return(ret);
}



