<<<<<<< HEAD

vfio-pci-y := vfio_pci.o vfio_pci_intrs.o vfio_pci_rdwr.o vfio_pci_config.o
vfio-pci-$(CONFIG_VFIO_PCI_IGD) += vfio_pci_igd.o

=======
# SPDX-License-Identifier: GPL-2.0-only

vfio-pci-core-y := vfio_pci_core.o vfio_pci_intrs.o vfio_pci_rdwr.o vfio_pci_config.o
vfio-pci-core-$(CONFIG_S390) += vfio_pci_zdev.o
obj-$(CONFIG_VFIO_PCI_CORE) += vfio-pci-core.o

vfio-pci-y := vfio_pci.o
vfio-pci-$(CONFIG_VFIO_PCI_IGD) += vfio_pci_igd.o
>>>>>>> upstream/android-13
obj-$(CONFIG_VFIO_PCI) += vfio-pci.o
