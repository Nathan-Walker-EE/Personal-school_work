// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/regs/menu.c $
// $Rev: 45047 $
// $Date: 2019-06-04 16:36:14 -0500 (Tue, 04 Jun 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



/*****************************************************************************
*
*	Function:	_get_char
*
*	Purpose:
*
*		Read a character from the input.
*
*	Arguments:
*
*		None.
*
*	Returned:
*
*		int		The character read.
*
*****************************************************************************/

static int _get_char(void)
{
	char	buf[1024];
	int		c;

	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), stdin);
	c	= buf[0];
	return(c);
}



/*****************************************************************************
*
*	Function:	_make_selection
*
*	Purpose:
*
*		Ask for and wait for a user menu selection.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*		qty		This is the number of menu items available.
*
*	Returned:
*
*		>= 0	This is the index of the menu selection made.
*
*****************************************************************************/

static int _make_selection(const menu_t* menu, int qty)
{
	int	c;
	int	i;

	for (;;)
	{
		printf("    Make a menu selection from 'A' to '%c': ", 'A' + qty - 1);
		c	= _get_char();

		if ((c >= 'A') && (c <= ('A' + qty - 1)))
		{
			i	= c - 'A';
			break;
		}

		if ((c >= 'a') && (c <= ('a' + qty - 1)))
		{
			i	= c - 'a';
			break;
		}
	}

	return(i);
}



/*****************************************************************************
*
*	Function:	_menu_show
*
*	Purpose:
*
*		Show the menu content. After the last menu item we add a "Done" option.
*		Since the menu selections are alphabetical, the menu code is designed
*		for 25 or fewer menu options.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*		done	Include a DONE menu option.
*
*	Returned:
*
*		>= 1	The number of menu items presented.
*
*****************************************************************************/

static int _menu_show(const menu_t* menu, int done)
{
	int	i;

	for (i = 0; i <= 25; i++)
	{
		if (menu->list[i].name == NULL)
			break;

		printf("    %c. %s\n", 'A' + i, menu->list[i].name);
	}

	if (done)
	{
		printf("    %c. %s\n", 'A' + i, "Done");
		i++;
	}

	printf("\n");
	return(i);
}



/*****************************************************************************
*
*	Function:	menu_call
*
*	Purpose:
*
*		Present the menu, then call the function for the selected item..
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*		menu	The structure for the menu to display.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

void menu_call(int fd, const menu_t* menu)
{
	int			i;
	const char*	psz;
	int			qty;
	struct tm*	stm;
	time_t		tt;

	for (;;)
	{
		time(&tt);
		stm	= localtime(&tt);
		psz	= asctime(stm);
		printf("\n");
		printf("  %s          %s", menu->title, psz);
		qty	= _menu_show(menu, 1);
		i	= _make_selection(menu, qty);

		if (i >= (qty - 1))
		{
			printf("    -> %c. %s\n", 'A' + i, "Done");
			break;
		}

		printf("    -> %c. %s\n", 'A' + i, menu->list[i].name);
		printf("\n");
		(menu->list[i].func)(fd);
		printf("\n");
		printf("  Press return to continue: ");
		_get_char();
	}
}



/*****************************************************************************
*
*	Function:	menu_select
*
*	Purpose:
*
*		Present the menu an tell the caller which selection was made.
*
*	Arguments:
*
*		menu	The structure for the menu to display.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

int menu_select(const menu_t* menu)
{
	int			i;
	const char*	psz;
	int			qty;
	struct tm*	stm;
	time_t		tt;

	time(&tt);
	stm	= localtime(&tt);
	psz	= asctime(stm);
	printf("\n");
	printf("  %s          %s", menu->title, psz);
	qty	= _menu_show(menu, 0);
	i	= _make_selection(menu, qty);
	printf("    -> %c. %s\n", 'A' + i, menu->list[i].name);
	printf("\n");

	return(i);
}



