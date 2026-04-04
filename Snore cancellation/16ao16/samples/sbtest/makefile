# $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/sbtest/makefile $
# $Rev: 45263 $
# $Date: 2019-07-11 12:49:09 -0500 (Thu, 11 Jul 2019) $

# 16AO16: Sample Application: make file

default:	all

CC			= gcc
CC_FLAGS	= -c -Wall -I../../include -L/usr/lib
DEP_FILE	= makefile.dep
LIB_FILES	= ../../lib/16ao16_main.a ../../lib/lib16ao16_api.so
TARGET		= sbtest

.NOTPARALLEL: %.o

ECHO	:= ${shell ls /bin/echo 2>/dev/null | wc -l}
ifeq ("${ECHO}","1")
ECHO	:= /bin/echo
else
ECHO	:= echo
endif

OBJ_FILES	=						\
			main.o					\
			service.o				\
									\
			auto_calibrate.o		\
			auto_cal_sts.o			\
			buffer_clear.o			\
			buffer_mode.o			\
			buffer_over_data.o		\
			buffer_over_frame.o		\
			buffer_size.o			\
			buffer_status.o			\
			burst_enable.o			\
			burst_ready.o			\
			burst_trig_src.o		\
			burst_trigger.o			\
			cbl_iso_clock_io.o		\
			cbl_iso_dac_clk_out.o	\
			cbl_iso_trig_out.o		\
			cbl_pol_clock_io.o		\
			cbl_pol_dac_clk_out.o	\
			cbl_pol_trig_in.o		\
			cbl_pol_trig_out.o		\
			channel_sel.o			\
			clock_enable.o			\
			clock_ready.o			\
			clock_ref_src.o			\
			clock_src.o				\
			clock_sw.o				\
			data_format.o			\
			ground_sense.o			\
			initialize.o			\
			irq_sel.o				\
			load_ready.o			\
			load_request.o			\
			nclk.o					\
			nrate.o					\
			output_mode.o			\
			query.o					\
			range.o					\
			reg_read.o				\
			reg_write.o				\
			reg_mod.o				\
			tx_io_abort.o			\
			tx_io_mode.o			\
			tx_io_over_data.o		\
			tx_io_over_frame.o		\
			tx_io_timeout.o			\
			wait_cancel.o			\
			wait_event.o			\
			wait_status.o			\
			xcvr.o

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



all: ${TARGET}
	@echo ==== All Done

release: ${TARGET}
	@rm -f *.a *.o ${OBJ_FILES}
	@echo ==== Release Done

clean:
	@echo ==== Cleaning ${TARGET} ...
	@rm -f *.a *.o *.scc ${TARGET}
	@-chmod +rw ${DEP_FILE}
	@echo > ${DEP_FILE}

${TARGET}: ${OBJ_FILES} ${LIB_FILES}
	@echo ==== Linking: $@
	@${CC} ${OBJ_FILES} ${LIB_FILES} -o $@ -lm -lpthread -lrt

${DEP_FILE}:
	@echo ==== Creating: $@
	@echo > ${DEP_FILE}

include ${DEP_FILE}
