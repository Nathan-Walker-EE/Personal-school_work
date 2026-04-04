// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/linux/os_util_sem.c $
// $Rev: 42867 $
// $Date: 2018-05-18 12:43:42 -0500 (Fri, 18 May 2018) $

// Linux: Utility: source file

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "os_utils.h"



//*****************************************************************************
int os_sem_create(os_sem_t* sem)
{
	int	ret;

	ret	= os_sem_create_qty(sem, 1, 1);
	return(ret);
}



//*****************************************************************************
int os_sem_create_qty(os_sem_t* sem, int cap, int put)
{
	int	ret;

	if ((sem) && (cap > 0) && (put >= 0) && (put <= cap))
	{
		memset(sem, 0, sizeof(os_sem_t));
		ret	= sem_init(&sem->sem, 0, put);

		if (ret)
			ret	= -errno;
		else
			sem->key	= (void*) sem;
	}
	else
	{
		ret	= -EINVAL;
	}

	return(ret);
}



//*****************************************************************************
int os_sem_destroy(os_sem_t* sem)
{
	int	ret	= 0;

	if (sem)
	{
		if (sem->key == sem)
		{
			os_sem_unlock(sem);
			ret	= sem_destroy(&sem->sem);

			if (ret)
				ret	= -errno;
		}

		memset(sem, 0, sizeof(os_sem_t));
	}
	else
	{
		ret	= -EINVAL;
	}

	return(ret);
}



//*****************************************************************************
int os_sem_lock(os_sem_t* sem)
{
	int	ret;

	if ((sem) && (sem->key == sem))
	{
		ret	= sem_wait(&sem->sem);

		if (ret)
			ret	= -errno;
	}
	else
	{
		ret	= -EINVAL;
	}

	return(ret);
}



//*****************************************************************************
int os_sem_unlock(os_sem_t* sem)
{
	int	ret;

	if ((sem) && (sem->key == sem))
	{
		ret	= sem_post(&sem->sem);

		if (ret)
			ret	= -errno;
	}
	else
	{
		ret	= -EINVAL;
	}

	return(ret);
}


