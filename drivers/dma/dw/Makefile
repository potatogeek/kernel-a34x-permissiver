# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_DW_DMAC_CORE)	+= dw_dmac_core.o
<<<<<<< HEAD
dw_dmac_core-objs	:= core.o

obj-$(CONFIG_DW_DMAC)		+= dw_dmac.o
dw_dmac-objs		:= platform.o

obj-$(CONFIG_DW_DMAC_PCI)	+= dw_dmac_pci.o
dw_dmac_pci-objs	:= pci.o
=======
dw_dmac_core-y			:= core.o dw.o idma32.o
dw_dmac_core-$(CONFIG_ACPI)	+= acpi.o

obj-$(CONFIG_DW_DMAC)		+= dw_dmac.o
dw_dmac-y			:= platform.o
dw_dmac-$(CONFIG_OF)		+= of.o

obj-$(CONFIG_DW_DMAC_PCI)	+= dw_dmac_pci.o
dw_dmac_pci-y			:= pci.o
>>>>>>> upstream/android-13
