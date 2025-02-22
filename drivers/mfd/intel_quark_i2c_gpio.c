<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Intel Quark MFD PCI driver for I2C & GPIO
 *
 * Copyright(c) 2014 Intel Corporation.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
=======
>>>>>>> upstream/android-13
 * Intel Quark PCI device for I2C and GPIO controller sharing the same
 * PCI function. This PCI driver will split the 2 devices into their
 * respective drivers.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/mfd/core.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/dmi.h>
<<<<<<< HEAD
#include <linux/platform_data/gpio-dwapb.h>
#include <linux/platform_data/i2c-designware.h>
=======
#include <linux/i2c.h>
#include <linux/property.h>
>>>>>>> upstream/android-13

/* PCI BAR for register base address */
#define MFD_I2C_BAR		0
#define MFD_GPIO_BAR		1

/* ACPI _ADR value to match the child node */
#define MFD_ACPI_MATCH_GPIO	0ULL
#define MFD_ACPI_MATCH_I2C	1ULL

<<<<<<< HEAD
/* The base GPIO number under GPIOLIB framework */
#define INTEL_QUARK_MFD_GPIO_BASE	8

/* The default number of South-Cluster GPIO on Quark. */
#define INTEL_QUARK_MFD_NGPIO		8

/* The DesignWare GPIO ports on Quark. */
#define INTEL_QUARK_GPIO_NPORTS	1

=======
>>>>>>> upstream/android-13
#define INTEL_QUARK_IORES_MEM	0
#define INTEL_QUARK_IORES_IRQ	1

#define INTEL_QUARK_I2C_CONTROLLER_CLK "i2c_designware.0"

/* The Quark I2C controller source clock */
#define INTEL_QUARK_I2C_CLK_HZ	33000000

struct intel_quark_mfd {
<<<<<<< HEAD
	struct device		*dev;
=======
>>>>>>> upstream/android-13
	struct clk		*i2c_clk;
	struct clk_lookup	*i2c_clk_lookup;
};

<<<<<<< HEAD
=======
static const struct property_entry intel_quark_i2c_controller_standard_properties[] = {
	PROPERTY_ENTRY_U32("clock-frequency", I2C_MAX_STANDARD_MODE_FREQ),
	{ }
};

static const struct software_node intel_quark_i2c_controller_standard_node = {
	.name = "intel-quark-i2c-controller",
	.properties = intel_quark_i2c_controller_standard_properties,
};

static const struct property_entry intel_quark_i2c_controller_fast_properties[] = {
	PROPERTY_ENTRY_U32("clock-frequency", I2C_MAX_FAST_MODE_FREQ),
	{ }
};

static const struct software_node intel_quark_i2c_controller_fast_node = {
	.name = "intel-quark-i2c-controller",
	.properties = intel_quark_i2c_controller_fast_properties,
};

>>>>>>> upstream/android-13
static const struct dmi_system_id dmi_platform_info[] = {
	{
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "Galileo"),
		},
<<<<<<< HEAD
		.driver_data = (void *)100000,
=======
		.driver_data = (void *)&intel_quark_i2c_controller_standard_node,
>>>>>>> upstream/android-13
	},
	{
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "GalileoGen2"),
		},
<<<<<<< HEAD
		.driver_data = (void *)400000,
=======
		.driver_data = (void *)&intel_quark_i2c_controller_fast_node,
>>>>>>> upstream/android-13
	},
	{
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "SIMATIC IOT2000"),
<<<<<<< HEAD
			DMI_EXACT_MATCH(DMI_BOARD_ASSET_TAG,
					"6ES7647-0AA00-0YA2"),
		},
		.driver_data = (void *)400000,
	},
	{
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "SIMATIC IOT2000"),
			DMI_EXACT_MATCH(DMI_BOARD_ASSET_TAG,
					"6ES7647-0AA00-1YA2"),
		},
		.driver_data = (void *)400000,
=======
		},
		.driver_data = (void *)&intel_quark_i2c_controller_fast_node,
>>>>>>> upstream/android-13
	},
	{}
};

<<<<<<< HEAD
=======
/* This is used as a place holder and will be modified at run-time */
>>>>>>> upstream/android-13
static struct resource intel_quark_i2c_res[] = {
	[INTEL_QUARK_IORES_MEM] = {
		.flags = IORESOURCE_MEM,
	},
	[INTEL_QUARK_IORES_IRQ] = {
		.flags = IORESOURCE_IRQ,
	},
};

static struct mfd_cell_acpi_match intel_quark_acpi_match_i2c = {
	.adr = MFD_ACPI_MATCH_I2C,
};

<<<<<<< HEAD
=======
/* This is used as a place holder and will be modified at run-time */
>>>>>>> upstream/android-13
static struct resource intel_quark_gpio_res[] = {
	[INTEL_QUARK_IORES_MEM] = {
		.flags = IORESOURCE_MEM,
	},
<<<<<<< HEAD
=======
	[INTEL_QUARK_IORES_IRQ] = {
		.flags = IORESOURCE_IRQ,
	},
>>>>>>> upstream/android-13
};

static struct mfd_cell_acpi_match intel_quark_acpi_match_gpio = {
	.adr = MFD_ACPI_MATCH_GPIO,
};

<<<<<<< HEAD
static struct mfd_cell intel_quark_mfd_cells[] = {
	{
		.id = MFD_GPIO_BAR,
		.name = "gpio-dwapb",
		.acpi_match = &intel_quark_acpi_match_gpio,
		.num_resources = ARRAY_SIZE(intel_quark_gpio_res),
		.resources = intel_quark_gpio_res,
		.ignore_resource_conflicts = true,
	},
	{
=======
static const struct software_node intel_quark_gpio_controller_node = {
	.name = "intel-quark-gpio-controller",
};

static const struct property_entry intel_quark_gpio_portA_properties[] = {
	PROPERTY_ENTRY_U32("reg", 0),
	PROPERTY_ENTRY_U32("snps,nr-gpios", 8),
	PROPERTY_ENTRY_U32("gpio-base", 8),
	{ }
};

static const struct software_node intel_quark_gpio_portA_node = {
	.name = "portA",
	.parent = &intel_quark_gpio_controller_node,
	.properties = intel_quark_gpio_portA_properties,
};

static const struct software_node *intel_quark_gpio_node_group[] = {
	&intel_quark_gpio_controller_node,
	&intel_quark_gpio_portA_node,
	NULL
};

static struct mfd_cell intel_quark_mfd_cells[] = {
	[MFD_I2C_BAR] = {
>>>>>>> upstream/android-13
		.id = MFD_I2C_BAR,
		.name = "i2c_designware",
		.acpi_match = &intel_quark_acpi_match_i2c,
		.num_resources = ARRAY_SIZE(intel_quark_i2c_res),
		.resources = intel_quark_i2c_res,
		.ignore_resource_conflicts = true,
	},
<<<<<<< HEAD
=======
	[MFD_GPIO_BAR] = {
		.id = MFD_GPIO_BAR,
		.name = "gpio-dwapb",
		.acpi_match = &intel_quark_acpi_match_gpio,
		.num_resources = ARRAY_SIZE(intel_quark_gpio_res),
		.resources = intel_quark_gpio_res,
		.ignore_resource_conflicts = true,
	},
>>>>>>> upstream/android-13
};

static const struct pci_device_id intel_quark_mfd_ids[] = {
	{ PCI_VDEVICE(INTEL, 0x0934), },
	{},
};
MODULE_DEVICE_TABLE(pci, intel_quark_mfd_ids);

static int intel_quark_register_i2c_clk(struct device *dev)
{
	struct intel_quark_mfd *quark_mfd = dev_get_drvdata(dev);
	struct clk *i2c_clk;

	i2c_clk = clk_register_fixed_rate(dev,
					  INTEL_QUARK_I2C_CONTROLLER_CLK, NULL,
					  0, INTEL_QUARK_I2C_CLK_HZ);
	if (IS_ERR(i2c_clk))
		return PTR_ERR(i2c_clk);

	quark_mfd->i2c_clk = i2c_clk;
	quark_mfd->i2c_clk_lookup = clkdev_create(i2c_clk, NULL,
						INTEL_QUARK_I2C_CONTROLLER_CLK);

	if (!quark_mfd->i2c_clk_lookup) {
		clk_unregister(quark_mfd->i2c_clk);
		dev_err(dev, "Fixed clk register failed\n");
		return -ENOMEM;
	}

	return 0;
}

static void intel_quark_unregister_i2c_clk(struct device *dev)
{
	struct intel_quark_mfd *quark_mfd = dev_get_drvdata(dev);

	if (!quark_mfd->i2c_clk_lookup)
		return;

	clkdev_drop(quark_mfd->i2c_clk_lookup);
	clk_unregister(quark_mfd->i2c_clk);
}

<<<<<<< HEAD
static int intel_quark_i2c_setup(struct pci_dev *pdev, struct mfd_cell *cell)
{
	const struct dmi_system_id *dmi_id;
	struct dw_i2c_platform_data *pdata;
	struct resource *res = (struct resource *)cell->resources;
	struct device *dev = &pdev->dev;

	res[INTEL_QUARK_IORES_MEM].start =
		pci_resource_start(pdev, MFD_I2C_BAR);
	res[INTEL_QUARK_IORES_MEM].end =
		pci_resource_end(pdev, MFD_I2C_BAR);

	res[INTEL_QUARK_IORES_IRQ].start = pdev->irq;
	res[INTEL_QUARK_IORES_IRQ].end = pdev->irq;

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	/* Normal mode by default */
	pdata->i2c_scl_freq = 100000;

	dmi_id = dmi_first_match(dmi_platform_info);
	if (dmi_id)
		pdata->i2c_scl_freq = (uintptr_t)dmi_id->driver_data;

	cell->platform_data = pdata;
	cell->pdata_size = sizeof(*pdata);
=======
static int intel_quark_i2c_setup(struct pci_dev *pdev)
{
	struct mfd_cell *cell = &intel_quark_mfd_cells[MFD_I2C_BAR];
	struct resource *res = intel_quark_i2c_res;
	const struct dmi_system_id *dmi_id;

	res[INTEL_QUARK_IORES_MEM].start = pci_resource_start(pdev, MFD_I2C_BAR);
	res[INTEL_QUARK_IORES_MEM].end = pci_resource_end(pdev, MFD_I2C_BAR);

	res[INTEL_QUARK_IORES_IRQ].start = pci_irq_vector(pdev, 0);
	res[INTEL_QUARK_IORES_IRQ].end = pci_irq_vector(pdev, 0);

	/* Normal mode by default */
	cell->swnode = &intel_quark_i2c_controller_standard_node;

	dmi_id = dmi_first_match(dmi_platform_info);
	if (dmi_id)
		cell->swnode = (struct software_node *)dmi_id->driver_data;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int intel_quark_gpio_setup(struct pci_dev *pdev, struct mfd_cell *cell)
{
	struct dwapb_platform_data *pdata;
	struct resource *res = (struct resource *)cell->resources;
	struct device *dev = &pdev->dev;

	res[INTEL_QUARK_IORES_MEM].start =
		pci_resource_start(pdev, MFD_GPIO_BAR);
	res[INTEL_QUARK_IORES_MEM].end =
		pci_resource_end(pdev, MFD_GPIO_BAR);

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	/* For intel quark x1000, it has only one port: portA */
	pdata->nports = INTEL_QUARK_GPIO_NPORTS;
	pdata->properties = devm_kcalloc(dev, pdata->nports,
					 sizeof(*pdata->properties),
					 GFP_KERNEL);
	if (!pdata->properties)
		return -ENOMEM;

	/* Set the properties for portA */
	pdata->properties->fwnode	= NULL;
	pdata->properties->idx		= 0;
	pdata->properties->ngpio	= INTEL_QUARK_MFD_NGPIO;
	pdata->properties->gpio_base	= INTEL_QUARK_MFD_GPIO_BASE;
	pdata->properties->irq[0]	= pdev->irq;
	pdata->properties->has_irq	= true;
	pdata->properties->irq_shared	= true;

	cell->platform_data = pdata;
	cell->pdata_size = sizeof(*pdata);

=======
static int intel_quark_gpio_setup(struct pci_dev *pdev)
{
	struct mfd_cell *cell = &intel_quark_mfd_cells[MFD_GPIO_BAR];
	struct resource *res = intel_quark_gpio_res;
	int ret;

	res[INTEL_QUARK_IORES_MEM].start = pci_resource_start(pdev, MFD_GPIO_BAR);
	res[INTEL_QUARK_IORES_MEM].end = pci_resource_end(pdev, MFD_GPIO_BAR);

	res[INTEL_QUARK_IORES_IRQ].start = pci_irq_vector(pdev, 0);
	res[INTEL_QUARK_IORES_IRQ].end = pci_irq_vector(pdev, 0);

	ret = software_node_register_node_group(intel_quark_gpio_node_group);
	if (ret)
		return ret;

	cell->swnode = &intel_quark_gpio_controller_node;
>>>>>>> upstream/android-13
	return 0;
}

static int intel_quark_mfd_probe(struct pci_dev *pdev,
				 const struct pci_device_id *id)
{
	struct intel_quark_mfd *quark_mfd;
	int ret;

	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;

	quark_mfd = devm_kzalloc(&pdev->dev, sizeof(*quark_mfd), GFP_KERNEL);
	if (!quark_mfd)
		return -ENOMEM;

<<<<<<< HEAD
	quark_mfd->dev = &pdev->dev;
=======
>>>>>>> upstream/android-13
	dev_set_drvdata(&pdev->dev, quark_mfd);

	ret = intel_quark_register_i2c_clk(&pdev->dev);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = intel_quark_i2c_setup(pdev, &intel_quark_mfd_cells[1]);
	if (ret)
		goto err_unregister_i2c_clk;

	ret = intel_quark_gpio_setup(pdev, &intel_quark_mfd_cells[0]);
	if (ret)
		goto err_unregister_i2c_clk;
=======
	pci_set_master(pdev);

	/* This driver only requires 1 IRQ vector */
	ret = pci_alloc_irq_vectors(pdev, 1, 1, PCI_IRQ_ALL_TYPES);
	if (ret < 0)
		goto err_unregister_i2c_clk;

	ret = intel_quark_i2c_setup(pdev);
	if (ret)
		goto err_free_irq_vectors;

	ret = intel_quark_gpio_setup(pdev);
	if (ret)
		goto err_free_irq_vectors;
>>>>>>> upstream/android-13

	ret = mfd_add_devices(&pdev->dev, 0, intel_quark_mfd_cells,
			      ARRAY_SIZE(intel_quark_mfd_cells), NULL, 0,
			      NULL);
	if (ret)
<<<<<<< HEAD
		goto err_unregister_i2c_clk;

	return 0;

=======
		goto err_unregister_gpio_node_group;

	return 0;

err_unregister_gpio_node_group:
	software_node_unregister_node_group(intel_quark_gpio_node_group);
err_free_irq_vectors:
	pci_free_irq_vectors(pdev);
>>>>>>> upstream/android-13
err_unregister_i2c_clk:
	intel_quark_unregister_i2c_clk(&pdev->dev);
	return ret;
}

static void intel_quark_mfd_remove(struct pci_dev *pdev)
{
<<<<<<< HEAD
	intel_quark_unregister_i2c_clk(&pdev->dev);
	mfd_remove_devices(&pdev->dev);
=======
	mfd_remove_devices(&pdev->dev);
	software_node_unregister_node_group(intel_quark_gpio_node_group);
	pci_free_irq_vectors(pdev);
	intel_quark_unregister_i2c_clk(&pdev->dev);
>>>>>>> upstream/android-13
}

static struct pci_driver intel_quark_mfd_driver = {
	.name		= "intel_quark_mfd_i2c_gpio",
	.id_table	= intel_quark_mfd_ids,
	.probe		= intel_quark_mfd_probe,
	.remove		= intel_quark_mfd_remove,
};

module_pci_driver(intel_quark_mfd_driver);

MODULE_AUTHOR("Raymond Tan <raymond.tan@intel.com>");
MODULE_DESCRIPTION("Intel Quark MFD PCI driver for I2C & GPIO");
MODULE_LICENSE("GPL v2");
