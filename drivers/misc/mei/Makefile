# SPDX-License-Identifier: GPL-2.0
#
<<<<<<< HEAD
# Makefile - Intel Management Engine Interface (Intel MEI) Linux driver
# Copyright (c) 2010-2014, Intel Corporation.
=======
# Copyright (c) 2010-2019, Intel Corporation. All rights reserved.
# Makefile - Intel Management Engine Interface (Intel MEI) Linux driver
>>>>>>> upstream/android-13
#
obj-$(CONFIG_INTEL_MEI) += mei.o
mei-objs := init.o
mei-objs += hbm.o
mei-objs += interrupt.o
mei-objs += client.o
mei-objs += main.o
<<<<<<< HEAD
=======
mei-objs += dma-ring.o
>>>>>>> upstream/android-13
mei-objs += bus.o
mei-objs += bus-fixup.o
mei-$(CONFIG_DEBUG_FS) += debugfs.o

obj-$(CONFIG_INTEL_MEI_ME) += mei-me.o
mei-me-objs := pci-me.o
mei-me-objs += hw-me.o

obj-$(CONFIG_INTEL_MEI_TXE) += mei-txe.o
mei-txe-objs := pci-txe.o
mei-txe-objs += hw-txe.o

mei-$(CONFIG_EVENT_TRACING) += mei-trace.o
CFLAGS_mei-trace.o = -I$(src)
<<<<<<< HEAD
=======

obj-$(CONFIG_INTEL_MEI_HDCP) += hdcp/
>>>>>>> upstream/android-13
