<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2010-2011 Canonical Ltd <jeremy.kerr@canonical.com>
 * Copyright (C) 2011-2012 Mike Turquette, Linaro Ltd <mturquette@linaro.org>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 * Fixed rate clock implementation
 */

#include <linux/clk-provider.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/platform_device.h>

/*
 * DOC: basic fixed-rate clock that cannot gate
 *
 * Traits of this clock:
 * prepare - clk_(un)prepare only ensures parents are prepared
 * enable - clk_enable only ensures parents are enabled
 * rate - rate is always a fixed value.  No clk_set_rate support
 * parent - fixed parent.  No clk_set_parent support
 */

<<<<<<< HEAD
=======
#define to_clk_fixed_rate(_hw) container_of(_hw, struct clk_fixed_rate, hw)

>>>>>>> upstream/android-13
static unsigned long clk_fixed_rate_recalc_rate(struct clk_hw *hw,
		unsigned long parent_rate)
{
	return to_clk_fixed_rate(hw)->fixed_rate;
}

static unsigned long clk_fixed_rate_recalc_accuracy(struct clk_hw *hw,
		unsigned long parent_accuracy)
{
<<<<<<< HEAD
	return to_clk_fixed_rate(hw)->fixed_accuracy;
=======
	struct clk_fixed_rate *fixed = to_clk_fixed_rate(hw);

	if (fixed->flags & CLK_FIXED_RATE_PARENT_ACCURACY)
		return parent_accuracy;

	return fixed->fixed_accuracy;
>>>>>>> upstream/android-13
}

const struct clk_ops clk_fixed_rate_ops = {
	.recalc_rate = clk_fixed_rate_recalc_rate,
	.recalc_accuracy = clk_fixed_rate_recalc_accuracy,
};
EXPORT_SYMBOL_GPL(clk_fixed_rate_ops);

<<<<<<< HEAD
/**
 * clk_hw_register_fixed_rate_with_accuracy - register fixed-rate clock with
 * the clock framework
 * @dev: device that is registering this clock
 * @name: name of this clock
 * @parent_name: name of clock's parent
 * @flags: framework-specific flags
 * @fixed_rate: non-adjustable clock rate
 * @fixed_accuracy: non-adjustable clock rate
 */
struct clk_hw *clk_hw_register_fixed_rate_with_accuracy(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned long fixed_rate, unsigned long fixed_accuracy)
=======
struct clk_hw *__clk_hw_register_fixed_rate(struct device *dev,
		struct device_node *np, const char *name,
		const char *parent_name, const struct clk_hw *parent_hw,
		const struct clk_parent_data *parent_data, unsigned long flags,
		unsigned long fixed_rate, unsigned long fixed_accuracy,
		unsigned long clk_fixed_flags)
>>>>>>> upstream/android-13
{
	struct clk_fixed_rate *fixed;
	struct clk_hw *hw;
	struct clk_init_data init = {};
<<<<<<< HEAD
	int ret;
=======
	int ret = -EINVAL;
>>>>>>> upstream/android-13

	/* allocate fixed-rate clock */
	fixed = kzalloc(sizeof(*fixed), GFP_KERNEL);
	if (!fixed)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &clk_fixed_rate_ops;
<<<<<<< HEAD
	init.flags = flags | CLK_IS_BASIC;
	init.parent_names = (parent_name ? &parent_name: NULL);
	init.num_parents = (parent_name ? 1 : 0);

	/* struct clk_fixed_rate assignments */
=======
	init.flags = flags;
	init.parent_names = parent_name ? &parent_name : NULL;
	init.parent_hws = parent_hw ? &parent_hw : NULL;
	init.parent_data = parent_data;
	if (parent_name || parent_hw || parent_data)
		init.num_parents = 1;
	else
		init.num_parents = 0;

	/* struct clk_fixed_rate assignments */
	fixed->flags = clk_fixed_flags;
>>>>>>> upstream/android-13
	fixed->fixed_rate = fixed_rate;
	fixed->fixed_accuracy = fixed_accuracy;
	fixed->hw.init = &init;

	/* register the clock */
	hw = &fixed->hw;
<<<<<<< HEAD
	ret = clk_hw_register(dev, hw);
=======
	if (dev || !np)
		ret = clk_hw_register(dev, hw);
	else if (np)
		ret = of_clk_hw_register(np, hw);
>>>>>>> upstream/android-13
	if (ret) {
		kfree(fixed);
		hw = ERR_PTR(ret);
	}

	return hw;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(clk_hw_register_fixed_rate_with_accuracy);

struct clk *clk_register_fixed_rate_with_accuracy(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned long fixed_rate, unsigned long fixed_accuracy)
{
	struct clk_hw *hw;

	hw = clk_hw_register_fixed_rate_with_accuracy(dev, name, parent_name,
			flags, fixed_rate, fixed_accuracy);
	if (IS_ERR(hw))
		return ERR_CAST(hw);
	return hw->clk;
}
EXPORT_SYMBOL_GPL(clk_register_fixed_rate_with_accuracy);

/**
 * clk_hw_register_fixed_rate - register fixed-rate clock with the clock
 * framework
 * @dev: device that is registering this clock
 * @name: name of this clock
 * @parent_name: name of clock's parent
 * @flags: framework-specific flags
 * @fixed_rate: non-adjustable clock rate
 */
struct clk_hw *clk_hw_register_fixed_rate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned long fixed_rate)
{
	return clk_hw_register_fixed_rate_with_accuracy(dev, name, parent_name,
						     flags, fixed_rate, 0);
}
EXPORT_SYMBOL_GPL(clk_hw_register_fixed_rate);
=======
EXPORT_SYMBOL_GPL(__clk_hw_register_fixed_rate);
>>>>>>> upstream/android-13

struct clk *clk_register_fixed_rate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned long fixed_rate)
{
<<<<<<< HEAD
	return clk_register_fixed_rate_with_accuracy(dev, name, parent_name,
						     flags, fixed_rate, 0);
=======
	struct clk_hw *hw;

	hw = clk_hw_register_fixed_rate_with_accuracy(dev, name, parent_name,
						      flags, fixed_rate, 0);
	if (IS_ERR(hw))
		return ERR_CAST(hw);
	return hw->clk;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(clk_register_fixed_rate);

void clk_unregister_fixed_rate(struct clk *clk)
{
	struct clk_hw *hw;

	hw = __clk_get_hw(clk);
	if (!hw)
		return;

	clk_unregister(clk);
	kfree(to_clk_fixed_rate(hw));
}
EXPORT_SYMBOL_GPL(clk_unregister_fixed_rate);

void clk_hw_unregister_fixed_rate(struct clk_hw *hw)
{
	struct clk_fixed_rate *fixed;

	fixed = to_clk_fixed_rate(hw);

	clk_hw_unregister(hw);
	kfree(fixed);
}
EXPORT_SYMBOL_GPL(clk_hw_unregister_fixed_rate);

#ifdef CONFIG_OF
<<<<<<< HEAD
static struct clk *_of_fixed_clk_setup(struct device_node *node)
{
	struct clk *clk;
=======
static struct clk_hw *_of_fixed_clk_setup(struct device_node *node)
{
	struct clk_hw *hw;
>>>>>>> upstream/android-13
	const char *clk_name = node->name;
	u32 rate;
	u32 accuracy = 0;
	int ret;

	if (of_property_read_u32(node, "clock-frequency", &rate))
		return ERR_PTR(-EIO);

	of_property_read_u32(node, "clock-accuracy", &accuracy);

	of_property_read_string(node, "clock-output-names", &clk_name);

<<<<<<< HEAD
	clk = clk_register_fixed_rate_with_accuracy(NULL, clk_name, NULL,
						    0, rate, accuracy);
	if (IS_ERR(clk))
		return clk;

	ret = of_clk_add_provider(node, of_clk_src_simple_get, clk);
	if (ret) {
		clk_unregister(clk);
		return ERR_PTR(ret);
	}

	return clk;
=======
	hw = clk_hw_register_fixed_rate_with_accuracy(NULL, clk_name, NULL,
						    0, rate, accuracy);
	if (IS_ERR(hw))
		return hw;

	ret = of_clk_add_hw_provider(node, of_clk_hw_simple_get, hw);
	if (ret) {
		clk_hw_unregister_fixed_rate(hw);
		return ERR_PTR(ret);
	}

	return hw;
>>>>>>> upstream/android-13
}

/**
 * of_fixed_clk_setup() - Setup function for simple fixed rate clock
<<<<<<< HEAD
=======
 * @node:	device node for the clock
>>>>>>> upstream/android-13
 */
void __init of_fixed_clk_setup(struct device_node *node)
{
	_of_fixed_clk_setup(node);
}
CLK_OF_DECLARE(fixed_clk, "fixed-clock", of_fixed_clk_setup);

static int of_fixed_clk_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct clk *clk = platform_get_drvdata(pdev);

	of_clk_del_provider(pdev->dev.of_node);
	clk_unregister_fixed_rate(clk);
=======
	struct clk_hw *hw = platform_get_drvdata(pdev);

	of_clk_del_provider(pdev->dev.of_node);
	clk_hw_unregister_fixed_rate(hw);
>>>>>>> upstream/android-13

	return 0;
}

static int of_fixed_clk_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct clk *clk;
=======
	struct clk_hw *hw;
>>>>>>> upstream/android-13

	/*
	 * This function is not executed when of_fixed_clk_setup
	 * succeeded.
	 */
<<<<<<< HEAD
	clk = _of_fixed_clk_setup(pdev->dev.of_node);
	if (IS_ERR(clk))
		return PTR_ERR(clk);

	platform_set_drvdata(pdev, clk);
=======
	hw = _of_fixed_clk_setup(pdev->dev.of_node);
	if (IS_ERR(hw))
		return PTR_ERR(hw);

	platform_set_drvdata(pdev, hw);
>>>>>>> upstream/android-13

	return 0;
}

static const struct of_device_id of_fixed_clk_ids[] = {
	{ .compatible = "fixed-clock" },
	{ }
};
<<<<<<< HEAD
MODULE_DEVICE_TABLE(of, of_fixed_clk_ids);
=======
>>>>>>> upstream/android-13

static struct platform_driver of_fixed_clk_driver = {
	.driver = {
		.name = "of_fixed_clk",
		.of_match_table = of_fixed_clk_ids,
	},
	.probe = of_fixed_clk_probe,
	.remove = of_fixed_clk_remove,
};
builtin_platform_driver(of_fixed_clk_driver);
#endif
