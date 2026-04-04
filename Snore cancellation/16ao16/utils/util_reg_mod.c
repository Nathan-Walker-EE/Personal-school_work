// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_reg_mod.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_reg_mod
*
*	Purpose:
*
*		Provide a visual wrapper for the AO16_IOCTL_REG_MOD service.
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
*		reg		This is the register to access.
*
*		set		This is the value to apply via the mask.
*
*		mask	These are the bits to modify.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int ao16_reg_mod(int fd, int index, int verbose, u32 reg, u32 set, u32 mask)
{
	gsc_reg_t	arg;
	int			errs	= 0;
	int			ret;
	const char*	name;

	if (verbose)
		gsc_label_index("Register Mod", index);

	arg.reg		= reg;
	arg.value	= set;
	arg.mask	= mask;
	ret			= ao16_ioctl(fd, AO16_IOCTL_REG_MOD, &arg);
	errs		+= ret ? 1 : 0;

	if (verbose)
	{
		name	= ao16_reg_get_name(reg);
		printf(	"%s  (val 0x%08lX, mask 0x%08lX, %s)\n",
				errs ? "FAIL <---" : "PASS",
				(unsigned long) set,
				(unsigned long) mask,
				name ? name : "UNKNOWN");
	}

	return(errs);
}


