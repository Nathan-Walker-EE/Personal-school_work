// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/txrate/tx.c $
// $Rev: 44921 $
// $Date: 2019-05-16 20:56:31 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// #defines *******************************************************************

#define	_1M	(1024L * 1024L)



// variables *******************************************************************

static u32	_tx[_1M];



/******************************************************************************
*
*	Function:	_rate_test
*
*	Purpose:
*
*		Perform for the write rate test.
*
*	Arguments:
*
*		fd		The handle to the board.
*
*		seconds	The minimum number of seconds to operate.
*
*		mb		The niminum number of mega-bytes to write.
*
*		send	The number of bytes to request with each write call.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

static int _rate_test(int fd, int seconds, long mb, long send)
{
	struct timeb	begin;
	struct timeb	end;
	int				errs		= 0;
	long long		data_limit	= mb * 1000000;
	long long		data_total	= 0;
	long long		ms;
	long long		ms_limit	= 1000 * seconds;
	long long		ms_total;
	double			rate;
	long			sent;
	long long		total;

	gsc_label("Writing");
	memset(_tx, 0x01, sizeof(_tx));
	ftime(&begin);

	for (;;)
	{
		sent	= ao16_write(fd, _tx, send);

		if (sent < 0)
		{
			errs++;
			printf("FAIL <--- (ao16_write, errno %d)\n", errno);
			break;
		}

		ftime(&end);
		data_total	+= sent;
		ms_total	= ((long long) end.time * 1000 + end.millitm)
					- ((long long) begin.time * 1000 + begin.millitm);

		if ((ms_total >= ms_limit) && (data_total >= data_limit))
			break;
	}

	printf(	"%s  (", errs ? "FAIL <---" : "PASS");

	// Samples
	total	= data_total;
	gsc_label_long_comma(total / 4);
	printf(" Samples, ");

	// Period
	ms	= ms_total;
	gsc_label_float_comma((double) ms / 1000, -1, 3);
	printf(" Seconds, ");

	// Rate
	ms		= (ms <= 0) ? 1 : ms;
	rate	= ((double) total * 1000000 / 4) / ms / 1000;
	gsc_label_float_comma(rate, -1, 3);
	printf(" S/S)\n");

	return(errs);
}



/******************************************************************************
*
*	Function:	tx_rate_test
*
*	Purpose:
*
*		Perform an Tx Rate test - measure the write throughput.
*
*	Arguments:
*
*		fd		The handle to the board.
*
*		mb		The niminum number of mega-bytes to write.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int tx_rate_test(int fd, long mb)
{
	int		errs	= 0;
	int		seconds	= 5;
	long	send	= sizeof(_tx);

	gsc_label ("Write Rate");
	printf("(%d second minimum, %ld MB minimum)\n", seconds, mb);
	gsc_label_level_inc();

	errs	+= _rate_test(fd, seconds, mb, send);

	gsc_label_level_dec();
	return(errs);
}



