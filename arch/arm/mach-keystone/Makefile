# SPDX-License-Identifier: GPL-2.0
obj-y					:= keystone.o smc.o

<<<<<<< HEAD
plus_sec := $(call as-instr,.arch_extension sec,+sec)
AFLAGS_smc.o				:=-Wa,-march=armv7-a$(plus_sec)

=======
>>>>>>> upstream/android-13
obj-$(CONFIG_SMP)			+= platsmp.o

# PM domain driver for Keystone SOCs
obj-$(CONFIG_ARCH_KEYSTONE)		+= pm_domain.o
