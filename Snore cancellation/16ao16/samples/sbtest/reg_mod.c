// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/reg_mod.c $
// $Rev: 43003 $
// $Date: 2018-06-22 17:26:33 -0500 (Fri, 22 Jun 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



/******************************************************************************
*
*	Function:	reg_mod_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_REG_MOD.
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

int reg_mod_test(int fd)
{
	static const struct
	{
		u32	reg;
		u32	mask;
	} list[]	=
	{
		// reg			mask
		{ AO16_GSC_BCR,	0x000307F9	},
		{ AO16_GSC_BOR,	0x0000013F	},
	};

	u32			bit;
	int			errs	= 0;
	int			i;
	int			j;
	gsc_reg_t	parm;
	u32			value;

	gsc_label("AO16_IOCTL_REG_MOD");

	for (i = 0; (errs == 0) && (i < SIZEOF_ARRAY(list)); i++)
	{

		for (bit = 0x1; bit; bit <<= 1)
		{
			if ((list[i].mask & bit) == 0)
				continue;

			// Read the register value so we can restore it later.
			parm.reg	= list[i].reg;
			j			= ao16_ioctl(fd, AO16_IOCTL_REG_READ, (void*) &parm);
			value		= parm.value;

			if (j)
			{
				errs	= 1;
				printf(	"FAIL <---  (%d. i %d, ao16_ioctl() failure, errno %d)\n",
						__LINE__,
						i,
						errno);
				break;
			}

			// Set the bit low.
			parm.value	= 0;
			parm.mask	= bit;
			j			= ao16_ioctl(fd, AO16_IOCTL_REG_MOD, (void*) &parm);

			if (j)
			{
				errs	= 1;
				printf(	"FAIL <---  (%d. i %d, ao16_ioctl() failure, errno %d)\n",
						__LINE__,
						i,
						errno);
				break;
			}

			// Now verify that the bit is low.
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

			if (parm.value & bit)
			{
				errs	= 1;
				printf(	"FAIL <---  (%d. i %d, bit not low, mask 0x%lX, got 0x%lX)\n",
						__LINE__,
						i,
						(long) bit,
						(long) parm.value);
				break;
			}

			// Now restore the register to its previous value.
			parm.value	= value;
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

			// Set the bit high.
			parm.value	= bit;
			parm.mask	= bit;
			j			= ao16_ioctl(fd, AO16_IOCTL_REG_MOD, (void*) &parm);

			if (j)
			{
				errs	= 1;
				printf(	"FAIL <---  (%d. i %d, ao16_ioctl() failure, errno %d)\n",
						__LINE__,
						i,
						errno);
				break;
			}

			// Now verify that the bit is high.
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

			if ((parm.value & bit) == 0)
			{
				errs	= 1;
				printf(	"FAIL <---  (%d. i %d, bit not high, mask 0x%lX, got 0x%lX)\n",
						__LINE__,
						i,
						(long) bit,
						(long) parm.value);
				break;
			}

			// Now restore the register to its previous value.
			parm.value	= value;
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
		}
	}

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



