<<<<<<< HEAD
/*
 * Copyright (C) 2013 - 2014 Texas Instruments Incorporated - http://www.ti.com
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2013 - 2014 Texas Instruments Incorporated - https://www.ti.com
>>>>>>> upstream/android-13
 *
 * Authors:
 *    Jyri Sarha <jsarha@ti.com>
 *    Sergej Sawazki <ce3a@gmx.de>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 * Gpio controlled clock implementation
 */

#include <linux/clk-provider.h>
#include <linux/export.h>
#include <linux/slab.h>
#include <linux/gpio/consumer.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

/**
 * DOC: basic gpio gated clock which can be enabled and disabled
 *      with gpio output
 * Traits of this clock:
 * prepare - clk_(un)prepare only ensures parent is (un)prepared
 * enable - clk_enable and clk_disable are functional & control gpio
 * rate - inherits rate from parent.  No clk_set_rate support
 * parent - fixed parent.  No clk_set_parent support
 */

<<<<<<< HEAD
=======
/**
 * struct clk_gpio - gpio gated clock
 *
 * @hw:		handle between common and hardware-specific interfaces
 * @gpiod:	gpio descriptor
 *
 * Clock with a gpio control for enabling and disabling the parent clock
 * or switching between two parents by asserting or deasserting the gpio.
 *
 * Implements .enable, .disable and .is_enabled or
 * .get_parent, .set_parent and .determine_rate depending on which clk_ops
 * is used.
 */
struct clk_gpio {
	struct clk_hw	hw;
	struct gpio_desc *gpiod;
};

#define to_clk_gpio(_hw) container_of(_hw, struct clk_gpio, hw)

>>>>>>> upstream/android-13
static int clk_gpio_gate_enable(struct clk_hw *hw)
{
	struct clk_gpio *clk = to_clk_gpio(hw);

	gpiod_set_value(clk->gpiod, 1);

	return 0;
}

static void clk_gpio_gate_disable(struct clk_hw *hw)
{
	struct clk_gpio *clk = to_clk_gpio(hw);

	gpiod_set_value(clk->gpiod, 0);
}

static int clk_gpio_gate_is_enabled(struct clk_hw *hw)
{
	struct clk_gpio *clk = to_clk_gpio(hw);

	return gpiod_get_value(clk->gpiod);
}

<<<<<<< HEAD
const struct clk_ops clk_gpio_gate_ops = {
=======
static const struct clk_ops clk_gpio_gate_ops = {
>>>>>>> upstream/android-13
	.enable = clk_gpio_gate_enable,
	.disable = clk_gpio_gate_disable,
	.is_enabled = clk_gpio_gate_is_enabled,
};
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(clk_gpio_gate_ops);
=======

static int clk_sleeping_gpio_gate_prepare(struct clk_hw *hw)
{
	struct clk_gpio *clk = to_clk_gpio(hw);

	gpiod_set_value_cansleep(clk->gpiod, 1);

	return 0;
}

static void clk_sleeping_gpio_gate_unprepare(struct clk_hw *hw)
{
	struct clk_gpio *clk = to_clk_gpio(hw);

	gpiod_set_value_cansleep(clk->gpiod, 0);
}

static int clk_sleeping_gpio_gate_is_prepared(struct clk_hw *hw)
{
	struct clk_gpio *clk = to_clk_gpio(hw);

	return gpiod_get_value_cansleep(clk->gpiod);
}

static const struct clk_ops clk_sleeping_gpio_gate_ops = {
	.prepare = clk_sleeping_gpio_gate_prepare,
	.unprepare = clk_sleeping_gpio_gate_unprepare,
	.is_prepared = clk_sleeping_gpio_gate_is_prepared,
};
>>>>>>> upstream/android-13

/**
 * DOC: basic clock multiplexer which can be controlled with a gpio output
 * Traits of this clock:
 * prepare - clk_prepare only ensures that parents are prepared
 * rate - rate is only affected by parent switching.  No clk_set_rate support
 * parent - parent is adjustable through clk_set_parent
 */

static u8 clk_gpio_mux_get_parent(struct clk_hw *hw)
{
	struct clk_gpio *clk = to_clk_gpio(hw);

	return gpiod_get_value_cansleep(clk->gpiod);
}

static int clk_gpio_mux_set_parent(struct clk_hw *hw, u8 index)
{
	struct clk_gpio *clk = to_clk_gpio(hw);

	gpiod_set_value_cansleep(clk->gpiod, index);

	return 0;
}

<<<<<<< HEAD
const struct clk_ops clk_gpio_mux_ops = {
=======
static const struct clk_ops clk_gpio_mux_ops = {
>>>>>>> upstream/android-13
	.get_parent = clk_gpio_mux_get_parent,
	.set_parent = clk_gpio_mux_set_parent,
	.determine_rate = __clk_mux_determine_rate,
};
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(clk_gpio_mux_ops);

static struct clk_hw *clk_register_gpio(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents, struct gpio_desc *gpiod,
		unsigned long flags, const struct clk_ops *clk_gpio_ops)
=======

static struct clk_hw *clk_register_gpio(struct device *dev, u8 num_parents,
					struct gpio_desc *gpiod,
					const struct clk_ops *clk_gpio_ops)
>>>>>>> upstream/android-13
{
	struct clk_gpio *clk_gpio;
	struct clk_hw *hw;
	struct clk_init_data init = {};
	int err;
<<<<<<< HEAD

	if (dev)
		clk_gpio = devm_kzalloc(dev, sizeof(*clk_gpio),	GFP_KERNEL);
	else
		clk_gpio = kzalloc(sizeof(*clk_gpio), GFP_KERNEL);

	if (!clk_gpio)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = clk_gpio_ops;
	init.flags = flags | CLK_IS_BASIC;
	init.parent_names = parent_names;
	init.num_parents = num_parents;
=======
	const struct clk_parent_data gpio_parent_data[] = {
		{ .index = 0 },
		{ .index = 1 },
	};

	clk_gpio = devm_kzalloc(dev, sizeof(*clk_gpio),	GFP_KERNEL);
	if (!clk_gpio)
		return ERR_PTR(-ENOMEM);

	init.name = dev->of_node->name;
	init.ops = clk_gpio_ops;
	init.parent_data = gpio_parent_data;
	init.num_parents = num_parents;
	init.flags = CLK_SET_RATE_PARENT;
>>>>>>> upstream/android-13

	clk_gpio->gpiod = gpiod;
	clk_gpio->hw.init = &init;

	hw = &clk_gpio->hw;
<<<<<<< HEAD
	if (dev)
		err = devm_clk_hw_register(dev, hw);
	else
		err = clk_hw_register(NULL, hw);

	if (!err)
		return hw;

	if (!dev) {
		kfree(clk_gpio);
	}

	return ERR_PTR(err);
}

/**
 * clk_hw_register_gpio_gate - register a gpio clock gate with the clock
 * framework
 * @dev: device that is registering this clock
 * @name: name of this clock
 * @parent_name: name of this clock's parent
 * @gpiod: gpio descriptor to gate this clock
 * @flags: clock flags
 */
struct clk_hw *clk_hw_register_gpio_gate(struct device *dev, const char *name,
		const char *parent_name, struct gpio_desc *gpiod,
		unsigned long flags)
{
	return clk_register_gpio(dev, name,
			(parent_name ? &parent_name : NULL),
			(parent_name ? 1 : 0), gpiod, flags,
			&clk_gpio_gate_ops);
}
EXPORT_SYMBOL_GPL(clk_hw_register_gpio_gate);

struct clk *clk_register_gpio_gate(struct device *dev, const char *name,
		const char *parent_name, struct gpio_desc *gpiod,
		unsigned long flags)
{
	struct clk_hw *hw;

	hw = clk_hw_register_gpio_gate(dev, name, parent_name, gpiod, flags);
	if (IS_ERR(hw))
		return ERR_CAST(hw);
	return hw->clk;
}
EXPORT_SYMBOL_GPL(clk_register_gpio_gate);

/**
 * clk_hw_register_gpio_mux - register a gpio clock mux with the clock framework
 * @dev: device that is registering this clock
 * @name: name of this clock
 * @parent_names: names of this clock's parents
 * @num_parents: number of parents listed in @parent_names
 * @gpiod: gpio descriptor to gate this clock
 * @flags: clock flags
 */
struct clk_hw *clk_hw_register_gpio_mux(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents, struct gpio_desc *gpiod,
		unsigned long flags)
{
	if (num_parents != 2) {
		pr_err("mux-clock %s must have 2 parents\n", name);
		return ERR_PTR(-EINVAL);
	}

	return clk_register_gpio(dev, name, parent_names, num_parents,
			gpiod, flags, &clk_gpio_mux_ops);
}
EXPORT_SYMBOL_GPL(clk_hw_register_gpio_mux);

struct clk *clk_register_gpio_mux(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents, struct gpio_desc *gpiod,
		unsigned long flags)
{
	struct clk_hw *hw;

	hw = clk_hw_register_gpio_mux(dev, name, parent_names, num_parents,
			gpiod, flags);
	if (IS_ERR(hw))
		return ERR_CAST(hw);
	return hw->clk;
}
EXPORT_SYMBOL_GPL(clk_register_gpio_mux);

static int gpio_clk_driver_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	const char **parent_names, *gpio_name;
	unsigned int num_parents;
	struct gpio_desc *gpiod;
	struct clk *clk;
	bool is_mux;
	int ret;

	num_parents = of_clk_get_parent_count(node);
	if (num_parents) {
		parent_names = devm_kcalloc(&pdev->dev, num_parents,
					    sizeof(char *), GFP_KERNEL);
		if (!parent_names)
			return -ENOMEM;

		of_clk_parent_fill(node, parent_names, num_parents);
	} else {
		parent_names = NULL;
	}

	is_mux = of_device_is_compatible(node, "gpio-mux-clock");

	gpio_name = is_mux ? "select" : "enable";
	gpiod = devm_gpiod_get(&pdev->dev, gpio_name, GPIOD_OUT_LOW);
	if (IS_ERR(gpiod)) {
		ret = PTR_ERR(gpiod);
		if (ret == -EPROBE_DEFER)
			pr_debug("%s: %s: GPIOs not yet available, retry later\n",
					node->name, __func__);
		else
			pr_err("%s: %s: Can't get '%s' named GPIO property\n",
					node->name, __func__,
=======
	err = devm_clk_hw_register(dev, hw);
	if (err)
		return ERR_PTR(err);

	return hw;
}

static struct clk_hw *clk_hw_register_gpio_gate(struct device *dev,
						int num_parents,
						struct gpio_desc *gpiod)
{
	const struct clk_ops *ops;

	if (gpiod_cansleep(gpiod))
		ops = &clk_sleeping_gpio_gate_ops;
	else
		ops = &clk_gpio_gate_ops;

	return clk_register_gpio(dev, num_parents, gpiod, ops);
}

static struct clk_hw *clk_hw_register_gpio_mux(struct device *dev,
					       struct gpio_desc *gpiod)
{
	return clk_register_gpio(dev, 2, gpiod, &clk_gpio_mux_ops);
}

static int gpio_clk_driver_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *node = dev->of_node;
	const char *gpio_name;
	unsigned int num_parents;
	struct gpio_desc *gpiod;
	struct clk_hw *hw;
	bool is_mux;
	int ret;

	is_mux = of_device_is_compatible(node, "gpio-mux-clock");

	num_parents = of_clk_get_parent_count(node);
	if (is_mux && num_parents != 2) {
		dev_err(dev, "mux-clock must have 2 parents\n");
		return -EINVAL;
	}

	gpio_name = is_mux ? "select" : "enable";
	gpiod = devm_gpiod_get(dev, gpio_name, GPIOD_OUT_LOW);
	if (IS_ERR(gpiod)) {
		ret = PTR_ERR(gpiod);
		if (ret == -EPROBE_DEFER)
			pr_debug("%pOFn: %s: GPIOs not yet available, retry later\n",
					node, __func__);
		else
			pr_err("%pOFn: %s: Can't get '%s' named GPIO property\n",
					node, __func__,
>>>>>>> upstream/android-13
					gpio_name);
		return ret;
	}

	if (is_mux)
<<<<<<< HEAD
		clk = clk_register_gpio_mux(&pdev->dev, node->name,
				parent_names, num_parents, gpiod, 0);
	else
		clk = clk_register_gpio_gate(&pdev->dev, node->name,
				parent_names ?  parent_names[0] : NULL, gpiod,
				CLK_SET_RATE_PARENT);
	if (IS_ERR(clk))
		return PTR_ERR(clk);

	return of_clk_add_provider(node, of_clk_src_simple_get, clk);
=======
		hw = clk_hw_register_gpio_mux(dev, gpiod);
	else
		hw = clk_hw_register_gpio_gate(dev, num_parents, gpiod);
	if (IS_ERR(hw))
		return PTR_ERR(hw);

	return devm_of_clk_add_hw_provider(dev, of_clk_hw_simple_get, hw);
>>>>>>> upstream/android-13
}

static const struct of_device_id gpio_clk_match_table[] = {
	{ .compatible = "gpio-mux-clock" },
	{ .compatible = "gpio-gate-clock" },
	{ }
};

static struct platform_driver gpio_clk_driver = {
	.probe		= gpio_clk_driver_probe,
	.driver		= {
		.name	= "gpio-clk",
		.of_match_table = gpio_clk_match_table,
	},
};
builtin_platform_driver(gpio_clk_driver);
