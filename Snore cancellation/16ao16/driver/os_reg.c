// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/driver/linux/os_reg.c $
// $Rev: 43585 $
// $Date: 2018-10-12 13:40:10 -0500 (Fri, 12 Oct 2018) $

// Linux: Device Driver: source file: This software is covered by the GNU GENERAL PUBLIC LICENSE (GPL).

#include "main.h"



// #defines *******************************************************************

#ifndef	OS_REG_IO_DEBUG
	#define	OS_REG_IO_DEBUG		0
#endif

#if OS_REG_IO_DEBUG

	#define	DBPRINTF(a)			printf a

	#ifndef	UL
		#define	UL				unsigned long
	#endif

#else
	#define	DBPRINTF(a)
#endif



//*****************************************************************************
u8 os_reg_mem_mx_u8(dev_data_t* dev, VADDR_T va, u8 value, u8 mask)
{
	u8	reg	= 0xEF;	// lower portion of DEADBEEF
	u8	set;

	// There is a special case for reads of register id value 0. This maps to
	// an 8-bit, BAR0 register. At this level we see it as va == 0.

	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		reg	= readb(va);
		set	= (reg & ~mask) | (value & mask);
		writeb(set, va);
		DBPRINTF(("Mem Mx  u8: rx %02lX, M %02lX, V %02lX, tx %02lX: va 0x%lX\n", (UL) reg, (UL) mask, (UL) value, (UL) set, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);

	return(reg);
}



//*****************************************************************************
u16 os_reg_mem_mx_u16(dev_data_t* dev, VADDR_T va, u16 value, u16 mask)
{
	u16	reg	= 0xBEEF;	// lower portion of DEADBEEF
	u16	set;

	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		reg	= readw(va);
		set	= (reg & ~mask) | (value & mask);
		writew(set, va);
		DBPRINTF(("Mem Mx u16: rx %04lX, M %04lX, V %04lX, tx %04lX: va 0x%lX\n", (UL) reg, (UL) mask, (UL) value, (UL) set, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);

	return(reg);
}



//*****************************************************************************
u32 os_reg_mem_mx_u32(dev_data_t* dev, VADDR_T va, u32 value, u32 mask)
{
	u32	reg	= 0xDEAFBEEF;
	u32	set;

	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		reg	= readl(va);
		set	= (reg & ~mask) | (value & mask);
		writel(set, va);
		DBPRINTF(("Mem Mx u32: rx %08lX, M %08lX, V %08lX, tx %08lX: va 0x%lX\n", (UL) reg, (UL) mask, (UL) value, (UL) set, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);

	return(reg);
}



//*****************************************************************************
u8 os_reg_mem_rx_u8(dev_data_t* dev, VADDR_T va)
{
	u8	value	= 0xEF;	// lower portion of DEADBEEF

	// There is a special case for reads of register id value 0. This maps to
	// an 8-bit, BAR0 register. At this level we see it as va == 0.

	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		value	= readb(va);
		DBPRINTF(("Mem Rx  u8: rx %02lX: va 0x%lX\n", (UL) value, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);

	return(value);
}



//*****************************************************************************
u16 os_reg_mem_rx_u16(dev_data_t* dev, VADDR_T va)
{
	u16	value	= 0xBEEF;	// lower portion of DEADBEEF

	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		value	= readw(va);
		DBPRINTF(("Mem Rx u16: rx %04lX: va 0x%lX\n", (UL) value, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);

	return(value);
}



//*****************************************************************************
u32 os_reg_mem_rx_u32(dev_data_t* dev, VADDR_T va)
{
	u32	value	= 0xDEADBEEF;

	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		value	= readl(va);
		DBPRINTF(("Mem Rx u32: rx %08lX: va 0x%lX\n", (UL) value, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);

	return(value);
}



//*****************************************************************************
void os_reg_mem_tx_u8(dev_data_t* dev, VADDR_T va, u8 value)
{
	// There is a special case for reads of register id value 0. This maps to
	// an 8-bit, BAR0 register. At this level we see it as va == 0.

	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		writeb(value, va);
		DBPRINTF(("Mem Tx  u8: tx %02lX: va 0x%lX\n", (UL) value, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);
}



//*****************************************************************************
void os_reg_mem_tx_u16(dev_data_t* dev, VADDR_T va, u16 value)
{
	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		writew(value, va);
		DBPRINTF(("Mem Tx u16: tx %04lX: va 0x%lX\n", (UL) value, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);
}



//*****************************************************************************
void os_reg_mem_tx_u32(dev_data_t* dev, VADDR_T va, u32 value)
{
	if (dev)
		gsc_irq_access_lock(dev);

	if (va)
	{
		writel(value, va);
		DBPRINTF(("Mem Tx u32: tx %08lX: va 0x%lX\n", (UL) value, (UL) va));
	}

	if (dev)
		gsc_irq_access_unlock(dev);
}



//*****************************************************************************
u8 os_reg_pci_mx_u8(dev_data_t* dev, int lock, u16 offset, u8 value, u8 mask)
{
	u8	reg	= 0xEF;	// lower portion of DEADBEEF
	u8	set;

	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_read_config_byte(dev->pci.pd, offset, &reg);
		set	= (reg & ~mask) | (value & mask);
		pci_write_config_byte(dev->pci.pd, offset, set);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Mx  u8: rx %02lX, M %02lX, V %02lX, tx %02lX: offset 0x%02lX\n", (UL) reg, (UL) mask, (UL) value, (UL) set, (UL) offset));
	}

	return(reg);
}



//*****************************************************************************
u16 os_reg_pci_mx_u16(dev_data_t* dev, int lock, u16 offset, u16 value, u16 mask)
{
	u16	reg	= 0xBEEF;	// lower portion of DEADBEEF
	u16	set;

	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_read_config_word(dev->pci.pd, offset, &reg);
		set	= (reg & ~mask) | (value & mask);
		pci_write_config_word(dev->pci.pd, offset, set);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Mx u16: rx %04lX, M %04lX, V %04lX, tx %04lX: offset 0x%02lX\n", (UL) reg, (UL) mask, (UL) value, (UL) set, (UL) offset));
	}

	return(reg);
}



//*****************************************************************************
u32 os_reg_pci_mx_u32(dev_data_t* dev, int lock, u16 offset, u32 value, u32 mask)
{
	u32	reg	= 0xDEADBEEF;
	u32	set;

	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_read_config_dword(dev->pci.pd, offset, &reg);
		set	= (reg & ~mask) | (value & mask);
		pci_write_config_dword(dev->pci.pd, offset, set);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Mx u32: rx %08lX, M %08lX, V %08lX, tx %08lX: offset 0x%02lX\n", (UL) reg, (UL) mask, (UL) value, (UL) set, (UL) offset));
	}

	return(reg);
}



//*****************************************************************************
u8 os_reg_pci_rx_u8(dev_data_t* dev, int lock, u16 offset)
{
	u8	reg	= 0xEF;	// lower portion of DEADBEEF

	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_read_config_byte(dev->pci.pd, offset, &reg);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Rx  u8: rx %02lX: offset 0x%02lX\n", (UL) reg, (UL) offset));
	}

	return(reg);
}



//*****************************************************************************
u16 os_reg_pci_rx_u16(dev_data_t* dev, int lock, u16 offset)
{
	u16	reg	= 0xBEEF;	// lower portion of DEADBEEF

	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_read_config_word(dev->pci.pd, offset, &reg);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Rx u16: rx %04lX: offset 0x%02lX\n", (UL) reg, (UL) offset));
	}

	return(reg);
}



//*****************************************************************************
u32 os_reg_pci_rx_u32(dev_data_t* dev, int lock, u16 offset)
{
	u32	reg	= 0xDEADBEEF;

	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_read_config_dword(dev->pci.pd, offset, &reg);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Rx u32: rx %08lX: offset 0x%02lX\n", (UL) reg, (UL) offset));
	}

	return(reg);
}



//*****************************************************************************
void os_reg_pci_tx_u8(dev_data_t* dev, int lock, u16 offset, u8 value)
{
	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_write_config_byte(dev->pci.pd, offset, value);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Tx  u8: tx %02lX: offset 0x%02lX\n", (UL) value, (UL) offset));
	}
}



//*****************************************************************************
void os_reg_pci_tx_u16(dev_data_t* dev, int lock, u16 offset, u16 value)
{
	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_write_config_word(dev->pci.pd, offset, value);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Tx u16: tx %04lX: offset 0x%02lX\n", (UL) value, (UL) offset));
	}
}



//*****************************************************************************
void os_reg_pci_tx_u32(dev_data_t* dev, int lock, u16 offset, u32 value)
{
	if (dev)
	{
		if (lock)
			gsc_irq_access_lock(dev);

		pci_write_config_dword(dev->pci.pd, offset, value);

		if (lock)
			gsc_irq_access_unlock(dev);

		DBPRINTF(("PCI Tx u32: tx %08lX: offset 0x%02lX\n", (UL) value, (UL) offset));
	}
}


