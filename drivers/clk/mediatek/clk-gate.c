<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
*/

#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/slab.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2014 MediaTek Inc.
 * Author: James Liao <jamesjj.liao@mediatek.com>
 */

#include <linux/of.h>
#include <linux/of_address.h>

#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/clkdev.h>
#include <linux/module.h>
>>>>>>> upstream/android-13

#include "clk-mtk.h"
#include "clk-gate.h"

<<<<<<< HEAD
#define INV_OFS		-1

static int is_subsys_pwr_on(struct mtk_clk_gate *cg)
{
	struct pwr_status *pwr = cg->pwr_stat;
	u32 val = 0, val2 = 0;

	if (pwr != NULL && cg->pwr_regmap != NULL) {
		if (pwr->pwr_ofs != INV_OFS && pwr->pwr2_ofs != INV_OFS) {
			regmap_read(cg->pwr_regmap, pwr->pwr_ofs, &val);
			regmap_read(cg->pwr_regmap, pwr->pwr2_ofs, &val2);

			if ((val & pwr->mask) != pwr->val &&
					(val2 & pwr->mask) != pwr->val)
				return false;
		} else if (pwr->other_ofs != INV_OFS) {
			regmap_read(cg->pwr_regmap, pwr->other_ofs, &val);
			if ((val & pwr->mask) != pwr->val)
				return false;
		}
	}

	return true;
}
static void mtk_cg_set_bit_unused(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
	const char *c_n = clk_hw_get_name(hw);

	pr_notice("disable_unused - %s\n", c_n);

	regmap_write(cg->regmap, cg->set_ofs, BIT(cg->bit));
}

static void mtk_cg_clr_bit_unused(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
	const char *c_n = clk_hw_get_name(hw);

	pr_notice("disable_unused - %s\n", c_n);

	regmap_write(cg->regmap, cg->clr_ofs, BIT(cg->bit));
}

static void mtk_cg_set_bit_no_setclr_unused(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
	u32 cgbit = BIT(cg->bit);
	const char *c_n = clk_hw_get_name(hw);

	pr_notice("disable_unused - %s\n", c_n);

	regmap_update_bits(cg->regmap, cg->sta_ofs, cgbit, cgbit);
}

static void mtk_cg_clr_bit_no_setclr_unused(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
	u32 cgbit = BIT(cg->bit);
	const char *c_n = clk_hw_get_name(hw);

	pr_notice("disable_unused - %s\n", c_n);

	regmap_update_bits(cg->regmap, cg->sta_ofs, cgbit, 0);
}

static void mtk_cg_disable_inv_unused(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);

	if (!is_subsys_pwr_on(cg))
		return;

	if (!clk_hw_is_enabled(clk_hw_get_parent(hw)))
		return;

	mtk_cg_clr_bit_unused(hw);
}

static void mtk_cg_disable_unused(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);

	if (!is_subsys_pwr_on(cg))
		return;

	if (!clk_hw_is_enabled(clk_hw_get_parent(hw)))
		return;

	mtk_cg_set_bit_unused(hw);
}

static void mtk_cg_disable_inv_no_setclr_unused(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);

	if (!is_subsys_pwr_on(cg))
		return;

	if (!clk_hw_is_enabled(clk_hw_get_parent(hw)))
		return;

	mtk_cg_clr_bit_no_setclr_unused(hw);
}

static void mtk_cg_disable_no_setclr_unused(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);

	if (!is_subsys_pwr_on(cg))
		return;

	if (!clk_hw_is_enabled(clk_hw_get_parent(hw)))
		return;

	mtk_cg_set_bit_no_setclr_unused(hw);
}

=======
>>>>>>> upstream/android-13
static int mtk_cg_bit_is_cleared(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
	u32 val;

	regmap_read(cg->regmap, cg->sta_ofs, &val);

	val &= BIT(cg->bit);

	return val == 0;
}

static int mtk_cg_bit_is_set(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
	u32 val;

	regmap_read(cg->regmap, cg->sta_ofs, &val);

	val &= BIT(cg->bit);

	return val != 0;
}

<<<<<<< HEAD
static int mtk_cg_is_enabled(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);

	if (!is_subsys_pwr_on(cg))
		return 0;

	return mtk_cg_bit_is_cleared(hw);
}

static int mtk_en_is_enabled(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);

	if (!is_subsys_pwr_on(cg))
		return 0;

	return mtk_cg_bit_is_set(hw);
}

=======
>>>>>>> upstream/android-13
static void mtk_cg_set_bit(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);

	regmap_write(cg->regmap, cg->set_ofs, BIT(cg->bit));
}

static void mtk_cg_clr_bit(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
<<<<<<< HEAD
#ifdef CONFIG_MACH_MT6853
	int val = 0;
	int i = 0;
#endif

	regmap_write(cg->regmap, cg->clr_ofs, BIT(cg->bit));
#ifdef CONFIG_MACH_MT6853
	regmap_read(cg->regmap, cg->sta_ofs, &val);
	while ((val & BIT(cg->bit)) != 0) {
		regmap_write(cg->regmap, cg->clr_ofs, BIT(cg->bit));
		regmap_read(cg->regmap, cg->sta_ofs, &val);

		if (i > 5)
			break;
		i++;
	}
#endif
}


=======

	regmap_write(cg->regmap, cg->clr_ofs, BIT(cg->bit));
}

>>>>>>> upstream/android-13
static void mtk_cg_set_bit_no_setclr(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
	u32 cgbit = BIT(cg->bit);

	regmap_update_bits(cg->regmap, cg->sta_ofs, cgbit, cgbit);
}

static void mtk_cg_clr_bit_no_setclr(struct clk_hw *hw)
{
	struct mtk_clk_gate *cg = to_mtk_clk_gate(hw);
	u32 cgbit = BIT(cg->bit);

	regmap_update_bits(cg->regmap, cg->sta_ofs, cgbit, 0);
}

static int mtk_cg_enable(struct clk_hw *hw)
{
	mtk_cg_clr_bit(hw);

	return 0;
}

static void mtk_cg_disable(struct clk_hw *hw)
{
	mtk_cg_set_bit(hw);
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
static int mtk_cg_enable_inv(struct clk_hw *hw)
{
	mtk_cg_set_bit(hw);

	return 0;
}

static void mtk_cg_disable_inv(struct clk_hw *hw)
{
	mtk_cg_clr_bit(hw);
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
static int mtk_cg_enable_no_setclr(struct clk_hw *hw)
{
	mtk_cg_clr_bit_no_setclr(hw);

	return 0;
}

static void mtk_cg_disable_no_setclr(struct clk_hw *hw)
{
	mtk_cg_set_bit_no_setclr(hw);
}

static int mtk_cg_enable_inv_no_setclr(struct clk_hw *hw)
{
	mtk_cg_set_bit_no_setclr(hw);

	return 0;
}

static void mtk_cg_disable_inv_no_setclr(struct clk_hw *hw)
{
	mtk_cg_clr_bit_no_setclr(hw);
}

const struct clk_ops mtk_clk_gate_ops_setclr = {
<<<<<<< HEAD
	.is_enabled	= mtk_cg_is_enabled,
	.enable		= mtk_cg_enable,
	.disable	= mtk_cg_disable,
#if !defined(CONFIG_MACH_MT6761) && \
		!defined(CONFIG_MACH_MT6765)
	.disable_unused = mtk_cg_disable_unused,
#endif
};
EXPORT_SYMBOL(mtk_clk_gate_ops_setclr);

static void mtk_cg_disable_dummy(struct clk_hw *hw)
{
	/* do nothing */
}

const struct clk_ops mtk_clk_gate_ops_setclr_dummy = {
	.is_enabled = mtk_cg_is_enabled,
	.enable = mtk_cg_enable,
	.disable = mtk_cg_disable_dummy,
};

EXPORT_SYMBOL(mtk_clk_gate_ops_setclr_dummy);

const struct clk_ops mtk_clk_gate_ops_setclr_inv = {
	.is_enabled	= mtk_en_is_enabled,
	.enable		= mtk_cg_enable_inv,
	.disable	= mtk_cg_disable_inv,
#if !defined(CONFIG_MACH_MT6761) && \
		!defined(CONFIG_MACH_MT6765)
	.disable_unused = mtk_cg_disable_inv_unused,
#endif
};
EXPORT_SYMBOL(mtk_clk_gate_ops_setclr_inv);

const struct clk_ops mtk_clk_gate_ops_setclr_inv_dummy = {
	.is_enabled	= mtk_en_is_enabled,
	.enable		= mtk_cg_enable_inv,
	.disable = mtk_cg_disable_dummy,
};
EXPORT_SYMBOL(mtk_clk_gate_ops_setclr_inv_dummy);

const struct clk_ops mtk_clk_gate_ops_no_setclr = {
	.is_enabled	= mtk_cg_is_enabled,
	.enable		= mtk_cg_enable_no_setclr,
	.disable	= mtk_cg_disable_no_setclr,
	.disable_unused	= mtk_cg_disable_no_setclr_unused,
};
EXPORT_SYMBOL(mtk_clk_gate_ops_no_setclr);

const struct clk_ops mtk_clk_gate_ops_no_setclr_inv = {
	.is_enabled	= mtk_en_is_enabled,
	.enable		= mtk_cg_enable_inv_no_setclr,
	.disable	= mtk_cg_disable_inv_no_setclr,
	.disable_unused = mtk_cg_disable_inv_no_setclr_unused,
};
EXPORT_SYMBOL(mtk_clk_gate_ops_no_setclr_inv);
=======
	.is_enabled	= mtk_cg_bit_is_cleared,
	.enable		= mtk_cg_enable,
	.disable	= mtk_cg_disable,
};
EXPORT_SYMBOL_GPL(mtk_clk_gate_ops_setclr);

const struct clk_ops mtk_clk_gate_ops_setclr_inv = {
	.is_enabled	= mtk_cg_bit_is_set,
	.enable		= mtk_cg_enable_inv,
	.disable	= mtk_cg_disable_inv,
};
EXPORT_SYMBOL_GPL(mtk_clk_gate_ops_setclr_inv);

const struct clk_ops mtk_clk_gate_ops_no_setclr = {
	.is_enabled	= mtk_cg_bit_is_cleared,
	.enable		= mtk_cg_enable_no_setclr,
	.disable	= mtk_cg_disable_no_setclr,
};
EXPORT_SYMBOL_GPL(mtk_clk_gate_ops_no_setclr);

const struct clk_ops mtk_clk_gate_ops_no_setclr_inv = {
	.is_enabled	= mtk_cg_bit_is_set,
	.enable		= mtk_cg_enable_inv_no_setclr,
	.disable	= mtk_cg_disable_inv_no_setclr,
};
EXPORT_SYMBOL_GPL(mtk_clk_gate_ops_no_setclr_inv);
>>>>>>> upstream/android-13

struct clk *mtk_clk_register_gate(
		const char *name,
		const char *parent_name,
		struct regmap *regmap,
		int set_ofs,
		int clr_ofs,
		int sta_ofs,
		u8 bit,
		const struct clk_ops *ops,
		unsigned long flags,
<<<<<<< HEAD
		struct pwr_status *pwr_stat,
		struct regmap *pwr_regmap)
=======
		struct device *dev)
>>>>>>> upstream/android-13
{
	struct mtk_clk_gate *cg;
	struct clk *clk;
	struct clk_init_data init = {};

	cg = kzalloc(sizeof(*cg), GFP_KERNEL);
	if (!cg)
		return ERR_PTR(-ENOMEM);

	init.name = name;
<<<<<<< HEAD
	init.flags = flags | CLK_SET_RATE_PARENT | CLK_OPS_PARENT_ENABLE;
=======
	init.flags = flags | CLK_SET_RATE_PARENT;
>>>>>>> upstream/android-13
	init.parent_names = parent_name ? &parent_name : NULL;
	init.num_parents = parent_name ? 1 : 0;
	init.ops = ops;

	cg->regmap = regmap;
	cg->set_ofs = set_ofs;
	cg->clr_ofs = clr_ofs;
	cg->sta_ofs = sta_ofs;
	cg->bit = bit;
<<<<<<< HEAD
	cg->pwr_stat = pwr_stat;
	cg->pwr_regmap = pwr_regmap;

	cg->hw.init = &init;

	clk = clk_register(NULL, &cg->hw);
=======

	cg->hw.init = &init;

	clk = clk_register(dev, &cg->hw);
>>>>>>> upstream/android-13
	if (IS_ERR(clk))
		kfree(cg);

	return clk;
}
<<<<<<< HEAD
EXPORT_SYMBOL(mtk_clk_register_gate);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MediaTek GATE");
MODULE_AUTHOR("MediaTek Inc.");

=======
EXPORT_SYMBOL_GPL(mtk_clk_register_gate);

MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
