// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/linux/os_utils.h $
// $Rev: 43567 $
// $Date: 2018-10-12 13:23:25 -0500 (Fri, 12 Oct 2018) $

// aPPLICATION: Linux: Utility: header file

#ifndef	__OS_UTILS_H__
#define	__OS_UTILS_H__

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "os_common.h"



// data types *****************************************************************

typedef struct
{
	pthread_t	thread;
	char		name[64];
} os_thread_t;

typedef	struct
{
	void*		key;	// struct is valid only if key == & of struct
	sem_t		sem;
} os_sem_t;



// prototypes *****************************************************************

int		os_count_boards(	int		verbose,
							int*	qty,
							int		(*dev_open)(int device, int share, int* fd),
							int		(*dev_read)(int fd, void* dst, size_t size),
							int		(*dev_close)(int fd));

int		os_id_driver(	int	(*dev_open)(int index, int share, int* fd),
						int	(*dev_read)(int fd, void* dst, size_t size),
						int	(*dev_close)(int fd));
void	os_id_host(void);

void	os_kbd_close(void);
void	os_kbd_open(void);
int		os_kbd_hit(void);
int		os_kbd_read(void);

int		os_sem_create(os_sem_t* sem);
int		os_sem_create_qty(os_sem_t* sem, int cap, int put);
int		os_sem_destroy(os_sem_t* sem);
int		os_sem_lock(os_sem_t* sem);
int		os_sem_unlock(os_sem_t* sem);

void	os_sleep_ms(int ms);

int		os_thread_create(os_thread_t* thread, const char* name, int (*func)(void* arg), void* arg);
int		os_thread_destroy(os_thread_t* thread);
size_t	os_time_delta_ms(void);



#endif
