// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_query.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_query
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_QUERY service.
*
*	Arguments:
*
*		fd		Use this handle to access the device.
*
*		index	The index of the device to access. Ignore if < 0.
*				This is for display purposes only.
*
*		verbose	Work verbosely?
*
*		set		This is the query option to access.
*
*		get		The results are reported here. This may be NULL.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int	ao16_query(int fd, int index, int verbose, s32 set, s32* get)
{
	char		buf[128];
	int			errs	= 0;
	const char*	ptr;
	s32			query	= set;
	int			ret;
	s32			tmp;

	if (verbose)
	{
		switch (query)
		{
			default:

				errs++;
				ptr	= buf;
				printf(buf, "Query Error (#%ld)", (long) query);
				break;

			case AO16_QUERY_AUTO_CAL_MS:		ptr	= "Auto-Cal Duration";	break;
			case AO16_QUERY_CABLE_INVERT_4:		ptr	= "4 Trig/Clock Sigs";	break;
			case AO16_QUERY_CABLE_PASSIVE_3:	ptr	= "3 Trig/Clock Sigs";	break;
			case AO16_QUERY_CHANNEL_MASK:		ptr	= "Channel Mask";		break;
			case AO16_QUERY_CHANNEL_MAX:		ptr	= "Max Channels";		break;
			case AO16_QUERY_CHANNEL_QTY:		ptr	= "Installed Channels";	break;
			case AO16_QUERY_COUNT:				ptr	= "Query Options";		break;
			case AO16_QUERY_DEVICE_TYPE:		ptr	= "Device Type";		break;
			case AO16_QUERY_DIFFERENTIAL:		ptr	= "Diff Outputs";		break;
			case AO16_QUERY_DMDMA:				ptr	= "DMDMA Support";		break;
			case AO16_QUERY_FIFO_SIZE:			ptr	= "FIFO Size";			break;
			case AO16_QUERY_FILTER:				ptr	= "Filter";				break;
			case AO16_QUERY_FREF_DEFAULT:		ptr	= "Fref Default";		break;
			case AO16_QUERY_FSAMP_MAX:			ptr	= "Fsamp Max";			break;
			case AO16_QUERY_FSAMP_MIN:			ptr	= "Fsamp Min";			break;
			case AO16_QUERY_INIT_MS:			ptr	= "Init. Duration";		break;
			case AO16_QUERY_NCLK_MASK:			ptr	= "Nclk Mask";			break;
			case AO16_QUERY_NCLK_MAX:			ptr	= "Nclk Max";			break;
			case AO16_QUERY_NCLK_MIN:			ptr	= "Nclk Min";			break;
			case AO16_QUERY_NRATE_MASK:			ptr	= "Nrate Mask";			break;
			case AO16_QUERY_NRATE_MAX:			ptr	= "Nrate Max";			break;
			case AO16_QUERY_NRATE_MIN:			ptr	= "Nrate Min";			break;
			case AO16_QUERY_VOLT_RANGE:			ptr	= "Voltage Range";		break;
		}

		gsc_label_index(ptr, index);
	}

	ret		= ao16_ioctl(fd, AO16_IOCTL_QUERY, &set);
	errs	+= ret ? 1 : 0;

	if (verbose)
	{
		switch (query)
		{
			default:

				errs++;
				printf(buf, "%ld", (long) set);
				break;

			case AO16_QUERY_CHANNEL_MAX:
			case AO16_QUERY_CHANNEL_QTY:
			case AO16_QUERY_COUNT:
			case AO16_QUERY_NCLK_MAX:
			case AO16_QUERY_NCLK_MIN:
			case AO16_QUERY_NRATE_MAX:
			case AO16_QUERY_NRATE_MIN:

				sprintf(buf, "%ld", (long) set);
				break;

			case AO16_QUERY_CHANNEL_MASK:
			case AO16_QUERY_NCLK_MASK:
			case AO16_QUERY_NRATE_MASK:

				sprintf(buf, "0x%lX", (long) set);
				break;

			case AO16_QUERY_FREF_DEFAULT:

				sprintf(buf, "%ld Hz", (long) set);
				break;

			case AO16_QUERY_AUTO_CAL_MS:
			case AO16_QUERY_INIT_MS:

				sprintf(buf, "%ld ms", (long) set);
				break;

			case AO16_QUERY_FSAMP_MAX:
			case AO16_QUERY_FSAMP_MIN:

				sprintf(buf, "%ld S/S", (long) set);
				break;

			case AO16_QUERY_DIFFERENTIAL:
			case AO16_QUERY_DMDMA:
			case AO16_QUERY_CABLE_INVERT_4:
			case AO16_QUERY_CABLE_PASSIVE_3:

				sprintf(buf, "%s", set ? "Supported" : "Not Supported");
				break;

			case AO16_QUERY_FIFO_SIZE:

				ptr	= "";
				tmp	= set;

				if ((tmp) && ((tmp % 1024) == 0))
				{
					ptr	= "K";
					tmp	/= 1024;
				}

				sprintf(buf, "%ld%s Words", (long) tmp, ptr);
				break;

			case AO16_QUERY_DEVICE_TYPE:

				if (set == GSC_DEV_TYPE_16AO16)
				{
					strcpy(buf, "16AO16");
				}
				else
				{
					errs++;
					sprintf(buf, "INVALID: %ld", (long) set);
				}

				break;

			case AO16_QUERY_FILTER:

				switch (set)
				{
					default:

						errs++;
						sprintf(buf, "Not Recognized: %ld", (long) set);
						break;

					case AO16_FILTER_NONE:		strcpy(buf, "None");	break;
					case AO16_FILTER_10KHZ:		strcpy(buf, "10K Kz");	break;
					case AO16_FILTER_100KHZ:	strcpy(buf, "100K Hz");	break;
				}

				break;

			case AO16_QUERY_VOLT_RANGE:

				switch (set)
				{
					default:

						errs++;
						sprintf(buf, "Not Recognized: %ld", (long) set);
						break;

					case AO16_VOLT_RANGE_LOW:	strcpy(buf, "Low");		break;
					case AO16_VOLT_RANGE_HIGH:	strcpy(buf, "High");	break;
				}

				break;
		}

		if (errs)
			printf("FAIL <---  (%s)\n", buf);
		else
			printf("%s\n", buf);
	}

	if (get)
		get[0]	= set;

	return(errs);
}


