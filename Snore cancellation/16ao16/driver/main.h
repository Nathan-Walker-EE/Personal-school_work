// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/main.h $
// $Rev: 44907 $
// $Date: 2019-05-16 20:31:04 -0500 (Thu, 16 May 2019) $

// 16AO16: Device Driver: header file

#ifndef	__MAIN_H__
#define	__MAIN_H__

#include "16ao16.h"

#define	DEV_BAR_SHOW						0
#define	DEV_METRICS_SHOW					0
#define	DEV_PCI_ID_SHOW						0
#define	DEV_SUPPORTS_IRQ
#define	DEV_SUPPORTS_WAIT
#define	DEV_SUPPORTS_WRITE
#define	GSC_ALT_DEV_GET(a)					(a)
#define	GSC_ALT_STRUCT_GET(i,d)				(d)
#define	GSC_ALT_STRUCT_T					dev_data_t
#define	GSC_DEVS_PER_BOARD					1
typedef struct _dev_data_t					dev_data_t;

#define	GSC_RX_IO_DEBUG						0
#define	GSC_TX_IO_DEBUG						0
#define	OS_DMA_ACQUIRE_METRICS				0
#define	OS_REG_IO_DEBUG						0

#include "gsc_main.h"



// #defines *******************************************************************

#define	DEV_MODEL							"16AO16"	// Upper case form of the below.
#define	DEV_NAME							"16ao16"	// MUST AGREE WITH AO16_BASE_NAME

#define	DEV_VERSION							"3.5"		// FOR DEVICE SPECIFIC CODE ONLY!
// 3.5	Added support for the 5.x kernel series.
//		Added support for Big Endian hosts.
// 3.4	Updated BMDMA macro spelling.
//		Several minor code changes.
//		White space cleanup.
// 3.3	Updated per changes to common code.
//		Changed some field names for consistency.
// 3.2	Bug fix: was ignoring BAR setup return value.
//		Updated per changes to the OS specific PCI services.
//		Made updates for DEV_SUPPORTS_XXX macro changes.
//		Modified code so open succeeds even if initialization fails.
// 3.1	The BAR code has been updated to include the data under a single structure.
//		The register definitions have been updated.
//		I/O services now pass around an os_mem_t structure instead of a buffer pointer.
//		White space cleanup.
//		Changed the arbitrary wait event callback argument to a void* type.
//		Modified to allow multiple apps to simultaneously access a single device.
//		Correct access to BAR0 and BAR1 (is RO, was RW).
//		Added an infinite I/O timeout option.
//		Corrected the IOCTL code for the register write and mod services.
// 3.0	Updated to use the newer common driver sources.
//		Fixed a bug in the dev_write_startup() service.
//		Removed GNU notice from non-Linux specific files.
//		Removed Linux specific content from non-Linux specific source files.
//		White space cleanup.
//		Now using a spinlock rather than enabling and disabling interrupts.
//		Updated gsc_irq_open() and gsc_irq_close().
//		Updated gsc_dma_open() and gsc_dma_close().
// 2.6	Added support for Demand Mode DMA.
//		Added support for the 49.152MHz reference clock.
//		Added support for the High Voltage Range feature.
//		Added support for the Cable Signal Polarity feature.
//		Added support for the Cable Signal Isolation feature.
// 2.5	Reduced #include list in driver interface header.
// 2.4	Updated for the 3.x kernel.
//		Corrected a system clock roll over bug in ioctl.c.
// 2.3	BAR0 and BAR2 are now the only BAR regions used.
//		Include all common source, though not all are used.
// 2.2	Overhauled IOCTL based local interrupt implementation.
//		Updated per changes to the common source code.
//		Implemented IOCTL based I/O Abort support.
//		Implemented IOCTL based event/interrupt wait support.
//		Updated to use the common gsc_open() and gsc_close() services.
//		Implemented explicit local interrupt support.
//		Updated various Auto Cal related strings and macros for consistency.
//		Implemented support for the common PIO I/O routines.
//		Changed use of DEV_SUPPORTS_READ macro.
//		Changed use of DEV_SUPPORTS_WRITE macro.
//		Eliminated global dev_check_id() routine.
// 2.1	Fixed a bug in how the driver computes the buffer size.
//		Changed AO16_IOCTL_IRQ_STATE to AO16_IOCTL_IRQ_STATUS.
//		Implemented support for the common gsc_open() and gsc_close() calls.
// 2.0	Initial release of the new driver.

// I/O services
#define	DEV_IO_STREAM_QTY					(DEV_IO_RX_STREAM_QTY + DEV_IO_TX_STREAM_QTY)
#define	DEV_IO_RX_STREAM_QTY				0
#define	DEV_IO_TX_STREAM_QTY				1

#define	GSC_WRITE_PIO_WORK_32_BIT

// WAIT services
#define	DEV_WAIT_GSC_ALL					AO16_WAIT_GSC_ALL
#define	DEV_WAIT_ALT_ALL					AO16_WAIT_ALT_ALL
#define	DEV_WAIT_IO_ALL						AO16_WAIT_IO_ALL

#define	BUFFER_EMPTY						(1L << 12)
#define	BUFFER_FULL_1Q						(1L << 13)
#define	BUFFER_FULL_3Q						(1L << 14)
#define	BUFFER_FULL							(1L << 15)



// data types *****************************************************************

struct _dev_io_t
{
	// Initialized by open and IOCTL services (initialize and/or service specific).

	s32					io_mode;			// PIO, BMDMA, DMDMA
	s32					overflow_data;		// Check data overflow when writing?
	s32					overflow_frame;		// Check frame overflow when writing?
	s32					pio_threshold;		// Use PIO if samples <= this.
	s32					timeout_s;			// I/O timeout in seconds.

	// Initialized by I/O service at time of use.

	int					abort;
	gsc_dma_ch_t*		dma_channel;		// Use this channel for DMA.
	int					non_blocking;		// Is this non-blocking I/O?

	// Initialized by device specific I/O create code.

	int					bytes_per_sample;	// Sample size in bytes.
	u32					io_reg_offset;		// Offset of board's I/O FIFO.
	VADDR_T				io_reg_vaddr;		// Address of board's I/O FIFO.

	void				(*dev_io_sw_init)		(GSC_ALT_STRUCT_T* alt, dev_io_t* io);
	void				(*dev_io_close)			(GSC_ALT_STRUCT_T* alt, dev_io_t* io);
	void				(*dev_io_open)			(GSC_ALT_STRUCT_T* alt, dev_io_t* io);
	int					(*dev_io_startup)		(GSC_ALT_STRUCT_T* alt, dev_io_t* io);
	long				(*dev_pio_available)	(GSC_ALT_STRUCT_T* alt, dev_io_t* io, size_t count);
	long				(*dev_bmdma_available)	(GSC_ALT_STRUCT_T* alt, dev_io_t* io, size_t count);
	long				(*dev_dmdma_available)	(GSC_ALT_STRUCT_T* alt, dev_io_t* io, size_t count);
	long				(*dev_pio_xfer)			(GSC_ALT_STRUCT_T* alt, dev_io_t* io, const os_mem_t* mem, size_t count, os_time_tick_t st_end);
	long				(*dev_bmdma_xfer)		(GSC_ALT_STRUCT_T* alt, dev_io_t* io, const os_mem_t* mem, size_t count, os_time_tick_t st_end);
	long				(*dev_dmdma_xfer)		(GSC_ALT_STRUCT_T* alt, dev_io_t* io, const os_mem_t* mem, size_t count, os_time_tick_t st_end);

	struct
	{
		u32				abort;		// ...WAIT_IO_XXX_ABORT
		u32				done;		// ...WAIT_IO_XXX_DONE
		u32				error;		// ...WAIT_IO_XXX_ERROR
		u32				timeout;	// ...WAIT_IO_XXX_TIMEOUT
	} wait;

	// Initialize by GSC commone create code.

	os_mem_t			mem;				// I/O buffer.
	os_sem_t			sem;				// Only one Tx or Rx at a time.
};

struct _dev_data_t
{
	os_pci_t			pci;			// The kernel PCI device descriptor.
	os_data_t			os;				// OS specific data.
	os_spinlock_t		spinlock;		// Control ISR access.
	os_sem_t			sem;			// Control thread access.
	gsc_dev_type_t		board_type;		// Corresponds to basic board type.
	const char*			model;			// Base model number in upper case.
	int					board_index;	// Index of the board being accessed.
	int					users;			// Number of currently active open() requests.
	int					share;			// Were we opened in shared mode?

	gsc_bar_t			bar;			// device register mappings
	gsc_dma_t			dma;			// For DMA based I/O.
	gsc_irq_t			irq;			// For interrut support.
	gsc_wait_node_t*	wait_list;

	struct
	{					// This is for streaming I/O
		dev_io_t		tx;				// Analog Output write
		dev_io_t*		io_streams[DEV_IO_STREAM_QTY];
	} io;

	struct
	{
		VADDR_T			plx_intcsr_32;	// Interrupt Control/Status Register
		VADDR_T			plx_dmaarb_32;	// DMA Arbitration Register
		VADDR_T			plx_dmathr_32;	// DMA Threshold Register

		VADDR_T			gsc_bcr_32;		// 0x00 Board Control Register
		VADDR_T			gsc_csr_32;		// 0x04 Channel Selection Register
		VADDR_T			gsc_srr_32;		// 0x08 Sample Rate Register
		VADDR_T			gsc_bor_32;		// 0x0C Buffer Operations Register
		VADDR_T			gsc_for_32;		// 0x10 Firmware Options Register

		VADDR_T			gsc_odbr_32;	// 0x18 Output Data Buffer Register
		VADDR_T			gsc_acr_32;		// 0x1C Adjustable Clock Register

	} vaddr;

	struct
	{
		u32				gsc_for_32;			// Firmware Options Register

		int				auto_cal_ms;		// Maximum ms for auto-cal
		u32				fifo_size;			// Size of FIFO - not the fill level.
		int				initialize_ms;		// Maximum ms for initialize

		s32				cable_invert_4;		// Can the 4 trig/clock signals be inverted?
		s32				cable_passive_3;	// Can the 3 trig/clock signals go passive?
		s32				channel_mask;		// The channel selection mask.
		s32				channel_qty;		// The number of channels on the board.
		s32				channels_max;		// Maximum channels supported by model.

		s32				dmdma;				// Is Demand Mode DMA supported?
		s32				differential;		// Are the outputs differential?

		s32				filter;				// see AO16_FILTER_XXX
		s32				fref_default;		// Default reference clock rate.
		s32				fsamp_max;			// Maximum sample rate.
		s32				fsamp_min;			// Minimum sample rate.

		s32				nclk_mask;			// Mask of valid Nclk bits.
		s32				nclk_max;			// Minimum Nclk value.
		s32				nclk_min;			// Maximum Nclk value.
		s32				nrate_mask;			// Mask of valid Nrate bits.
		s32				nrate_max;			// Minimum Nrate value.
		s32				nrate_min;			// Maximum Nrate value.

		s32				volt_range;			// Which voltage range is supported?

	} cache;
};



// prototypes *****************************************************************

int			buffer_size(dev_data_t* dev, u32* size);

void		dev_io_close(dev_data_t* dev);
int			dev_io_create(dev_data_t* dev);
void		dev_io_destroy(dev_data_t* dev);
int			dev_io_open(dev_data_t* dev);
dev_io_t*	dev_io_write_select(dev_data_t* dev, size_t count);
int			dev_irq_create(dev_data_t* dev);
void		dev_irq_destroy(dev_data_t* dev);
int			dev_write_create(dev_data_t* dev, dev_io_t* io);

int			initialize_ioctl(dev_data_t* dev, void* arg);



#endif
