<<<<<<< HEAD
/*
 * Copyright (c) 2014 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2014 MediaTek Inc.
>>>>>>> upstream/android-13
 */

#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/reset-controller.h>
#include <linux/slab.h>

#include "clk-mtk.h"

struct mtk_reset {
	struct regmap *regmap;
	int regofs;
	struct reset_controller_dev rcdev;
};

<<<<<<< HEAD
=======
static int mtk_reset_assert_set_clr(struct reset_controller_dev *rcdev,
	unsigned long id)
{
	struct mtk_reset *data = container_of(rcdev, struct mtk_reset, rcdev);
	unsigned int reg = data->regofs + ((id / 32) << 4);

	return regmap_write(data->regmap, reg, 1);
}

static int mtk_reset_deassert_set_clr(struct reset_controller_dev *rcdev,
	unsigned long id)
{
	struct mtk_reset *data = container_of(rcdev, struct mtk_reset, rcdev);
	unsigned int reg = data->regofs + ((id / 32) << 4) + 0x4;

	return regmap_write(data->regmap, reg, 1);
}

>>>>>>> upstream/android-13
static int mtk_reset_assert(struct reset_controller_dev *rcdev,
			      unsigned long id)
{
	struct mtk_reset *data = container_of(rcdev, struct mtk_reset, rcdev);

	return regmap_update_bits(data->regmap, data->regofs + ((id / 32) << 2),
			BIT(id % 32), ~0);
}

static int mtk_reset_deassert(struct reset_controller_dev *rcdev,
				unsigned long id)
{
	struct mtk_reset *data = container_of(rcdev, struct mtk_reset, rcdev);

	return regmap_update_bits(data->regmap, data->regofs + ((id / 32) << 2),
			BIT(id % 32), 0);
}

static int mtk_reset(struct reset_controller_dev *rcdev,
			      unsigned long id)
{
	int ret;

	ret = mtk_reset_assert(rcdev, id);
	if (ret)
		return ret;

	return mtk_reset_deassert(rcdev, id);
}

<<<<<<< HEAD
=======
static int mtk_reset_set_clr(struct reset_controller_dev *rcdev,
	unsigned long id)
{
	int ret;

	ret = mtk_reset_assert_set_clr(rcdev, id);
	if (ret)
		return ret;
	return mtk_reset_deassert_set_clr(rcdev, id);
}

>>>>>>> upstream/android-13
static const struct reset_control_ops mtk_reset_ops = {
	.assert = mtk_reset_assert,
	.deassert = mtk_reset_deassert,
	.reset = mtk_reset,
};

<<<<<<< HEAD
void mtk_register_reset_controller(struct device_node *np,
			unsigned int num_regs, int regofs)
=======
static const struct reset_control_ops mtk_reset_ops_set_clr = {
	.assert = mtk_reset_assert_set_clr,
	.deassert = mtk_reset_deassert_set_clr,
	.reset = mtk_reset_set_clr,
};

static void mtk_register_reset_controller_common(struct device_node *np,
			unsigned int num_regs, int regofs,
			const struct reset_control_ops *reset_ops)
>>>>>>> upstream/android-13
{
	struct mtk_reset *data;
	int ret;
	struct regmap *regmap;

<<<<<<< HEAD
	regmap = syscon_node_to_regmap(np);
=======
	regmap = device_node_to_regmap(np);
>>>>>>> upstream/android-13
	if (IS_ERR(regmap)) {
		pr_err("Cannot find regmap for %pOF: %ld\n", np,
				PTR_ERR(regmap));
		return;
	}

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return;

	data->regmap = regmap;
	data->regofs = regofs;
	data->rcdev.owner = THIS_MODULE;
	data->rcdev.nr_resets = num_regs * 32;
<<<<<<< HEAD
	data->rcdev.ops = &mtk_reset_ops;
=======
	data->rcdev.ops = reset_ops;
>>>>>>> upstream/android-13
	data->rcdev.of_node = np;

	ret = reset_controller_register(&data->rcdev);
	if (ret) {
		pr_err("could not register reset controller: %d\n", ret);
		kfree(data);
		return;
	}
}
<<<<<<< HEAD
=======

void mtk_register_reset_controller(struct device_node *np,
	unsigned int num_regs, int regofs)
{
	mtk_register_reset_controller_common(np, num_regs, regofs,
		&mtk_reset_ops);
}

void mtk_register_reset_controller_set_clr(struct device_node *np,
	unsigned int num_regs, int regofs)
{
	mtk_register_reset_controller_common(np, num_regs, regofs,
		&mtk_reset_ops_set_clr);
}

MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
