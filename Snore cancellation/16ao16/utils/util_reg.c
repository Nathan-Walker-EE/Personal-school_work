// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/util_reg.c $
// $Rev: 42922 $
// $Date: 2018-05-31 12:59:53 -0500 (Thu, 31 May 2018) $

// 16AO16: Utilities: source file

#include "main.h"



// #defines *******************************************************************

#define _GSC_REG(a)							"GSC " #a, AO16_GSC_##a, 0, 0
#define SIZEOF_ARRAY(a)						(sizeof((a)) / sizeof((a)[0]))
#define WIDTH								19



//*****************************************************************************
static int _bcr_decode(int fd, int supported, u32 value, int width)
{
	static const char*	irq[]	=
	{
		"Initialize Done",
		"Auto-Calibration Done",
		"Output Buffer Empty",
		"Output Buffer < 1/4 Full",
		"Output Buffer 3/4 Full",
		"Burst Trigger Ready",
		"Load Ready (low-to-high transition)",
		"End Load Ready (high-to-low transition)"
	};

	static const char*	range[]	=
	{
		"+-1.25 Volts",
		"+-2.5 Volts",
		"+-5 Volts",
		"+-10 Volts"
	};

	static const char*	disable[]	= { "Disable",			"Enable"		};
	static const char*	enable[]	= { "Enable",			"Disable"		};
	static const char*	idle[]		= { "Idle",				"Active"		};
	static const char*	no[]		= { "No",				"Yes"			};
	static const char*	pass[]		= { "Pass",				"Fail"			};
	static const char*	seq[]		= { "Sequential",		"Simultaneous"	};
	static const char*	ttl[]		= { "TTL",				"LVDS"			};
	static const char*	twos[]		= { "Twos Compliment",	"Offset Binary"	};

	width	+= 10;

	gsc_reg_field_show(width, WIDTH, value, 31, 18, 1, NULL,	"Reserved"			);
	gsc_reg_field_show(width, WIDTH, value, 17, 16, 1, range,	"Output Range"		);
	gsc_reg_field_show(width, WIDTH, value, 15, 15, 1, idle,	"Initialize"		);
	gsc_reg_field_show(width, WIDTH, value, 14, 14, 1, pass,	"Auto-Cal Status"	);
	gsc_reg_field_show(width, WIDTH, value, 13, 13, 1, idle,	"Auto-Cal"			);
	gsc_reg_field_show(width, WIDTH, value, 12, 12, 1, NULL,	"Reserved"			);
	gsc_reg_field_show(width, WIDTH, value, 11, 11, 1, idle,	"IRQ Request Flag"	);
	gsc_reg_field_show(width, WIDTH, value, 10,  8, 1, irq,		"IRQ"				);
	gsc_reg_field_show(width, WIDTH, value,  7,  7, 1, seq,		"Simul. Output"		);
	gsc_reg_field_show(width, WIDTH, value,  6,  6, 1, enable,	"Ext Burst Trig"	);
	gsc_reg_field_show(width, WIDTH, value,  5,  5, 1, ttl,		"Diff. Sync I/O"	);
	gsc_reg_field_show(width, WIDTH, value,  4,  4, 1, twos,	"Data Format"		);
	gsc_reg_field_show(width, WIDTH, value,  3,  3, 1, disable,	"Ground Sense"		);
	gsc_reg_field_show(width, WIDTH, value,  2,  2, 1, idle,	"Burst Trigger"		);
	gsc_reg_field_show(width, WIDTH, value,  1,  1, 1, no,		"Burst Ready"		);
	gsc_reg_field_show(width, WIDTH, value,  0,  0, 1, disable,	"Burst Enabled"		);

	return(0);
}



//*****************************************************************************
static int _csr_decode(int fd, int supported, u32 value, int width)
{
	static const char*	disable[]	= { "Disable",	"Enable"	};

	width	+= 10;

	gsc_reg_field_show(width, WIDTH, value, 31, 16, 1, NULL,	"Reserved"		);
	gsc_reg_field_show(width, WIDTH, value, 15, 15, 1, disable,	"Channel 15"	);
	gsc_reg_field_show(width, WIDTH, value, 14, 14, 1, disable,	"Channel 14"	);
	gsc_reg_field_show(width, WIDTH, value, 13, 13, 1, disable,	"Channel 13"	);
	gsc_reg_field_show(width, WIDTH, value, 12, 12, 1, disable,	"Channel 12"	);
	gsc_reg_field_show(width, WIDTH, value, 11, 11, 1, disable,	"Channel 11"	);
	gsc_reg_field_show(width, WIDTH, value, 10, 10, 1, disable,	"Channel 10"	);
	gsc_reg_field_show(width, WIDTH, value,  9,  9, 1, disable,	"Channel 9"		);
	gsc_reg_field_show(width, WIDTH, value,  8,  8, 1, disable,	"Channel 8"		);
	gsc_reg_field_show(width, WIDTH, value,  7,  7, 1, disable,	"Channel 7"		);
	gsc_reg_field_show(width, WIDTH, value,  6,  6, 1, disable,	"Channel 6"		);
	gsc_reg_field_show(width, WIDTH, value,  5,  5, 1, disable,	"Channel 5"		);
	gsc_reg_field_show(width, WIDTH, value,  4,  4, 1, disable,	"Channel 4"		);
	gsc_reg_field_show(width, WIDTH, value,  3,  3, 1, disable,	"Channel 3"		);
	gsc_reg_field_show(width, WIDTH, value,  2,  2, 1, disable,	"Channel 2"		);
	gsc_reg_field_show(width, WIDTH, value,  1,  1, 1, disable,	"Channel 1"		);
	gsc_reg_field_show(width, WIDTH, value,  0,  0, 1, disable,	"Channel 0"		);

	return(0);
}



//*****************************************************************************
static int _srr_decode(int fd, int supported, u32 value, int width)
{
	long	v;

	width	+= 10;

	gsc_reg_field_show(width, WIDTH, value, 31, 18, 1, NULL,	"Reserved"	);
	gsc_reg_field_show(width, WIDTH, value, 17,  0, 0, NULL,	"Nrate"		);
	v	= value & 0x3FFFF;
	printf("Fref / %ld\n", v);

	return(0);
}



//*****************************************************************************
static int _bor_decode(int fd, int supported, u32 value, int width)
{
	static const char*	size[]	=
	{
		"8 samples",
		"16 samples",
		"32 samples",
		"64 samples",
		"128 samples",
		"256 samples",
		"512 samples",
		"1024 samples",
		"2048 samples",
		"4096 samples",
		"8192 samples",
		"16384 samples",
		"32768 samples",
		"65536 samples",
		"131072 samples",
		"262144 samples"
	};

	static const char*	clock[]		= { "Rate Gen",	"Extern/SW"	};
	static const char*	disable[]	= { "Disable",	"Enable"	};
	static const char*	idle[]		= { "Idle",		"Active"	};
	static const char*	no[]		= { "No",		"Yes"		};

	width	+= 10;

	gsc_reg_field_show(width, WIDTH, value, 31, 18, 1, NULL,	"Reserved"			);
	gsc_reg_field_show(width, WIDTH, value, 17, 17, 1, no,		"Frame Overflow"	);
	gsc_reg_field_show(width, WIDTH, value, 16, 16, 1, no,		"Buffer Overflow"	);
	gsc_reg_field_show(width, WIDTH, value, 15, 15, 1, no,		"Buffer Full"		);
	gsc_reg_field_show(width, WIDTH, value, 14, 14, 1, no,		"Buffer 3/4 Full"	);
	gsc_reg_field_show(width, WIDTH, value, 13, 13, 1, no,		"Buffer < 1/4 Full"	);
	gsc_reg_field_show(width, WIDTH, value, 12, 12, 1, no,		"Buffer Empty"		);
	gsc_reg_field_show(width, WIDTH, value, 11, 11, 1, idle,	"Clear Buffer"		);
	gsc_reg_field_show(width, WIDTH, value, 10, 10, 1, no,		"Load Ready"		);
	gsc_reg_field_show(width, WIDTH, value,  9,  9, 1, idle,	"Load Request"		);
	gsc_reg_field_show(width, WIDTH, value,  8,  8, 1, no,		"Circ. Buffer"		);
	gsc_reg_field_show(width, WIDTH, value,  7,  7, 1, idle,	"SW Clock"			);
	gsc_reg_field_show(width, WIDTH, value,  6,  6, 1, no,		"Clock Ready"		);
	gsc_reg_field_show(width, WIDTH, value,  5,  5, 1, disable,	"Enable Clock"		);
	gsc_reg_field_show(width, WIDTH, value,  4,  4, 1, clock,	"External. Clock"	);
	gsc_reg_field_show(width, WIDTH, value,  3,  0, 1, size,	"Buffer Size"		);

	return(0);
}



//*****************************************************************************
static int _for_decode(int fd, int supported, u32 value, int width)
{
	static const char*	chans[]	=
	{
		"RESERVED",
		"8 Channels",
		"12 Channels",
		"16 Channels"
	};

	static const char*	filter[]	=
	{
		"None (>300 KHz)",
		"10 KHz",
		"100 KHz",
		"RESERVED"
	};

	static const char*	master[]	= { "45MHz",		"49.152MHz"		};
	static const char*	no[]		= { "No",			"Yes"			};
	static const char*	outputs[]	= { "Single Ended",	"Differential"	};
	static const char*	product[]	= { "16AO16",		"16AO16FLV"		};

	long	fw	= value & 0xFFF;

	width	+= 10;

	gsc_reg_field_show(width, WIDTH, value, 31, 24, 1, NULL,	"Reserved"			);
	gsc_reg_field_show(width, WIDTH, value, 23, 23, 1, no,		"High-V Outputs"	);

	if (fw >= 0x5)
		gsc_reg_field_show(width, WIDTH, value, 22, 22, 1, product,	"Model"			);
	else
		gsc_reg_field_show(width, WIDTH, value, 22, 22, 1, NULL,	"Reserved"		);

	gsc_reg_field_show(width, WIDTH, value, 21, 21, 1, master,	"Master Oscillator"	);
	gsc_reg_field_show(width, WIDTH, value, 20, 20, 1, outputs,	"Output Mode"		);
	gsc_reg_field_show(width, WIDTH, value, 19, 18, 1, filter,	"Output Filter"		);
	gsc_reg_field_show(width, WIDTH, value, 17, 16, 1, chans,	"Output Channels"	);
	gsc_reg_field_show(width, WIDTH, value, 15, 12, 1, NULL,	"Reserved"			);
	gsc_reg_field_show(width, WIDTH, value, 11,  0, 0, NULL,	"Firmware Revision"	);
	printf("%03lX\n", fw);

	return(0);
}



//*****************************************************************************
static int _acr_decode(int fd, int supported, u32 value, int width)
{
	static const char*	source[]	= { "Primary",	"Alternate"	};

	long	v;

	width	+= 10;

	gsc_reg_field_show(width, WIDTH, value, 31, 10, 1, NULL,	"Reserved"		);
	gsc_reg_field_show(width, WIDTH, value,  9,  9, 1, source,	"Ref. Source"	);
	gsc_reg_field_show(width, WIDTH, value,  8,  0, 0, NULL,	"Nclk"			);
	v	= value & 0x1FF;
	printf("Fref-alt = 16,000,000 * (1 + %ld / 511)\n", v);

	return(0);
}



// variables *******************************************************************

static gsc_reg_def_t	_gsc[]	=
{
	{ _GSC_REG(BCR),		0,	_bcr_decode,	"Board Control Register"		},
	{ _GSC_REG(CSR),		0,	_csr_decode,	"Channel Selection Register"	},
	{ _GSC_REG(SRR),		0,	_srr_decode,	"Sample Rate Register"			},
	{ _GSC_REG(BOR),		0,	_bor_decode,	"Buffer Operations Register"	},
	{ _GSC_REG(FOR),		0,	_for_decode,	"Firmware Options Register"		},
	{ _GSC_REG(AVR),		0,	NULL,			"Auto Cal Values Register"		},
	{ _GSC_REG(ODBR),		0,	NULL,			"Output Data Buffer Register"	},
	{ _GSC_REG(ACR),		0,	_acr_decode,	"Adjustable Clock Register"		},

	{ NULL, 0, 0, 0, 0,		NULL	}
};



//*****************************************************************************
static const gsc_reg_def_t* _find_reg(u32 reg, const gsc_reg_def_t* list)
{
	const gsc_reg_def_t*	def	= NULL;
	int						i;

	for (i = 0; list[i].name; i++)
	{
		if (reg == list[i].reg)
		{
			def	= &list[i];
			break;
		}
	}

	return(def);
}



/*****************************************************************************
*
*	Function:	ao16_reg_get_def_id
*
*	Purpose:
*
*		Retrieve the register definition structure given the register id.
*
*	Arguments:
*
*		reg		The id of the register to access.
*
*	Returned:
*
*		NULL	The register id wasn't found.
*		else	A pointer to the register definition.
*
*****************************************************************************/

const gsc_reg_def_t* ao16_reg_get_def_id(u32 reg)
{
	const gsc_reg_def_t*	def;

	def	= _find_reg(reg, _gsc);
	return(def);
}



/*****************************************************************************
*
*	Function:	ao16_reg_get_def_index
*
*	Purpose:
*
*		Retrieve the register definition structure based on an index.
*
*	Arguments:
*
*		index	The index of the register to access.
*
*	Returned:
*
*		NULL	The index doesn't correspond to a known register.
*		else	A pointer to the register definition.
*
*****************************************************************************/

const gsc_reg_def_t* ao16_reg_get_def_index(int index)
{
	const gsc_reg_def_t*	def;

	if (index < 0)
		def	= NULL;
	else if (index >= (SIZEOF_ARRAY(_gsc) - 1))
		def	= NULL;
	else
		def	= &_gsc[index];

	return(def);
}



/*****************************************************************************
*
*	Function:	ao16_reg_get_desc
*
*	Purpose:
*
*		Retrieve the description of the specified register.
*
*	Arguments:
*
*		reg		The register whose description is desired.
*
*	Returned:
*
*		!NULL	The register's name.
*
*****************************************************************************/

const char* ao16_reg_get_desc(u32 reg)
{
	const gsc_reg_def_t*	def;
	const char*				desc;

	def	= _find_reg(reg, _gsc);

	if (def)
		desc	= def->desc;
	else
		desc	= "UNKNOWN";

	return(desc);
}



/*****************************************************************************
*
*	Function:	ao16_reg_get_name
*
*	Purpose:
*
*		Retrieve the name of the specified register.
*
*	Arguments:
*
*		reg		The register whose name is desired.
*
*	Returned:
*
*		!NULL	The register's name.
*
*****************************************************************************/

const char* ao16_reg_get_name(u32 reg)
{
	const gsc_reg_def_t*	def;
	const char*				name;

	def	= _find_reg(reg, _gsc);

	if (def)
		name	= def->name;
	else
		name	= "UNKNOWN";

	return(name);
}



/*****************************************************************************
*
*	Function:	ao16_reg_list
*
*	Purpose:
*
*		List the GSC registers and their values.
*
*	Arguments:
*
*		fd		The handle to access the device.
*
*		detail	List the register details?
*
*	Returned:
*
*		>= 0	The number of errors encountered here.
*
*****************************************************************************/

int ao16_reg_list(int fd, int detail)
{
	int	errs;

	errs	= gsc_reg_list(fd, _gsc, detail, ao16_reg_read);
	return(errs);
}



