# $URL: http://subversion:8080/svn/gsc/trunk/drivers/LINUX/16AO16/16AO16_Linux_2.x.x.x_DN/samples/aout/makefile $
# $Rev: 45258 $
# $Date: 2019-07-11 12:47:53 -0500 (Thu, 11 Jul 2019) $

# 16AO16: Sample Application: make file

default:	all

CC			= gcc
CC_FLAGS	= -c -Wall -I../../include -L/usr/lib
DEP_FILE	= makefile.dep
LIB_FILES	= ../../lib/16ao16_main.a ../../lib/lib16ao16_api.so
TARGET		= aout

.NOTPARALLEL: %.o

ECHO	:= ${shell ls /bin/echo 2>/dev/null | wc -l}
ifeq ("${ECHO}","1")
ECHO	:= /bin/echo
else
ECHO	:= echo
endif

OBJ_FILES	=		\
			main.o	\
			test.o

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
