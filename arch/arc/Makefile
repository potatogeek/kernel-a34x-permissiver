<<<<<<< HEAD
#
# Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#

KBUILD_DEFCONFIG := nsim_hs_defconfig

cflags-y	+= -fno-common -pipe -fno-builtin -mmedium-calls -D__linux__
cflags-$(CONFIG_ISA_ARCOMPACT)	+= -mA7
cflags-$(CONFIG_ISA_ARCV2)	+= -mcpu=hs38

ifdef CONFIG_ARC_CURR_IN_REG
# For a global register defintion, make sure it gets passed to every file
=======
# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
#

KBUILD_DEFCONFIG := haps_hs_smp_defconfig

ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE := $(call cc-cross-prefix, arc-linux- arceb-linux-)
endif

cflags-y	+= -fno-common -pipe -fno-builtin -mmedium-calls -D__linux__

tune-mcpu-def-$(CONFIG_ISA_ARCOMPACT)	:= -mcpu=arc700
tune-mcpu-def-$(CONFIG_ISA_ARCV2)	:= -mcpu=hs38

ifeq ($(CONFIG_ARC_TUNE_MCPU),"")
cflags-y				+= $(tune-mcpu-def-y)
else
tune-mcpu				:= $(shell echo $(CONFIG_ARC_TUNE_MCPU))
ifneq ($(call cc-option,$(tune-mcpu)),)
cflags-y				+= $(tune-mcpu)
else
# The flag provided by 'CONFIG_ARC_TUNE_MCPU' option isn't known by this compiler
# (probably the compiler is too old). Use ISA default mcpu flag instead as a safe option.
$(warning ** WARNING ** CONFIG_ARC_TUNE_MCPU flag '$(tune-mcpu)' is unknown, fallback to '$(tune-mcpu-def-y)')
cflags-y				+= $(tune-mcpu-def-y)
endif
endif


ifdef CONFIG_ARC_CURR_IN_REG
# For a global register definition, make sure it gets passed to every file
>>>>>>> upstream/android-13
# We had a customer reported bug where some code built in kernel was NOT using
# any kernel headers, and missing the r25 global register
# Can't do unconditionally because of recursive include issues
# due to <linux/thread_info.h>
<<<<<<< HEAD
LINUXINCLUDE	+=  -include ${src}/arch/arc/include/asm/current.h
=======
LINUXINCLUDE	+=  -include $(srctree)/arch/arc/include/asm/current.h
>>>>>>> upstream/android-13
endif

cflags-y				+= -fsection-anchors

cflags-$(CONFIG_ARC_HAS_LLSC)		+= -mlock
cflags-$(CONFIG_ARC_HAS_SWAPE)		+= -mswape

ifdef CONFIG_ISA_ARCV2

<<<<<<< HEAD
=======
ifdef CONFIG_ARC_USE_UNALIGNED_MEM_ACCESS
cflags-y				+= -munaligned-access
else
cflags-y				+= -mno-unaligned-access
endif

>>>>>>> upstream/android-13
ifndef CONFIG_ARC_HAS_LL64
cflags-y				+= -mno-ll64
endif

ifndef CONFIG_ARC_HAS_DIV_REM
cflags-y				+= -mno-div-rem
endif

endif

cfi := $(call as-instr,.cfi_startproc\n.cfi_endproc,-DARC_DW2_UNWIND_AS_CFI)
cflags-$(CONFIG_ARC_DW2_UNWIND)		+= -fasynchronous-unwind-tables $(cfi)

<<<<<<< HEAD
ifndef CONFIG_CC_OPTIMIZE_FOR_SIZE
# Generic build system uses -O2, we want -O3
# Note: No need to add to cflags-y as that happens anyways
#
# Disable the false maybe-uninitialized warings gcc spits out at -O3
ARCH_CFLAGS += -O3 $(call cc-disable-warning,maybe-uninitialized,)
endif

=======
>>>>>>> upstream/android-13
# small data is default for elf32 tool-chain. If not usable, disable it
# This also allows repurposing GP as scratch reg to gcc reg allocator
disable_small_data := y
cflags-$(disable_small_data)		+= -mno-sdata -fcall-used-gp

cflags-$(CONFIG_CPU_BIG_ENDIAN)		+= -mbig-endian
ldflags-$(CONFIG_CPU_BIG_ENDIAN)	+= -EB

LIBGCC	= $(shell $(CC) $(cflags-y) --print-libgcc-file-name)

# Modules with short calls might break for calls into builtin-kernel
KBUILD_CFLAGS_MODULE	+= -mlong-calls -mno-millicode

# Finally dump eveything into kernel build system
KBUILD_CFLAGS	+= $(cflags-y)
KBUILD_AFLAGS	+= $(KBUILD_CFLAGS)
KBUILD_LDFLAGS	+= $(ldflags-y)

head-y		:= arch/arc/kernel/head.o

<<<<<<< HEAD
# See arch/arc/Kbuild for content of core part of the kernel
core-y		+= arch/arc/

=======
>>>>>>> upstream/android-13
# w/o this dtb won't embed into kernel binary
core-y		+= arch/arc/boot/dts/

core-y				+= arch/arc/plat-sim/
core-$(CONFIG_ARC_PLAT_TB10X)	+= arch/arc/plat-tb10x/
core-$(CONFIG_ARC_PLAT_AXS10X)	+= arch/arc/plat-axs10x/
<<<<<<< HEAD
core-$(CONFIG_ARC_PLAT_EZNPS)	+= arch/arc/plat-eznps/
core-$(CONFIG_ARC_SOC_HSDK)	+= arch/arc/plat-hsdk/

ifdef CONFIG_ARC_PLAT_EZNPS
KBUILD_CPPFLAGS += -I$(srctree)/arch/arc/plat-eznps/include
endif

drivers-$(CONFIG_OPROFILE)	+= arch/arc/oprofile/

=======
core-$(CONFIG_ARC_SOC_HSDK)	+= arch/arc/plat-hsdk/

>>>>>>> upstream/android-13
libs-y		+= arch/arc/lib/ $(LIBGCC)

boot		:= arch/arc/boot

<<<<<<< HEAD
boot_targets := uImage uImage.bin uImage.gz uImage.lzma
=======
boot_targets := uImage.bin uImage.gz uImage.lzma
>>>>>>> upstream/android-13

PHONY += $(boot_targets)
$(boot_targets): vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

<<<<<<< HEAD
%.dtb %.dtb.S %.dtb.o: scripts
	$(Q)$(MAKE) $(build)=$(boot)/dts $(boot)/dts/$@

dtbs: scripts
	$(Q)$(MAKE) $(build)=$(boot)/dts
=======
uimage-default-y			:= uImage.bin
uimage-default-$(CONFIG_KERNEL_GZIP)	:= uImage.gz
uimage-default-$(CONFIG_KERNEL_LZMA)	:= uImage.lzma

PHONY += uImage
uImage: $(uimage-default-y)
	@ln -sf $< $(boot)/uImage
	@$(kecho) '  Image $(boot)/uImage is ready'

CLEAN_FILES += $(boot)/uImage
>>>>>>> upstream/android-13

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)
