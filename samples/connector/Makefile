# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_SAMPLE_CONNECTOR) += cn_test.o

<<<<<<< HEAD
# List of programs to build
ifdef CONFIG_SAMPLE_CONNECTOR
hostprogs-y := ucon
endif

# Tell kbuild to always build the programs
always := $(hostprogs-y)

HOSTCFLAGS_ucon.o += -I$(objtree)/usr/include

all: modules

modules clean:
	$(MAKE) -C ../.. SUBDIRS=$(CURDIR) $@
=======
userprogs-always-$(CONFIG_CC_CAN_LINK) += ucon

userccflags += -I usr/include
>>>>>>> upstream/android-13
