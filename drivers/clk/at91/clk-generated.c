<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *  Copyright (C) 2015 Atmel Corporation,
 *                     Nicolas Ferre <nicolas.ferre@atmel.com>
 *
 * Based on clk-programmable & clk-peripheral drivers by Boris BREZILLON.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

=======
 */

#include <linux/bitfield.h>
>>>>>>> upstream/android-13
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/clk/at91_pmc.h>
#include <linux/of.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>

#include "pmc.h"

<<<<<<< HEAD
#define PERIPHERAL_MAX		64
#define PERIPHERAL_ID_MIN	2

#define GENERATED_SOURCE_MAX	6
#define GENERATED_MAX_DIV	255

#define GCK_ID_SSC0		43
#define GCK_ID_SSC1		44
#define GCK_ID_I2S0		54
#define GCK_ID_I2S1		55
#define GCK_ID_CLASSD		59
#define GCK_INDEX_DT_AUDIO_PLL	5

=======
#define GENERATED_MAX_DIV	255

>>>>>>> upstream/android-13
struct clk_generated {
	struct clk_hw hw;
	struct regmap *regmap;
	struct clk_range range;
	spinlock_t *lock;
<<<<<<< HEAD
	u32 id;
	u32 gckdiv;
	u8 parent_id;
	bool audio_pll_allowed;
=======
	u32 *mux_table;
	u32 id;
	u32 gckdiv;
	const struct clk_pcr_layout *layout;
	u8 parent_id;
	int chg_pid;
>>>>>>> upstream/android-13
};

#define to_clk_generated(hw) \
	container_of(hw, struct clk_generated, hw)

static int clk_generated_enable(struct clk_hw *hw)
{
	struct clk_generated *gck = to_clk_generated(hw);
	unsigned long flags;

	pr_debug("GCLK: %s, gckdiv = %d, parent id = %d\n",
		 __func__, gck->gckdiv, gck->parent_id);

	spin_lock_irqsave(gck->lock, flags);
<<<<<<< HEAD
	regmap_write(gck->regmap, AT91_PMC_PCR,
		     (gck->id & AT91_PMC_PCR_PID_MASK));
	regmap_update_bits(gck->regmap, AT91_PMC_PCR,
			   AT91_PMC_PCR_GCKDIV_MASK | AT91_PMC_PCR_GCKCSS_MASK |
			   AT91_PMC_PCR_CMD | AT91_PMC_PCR_GCKEN,
			   AT91_PMC_PCR_GCKCSS(gck->parent_id) |
			   AT91_PMC_PCR_CMD |
			   AT91_PMC_PCR_GCKDIV(gck->gckdiv) |
=======
	regmap_write(gck->regmap, gck->layout->offset,
		     (gck->id & gck->layout->pid_mask));
	regmap_update_bits(gck->regmap, gck->layout->offset,
			   AT91_PMC_PCR_GCKDIV_MASK | gck->layout->gckcss_mask |
			   gck->layout->cmd | AT91_PMC_PCR_GCKEN,
			   field_prep(gck->layout->gckcss_mask, gck->parent_id) |
			   gck->layout->cmd |
			   FIELD_PREP(AT91_PMC_PCR_GCKDIV_MASK, gck->gckdiv) |
>>>>>>> upstream/android-13
			   AT91_PMC_PCR_GCKEN);
	spin_unlock_irqrestore(gck->lock, flags);
	return 0;
}

static void clk_generated_disable(struct clk_hw *hw)
{
	struct clk_generated *gck = to_clk_generated(hw);
	unsigned long flags;

	spin_lock_irqsave(gck->lock, flags);
<<<<<<< HEAD
	regmap_write(gck->regmap, AT91_PMC_PCR,
		     (gck->id & AT91_PMC_PCR_PID_MASK));
	regmap_update_bits(gck->regmap, AT91_PMC_PCR,
			   AT91_PMC_PCR_CMD | AT91_PMC_PCR_GCKEN,
			   AT91_PMC_PCR_CMD);
=======
	regmap_write(gck->regmap, gck->layout->offset,
		     (gck->id & gck->layout->pid_mask));
	regmap_update_bits(gck->regmap, gck->layout->offset,
			   gck->layout->cmd | AT91_PMC_PCR_GCKEN,
			   gck->layout->cmd);
>>>>>>> upstream/android-13
	spin_unlock_irqrestore(gck->lock, flags);
}

static int clk_generated_is_enabled(struct clk_hw *hw)
{
	struct clk_generated *gck = to_clk_generated(hw);
	unsigned long flags;
	unsigned int status;

	spin_lock_irqsave(gck->lock, flags);
<<<<<<< HEAD
	regmap_write(gck->regmap, AT91_PMC_PCR,
		     (gck->id & AT91_PMC_PCR_PID_MASK));
	regmap_read(gck->regmap, AT91_PMC_PCR, &status);
	spin_unlock_irqrestore(gck->lock, flags);

	return status & AT91_PMC_PCR_GCKEN ? 1 : 0;
=======
	regmap_write(gck->regmap, gck->layout->offset,
		     (gck->id & gck->layout->pid_mask));
	regmap_read(gck->regmap, gck->layout->offset, &status);
	spin_unlock_irqrestore(gck->lock, flags);

	return !!(status & AT91_PMC_PCR_GCKEN);
>>>>>>> upstream/android-13
}

static unsigned long
clk_generated_recalc_rate(struct clk_hw *hw,
			  unsigned long parent_rate)
{
	struct clk_generated *gck = to_clk_generated(hw);

	return DIV_ROUND_CLOSEST(parent_rate, gck->gckdiv + 1);
}

static void clk_generated_best_diff(struct clk_rate_request *req,
				    struct clk_hw *parent,
				    unsigned long parent_rate, u32 div,
				    int *best_diff, long *best_rate)
{
	unsigned long tmp_rate;
	int tmp_diff;

	if (!div)
		tmp_rate = parent_rate;
	else
		tmp_rate = parent_rate / div;
	tmp_diff = abs(req->rate - tmp_rate);

<<<<<<< HEAD
	if (*best_diff < 0 || *best_diff > tmp_diff) {
=======
	if (*best_diff < 0 || *best_diff >= tmp_diff) {
>>>>>>> upstream/android-13
		*best_rate = tmp_rate;
		*best_diff = tmp_diff;
		req->best_parent_rate = parent_rate;
		req->best_parent_hw = parent;
	}
}

static int clk_generated_determine_rate(struct clk_hw *hw,
					struct clk_rate_request *req)
{
	struct clk_generated *gck = to_clk_generated(hw);
	struct clk_hw *parent = NULL;
	struct clk_rate_request req_parent = *req;
	long best_rate = -EINVAL;
	unsigned long min_rate, parent_rate;
	int best_diff = -1;
	int i;
	u32 div;

<<<<<<< HEAD
	for (i = 0; i < clk_hw_get_num_parents(hw) - 1; i++) {
=======
	/* do not look for a rate that is outside of our range */
	if (gck->range.max && req->rate > gck->range.max)
		req->rate = gck->range.max;
	if (gck->range.min && req->rate < gck->range.min)
		req->rate = gck->range.min;

	for (i = 0; i < clk_hw_get_num_parents(hw); i++) {
		if (gck->chg_pid == i)
			continue;

>>>>>>> upstream/android-13
		parent = clk_hw_get_parent_by_index(hw, i);
		if (!parent)
			continue;

		parent_rate = clk_hw_get_rate(parent);
		min_rate = DIV_ROUND_CLOSEST(parent_rate, GENERATED_MAX_DIV + 1);
		if (!parent_rate ||
		    (gck->range.max && min_rate > gck->range.max))
			continue;

		div = DIV_ROUND_CLOSEST(parent_rate, req->rate);
		if (div > GENERATED_MAX_DIV + 1)
			div = GENERATED_MAX_DIV + 1;

		clk_generated_best_diff(req, parent, parent_rate, div,
					&best_diff, &best_rate);

		if (!best_diff)
			break;
	}

	/*
	 * The audio_pll rate can be modified, unlike the five others clocks
	 * that should never be altered.
	 * The audio_pll can technically be used by multiple consumers. However,
	 * with the rate locking, the first consumer to enable to clock will be
	 * the one definitely setting the rate of the clock.
	 * Since audio IPs are most likely to request the same rate, we enforce
	 * that the only clks able to modify gck rate are those of audio IPs.
	 */

<<<<<<< HEAD
	if (!gck->audio_pll_allowed)
		goto end;

	parent = clk_hw_get_parent_by_index(hw, GCK_INDEX_DT_AUDIO_PLL);
=======
	if (gck->chg_pid < 0)
		goto end;

	parent = clk_hw_get_parent_by_index(hw, gck->chg_pid);
>>>>>>> upstream/android-13
	if (!parent)
		goto end;

	for (div = 1; div < GENERATED_MAX_DIV + 2; div++) {
		req_parent.rate = req->rate * div;
<<<<<<< HEAD
		__clk_determine_rate(parent, &req_parent);
=======
		if (__clk_determine_rate(parent, &req_parent))
			continue;
>>>>>>> upstream/android-13
		clk_generated_best_diff(req, parent, req_parent.rate, div,
					&best_diff, &best_rate);

		if (!best_diff)
			break;
	}

end:
	pr_debug("GCLK: %s, best_rate = %ld, parent clk: %s @ %ld\n",
		 __func__, best_rate,
		 __clk_get_name((req->best_parent_hw)->clk),
		 req->best_parent_rate);

<<<<<<< HEAD
	if (best_rate < 0)
		return best_rate;
=======
	if (best_rate < 0 || (gck->range.max && best_rate > gck->range.max))
		return -EINVAL;
>>>>>>> upstream/android-13

	req->rate = best_rate;
	return 0;
}

/* No modification of hardware as we have the flag CLK_SET_PARENT_GATE set */
static int clk_generated_set_parent(struct clk_hw *hw, u8 index)
{
	struct clk_generated *gck = to_clk_generated(hw);

	if (index >= clk_hw_get_num_parents(hw))
		return -EINVAL;

<<<<<<< HEAD
	gck->parent_id = index;
=======
	if (gck->mux_table)
		gck->parent_id = clk_mux_index_to_val(gck->mux_table, 0, index);
	else
		gck->parent_id = index;

>>>>>>> upstream/android-13
	return 0;
}

static u8 clk_generated_get_parent(struct clk_hw *hw)
{
	struct clk_generated *gck = to_clk_generated(hw);

	return gck->parent_id;
}

/* No modification of hardware as we have the flag CLK_SET_RATE_GATE set */
static int clk_generated_set_rate(struct clk_hw *hw,
				  unsigned long rate,
				  unsigned long parent_rate)
{
	struct clk_generated *gck = to_clk_generated(hw);
	u32 div;

	if (!rate)
		return -EINVAL;

	if (gck->range.max && rate > gck->range.max)
		return -EINVAL;

	div = DIV_ROUND_CLOSEST(parent_rate, rate);
	if (div > GENERATED_MAX_DIV + 1 || !div)
		return -EINVAL;

	gck->gckdiv = div - 1;
	return 0;
}

static const struct clk_ops generated_ops = {
	.enable = clk_generated_enable,
	.disable = clk_generated_disable,
	.is_enabled = clk_generated_is_enabled,
	.recalc_rate = clk_generated_recalc_rate,
	.determine_rate = clk_generated_determine_rate,
	.get_parent = clk_generated_get_parent,
	.set_parent = clk_generated_set_parent,
	.set_rate = clk_generated_set_rate,
};

/**
 * clk_generated_startup - Initialize a given clock to its default parent and
 * divisor parameter.
 *
 * @gck:	Generated clock to set the startup parameters for.
 *
 * Take parameters from the hardware and update local clock configuration
 * accordingly.
 */
static void clk_generated_startup(struct clk_generated *gck)
{
	u32 tmp;
	unsigned long flags;

	spin_lock_irqsave(gck->lock, flags);
<<<<<<< HEAD
	regmap_write(gck->regmap, AT91_PMC_PCR,
		     (gck->id & AT91_PMC_PCR_PID_MASK));
	regmap_read(gck->regmap, AT91_PMC_PCR, &tmp);
	spin_unlock_irqrestore(gck->lock, flags);

	gck->parent_id = (tmp & AT91_PMC_PCR_GCKCSS_MASK)
					>> AT91_PMC_PCR_GCKCSS_OFFSET;
	gck->gckdiv = (tmp & AT91_PMC_PCR_GCKDIV_MASK)
					>> AT91_PMC_PCR_GCKDIV_OFFSET;
}

static struct clk_hw * __init
at91_clk_register_generated(struct regmap *regmap, spinlock_t *lock,
			    const char *name, const char **parent_names,
			    u8 num_parents, u8 id, bool pll_audio,
			    const struct clk_range *range)
{
	struct clk_generated *gck;
	struct clk_init_data init = {};
=======
	regmap_write(gck->regmap, gck->layout->offset,
		     (gck->id & gck->layout->pid_mask));
	regmap_read(gck->regmap, gck->layout->offset, &tmp);
	spin_unlock_irqrestore(gck->lock, flags);

	gck->parent_id = field_get(gck->layout->gckcss_mask, tmp);
	gck->gckdiv = FIELD_GET(AT91_PMC_PCR_GCKDIV_MASK, tmp);
}

struct clk_hw * __init
at91_clk_register_generated(struct regmap *regmap, spinlock_t *lock,
			    const struct clk_pcr_layout *layout,
			    const char *name, const char **parent_names,
			    u32 *mux_table, u8 num_parents, u8 id,
			    const struct clk_range *range,
			    int chg_pid)
{
	struct clk_generated *gck;
	struct clk_init_data init;
>>>>>>> upstream/android-13
	struct clk_hw *hw;
	int ret;

	gck = kzalloc(sizeof(*gck), GFP_KERNEL);
	if (!gck)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &generated_ops;
	init.parent_names = parent_names;
	init.num_parents = num_parents;
<<<<<<< HEAD
	init.flags = CLK_SET_RATE_GATE | CLK_SET_PARENT_GATE |
		CLK_SET_RATE_PARENT;
=======
	init.flags = CLK_SET_RATE_GATE | CLK_SET_PARENT_GATE;
	if (chg_pid >= 0)
		init.flags |= CLK_SET_RATE_PARENT;
>>>>>>> upstream/android-13

	gck->id = id;
	gck->hw.init = &init;
	gck->regmap = regmap;
	gck->lock = lock;
	gck->range = *range;
<<<<<<< HEAD
	gck->audio_pll_allowed = pll_audio;
=======
	gck->chg_pid = chg_pid;
	gck->layout = layout;
	gck->mux_table = mux_table;
>>>>>>> upstream/android-13

	clk_generated_startup(gck);
	hw = &gck->hw;
	ret = clk_hw_register(NULL, &gck->hw);
	if (ret) {
		kfree(gck);
		hw = ERR_PTR(ret);
	} else {
		pmc_register_id(id);
	}

	return hw;
}
<<<<<<< HEAD

static void __init of_sama5d2_clk_generated_setup(struct device_node *np)
{
	int num;
	u32 id;
	const char *name;
	struct clk_hw *hw;
	unsigned int num_parents;
	const char *parent_names[GENERATED_SOURCE_MAX];
	struct device_node *gcknp;
	struct clk_range range = CLK_RANGE(0, 0);
	struct regmap *regmap;

	num_parents = of_clk_get_parent_count(np);
	if (num_parents == 0 || num_parents > GENERATED_SOURCE_MAX)
		return;

	of_clk_parent_fill(np, parent_names, num_parents);

	num = of_get_child_count(np);
	if (!num || num > PERIPHERAL_MAX)
		return;

	regmap = syscon_node_to_regmap(of_get_parent(np));
	if (IS_ERR(regmap))
		return;

	for_each_child_of_node(np, gcknp) {
		bool pll_audio = false;

		if (of_property_read_u32(gcknp, "reg", &id))
			continue;

		if (id < PERIPHERAL_ID_MIN || id >= PERIPHERAL_MAX)
			continue;

		if (of_property_read_string(np, "clock-output-names", &name))
			name = gcknp->name;

		of_at91_get_clk_range(gcknp, "atmel,clk-output-range",
				      &range);

		if (of_device_is_compatible(np, "atmel,sama5d2-clk-generated") &&
		    (id == GCK_ID_I2S0 || id == GCK_ID_I2S1 ||
		     id == GCK_ID_CLASSD))
			pll_audio = true;

		hw = at91_clk_register_generated(regmap, &pmc_pcr_lock, name,
						  parent_names, num_parents,
						  id, pll_audio, &range);
		if (IS_ERR(hw))
			continue;

		of_clk_add_hw_provider(gcknp, of_clk_hw_simple_get, hw);
	}
}
CLK_OF_DECLARE(of_sama5d2_clk_generated_setup, "atmel,sama5d2-clk-generated",
	       of_sama5d2_clk_generated_setup);
=======
>>>>>>> upstream/android-13
