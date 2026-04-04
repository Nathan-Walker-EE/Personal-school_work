// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/reg_write.c $
// $Rev: 43003 $
// $Date: 2018-06-22 17:26:33 -0500 (Fri, 22 Jun 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



/******************************************************************************
*
*	Function:	reg_write_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_REG_WRITE.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		>= 0	The number of errors encounterred.
*
******************************************************************************/

int reg_write_test(int fd)
{
	static const struct
	{
		u32	reg;
		u32	value;
		u32	mask;
	} list[]	=
	{
		// reg			value		mask
		{ AO16_GSC_ACR,	0x000003AA,	0x0003FFFF	},
		{ AO16_GSC_ACR,	0x00000005,	0x0003FFFF	},

		{ AO16_GSC_SRR,	0x00025599,	0x0003FFFF	},
		{ AO16_GSC_SRR,	0x0001AA66,	0x0003FFFF	},
	};

	int			errs	= 0;
	int			i;
	int			j;
	gsc_reg_t	parm;

	gsc_label("AO16_IOCTL_REG_WRITE");

	for (i = 0; i < SIZEOF_ARRAY(list); i++)
	{
		parm.reg	= list[i].reg;
		parm.value	= list[i].value;
		parm.mask	= 0xBEEFDEAD;
		j			= ao16_ioctl(fd, AO16_IOCTL_REG_WRITE, (void*) &parm);

		if (j)
		{
			errs	= 1;
			printf(	"FAIL <---  (%d. i %d, ao16_ioctl() failure, errno %d)\n",
					__LINE__,
					i,
					errno);
			break;
		}

		if (parm.mask != 0xBEEFDEAD)
		{
			errs	= 1;
			printf("FAIL <---  (%d. i %d, mask changed)\n", __LINE__, i);
			break;
		}

		// Now read the register and verify the bits of interest.
		j	= ao16_ioctl(fd, AO16_IOCTL_REG_READ, (void*) &parm);

		if (j)
		{
			errs	= 1;
			printf(	"FAIL <---  (%d. i %d, ao16_ioctl() failure, errno %d)\n",
					__LINE__,
					i,
					errno);
			break;
		}

		if ((list[i].value & list[i].mask) != (parm.value & list[i].mask))
		{
			errs	= 1;
			printf(	"FAIL <---  (%d. i %d, mask 0x%lX, got 0x%lX, expected 0x%lX)\n",
					__LINE__,
					i,
					(long) list[i].mask,
					(long) parm.value,
					(long) list[i].value);
			break;
		}
	}

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



