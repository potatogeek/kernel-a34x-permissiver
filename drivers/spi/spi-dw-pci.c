<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * PCI interface driver for DW SPI Core
 *
 * Copyright (c) 2009, 2014 Intel Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include <linux/interrupt.h>
#include <linux/pci.h>
=======
 */

#include <linux/pci.h>
#include <linux/pm_runtime.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/spi/spi.h>
#include <linux/module.h>

#include "spi-dw.h"

#define DRIVER_NAME "dw_spi_pci"

<<<<<<< HEAD
=======
/* HW info for MRST Clk Control Unit, 32b reg per controller */
#define MRST_SPI_CLK_BASE	100000000	/* 100m */
#define MRST_CLK_SPI_REG	0xff11d86c
#define CLK_SPI_BDIV_OFFSET	0
#define CLK_SPI_BDIV_MASK	0x00000007
#define CLK_SPI_CDIV_OFFSET	9
#define CLK_SPI_CDIV_MASK	0x00000e00
#define CLK_SPI_DISABLE_OFFSET	8

>>>>>>> upstream/android-13
struct spi_pci_desc {
	int	(*setup)(struct dw_spi *);
	u16	num_cs;
	u16	bus_num;
<<<<<<< HEAD
};

static struct spi_pci_desc spi_pci_mid_desc_1 = {
	.setup = dw_spi_mid_init,
=======
	u32	max_freq;
};

static int spi_mid_init(struct dw_spi *dws)
{
	void __iomem *clk_reg;
	u32 clk_cdiv;

	clk_reg = ioremap(MRST_CLK_SPI_REG, 16);
	if (!clk_reg)
		return -ENOMEM;

	/* Get SPI controller operating freq info */
	clk_cdiv = readl(clk_reg + dws->bus_num * sizeof(u32));
	clk_cdiv &= CLK_SPI_CDIV_MASK;
	clk_cdiv >>= CLK_SPI_CDIV_OFFSET;
	dws->max_freq = MRST_SPI_CLK_BASE / (clk_cdiv + 1);

	iounmap(clk_reg);

	dw_spi_dma_setup_mfld(dws);

	return 0;
}

static int spi_generic_init(struct dw_spi *dws)
{
	dw_spi_dma_setup_generic(dws);

	return 0;
}

static struct spi_pci_desc spi_pci_mid_desc_1 = {
	.setup = spi_mid_init,
>>>>>>> upstream/android-13
	.num_cs = 5,
	.bus_num = 0,
};

static struct spi_pci_desc spi_pci_mid_desc_2 = {
<<<<<<< HEAD
	.setup = dw_spi_mid_init,
=======
	.setup = spi_mid_init,
>>>>>>> upstream/android-13
	.num_cs = 2,
	.bus_num = 1,
};

<<<<<<< HEAD
=======
static struct spi_pci_desc spi_pci_ehl_desc = {
	.setup = spi_generic_init,
	.num_cs = 2,
	.bus_num = -1,
	.max_freq = 100000000,
};

>>>>>>> upstream/android-13
static int spi_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct dw_spi *dws;
	struct spi_pci_desc *desc = (struct spi_pci_desc *)ent->driver_data;
	int pci_bar = 0;
	int ret;

	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;

	dws = devm_kzalloc(&pdev->dev, sizeof(*dws), GFP_KERNEL);
	if (!dws)
		return -ENOMEM;

	/* Get basic io resource and map it */
	dws->paddr = pci_resource_start(pdev, pci_bar);
<<<<<<< HEAD
=======
	pci_set_master(pdev);
>>>>>>> upstream/android-13

	ret = pcim_iomap_regions(pdev, 1 << pci_bar, pci_name(pdev));
	if (ret)
		return ret;

<<<<<<< HEAD
	dws->regs = pcim_iomap_table(pdev)[pci_bar];
	dws->irq = pdev->irq;
=======
	ret = pci_alloc_irq_vectors(pdev, 1, 1, PCI_IRQ_ALL_TYPES);
	if (ret < 0)
		return ret;

	dws->regs = pcim_iomap_table(pdev)[pci_bar];
	dws->irq = pci_irq_vector(pdev, 0);
>>>>>>> upstream/android-13

	/*
	 * Specific handling for platforms, like dma setup,
	 * clock rate, FIFO depth.
	 */
	if (desc) {
		dws->num_cs = desc->num_cs;
		dws->bus_num = desc->bus_num;
<<<<<<< HEAD
=======
		dws->max_freq = desc->max_freq;
>>>>>>> upstream/android-13

		if (desc->setup) {
			ret = desc->setup(dws);
			if (ret)
<<<<<<< HEAD
				return ret;
		}
	} else {
		return -ENODEV;
=======
				goto err_free_irq_vectors;
		}
	} else {
		ret = -ENODEV;
		goto err_free_irq_vectors;
>>>>>>> upstream/android-13
	}

	ret = dw_spi_add_host(&pdev->dev, dws);
	if (ret)
<<<<<<< HEAD
		return ret;
=======
		goto err_free_irq_vectors;
>>>>>>> upstream/android-13

	/* PCI hook and SPI hook use the same drv data */
	pci_set_drvdata(pdev, dws);

	dev_info(&pdev->dev, "found PCI SPI controller(ID: %04x:%04x)\n",
		pdev->vendor, pdev->device);

<<<<<<< HEAD
	return 0;
=======
	pm_runtime_set_autosuspend_delay(&pdev->dev, 1000);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_put_autosuspend(&pdev->dev);
	pm_runtime_allow(&pdev->dev);

	return 0;

err_free_irq_vectors:
	pci_free_irq_vectors(pdev);
	return ret;
>>>>>>> upstream/android-13
}

static void spi_pci_remove(struct pci_dev *pdev)
{
	struct dw_spi *dws = pci_get_drvdata(pdev);

<<<<<<< HEAD
	dw_spi_remove_host(dws);
=======
	pm_runtime_forbid(&pdev->dev);
	pm_runtime_get_noresume(&pdev->dev);

	dw_spi_remove_host(dws);
	pci_free_irq_vectors(pdev);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_PM_SLEEP
static int spi_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct pci_dev *pdev = to_pci_dev(dev);
	struct dw_spi *dws = pci_get_drvdata(pdev);
=======
	struct dw_spi *dws = dev_get_drvdata(dev);
>>>>>>> upstream/android-13

	return dw_spi_suspend_host(dws);
}

static int spi_resume(struct device *dev)
{
<<<<<<< HEAD
	struct pci_dev *pdev = to_pci_dev(dev);
	struct dw_spi *dws = pci_get_drvdata(pdev);
=======
	struct dw_spi *dws = dev_get_drvdata(dev);
>>>>>>> upstream/android-13

	return dw_spi_resume_host(dws);
}
#endif

static SIMPLE_DEV_PM_OPS(dw_spi_pm_ops, spi_suspend, spi_resume);

static const struct pci_device_id pci_ids[] = {
	/* Intel MID platform SPI controller 0 */
	/*
	 * The access to the device 8086:0801 is disabled by HW, since it's
	 * exclusively used by SCU to communicate with MSIC.
	 */
	/* Intel MID platform SPI controller 1 */
	{ PCI_VDEVICE(INTEL, 0x0800), (kernel_ulong_t)&spi_pci_mid_desc_1},
	/* Intel MID platform SPI controller 2 */
	{ PCI_VDEVICE(INTEL, 0x0812), (kernel_ulong_t)&spi_pci_mid_desc_2},
<<<<<<< HEAD
	{},
};
=======
	/* Intel Elkhart Lake PSE SPI controllers */
	{ PCI_VDEVICE(INTEL, 0x4b84), (kernel_ulong_t)&spi_pci_ehl_desc},
	{ PCI_VDEVICE(INTEL, 0x4b85), (kernel_ulong_t)&spi_pci_ehl_desc},
	{ PCI_VDEVICE(INTEL, 0x4b86), (kernel_ulong_t)&spi_pci_ehl_desc},
	{ PCI_VDEVICE(INTEL, 0x4b87), (kernel_ulong_t)&spi_pci_ehl_desc},
	{},
};
MODULE_DEVICE_TABLE(pci, pci_ids);
>>>>>>> upstream/android-13

static struct pci_driver dw_spi_driver = {
	.name =		DRIVER_NAME,
	.id_table =	pci_ids,
	.probe =	spi_pci_probe,
	.remove =	spi_pci_remove,
	.driver         = {
		.pm     = &dw_spi_pm_ops,
	},
};

module_pci_driver(dw_spi_driver);

MODULE_AUTHOR("Feng Tang <feng.tang@intel.com>");
MODULE_DESCRIPTION("PCI interface driver for DW SPI Core");
MODULE_LICENSE("GPL v2");
