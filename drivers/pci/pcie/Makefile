# SPDX-License-Identifier: GPL-2.0
#
# Makefile for PCI Express features and port driver

<<<<<<< HEAD
pcieportdrv-y			:= portdrv_core.o portdrv_pci.o err.o
=======
pcieportdrv-y			:= portdrv_core.o portdrv_pci.o err.o rcec.o
>>>>>>> upstream/android-13

obj-$(CONFIG_PCIEPORTBUS)	+= pcieportdrv.o

obj-$(CONFIG_PCIEASPM)		+= aspm.o
obj-$(CONFIG_PCIEAER)		+= aer.o
obj-$(CONFIG_PCIEAER_INJECT)	+= aer_inject.o
obj-$(CONFIG_PCIE_PME)		+= pme.o
obj-$(CONFIG_PCIE_DPC)		+= dpc.o
obj-$(CONFIG_PCIE_PTM)		+= ptm.o
<<<<<<< HEAD
=======
obj-$(CONFIG_PCIE_EDR)		+= edr.o
>>>>>>> upstream/android-13
