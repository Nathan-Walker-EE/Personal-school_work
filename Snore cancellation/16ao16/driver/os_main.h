// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/driver/linux/os_main.h $
// $Rev: 45073 $
// $Date: 2019-06-12 14:13:28 -0500 (Wed, 12 Jun 2019) $

// Device Driver: Linux: header file: This software is covered by the GNU GENERAL PUBLIC LICENSE (GPL).

#ifndef	__OS_MAIN_H__
#define	__OS_MAIN_H__

#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/page.h>
#include <asm/types.h>

#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/wait.h>

typedef struct
{
	struct pci_dev*	pd;
	int				enabled;	// Has the device been enabled?
} os_pci_t;

#include "os_kernel_2_2.h"
#include "os_kernel_2_4.h"
#include "os_kernel_2_6.h"
#include "os_kernel_3.h"
#include "os_kernel_4.h"
#include "os_kernel_5.h"
#include "gsc_common.h"

#ifndef	OS_DMA_ACQUIRE_METRICS
#define	OS_DMA_ACQUIRE_METRICS				0
#endif

#if (BITS_PER_LONG == 32)
// This is supported on 64-bit builds only.
#undef	OS_DMA_ACQUIRE_METRICS
#define	OS_DMA_ACQUIRE_METRICS				0
#endif

#ifndef	CONFIG_PCI
	#error This driver requires PCI support.
#endif



// #defines *******************************************************************

// This is for the OS specific code.
#define	OS_COMMON_VERSION			"28"
// 28	Added support for the 5.x kernel series.
//		Added DMA support for an IOMMU.
//		Added error reporting when DMA memory cound not be allocated.
//		Reduced code included when read and writ are both unsupported.
//		Added support for Big Endian hosts.
//		Added error reporting when IRQ cound not be acquired during open requests.
//		Some minor code reorganization.
// 27	Updated BAR creation logic.
// 26	Implemented support for a stream selection field in the I/O bytes argument.
//		Implemented register access debug code.
// 25	Implemented calls: os_irq_create() and os_irq_destroy()
// 24	Field in gsc_irq_t renamed for clarity.
// 23	Changed macro name for clarity.
// 22	Added support for some DMA metrics.
// 21	Updated to accomodate Fedora 27.
// 20	Updated to accommodate porting to Windows KMDF.
// 19	Updated the low level register interface.
//		Updated the driver initialization logic.
// 18	Added special case handling for reads of register id value 0.
//		Added support for reporting metrics information during driver installation.
//		Improved response to being signalled (i.e. being told to abort).
// 17	Removed use of function check_mem_region() in 4.x kernels.
// 16	Implemented support for the 4.x kernels.
//		Added checks on some pointers before using them.
// 15	An event wait timeout value of zero now means do not timeout.
//		Modified to allow multiple apps to simultaneously access a single device.
// 14	Added safety check in EVENT_RESUME_IRQ().
// 13	Removed unused macro (OS_CAST_VOIDP_TO_U32).
// 12	Moved some BAR functionality here from more common code.
//		Renamed os_event_cleanup() to os_event_destroy().
//		Copying to and from user space is now done with more use specific routines.
//		Simplified use of os_mem_t structures and content.
//		Dropped I/O register access services.
// 11	Simplified error messages.
// 10	Removed the use of __DATE__ and __TIME__.
// 9	Made ISR access more OS agnostic.
//		Made PCI registers accessible from inside an ISR.
// 8	Additional LINT modifications.
// 7	Updated sources for use by all drivers.
// 6	Changes to support the 16AO4MF: added macro for OS_IOCTL()
// 5	Additional changes for cross-OS porting.
// 4	Cleanup of #includes.
//		Updated the /proc start code.
// 3	Additional LINTing.
// 2	Changed how DMA memory is allocated.
//		LINTing update.
//		Updated read and write code to aid clarity.
//		Changed some routine names from gsc_* to os_*.
// 1	Code cleanup.
//		Merge of limited content from the Linux only source tree.
// 0	Initial release.
//		This code supports devices with multiple data streams (i.e. SIO4).
//		This is a modified version of the single stream driver tree.

// 32-bit compatibility support.
#define	GSC_IOCTL_32BIT_ERROR				(-1)	// There is a problem.
#define	GSC_IOCTL_32BIT_NONE				0		// Support not in the kernel.
#define	GSC_IOCTL_32BIT_NATIVE				1		// 32-bit support on 32-bit OS
#define	GSC_IOCTL_32BIT_TRANSLATE			2		// globally translated "cmd"s
#define	GSC_IOCTL_32BIT_COMPAT				3		// compat_ioctl service
#define	GSC_IOCTL_32BIT_DISABLED			4		// Support is disabled.

#define	printf								printk
#define	PRINTF_ISR							printk

#define	os_mem_copy_from_user_ioctl			os_mem_copy_from_user
#define	os_mem_copy_from_user_tx			os_mem_copy_from_user
#define	os_mem_copy_to_user_ioctl			os_mem_copy_to_user
#define	os_mem_copy_to_user_rx				os_mem_copy_to_user



// data types *****************************************************************

typedef struct
{
	void*			key;
	spinlock_t		lock;
	unsigned long	flags;
} os_spinlock_t;

typedef	struct
{
	void*				key;	// struct is valid only if key == & of struct
	struct semaphore	sem;
} os_sem_t;

typedef struct							// Don't forget: _pci_region in gsc_reg.c
{
	// These fields MUST be kept as is.
	int					index;		// BARx
	int					offset;		// Offset of BARx register in PCI space.
	u32					reg;		// Actual BARx register value.
	u32					flags;		// lower register bits
	int					io_mapped;	// Is this an I/O mapped region?
	unsigned long		phys_adrs;	// Physical address of region.
	u32					size;		// Region size in bytes.
	int					rw;			// Read/Write access? If not, then read-only.

	// The following are not order specific.

	// These are computed when mapped in.
	u32					requested;	// Is resource requested from OS?
	VADDR_T				vaddr;		// Kernel virtual address.

	// Any OS specific fields must go after this point.
	dev_data_t*			dev;

} os_bar_t;

typedef unsigned long	os_time_tick_t;

typedef struct
{
	int					reserved;
} os_data_t;

typedef struct
{
	WAIT_QUEUE_ENTRY_T	entry;
	WAIT_QUEUE_HEAD_T	queue;
	int					condition;
} os_event_t;

typedef struct
{
	int	allotted;
} os_irq_t;

typedef struct
{
	unsigned long	adrs;
	void*			ptr;
	u32				bytes;
	int				order;
	dev_data_t*		dev;
} os_mem_t;

typedef struct
{
	dev_data_t*				dev_list[10];
	int						dev_qty;
	int						driver_loaded;
	struct file_operations	fops;
	int						ioctl_32bit;	// IOCTL_32BIT_XXX
	int						major_number;
	int						proc_enabled;
} gsc_global_t;



// variables ******************************************************************

#if OS_DMA_ACQUIRE_METRICS

extern	long long	dma_met_chan_get_count;
extern	long long	dma_met_chan_get_byte_sum;
extern	long long	dma_met_chan_get_ns_sum;
extern	long long	dma_met_chan_get_retry_sum;

#endif



// prototypes *****************************************************************

int				os_bar_create(dev_data_t* dev, int index, int io, int mem, os_bar_t* bar);
void			os_bar_destroy(os_bar_t* bar);

int				os_close(struct inode *inode, struct file *fp);

void			os_event_create(os_event_t* evnt);
void			os_event_destroy(os_event_t* evnt);
void			os_event_resume(os_event_t* evnt);
void			os_event_wait(os_event_t* evnt, os_time_tick_t timeout);

int				os_ioctl_bkl(struct inode* inode, struct file* fp, unsigned int cmd, unsigned long arg);
long			os_ioctl_compat(struct file* fp, unsigned int cmd, unsigned long arg);
int				os_ioctl_init(void);
void			os_ioctl_reset(void);
long			os_ioctl_unlocked(struct file* fp, unsigned int cmd, unsigned long arg);
int				os_irq_acquire(dev_data_t* dev);
int				os_irq_create(dev_data_t* dev);
void			os_irq_destroy(dev_data_t* dev);
void			os_irq_release(dev_data_t* dev);

int				os_mem_copy_from_user(void* dst, const void* src, long size);
int				os_mem_copy_to_user(void* dst, const void* src, long size);
void*			os_mem_data_alloc(size_t size);
void			os_mem_data_free(void* ptr);
void*			os_mem_dma_alloc(GSC_ALT_STRUCT_T* alt, size_t* size, os_mem_t* mem);		// common call
void*			os_mem_dma_alloc_kernel(dev_data_t* dev, u8 order, unsigned long* adrs);	// kernel specific, called by common call
void			os_mem_dma_free(os_mem_t* mem);			// common call
void			os_mem_dma_free_kernel(os_mem_t* mem);	// kernel specific, called by common call
void			os_metrics(dev_data_t* dev, VADDR_T va);
void			os_module_count_dec(void);
int				os_module_count_inc(void);


int				os_open(struct inode *inode, struct file *fp);

void			os_pci_dev_disable(os_pci_t* pci);
int				os_pci_dev_enable(os_pci_t* pci);
void			os_pci_master_clear(os_pci_t* pci);
int				os_pci_master_set(os_pci_t* pci);
int				os_proc_read(char* page, char** start, off_t offset, int count, int* eof, void* data);
int				os_proc_start(void);
int				os_proc_start_detail(void);
void			os_proc_stop(void);

ssize_t			os_read(struct file* filp, char* buf, size_t count, loff_t* offp);

u8				os_reg_mem_mx_u8	(dev_data_t* dev, VADDR_T va, u8  value, u8  mask);
u16				os_reg_mem_mx_u16	(dev_data_t* dev, VADDR_T va, u16 value, u16 mask);
u32				os_reg_mem_mx_u32	(dev_data_t* dev, VADDR_T va, u32 value, u32 mask);
u8				os_reg_mem_rx_u8	(dev_data_t* dev, VADDR_T va);
u16				os_reg_mem_rx_u16	(dev_data_t* dev, VADDR_T va);
u32				os_reg_mem_rx_u32	(dev_data_t* dev, VADDR_T va);
void			os_reg_mem_tx_u8	(dev_data_t* dev, VADDR_T va, u8 value);
void			os_reg_mem_tx_u16	(dev_data_t* dev, VADDR_T va, u16 value);
void			os_reg_mem_tx_u32	(dev_data_t* dev, VADDR_T va, u32 value);

u8				os_reg_pci_mx_u8	(dev_data_t* dev, int lock, u16 offset, u8  value, u8  mask);
u16				os_reg_pci_mx_u16	(dev_data_t* dev, int lock, u16 offset, u16 value, u16 mask);
u32				os_reg_pci_mx_u32	(dev_data_t* dev, int lock, u16 offset, u32 value, u32 mask);
u8				os_reg_pci_rx_u8	(dev_data_t* dev, int lock, u16 offset);
u16				os_reg_pci_rx_u16	(dev_data_t* dev, int lock, u16 offset);
u32				os_reg_pci_rx_u32	(dev_data_t* dev, int lock, u16 offset);
void			os_reg_pci_tx_u8	(dev_data_t* dev, int lock, u16 offset, u8 value);
void			os_reg_pci_tx_u16	(dev_data_t* dev, int lock, u16 offset, u16 value);
void			os_reg_pci_tx_u32	(dev_data_t* dev, int lock, u16 offset, u32 value);

int				os_sem_create(os_sem_t* sem);
void			os_sem_destroy(os_sem_t* sem);
int				os_sem_lock(os_sem_t* sem);
void			os_sem_unlock(os_sem_t* sem);
int				os_spinlock_create(os_spinlock_t* lock);
void			os_spinlock_destroy(os_spinlock_t* lock);
void			os_spinlock_lock(os_spinlock_t* lock);
void			os_spinlock_unlock(os_spinlock_t* lock);

long			os_time_delta_ms(const os_time_t* tt1, const os_time_t* tt2);
void			os_time_get(os_time_t* tt);
os_time_tick_t	os_time_ms_to_ticks(long ms);
os_time_tick_t	os_time_tick_get(void);
int				os_time_tick_rate(void);
int				os_time_tick_sleep(int ticks);
int				os_time_tick_timedout(os_time_tick_t tick_time);
long			os_time_ticks_to_ms(os_time_tick_t ticks);
void			os_time_us_delay(long us);
int				os_time_sleep_ms(long ms);

ssize_t			os_write(struct file* filp, const char* buf, size_t count, loff_t* offp);



#endif
