#******************************************************************************
#
# Makefile - Rules for building the power example.
#
# Copyright (c) 2005-2008 Luminary Micro, Inc.  All rights reserved.
# 
# Software License Agreement
# 
# Luminary Micro, Inc. (LMI) is supplying this software for use solely and
# exclusively on LMI's microcontroller products.
# 
# The software is owned by LMI and/or its suppliers, and is protected under
# applicable copyright laws.  All rights are reserved.  You may not combine
# this software with "viral" open-source software in order to form a larger
# program.  Any use in violation of the foregoing restrictions may subject
# the user to criminal sanctions under applicable laws, as well as to civil
# liability for the breach of the terms and conditions of this license.
# 
# THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
# OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
# LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
# CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
# 
# This is part of revision 3416 of the Stellaris Peripheral Driver Library.
#
#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=LM3S2110
#PART=LM3S8962
#PART=LM3S811
#PART=LM3S1138
NAME=power
VERSION=1
SUBLEVEL=1
PACKETNAME=$(PART)-$(NAME)-$(VERSION).$(SUBLEVEL)

DEBUG=1

#
# The base directory for DriverLib.
#
ROOT=.

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH +=${ROOT}/source
VPATH +=${ROOT}/source/lcd
VPATH +=${ROOT}/source/periph
VPATH +=${ROOT}/source/system

#
# The default rule, which causes the power example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/power.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}
	@cd ./tools;make clean

distclean:
	@rm -rf ${COMPILER} ${wildcard *~} tags cscope.* *.vim vim.*

dist: $(PACKETNAME).tar.bz2
$(PACKETNAME).tar.bz2:
	-rm -rf $(PACKETNAME)
	mkdir $(PACKETNAME)
	make distclean
	-cp * -R $(PACKETNAME)
	tar jcvf $@ $(PACKETNAME)
	-rm -rf $(PACKETNAME)

#
# Other build
#
tools:
	cd tools;make
cscope:
	ctags -R
	cscope -RqkbvI /usr/local/arm/luminary/arm-luminary-eabi/include
#cscope -RqkbvI /opt/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/arm-none-eabi/include/
#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir ${COMPILER}

#
# Rules for building the power example.
#
#${COMPILER}/power.axf: ${COMPILER}/osram96x16x1.o
${COMPILER}/power.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/power.axf: ${COMPILER}/power.o
#
# Add power source
#
# ${COMPILER}/power.axf: ${COMPILER}/sys_adc.o

${COMPILER}/power.axf: ${COMPILER}/lcd_5110.o
${COMPILER}/power.axf: ${COMPILER}/frame-buffer.o
${COMPILER}/power.axf: ${COMPILER}/display.o
${COMPILER}/power.axf: ${COMPILER}/dac_5618.o
${COMPILER}/power.axf: ${COMPILER}/button.o
${COMPILER}/power.axf: ${COMPILER}/sys_pwm.o
${COMPILER}/power.axf: ${COMPILER}/sys_timer.o
${COMPILER}/power.axf: ${COMPILER}/menu.o
${COMPILER}/power.axf: ${COMPILER}/wave.o
${COMPILER}/power.axf: ${COMPILER}/iic.o
${COMPILER}/power.axf: ${COMPILER}/ads1115.o
${COMPILER}/power.axf: ${COMPILER}/infrared.o

#
# power source end
#
#${COMPILER}/power.axf: ${ROOT}/src/${COMPILER}/libdriver.a
ifdef DEBUG
${COMPILER}/power.axf: ${ROOT}/lib/libdriver.a
else
${COMPILER}/power.axf: ${ROOT}/lib/libdriver-nodebug.a
endif

ifeq (${COMPILER}, gcc)
${COMPILER}/power.axf: power.ld
endif

SCATTERgcc_power=power.ld
SCATTERsourcerygxx_power=lm3s811-rom.ld -T power_sourcerygxx.ld
ENTRY_power=ResetISR

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
