// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/fsamp/fsamp.c $
// $Rev: 45048 $
// $Date: 2019-06-04 16:36:47 -0500 (Tue, 04 Jun 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



// #defines *******************************************************************

#define	FILE_NAME				"fsamp.txt"



/******************************************************************************
*
*	Function:	fsamp_compute
*
*	Purpose:
*
*		Configure the board, then capture data.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*		fsamp	The desired rate, if "range" is zero.
*
*		range	Compute the rate for the entire range of supported rates.
*
*		save	Save the range data to fsamp.txt?
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int fsamp_compute(int fd, s32 fsamp, int range, int save)
{
	int		errs	= 0;
	FILE*	file	= NULL;
	s32		max;
	s32		min;
	s32		nclk;
	s32		nrate;
	double	rate;
	s32		ref_src;

	gsc_label("Fsamp Computation");
	printf("\n");
	gsc_label_level_inc();

	if (range == 0)
	{
		min		= fsamp;
		max		= fsamp;
	}
	else
	{
		errs	+= ao16_query(fd, -1, 0, AO16_QUERY_FSAMP_MAX, &max);
		errs	+= ao16_query(fd, -1, 0, AO16_QUERY_FSAMP_MIN, &min);
	}

	if (save)
	{
		file	= fopen(FILE_NAME, "w+b");

		if (file == NULL)
		{
			printf("FAIL <---  (unable to create %s)\n", FILE_NAME);
			errs	= 1;
		}
		else
		{
			fprintf(file, "# 16AO16 Scan Rates\n");
			fprintf(file, "# Request   Produced\n");
			fprintf(file, "# ========  ==========\n");
		}
	}

	for (fsamp = min; fsamp <= max; fsamp++)
	{
		errs	+= ao16_fsamp_compute(fd, -1, 1, fsamp, &ref_src, &nrate, &nclk, &rate);

		if (file)
			fprintf(file, "%10ld  %10.3f\n", (long) fsamp, (double) rate);
	}

	if (file)
		fclose(file);

	gsc_label_level_dec();
	return(errs);
}


