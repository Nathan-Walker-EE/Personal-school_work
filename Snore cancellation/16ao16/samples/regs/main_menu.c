// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/regs/main_menu.c $
// $Rev: 44919 $
// $Date: 2019-05-16 20:55:11 -0500 (Thu, 16 May 2019) $

// 16AO16: Sample Application: source file

#include "main.h"



/*****************************************************************************
*
*	Function:	_dump_everything
*
*	Purpose:
*
*		Dump all available data to the screen.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

static void _dump_everything(int fd)
{
	os_id_host();
	printf("\n");
	os_id_driver(ao16_open, ao16_read, ao16_close);
	printf("\n");
	ao16_id_device(fd, -1, 1);
	printf("\n");
	gsc_reg_plx9056_list_pci(fd, ao16_reg_read);
	printf("\n");
	gsc_reg_plx9056_list_plx(fd, ao16_reg_read);
	printf("\n");
	ao16_reg_list(fd, 1);
}



/*****************************************************************************
*
*	Function:	_dump_gsc
*
*	Purpose:
*
*		Dump the GSC registers.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

static void _dump_gsc(int fd)
{
	ao16_reg_list(fd, 0);
}



/*****************************************************************************
*
*	Function:	_dump_gsc_detail
*
*	Purpose:
*
*		Dump the GSC register details.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

static void _dump_gsc_detail(int fd)
{
	ao16_reg_list(fd, 1);
}



/*****************************************************************************
*
*	Function:	_dump_pci
*
*	Purpose:
*
*		Dump the PCI registers.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

static void _dump_pci(int fd)
{
	gsc_reg_plx9056_list_pci(fd, ao16_reg_read);
}



/*****************************************************************************
*
*	Function:	_dump_plx
*
*	Purpose:
*
*		Dump the PLX registers.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

static void _dump_plx(int fd)
{
	gsc_reg_plx9056_list_plx(fd, ao16_reg_read);
}



/*****************************************************************************
*
*	Function:	_id_board
*
*	Purpose:
*
*		Identify the device being accessed.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

static void _id_board(int fd)
{
	ao16_id_device(fd, -1, 1);
}



/*****************************************************************************
*
*	Function:	_id_driver
*
*	Purpose:
*
*		Identify the device driver.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

static void _id_driver(int fd)
{
	os_id_driver(ao16_open, ao16_read, ao16_close);
}



/*****************************************************************************
*
*	Function:	_id_host
*
*	Purpose:
*
*		Identify the host OS.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

static void _id_host(int fd)
{
	os_id_host();
}



/*****************************************************************************
*
*	Function:	main_menu
*
*	Purpose:
*
*		Present the main menu.
*
*	Arguments:
*
*		fd		The handle for the board to access.
*
*	Returned:
*
*		None.
*
*****************************************************************************/

void main_menu(int fd)
{
	static const menu_item_t	list[]	=
	{
		// name								func
		{ "Dump Everything",				_dump_everything	},
		{ "Host OS Identification",			_id_host			},
		{ "Driver Identification",			_id_driver			},
		{ "Board Identification",			_id_board			},
		{ "PCI Register Dump",				_dump_pci			},
		{ "PLX Register Dump",				_dump_plx			},
		{ "GSC Register Dump",				_dump_gsc			},
		{ "GSC Register Detail Dump",		_dump_gsc_detail	},
		{ "Edit GSC Register By Name",		reg_mod_by_name		},
		{ "Edit GSC Register By Offset",	reg_mod_by_offset	},
		{ NULL,								NULL				}
	};

	static const menu_t	menu	=
	{
		/* title	*/	"Main Menu",
		/* list		*/	list
	};

	printf("\n\n");
	printf("Register Access Application\n");
	menu_call(fd, &menu);
}



