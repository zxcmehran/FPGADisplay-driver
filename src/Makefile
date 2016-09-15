##############################################################################
## FPGA Display Handler IP Core By Mehran Ahadi (http://mehran.ahadi.me)
## This IP allows you to draw shapes and print texts on VGA screen.
## Copyright (C) 2015-2016  Mehran Ahadi
## This work is released under MIT License.
##
## Description:       Microprocessor Driver Makefile
## Date:              Mon Aug 08 23:53:13 2016 (by Create and Import Peripheral Wizard)
##############################################################################

COMPILER=
ARCHIVER=
CP=cp
COMPILER_FLAGS=
EXTRA_COMPILER_FLAGS=
LIB=libxil.a

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I./. -I${INCLUDEDIR}

INCLUDEFILES=*.h
LIBSOURCES=*.c
OUTS = *.o

libs:
	echo "Compiling display_handler"
	$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) $(LIBSOURCES)
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OUTS}
	make clean

include:
	${CP} $(INCLUDEFILES) $(INCLUDEDIR)

clean:
	rm -rf ${OUTS}
