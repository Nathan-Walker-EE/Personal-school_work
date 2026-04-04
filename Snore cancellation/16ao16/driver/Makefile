# $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/driver/Makefile $
# $Rev: 44907 $
# $Date: 2019-05-16 20:31:04 -0500 (Thu, 16 May 2019) $

# 16AO16: Device Driver: make file

TARGET		:= 16ao16

.NOTPARALLEL: %.o

OBJ_FILES	=					\
			close.o				\
			device.o			\
			io.o				\
			ioctl.o				\
			irq.o				\
			open.o				\
			reg.o				\
			write.o				\
								\
			gsc_bar.o			\
			gsc_close.o			\
			gsc_dma.o			\
			gsc_endian.o		\
			gsc_init.o			\
			gsc_io.o			\
			gsc_ioctl.o			\
			gsc_irq.o			\
			gsc_open.o			\
			gsc_read.o			\
			gsc_reg.o			\
			gsc_utils.o			\
			gsc_wait.o			\
			gsc_write.o			\
								\
			os_bar.o			\
			os_close.o			\
			os_event.o			\
			os_init.o			\
			os_ioctl.o			\
			os_irq.o			\
			os_kernel_2_2.o		\
			os_kernel_2_4.o		\
			os_kernel_2_6.o		\
			os_kernel_3.o		\
			os_kernel_4.o		\
			os_kernel_5.o		\
			os_mem.o			\
			os_open.o			\
			os_pci.o			\
			os_proc.o			\
			os_read.o			\
			os_reg.o			\
			os_sem.o			\
			os_spinlock.o		\
			os_time.o			\
			os_write.o




###############################################################################
# Everything here is needed so we can access os_makefile.inc.

.EXPORT_ALL_VARIABLES:

# Locate the source from the starting directory.
ifndef GSC_DEV_DIR
GSC_DEV_DIR	:= $(shell pwd)
endif

include ${GSC_DEV_DIR}/os_makefile.inc
