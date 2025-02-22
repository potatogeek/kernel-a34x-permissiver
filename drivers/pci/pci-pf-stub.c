// SPDX-License-Identifier: GPL-2.0
/* pci-pf-stub - simple stub driver for PCI SR-IOV PF device
 *
<<<<<<< HEAD
 * This driver is meant to act as a "whitelist" for devices that provde
=======
 * This driver is meant to act as a "whitelist" for devices that provide
>>>>>>> upstream/android-13
 * SR-IOV functionality while at the same time not actually needing a
 * driver of their own.
 */

#include <linux/module.h>
#include <linux/pci.h>

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * pci_pf_stub_whitelist - White list of devices to bind pci-pf-stub onto
 *
 * This table provides the list of IDs this driver is supposed to bind
 * onto.  You could think of this as a list of "quirked" devices where we
 * are adding support for SR-IOV here since there are no other drivers
 * that they would be running under.
 */
static const struct pci_device_id pci_pf_stub_whitelist[] = {
	{ PCI_VDEVICE(AMAZON, 0x0053) },
	/* required last entry */
	{ 0 }
};
MODULE_DEVICE_TABLE(pci, pci_pf_stub_whitelist);

static int pci_pf_stub_probe(struct pci_dev *dev,
			     const struct pci_device_id *id)
{
	pci_info(dev, "claimed by pci-pf-stub\n");
	return 0;
}

static struct pci_driver pf_stub_driver = {
	.name			= "pci-pf-stub",
	.id_table		= pci_pf_stub_whitelist,
	.probe			= pci_pf_stub_probe,
	.sriov_configure	= pci_sriov_configure_simple,
};
<<<<<<< HEAD

static int __init pci_pf_stub_init(void)
{
	return pci_register_driver(&pf_stub_driver);
}

static void __exit pci_pf_stub_exit(void)
{
	pci_unregister_driver(&pf_stub_driver);
}

module_init(pci_pf_stub_init);
module_exit(pci_pf_stub_exit);
=======
module_pci_driver(pf_stub_driver);
>>>>>>> upstream/android-13

MODULE_LICENSE("GPL");
