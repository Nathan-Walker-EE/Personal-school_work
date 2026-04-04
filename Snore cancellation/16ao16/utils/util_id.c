// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_id.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	_id_board_pci
*
*	Purpose:
*
*		Identify the board using the PCI registers.
*
*	Arguments:
*
*		fd		The handle used to access the device.
*
*	Returned:
*
*		>= 0	The number of errors encountered here.
*
******************************************************************************/

static int _id_board_pci(int fd)
{
	int	errs	= 0;
	u32	reg;

	gsc_label("Vendor ID");
	errs	+= ao16_reg_read(fd, -1, 0, GSC_PCI_9056_VIDR, &reg);
	printf("0x%04lX      ", (long) reg);

	if (reg == 0x10B5)
	{
		printf("(PLX)\n");
	}
	else
	{
		errs++;
		printf("(UNKNOWN) FAIL <---\n");
	}

	gsc_label("Device ID");
	errs	+= ao16_reg_read(fd, -1, 0, GSC_PCI_9056_DIDR, &reg);
	printf("0x%04lX      ", (long) reg);

	if (reg == 0x9056)
	{
		printf("(PCI9056)\n");
	}
	else
	{
		errs++;
		printf("(UNKNOWN) FAIL <---\n");
	}

	gsc_label("Sub Vendor ID");
	errs	+= ao16_reg_read(fd, -1, 0, GSC_PCI_9056_SVID, &reg);
	printf("0x%04lX      ", (long) reg);

	if (reg == 0x10B5)
	{
		printf("(PLX)\n");
	}
	else
	{
		errs++;
		printf("(UNKNOWN) FAIL <---\n");
	}

	gsc_label("Subsystem ID");
	errs	+= ao16_reg_read(fd, -1, 0, GSC_PCI_9056_SID, &reg);
	printf("0x%04lX      ", (long) reg);

	if (reg == 0x3120)
	{
		printf("(16AO16)\n");
	}
	else
	{
		errs++;
		printf("(UNKNOWN) FAIL <---\n");
	}

	return(errs);
}



/******************************************************************************
*
*	Function:	ao16_id_device
*
*	Purpose:
*
*		Identify the board.
*
*	Arguments:
*
*		fd		The handle to use to access the board.
*
*		index	The index of the board or -1 if we don't care.
*
*		verbose	Work verbosely?
*
*	Returned:
*
*		>= 0	The number of errors encountered here.
*
******************************************************************************/

int ao16_id_device(int fd, int index, int verbose)
{
	s32			device_type;
	int			errs	= 0;
	const char*	psz		= "";

	if (verbose)
		gsc_label_index("Device", index);

	errs	= ao16_query(fd, -1, 0, AO16_QUERY_DEVICE_TYPE, &device_type);

	if (errs == 0)
	{
		switch (device_type)
		{
			default:

				errs	= 1;

				if (verbose)
				{
					printf(	"FAIL <---  (unexpected device type: %ld)",
							(long) device_type);
				}

				break;

			case GSC_DEV_TYPE_16AO16:

				psz	= "16AO16";
				break;
		}
	}

	if (verbose)
	{
		printf("%s\n", psz);
		gsc_label_level_inc();
	}

	errs	+= _id_board_pci(fd);

	if (verbose)
		gsc_label_level_dec();

	return(errs);
}


