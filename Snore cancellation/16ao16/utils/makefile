# $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/utils/makefile $
# $Rev: 45045 $
# $Date: 2019-06-04 16:05:38 -0500 (Tue, 04 Jun 2019) $

# 16AO16: Utilities: make file

default: all

CC			= gcc
CC_FLAGS	= -c -Wall -I../include
DEP_FILE	= makefile.dep
TARGET		= ../lib/16ao16_utils.a

.NOTPARALLEL: %.o

ECHO	:= ${shell ls /bin/echo 2>/dev/null | wc -l}
ifeq ("${ECHO}","1")
ECHO	:= /bin/echo
else
ECHO	:= echo
endif

OBJ_FILES	=		\
			close.o	\
			init.o	\
			ioctl.o	\
			open.o	\
			read.o	\
			write.o	\
			\
			util_config_ao.o	\
			util_count.o		\
			util_fsamp_ao.o		\
			util_id.o			\
			util_reg.o			\
			\
			util_auto_cal_sts.o			\
			util_auto_calibrate.o		\
			util_buffer_clear.o			\
			util_buffer_mode.o			\
			util_buffer_over_data.o		\
			util_buffer_over_frame.o	\
			util_buffer_size.o			\
			util_buffer_status.o		\
			util_burst_enable.o			\
			util_burst_ready.o			\
			util_burst_trig_src.o		\
			util_burst_trigger.o		\
			util_cbl_iso_clock_io.o		\
			util_cbl_iso_dac_clk_out.o	\
			util_cbl_iso_trig_out.o		\
			util_cbl_pol_clock_io.o		\
			util_cbl_pol_dac_clk_out.o	\
			util_cbl_pol_trig_in.o		\
			util_cbl_pol_trig_out.o		\
			util_channel_sel.o			\
			util_clock_enable.o			\
			util_clock_ready.o			\
			util_clock_ref_src.o		\
			util_clock_src.o			\
			util_clock_sw.o				\
			util_data_format.o			\
			util_ground_sense.o			\
			util_initialize.o			\
			util_irq_sel.o				\
			util_load_ready.o			\
			util_load_request.o			\
			util_nclk.o					\
			util_nrate.o				\
			util_output_mode.o			\
			util_query.o				\
			util_range.o				\
			util_reg_mod.o				\
			util_reg_read.o				\
			util_reg_write.o			\
			util_tx_io_abort.o			\
			util_tx_io_mode.o			\
			util_tx_io_over_data.o		\
			util_tx_io_over_frame.o		\
			util_tx_io_timeout.o		\
			util_wait_cancel.o			\
			util_wait_event.o			\
			util_wait_status.o			\
			util_xcvr_type.o



include ./os_util_makefile.inc



.c.o:
	@echo == Compiling: $<
	@-chmod +rw ${DEP_FILE}
	@# Get the dependency list for this module.
	@-${CC} -MM ${CC_FLAGS} $< > .tmp1
	@# Remove the trailing white space and backslash.
	@-sed -e "s/[ ]*[\\\\]//g" < .tmp1 > .tmp2
	@# Put everything on seperate lines.
	@-tr [:space:] \\n < .tmp2 > .tmp3
	@# Remove all of the system include files.
	@-grep -v "^[ ]*/" < .tmp3 > .tmp4
	@# Remove all empty lines.
	@-grep [[:alnum:]] < .tmp4 > .tmp5
	@# Put everything on the same line.
	@-tr '\n' '\040' < .tmp5 > .tmp6
	@-${ECHO} -e '\012' >> .tmp6
	@# Add all of the other dependencies to the end of this file.
	@-echo >> ${DEP_FILE}
	@-grep -v "^[ ]*$@" < ${DEP_FILE} >> .tmp6
	@# Remove blank lines from the list.
	@-grep "[[:alnum:]]" < .tmp6 > .tmp7
	@# Sort the list and put it in the dependency file.
	@-sort < .tmp7 > ${DEP_FILE}
	@# Cleanup.
	@rm -f ${DEP_FILE}.tmp* .tmp*
	@# Compile the module.
	@${CC} ${CC_FLAGS} $< -o $@



all: ${GSC_TARGET} ${TARGET}
	@echo ==== All Done

release: ${GSC_TARGET} ${TARGET}
	@rm -f *.o
	@echo ==== release Done

clean:
	@echo ==== Cleaning ${GSC_TARGET} ${TARGET} ...
	@rm -f *.a *.o *.scc ${GSC_TARGET} ${TARGET}
	@-chmod +rw ${DEP_FILE}
	@echo > ${DEP_FILE}

${TARGET}: ${GSC_TARGET} ${OBJ_FILES}
	@echo ==== Linking: $@
	@ld -r -o $@ ${OBJ_FILES}

${DEP_FILE}:
	@echo ==== Creating: $@
	@echo > ${DEP_FILE}

include ${DEP_FILE}
