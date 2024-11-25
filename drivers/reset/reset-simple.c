<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Simple Reset Controller Driver
 *
 * Copyright (C) 2017 Pengutronix, Philipp Zabel <kernel@pengutronix.de>
 *
 * Based on Allwinner SoCs Reset Controller driver
 *
 * Copyright 2013 Maxime Ripard
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

=======
 */

#include <linux/delay.h>
>>>>>>> upstream/android-13
#include <linux/device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/reset-controller.h>
<<<<<<< HEAD
#include <linux/spinlock.h>

#include "reset-simple.h"

=======
#include <linux/reset/reset-simple.h>
#include <linux/spinlock.h>

>>>>>>> upstream/android-13
static inline struct reset_simple_data *
to_reset_simple_data(struct reset_controller_dev *rcdev)
{
	return container_of(rcdev, struct reset_simple_data, rcdev);
}

static int reset_simple_update(struct reset_controller_dev *rcdev,
			       unsigned long id, bool assert)
{
	struct reset_simple_data *data = to_reset_simple_data(rcdev);
	int reg_width = sizeof(u32);
	int bank = id / (reg_width * BITS_PER_BYTE);
	int offset = id % (reg_width * BITS_PER_BYTE);
	unsigned long flags;
	u32 reg;

	spin_lock_irqsave(&data->lock, flags);

	reg = readl(data->membase + (bank * reg_width));
	if (assert ^ data->active_low)
		reg |= BIT(offset);
	else
		reg &= ~BIT(offset);
	writel(reg, data->membase + (bank * reg_width));

	spin_unlock_irqrestore(&data->lock, flags);

	return 0;
}

static int reset_simple_assert(struct reset_controller_dev *rcdev,
			       unsigned long id)
{
	return reset_simple_update(rcdev, id, true);
}

static int reset_simple_deassert(struct reset_controller_dev *rcdev,
				 unsigned long id)
{
	return reset_simple_update(rcdev, id, false);
}

<<<<<<< HEAD
=======
static int reset_simple_reset(struct reset_controller_dev *rcdev,
			      unsigned long id)
{
	struct reset_simple_data *data = to_reset_simple_data(rcdev);
	int ret;

	if (!data->reset_us)
		return -ENOTSUPP;

	ret = reset_simple_assert(rcdev, id);
	if (ret)
		return ret;

	usleep_range(data->reset_us, data->reset_us * 2);

	return reset_simple_deassert(rcdev, id);
}

>>>>>>> upstream/android-13
static int reset_simple_status(struct reset_controller_dev *rcdev,
			       unsigned long id)
{
	struct reset_simple_data *data = to_reset_simple_data(rcdev);
	int reg_width = sizeof(u32);
	int bank = id / (reg_width * BITS_PER_BYTE);
	int offset = id % (reg_width * BITS_PER_BYTE);
	u32 reg;

	reg = readl(data->membase + (bank * reg_width));

	return !(reg & BIT(offset)) ^ !data->status_active_low;
}

const struct reset_control_ops reset_simple_ops = {
	.assert		= reset_simple_assert,
	.deassert	= reset_simple_deassert,
<<<<<<< HEAD
=======
	.reset		= reset_simple_reset,
>>>>>>> upstream/android-13
	.status		= reset_simple_status,
};
EXPORT_SYMBOL_GPL(reset_simple_ops);

/**
 * struct reset_simple_devdata - simple reset controller properties
 * @reg_offset: offset between base address and first reset register.
 * @nr_resets: number of resets. If not set, default to resource size in bits.
 * @active_low: if true, bits are cleared to assert the reset. Otherwise, bits
 *              are set to assert the reset.
 * @status_active_low: if true, bits read back as cleared while the reset is
 *                     asserted. Otherwise, bits read back as set while the
 *                     reset is asserted.
 */
struct reset_simple_devdata {
	u32 reg_offset;
	u32 nr_resets;
	bool active_low;
	bool status_active_low;
};

#define SOCFPGA_NR_BANKS	8

static const struct reset_simple_devdata reset_simple_socfpga = {
<<<<<<< HEAD
	.reg_offset = 0x10,
=======
	.reg_offset = 0x20,
>>>>>>> upstream/android-13
	.nr_resets = SOCFPGA_NR_BANKS * 32,
	.status_active_low = true,
};

static const struct reset_simple_devdata reset_simple_active_low = {
	.active_low = true,
	.status_active_low = true,
};

static const struct of_device_id reset_simple_dt_ids[] = {
<<<<<<< HEAD
	{ .compatible = "altr,rst-mgr", .data = &reset_simple_socfpga },
=======
	{ .compatible = "altr,stratix10-rst-mgr",
		.data = &reset_simple_socfpga },
>>>>>>> upstream/android-13
	{ .compatible = "st,stm32-rcc", },
	{ .compatible = "allwinner,sun6i-a31-clock-reset",
		.data = &reset_simple_active_low },
	{ .compatible = "zte,zx296718-reset",
		.data = &reset_simple_active_low },
	{ .compatible = "aspeed,ast2400-lpc-reset" },
	{ .compatible = "aspeed,ast2500-lpc-reset" },
<<<<<<< HEAD
=======
	{ .compatible = "bitmain,bm1880-reset",
		.data = &reset_simple_active_low },
	{ .compatible = "brcm,bcm4908-misc-pcie-reset",
		.data = &reset_simple_active_low },
	{ .compatible = "snps,dw-high-reset" },
	{ .compatible = "snps,dw-low-reset",
		.data = &reset_simple_active_low },
>>>>>>> upstream/android-13
	{ /* sentinel */ },
};

static int reset_simple_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	const struct reset_simple_devdata *devdata;
	struct reset_simple_data *data;
	void __iomem *membase;
	struct resource *res;
	u32 reg_offset = 0;

	devdata = of_device_get_match_data(dev);

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	membase = devm_ioremap_resource(dev, res);
	if (IS_ERR(membase))
		return PTR_ERR(membase);

	spin_lock_init(&data->lock);
	data->membase = membase;
	data->rcdev.owner = THIS_MODULE;
	data->rcdev.nr_resets = resource_size(res) * BITS_PER_BYTE;
	data->rcdev.ops = &reset_simple_ops;
	data->rcdev.of_node = dev->of_node;

	if (devdata) {
		reg_offset = devdata->reg_offset;
		if (devdata->nr_resets)
			data->rcdev.nr_resets = devdata->nr_resets;
		data->active_low = devdata->active_low;
		data->status_active_low = devdata->status_active_low;
	}

<<<<<<< HEAD
	if (of_device_is_compatible(dev->of_node, "altr,rst-mgr") &&
	    of_property_read_u32(dev->of_node, "altr,modrst-offset",
				 &reg_offset)) {
		dev_warn(dev,
			 "missing altr,modrst-offset property, assuming 0x%x!\n",
			 reg_offset);
	}

=======
>>>>>>> upstream/android-13
	data->membase += reg_offset;

	return devm_reset_controller_register(dev, &data->rcdev);
}

static struct platform_driver reset_simple_driver = {
	.probe	= reset_simple_probe,
	.driver = {
		.name		= "simple-reset",
		.of_match_table	= reset_simple_dt_ids,
	},
};
builtin_platform_driver(reset_simple_driver);
