// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/reg_read.c $
// $Rev: 43003 $
// $Date: 2018-06-22 17:26:33 -0500 (Fri, 22 Jun 2018) $

// 16AO16: Sample Application: source file

#include "main.h"



/******************************************************************************
*
*	Function:	reg_read_test
*
*	Purpose:
*
*		Perform a test of the IOCTL service AO16_IOCTL_REG_READ.
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

int reg_read_test(int fd)
{
	static const struct
	{
		u32	reg;
		u32	value;
	} list[]	=
	{
		// reg					value
		{ AO16_GSC_BCR,			0		},
		{ AO16_GSC_CSR,			0		},
		{ AO16_GSC_SRR,			0		},
		{ AO16_GSC_BOR,			0		},
		{ AO16_GSC_FOR,			0		},
		{ AO16_GSC_AVR,			0		},
		{ AO16_GSC_ODBR,		0		},
		{ AO16_GSC_ACR,			0		},

		{ GSC_PCI_9056_VIDR,	0x10B5	},
		{ GSC_PCI_9056_DIDR,	0x9056	},
		{ GSC_PCI_9056_SVID,	0x10B5	},
		{ GSC_PCI_9056_SID,		0x3120	},

		{ GSC_PLX_9056_VIDR,	0x10B5	},
		{ GSC_PLX_9056_DIDR,	0x9056	}
	};


	int			errs	= 0;
	int			i;
	int			j;
	gsc_reg_t	parm;

	gsc_label("AO16_IOCTL_REG_READ");

	for (i = 0; i < SIZEOF_ARRAY(list); i++)
	{
		parm.reg	= list[i].reg;
		parm.value	= 0xDEADBEEF;
		parm.mask	= 0xBEEFDEAD;
		j			= ao16_ioctl(fd, AO16_IOCTL_REG_READ, (void*) &parm);

		if (j)
		{
			errs	= 1;
			printf(	"FAIL <---  (%d. i %d, ao16_ioctl() failure, errno %d)\n",
					__LINE__,
					i,
					errno);
			break;
		}

		if (parm.value == 0xDEADBEEF)
		{
			errs	= 1;
			printf("FAIL <---  (%d. i %d, value not changed)\n", __LINE__, i);
			break;
		}

		if (parm.mask != 0xBEEFDEAD)
		{
			errs	= 1;
			printf("FAIL <---  (%d. i %d, mask changed)\n", __LINE__, i);
			break;
		}

		if ((list[i].value) && (parm.value != list[i].value))
		{
			errs	= 1;
			printf(	"FAIL <---  (%d. i %d, got 0x%lX, expected 0x%lX)\n",
					__LINE__,
					i,
					(long) parm.value,
					(long) list[i].value);
			break;
		}
	}

	if (errs == 0)
		printf("PASS\n");

	return(errs);
}



