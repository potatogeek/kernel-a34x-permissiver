<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  lpc_sch.c - LPC interface for Intel Poulsbo SCH
 *
 *  LPC bridge function of the Intel SCH contains many other
 *  functional units, such as Interrupt controllers, Timers,
 *  Power Management, System Management, GPIO, RTC, and LPC
 *  Configuration Registers.
 *
 *  Copyright (c) 2010 CompuLab Ltd
 *  Copyright (c) 2014 Intel Corp.
 *  Author: Denis Turischev <denis@compulab.co.il>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License 2 as published
 *  by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/acpi.h>
#include <linux/pci.h>
#include <linux/mfd/core.h>

#define SMBASE		0x40
#define SMBUS_IO_SIZE	64

<<<<<<< HEAD
#define GPIOBASE	0x44
#define GPIO_IO_SIZE	64
#define GPIO_IO_SIZE_CENTERTON	128

/* Intel Quark X1000 GPIO IRQ Number */
#define GPIO_IRQ_QUARK_X1000	9

=======
#define GPIO_BASE	0x44
#define GPIO_IO_SIZE	64
#define GPIO_IO_SIZE_CENTERTON	128

>>>>>>> upstream/android-13
#define WDTBASE		0x84
#define WDT_IO_SIZE	64

enum sch_chipsets {
	LPC_SCH = 0,		/* Intel Poulsbo SCH */
	LPC_ITC,		/* Intel Tunnel Creek */
	LPC_CENTERTON,		/* Intel Centerton */
	LPC_QUARK_X1000,	/* Intel Quark X1000 */
};

struct lpc_sch_info {
	unsigned int io_size_smbus;
	unsigned int io_size_gpio;
	unsigned int io_size_wdt;
<<<<<<< HEAD
	int irq_gpio;
=======
>>>>>>> upstream/android-13
};

static struct lpc_sch_info sch_chipset_info[] = {
	[LPC_SCH] = {
		.io_size_smbus = SMBUS_IO_SIZE,
		.io_size_gpio = GPIO_IO_SIZE,
<<<<<<< HEAD
		.irq_gpio = -1,
=======
>>>>>>> upstream/android-13
	},
	[LPC_ITC] = {
		.io_size_smbus = SMBUS_IO_SIZE,
		.io_size_gpio = GPIO_IO_SIZE,
		.io_size_wdt = WDT_IO_SIZE,
<<<<<<< HEAD
		.irq_gpio = -1,
=======
>>>>>>> upstream/android-13
	},
	[LPC_CENTERTON] = {
		.io_size_smbus = SMBUS_IO_SIZE,
		.io_size_gpio = GPIO_IO_SIZE_CENTERTON,
		.io_size_wdt = WDT_IO_SIZE,
<<<<<<< HEAD
		.irq_gpio = -1,
	},
	[LPC_QUARK_X1000] = {
		.io_size_gpio = GPIO_IO_SIZE,
		.irq_gpio = GPIO_IRQ_QUARK_X1000,
=======
	},
	[LPC_QUARK_X1000] = {
		.io_size_gpio = GPIO_IO_SIZE,
>>>>>>> upstream/android-13
		.io_size_wdt = WDT_IO_SIZE,
	},
};

static const struct pci_device_id lpc_sch_ids[] = {
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_SCH_LPC), LPC_SCH },
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_ITC_LPC), LPC_ITC },
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_CENTERTON_ILB), LPC_CENTERTON },
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_QUARK_X1000_ILB), LPC_QUARK_X1000 },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, lpc_sch_ids);

#define LPC_NO_RESOURCE		1
#define LPC_SKIP_RESOURCE	2

static int lpc_sch_get_io(struct pci_dev *pdev, int where, const char *name,
			  struct resource *res, int size)
{
	unsigned int base_addr_cfg;
	unsigned short base_addr;

	if (size == 0)
		return LPC_NO_RESOURCE;

	pci_read_config_dword(pdev, where, &base_addr_cfg);
	base_addr = 0;
	if (!(base_addr_cfg & (1 << 31)))
		dev_warn(&pdev->dev, "Decode of the %s I/O range disabled\n",
			 name);
	else
		base_addr = (unsigned short)base_addr_cfg;

	if (base_addr == 0) {
		dev_warn(&pdev->dev, "I/O space for %s uninitialized\n", name);
		return LPC_SKIP_RESOURCE;
	}

	res->start = base_addr;
	res->end = base_addr + size - 1;
	res->flags = IORESOURCE_IO;

	return 0;
}

static int lpc_sch_populate_cell(struct pci_dev *pdev, int where,
<<<<<<< HEAD
				 const char *name, int size, int irq,
				 int id, struct mfd_cell *cell)
=======
				 const char *name, int size, int id,
				 struct mfd_cell *cell)
>>>>>>> upstream/android-13
{
	struct resource *res;
	int ret;

<<<<<<< HEAD
	res = devm_kcalloc(&pdev->dev, 2, sizeof(*res), GFP_KERNEL);
=======
	res = devm_kzalloc(&pdev->dev, sizeof(*res), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!res)
		return -ENOMEM;

	ret = lpc_sch_get_io(pdev, where, name, res, size);
	if (ret)
		return ret;

	memset(cell, 0, sizeof(*cell));

	cell->name = name;
	cell->resources = res;
	cell->num_resources = 1;
	cell->ignore_resource_conflicts = true;
	cell->id = id;

<<<<<<< HEAD
	/* Check if we need to add an IRQ resource */
	if (irq < 0)
		return 0;

	res++;

	res->start = irq;
	res->end = irq;
	res->flags = IORESOURCE_IRQ;

	cell->num_resources++;

=======
>>>>>>> upstream/android-13
	return 0;
}

static int lpc_sch_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	struct mfd_cell lpc_sch_cells[3];
	struct lpc_sch_info *info = &sch_chipset_info[id->driver_data];
	unsigned int cells = 0;
	int ret;

	ret = lpc_sch_populate_cell(dev, SMBASE, "isch_smbus",
<<<<<<< HEAD
				    info->io_size_smbus, -1,
=======
				    info->io_size_smbus,
>>>>>>> upstream/android-13
				    id->device, &lpc_sch_cells[cells]);
	if (ret < 0)
		return ret;
	if (ret == 0)
		cells++;

<<<<<<< HEAD
	ret = lpc_sch_populate_cell(dev, GPIOBASE, "sch_gpio",
				    info->io_size_gpio, info->irq_gpio,
=======
	ret = lpc_sch_populate_cell(dev, GPIO_BASE, "sch_gpio",
				    info->io_size_gpio,
>>>>>>> upstream/android-13
				    id->device, &lpc_sch_cells[cells]);
	if (ret < 0)
		return ret;
	if (ret == 0)
		cells++;

	ret = lpc_sch_populate_cell(dev, WDTBASE, "ie6xx_wdt",
<<<<<<< HEAD
				    info->io_size_wdt, -1,
=======
				    info->io_size_wdt,
>>>>>>> upstream/android-13
				    id->device, &lpc_sch_cells[cells]);
	if (ret < 0)
		return ret;
	if (ret == 0)
		cells++;

	if (cells == 0) {
		dev_err(&dev->dev, "All decode registers disabled.\n");
		return -ENODEV;
	}

	return mfd_add_devices(&dev->dev, 0, lpc_sch_cells, cells, NULL, 0, NULL);
}

static void lpc_sch_remove(struct pci_dev *dev)
{
	mfd_remove_devices(&dev->dev);
}

static struct pci_driver lpc_sch_driver = {
	.name		= "lpc_sch",
	.id_table	= lpc_sch_ids,
	.probe		= lpc_sch_probe,
	.remove		= lpc_sch_remove,
};

module_pci_driver(lpc_sch_driver);

MODULE_AUTHOR("Denis Turischev <denis@compulab.co.il>");
MODULE_DESCRIPTION("LPC interface for Intel Poulsbo SCH");
MODULE_LICENSE("GPL");
