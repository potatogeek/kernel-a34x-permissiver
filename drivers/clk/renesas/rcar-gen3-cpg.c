<<<<<<< HEAD
/*
 * R-Car Gen3 Clock Pulse Generator
 *
 * Copyright (C) 2015-2016 Glider bvba
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * R-Car Gen3 Clock Pulse Generator
 *
 * Copyright (C) 2015-2018 Glider bvba
 * Copyright (C) 2019 Renesas Electronics Corp.
>>>>>>> upstream/android-13
 *
 * Based on clk-rcar-gen3.c
 *
 * Copyright (C) 2015 Renesas Electronics Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
=======
>>>>>>> upstream/android-13
 */

#include <linux/bug.h>
#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/sys_soc.h>

#include "renesas-cpg-mssr.h"
<<<<<<< HEAD
#include "rcar-gen3-cpg.h"

#define CPG_PLL0CR		0x00d8
#define CPG_PLL2CR		0x002c
#define CPG_PLL4CR		0x01f4

struct cpg_simple_notifier {
	struct notifier_block nb;
	void __iomem *reg;
	u32 saved;
};

static int cpg_simple_notifier_call(struct notifier_block *nb,
				    unsigned long action, void *data)
{
	struct cpg_simple_notifier *csn =
		container_of(nb, struct cpg_simple_notifier, nb);

	switch (action) {
	case PM_EVENT_SUSPEND:
		csn->saved = readl(csn->reg);
		return NOTIFY_OK;

	case PM_EVENT_RESUME:
		writel(csn->saved, csn->reg);
		return NOTIFY_OK;
	}
	return NOTIFY_DONE;
}

static void cpg_simple_notifier_register(struct raw_notifier_head *notifiers,
					 struct cpg_simple_notifier *csn)
{
	csn->nb.notifier_call = cpg_simple_notifier_call;
	raw_notifier_chain_register(notifiers, &csn->nb);
=======
#include "rcar-cpg-lib.h"
#include "rcar-gen3-cpg.h"

#define CPG_PLLECR		0x00d0	/* PLL Enable Control Register */

#define CPG_PLLECR_PLLST(n)	BIT(8 + (n))	/* PLLn Circuit Status */

#define CPG_PLL0CR		0x00d8	/* PLLn Control Registers */
#define CPG_PLL2CR		0x002c
#define CPG_PLL4CR		0x01f4

#define CPG_PLLnCR_STC_MASK	GENMASK(30, 24)	/* PLL Circuit Mult. Ratio */

#define CPG_RCKCR_CKSEL	BIT(15)	/* RCLK Clock Source Select */

/* PLL Clocks */
struct cpg_pll_clk {
	struct clk_hw hw;
	void __iomem *pllcr_reg;
	void __iomem *pllecr_reg;
	unsigned int fixed_mult;
	u32 pllecr_pllst_mask;
};

#define to_pll_clk(_hw)   container_of(_hw, struct cpg_pll_clk, hw)

static unsigned long cpg_pll_clk_recalc_rate(struct clk_hw *hw,
					     unsigned long parent_rate)
{
	struct cpg_pll_clk *pll_clk = to_pll_clk(hw);
	unsigned int mult;
	u32 val;

	val = readl(pll_clk->pllcr_reg) & CPG_PLLnCR_STC_MASK;
	mult = (val >> __ffs(CPG_PLLnCR_STC_MASK)) + 1;

	return parent_rate * mult * pll_clk->fixed_mult;
}

static int cpg_pll_clk_determine_rate(struct clk_hw *hw,
				      struct clk_rate_request *req)
{
	struct cpg_pll_clk *pll_clk = to_pll_clk(hw);
	unsigned int min_mult, max_mult, mult;
	unsigned long prate;

	prate = req->best_parent_rate * pll_clk->fixed_mult;
	min_mult = max(div64_ul(req->min_rate, prate), 1ULL);
	max_mult = min(div64_ul(req->max_rate, prate), 128ULL);
	if (max_mult < min_mult)
		return -EINVAL;

	mult = DIV_ROUND_CLOSEST_ULL(req->rate, prate);
	mult = clamp(mult, min_mult, max_mult);

	req->rate = prate * mult;
	return 0;
}

static int cpg_pll_clk_set_rate(struct clk_hw *hw, unsigned long rate,
				unsigned long parent_rate)
{
	struct cpg_pll_clk *pll_clk = to_pll_clk(hw);
	unsigned int mult, i;
	u32 val;

	mult = DIV_ROUND_CLOSEST_ULL(rate, parent_rate * pll_clk->fixed_mult);
	mult = clamp(mult, 1U, 128U);

	val = readl(pll_clk->pllcr_reg);
	val &= ~CPG_PLLnCR_STC_MASK;
	val |= (mult - 1) << __ffs(CPG_PLLnCR_STC_MASK);
	writel(val, pll_clk->pllcr_reg);

	for (i = 1000; i; i--) {
		if (readl(pll_clk->pllecr_reg) & pll_clk->pllecr_pllst_mask)
			return 0;

		cpu_relax();
	}

	return -ETIMEDOUT;
}

static const struct clk_ops cpg_pll_clk_ops = {
	.recalc_rate = cpg_pll_clk_recalc_rate,
	.determine_rate = cpg_pll_clk_determine_rate,
	.set_rate = cpg_pll_clk_set_rate,
};

static struct clk * __init cpg_pll_clk_register(const char *name,
						const char *parent_name,
						void __iomem *base,
						unsigned int mult,
						unsigned int offset,
						unsigned int index)

{
	struct cpg_pll_clk *pll_clk;
	struct clk_init_data init = {};
	struct clk *clk;

	pll_clk = kzalloc(sizeof(*pll_clk), GFP_KERNEL);
	if (!pll_clk)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &cpg_pll_clk_ops;
	init.parent_names = &parent_name;
	init.num_parents = 1;

	pll_clk->hw.init = &init;
	pll_clk->pllcr_reg = base + offset;
	pll_clk->pllecr_reg = base + CPG_PLLECR;
	pll_clk->fixed_mult = mult;	/* PLL refclk x (setting + 1) x mult */
	pll_clk->pllecr_pllst_mask = CPG_PLLECR_PLLST(index);

	clk = clk_register(NULL, &pll_clk->hw);
	if (IS_ERR(clk))
		kfree(pll_clk);

	return clk;
>>>>>>> upstream/android-13
}

/*
 * Z Clock & Z2 Clock
 *
 * Traits of this clock:
 * prepare - clk_prepare only ensures that parents are prepared
 * enable - clk_enable only ensures that parents are enabled
<<<<<<< HEAD
 * rate - rate is adjustable.  clk->rate = (parent->rate * mult / 32 ) / 2
=======
 * rate - rate is adjustable.
 *        clk->rate = (parent->rate * mult / 32 ) / fixed_div
>>>>>>> upstream/android-13
 * parent - fixed parent.  No clk_set_parent support
 */
#define CPG_FRQCRB			0x00000004
#define CPG_FRQCRB_KICK			BIT(31)
#define CPG_FRQCRC			0x000000e0
<<<<<<< HEAD
#define CPG_FRQCRC_ZFC_MASK		GENMASK(12, 8)
#define CPG_FRQCRC_Z2FC_MASK		GENMASK(4, 0)
=======
>>>>>>> upstream/android-13

struct cpg_z_clk {
	struct clk_hw hw;
	void __iomem *reg;
	void __iomem *kick_reg;
<<<<<<< HEAD
	unsigned long mask;
=======
	unsigned long max_rate;		/* Maximum rate for normal mode */
	unsigned int fixed_div;
	u32 mask;
>>>>>>> upstream/android-13
};

#define to_z_clk(_hw)	container_of(_hw, struct cpg_z_clk, hw)

static unsigned long cpg_z_clk_recalc_rate(struct clk_hw *hw,
					   unsigned long parent_rate)
{
	struct cpg_z_clk *zclk = to_z_clk(hw);
	unsigned int mult;
	u32 val;

	val = readl(zclk->reg) & zclk->mask;
	mult = 32 - (val >> __ffs(zclk->mask));

<<<<<<< HEAD
	/* Factor of 2 is for fixed divider */
	return DIV_ROUND_CLOSEST_ULL((u64)parent_rate * mult, 32 * 2);
}

static long cpg_z_clk_round_rate(struct clk_hw *hw, unsigned long rate,
				 unsigned long *parent_rate)
{
	/* Factor of 2 is for fixed divider */
	unsigned long prate = *parent_rate / 2;
	unsigned int mult;

	mult = div_u64(rate * 32ULL, prate);
	mult = clamp(mult, 1U, 32U);

	return (u64)prate * mult / 32;
=======
	return DIV_ROUND_CLOSEST_ULL((u64)parent_rate * mult,
				     32 * zclk->fixed_div);
}

static int cpg_z_clk_determine_rate(struct clk_hw *hw,
				    struct clk_rate_request *req)
{
	struct cpg_z_clk *zclk = to_z_clk(hw);
	unsigned int min_mult, max_mult, mult;
	unsigned long rate, prate;

	rate = min(req->rate, req->max_rate);
	if (rate <= zclk->max_rate) {
		/* Set parent rate to initial value for normal modes */
		prate = zclk->max_rate;
	} else {
		/* Set increased parent rate for boost modes */
		prate = rate;
	}
	req->best_parent_rate = clk_hw_round_rate(clk_hw_get_parent(hw),
						  prate * zclk->fixed_div);

	prate = req->best_parent_rate / zclk->fixed_div;
	min_mult = max(div64_ul(req->min_rate * 32ULL, prate), 1ULL);
	max_mult = min(div64_ul(req->max_rate * 32ULL, prate), 32ULL);
	if (max_mult < min_mult)
		return -EINVAL;

	mult = DIV_ROUND_CLOSEST_ULL(rate * 32ULL, prate);
	mult = clamp(mult, min_mult, max_mult);

	req->rate = DIV_ROUND_CLOSEST_ULL((u64)prate * mult, 32);
	return 0;
>>>>>>> upstream/android-13
}

static int cpg_z_clk_set_rate(struct clk_hw *hw, unsigned long rate,
			      unsigned long parent_rate)
{
	struct cpg_z_clk *zclk = to_z_clk(hw);
	unsigned int mult;
	unsigned int i;
<<<<<<< HEAD
	u32 val, kick;

	/* Factor of 2 is for fixed divider */
	mult = DIV_ROUND_CLOSEST_ULL(rate * 32ULL * 2, parent_rate);
=======

	mult = DIV64_U64_ROUND_CLOSEST(rate * 32ULL * zclk->fixed_div,
				       parent_rate);
>>>>>>> upstream/android-13
	mult = clamp(mult, 1U, 32U);

	if (readl(zclk->kick_reg) & CPG_FRQCRB_KICK)
		return -EBUSY;

<<<<<<< HEAD
	val = readl(zclk->reg) & ~zclk->mask;
	val |= ((32 - mult) << __ffs(zclk->mask)) & zclk->mask;
	writel(val, zclk->reg);
=======
	cpg_reg_modify(zclk->reg, zclk->mask, (32 - mult) << __ffs(zclk->mask));
>>>>>>> upstream/android-13

	/*
	 * Set KICK bit in FRQCRB to update hardware setting and wait for
	 * clock change completion.
	 */
<<<<<<< HEAD
	kick = readl(zclk->kick_reg);
	kick |= CPG_FRQCRB_KICK;
	writel(kick, zclk->kick_reg);
=======
	cpg_reg_modify(zclk->kick_reg, 0, CPG_FRQCRB_KICK);
>>>>>>> upstream/android-13

	/*
	 * Note: There is no HW information about the worst case latency.
	 *
	 * Using experimental measurements, it seems that no more than
	 * ~10 iterations are needed, independently of the CPU rate.
<<<<<<< HEAD
	 * Since this value might be dependent of external xtal rate, pll1
=======
	 * Since this value might be dependent on external xtal rate, pll1
>>>>>>> upstream/android-13
	 * rate or even the other emulation clocks rate, use 1000 as a
	 * "super" safe value.
	 */
	for (i = 1000; i; i--) {
		if (!(readl(zclk->kick_reg) & CPG_FRQCRB_KICK))
			return 0;

		cpu_relax();
	}

	return -ETIMEDOUT;
}

static const struct clk_ops cpg_z_clk_ops = {
	.recalc_rate = cpg_z_clk_recalc_rate,
<<<<<<< HEAD
	.round_rate = cpg_z_clk_round_rate,
=======
	.determine_rate = cpg_z_clk_determine_rate,
>>>>>>> upstream/android-13
	.set_rate = cpg_z_clk_set_rate,
};

static struct clk * __init cpg_z_clk_register(const char *name,
					      const char *parent_name,
					      void __iomem *reg,
<<<<<<< HEAD
					      unsigned long mask)
=======
					      unsigned int div,
					      unsigned int offset)
>>>>>>> upstream/android-13
{
	struct clk_init_data init = {};
	struct cpg_z_clk *zclk;
	struct clk *clk;

	zclk = kzalloc(sizeof(*zclk), GFP_KERNEL);
	if (!zclk)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &cpg_z_clk_ops;
<<<<<<< HEAD
	init.flags = 0;
=======
	init.flags = CLK_SET_RATE_PARENT;
>>>>>>> upstream/android-13
	init.parent_names = &parent_name;
	init.num_parents = 1;

	zclk->reg = reg + CPG_FRQCRC;
	zclk->kick_reg = reg + CPG_FRQCRB;
	zclk->hw.init = &init;
<<<<<<< HEAD
	zclk->mask = mask;

	clk = clk_register(NULL, &zclk->hw);
	if (IS_ERR(clk))
		kfree(zclk);

	return clk;
}

/*
 * SDn Clock
 */
#define CPG_SD_STP_HCK		BIT(9)
#define CPG_SD_STP_CK		BIT(8)

#define CPG_SD_STP_MASK		(CPG_SD_STP_HCK | CPG_SD_STP_CK)
#define CPG_SD_FC_MASK		(0x7 << 2 | 0x3 << 0)

#define CPG_SD_DIV_TABLE_DATA(stp_hck, stp_ck, sd_srcfc, sd_fc, sd_div) \
{ \
	.val = ((stp_hck) ? CPG_SD_STP_HCK : 0) | \
	       ((stp_ck) ? CPG_SD_STP_CK : 0) | \
	       ((sd_srcfc) << 2) | \
	       ((sd_fc) << 0), \
	.div = (sd_div), \
}

struct sd_div_table {
	u32 val;
	unsigned int div;
};

struct sd_clock {
	struct clk_hw hw;
	const struct sd_div_table *div_table;
	struct cpg_simple_notifier csn;
	unsigned int div_num;
	unsigned int div_min;
	unsigned int div_max;
	unsigned int cur_div_idx;
};

/* SDn divider
 *                     sd_srcfc   sd_fc   div
 * stp_hck   stp_ck    (div)      (div)     = sd_srcfc x sd_fc
 *-------------------------------------------------------------------
 *  0         0         0 (1)      1 (4)      4
 *  0         0         1 (2)      1 (4)      8
 *  1         0         2 (4)      1 (4)     16
 *  1         0         3 (8)      1 (4)     32
 *  1         0         4 (16)     1 (4)     64
 *  0         0         0 (1)      0 (2)      2
 *  0         0         1 (2)      0 (2)      4
 *  1         0         2 (4)      0 (2)      8
 *  1         0         3 (8)      0 (2)     16
 *  1         0         4 (16)     0 (2)     32
 */
static const struct sd_div_table cpg_sd_div_table[] = {
/*	CPG_SD_DIV_TABLE_DATA(stp_hck,  stp_ck,   sd_srcfc,   sd_fc,  sd_div) */
	CPG_SD_DIV_TABLE_DATA(0,        0,        0,          1,        4),
	CPG_SD_DIV_TABLE_DATA(0,        0,        1,          1,        8),
	CPG_SD_DIV_TABLE_DATA(1,        0,        2,          1,       16),
	CPG_SD_DIV_TABLE_DATA(1,        0,        3,          1,       32),
	CPG_SD_DIV_TABLE_DATA(1,        0,        4,          1,       64),
	CPG_SD_DIV_TABLE_DATA(0,        0,        0,          0,        2),
	CPG_SD_DIV_TABLE_DATA(0,        0,        1,          0,        4),
	CPG_SD_DIV_TABLE_DATA(1,        0,        2,          0,        8),
	CPG_SD_DIV_TABLE_DATA(1,        0,        3,          0,       16),
	CPG_SD_DIV_TABLE_DATA(1,        0,        4,          0,       32),
};

#define to_sd_clock(_hw) container_of(_hw, struct sd_clock, hw)

static int cpg_sd_clock_enable(struct clk_hw *hw)
{
	struct sd_clock *clock = to_sd_clock(hw);
	u32 val = readl(clock->csn.reg);

	val &= ~(CPG_SD_STP_MASK);
	val |= clock->div_table[clock->cur_div_idx].val & CPG_SD_STP_MASK;

	writel(val, clock->csn.reg);

	return 0;
}

static void cpg_sd_clock_disable(struct clk_hw *hw)
{
	struct sd_clock *clock = to_sd_clock(hw);

	writel(readl(clock->csn.reg) | CPG_SD_STP_MASK, clock->csn.reg);
}

static int cpg_sd_clock_is_enabled(struct clk_hw *hw)
{
	struct sd_clock *clock = to_sd_clock(hw);

	return !(readl(clock->csn.reg) & CPG_SD_STP_MASK);
}

static unsigned long cpg_sd_clock_recalc_rate(struct clk_hw *hw,
						unsigned long parent_rate)
{
	struct sd_clock *clock = to_sd_clock(hw);

	return DIV_ROUND_CLOSEST(parent_rate,
				 clock->div_table[clock->cur_div_idx].div);
}

static unsigned int cpg_sd_clock_calc_div(struct sd_clock *clock,
					  unsigned long rate,
					  unsigned long parent_rate)
{
	unsigned int div;

	if (!rate)
		rate = 1;

	div = DIV_ROUND_CLOSEST(parent_rate, rate);

	return clamp_t(unsigned int, div, clock->div_min, clock->div_max);
}

static long cpg_sd_clock_round_rate(struct clk_hw *hw, unsigned long rate,
				      unsigned long *parent_rate)
{
	struct sd_clock *clock = to_sd_clock(hw);
	unsigned int div = cpg_sd_clock_calc_div(clock, rate, *parent_rate);

	return DIV_ROUND_CLOSEST(*parent_rate, div);
}

static int cpg_sd_clock_set_rate(struct clk_hw *hw, unsigned long rate,
				   unsigned long parent_rate)
{
	struct sd_clock *clock = to_sd_clock(hw);
	unsigned int div = cpg_sd_clock_calc_div(clock, rate, parent_rate);
	u32 val;
	unsigned int i;

	for (i = 0; i < clock->div_num; i++)
		if (div == clock->div_table[i].div)
			break;

	if (i >= clock->div_num)
		return -EINVAL;

	clock->cur_div_idx = i;

	val = readl(clock->csn.reg);
	val &= ~(CPG_SD_STP_MASK | CPG_SD_FC_MASK);
	val |= clock->div_table[i].val & (CPG_SD_STP_MASK | CPG_SD_FC_MASK);
	writel(val, clock->csn.reg);

	return 0;
}

static const struct clk_ops cpg_sd_clock_ops = {
	.enable = cpg_sd_clock_enable,
	.disable = cpg_sd_clock_disable,
	.is_enabled = cpg_sd_clock_is_enabled,
	.recalc_rate = cpg_sd_clock_recalc_rate,
	.round_rate = cpg_sd_clock_round_rate,
	.set_rate = cpg_sd_clock_set_rate,
};

static struct clk * __init cpg_sd_clk_register(const struct cpg_core_clk *core,
	void __iomem *base, const char *parent_name,
	struct raw_notifier_head *notifiers)
{
	struct clk_init_data init = {};
	struct sd_clock *clock;
	struct clk *clk;
	unsigned int i;
	u32 val;

	clock = kzalloc(sizeof(*clock), GFP_KERNEL);
	if (!clock)
		return ERR_PTR(-ENOMEM);

	init.name = core->name;
	init.ops = &cpg_sd_clock_ops;
	init.flags = CLK_IS_BASIC | CLK_SET_RATE_PARENT;
	init.parent_names = &parent_name;
	init.num_parents = 1;

	clock->csn.reg = base + core->offset;
	clock->hw.init = &init;
	clock->div_table = cpg_sd_div_table;
	clock->div_num = ARRAY_SIZE(cpg_sd_div_table);

	val = readl(clock->csn.reg) & ~CPG_SD_FC_MASK;
	val |= CPG_SD_STP_MASK | (clock->div_table[0].val & CPG_SD_FC_MASK);
	writel(val, clock->csn.reg);

	clock->div_max = clock->div_table[0].div;
	clock->div_min = clock->div_max;
	for (i = 1; i < clock->div_num; i++) {
		clock->div_max = max(clock->div_max, clock->div_table[i].div);
		clock->div_min = min(clock->div_min, clock->div_table[i].div);
	}

	clk = clk_register(NULL, &clock->hw);
	if (IS_ERR(clk))
		goto free_clock;

	cpg_simple_notifier_register(notifiers, &clock->csn);
	return clk;

free_clock:
	kfree(clock);
=======
	zclk->mask = GENMASK(offset + 4, offset);
	zclk->fixed_div = div; /* PLLVCO x 1/div x SYS-CPU divider */

	clk = clk_register(NULL, &zclk->hw);
	if (IS_ERR(clk)) {
		kfree(zclk);
		return clk;
	}

	zclk->max_rate = clk_hw_get_rate(clk_hw_get_parent(&zclk->hw)) /
			 zclk->fixed_div;
	return clk;
}

struct rpc_clock {
	struct clk_divider div;
	struct clk_gate gate;
	/*
	 * One notifier covers both RPC and RPCD2 clocks as they are both
	 * controlled by the same RPCCKCR register...
	 */
	struct cpg_simple_notifier csn;
};

static const struct clk_div_table cpg_rpcsrc_div_table[] = {
	{ 2, 5 }, { 3, 6 }, { 0, 0 },
};

static const struct clk_div_table cpg_rpc_div_table[] = {
	{ 1, 2 }, { 3, 4 }, { 5, 6 }, { 7, 8 }, { 0, 0 },
};

static struct clk * __init cpg_rpc_clk_register(const char *name,
	void __iomem *base, const char *parent_name,
	struct raw_notifier_head *notifiers)
{
	struct rpc_clock *rpc;
	struct clk *clk;

	rpc = kzalloc(sizeof(*rpc), GFP_KERNEL);
	if (!rpc)
		return ERR_PTR(-ENOMEM);

	rpc->div.reg = base + CPG_RPCCKCR;
	rpc->div.width = 3;
	rpc->div.table = cpg_rpc_div_table;
	rpc->div.lock = &cpg_lock;

	rpc->gate.reg = base + CPG_RPCCKCR;
	rpc->gate.bit_idx = 8;
	rpc->gate.flags = CLK_GATE_SET_TO_DISABLE;
	rpc->gate.lock = &cpg_lock;

	rpc->csn.reg = base + CPG_RPCCKCR;

	clk = clk_register_composite(NULL, name, &parent_name, 1, NULL, NULL,
				     &rpc->div.hw,  &clk_divider_ops,
				     &rpc->gate.hw, &clk_gate_ops,
				     CLK_SET_RATE_PARENT);
	if (IS_ERR(clk)) {
		kfree(rpc);
		return clk;
	}

	cpg_simple_notifier_register(notifiers, &rpc->csn);
	return clk;
}

struct rpcd2_clock {
	struct clk_fixed_factor fixed;
	struct clk_gate gate;
};

static struct clk * __init cpg_rpcd2_clk_register(const char *name,
						  void __iomem *base,
						  const char *parent_name)
{
	struct rpcd2_clock *rpcd2;
	struct clk *clk;

	rpcd2 = kzalloc(sizeof(*rpcd2), GFP_KERNEL);
	if (!rpcd2)
		return ERR_PTR(-ENOMEM);

	rpcd2->fixed.mult = 1;
	rpcd2->fixed.div = 2;

	rpcd2->gate.reg = base + CPG_RPCCKCR;
	rpcd2->gate.bit_idx = 9;
	rpcd2->gate.flags = CLK_GATE_SET_TO_DISABLE;
	rpcd2->gate.lock = &cpg_lock;

	clk = clk_register_composite(NULL, name, &parent_name, 1, NULL, NULL,
				     &rpcd2->fixed.hw, &clk_fixed_factor_ops,
				     &rpcd2->gate.hw, &clk_gate_ops,
				     CLK_SET_RATE_PARENT);
	if (IS_ERR(clk))
		kfree(rpcd2);

>>>>>>> upstream/android-13
	return clk;
}


static const struct rcar_gen3_cpg_pll_config *cpg_pll_config __initdata;
static unsigned int cpg_clk_extalr __initdata;
static u32 cpg_mode __initdata;
static u32 cpg_quirks __initdata;

#define PLL_ERRATA	BIT(0)		/* Missing PLL0/2/4 post-divider */
#define RCKCR_CKSEL	BIT(1)		/* Manual RCLK parent selection */
<<<<<<< HEAD
=======
#define SD_SKIP_FIRST	BIT(2)		/* Skip first clock in SD table */

>>>>>>> upstream/android-13

static const struct soc_device_attribute cpg_quirks_match[] __initconst = {
	{
		.soc_id = "r8a7795", .revision = "ES1.0",
<<<<<<< HEAD
		.data = (void *)(PLL_ERRATA | RCKCR_CKSEL),
	},
	{
		.soc_id = "r8a7795", .revision = "ES1.*",
		.data = (void *)RCKCR_CKSEL,
	},
	{
		.soc_id = "r8a7796", .revision = "ES1.0",
		.data = (void *)RCKCR_CKSEL,
=======
		.data = (void *)(PLL_ERRATA | RCKCR_CKSEL | SD_SKIP_FIRST),
	},
	{
		.soc_id = "r8a7795", .revision = "ES1.*",
		.data = (void *)(RCKCR_CKSEL | SD_SKIP_FIRST),
	},
	{
		.soc_id = "r8a7795", .revision = "ES2.0",
		.data = (void *)SD_SKIP_FIRST,
	},
	{
		.soc_id = "r8a7796", .revision = "ES1.0",
		.data = (void *)(RCKCR_CKSEL | SD_SKIP_FIRST),
	},
	{
		.soc_id = "r8a7796", .revision = "ES1.1",
		.data = (void *)SD_SKIP_FIRST,
>>>>>>> upstream/android-13
	},
	{ /* sentinel */ }
};

struct clk * __init rcar_gen3_cpg_clk_register(struct device *dev,
	const struct cpg_core_clk *core, const struct cpg_mssr_info *info,
	struct clk **clks, void __iomem *base,
	struct raw_notifier_head *notifiers)
{
	const struct clk *parent;
	unsigned int mult = 1;
	unsigned int div = 1;
	u32 value;

<<<<<<< HEAD
	parent = clks[core->parent & 0xffff];	/* CLK_TYPE_PE uses high bits */
=======
	parent = clks[core->parent & 0xffff];	/* some types use high bits */
>>>>>>> upstream/android-13
	if (IS_ERR(parent))
		return ERR_CAST(parent);

	switch (core->type) {
	case CLK_TYPE_GEN3_MAIN:
		div = cpg_pll_config->extal_div;
		break;

	case CLK_TYPE_GEN3_PLL0:
		/*
<<<<<<< HEAD
		 * PLL0 is a configurable multiplier clock. Register it as a
		 * fixed factor clock for now as there's no generic multiplier
		 * clock implementation and we currently have no need to change
		 * the multiplier value.
		 */
		value = readl(base + CPG_PLL0CR);
		mult = (((value >> 24) & 0x7f) + 1) * 2;
		if (cpg_quirks & PLL_ERRATA)
			mult *= 2;
		break;
=======
		 * PLL0 is implemented as a custom clock, to change the
		 * multiplier when cpufreq changes between normal and boost
		 * modes.
		 */
		mult = (cpg_quirks & PLL_ERRATA) ? 4 : 2;
		return cpg_pll_clk_register(core->name, __clk_get_name(parent),
					    base, mult, CPG_PLL0CR, 0);
>>>>>>> upstream/android-13

	case CLK_TYPE_GEN3_PLL1:
		mult = cpg_pll_config->pll1_mult;
		div = cpg_pll_config->pll1_div;
		break;

	case CLK_TYPE_GEN3_PLL2:
		/*
<<<<<<< HEAD
		 * PLL2 is a configurable multiplier clock. Register it as a
		 * fixed factor clock for now as there's no generic multiplier
		 * clock implementation and we currently have no need to change
		 * the multiplier value.
		 */
		value = readl(base + CPG_PLL2CR);
		mult = (((value >> 24) & 0x7f) + 1) * 2;
		if (cpg_quirks & PLL_ERRATA)
			mult *= 2;
		break;
=======
		 * PLL2 is implemented as a custom clock, to change the
		 * multiplier when cpufreq changes between normal and boost
		 * modes.
		 */
		mult = (cpg_quirks & PLL_ERRATA) ? 4 : 2;
		return cpg_pll_clk_register(core->name, __clk_get_name(parent),
					    base, mult, CPG_PLL2CR, 2);
>>>>>>> upstream/android-13

	case CLK_TYPE_GEN3_PLL3:
		mult = cpg_pll_config->pll3_mult;
		div = cpg_pll_config->pll3_div;
		break;

	case CLK_TYPE_GEN3_PLL4:
		/*
		 * PLL4 is a configurable multiplier clock. Register it as a
		 * fixed factor clock for now as there's no generic multiplier
		 * clock implementation and we currently have no need to change
		 * the multiplier value.
		 */
		value = readl(base + CPG_PLL4CR);
		mult = (((value >> 24) & 0x7f) + 1) * 2;
		if (cpg_quirks & PLL_ERRATA)
			mult *= 2;
		break;

	case CLK_TYPE_GEN3_SD:
<<<<<<< HEAD
		return cpg_sd_clk_register(core, base, __clk_get_name(parent),
					   notifiers);
=======
		return cpg_sd_clk_register(core->name, base, core->offset,
					   __clk_get_name(parent), notifiers,
					   cpg_quirks & SD_SKIP_FIRST);
>>>>>>> upstream/android-13

	case CLK_TYPE_GEN3_R:
		if (cpg_quirks & RCKCR_CKSEL) {
			struct cpg_simple_notifier *csn;

			csn = kzalloc(sizeof(*csn), GFP_KERNEL);
			if (!csn)
				return ERR_PTR(-ENOMEM);

			csn->reg = base + CPG_RCKCR;

			/*
			 * RINT is default.
			 * Only if EXTALR is populated, we switch to it.
			 */
			value = readl(csn->reg) & 0x3f;

			if (clk_get_rate(clks[cpg_clk_extalr])) {
				parent = clks[cpg_clk_extalr];
<<<<<<< HEAD
				value |= BIT(15);
=======
				value |= CPG_RCKCR_CKSEL;
>>>>>>> upstream/android-13
			}

			writel(value, csn->reg);
			cpg_simple_notifier_register(notifiers, csn);
			break;
		}

		/* Select parent clock of RCLK by MD28 */
		if (cpg_mode & BIT(28))
			parent = clks[cpg_clk_extalr];
		break;

<<<<<<< HEAD
	case CLK_TYPE_GEN3_PE:
		/*
		 * Peripheral clock with a fixed divider, selectable between
		 * clean and spread spectrum parents using MD12
		 */
		if (cpg_mode & BIT(12)) {
			/* Clean */
			div = core->div & 0xffff;
		} else {
			/* SCCG */
=======
	case CLK_TYPE_GEN3_MDSEL:
		/*
		 * Clock selectable between two parents and two fixed dividers
		 * using a mode pin
		 */
		if (cpg_mode & BIT(core->offset)) {
			div = core->div & 0xffff;
		} else {
>>>>>>> upstream/android-13
			parent = clks[core->parent >> 16];
			if (IS_ERR(parent))
				return ERR_CAST(parent);
			div = core->div >> 16;
		}
		mult = 1;
		break;

	case CLK_TYPE_GEN3_Z:
		return cpg_z_clk_register(core->name, __clk_get_name(parent),
<<<<<<< HEAD
					  base, CPG_FRQCRC_ZFC_MASK);

	case CLK_TYPE_GEN3_Z2:
		return cpg_z_clk_register(core->name, __clk_get_name(parent),
					  base, CPG_FRQCRC_Z2FC_MASK);
=======
					  base, core->div, core->offset);

	case CLK_TYPE_GEN3_OSC:
		/*
		 * Clock combining OSC EXTAL predivider and a fixed divider
		 */
		div = cpg_pll_config->osc_prediv * core->div;
		break;

	case CLK_TYPE_GEN3_RCKSEL:
		/*
		 * Clock selectable between two parents and two fixed dividers
		 * using RCKCR.CKSEL
		 */
		if (readl(base + CPG_RCKCR) & CPG_RCKCR_CKSEL) {
			div = core->div & 0xffff;
		} else {
			parent = clks[core->parent >> 16];
			if (IS_ERR(parent))
				return ERR_CAST(parent);
			div = core->div >> 16;
		}
		break;

	case CLK_TYPE_GEN3_RPCSRC:
		return clk_register_divider_table(NULL, core->name,
						  __clk_get_name(parent), 0,
						  base + CPG_RPCCKCR, 3, 2, 0,
						  cpg_rpcsrc_div_table,
						  &cpg_lock);

	case CLK_TYPE_GEN3_E3_RPCSRC:
		/*
		 * Register RPCSRC as fixed factor clock based on the
		 * MD[4:1] pins and CPG_RPCCKCR[4:3] register value for
		 * which has been set prior to booting the kernel.
		 */
		value = (readl(base + CPG_RPCCKCR) & GENMASK(4, 3)) >> 3;

		switch (value) {
		case 0:
			div = 5;
			break;
		case 1:
			div = 3;
			break;
		case 2:
			parent = clks[core->parent >> 16];
			if (IS_ERR(parent))
				return ERR_CAST(parent);
			div = core->div;
			break;
		case 3:
		default:
			div = 2;
			break;
		}
		break;

	case CLK_TYPE_GEN3_RPC:
		return cpg_rpc_clk_register(core->name, base,
					    __clk_get_name(parent), notifiers);

	case CLK_TYPE_GEN3_RPCD2:
		return cpg_rpcd2_clk_register(core->name, base,
					      __clk_get_name(parent));
>>>>>>> upstream/android-13

	default:
		return ERR_PTR(-EINVAL);
	}

	return clk_register_fixed_factor(NULL, core->name,
					 __clk_get_name(parent), 0, mult, div);
}

int __init rcar_gen3_cpg_init(const struct rcar_gen3_cpg_pll_config *config,
			      unsigned int clk_extalr, u32 mode)
{
	const struct soc_device_attribute *attr;

	cpg_pll_config = config;
	cpg_clk_extalr = clk_extalr;
	cpg_mode = mode;
	attr = soc_device_match(cpg_quirks_match);
	if (attr)
		cpg_quirks = (uintptr_t)attr->data;
	pr_debug("%s: mode = 0x%x quirks = 0x%x\n", __func__, mode, cpg_quirks);
<<<<<<< HEAD
=======

	spin_lock_init(&cpg_lock);

>>>>>>> upstream/android-13
	return 0;
}
