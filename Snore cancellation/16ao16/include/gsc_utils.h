// $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/gsc_utils.h $
// $Rev: 44503 $
// $Date: 2019-03-01 14:38:03 -0600 (Fri, 01 Mar 2019) $

// OS & Device Independent: Utility: header file

#ifndef	__GSC_UTILS_H__
#define	__GSC_UTILS_H__

#include "gsc_common.h"
#include "os_utils.h"



// data types *****************************************************************

typedef enum
{
	GSC_ARG_DEV_INDEX,	// index of device/board to access, values[0] is minimum, MUST LIST LAST
	GSC_ARG_S32_FLAG,	// values[0] is assigned to var
	GSC_ARG_S32_MASK,	// arg is a decimal, values[0] is mask of valid bits
	GSC_ARG_S32_MIN,	// arg is a decimal, values[0] is minimum
	GSC_ARG_S32_RANGE,	// arg is a decimal, values[0] is minimum, values[1] is maximum
	GSC_ARG_DEV_NOTE,	// Report the description only.
	GSC_ARG_STR_PTR		// arg is a string, values are unused, var is char**
} gsc_arg_type_t;

typedef struct
{
	gsc_arg_type_t	type;
	s32*			var;	// The command line argument value is tstored here.
	const char*		arg;	// Portion which must match. "#" is added automatically
	s32				values[2];
	const char*		desc;
} gsc_arg_item_t;

typedef struct
{
	const char*				name;
	const char*				desc;
	unsigned int			qty;	// size of following list
	const gsc_arg_item_t*	list;
} gsc_arg_set_t;

typedef struct
{
	void*	buffer;	// Data goes here.
	size_t	size;	// This is the size of the buffer in bytes.
	size_t	offset;	// This is where the data begins.
	u32		count;	// This is the number of bytes of data available beginning at offset.
	int		eof;	// Have we hit the end of an input file?
} gsc_buf_man_t;

typedef struct		// For listing register values.
{
	const char*	name;
	int			reg;
	int			err;
	u32			value;
	int			ask_support;	// Call below function for this.
	int			(*decode)(int fd, int supported, u32 value, int width);
	const char*	desc;			// Description
} gsc_reg_def_t;



// prototypes *****************************************************************

int			gsc_args_parse(int argc, char** argv, const gsc_arg_set_t* set);

int			gsc_buf_man_free_all(void);
int			gsc_buf_man_init(void);
int			gsc_buf_man_release_buffer(gsc_buf_man_t* bm);
int			gsc_buf_man_request_data(gsc_buf_man_t* bm);
int			gsc_buf_man_request_empty(gsc_buf_man_t* bm);
int			gsc_buf_man_setup(size_t qty, size_t size);
int			gsc_buf_man_stop(void);
int			gsc_buf_man_stats(void);

void		gsc_label(const char* label);
void		gsc_label_float_comma(double value, int len, int places);
void		gsc_label_float_comma_buf(double value, int len, int places, char* dest);
void		gsc_label_init(int width);
int			gsc_label_indent(int delta);
void		gsc_label_index(const char* label, int index);
void		gsc_label_level_dec(void);
void		gsc_label_level_inc(void);
void		gsc_label_long_comma(long long value);
void		gsc_label_long_comma_buf(long long value, char* buffer);
void		gsc_label_suffix(const char* label, const char* suffix);

const char*	gsc_reg_pex8111_get_desc(u32 reg);
const char*	gsc_reg_pex8111_get_name(u32 reg);
int			gsc_reg_pex8111_list_pci(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));
int			gsc_reg_pex8111_list_plx(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));

const char*	gsc_reg_pex8112_get_desc(u32 reg);
const char*	gsc_reg_pex8112_get_name(u32 reg);
int			gsc_reg_pex8112_list_pci(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));
int			gsc_reg_pex8112_list_plx(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));

const char*	gsc_reg_plx9056_get_desc(u32 reg);
const char*	gsc_reg_plx9056_get_name(u32 reg);
int			gsc_reg_plx9056_list_pci(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));
int			gsc_reg_plx9056_list_plx(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));

const char*	gsc_reg_plx9060es_get_desc(u32 reg);
const char*	gsc_reg_plx9060es_get_name(u32 reg);
int			gsc_reg_plx9060es_list_pci(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));
int			gsc_reg_plx9060es_list_plx(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));

const char*	gsc_reg_plx9080_get_desc(u32 reg);
const char*	gsc_reg_plx9080_get_name(u32 reg);
int			gsc_reg_plx9080_list_pci(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));
int			gsc_reg_plx9080_list_plx(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));

const char*	gsc_reg_plx9656_get_desc(u32 reg);
const char*	gsc_reg_plx9656_get_name(u32 reg);
int			gsc_reg_plx9656_list_pci(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));
int			gsc_reg_plx9656_list_plx(int fd, int (reg_read)(int fd, int index, int verbose, u32 reg, u32* value));

size_t		gsc_time_delta_ms(void);
void		gsc_time_format_ms(long ms, char* dest, size_t size);
void		gsc_time_sleep_ms(long ms);

void		gsc_reg_field_show(
				int					label_width,
				int					name_width,
				u32					value,
				int					hi_bit,
				int					low_bit,
				int					eol,
				const char* const *	list,
				const char*			name);

int			gsc_reg_list(
				int				fd,
				gsc_reg_def_t*	list,
				int				detail,
				int				(reg_read)(int fd, int index, int verbose, u32 reg, u32* value));



#endif
