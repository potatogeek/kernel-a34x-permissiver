<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Generic Syscon Reboot Driver
 *
 * Copyright (c) 2013, Applied Micro Circuits Corporation
 * Author: Feng Kan <fkan@apm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/notifier.h>
#include <linux/mfd/syscon.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <linux/regmap.h>

struct syscon_reboot_context {
	struct regmap *map;
	u32 offset;
<<<<<<< HEAD
=======
	u32 value;
>>>>>>> upstream/android-13
	u32 mask;
	struct notifier_block restart_handler;
};

static int syscon_restart_handle(struct notifier_block *this,
					unsigned long mode, void *cmd)
{
	struct syscon_reboot_context *ctx =
			container_of(this, struct syscon_reboot_context,
					restart_handler);

	/* Issue the reboot */
<<<<<<< HEAD
	regmap_write(ctx->map, ctx->offset, ctx->mask);
=======
	regmap_update_bits(ctx->map, ctx->offset, ctx->mask, ctx->value);
>>>>>>> upstream/android-13

	mdelay(1000);

	pr_emerg("Unable to restart system\n");
	return NOTIFY_DONE;
}

static int syscon_reboot_probe(struct platform_device *pdev)
{
	struct syscon_reboot_context *ctx;
	struct device *dev = &pdev->dev;
<<<<<<< HEAD
=======
	int mask_err, value_err;
>>>>>>> upstream/android-13
	int err;

	ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->map = syscon_regmap_lookup_by_phandle(dev->of_node, "regmap");
<<<<<<< HEAD
	if (IS_ERR(ctx->map))
		return PTR_ERR(ctx->map);
=======
	if (IS_ERR(ctx->map)) {
		ctx->map = syscon_node_to_regmap(dev->parent->of_node);
		if (IS_ERR(ctx->map))
			return PTR_ERR(ctx->map);
	}
>>>>>>> upstream/android-13

	if (of_property_read_u32(pdev->dev.of_node, "offset", &ctx->offset))
		return -EINVAL;

<<<<<<< HEAD
	if (of_property_read_u32(pdev->dev.of_node, "mask", &ctx->mask))
		return -EINVAL;
=======
	value_err = of_property_read_u32(pdev->dev.of_node, "value", &ctx->value);
	mask_err = of_property_read_u32(pdev->dev.of_node, "mask", &ctx->mask);
	if (value_err && mask_err) {
		dev_err(dev, "unable to read 'value' and 'mask'");
		return -EINVAL;
	}

	if (value_err) {
		/* support old binding */
		ctx->value = ctx->mask;
		ctx->mask = 0xFFFFFFFF;
	} else if (mask_err) {
		/* support value without mask*/
		ctx->mask = 0xFFFFFFFF;
	}
>>>>>>> upstream/android-13

	ctx->restart_handler.notifier_call = syscon_restart_handle;
	ctx->restart_handler.priority = 192;
	err = register_restart_handler(&ctx->restart_handler);
	if (err)
		dev_err(dev, "can't register restart notifier (err=%d)\n", err);

	return err;
}

static const struct of_device_id syscon_reboot_of_match[] = {
	{ .compatible = "syscon-reboot" },
	{}
};

static struct platform_driver syscon_reboot_driver = {
	.probe = syscon_reboot_probe,
	.driver = {
		.name = "syscon-reboot",
		.of_match_table = syscon_reboot_of_match,
	},
};
builtin_platform_driver(syscon_reboot_driver);
