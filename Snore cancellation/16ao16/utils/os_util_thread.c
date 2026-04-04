// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/linux/os_util_thread.c $
// $Rev: 42867 $
// $Date: 2018-05-18 12:43:42 -0500 (Fri, 18 May 2018) $

// Linux: Utility: source file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gsc_utils.h"



// data types *****************************************************************

typedef struct
{
	int		(*function)(void* arg);
	void*	arg;
	int		running;
	int		free;
} _thread_data_t;



/******************************************************************************
*
*	Function:	_thread_func
*
*	Purpose:
*
*		This is an OS specific thread entry function, whose purpose is to call
*		the application's OS independent thread entry function.
*
*	Arguments:
*
*		pvArg	The thread's entry arguemtn.
*
*	Returned:
*
*		The thread exit code, which we ignore.
*
******************************************************************************/

static void* _thread_func(void* arg)
{
	_thread_data_t*	data	= (_thread_data_t*) arg;
	int				ret;

	data->running	= 1;
	ret				= (data->function)(data->arg);

	for (;;)
	{
		if (data->free)
			break;

		usleep(10000);
	}

	free(arg);
	pthread_exit((void*) (unsigned long) (long) ret);
	return((void*) (unsigned long) (long) ret);
}



/******************************************************************************
*
*	Function:	os_thread_create
*
*	Purpose:
*
*		Create a thread whose effective entry point and argument are as given.
*
*	Arguments:
*
*		thread	The OS specific thread data is stored here.
*
*		name	A name to associate with the thread.
*
*		func	The effective entry point.
*
*		arg		The entry argument.
*
*	Returned:
*
*		>= 0	The number of errors encountered.
*
******************************************************************************/

int os_thread_create(	os_thread_t*	thread,
						const char*		name,
						int				(*func)(void* arg),
						void*			arg)
{
	_thread_data_t*	data;
	int				errs	= 0;
	int				i;

	for (;;)	// A convenience loop.
	{
		if ((thread == NULL) || (name == NULL) || (func == NULL))
		{
			printf("FAIL <---  (invalid argument)\n");
			errs++;
			break;
		}

		memset(thread, 0, sizeof(os_thread_t));
		data	= malloc(sizeof(_thread_data_t));

		if (data == NULL)
		{
			printf("FAIL <---  (malloc failed)\n");
			errs++;
			break;
		}

		data->function	= func;
		data->arg		= arg;
		data->running	= 0;
		data->free		= 0;
		i				= pthread_create(	&thread->thread,
											NULL,
											_thread_func,
											data);

		if (i)
		{
			printf("FAIL <---  (thread creation failed)\n");
			free(data);
			errs++;
			break;
		}

		// Wait for upto one second for the thread to become running.

		for (i = 0; i < 100; i++)
		{
			if (data->running)
				break;

			usleep(1000);
		}

		data->free	= 1;
		strncpy(thread->name, name, sizeof(thread->name));
		thread->name[sizeof(thread->name) - 1]	= 0;
		break;
	}

	//lint -save -e593
    return(errs);
	//lint -restore
}



/******************************************************************************
*
*	Function:	os_thread_destroy
*
*	Purpose:
*
*		End a thread in a well behaved manner.
*
*	Arguments:
*
*		thread	The thread to end.
*
*	Returned:
*
*		>= 0	The number of errors encountered.
*
******************************************************************************/

int os_thread_destroy(os_thread_t* thread)
{
	int	errs	= 0;

	if (thread)
	{
		if (thread->thread)
			pthread_join(thread->thread, NULL);

		memset(thread, 0, sizeof(os_thread_t));
	}
	else
	{
		printf("FAIL <---  (invalid argument)\n");
		errs++;
	}

	return(errs);
}


