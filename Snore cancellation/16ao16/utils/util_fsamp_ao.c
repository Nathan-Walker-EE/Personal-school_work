// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_fsamp_ao.c $
// $Rev: 44911 $
// $Date: 2019-05-16 20:44:07 -0500 (Thu, 16 May 2019) $

// 16AO16: Utilities: source file

#include "main.h"



// #defines *******************************************************************

#define	ABS(v)					(((v) < 0) ? (-(v)) : (v))
#define	LIMIT_MAX(v,max)		(((v) > (max)) ? (max) : (v))
#define	LIMIT_MIN(v,min)		(((v) < (min)) ? (min) : (v))
#define	LIMIT_RANGE(v,min,max)	(LIMIT_MIN((LIMIT_MAX((v),(max))),(min)))



//*****************************************************************************
// Burst Enable: No
// Clock Source: Internal
// Reference Source: Alternate
static int _fsamp_ben_csi_rsa(int fd, s32* fsamp, char* buf)
{
	int	errs		= 0;
	s32	fref;
	s32	nclk		= -1;
	s32	nrate		= -1;

	fsamp[0]	= 0;
	buf[0]		= 0;
	errs		+= ao16_ioctl_dsl(fd, AO16_IOCTL_NCLK, &nclk);
	errs		+= ao16_ioctl_dsl(fd, AO16_IOCTL_NRATE, &nrate);

	if (errs == 0)
	{
		fref		= 16000000L + (16000000LL * nclk) / 511;
		fsamp[0]	= fref / nrate;
		sprintf(buf,
				"Alt Ref Src, Ref %ld, Nclk %ld, Nrate %ld",
				(long) fref,
				(long) nclk,
				(long) nrate);
	}

	return(errs);
}



//*****************************************************************************
// Burst Enable: No
// Clock Source: Internal
// Reference Source: Primary
static int _fsamp_ben_csi_rsp(int fd, s32* fsamp, char* buf)
{
	int	errs		= 0;
	s32	fref;
	s32	nrate		= -1;

	fsamp[0]	= 0;
	buf[0]		= 0;
	errs		+= ao16_query(fd, -1, 0, AO16_QUERY_FREF_DEFAULT, &fref);
	errs		+= ao16_ioctl_dsl(fd, AO16_IOCTL_NRATE, &nrate);

	if (errs == 0)
	{
		fsamp[0]	= fref / nrate;
		sprintf(buf,
				"Pri Ref Src, Ref %ld, Nrate %ld",
				(long) fref,
				(long) nrate);
	}

	return(errs);
}




//*****************************************************************************
// Burst Enable: No
// Clock Source: External/Software
static int _fsamp_ben_cses(int fd, s32* fsamp, char* buf)
{
	int	errs	= 0;

	fsamp[0]	= 0;
	strcpy(buf, "Ext/SW Clock Source");
	return(errs);
}



//*****************************************************************************
// Burst Enable: Yes
// Burst Trigger Source: External
static int _fsamp_bey_btse(int fd, s32* fsamp, char* buf)
{
	int	errs	= 0;

	fsamp[0]	= 0;
	strcpy(buf, "Bursting, Ext Trigger");
	return(errs);
}



//*****************************************************************************
// Burst Enable: Yes
// Burst Trigger Source: Software
static int _fsamp_bey_btssw(int fd, s32* fsamp, char* buf)
{
	int	errs	= 0;

	fsamp[0]	= 0;
	strcpy(buf, "Bursting, SW Trigger");
	return(errs);
}



//*****************************************************************************
// Burst Enable: No
// Clock Source: Internal
static int _fsamp_ben_csi(int fd, s32* fsamp, char* buf)
{
	int	errs	= 0;
	s32	ref_src	= -1;

	fsamp[0]	= 0;
	buf[0]		= 0;
	errs		+= ao16_ioctl_dsl(fd, AO16_IOCTL_CLOCK_REF_SRC, &ref_src);

	if (errs)
	{
	}
	else
	{
		switch (ref_src)
		{
			default:

				sprintf(buf, "%d. INTERNAL ERROR: %ld", __LINE__, (long) ref_src);
				break;

			case AO16_CLOCK_REF_SRC_ALT:

				errs	+= _fsamp_ben_csi_rsa(fd, fsamp, buf);
				break;

			case AO16_CLOCK_REF_SRC_PRI:

				errs	+= _fsamp_ben_csi_rsp(fd, fsamp, buf);
				break;
		}
	}

	return(errs);
}



//*****************************************************************************
// Burst Enable: No
static int _fsamp_ben(int fd, s32* fsamp, char* buf)
{
	s32	clk_src	= -1;
	int	errs	= 0;

	fsamp[0]	= 0;
	buf[0]		= 0;
	errs		+= ao16_ioctl_dsl(fd, AO16_IOCTL_CLOCK_SRC, &clk_src);

	if (errs)
	{
	}
	else
	{
		switch (clk_src)
		{
			default:

				sprintf(buf, "%d. INTERNAL ERROR: %ld", __LINE__, (long) clk_src);
				break;

			case AO16_CLOCK_SRC_INT:

				errs	+= _fsamp_ben_csi(fd, fsamp, buf);
				break;

			case AO16_CLOCK_SRC_EXT_SW:

				errs	+= _fsamp_ben_cses(fd, fsamp, buf);
				break;
		}
	}

	return(errs);
}



//*****************************************************************************
// Burst Enable: Yes
static int _fsamp_bey(int fd, s32* fsamp, char* buf)
{
	int	errs	= 0;
	s32	trg_src	= -1;

	fsamp[0]	= 0;
	buf[0]		= 0;
	errs		+= ao16_ioctl_dsl(fd, AO16_IOCTL_BURST_TRIG_SRC, &trg_src);

	if (errs)
	{
	}
	else
	{
		switch (trg_src)
		{
			default:

				sprintf(buf, "%d. INTERNAL ERROR: %ld", __LINE__, (long) trg_src);
				break;

			case AO16_BURST_TRIG_SRC_EXT:

				errs	+= _fsamp_bey_btse(fd, fsamp, buf);
				break;

			case AO16_BURST_TRIG_SRC_SW:

				errs	+= _fsamp_bey_btssw(fd, fsamp, buf);
				break;
		}
	}

	return(errs);
}



/******************************************************************************
*
*	Function:	ao16_fsamp_compute
*
*	Purpose:
*
*		Compute the necessary parameters to produce the specified sample rate.
*
*	Arguments:
*
*		fd		The handle to use to access the driver.
*
*		index	The index of the board to access. Ignore if < 0.
*
*		verbose	Work verbosely?
*
*		fsamp	This is the desirted sample rate.
*
*		ref_src	Report the reference source here.
*
*		nrate	Report the Nrate value here.
*
*		nclk	Report the Nclk value here.
*
*		rate	Report the sample rate achieved here, if non-NULL.
*
*	Returned:
*
*		>= 0	The number of errors encountered here.
*
******************************************************************************/

int ao16_fsamp_compute(int fd, int index, int verbose, s32 fsamp, s32* ref_src, s32* nrate, s32* nclk, double* rate)
{
	#define	REF_ALT		16000000.0

	double		adj;
	double		delta;
	double		delta_best	= FLT_MAX;
	int			errs		= 0;
	s32			fref;
	double		fsamp_best	= 0;
	double		fsamp_got;
	s32			fsamp_max;	// Queried value.
	s32			fsamp_min;	// Queried value.
	double		fsamp_want;
	s32			nclk_best	= 0;
	s32			nclk_max;	// Queried value.
	s32			nclk_min;	// Queried value.
	s32			nclk_use;
	s32			nrate_best;
	s32			nrate_ll;	// lower limit
	s32			nrate_max;	// Queried value.
	s32			nrate_min;	// Queried value.
	s32			nrate_ul;	// upper limit
	s32			nrate_use;
	s32			ref_src_best	= 0;
	const char*	ref_src_name	= "";

	if (verbose)
	{
		gsc_label_index("AO Sample Rate", index);
		gsc_label_long_comma(fsamp);
		printf(" S/S");
	}

	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_FREF_DEFAULT, &fref);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_FSAMP_MAX, &fsamp_max);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_FSAMP_MIN, &fsamp_min);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_NCLK_MAX, &nclk_max);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_NCLK_MIN, &nclk_min);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_NRATE_MAX, &nrate_max);
	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_NRATE_MIN, &nrate_min);

	fsamp_want	= (double) LIMIT_RANGE(fsamp, fsamp_min, fsamp_max);

	// Check using the primary source.
	nrate_use	= (long) (fref / fsamp_want);
	nrate_ll	= nrate_use - 1;
	nrate_ul	= nrate_use + 1;
	nrate_ll	= (nrate_ll < nrate_min) ? nrate_min : nrate_ll;
	nrate_ul	= (nrate_ul > nrate_max) ? nrate_max : nrate_ul;

	for (nrate_use = nrate_ll; nrate_use <= nrate_ul; nrate_use++)
	{
		fsamp_got	= ((double) fref) / nrate_use;
		delta		= fsamp_got - fsamp_want;

		if ((ABS(delta)) < (ABS(delta_best)))
		{
			delta_best		= delta;
			fsamp_best		= fsamp_got;
			nrate_best		= nrate_use;
			ref_src_best	= AO16_CLOCK_REF_SRC_PRI;
			ref_src_name	= "Primary";
		}
	}

	// Check using the alternate source.

	for (nclk_use = nclk_min; nclk_use <= nclk_max; nclk_use++)
	{
		adj			= REF_ALT * (1 + ((double) nclk_use / 511));
		nrate_use	= (s32) (adj / fsamp_want);
		nrate_ll	= nrate_use - 1;
		nrate_ul	= nrate_use + 1;
		nrate_ll	= (nrate_ll < nrate_min) ? nrate_min : nrate_ll;
		nrate_ul	= (nrate_ul > nrate_max) ? nrate_max : nrate_ul;

		for (nrate_use = nrate_ll; nrate_use <= nrate_ul; nrate_use++)
		{
			fsamp_got	= ((double) adj) / nrate_use;
			delta		= fsamp_got - fsamp_want;

			if ((ABS(delta)) < (ABS(delta_best)))
			{
				delta_best		= delta;
				fsamp_best		= fsamp_got;
				nclk_best		= nclk_use;
				nrate_best		= nrate_use;
				fref			= (s32) REF_ALT;
				ref_src_best	= AO16_CLOCK_REF_SRC_ALT;
				ref_src_name	= "Alternate";
			}
		}
	}

	// Report the results.

	ref_src[0]	= ref_src_best;
	nrate[0]	= nrate_best;
	nclk[0]		= nclk_best;

	if (rate)
		rate[0]	= fsamp_best;

	if (verbose)
	{
		printf("%s  (", errs ? " FAIL <---" : "");
		printf("%s", ref_src_name);
		printf(", Fref ");
		gsc_label_long_comma((long) fref);
		printf(", Nrate ");
		gsc_label_long_comma((long) nrate_best);

		if (nclk_best == AO16_CLOCK_REF_SRC_ALT)
		{
			printf(", Nclk ");
			gsc_label_long_comma((long) nclk_best);
		}

		printf(", Fsamp %.3f", fsamp_best);
		printf(")\n");
	}

	return(errs);
}



/******************************************************************************
*
*	Function:	ao16_fsamp_report
*
*	Purpose:
*
*		Determine and report the sample rate for the specified channel.
*
*	Arguments:
*
*		fd		The handle to use to access the driver.
*
*		index	The index of the board to access. Ignore if < 0.
*
*		verbose	Work verbosely?
*
*		chan	The index of the channel of interest.
*
*		sps		Store the sample rate here, if non-NULL.
*
*	Returned:
*
*		>= 0	The number of errors encountered here.
*
******************************************************************************/

int ao16_fsamp_report(int fd, int index, int verbose, int chan, s32* sps)
{
	char	buf[1024];
	s32		burst		= -1;
	int		errs		= 0;
	s32		fsamp		= 0;

	if (verbose)
	{
		if (chan >= 0)
			sprintf(buf, "Channel %d", chan);
		else
			strcpy(buf, "Fsamp");

		gsc_label_index(buf, index);
	}

	for (;;)
	{
		fsamp	= 0;
		buf[0]	= 0;

		if ((chan >= 0) && ((chan < 0) || (chan > 16)))
		{
			errs++;

			if (verbose)
				printf("FAIL <---  (invalid channel index: %d)\n", chan);

			break;
		}

		errs	+= ao16_ioctl_dsl(fd, AO16_IOCTL_BURST_ENABLE, &burst);

		if (errs)
			break;

		switch (burst)
		{
			default:

				if (verbose)
					sprintf(buf, "%d. INTERNAL ERROR: %ld", __LINE__, (long) burst);

				break;

			case AO16_BURST_ENABLE_NO:

				errs	+= _fsamp_ben(fd, &fsamp, buf);
				break;

			case AO16_BURST_ENABLE_YES:

				errs	+= _fsamp_bey(fd, &fsamp, buf);
				break;

		}

		if (verbose)
		{
			gsc_label_long_comma((long) fsamp);
			printf(" S/S  (%s)\n", buf);
		}

		break;
	}

	if (sps)
		sps[0]	= fsamp;

	return(errs);
}



/******************************************************************************
*
*	Function:	ao16_fsamp_report_all
*
*	Purpose:
*
*		Determine and report the sample rate for all channels.
*
*	Arguments:
*
*		fd		The handle to use to access the driver.
*
*		index	The index of the board to access. Ignore if < 0.
*
*		verbose	Work verbosely?
*
*		fsamp	Report the overall sample rate here.
*
*	Returned:
*
*		>= 0	The number of errors encountered here.
*
******************************************************************************/

int ao16_fsamp_report_all(int fd, int index, int verbose, s32* fsamp)
{
	int	chan;
	s32	chans	= -1;
	int	errs	= 0;
	s32	qty;
	s32	sps;
	s32	total	= 0;

	if (verbose)
		gsc_label_index("Sample Rates", index);

	errs	+= ao16_query(fd, -1, 0, AO16_QUERY_CHANNEL_QTY, &qty);
	errs	+= ao16_ioctl_dsl(fd, AO16_IOCTL_CHANNEL_SEL, &chans);

	if (errs)
	{
	}
	else if (chans == 0)
	{
		if (verbose)
		printf("SKIPPED   (All channels are disabled.)\n");
	}
	else
	{
		if (verbose)
		{
			printf("\n");
			gsc_label_level_inc();
		}

		for (chan = 0; chan < qty; chan++)
		{
			if (chans & (0x1L << chan))
			{
				sps	= 0;
				errs	+= ao16_fsamp_report(fd, index, verbose, chan, &sps);
				total	+= sps;
			}
		}

		if (verbose)
		{
			gsc_label("Overall Rate");
			gsc_label_long_comma(total);
			printf(" S/S\n");
			gsc_label_level_dec();
		}
	}

	if (fsamp)
		fsamp[0]	= total;

	return(errs);
}


