// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/linux/os_util_kbd.c $
// $Rev: 42867 $
// $Date: 2018-05-18 12:43:42 -0500 (Fri, 18 May 2018) $

// Linux: Utility: source file

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "gsc_utils.h"



//lint -e459

// variables ******************************************************************

static	struct termios	initial_settings;
static	int				opened		= 0;
static	struct termios	new_settings;
static	int				peek_ch		= -1;
static	int				registered	= 0;



//*****************************************************************************
void os_kbd_close(void)
{
	if (opened)
	{
		opened	= 0;
		tcsetattr(0, TCSANOW, &initial_settings);
	}
}



//*****************************************************************************
void os_kbd_open(void)
{
	if (opened == 0)
	{
		opened	= 1;
		peek_ch	= -1;
		tcgetattr(0,&initial_settings);
		new_settings				= initial_settings;
		new_settings.c_lflag		&= ~ICANON;
		new_settings.c_lflag		&= ~ECHO;
		new_settings.c_lflag		&= ~ISIG;
		new_settings.c_cc[VMIN]		= 1;
		new_settings.c_cc[VTIME]	= 0;
		tcsetattr(0, TCSANOW, &new_settings);

		if (registered == 0)
		{
			registered	= 1;
			atexit(os_kbd_close);
		}
	}
}



//*****************************************************************************
int os_kbd_hit(void)
{
	unsigned char	ch;
	int				hit;
	int				nread;

	os_kbd_open();

	if (peek_ch != -1)
	{
		hit	= 1;
	}
	else
	{
		new_settings.c_cc[VMIN]	= 0;
		tcsetattr(0, TCSANOW, &new_settings);
		nread					= read(0,&ch,1);
		new_settings.c_cc[VMIN]	= 1;
		tcsetattr(0, TCSANOW, &new_settings);

		if(nread == 1)
		{
			peek_ch	= ch;
			hit		= 1;
		}
		else
		{
			hit	= 0;
		}
	}

	return(hit);
}



//*****************************************************************************
int os_kbd_read(void)
{
	char	ch;

	os_kbd_open();

	if(peek_ch != -1)
	{
		ch		= (char) peek_ch;
		peek_ch	= -1;
	}
	else
	{
		read(0, &ch, 1);
	}

	return(ch);
}


