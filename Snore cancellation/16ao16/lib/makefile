# $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/lib/makefile $
# $Rev: 45266 $
# $Date: 2019-07-11 12:50:59 -0500 (Thu, 11 Jul 2019) $

# 16AO16: Library Directory: make file

default: all

TARGET		= 16ao16_main.a

.NOTPARALLEL: %.a

LIB_FILES	=				\
			16ao16_dsl.a	\
			16ao16_utils.a	\
			gsc_utils.a



all: ${TARGET}
	@echo ==== All Done

release: ${TARGET}
	@echo ==== release Done

clean:
	@echo ==== Cleaning ${TARGET} ...
	@rm -f ${TARGET}

${TARGET}: ${LIB_FILES}
	@echo ==== Linking: $@
	@ld -r -o $@ ${LIB_FILES}
