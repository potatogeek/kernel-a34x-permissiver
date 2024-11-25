<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * r8a7790 Common Clock Framework support
 *
 * Copyright (C) 2013  Renesas Solutions Corp.
 *
 * Contact: Laurent Pinchart <laurent.pinchart@ideasonboard.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
=======
>>>>>>> upstream/android-13
 */

#include <linux/clk-provider.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/pm.h>
#include <linux/slab.h>

#include "clk-div6.h"

#define CPG_DIV6_CKSTP		BIT(8)
#define CPG_DIV6_DIV(d)		((d) & 0x3f)
#define CPG_DIV6_DIV_MASK	0x3f

/**
 * struct div6_clock - CPG 6 bit divider clock
 * @hw: handle between common and hardware-specific interfaces
 * @reg: IO-remapped register
 * @div: divisor value (1-64)
<<<<<<< HEAD
 * @src_shift: Shift to access the register bits to select the parent clock
 * @src_width: Number of register bits to select the parent clock (may be 0)
 * @parents: Array to map from valid parent clocks indices to hardware indices
 * @nb: Notifier block to save/restore clock state for system resume
=======
 * @src_mask: Bitmask covering the register bits to select the parent clock
 * @nb: Notifier block to save/restore clock state for system resume
 * @parents: Array to map from valid parent clocks indices to hardware indices
>>>>>>> upstream/android-13
 */
struct div6_clock {
	struct clk_hw hw;
	void __iomem *reg;
	unsigned int div;
<<<<<<< HEAD
	u32 src_shift;
	u32 src_width;
	u8 *parents;
	struct notifier_block nb;
=======
	u32 src_mask;
	struct notifier_block nb;
	u8 parents[];
>>>>>>> upstream/android-13
};

#define to_div6_clock(_hw) container_of(_hw, struct div6_clock, hw)

static int cpg_div6_clock_enable(struct clk_hw *hw)
{
	struct div6_clock *clock = to_div6_clock(hw);
	u32 val;

	val = (readl(clock->reg) & ~(CPG_DIV6_DIV_MASK | CPG_DIV6_CKSTP))
	    | CPG_DIV6_DIV(clock->div - 1);
	writel(val, clock->reg);

	return 0;
}

static void cpg_div6_clock_disable(struct clk_hw *hw)
{
	struct div6_clock *clock = to_div6_clock(hw);
	u32 val;

	val = readl(clock->reg);
	val |= CPG_DIV6_CKSTP;
	/*
	 * DIV6 clocks require the divisor field to be non-zero when stopping
	 * the clock. However, some clocks (e.g. ZB on sh73a0) fail to be
	 * re-enabled later if the divisor field is changed when stopping the
	 * clock
	 */
	if (!(val & CPG_DIV6_DIV_MASK))
		val |= CPG_DIV6_DIV_MASK;
	writel(val, clock->reg);
}

static int cpg_div6_clock_is_enabled(struct clk_hw *hw)
{
	struct div6_clock *clock = to_div6_clock(hw);

	return !(readl(clock->reg) & CPG_DIV6_CKSTP);
}

static unsigned long cpg_div6_clock_recalc_rate(struct clk_hw *hw,
						unsigned long parent_rate)
{
	struct div6_clock *clock = to_div6_clock(hw);

	return parent_rate / clock->div;
}

static unsigned int cpg_div6_clock_calc_div(unsigned long rate,
					    unsigned long parent_rate)
{
	unsigned int div;

	if (!rate)
		rate = 1;

	div = DIV_ROUND_CLOSEST(parent_rate, rate);
<<<<<<< HEAD
	return clamp_t(unsigned int, div, 1, 64);
}

static long cpg_div6_clock_round_rate(struct clk_hw *hw, unsigned long rate,
				      unsigned long *parent_rate)
{
	unsigned int div = cpg_div6_clock_calc_div(rate, *parent_rate);

	return *parent_rate / div;
=======
	return clamp(div, 1U, 64U);
}

static int cpg_div6_clock_determine_rate(struct clk_hw *hw,
					 struct clk_rate_request *req)
{
	unsigned long prate, calc_rate, diff, best_rate, best_prate;
	unsigned int num_parents = clk_hw_get_num_parents(hw);
	struct clk_hw *parent, *best_parent = NULL;
	unsigned int i, min_div, max_div, div;
	unsigned long min_diff = ULONG_MAX;

	for (i = 0; i < num_parents; i++) {
		parent = clk_hw_get_parent_by_index(hw, i);
		if (!parent)
			continue;

		prate = clk_hw_get_rate(parent);
		if (!prate)
			continue;

		min_div = max(DIV_ROUND_UP(prate, req->max_rate), 1UL);
		max_div = req->min_rate ? min(prate / req->min_rate, 64UL) : 64;
		if (max_div < min_div)
			continue;

		div = cpg_div6_clock_calc_div(req->rate, prate);
		div = clamp(div, min_div, max_div);
		calc_rate = prate / div;
		diff = calc_rate > req->rate ? calc_rate - req->rate
					     : req->rate - calc_rate;
		if (diff < min_diff) {
			best_rate = calc_rate;
			best_parent = parent;
			best_prate = prate;
			min_diff = diff;
		}
	}

	if (!best_parent)
		return -EINVAL;

	req->best_parent_rate = best_prate;
	req->best_parent_hw = best_parent;
	req->rate = best_rate;
	return 0;
>>>>>>> upstream/android-13
}

static int cpg_div6_clock_set_rate(struct clk_hw *hw, unsigned long rate,
				   unsigned long parent_rate)
{
	struct div6_clock *clock = to_div6_clock(hw);
	unsigned int div = cpg_div6_clock_calc_div(rate, parent_rate);
	u32 val;

	clock->div = div;

	val = readl(clock->reg) & ~CPG_DIV6_DIV_MASK;
	/* Only program the new divisor if the clock isn't stopped. */
	if (!(val & CPG_DIV6_CKSTP))
		writel(val | CPG_DIV6_DIV(clock->div - 1), clock->reg);

	return 0;
}

static u8 cpg_div6_clock_get_parent(struct clk_hw *hw)
{
	struct div6_clock *clock = to_div6_clock(hw);
	unsigned int i;
	u8 hw_index;

<<<<<<< HEAD
	if (clock->src_width == 0)
		return 0;

	hw_index = (readl(clock->reg) >> clock->src_shift) &
		   (BIT(clock->src_width) - 1);
=======
	if (clock->src_mask == 0)
		return 0;

	hw_index = (readl(clock->reg) & clock->src_mask) >>
		   __ffs(clock->src_mask);
>>>>>>> upstream/android-13
	for (i = 0; i < clk_hw_get_num_parents(hw); i++) {
		if (clock->parents[i] == hw_index)
			return i;
	}

	pr_err("%s: %s DIV6 clock set to invalid parent %u\n",
	       __func__, clk_hw_get_name(hw), hw_index);
	return 0;
}

static int cpg_div6_clock_set_parent(struct clk_hw *hw, u8 index)
{
	struct div6_clock *clock = to_div6_clock(hw);
<<<<<<< HEAD
	u8 hw_index;
	u32 mask;
=======
	u32 src;
>>>>>>> upstream/android-13

	if (index >= clk_hw_get_num_parents(hw))
		return -EINVAL;

<<<<<<< HEAD
	mask = ~((BIT(clock->src_width) - 1) << clock->src_shift);
	hw_index = clock->parents[index];

	writel((readl(clock->reg) & mask) | (hw_index << clock->src_shift),
	       clock->reg);

=======
	src = clock->parents[index] << __ffs(clock->src_mask);
	writel((readl(clock->reg) & ~clock->src_mask) | src, clock->reg);
>>>>>>> upstream/android-13
	return 0;
}

static const struct clk_ops cpg_div6_clock_ops = {
	.enable = cpg_div6_clock_enable,
	.disable = cpg_div6_clock_disable,
	.is_enabled = cpg_div6_clock_is_enabled,
	.get_parent = cpg_div6_clock_get_parent,
	.set_parent = cpg_div6_clock_set_parent,
	.recalc_rate = cpg_div6_clock_recalc_rate,
<<<<<<< HEAD
	.round_rate = cpg_div6_clock_round_rate,
=======
	.determine_rate = cpg_div6_clock_determine_rate,
>>>>>>> upstream/android-13
	.set_rate = cpg_div6_clock_set_rate,
};

static int cpg_div6_clock_notifier_call(struct notifier_block *nb,
					unsigned long action, void *data)
{
	struct div6_clock *clock = container_of(nb, struct div6_clock, nb);

	switch (action) {
	case PM_EVENT_RESUME:
		/*
		 * TODO: This does not yet support DIV6 clocks with multiple
		 * parents, as the parent selection bits are not restored.
		 * Fortunately so far such DIV6 clocks are found only on
		 * R/SH-Mobile SoCs, while the resume functionality is only
		 * needed on R-Car Gen3.
		 */
		if (__clk_get_enable_count(clock->hw.clk))
			cpg_div6_clock_enable(&clock->hw);
		else
			cpg_div6_clock_disable(&clock->hw);
		return NOTIFY_OK;
	}

	return NOTIFY_DONE;
}

/**
 * cpg_div6_register - Register a DIV6 clock
 * @name: Name of the DIV6 clock
 * @num_parents: Number of parent clocks of the DIV6 clock (1, 4, or 8)
 * @parent_names: Array containing the names of the parent clocks
 * @reg: Mapped register used to control the DIV6 clock
 * @notifiers: Optional notifier chain to save/restore state for system resume
 */
struct clk * __init cpg_div6_register(const char *name,
				      unsigned int num_parents,
				      const char **parent_names,
				      void __iomem *reg,
				      struct raw_notifier_head *notifiers)
{
	unsigned int valid_parents;
	struct clk_init_data init = {};
	struct div6_clock *clock;
	struct clk *clk;
	unsigned int i;

<<<<<<< HEAD
	clock = kzalloc(sizeof(*clock), GFP_KERNEL);
	if (!clock)
		return ERR_PTR(-ENOMEM);

	clock->parents = kmalloc_array(num_parents, sizeof(*clock->parents),
				       GFP_KERNEL);
	if (!clock->parents) {
		clk = ERR_PTR(-ENOMEM);
		goto free_clock;
	}

=======
	clock = kzalloc(struct_size(clock, parents, num_parents), GFP_KERNEL);
	if (!clock)
		return ERR_PTR(-ENOMEM);

>>>>>>> upstream/android-13
	clock->reg = reg;

	/*
	 * Read the divisor. Disabling the clock overwrites the divisor, so we
	 * need to cache its value for the enable operation.
	 */
	clock->div = (readl(clock->reg) & CPG_DIV6_DIV_MASK) + 1;

	switch (num_parents) {
	case 1:
		/* fixed parent clock */
<<<<<<< HEAD
		clock->src_shift = clock->src_width = 0;
		break;
	case 4:
		/* clock with EXSRC bits 6-7 */
		clock->src_shift = 6;
		clock->src_width = 2;
		break;
	case 8:
		/* VCLK with EXSRC bits 12-14 */
		clock->src_shift = 12;
		clock->src_width = 3;
=======
		clock->src_mask = 0;
		break;
	case 4:
		/* clock with EXSRC bits 6-7 */
		clock->src_mask = GENMASK(7, 6);
		break;
	case 8:
		/* VCLK with EXSRC bits 12-14 */
		clock->src_mask = GENMASK(14, 12);
>>>>>>> upstream/android-13
		break;
	default:
		pr_err("%s: invalid number of parents for DIV6 clock %s\n",
		       __func__, name);
		clk = ERR_PTR(-EINVAL);
<<<<<<< HEAD
		goto free_parents;
=======
		goto free_clock;
>>>>>>> upstream/android-13
	}

	/* Filter out invalid parents */
	for (i = 0, valid_parents = 0; i < num_parents; i++) {
		if (parent_names[i]) {
			parent_names[valid_parents] = parent_names[i];
			clock->parents[valid_parents] = i;
			valid_parents++;
		}
	}

	/* Register the clock. */
	init.name = name;
	init.ops = &cpg_div6_clock_ops;
<<<<<<< HEAD
	init.flags = CLK_IS_BASIC;
=======
>>>>>>> upstream/android-13
	init.parent_names = parent_names;
	init.num_parents = valid_parents;

	clock->hw.init = &init;

	clk = clk_register(NULL, &clock->hw);
	if (IS_ERR(clk))
<<<<<<< HEAD
		goto free_parents;
=======
		goto free_clock;
>>>>>>> upstream/android-13

	if (notifiers) {
		clock->nb.notifier_call = cpg_div6_clock_notifier_call;
		raw_notifier_chain_register(notifiers, &clock->nb);
	}

	return clk;

<<<<<<< HEAD
free_parents:
	kfree(clock->parents);
=======
>>>>>>> upstream/android-13
free_clock:
	kfree(clock);
	return clk;
}

static void __init cpg_div6_clock_init(struct device_node *np)
{
	unsigned int num_parents;
	const char **parent_names;
	const char *clk_name = np->name;
	void __iomem *reg;
	struct clk *clk;
	unsigned int i;

	num_parents = of_clk_get_parent_count(np);
	if (num_parents < 1) {
<<<<<<< HEAD
		pr_err("%s: no parent found for %s DIV6 clock\n",
		       __func__, np->name);
=======
		pr_err("%s: no parent found for %pOFn DIV6 clock\n",
		       __func__, np);
>>>>>>> upstream/android-13
		return;
	}

	parent_names = kmalloc_array(num_parents, sizeof(*parent_names),
				GFP_KERNEL);
	if (!parent_names)
		return;

	reg = of_iomap(np, 0);
	if (reg == NULL) {
<<<<<<< HEAD
		pr_err("%s: failed to map %s DIV6 clock register\n",
		       __func__, np->name);
=======
		pr_err("%s: failed to map %pOFn DIV6 clock register\n",
		       __func__, np);
>>>>>>> upstream/android-13
		goto error;
	}

	/* Parse the DT properties. */
	of_property_read_string(np, "clock-output-names", &clk_name);

	for (i = 0; i < num_parents; i++)
		parent_names[i] = of_clk_get_parent_name(np, i);

	clk = cpg_div6_register(clk_name, num_parents, parent_names, reg, NULL);
	if (IS_ERR(clk)) {
<<<<<<< HEAD
		pr_err("%s: failed to register %s DIV6 clock (%ld)\n",
		       __func__, np->name, PTR_ERR(clk));
=======
		pr_err("%s: failed to register %pOFn DIV6 clock (%ld)\n",
		       __func__, np, PTR_ERR(clk));
>>>>>>> upstream/android-13
		goto error;
	}

	of_clk_add_provider(np, of_clk_src_simple_get, clk);

	kfree(parent_names);
	return;

error:
	if (reg)
		iounmap(reg);
	kfree(parent_names);
}
CLK_OF_DECLARE(cpg_div6_clk, "renesas,cpg-div6-clock", cpg_div6_clock_init);
