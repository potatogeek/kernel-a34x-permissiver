<<<<<<< HEAD
#
# Common code for all Loongson based systems
#

obj-$(CONFIG_MACH_LOONGSON64) += common/

#
# Lemote Fuloong mini-PC (Loongson 2E-based)
#

obj-$(CONFIG_LEMOTE_FULOONG2E)	+= fuloong-2e/

#
# Lemote loongson2f family machines
#

obj-$(CONFIG_LEMOTE_MACH2F)  += lemote-2f/

#
# All Loongson-3 family machines
#

obj-$(CONFIG_CPU_LOONGSON3)  += loongson-3/
=======
# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for Loongson-3 family machines
#
obj-$(CONFIG_MACH_LOONGSON64) += cop2-ex.o dma.o \
				setup.o init.o env.o time.o reset.o \

obj-$(CONFIG_SMP)	+= smp.o
obj-$(CONFIG_NUMA)	+= numa.o
obj-$(CONFIG_RS780_HPET) += hpet.o
obj-$(CONFIG_SUSPEND) += pm.o
obj-$(CONFIG_PCI_QUIRKS) += vbios_quirk.o
obj-$(CONFIG_CPU_LOONGSON3_CPUCFG_EMULATION) += cpucfg-emul.o
obj-$(CONFIG_SYSFS) += boardinfo.o
>>>>>>> upstream/android-13
