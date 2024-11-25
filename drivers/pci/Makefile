# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the PCI bus specific drivers.

obj-$(CONFIG_PCI)		+= access.o bus.o probe.o host-bridge.o \
				   remove.o pci.o pci-driver.o search.o \
				   pci-sysfs.o rom.o setup-res.o irq.o vpd.o \
<<<<<<< HEAD
				   setup-bus.o vc.o mmap.o setup-irq.o
=======
				   setup-bus.o vc.o mmap.o setup-irq.o msi.o

obj-$(CONFIG_PCI)		+= pcie/
>>>>>>> upstream/android-13

ifdef CONFIG_PCI
obj-$(CONFIG_PROC_FS)		+= proc.o
obj-$(CONFIG_SYSFS)		+= slot.o
<<<<<<< HEAD
obj-$(CONFIG_OF)		+= of.o
endif

obj-$(CONFIG_PCI_QUIRKS)	+= quirks.o
obj-$(CONFIG_PCIEPORTBUS)	+= pcie/
obj-$(CONFIG_HOTPLUG_PCI)	+= hotplug/
obj-$(CONFIG_PCI_MSI)		+= msi.o
obj-$(CONFIG_PCI_ATS)		+= ats.o
obj-$(CONFIG_PCI_IOV)		+= iov.o
obj-$(CONFIG_ACPI)		+= pci-acpi.o
=======
obj-$(CONFIG_ACPI)		+= pci-acpi.o
endif

obj-$(CONFIG_OF)		+= of.o
obj-$(CONFIG_PCI_QUIRKS)	+= quirks.o
obj-$(CONFIG_HOTPLUG_PCI)	+= hotplug/
obj-$(CONFIG_PCI_ATS)		+= ats.o
obj-$(CONFIG_PCI_IOV)		+= iov.o
obj-$(CONFIG_PCI_BRIDGE_EMUL)	+= pci-bridge-emul.o
>>>>>>> upstream/android-13
obj-$(CONFIG_PCI_LABEL)		+= pci-label.o
obj-$(CONFIG_X86_INTEL_MID)	+= pci-mid.o
obj-$(CONFIG_PCI_SYSCALL)	+= syscall.o
obj-$(CONFIG_PCI_STUB)		+= pci-stub.o
obj-$(CONFIG_PCI_PF_STUB)	+= pci-pf-stub.o
obj-$(CONFIG_PCI_ECAM)		+= ecam.o
<<<<<<< HEAD
=======
obj-$(CONFIG_PCI_P2PDMA)	+= p2pdma.o
>>>>>>> upstream/android-13
obj-$(CONFIG_XEN_PCIDEV_FRONTEND) += xen-pcifront.o

# Endpoint library must be initialized before its users
obj-$(CONFIG_PCI_ENDPOINT)	+= endpoint/

obj-y				+= controller/
obj-y				+= switch/

<<<<<<< HEAD
ccflags-$(CONFIG_PCI_DEBUG) := -DDEBUG
=======
subdir-ccflags-$(CONFIG_PCI_DEBUG) := -DDEBUG
>>>>>>> upstream/android-13
