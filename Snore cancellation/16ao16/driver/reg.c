// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/reg.c $
// $Rev: 42902 $
// $Date: 2018-05-30 13:25:37 -0500 (Wed, 30 May 2018) $

// 16AO16: Device Driver: source file

#include "main.h"



//*****************************************************************************
int dev_reg_mod_alt(dev_data_t* dev, gsc_reg_t* arg)
{
	// No alternate register encodings are defined for this driver.
	return(-EINVAL);
}



//*****************************************************************************
int dev_reg_read_alt(dev_data_t* dev, gsc_reg_t* arg)
{
	// No alternate register encodings are defined for this driver.
	return(-EINVAL);
}



//*****************************************************************************
int dev_reg_write_alt(dev_data_t* dev, gsc_reg_t* arg)
{
	// No alternate register encodings are defined for this driver.
	return(-EINVAL);
}


