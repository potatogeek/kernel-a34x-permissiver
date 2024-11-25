<<<<<<< HEAD
/*
 *  Copyright (C) 2013 Boris BREZILLON <b.brezillon@overkiz.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2013 Boris BREZILLON <b.brezillon@overkiz.com>
 */

#include <linux/bitops.h>
>>>>>>> upstream/android-13
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/clk/at91_pmc.h>
#include <linux/of.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>

#include "pmc.h"

DEFINE_SPINLOCK(pmc_pcr_lock);

<<<<<<< HEAD
#define PERIPHERAL_MAX		64

#define PERIPHERAL_AT91RM9200	0
#define PERIPHERAL_AT91SAM9X5	1

=======
>>>>>>> upstream/android-13
#define PERIPHERAL_ID_MIN	2
#define PERIPHERAL_ID_MAX	31
#define PERIPHERAL_MASK(id)	(1 << ((id) & PERIPHERAL_ID_MAX))

<<<<<<< HEAD
#define PERIPHERAL_RSHIFT_MASK	0x3
#define PERIPHERAL_RSHIFT(val)	(((val) >> 16) & PERIPHERAL_RSHIFT_MASK)

=======
>>>>>>> upstream/android-13
#define PERIPHERAL_MAX_SHIFT	3

struct clk_peripheral {
	struct clk_hw hw;
	struct regmap *regmap;
	u32 id;
};

#define to_clk_peripheral(hw) container_of(hw, struct clk_peripheral, hw)

struct clk_sam9x5_peripheral {
	struct clk_hw hw;
	struct regmap *regmap;
	struct clk_range range;
	spinlock_t *lock;
	u32 id;
	u32 div;
<<<<<<< HEAD
	bool auto_div;
=======
	const struct clk_pcr_layout *layout;
	bool auto_div;
	int chg_pid;
>>>>>>> upstream/android-13
};

#define to_clk_sam9x5_peripheral(hw) \
	container_of(hw, struct clk_sam9x5_peripheral, hw)

static int clk_peripheral_enable(struct clk_hw *hw)
{
	struct clk_peripheral *periph = to_clk_peripheral(hw);
	int offset = AT91_PMC_PCER;
	u32 id = periph->id;

	if (id < PERIPHERAL_ID_MIN)
		return 0;
	if (id > PERIPHERAL_ID_MAX)
		offset = AT91_PMC_PCER1;
	regmap_write(periph->regmap, offset, PERIPHERAL_MASK(id));

	return 0;
}

static void clk_peripheral_disable(struct clk_hw *hw)
{
	struct clk_peripheral *periph = to_clk_peripheral(hw);
	int offset = AT91_PMC_PCDR;
	u32 id = periph->id;

	if (id < PERIPHERAL_ID_MIN)
		return;
	if (id > PERIPHERAL_ID_MAX)
		offset = AT91_PMC_PCDR1;
	regmap_write(periph->regmap, offset, PERIPHERAL_MASK(id));
}

static int clk_peripheral_is_enabled(struct clk_hw *hw)
{
	struct clk_peripheral *periph = to_clk_peripheral(hw);
	int offset = AT91_PMC_PCSR;
	unsigned int status;
	u32 id = periph->id;

	if (id < PERIPHERAL_ID_MIN)
		return 1;
	if (id > PERIPHERAL_ID_MAX)
		offset = AT91_PMC_PCSR1;
	regmap_read(periph->regmap, offset, &status);

	return status & PERIPHERAL_MASK(id) ? 1 : 0;
}

static const struct clk_ops peripheral_ops = {
	.enable = clk_peripheral_enable,
	.disable = clk_peripheral_disable,
	.is_enabled = clk_peripheral_is_enabled,
};

<<<<<<< HEAD
static struct clk_hw * __init
=======
struct clk_hw * __init
>>>>>>> upstream/android-13
at91_clk_register_peripheral(struct regmap *regmap, const char *name,
			     const char *parent_name, u32 id)
{
	struct clk_peripheral *periph;
<<<<<<< HEAD
	struct clk_init_data init = {};
=======
	struct clk_init_data init;
>>>>>>> upstream/android-13
	struct clk_hw *hw;
	int ret;

	if (!name || !parent_name || id > PERIPHERAL_ID_MAX)
		return ERR_PTR(-EINVAL);

	periph = kzalloc(sizeof(*periph), GFP_KERNEL);
	if (!periph)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &peripheral_ops;
<<<<<<< HEAD
	init.parent_names = (parent_name ? &parent_name : NULL);
	init.num_parents = (parent_name ? 1 : 0);
=======
	init.parent_names = &parent_name;
	init.num_parents = 1;
>>>>>>> upstream/android-13
	init.flags = 0;

	periph->id = id;
	periph->hw.init = &init;
	periph->regmap = regmap;

	hw = &periph->hw;
	ret = clk_hw_register(NULL, &periph->hw);
	if (ret) {
		kfree(periph);
		hw = ERR_PTR(ret);
	}

	return hw;
}

static void clk_sam9x5_peripheral_autodiv(struct clk_sam9x5_peripheral *periph)
{
	struct clk_hw *parent;
	unsigned long parent_rate;
	int shift = 0;

	if (!periph->auto_div)
		return;

	if (periph->range.max) {
		parent = clk_hw_get_parent_by_index(&periph->hw, 0);
		parent_rate = clk_hw_get_rate(parent);
		if (!parent_rate)
			return;

		for (; shift < PERIPHERAL_MAX_SHIFT; shift++) {
			if (parent_rate >> shift <= periph->range.max)
				break;
		}
	}

	periph->auto_div = false;
	periph->div = shift;
}

static int clk_sam9x5_peripheral_enable(struct clk_hw *hw)
{
	struct clk_sam9x5_peripheral *periph = to_clk_sam9x5_peripheral(hw);
	unsigned long flags;

	if (periph->id < PERIPHERAL_ID_MIN)
		return 0;

	spin_lock_irqsave(periph->lock, flags);
<<<<<<< HEAD
	regmap_write(periph->regmap, AT91_PMC_PCR,
		     (periph->id & AT91_PMC_PCR_PID_MASK));
	regmap_update_bits(periph->regmap, AT91_PMC_PCR,
			   AT91_PMC_PCR_DIV_MASK | AT91_PMC_PCR_CMD |
			   AT91_PMC_PCR_EN,
			   AT91_PMC_PCR_DIV(periph->div) |
			   AT91_PMC_PCR_CMD |
=======
	regmap_write(periph->regmap, periph->layout->offset,
		     (periph->id & periph->layout->pid_mask));
	regmap_update_bits(periph->regmap, periph->layout->offset,
			   periph->layout->div_mask | periph->layout->cmd |
			   AT91_PMC_PCR_EN,
			   field_prep(periph->layout->div_mask, periph->div) |
			   periph->layout->cmd |
>>>>>>> upstream/android-13
			   AT91_PMC_PCR_EN);
	spin_unlock_irqrestore(periph->lock, flags);

	return 0;
}

static void clk_sam9x5_peripheral_disable(struct clk_hw *hw)
{
	struct clk_sam9x5_peripheral *periph = to_clk_sam9x5_peripheral(hw);
	unsigned long flags;

	if (periph->id < PERIPHERAL_ID_MIN)
		return;

	spin_lock_irqsave(periph->lock, flags);
<<<<<<< HEAD
	regmap_write(periph->regmap, AT91_PMC_PCR,
		     (periph->id & AT91_PMC_PCR_PID_MASK));
	regmap_update_bits(periph->regmap, AT91_PMC_PCR,
			   AT91_PMC_PCR_EN | AT91_PMC_PCR_CMD,
			   AT91_PMC_PCR_CMD);
=======
	regmap_write(periph->regmap, periph->layout->offset,
		     (periph->id & periph->layout->pid_mask));
	regmap_update_bits(periph->regmap, periph->layout->offset,
			   AT91_PMC_PCR_EN | periph->layout->cmd,
			   periph->layout->cmd);
>>>>>>> upstream/android-13
	spin_unlock_irqrestore(periph->lock, flags);
}

static int clk_sam9x5_peripheral_is_enabled(struct clk_hw *hw)
{
	struct clk_sam9x5_peripheral *periph = to_clk_sam9x5_peripheral(hw);
	unsigned long flags;
	unsigned int status;

	if (periph->id < PERIPHERAL_ID_MIN)
		return 1;

	spin_lock_irqsave(periph->lock, flags);
<<<<<<< HEAD
	regmap_write(periph->regmap, AT91_PMC_PCR,
		     (periph->id & AT91_PMC_PCR_PID_MASK));
	regmap_read(periph->regmap, AT91_PMC_PCR, &status);
	spin_unlock_irqrestore(periph->lock, flags);

	return status & AT91_PMC_PCR_EN ? 1 : 0;
=======
	regmap_write(periph->regmap, periph->layout->offset,
		     (periph->id & periph->layout->pid_mask));
	regmap_read(periph->regmap, periph->layout->offset, &status);
	spin_unlock_irqrestore(periph->lock, flags);

	return !!(status & AT91_PMC_PCR_EN);
>>>>>>> upstream/android-13
}

static unsigned long
clk_sam9x5_peripheral_recalc_rate(struct clk_hw *hw,
				  unsigned long parent_rate)
{
	struct clk_sam9x5_peripheral *periph = to_clk_sam9x5_peripheral(hw);
	unsigned long flags;
	unsigned int status;

	if (periph->id < PERIPHERAL_ID_MIN)
		return parent_rate;

	spin_lock_irqsave(periph->lock, flags);
<<<<<<< HEAD
	regmap_write(periph->regmap, AT91_PMC_PCR,
		     (periph->id & AT91_PMC_PCR_PID_MASK));
	regmap_read(periph->regmap, AT91_PMC_PCR, &status);
	spin_unlock_irqrestore(periph->lock, flags);

	if (status & AT91_PMC_PCR_EN) {
		periph->div = PERIPHERAL_RSHIFT(status);
=======
	regmap_write(periph->regmap, periph->layout->offset,
		     (periph->id & periph->layout->pid_mask));
	regmap_read(periph->regmap, periph->layout->offset, &status);
	spin_unlock_irqrestore(periph->lock, flags);

	if (status & AT91_PMC_PCR_EN) {
		periph->div = field_get(periph->layout->div_mask, status);
>>>>>>> upstream/android-13
		periph->auto_div = false;
	} else {
		clk_sam9x5_peripheral_autodiv(periph);
	}

	return parent_rate >> periph->div;
}

<<<<<<< HEAD
=======
static void clk_sam9x5_peripheral_best_diff(struct clk_rate_request *req,
					    struct clk_hw *parent,
					    unsigned long parent_rate,
					    u32 shift, long *best_diff,
					    long *best_rate)
{
	unsigned long tmp_rate = parent_rate >> shift;
	unsigned long tmp_diff = abs(req->rate - tmp_rate);

	if (*best_diff < 0 || *best_diff >= tmp_diff) {
		*best_rate = tmp_rate;
		*best_diff = tmp_diff;
		req->best_parent_rate = parent_rate;
		req->best_parent_hw = parent;
	}
}

static int clk_sam9x5_peripheral_determine_rate(struct clk_hw *hw,
						struct clk_rate_request *req)
{
	struct clk_sam9x5_peripheral *periph = to_clk_sam9x5_peripheral(hw);
	struct clk_hw *parent = clk_hw_get_parent(hw);
	struct clk_rate_request req_parent = *req;
	unsigned long parent_rate = clk_hw_get_rate(parent);
	unsigned long tmp_rate;
	long best_rate = LONG_MIN;
	long best_diff = LONG_MIN;
	u32 shift;

	if (periph->id < PERIPHERAL_ID_MIN || !periph->range.max)
		return parent_rate;

	/* Fist step: check the available dividers. */
	for (shift = 0; shift <= PERIPHERAL_MAX_SHIFT; shift++) {
		tmp_rate = parent_rate >> shift;

		if (periph->range.max && tmp_rate > periph->range.max)
			continue;

		clk_sam9x5_peripheral_best_diff(req, parent, parent_rate,
						shift, &best_diff, &best_rate);

		if (!best_diff || best_rate <= req->rate)
			break;
	}

	if (periph->chg_pid < 0)
		goto end;

	/* Step two: try to request rate from parent. */
	parent = clk_hw_get_parent_by_index(hw, periph->chg_pid);
	if (!parent)
		goto end;

	for (shift = 0; shift <= PERIPHERAL_MAX_SHIFT; shift++) {
		req_parent.rate = req->rate << shift;

		if (__clk_determine_rate(parent, &req_parent))
			continue;

		clk_sam9x5_peripheral_best_diff(req, parent, req_parent.rate,
						shift, &best_diff, &best_rate);

		if (!best_diff)
			break;
	}
end:
	if (best_rate < 0 ||
	    (periph->range.max && best_rate > periph->range.max))
		return -EINVAL;

	pr_debug("PCK: %s, best_rate = %ld, parent clk: %s @ %ld\n",
		 __func__, best_rate,
		 __clk_get_name((req->best_parent_hw)->clk),
		 req->best_parent_rate);

	req->rate = best_rate;

	return 0;
}

>>>>>>> upstream/android-13
static long clk_sam9x5_peripheral_round_rate(struct clk_hw *hw,
					     unsigned long rate,
					     unsigned long *parent_rate)
{
	int shift = 0;
	unsigned long best_rate;
	unsigned long best_diff;
	unsigned long cur_rate = *parent_rate;
	unsigned long cur_diff;
	struct clk_sam9x5_peripheral *periph = to_clk_sam9x5_peripheral(hw);

	if (periph->id < PERIPHERAL_ID_MIN || !periph->range.max)
		return *parent_rate;

	if (periph->range.max) {
		for (; shift <= PERIPHERAL_MAX_SHIFT; shift++) {
			cur_rate = *parent_rate >> shift;
			if (cur_rate <= periph->range.max)
				break;
		}
	}

	if (rate >= cur_rate)
		return cur_rate;

	best_diff = cur_rate - rate;
	best_rate = cur_rate;
	for (; shift <= PERIPHERAL_MAX_SHIFT; shift++) {
		cur_rate = *parent_rate >> shift;
		if (cur_rate < rate)
			cur_diff = rate - cur_rate;
		else
			cur_diff = cur_rate - rate;

		if (cur_diff < best_diff) {
			best_diff = cur_diff;
			best_rate = cur_rate;
		}

		if (!best_diff || cur_rate < rate)
			break;
	}

	return best_rate;
}

static int clk_sam9x5_peripheral_set_rate(struct clk_hw *hw,
					  unsigned long rate,
					  unsigned long parent_rate)
{
	int shift;
	struct clk_sam9x5_peripheral *periph = to_clk_sam9x5_peripheral(hw);
	if (periph->id < PERIPHERAL_ID_MIN || !periph->range.max) {
		if (parent_rate == rate)
			return 0;
		else
			return -EINVAL;
	}

	if (periph->range.max && rate > periph->range.max)
		return -EINVAL;

	for (shift = 0; shift <= PERIPHERAL_MAX_SHIFT; shift++) {
		if (parent_rate >> shift == rate) {
			periph->auto_div = false;
			periph->div = shift;
			return 0;
		}
	}

	return -EINVAL;
}

static const struct clk_ops sam9x5_peripheral_ops = {
	.enable = clk_sam9x5_peripheral_enable,
	.disable = clk_sam9x5_peripheral_disable,
	.is_enabled = clk_sam9x5_peripheral_is_enabled,
	.recalc_rate = clk_sam9x5_peripheral_recalc_rate,
	.round_rate = clk_sam9x5_peripheral_round_rate,
	.set_rate = clk_sam9x5_peripheral_set_rate,
};

<<<<<<< HEAD
static struct clk_hw * __init
at91_clk_register_sam9x5_peripheral(struct regmap *regmap, spinlock_t *lock,
				    const char *name, const char *parent_name,
				    u32 id, const struct clk_range *range)
{
	struct clk_sam9x5_peripheral *periph;
	struct clk_init_data init = {};
=======
static const struct clk_ops sam9x5_peripheral_chg_ops = {
	.enable = clk_sam9x5_peripheral_enable,
	.disable = clk_sam9x5_peripheral_disable,
	.is_enabled = clk_sam9x5_peripheral_is_enabled,
	.recalc_rate = clk_sam9x5_peripheral_recalc_rate,
	.determine_rate = clk_sam9x5_peripheral_determine_rate,
	.set_rate = clk_sam9x5_peripheral_set_rate,
};

struct clk_hw * __init
at91_clk_register_sam9x5_peripheral(struct regmap *regmap, spinlock_t *lock,
				    const struct clk_pcr_layout *layout,
				    const char *name, const char *parent_name,
				    u32 id, const struct clk_range *range,
				    int chg_pid)
{
	struct clk_sam9x5_peripheral *periph;
	struct clk_init_data init;
>>>>>>> upstream/android-13
	struct clk_hw *hw;
	int ret;

	if (!name || !parent_name)
		return ERR_PTR(-EINVAL);

	periph = kzalloc(sizeof(*periph), GFP_KERNEL);
	if (!periph)
		return ERR_PTR(-ENOMEM);

	init.name = name;
<<<<<<< HEAD
	init.ops = &sam9x5_peripheral_ops;
	init.parent_names = (parent_name ? &parent_name : NULL);
	init.num_parents = (parent_name ? 1 : 0);
	init.flags = 0;
=======
	init.parent_names = &parent_name;
	init.num_parents = 1;
	if (chg_pid < 0) {
		init.flags = 0;
		init.ops = &sam9x5_peripheral_ops;
	} else {
		init.flags = CLK_SET_RATE_GATE | CLK_SET_PARENT_GATE |
			     CLK_SET_RATE_PARENT;
		init.ops = &sam9x5_peripheral_chg_ops;
	}
>>>>>>> upstream/android-13

	periph->id = id;
	periph->hw.init = &init;
	periph->div = 0;
	periph->regmap = regmap;
	periph->lock = lock;
<<<<<<< HEAD
	periph->auto_div = true;
	periph->range = *range;
=======
	if (layout->div_mask)
		periph->auto_div = true;
	periph->layout = layout;
	periph->range = *range;
	periph->chg_pid = chg_pid;
>>>>>>> upstream/android-13

	hw = &periph->hw;
	ret = clk_hw_register(NULL, &periph->hw);
	if (ret) {
		kfree(periph);
		hw = ERR_PTR(ret);
	} else {
		clk_sam9x5_peripheral_autodiv(periph);
		pmc_register_id(id);
	}

	return hw;
}
<<<<<<< HEAD

static void __init
of_at91_clk_periph_setup(struct device_node *np, u8 type)
{
	int num;
	u32 id;
	struct clk_hw *hw;
	const char *parent_name;
	const char *name;
	struct device_node *periphclknp;
	struct regmap *regmap;

	parent_name = of_clk_get_parent_name(np, 0);
	if (!parent_name)
		return;

	num = of_get_child_count(np);
	if (!num || num > PERIPHERAL_MAX)
		return;

	regmap = syscon_node_to_regmap(of_get_parent(np));
	if (IS_ERR(regmap))
		return;

	for_each_child_of_node(np, periphclknp) {
		if (of_property_read_u32(periphclknp, "reg", &id))
			continue;

		if (id >= PERIPHERAL_MAX)
			continue;

		if (of_property_read_string(np, "clock-output-names", &name))
			name = periphclknp->name;

		if (type == PERIPHERAL_AT91RM9200) {
			hw = at91_clk_register_peripheral(regmap, name,
							   parent_name, id);
		} else {
			struct clk_range range = CLK_RANGE(0, 0);

			of_at91_get_clk_range(periphclknp,
					      "atmel,clk-output-range",
					      &range);

			hw = at91_clk_register_sam9x5_peripheral(regmap,
								  &pmc_pcr_lock,
								  name,
								  parent_name,
								  id, &range);
		}

		if (IS_ERR(hw))
			continue;

		of_clk_add_hw_provider(periphclknp, of_clk_hw_simple_get, hw);
	}
}

static void __init of_at91rm9200_clk_periph_setup(struct device_node *np)
{
	of_at91_clk_periph_setup(np, PERIPHERAL_AT91RM9200);
}
CLK_OF_DECLARE(at91rm9200_clk_periph, "atmel,at91rm9200-clk-peripheral",
	       of_at91rm9200_clk_periph_setup);

static void __init of_at91sam9x5_clk_periph_setup(struct device_node *np)
{
	of_at91_clk_periph_setup(np, PERIPHERAL_AT91SAM9X5);
}
CLK_OF_DECLARE(at91sam9x5_clk_periph, "atmel,at91sam9x5-clk-peripheral",
	       of_at91sam9x5_clk_periph_setup);

=======
>>>>>>> upstream/android-13
