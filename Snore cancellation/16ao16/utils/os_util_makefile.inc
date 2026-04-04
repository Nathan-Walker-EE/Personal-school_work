# $URL: http://subversion:8080/svn/gsc/trunk/drivers/gsc_common/utils/linux/os_util_makefile.inc $
# $Rev: 43563 $
# $Date: 2018-10-04 19:40:44 -0500 (Thu, 04 Oct 2018) $

# Linux: Utility: Common makefile

.PHONY: all clean default makefile release

ECHO	:= ${shell ls -d ../lib 2>/dev/null | wc -l}
ifeq ("${ECHO}","1")
GSC_TARGET			= ../lib/gsc_utils.a
else
GSC_TARGET			= gsc_utils.a
endif

GSC_OBJ_FILES		:=							\
					./gsc_util_args.o			\
					./gsc_util_buf_man.o		\
					./gsc_util_label.o			\
					./gsc_util_reg.o			\
					./gsc_util_reg_pex8111.o	\
					./gsc_util_reg_pex8112.o	\
					./gsc_util_reg_plx9056.o	\
					./gsc_util_reg_plx9060es.o	\
					./gsc_util_reg_plx9080.o	\
					./gsc_util_reg_plx9656.o	\
					./gsc_util_time.o			\
												\
					./os_util_count.o			\
					./os_util_id.o				\
					./os_util_kbd.o				\
					./os_util_sem.o				\
					./os_util_sleep.o			\
					./os_util_thread.o			\
					./os_util_time.o



ECHO	:= ${shell ls /bin/echo 2>/dev/null | wc -l}
ifeq ("${ECHO}","1")
ECHO	:= /bin/echo
else
ECHO	:= echo
endif



${GSC_TARGET}: ${GSC_OBJ_FILES}
	@echo ==== Linking: $@
	@ld -r -o $@ ${GSC_OBJ_FILES}


