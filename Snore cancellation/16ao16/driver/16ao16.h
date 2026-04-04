// $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/16ao16.h $
// $Rev: 44907 $
// $Date: 2019-05-16 20:31:04 -0500 (Thu, 16 May 2019) $

// 16AO16: Device Driver: header file

#ifndef	__16AO16_H__
#define	__16AO16_H__

#include "gsc_common.h"
#include "gsc_pci9056.h"



// #defines *******************************************************************

#define	AO16_BASE_NAME						"16ao16"

// IOCTL command codes
#define	AO16_IOCTL_REG_READ					OS_IOCTL_RW( 0, 12, gsc_reg_t)
#define	AO16_IOCTL_REG_WRITE				OS_IOCTL_W ( 1, 12, gsc_reg_t)
#define	AO16_IOCTL_REG_MOD					OS_IOCTL_W ( 2, 12, gsc_reg_t)
#define	AO16_IOCTL_QUERY					OS_IOCTL_RW( 3,  4, s32)
#define	AO16_IOCTL_INITIALIZE				OS_IOCTL   ( 4)
#define	AO16_IOCTL_AUTO_CALIBRATE			OS_IOCTL   ( 5)
#define	AO16_IOCTL_AUTO_CAL_STS				OS_IOCTL_R ( 6,  4, s32)
#define	AO16_IOCTL_TX_IO_MODE				OS_IOCTL_RW( 7,  4, s32)
#define	AO16_IOCTL_TX_IO_OVER_DATA			OS_IOCTL_RW( 8,  4, s32)
#define	AO16_IOCTL_TX_IO_OVER_FRAME			OS_IOCTL_RW( 9,  4, s32)
#define	AO16_IOCTL_TX_IO_TIMEOUT			OS_IOCTL_RW(10,  4, s32)
#define	AO16_IOCTL_BUFFER_CLEAR				OS_IOCTL   (11)
#define	AO16_IOCTL_BUFFER_MODE				OS_IOCTL_RW(12,  4, s32)
#define	AO16_IOCTL_BUFFER_OVER_DATA			OS_IOCTL_RW(13,  4, s32)
#define	AO16_IOCTL_BUFFER_OVER_FRAME		OS_IOCTL_RW(14,  4, s32)
#define	AO16_IOCTL_BUFFER_SIZE				OS_IOCTL_RW(15,  4, s32)
#define	AO16_IOCTL_BUFFER_STATUS			OS_IOCTL_R (16,  4, s32)
#define	AO16_IOCTL_BURST_ENABLE				OS_IOCTL_RW(17,  4, s32)
#define	AO16_IOCTL_BURST_READY				OS_IOCTL_R (18,  4, s32)
#define	AO16_IOCTL_BURST_TRIG_SRC			OS_IOCTL_RW(19,  4, s32)
#define	AO16_IOCTL_BURST_TRIGGER			OS_IOCTL   (20)
#define	AO16_IOCTL_CHANNEL_SEL				OS_IOCTL_RW(21,  4, s32)
#define	AO16_IOCTL_CLOCK_ENABLE				OS_IOCTL_RW(22,  4, s32)
#define	AO16_IOCTL_CLOCK_READY				OS_IOCTL_R (23,  4, s32)
#define	AO16_IOCTL_CLOCK_REF_SRC			OS_IOCTL_RW(24,  4, s32)
#define	AO16_IOCTL_CLOCK_SRC				OS_IOCTL_RW(25,  4, s32)
#define	AO16_IOCTL_CLOCK_SW					OS_IOCTL   (26)
#define	AO16_IOCTL_DATA_FORMAT				OS_IOCTL_RW(27,  4, s32)
#define	AO16_IOCTL_GROUND_SENSE				OS_IOCTL_RW(28,  4, s32)
#define	AO16_IOCTL_IRQ_SEL					OS_IOCTL_RW(29,  4, s32)
#define	AO16_IOCTL_LOAD_READY				OS_IOCTL_R (30,  4, s32)
#define	AO16_IOCTL_LOAD_REQUEST				OS_IOCTL   (31)
#define	AO16_IOCTL_NCLK						OS_IOCTL_RW(32,  4, s32)
#define	AO16_IOCTL_NRATE					OS_IOCTL_RW(33,  4, s32)
#define	AO16_IOCTL_OUTPUT_MODE				OS_IOCTL_RW(34,  4, s32)
#define	AO16_IOCTL_RANGE					OS_IOCTL_RW(35,  4, s32)
#define	AO16_IOCTL_XCVR_TYPE				OS_IOCTL_RW(36,  4, s32)
#define	AO16_IOCTL_TX_IO_ABORT				OS_IOCTL_R (37,  4, s32)
#define	AO16_IOCTL_WAIT_EVENT				OS_IOCTL_RW(38, 28, gsc_wait_t)
#define	AO16_IOCTL_WAIT_CANCEL				OS_IOCTL_RW(39, 28, gsc_wait_t)
#define	AO16_IOCTL_WAIT_STATUS				OS_IOCTL_RW(40, 28, gsc_wait_t)
#define	AO16_IOCTL_CBL_POL_TRIG_IN			OS_IOCTL_RW(41,  4, s32)
#define	AO16_IOCTL_CBL_POL_TRIG_OUT			OS_IOCTL_RW(42,  4, s32)
#define	AO16_IOCTL_CBL_POL_DAC_CLK_OUT		OS_IOCTL_RW(43,  4, s32)
#define	AO16_IOCTL_CBL_POL_CLOCK_IO			OS_IOCTL_RW(44,  4, s32)
#define	AO16_IOCTL_CBL_ISO_TRIG_OUT			OS_IOCTL_RW(45,  4, s32)
#define	AO16_IOCTL_CBL_ISO_DAC_CLK_OUT		OS_IOCTL_RW(46,  4, s32)
#define	AO16_IOCTL_CBL_ISO_CLOCK_IO			OS_IOCTL_RW(47,  4, s32)



//*****************************************************************************
// AO16_IOCTL_REG_READ
// AO16_IOCTL_REG_WRITE
// AO16_IOCTL_REG_MOD
//
#define	AO16_REG_ENCODE(s,o)				GSC_REG_ENCODE(GSC_REG_TYPE_BAR2,(s),(o))
// Parameter:	gsc_reg_t*
#define	AO16_GSC_BCR						AO16_REG_ENCODE(4,0x00)// Board Control Register
#define	AO16_GSC_CSR						AO16_REG_ENCODE(4,0x04)// Channel Selection Register
#define	AO16_GSC_SRR						AO16_REG_ENCODE(4,0x08)// Sample Rate Register
#define	AO16_GSC_BOR						AO16_REG_ENCODE(4,0x0C)// Buffer Operations Register
#define	AO16_GSC_FOR						AO16_REG_ENCODE(4,0x10)// Firmware Options Register
#define	AO16_GSC_AVR						AO16_REG_ENCODE(4,0x14)// Auto Cal Values Register
#define	AO16_GSC_ODBR						AO16_REG_ENCODE(4,0x18)// Output Data Buffer Register
#define	AO16_GSC_ACR						AO16_REG_ENCODE(4,0x1C)// Adjustable Clock Register

//*****************************************************************************
// AO16_IOCTL_QUERY
//
//	Parameter:	s32
//		Pass in a value from the list below.
//		The value returned is the answer to the query.

typedef enum
{
	AO16_QUERY_AUTO_CAL_MS,		// Max auto-cal period in ms.
	AO16_QUERY_CHANNEL_MASK,	// The channel selection mask.
	AO16_QUERY_CHANNEL_MAX,		// Maximum number of channels supported.
	AO16_QUERY_CHANNEL_QTY,		// The number of A/D channels.
	AO16_QUERY_COUNT,			// Number of query options.
	AO16_QUERY_DEVICE_TYPE,		// Value from gsc_dev_type_t
	AO16_QUERY_DIFFERENTIAL,	// Are the outputs differential?
	AO16_QUERY_FIFO_SIZE,		// FIFO depth in 32-bit samples
	AO16_QUERY_FILTER,			// See filter options below.
	AO16_QUERY_FREF_DEFAULT,	// The default Fref value for the board.
	AO16_QUERY_FSAMP_MAX,		// Rate Generator maximum output rate.
	AO16_QUERY_FSAMP_MIN,		// Rate Generator minimum output rate.
	AO16_QUERY_INIT_MS,			// Max initialize period in ms.
	AO16_QUERY_NCLK_MASK,		// Mask of valid Nclk bits.
	AO16_QUERY_NCLK_MAX,		// Maximum rate divider Nclk value.
	AO16_QUERY_NCLK_MIN,		// Maximum rate divider Nclk value.
	AO16_QUERY_NRATE_MASK,		// Mask of valid Nrate bits.
	AO16_QUERY_NRATE_MAX,		// Maximum rate generator Nrate value.
	AO16_QUERY_NRATE_MIN,		// Maximum rate generator Nrate value.
	AO16_QUERY_DMDMA,			// Is DMDMA supported?
	AO16_QUERY_VOLT_RANGE,		// Which voltage range is supported?
	AO16_QUERY_CABLE_INVERT_4,	// Can the 4 trig/clock signals be inverted?
	AO16_QUERY_CABLE_PASSIVE_3,	// Can the 3 trig/clock signals go passive?
	AO16_QUERY_LAST				// Must be very last entry.

} AO16_query_t;

#define	AO16_IOCTL_QUERY_ERROR	(-1)	// A query request is unknown of invalid.

// AO16_QUERY_FILTER options.
#define	AO16_FILTER_NONE					0
#define	AO16_FILTER_10KHZ					1
#define	AO16_FILTER_100KHZ					2

// AO16_QUERY_VOLT_RANGE options.
#define	AO16_VOLT_RANGE_LOW					0
#define	AO16_VOLT_RANGE_HIGH				1

//*****************************************************************************
// AO16_IOCTL_INITIALIZE					BCR D15
//
//	Parameter:	None

//*****************************************************************************
// AO16_IOCTL_AUTO_CALIBRATE				BCR D13
//
//	Parameter:	None

//*****************************************************************************
// AO16_IOCTL_AUTO_CAL_STS					BCR D14
//
//	Parameter:	s32*
//		One of the below values is returned.
#define	AO16_AUTO_CAL_STS_ACTIVE			0
#define	AO16_AUTO_CAL_STS_FAIL				1
#define	AO16_AUTO_CAL_STS_PASS				2

//*****************************************************************************
// AO16_IOCTL_TX_IO_MODE
//
//	Parameter:	s32
//		Pass in any of the gsc_io_mode_t options, or
//		-1 to read the current setting.
#define	AO16_IO_MODE_DEFAULT				GSC_IO_MODE_PIO
// GSC_IO_MODE_PIO
// GSC_IO_MODE_BMDMA
// GSC_IO_MODE_DMDMA, if supported by firmware

//*****************************************************************************
// AO16_IOCTL_TX_IO_OVER_DATA
//
//	Parameter:	s32
//		Pass in any valid value, or
//		-1 to read the current setting.
#define	AO16_TX_IO_OVER_DATA_IGNORE			0
#define	AO16_TX_IO_OVER_DATA_CHECK			1
#define	AO16_TX_IO_OVER_DATA_DEFAULT		AO16_TX_IO_OVER_DATA_CHECK

//*****************************************************************************
// AO16_IOCTL_TX_IO_OVER_FRAME
//
//	Parameter:	s32
//		Pass in any valid value, or
//		-1 to read the current setting.
#define	AO16_TX_IO_OVER_FRAME_IGNORE		0
#define	AO16_TX_IO_OVER_FRAME_CHECK			1
#define	AO16_TX_IO_OVER_FRAME_DEFAULT		AO16_TX_IO_OVER_FRAME_CHECK

//*****************************************************************************
// AO16_IOCTL_TX_IO_TIMEOUT (in seconds)
//
//	Parameter:	s32
//		Pass in any value from the minimim to the maximim, the infinite option,
//		or -1 to read the current setting. The value -1 is returned if the
//		feature is not supported, but this should never happen.
#define	AO16_IO_TIMEOUT_DEFAULT				10
#define	AO16_IO_TIMEOUT_NO_SLEEP			0		// Do not sleep to wait for more data.
#define	AO16_IO_TIMEOUT_MIN					0
#define	AO16_IO_TIMEOUT_MAX					GSC_IO_TIMEOUT_MAX
#define	AO16_IO_TIMEOUT_INFINITE			GSC_IO_TIMEOUT_INFINITE

//*****************************************************************************
// AO16_IOCTL_BUFFER_CLEAR					BOR D11
//
//	Parameter:	None.

//*****************************************************************************
// AO16_IOCTL_BUFFER_MODE					BOR D8
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_BUFFER_MODE_OPEN				0
#define	AO16_BUFFER_MODE_CIRC				1

//*****************************************************************************
// AO16_IOCTL_BUFFER_OVER_DATA				BOR D16
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_BUFFER_OVER_DATA_CLR			0
#define	AO16_BUFFER_OVER_DATA_CHK			(-1)

//		The service returns one of the below listed values.
#define	AO16_BUFFER_OVER_DATA_NO			0
#define	AO16_BUFFER_OVER_DATA_YES			1

//*****************************************************************************
// AO16_IOCTL_BUFFER_OVER_FRAME				BOD D17
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_BUFFER_OVER_FRAME_CLR			0
#define	AO16_BUFFER_OVER_FRAME_CHK			(-1)

//		The service returns one of the below listed values.
#define	AO16_BUFFER_OVER_FRAME_NO			0
#define	AO16_BUFFER_OVER_FRAME_YES			1

//*****************************************************************************
// AO16_IOCTL_BUFFER_SIZE					BOR D0-D3
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_BUFFER_SIZE_8					0
#define	AO16_BUFFER_SIZE_16					1
#define	AO16_BUFFER_SIZE_32					2
#define	AO16_BUFFER_SIZE_64					3
#define	AO16_BUFFER_SIZE_128				4
#define	AO16_BUFFER_SIZE_256				5
#define	AO16_BUFFER_SIZE_512				6
#define	AO16_BUFFER_SIZE_1K					7
#define	AO16_BUFFER_SIZE_2K					8
#define	AO16_BUFFER_SIZE_4K					9
#define	AO16_BUFFER_SIZE_8K					10
#define	AO16_BUFFER_SIZE_16K				11
#define	AO16_BUFFER_SIZE_32K				12
#define	AO16_BUFFER_SIZE_64K				13
#define	AO16_BUFFER_SIZE_128K				14
#define	AO16_BUFFER_SIZE_256K				15

//*****************************************************************************
// AO16_IOCTL_BUFFER_STATUS					BOR D12-D15
//
//	Parameter:	s32
//		The service returns one of the below listed values.
#define	AO16_BUFFER_STATUS_EMPTY			0
#define	AO16_BUFFER_STATUS_1Q_FULL			1
#define	AO16_BUFFER_STATUS_MEDIUM			2
#define	AO16_BUFFER_STATUS_3Q_FULL			3
#define	AO16_BUFFER_STATUS_FULL				4

//*****************************************************************************
// AO16_IOCTL_BURST_ENABLE					BCR D0
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_BURST_ENABLE_NO				0
#define	AO16_BURST_ENABLE_YES				1

//*****************************************************************************
// AO16_IOCTL_BURST_READY					BCR D1
//
//	Parameter:	s32
//		The service returns one of the below listed values.
#define	AO16_BURST_READY_NO					0
#define	AO16_BURST_READY_YES				1

//*****************************************************************************
// AO16_IOCTL_BURST_TRIG_SRC				BCR D6
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_BURST_TRIG_SRC_EXT				0
#define	AO16_BURST_TRIG_SRC_SW				1

//*****************************************************************************
// AO16_IOCTL_BURST_TRIGGER					BCR D2
//
//	Parameter:	None.

//*****************************************************************************
// AO16_IOCTL_CHANNEL_SEL					CSR D0-D15
//
//	Parameter:	s32
//		Pass in any valid bitwise combination, or
//		-1 to read the current setting.
//		The range for  8 channel boards is 0x0000 to 0x00FF.
//		The range for 12 channel boards is 0x0000 to 0x0FFF.
//		The range for 16 channel boards is 0x0000 to 0xFFFF.

//*****************************************************************************
// AO16_IOCTL_CLOCK_ENABLE					BOR D5
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_CLOCK_ENABLE_NO				0
#define	AO16_CLOCK_ENABLE_YES				1

//*****************************************************************************
// AO16_IOCTL_CLOCK_READY					BOR D6
//
//	Parameter:	s32
//		The service returns one of the below listed values.
#define	AO16_CLOCK_READY_NO					0
#define	AO16_CLOCK_READY_YES				1

//*****************************************************************************
// AO16_IOCTL_CLOCK_REF_SRC					ACR D9
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_CLOCK_REF_SRC_PRI				0
#define	AO16_CLOCK_REF_SRC_ALT				1

//*****************************************************************************
// AO16_IOCTL_CLOCK_SRC						BOR D4
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_CLOCK_SRC_INT					0
#define	AO16_CLOCK_SRC_EXT_SW				1

//*****************************************************************************
// AO16_IOCTL_CLOCK_SW						BOR D7
//
//	Parameter:	None.

//*****************************************************************************
// AO16_IOCTL_DATA_FORMAT					BCR D4
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_DATA_FORMAT_2S_COMP			0	// Twos Compliment
#define	AO16_DATA_FORMAT_OFF_BIN			1	// Offset Binary

//*****************************************************************************
// AO16_IOCTL_GROUND_SENSE					BCR D3
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_GROUND_SENSE_DISABLE			0
#define	AO16_GROUND_SENSE_REMOTE			1

//*****************************************************************************
// AO16_IOCTL_IRQ_SEL						BCR D8-D10
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_IRQ_INIT_DONE					0
#define	AO16_IRQ_AUTO_CAL_DONE				1
#define	AO16_IRQ_BUF_EMPTY					2
#define	AO16_IRQ_BUF_1Q_FULL				3
#define	AO16_IRQ_BUF_3Q_FULL				4
#define	AO16_IRQ_BURST_TRIG_READY			5
#define	AO16_IRQ_LOAD_READY					6
#define	AO16_IRQ_LOAD_READY_END				7

//*****************************************************************************
// AO16_IOCTL_LOAD_READY					BOR D10
//
//	Parameter:	s32
//		The service returns one of the below listed values.
#define	AO16_LOAD_READY_NO					0
#define	AO16_LOAD_READY_YES					1

//*****************************************************************************
// AO16_IOCTL_LOAD_REQUEST					BOR D9
//	The driver does not wait for the ready state to appear.
//
//	Parameter:	None

//*****************************************************************************
// AO16_IOCTL_NCLK							ACR D0-D8
//
//	Parameter:	s32
//		Pass in any valid value, or
//		-1 to read the current setting.
//		The valid range is 0 to 0x1FF.

//*****************************************************************************
// AO16_IOCTL_NRATE							SRR D0-D17
//
//	Parameter:	s32
//		Pass in any valid value, or
//		-1 to read the current setting.
//		The valid range is 100 to 0x3FFFF.

//*****************************************************************************
// AO16_IOCTL_OUTPUT_MODE					BCR D7
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_OUTPUT_MODE_SEQ				0
#define	AO16_OUTPUT_MODE_SIM				1

//*****************************************************************************
// AO16_IOCTL_RANGE							BCR D16-D17
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_RANGE_1_25						0	// +- 1.25 volts
#define	AO16_RANGE_2_5						1	// +- 2.5 volta
#define	AO16_RANGE_5						2	// +- 5 volts
#define	AO16_RANGE_10						3	// +- 10 volts
#define	AO16_RANGE_20						4	// +- 20 volts

//*****************************************************************************
// AO16_IOCTL_XCVR_TYPE						BCR D5
//
//	Parameter:	s32
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_XCVR_TYPE_TTL					0
#define	AO16_XCVR_TYPE_LVDS					1

//*****************************************************************************
// AO16_IOCTL_TX_IO_ABORT
//
//	Parameter:	s32*
//		The returned value is one of the below options.
#define	AO16_IO_ABORT_NO					0
#define	AO16_IO_ABORT_YES					1

//*****************************************************************************
// AO16_IOCTL_WAIT_EVENT					all fields must be valid
// AO16_IOCTL_WAIT_CANCEL					fields need not be valid
// AO16_IOCTL_WAIT_STATUS					fields need not be valid
//
//	Parameter:	gsc_wait_t*
// gsc_wait_t.flags - see gsc_common.h
// gsc_wait_t.main - see gsc_common.h
// gsc_wait_t.gsc
#define	AO16_WAIT_GSC_INIT_DONE				0x0001
#define	AO16_WAIT_GSC_AUTO_CAL_DONE			0x0002
#define	AO16_WAIT_GSC_BUF_EMPTY				0x0004
#define	AO16_WAIT_GSC_BUF_1Q_FULL			0x0008
#define	AO16_WAIT_GSC_BUF_3Q_FULL			0x0010
#define	AO16_WAIT_GSC_BURST_TRIG_READY		0x0020
#define	AO16_WAIT_GSC_LOAD_READY			0x0040
#define	AO16_WAIT_GSC_LOAD_READY_END		0x0080
#define	AO16_WAIT_GSC_ALL					0x00FF
// gsc_wait_t.alt flags
#define	AO16_WAIT_ALT_ALL					0x0000
// gsc_wait_t.io
#define	AO16_WAIT_IO_TX_ABORT				0x0001
#define	AO16_WAIT_IO_TX_DONE				0x0002
#define	AO16_WAIT_IO_TX_ERROR				0x0004
#define	AO16_WAIT_IO_TX_TIMEOUT				0x0008
#define	AO16_WAIT_IO_ALL					0x000F

//*****************************************************************************
// AO16_IOCTL_CBL_POL_TRIG_IN				BCR D18 (firmware 0x009 or later)
// AO16_IOCTL_CBL_POL_TRIG_OUT				BCR D19 (firmware 0x009 or later)
// AO16_IOCTL_CBL_POL_DAC_CLK_OUT			BCR D20 (firmware 0x009 or later)
// AO16_IOCTL_CBL_POL_CLOCK_IO				BCR D21 (firmware 0x009 or later)
//
//	Parameter:	s32*
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_CBL_POL_NORM					0
#define	AO16_CBL_POL_INV					1

//*****************************************************************************
// AO16_IOCTL_CBL_ISO_TRIG_OUT				BOR D18 (firmware 0x011 or later)
// AO16_IOCTL_CBL_ISO_DAC_CLK_OUT			BOR D19 (firmware 0x011 or later)
// AO16_IOCTL_CBL_ISO_CLOCK_IO				BOR D20 (firmware 0x011 or later)
//
//	Parameter:	s32*
//		Pass in any of the below options, or
//		-1 to read the current setting.
#define	AO16_CBL_ISO_NORM					0
#define	AO16_CBL_ISO_OUT_0					1



#endif
