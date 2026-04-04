// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/driver/gsc_endian.c $
// $Rev: 45074 $
// $Date: 2019-06-12 14:17:47 -0500 (Wed, 12 Jun 2019) $

// OS & Device Independent: Device Driver: source file

#include "main.h"



//*****************************************************************************
int gsc_endian_init(dev_data_t* dev)
{
	u16		did		= os_reg_pci_rx_u16(dev, 1, 0x02);
	int		ret		= 0;
	VADDR_T	va		= PLX_VADDR(dev, 0x0C);
	u32		val;

#if defined(__BIG_ENDIAN)

	val	= 0xE6;

#else

	val	= 0x00;

#endif

	switch (did)
	{
		default:		ret	= -EOPNOTSUPP;
						printf(	"%d. gsc_endian_init: PLX device not supported: 0x%04lX)\n",
								__LINE__,
								(long) did);
						break;

		case 0x9060:	os_reg_mem_tx_u8(dev, va, val & 0x0F);
						break;

		case 0x9056:
		case 0x9080:
		case 0x9656:	os_reg_mem_tx_u8(dev, va, val & 0xFF);
						break;
	}

	return(ret);
}


