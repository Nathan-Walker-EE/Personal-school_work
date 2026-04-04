// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_buffer_size.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	16ao16_buffer_size
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_BUFFER_SIZE service.
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
*		set		The setting to apply.
*
*		get		The current setting is recorded here, if not NULL.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int	ao16_buffer_size(int fd, int index, int verbose, s32 set, s32* get)
{
	char	buf[128];
	int		errs	= 0;
	int		ret;

	if (verbose)
		gsc_label_index("Buffer Size", index);

	ret		= ao16_ioctl(fd, AO16_IOCTL_BUFFER_SIZE, &set);
	errs	+= ret ? 1 : 0;

	switch (set)
	{
		default:

			sprintf(buf, "INVALID: %ld", (long) set);
			break;

		case AO16_BUFFER_SIZE_8:

			strcpy(buf, "8");
			break;

		case AO16_BUFFER_SIZE_16:

			strcpy(buf, "16");
			break;

		case AO16_BUFFER_SIZE_32:

			strcpy(buf, "32");
			break;

		case AO16_BUFFER_SIZE_64:

			strcpy(buf, "64");
			break;

		case AO16_BUFFER_SIZE_128:

			strcpy(buf, "128");
			break;

		case AO16_BUFFER_SIZE_256:

			strcpy(buf, "256");
			break;

		case AO16_BUFFER_SIZE_512:

			strcpy(buf, "512");
			break;

		case AO16_BUFFER_SIZE_1K:

			strcpy(buf, "1,024");
			break;

		case AO16_BUFFER_SIZE_2K:

			strcpy(buf, "2,048");
			break;

		case AO16_BUFFER_SIZE_4K:

			strcpy(buf, "4,096");
			break;

		case AO16_BUFFER_SIZE_8K:

			strcpy(buf, "8,192");
			break;

		case AO16_BUFFER_SIZE_16K:

			strcpy(buf, "16,384");
			break;

		case AO16_BUFFER_SIZE_32K:

			strcpy(buf, "32,768");
			break;

		case AO16_BUFFER_SIZE_64K:

			strcpy(buf, "65,536");
			break;

		case AO16_BUFFER_SIZE_128K:

			strcpy(buf, "131,072");
			break;

		case AO16_BUFFER_SIZE_256K:

			strcpy(buf, "262,144");
			break;
	}

	if (verbose)
		printf("%s  (%s samples deep)\n", errs ? "FAIL <---" : "PASS", buf);

	if (get)
		get[0]	= set;

	return(errs);
}


