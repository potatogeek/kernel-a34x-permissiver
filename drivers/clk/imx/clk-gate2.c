<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
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
 * Gated clock implementation
 */

#include <linux/clk-provider.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/string.h>
#include "clk.h"

/**
<<<<<<< HEAD
 * DOC: basic gatable clock which can gate and ungate it's ouput
=======
 * DOC: basic gateable clock which can gate and ungate its output
>>>>>>> upstream/android-13
 *
 * Traits of this clock:
 * prepare - clk_(un)prepare only ensures parent is (un)prepared
 * enable - clk_enable and clk_disable are functional & control gating
 * rate - inherits rate from parent.  No clk_set_rate support
 * parent - fixed parent.  No clk_set_parent support
 */

struct clk_gate2 {
	struct clk_hw hw;
	void __iomem	*reg;
	u8		bit_idx;
	u8		cgr_val;
<<<<<<< HEAD
=======
	u8		cgr_mask;
>>>>>>> upstream/android-13
	u8		flags;
	spinlock_t	*lock;
	unsigned int	*share_count;
};

#define to_clk_gate2(_hw) container_of(_hw, struct clk_gate2, hw)

<<<<<<< HEAD
static int clk_gate2_enable(struct clk_hw *hw)
{
	struct clk_gate2 *gate = to_clk_gate2(hw);
	u32 reg;
	unsigned long flags = 0;
=======
static void clk_gate2_do_shared_clks(struct clk_hw *hw, bool enable)
{
	struct clk_gate2 *gate = to_clk_gate2(hw);
	u32 reg;

	reg = readl(gate->reg);
	reg &= ~(gate->cgr_mask << gate->bit_idx);
	if (enable)
		reg |= (gate->cgr_val & gate->cgr_mask) << gate->bit_idx;
	writel(reg, gate->reg);
}

static int clk_gate2_enable(struct clk_hw *hw)
{
	struct clk_gate2 *gate = to_clk_gate2(hw);
	unsigned long flags;
>>>>>>> upstream/android-13

	spin_lock_irqsave(gate->lock, flags);

	if (gate->share_count && (*gate->share_count)++ > 0)
		goto out;

<<<<<<< HEAD
	reg = readl(gate->reg);
	reg &= ~(3 << gate->bit_idx);
	reg |= gate->cgr_val << gate->bit_idx;
	writel(reg, gate->reg);

=======
	clk_gate2_do_shared_clks(hw, true);
>>>>>>> upstream/android-13
out:
	spin_unlock_irqrestore(gate->lock, flags);

	return 0;
}

static void clk_gate2_disable(struct clk_hw *hw)
{
	struct clk_gate2 *gate = to_clk_gate2(hw);
<<<<<<< HEAD
	u32 reg;
	unsigned long flags = 0;
=======
	unsigned long flags;
>>>>>>> upstream/android-13

	spin_lock_irqsave(gate->lock, flags);

	if (gate->share_count) {
		if (WARN_ON(*gate->share_count == 0))
			goto out;
		else if (--(*gate->share_count) > 0)
			goto out;
	}

<<<<<<< HEAD
	reg = readl(gate->reg);
	reg &= ~(3 << gate->bit_idx);
	writel(reg, gate->reg);

=======
	clk_gate2_do_shared_clks(hw, false);
>>>>>>> upstream/android-13
out:
	spin_unlock_irqrestore(gate->lock, flags);
}

<<<<<<< HEAD
static int clk_gate2_reg_is_enabled(void __iomem *reg, u8 bit_idx)
{
	u32 val = readl(reg);

	if (((val >> bit_idx) & 1) == 1)
=======
static int clk_gate2_reg_is_enabled(void __iomem *reg, u8 bit_idx,
					u8 cgr_val, u8 cgr_mask)
{
	u32 val = readl(reg);

	if (((val >> bit_idx) & cgr_mask) == cgr_val)
>>>>>>> upstream/android-13
		return 1;

	return 0;
}

static int clk_gate2_is_enabled(struct clk_hw *hw)
{
	struct clk_gate2 *gate = to_clk_gate2(hw);
<<<<<<< HEAD

	return clk_gate2_reg_is_enabled(gate->reg, gate->bit_idx);
=======
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(gate->lock, flags);

	ret = clk_gate2_reg_is_enabled(gate->reg, gate->bit_idx,
					gate->cgr_val, gate->cgr_mask);

	spin_unlock_irqrestore(gate->lock, flags);

	return ret;
>>>>>>> upstream/android-13
}

static void clk_gate2_disable_unused(struct clk_hw *hw)
{
	struct clk_gate2 *gate = to_clk_gate2(hw);
<<<<<<< HEAD
	unsigned long flags = 0;
	u32 reg;

	spin_lock_irqsave(gate->lock, flags);

	if (!gate->share_count || *gate->share_count == 0) {
		reg = readl(gate->reg);
		reg &= ~(3 << gate->bit_idx);
		writel(reg, gate->reg);
	}
=======
	unsigned long flags;

	spin_lock_irqsave(gate->lock, flags);

	if (!gate->share_count || *gate->share_count == 0)
		clk_gate2_do_shared_clks(hw, false);
>>>>>>> upstream/android-13

	spin_unlock_irqrestore(gate->lock, flags);
}

static const struct clk_ops clk_gate2_ops = {
	.enable = clk_gate2_enable,
	.disable = clk_gate2_disable,
	.disable_unused = clk_gate2_disable_unused,
	.is_enabled = clk_gate2_is_enabled,
};

<<<<<<< HEAD
struct clk *clk_register_gate2(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 bit_idx, u8 cgr_val,
=======
struct clk_hw *clk_hw_register_gate2(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 bit_idx, u8 cgr_val, u8 cgr_mask,
>>>>>>> upstream/android-13
		u8 clk_gate2_flags, spinlock_t *lock,
		unsigned int *share_count)
{
	struct clk_gate2 *gate;
<<<<<<< HEAD
	struct clk *clk;
	struct clk_init_data init = {};
=======
	struct clk_hw *hw;
	struct clk_init_data init;
	int ret;
>>>>>>> upstream/android-13

	gate = kzalloc(sizeof(struct clk_gate2), GFP_KERNEL);
	if (!gate)
		return ERR_PTR(-ENOMEM);

	/* struct clk_gate2 assignments */
	gate->reg = reg;
	gate->bit_idx = bit_idx;
	gate->cgr_val = cgr_val;
<<<<<<< HEAD
=======
	gate->cgr_mask = cgr_mask;
>>>>>>> upstream/android-13
	gate->flags = clk_gate2_flags;
	gate->lock = lock;
	gate->share_count = share_count;

	init.name = name;
	init.ops = &clk_gate2_ops;
	init.flags = flags;
	init.parent_names = parent_name ? &parent_name : NULL;
	init.num_parents = parent_name ? 1 : 0;

	gate->hw.init = &init;
<<<<<<< HEAD

	clk = clk_register(dev, &gate->hw);
	if (IS_ERR(clk))
		kfree(gate);

	return clk;
}
=======
	hw = &gate->hw;

	ret = clk_hw_register(dev, hw);
	if (ret) {
		kfree(gate);
		return ERR_PTR(ret);
	}

	return hw;
}
EXPORT_SYMBOL_GPL(clk_hw_register_gate2);
>>>>>>> upstream/android-13
