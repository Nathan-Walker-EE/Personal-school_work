// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/driver/linux/os_common.h $
// $Rev: 45073 $
// $Date: 2019-06-12 14:13:28 -0500 (Wed, 12 Jun 2019) $

// Device Driver: Linux: header file: This software is covered by the GNU GENERAL PUBLIC LICENSE (GPL).

#ifndef	__OS_COMMON_H__
#define	__OS_COMMON_H__



// #defines *******************************************************************

#define	OS_IOCTL(i)							_IO  (GSC_IOCTL,(i))
#define	OS_IOCTL_R(i,s,t)					_IOR (GSC_IOCTL,(i),t)
#define	OS_IOCTL_RW(i,s,t)					_IOWR(GSC_IOCTL,(i),t)
#define	OS_IOCTL_W(i,s,t)					_IOW (GSC_IOCTL,(i),t)

#define	OS_IOCTL_DIR_DECODE(c)				_IOC_DIR((c))
#define	OS_IOCTL_DIR_READ					_IOC_READ
#define	OS_IOCTL_DIR_WRITE					_IOC_WRITE

#define	OS_IOCTL_TYPE_DECODE(c)				_IOC_TYPE((c))
#define	OS_IOCTL_INDEX_DECODE(c)			_IOC_NR((c))
#define	OS_IOCTL_SIZE_DECODE(c)				_IOC_SIZE((c))

#define	GSC_EXPORT
#define	GSC_IOCTL							's'



// data types *****************************************************************

#ifdef __KERNEL__

#include <asm/types.h>
#include <linux/ioctl.h>
#include <linux/types.h>

#else

#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/timeb.h>

typedef __s8	s8;
typedef __u8	u8;

typedef __s16	s16;
typedef __u16	u16;

typedef __s32	s32;
typedef __u32	u32;

typedef __s64	s64;
typedef __u64	u64;

#endif



#endif
