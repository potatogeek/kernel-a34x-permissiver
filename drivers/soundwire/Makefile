<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
#
# Makefile for soundwire core
#

#Bus Objs
<<<<<<< HEAD
soundwire-bus-objs := bus_type.o bus.o slave.o mipi_disco.o stream.o
obj-$(CONFIG_SOUNDWIRE) += soundwire-bus.o

#Cadence Objs
soundwire-cadence-objs := cadence_master.o
obj-$(CONFIG_SOUNDWIRE_CADENCE) += soundwire-cadence.o

#Intel driver
soundwire-intel-objs :=	intel.o
obj-$(CONFIG_SOUNDWIRE_INTEL) += soundwire-intel.o

soundwire-intel-init-objs := intel_init.o
obj-$(CONFIG_SOUNDWIRE_INTEL) += soundwire-intel-init.o
=======
soundwire-bus-y := bus_type.o bus.o master.o slave.o mipi_disco.o stream.o  \
			sysfs_slave.o sysfs_slave_dpn.o
obj-$(CONFIG_SOUNDWIRE) += soundwire-bus.o

soundwire-generic-allocation-objs := generic_bandwidth_allocation.o
obj-$(CONFIG_SOUNDWIRE_GENERIC_ALLOCATION) += soundwire-generic-allocation.o

ifdef CONFIG_DEBUG_FS
soundwire-bus-y += debugfs.o
endif

#Cadence Objs
soundwire-cadence-y := cadence_master.o
obj-$(CONFIG_SOUNDWIRE_CADENCE) += soundwire-cadence.o

#Intel driver
soundwire-intel-y :=	intel.o intel_init.o dmi-quirks.o
obj-$(CONFIG_SOUNDWIRE_INTEL) += soundwire-intel.o

#Qualcomm driver
soundwire-qcom-y :=	qcom.o
obj-$(CONFIG_SOUNDWIRE_QCOM) += soundwire-qcom.o
>>>>>>> upstream/android-13
