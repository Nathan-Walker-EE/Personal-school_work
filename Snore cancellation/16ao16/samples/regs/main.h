// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/regs/main.h $
// $Rev: 44919 $
// $Date: 2019-05-16 20:55:11 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: header file

#ifndef	__MAIN_H__
#define	__MAIN_H__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "16ao16_main.h"



// #defines *******************************************************************

#define	SIZEOF_ARRAY(a)				(sizeof((a)) / sizeof((a)[0]))



// data types *****************************************************************

typedef struct
{
	const char*	name;				// NULL terminates list.
	void		(*func)(int fd);	// NULL terminates list.
} menu_item_t;

typedef struct
{
	const char*			title;
	const menu_item_t*	list;
} menu_t;



// prototypes *****************************************************************

void	main_menu(int fd);
void	menu_call(int fd, const menu_t* menu);
int		menu_select(const menu_t* menu);
void	reg_mod_by_name(int fd);
void	reg_mod_by_offset(int fd);



#endif
