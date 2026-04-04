// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_reg_read.c $
// $Rev: 42939 $
// $Date: 2018-06-01 11:02:54 -0500 (Fri, 01 Jun 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/*******************************************************************************
*
*	Function:	ao16_reg_read
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_REG_READ service.
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
*		reg		The register to access.
*
*		get		The value read goes here.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
*******************************************************************************/

int ao16_reg_read(int fd, int index, int verbose, u32 reg, u32* get)
{
	gsc_reg_t	arg;
	int			errs	= 0;
	int			ret;
	const char*	name;

	if (verbose)
		gsc_label_index("Register Read", index);

	arg.reg		= reg;
	arg.value	= 0xDEADBEEF;
	arg.mask	= 0;	// unused here
	ret			= ao16_ioctl(fd, AO16_IOCTL_REG_READ, &arg);
	errs		+= ret ? 1 : 0;

	if (verbose)
	{
		name	= ao16_reg_get_name(reg);
		printf(	"%s  (val 0x%08lX, %s)\n",
				errs ? "FAIL <---" : "PASS",
				(unsigned long) arg.value,
				name ? name : "UNKNOWN");
	}

	if (get)
		get[0]	= arg.value;

	return(errs);
}


