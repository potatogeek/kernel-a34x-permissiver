<<<<<<< HEAD
/*
 *  Copyright (C) 2013 Boris BREZILLON <b.brezillon@overkiz.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2013 Boris BREZILLON <b.brezillon@overkiz.com>
>>>>>>> upstream/android-13
 */

#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/clk/at91_pmc.h>
#include <linux/of.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>

#include "pmc.h"

<<<<<<< HEAD
#define MASTER_SOURCE_MAX	4

#define MASTER_PRES_MASK	0x7
#define MASTER_PRES_MAX		MASTER_PRES_MASK
#define MASTER_DIV_SHIFT	8
#define MASTER_DIV_MASK		0x3

struct clk_master_characteristics {
	struct clk_range output;
	u32 divisors[4];
	u8 have_div3_pres;
};

struct clk_master_layout {
	u32 mask;
	u8 pres_shift;
};
=======
#define MASTER_PRES_MASK	0x7
#define MASTER_PRES_MAX		MASTER_PRES_MASK
#define MASTER_DIV_SHIFT	8
#define MASTER_DIV_MASK		0x7

#define PMC_MCR			0x30
#define PMC_MCR_ID_MSK		GENMASK(3, 0)
#define PMC_MCR_CMD		BIT(7)
#define PMC_MCR_DIV		GENMASK(10, 8)
#define PMC_MCR_CSS		GENMASK(20, 16)
#define PMC_MCR_CSS_SHIFT	(16)
#define PMC_MCR_EN		BIT(28)

#define PMC_MCR_ID(x)		((x) & PMC_MCR_ID_MSK)

#define MASTER_MAX_ID		4
>>>>>>> upstream/android-13

#define to_clk_master(hw) container_of(hw, struct clk_master, hw)

struct clk_master {
	struct clk_hw hw;
	struct regmap *regmap;
<<<<<<< HEAD
	const struct clk_master_layout *layout;
	const struct clk_master_characteristics *characteristics;
};

static inline bool clk_master_ready(struct regmap *regmap)
{
	unsigned int status;

	regmap_read(regmap, AT91_PMC_SR, &status);

	return status & AT91_PMC_MCKRDY ? 1 : 0;
=======
	spinlock_t *lock;
	const struct clk_master_layout *layout;
	const struct clk_master_characteristics *characteristics;
	u32 *mux_table;
	u32 mckr;
	int chg_pid;
	u8 id;
	u8 parent;
	u8 div;
};

static inline bool clk_master_ready(struct clk_master *master)
{
	unsigned int bit = master->id ? AT91_PMC_MCKXRDY : AT91_PMC_MCKRDY;
	unsigned int status;

	regmap_read(master->regmap, AT91_PMC_SR, &status);

	return !!(status & bit);
>>>>>>> upstream/android-13
}

static int clk_master_prepare(struct clk_hw *hw)
{
	struct clk_master *master = to_clk_master(hw);
<<<<<<< HEAD

	while (!clk_master_ready(master->regmap))
		cpu_relax();

=======
	unsigned long flags;

	spin_lock_irqsave(master->lock, flags);

	while (!clk_master_ready(master))
		cpu_relax();

	spin_unlock_irqrestore(master->lock, flags);

>>>>>>> upstream/android-13
	return 0;
}

static int clk_master_is_prepared(struct clk_hw *hw)
{
	struct clk_master *master = to_clk_master(hw);
<<<<<<< HEAD

	return clk_master_ready(master->regmap);
}

static unsigned long clk_master_recalc_rate(struct clk_hw *hw,
					    unsigned long parent_rate)
{
	u8 pres;
	u8 div;
	unsigned long rate = parent_rate;
=======
	unsigned long flags;
	bool status;

	spin_lock_irqsave(master->lock, flags);
	status = clk_master_ready(master);
	spin_unlock_irqrestore(master->lock, flags);

	return status;
}

static unsigned long clk_master_div_recalc_rate(struct clk_hw *hw,
						unsigned long parent_rate)
{
	u8 div;
	unsigned long flags, rate = parent_rate;
>>>>>>> upstream/android-13
	struct clk_master *master = to_clk_master(hw);
	const struct clk_master_layout *layout = master->layout;
	const struct clk_master_characteristics *characteristics =
						master->characteristics;
	unsigned int mckr;

<<<<<<< HEAD
	regmap_read(master->regmap, AT91_PMC_MCKR, &mckr);
	mckr &= layout->mask;

	pres = (mckr >> layout->pres_shift) & MASTER_PRES_MASK;
	div = (mckr >> MASTER_DIV_SHIFT) & MASTER_DIV_MASK;

	if (characteristics->have_div3_pres && pres == MASTER_PRES_MAX)
		rate /= 3;
	else
		rate >>= pres;

	rate /= characteristics->divisors[div];

	if (rate < characteristics->output.min)
		pr_warn("master clk is underclocked");
	else if (rate > characteristics->output.max)
		pr_warn("master clk is overclocked");
=======
	spin_lock_irqsave(master->lock, flags);
	regmap_read(master->regmap, master->layout->offset, &mckr);
	spin_unlock_irqrestore(master->lock, flags);

	mckr &= layout->mask;

	div = (mckr >> MASTER_DIV_SHIFT) & MASTER_DIV_MASK;

	rate /= characteristics->divisors[div];

	if (rate < characteristics->output.min)
		pr_warn("master clk div is underclocked");
	else if (rate > characteristics->output.max)
		pr_warn("master clk div is overclocked");
>>>>>>> upstream/android-13

	return rate;
}

<<<<<<< HEAD
static u8 clk_master_get_parent(struct clk_hw *hw)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned int mckr;

	regmap_read(master->regmap, AT91_PMC_MCKR, &mckr);
=======
static const struct clk_ops master_div_ops = {
	.prepare = clk_master_prepare,
	.is_prepared = clk_master_is_prepared,
	.recalc_rate = clk_master_div_recalc_rate,
};

static int clk_master_div_set_rate(struct clk_hw *hw, unsigned long rate,
				   unsigned long parent_rate)
{
	struct clk_master *master = to_clk_master(hw);
	const struct clk_master_characteristics *characteristics =
						master->characteristics;
	unsigned long flags;
	int div, i;

	div = DIV_ROUND_CLOSEST(parent_rate, rate);
	if (div > ARRAY_SIZE(characteristics->divisors))
		return -EINVAL;

	for (i = 0; i < ARRAY_SIZE(characteristics->divisors); i++) {
		if (!characteristics->divisors[i])
			break;

		if (div == characteristics->divisors[i]) {
			div = i;
			break;
		}
	}

	if (i == ARRAY_SIZE(characteristics->divisors))
		return -EINVAL;

	spin_lock_irqsave(master->lock, flags);
	regmap_update_bits(master->regmap, master->layout->offset,
			   (MASTER_DIV_MASK << MASTER_DIV_SHIFT),
			   (div << MASTER_DIV_SHIFT));
	while (!clk_master_ready(master))
		cpu_relax();
	spin_unlock_irqrestore(master->lock, flags);

	return 0;
}

static int clk_master_div_determine_rate(struct clk_hw *hw,
					 struct clk_rate_request *req)
{
	struct clk_master *master = to_clk_master(hw);
	const struct clk_master_characteristics *characteristics =
						master->characteristics;
	struct clk_hw *parent;
	unsigned long parent_rate, tmp_rate, best_rate = 0;
	int i, best_diff = INT_MIN, tmp_diff;

	parent = clk_hw_get_parent(hw);
	if (!parent)
		return -EINVAL;

	parent_rate = clk_hw_get_rate(parent);
	if (!parent_rate)
		return -EINVAL;

	for (i = 0; i < ARRAY_SIZE(characteristics->divisors); i++) {
		if (!characteristics->divisors[i])
			break;

		tmp_rate = DIV_ROUND_CLOSEST_ULL(parent_rate,
						 characteristics->divisors[i]);
		tmp_diff = abs(tmp_rate - req->rate);

		if (!best_rate || best_diff > tmp_diff) {
			best_diff = tmp_diff;
			best_rate = tmp_rate;
		}

		if (!best_diff)
			break;
	}

	req->best_parent_rate = best_rate;
	req->best_parent_hw = parent;
	req->rate = best_rate;

	return 0;
}

static const struct clk_ops master_div_ops_chg = {
	.prepare = clk_master_prepare,
	.is_prepared = clk_master_is_prepared,
	.recalc_rate = clk_master_div_recalc_rate,
	.determine_rate = clk_master_div_determine_rate,
	.set_rate = clk_master_div_set_rate,
};

static void clk_sama7g5_master_best_diff(struct clk_rate_request *req,
					 struct clk_hw *parent,
					 unsigned long parent_rate,
					 long *best_rate,
					 long *best_diff,
					 u32 div)
{
	unsigned long tmp_rate, tmp_diff;

	if (div == MASTER_PRES_MAX)
		tmp_rate = parent_rate / 3;
	else
		tmp_rate = parent_rate >> div;

	tmp_diff = abs(req->rate - tmp_rate);

	if (*best_diff < 0 || *best_diff >= tmp_diff) {
		*best_rate = tmp_rate;
		*best_diff = tmp_diff;
		req->best_parent_rate = parent_rate;
		req->best_parent_hw = parent;
	}
}

static int clk_master_pres_determine_rate(struct clk_hw *hw,
					  struct clk_rate_request *req)
{
	struct clk_master *master = to_clk_master(hw);
	struct clk_rate_request req_parent = *req;
	const struct clk_master_characteristics *characteristics =
							master->characteristics;
	struct clk_hw *parent;
	long best_rate = LONG_MIN, best_diff = LONG_MIN;
	u32 pres;
	int i;

	if (master->chg_pid < 0)
		return -EOPNOTSUPP;

	parent = clk_hw_get_parent_by_index(hw, master->chg_pid);
	if (!parent)
		return -EOPNOTSUPP;

	for (i = 0; i <= MASTER_PRES_MAX; i++) {
		if (characteristics->have_div3_pres && i == MASTER_PRES_MAX)
			pres = 3;
		else
			pres = 1 << i;

		req_parent.rate = req->rate * pres;
		if (__clk_determine_rate(parent, &req_parent))
			continue;

		clk_sama7g5_master_best_diff(req, parent, req_parent.rate,
					     &best_diff, &best_rate, pres);
		if (!best_diff)
			break;
	}

	return 0;
}

static int clk_master_pres_set_rate(struct clk_hw *hw, unsigned long rate,
				    unsigned long parent_rate)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned long flags;
	unsigned int pres;

	pres = DIV_ROUND_CLOSEST(parent_rate, rate);
	if (pres > MASTER_PRES_MAX)
		return -EINVAL;

	else if (pres == 3)
		pres = MASTER_PRES_MAX;
	else if (pres)
		pres = ffs(pres) - 1;

	spin_lock_irqsave(master->lock, flags);
	regmap_update_bits(master->regmap, master->layout->offset,
			   (MASTER_PRES_MASK << master->layout->pres_shift),
			   (pres << master->layout->pres_shift));

	while (!clk_master_ready(master))
		cpu_relax();
	spin_unlock_irqrestore(master->lock, flags);

	return 0;
}

static unsigned long clk_master_pres_recalc_rate(struct clk_hw *hw,
						 unsigned long parent_rate)
{
	struct clk_master *master = to_clk_master(hw);
	const struct clk_master_characteristics *characteristics =
						master->characteristics;
	unsigned long flags;
	unsigned int val, pres;

	spin_lock_irqsave(master->lock, flags);
	regmap_read(master->regmap, master->layout->offset, &val);
	spin_unlock_irqrestore(master->lock, flags);

	pres = (val >> master->layout->pres_shift) & MASTER_PRES_MASK;
	if (pres == MASTER_PRES_MAX && characteristics->have_div3_pres)
		pres = 3;
	else
		pres = (1 << pres);

	return DIV_ROUND_CLOSEST_ULL(parent_rate, pres);
}

static u8 clk_master_pres_get_parent(struct clk_hw *hw)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned long flags;
	unsigned int mckr;

	spin_lock_irqsave(master->lock, flags);
	regmap_read(master->regmap, master->layout->offset, &mckr);
	spin_unlock_irqrestore(master->lock, flags);
>>>>>>> upstream/android-13

	return mckr & AT91_PMC_CSS;
}

<<<<<<< HEAD
static const struct clk_ops master_ops = {
	.prepare = clk_master_prepare,
	.is_prepared = clk_master_is_prepared,
	.recalc_rate = clk_master_recalc_rate,
	.get_parent = clk_master_get_parent,
};

static struct clk_hw * __init
at91_clk_register_master(struct regmap *regmap,
		const char *name, int num_parents,
		const char **parent_names,
		const struct clk_master_layout *layout,
		const struct clk_master_characteristics *characteristics)
{
	struct clk_master *master;
	struct clk_init_data init = {};
	struct clk_hw *hw;
	int ret;

	if (!name || !num_parents || !parent_names)
=======
static const struct clk_ops master_pres_ops = {
	.prepare = clk_master_prepare,
	.is_prepared = clk_master_is_prepared,
	.recalc_rate = clk_master_pres_recalc_rate,
	.get_parent = clk_master_pres_get_parent,
};

static const struct clk_ops master_pres_ops_chg = {
	.prepare = clk_master_prepare,
	.is_prepared = clk_master_is_prepared,
	.determine_rate = clk_master_pres_determine_rate,
	.recalc_rate = clk_master_pres_recalc_rate,
	.get_parent = clk_master_pres_get_parent,
	.set_rate = clk_master_pres_set_rate,
};

static struct clk_hw * __init
at91_clk_register_master_internal(struct regmap *regmap,
		const char *name, int num_parents,
		const char **parent_names,
		const struct clk_master_layout *layout,
		const struct clk_master_characteristics *characteristics,
		const struct clk_ops *ops, spinlock_t *lock, u32 flags,
		int chg_pid)
{
	struct clk_master *master;
	struct clk_init_data init;
	struct clk_hw *hw;
	int ret;

	if (!name || !num_parents || !parent_names || !lock)
>>>>>>> upstream/android-13
		return ERR_PTR(-EINVAL);

	master = kzalloc(sizeof(*master), GFP_KERNEL);
	if (!master)
		return ERR_PTR(-ENOMEM);

	init.name = name;
<<<<<<< HEAD
	init.ops = &master_ops;
	init.parent_names = parent_names;
	init.num_parents = num_parents;
	init.flags = 0;
=======
	init.ops = ops;
	init.parent_names = parent_names;
	init.num_parents = num_parents;
	init.flags = flags;
>>>>>>> upstream/android-13

	master->hw.init = &init;
	master->layout = layout;
	master->characteristics = characteristics;
	master->regmap = regmap;
<<<<<<< HEAD
=======
	master->chg_pid = chg_pid;
	master->lock = lock;
>>>>>>> upstream/android-13

	hw = &master->hw;
	ret = clk_hw_register(NULL, &master->hw);
	if (ret) {
		kfree(master);
		hw = ERR_PTR(ret);
	}

	return hw;
}

<<<<<<< HEAD

static const struct clk_master_layout at91rm9200_master_layout = {
	.mask = 0x31F,
	.pres_shift = 2,
};

static const struct clk_master_layout at91sam9x5_master_layout = {
	.mask = 0x373,
	.pres_shift = 4,
};


static struct clk_master_characteristics * __init
of_at91_clk_master_get_characteristics(struct device_node *np)
{
	struct clk_master_characteristics *characteristics;

	characteristics = kzalloc(sizeof(*characteristics), GFP_KERNEL);
	if (!characteristics)
		return NULL;

	if (of_at91_get_clk_range(np, "atmel,clk-output-range", &characteristics->output))
		goto out_free_characteristics;

	of_property_read_u32_array(np, "atmel,clk-divisors",
				   characteristics->divisors, 4);

	characteristics->have_div3_pres =
		of_property_read_bool(np, "atmel,master-clk-have-div3-pres");

	return characteristics;

out_free_characteristics:
	kfree(characteristics);
	return NULL;
}

static void __init
of_at91_clk_master_setup(struct device_node *np,
			 const struct clk_master_layout *layout)
{
	struct clk_hw *hw;
	unsigned int num_parents;
	const char *parent_names[MASTER_SOURCE_MAX];
	const char *name = np->name;
	struct clk_master_characteristics *characteristics;
	struct regmap *regmap;

	num_parents = of_clk_get_parent_count(np);
	if (num_parents == 0 || num_parents > MASTER_SOURCE_MAX)
		return;

	of_clk_parent_fill(np, parent_names, num_parents);

	of_property_read_string(np, "clock-output-names", &name);

	characteristics = of_at91_clk_master_get_characteristics(np);
	if (!characteristics)
		return;

	regmap = syscon_node_to_regmap(of_get_parent(np));
	if (IS_ERR(regmap))
		return;

	hw = at91_clk_register_master(regmap, name, num_parents,
				       parent_names, layout,
				       characteristics);
	if (IS_ERR(hw))
		goto out_free_characteristics;

	of_clk_add_hw_provider(np, of_clk_hw_simple_get, hw);
	return;

out_free_characteristics:
	kfree(characteristics);
}

static void __init of_at91rm9200_clk_master_setup(struct device_node *np)
{
	of_at91_clk_master_setup(np, &at91rm9200_master_layout);
}
CLK_OF_DECLARE(at91rm9200_clk_master, "atmel,at91rm9200-clk-master",
	       of_at91rm9200_clk_master_setup);

static void __init of_at91sam9x5_clk_master_setup(struct device_node *np)
{
	of_at91_clk_master_setup(np, &at91sam9x5_master_layout);
}
CLK_OF_DECLARE(at91sam9x5_clk_master, "atmel,at91sam9x5-clk-master",
	       of_at91sam9x5_clk_master_setup);
=======
struct clk_hw * __init
at91_clk_register_master_pres(struct regmap *regmap,
		const char *name, int num_parents,
		const char **parent_names,
		const struct clk_master_layout *layout,
		const struct clk_master_characteristics *characteristics,
		spinlock_t *lock, u32 flags, int chg_pid)
{
	const struct clk_ops *ops;

	if (flags & CLK_SET_RATE_GATE)
		ops = &master_pres_ops;
	else
		ops = &master_pres_ops_chg;

	return at91_clk_register_master_internal(regmap, name, num_parents,
						 parent_names, layout,
						 characteristics, ops,
						 lock, flags, chg_pid);
}

struct clk_hw * __init
at91_clk_register_master_div(struct regmap *regmap,
		const char *name, const char *parent_name,
		const struct clk_master_layout *layout,
		const struct clk_master_characteristics *characteristics,
		spinlock_t *lock, u32 flags)
{
	const struct clk_ops *ops;

	if (flags & CLK_SET_RATE_GATE)
		ops = &master_div_ops;
	else
		ops = &master_div_ops_chg;

	return at91_clk_register_master_internal(regmap, name, 1,
						 &parent_name, layout,
						 characteristics, ops,
						 lock, flags, -EINVAL);
}

static unsigned long
clk_sama7g5_master_recalc_rate(struct clk_hw *hw,
			       unsigned long parent_rate)
{
	struct clk_master *master = to_clk_master(hw);

	return DIV_ROUND_CLOSEST_ULL(parent_rate, (1 << master->div));
}

static int clk_sama7g5_master_determine_rate(struct clk_hw *hw,
					     struct clk_rate_request *req)
{
	struct clk_master *master = to_clk_master(hw);
	struct clk_rate_request req_parent = *req;
	struct clk_hw *parent;
	long best_rate = LONG_MIN, best_diff = LONG_MIN;
	unsigned long parent_rate;
	unsigned int div, i;

	/* First: check the dividers of MCR. */
	for (i = 0; i < clk_hw_get_num_parents(hw); i++) {
		parent = clk_hw_get_parent_by_index(hw, i);
		if (!parent)
			continue;

		parent_rate = clk_hw_get_rate(parent);
		if (!parent_rate)
			continue;

		for (div = 0; div < MASTER_PRES_MAX + 1; div++) {
			clk_sama7g5_master_best_diff(req, parent, parent_rate,
						     &best_rate, &best_diff,
						     div);
			if (!best_diff)
				break;
		}

		if (!best_diff)
			break;
	}

	/* Second: try to request rate form changeable parent. */
	if (master->chg_pid < 0)
		goto end;

	parent = clk_hw_get_parent_by_index(hw, master->chg_pid);
	if (!parent)
		goto end;

	for (div = 0; div < MASTER_PRES_MAX + 1; div++) {
		if (div == MASTER_PRES_MAX)
			req_parent.rate = req->rate * 3;
		else
			req_parent.rate = req->rate << div;

		if (__clk_determine_rate(parent, &req_parent))
			continue;

		clk_sama7g5_master_best_diff(req, parent, req_parent.rate,
					     &best_rate, &best_diff, div);

		if (!best_diff)
			break;
	}

end:
	pr_debug("MCK: %s, best_rate = %ld, parent clk: %s @ %ld\n",
		 __func__, best_rate,
		 __clk_get_name((req->best_parent_hw)->clk),
		req->best_parent_rate);

	if (best_rate < 0)
		return -EINVAL;

	req->rate = best_rate;

	return 0;
}

static u8 clk_sama7g5_master_get_parent(struct clk_hw *hw)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned long flags;
	u8 index;

	spin_lock_irqsave(master->lock, flags);
	index = clk_mux_val_to_index(&master->hw, master->mux_table, 0,
				     master->parent);
	spin_unlock_irqrestore(master->lock, flags);

	return index;
}

static int clk_sama7g5_master_set_parent(struct clk_hw *hw, u8 index)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned long flags;

	if (index >= clk_hw_get_num_parents(hw))
		return -EINVAL;

	spin_lock_irqsave(master->lock, flags);
	master->parent = clk_mux_index_to_val(master->mux_table, 0, index);
	spin_unlock_irqrestore(master->lock, flags);

	return 0;
}

static int clk_sama7g5_master_enable(struct clk_hw *hw)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned long flags;
	unsigned int val, cparent;

	spin_lock_irqsave(master->lock, flags);

	regmap_write(master->regmap, PMC_MCR, PMC_MCR_ID(master->id));
	regmap_read(master->regmap, PMC_MCR, &val);
	regmap_update_bits(master->regmap, PMC_MCR,
			   PMC_MCR_EN | PMC_MCR_CSS | PMC_MCR_DIV |
			   PMC_MCR_CMD | PMC_MCR_ID_MSK,
			   PMC_MCR_EN | (master->parent << PMC_MCR_CSS_SHIFT) |
			   (master->div << MASTER_DIV_SHIFT) |
			   PMC_MCR_CMD | PMC_MCR_ID(master->id));

	cparent = (val & PMC_MCR_CSS) >> PMC_MCR_CSS_SHIFT;

	/* Wait here only if parent is being changed. */
	while ((cparent != master->parent) && !clk_master_ready(master))
		cpu_relax();

	spin_unlock_irqrestore(master->lock, flags);

	return 0;
}

static void clk_sama7g5_master_disable(struct clk_hw *hw)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned long flags;

	spin_lock_irqsave(master->lock, flags);

	regmap_write(master->regmap, PMC_MCR, master->id);
	regmap_update_bits(master->regmap, PMC_MCR,
			   PMC_MCR_EN | PMC_MCR_CMD | PMC_MCR_ID_MSK,
			   PMC_MCR_CMD | PMC_MCR_ID(master->id));

	spin_unlock_irqrestore(master->lock, flags);
}

static int clk_sama7g5_master_is_enabled(struct clk_hw *hw)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned long flags;
	unsigned int val;

	spin_lock_irqsave(master->lock, flags);

	regmap_write(master->regmap, PMC_MCR, master->id);
	regmap_read(master->regmap, PMC_MCR, &val);

	spin_unlock_irqrestore(master->lock, flags);

	return !!(val & PMC_MCR_EN);
}

static int clk_sama7g5_master_set_rate(struct clk_hw *hw, unsigned long rate,
				       unsigned long parent_rate)
{
	struct clk_master *master = to_clk_master(hw);
	unsigned long div, flags;

	div = DIV_ROUND_CLOSEST(parent_rate, rate);
	if ((div > (1 << (MASTER_PRES_MAX - 1))) || (div & (div - 1)))
		return -EINVAL;

	if (div == 3)
		div = MASTER_PRES_MAX;
	else if (div)
		div = ffs(div) - 1;

	spin_lock_irqsave(master->lock, flags);
	master->div = div;
	spin_unlock_irqrestore(master->lock, flags);

	return 0;
}

static const struct clk_ops sama7g5_master_ops = {
	.enable = clk_sama7g5_master_enable,
	.disable = clk_sama7g5_master_disable,
	.is_enabled = clk_sama7g5_master_is_enabled,
	.recalc_rate = clk_sama7g5_master_recalc_rate,
	.determine_rate = clk_sama7g5_master_determine_rate,
	.set_rate = clk_sama7g5_master_set_rate,
	.get_parent = clk_sama7g5_master_get_parent,
	.set_parent = clk_sama7g5_master_set_parent,
};

struct clk_hw * __init
at91_clk_sama7g5_register_master(struct regmap *regmap,
				 const char *name, int num_parents,
				 const char **parent_names,
				 u32 *mux_table,
				 spinlock_t *lock, u8 id,
				 bool critical, int chg_pid)
{
	struct clk_master *master;
	struct clk_hw *hw;
	struct clk_init_data init;
	unsigned long flags;
	unsigned int val;
	int ret;

	if (!name || !num_parents || !parent_names || !mux_table ||
	    !lock || id > MASTER_MAX_ID)
		return ERR_PTR(-EINVAL);

	master = kzalloc(sizeof(*master), GFP_KERNEL);
	if (!master)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &sama7g5_master_ops;
	init.parent_names = parent_names;
	init.num_parents = num_parents;
	init.flags = CLK_SET_RATE_GATE | CLK_SET_PARENT_GATE;
	if (chg_pid >= 0)
		init.flags |= CLK_SET_RATE_PARENT;
	if (critical)
		init.flags |= CLK_IS_CRITICAL;

	master->hw.init = &init;
	master->regmap = regmap;
	master->id = id;
	master->chg_pid = chg_pid;
	master->lock = lock;
	master->mux_table = mux_table;

	spin_lock_irqsave(master->lock, flags);
	regmap_write(master->regmap, PMC_MCR, master->id);
	regmap_read(master->regmap, PMC_MCR, &val);
	master->parent = (val & PMC_MCR_CSS) >> PMC_MCR_CSS_SHIFT;
	master->div = (val & PMC_MCR_DIV) >> MASTER_DIV_SHIFT;
	spin_unlock_irqrestore(master->lock, flags);

	hw = &master->hw;
	ret = clk_hw_register(NULL, &master->hw);
	if (ret) {
		kfree(master);
		hw = ERR_PTR(ret);
	}

	return hw;
}

const struct clk_master_layout at91rm9200_master_layout = {
	.mask = 0x31F,
	.pres_shift = 2,
	.offset = AT91_PMC_MCKR,
};

const struct clk_master_layout at91sam9x5_master_layout = {
	.mask = 0x373,
	.pres_shift = 4,
	.offset = AT91_PMC_MCKR,
};
>>>>>>> upstream/android-13
