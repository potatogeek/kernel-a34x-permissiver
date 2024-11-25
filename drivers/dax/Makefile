# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_DAX) += dax.o
obj-$(CONFIG_DEV_DAX) += device_dax.o
<<<<<<< HEAD
obj-$(CONFIG_DEV_DAX_PMEM) += dax_pmem.o

dax-y := super.o
dax_pmem-y := pmem.o
device_dax-y := device.o
=======
obj-$(CONFIG_DEV_DAX_KMEM) += kmem.o

dax-y := super.o
dax-y += bus.o
device_dax-y := device.o

obj-y += pmem/
obj-y += hmem/
>>>>>>> upstream/android-13
