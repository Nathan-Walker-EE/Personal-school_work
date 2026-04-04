// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/driver/linux/os_open.c $
// $Rev: 43585 $
// $Date: 2018-10-12 13:40:10 -0500 (Fri, 12 Oct 2018) $

// Linux: Device Driver: source file: This software is covered by the GNU GENERAL PUBLIC LICENSE (GPL).

#include "main.h"



//*****************************************************************************
static GSC_ALT_STRUCT_T* _alt_data_t_locate(struct inode* inode)
{
	GSC_ALT_STRUCT_T*	alt		= NULL;
	unsigned int		index	= MINOR(inode->i_rdev);

#if (GSC_DEVS_PER_BOARD > 1)

	unsigned int		board	= index / GSC_DEVS_PER_BOARD;
	unsigned int		channel	= index % GSC_DEVS_PER_BOARD;

	if (board >= SIZEOF_ARRAY(gsc_global.dev_list))
		alt	= NULL;
	else
		alt	= &gsc_global.dev_list[board]->channel[channel];

#else

	if (index >= SIZEOF_ARRAY(gsc_global.dev_list))
		alt	= NULL;
	else
		alt	= gsc_global.dev_list[index];

#endif

	return(alt);
}



//*****************************************************************************
int os_open(struct inode *inode, struct file *fp)
{
	#define	_RW_	(FMODE_READ | FMODE_WRITE)

	GSC_ALT_STRUCT_T*	alt;
	int					ret;
	int					share;

	for (;;)	// We'll use a loop for convenience.
	{
		if ((fp->f_mode & _RW_) != _RW_)
		{
			// Read and write access are both required.
			ret	= -EINVAL;
			break;
		}

		alt	= _alt_data_t_locate(inode);

		if (alt == NULL)
		{
			// The referenced device doesn't exist.
			ret	= -ENODEV;
			break;
		}

		share	= (fp->f_flags & O_APPEND) ? 1 : 0;
		ret		= gsc_open(alt, share);

		if (ret == 0)
		{
			ret	= os_module_count_inc();

			if (ret == 0)
			{
				fp->private_data	= alt;
			}
			else
			{
				// We coundn't increase the usage count.
				gsc_close(alt);
			}
		}

		break;
	}

	return(ret);
}



