// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_count.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



/******************************************************************************
*
*	Function:	ao16_count_boards
*
*	Purpose:
*
*		Count the number of installed boards.
*
*	Arguments:
*
*		verbose	Work verbosely?
*
*		get		The count is returned here.
*
*	Returned:
*
*		> 0		The number of boards found.
*		== 0	No boards were found.
*
******************************************************************************/

int ao16_count_boards(int verbose, s32* get)
{
	int	errs;

	errs	= os_count_boards(verbose, get, ao16_open, ao16_read, ao16_close);
	return(errs);
}



