// SPDX-License-Identifier:	GPL-2.0
/*
 * Copyright (C) 2017, Intel Corporation
 */
#include <linux/slab.h>
#include <linux/clk-provider.h>
<<<<<<< HEAD
=======
#include <linux/io.h>
>>>>>>> upstream/android-13

#include "stratix10-clk.h"
#include "clk.h"

#define CLK_MGR_FREE_SHIFT		16
#define CLK_MGR_FREE_MASK		0x7
#define SWCTRLBTCLKSEN_SHIFT		8

#define to_periph_clk(p) container_of(p, struct socfpga_periph_clk, hw.hw)

<<<<<<< HEAD
=======
static unsigned long n5x_clk_peri_c_clk_recalc_rate(struct clk_hw *hwclk,
					     unsigned long parent_rate)
{
	struct socfpga_periph_clk *socfpgaclk = to_periph_clk(hwclk);
	unsigned long div;
	unsigned long shift = socfpgaclk->shift;
	u32 val;

	val = readl(socfpgaclk->hw.reg);
	val &= (0x1f << shift);
	div = (val >> shift) + 1;

	return parent_rate / div;
}

>>>>>>> upstream/android-13
static unsigned long clk_peri_c_clk_recalc_rate(struct clk_hw *hwclk,
					     unsigned long parent_rate)
{
	struct socfpga_periph_clk *socfpgaclk = to_periph_clk(hwclk);
	unsigned long div = 1;
	u32 val;

	val = readl(socfpgaclk->hw.reg);
	val &= GENMASK(SWCTRLBTCLKSEN_SHIFT - 1, 0);
	parent_rate /= val;

	return parent_rate / div;
}

static unsigned long clk_peri_cnt_clk_recalc_rate(struct clk_hw *hwclk,
					     unsigned long parent_rate)
{
	struct socfpga_periph_clk *socfpgaclk = to_periph_clk(hwclk);
	unsigned long div = 1;

	if (socfpgaclk->fixed_div) {
		div = socfpgaclk->fixed_div;
	} else {
		if (socfpgaclk->hw.reg)
			div = ((readl(socfpgaclk->hw.reg) & 0x7ff) + 1);
	}

	return parent_rate / div;
}

static u8 clk_periclk_get_parent(struct clk_hw *hwclk)
{
	struct socfpga_periph_clk *socfpgaclk = to_periph_clk(hwclk);
	u32 clk_src, mask;
<<<<<<< HEAD
	u8 parent;

=======
	u8 parent = 0;

	/* handle the bypass first */
>>>>>>> upstream/android-13
	if (socfpgaclk->bypass_reg) {
		mask = (0x1 << socfpgaclk->bypass_shift);
		parent = ((readl(socfpgaclk->bypass_reg) & mask) >>
			   socfpgaclk->bypass_shift);
<<<<<<< HEAD
	} else {
		clk_src = readl(socfpgaclk->hw.reg);
		parent = (clk_src >> CLK_MGR_FREE_SHIFT) &
			CLK_MGR_FREE_MASK;
=======
		if (parent)
			return parent;
	}

	if (socfpgaclk->hw.reg) {
		clk_src = readl(socfpgaclk->hw.reg);
		parent = (clk_src >> CLK_MGR_FREE_SHIFT) &
			  CLK_MGR_FREE_MASK;
>>>>>>> upstream/android-13
	}
	return parent;
}

<<<<<<< HEAD
=======
static const struct clk_ops n5x_peri_c_clk_ops = {
	.recalc_rate = n5x_clk_peri_c_clk_recalc_rate,
	.get_parent = clk_periclk_get_parent,
};

>>>>>>> upstream/android-13
static const struct clk_ops peri_c_clk_ops = {
	.recalc_rate = clk_peri_c_clk_recalc_rate,
	.get_parent = clk_periclk_get_parent,
};

static const struct clk_ops peri_cnt_clk_ops = {
	.recalc_rate = clk_peri_cnt_clk_recalc_rate,
	.get_parent = clk_periclk_get_parent,
};

<<<<<<< HEAD
struct clk *s10_register_periph(const char *name, const char *parent_name,
				const char * const *parent_names,
				u8 num_parents, unsigned long flags,
				void __iomem *reg, unsigned long offset)
{
	struct clk *clk;
	struct socfpga_periph_clk *periph_clk;
	struct clk_init_data init = {};
=======
struct clk_hw *s10_register_periph(const struct stratix10_perip_c_clock *clks,
				void __iomem *reg)
{
	struct clk_hw *hw_clk;
	struct socfpga_periph_clk *periph_clk;
	struct clk_init_data init;
	const char *name = clks->name;
	const char *parent_name = clks->parent_name;
	int ret;
>>>>>>> upstream/android-13

	periph_clk = kzalloc(sizeof(*periph_clk), GFP_KERNEL);
	if (WARN_ON(!periph_clk))
		return NULL;

<<<<<<< HEAD
	periph_clk->hw.reg = reg + offset;

	init.name = name;
	init.ops = &peri_c_clk_ops;
	init.flags = flags;

	init.num_parents = num_parents;
	init.parent_names = parent_names ? parent_names : &parent_name;

	periph_clk->hw.hw.init = &init;

	clk = clk_register(NULL, &periph_clk->hw.hw);
	if (WARN_ON(IS_ERR(clk))) {
		kfree(periph_clk);
		return NULL;
	}
	return clk;
}

struct clk *s10_register_cnt_periph(const char *name, const char *parent_name,
				    const char * const *parent_names,
				    u8 num_parents, unsigned long flags,
				    void __iomem *regbase, unsigned long offset,
				    u8 fixed_divider, unsigned long bypass_reg,
				    unsigned long bypass_shift)
{
	struct clk *clk;
	struct socfpga_periph_clk *periph_clk;
	struct clk_init_data init = {};
=======
	periph_clk->hw.reg = reg + clks->offset;

	init.name = name;
	init.ops = &peri_c_clk_ops;
	init.flags = clks->flags;

	init.num_parents = clks->num_parents;
	init.parent_names = parent_name ? &parent_name : NULL;
	if (init.parent_names == NULL)
		init.parent_data = clks->parent_data;

	periph_clk->hw.hw.init = &init;
	hw_clk = &periph_clk->hw.hw;

	ret = clk_hw_register(NULL, hw_clk);
	if (ret) {
		kfree(periph_clk);
		return ERR_PTR(ret);
	}
	return hw_clk;
}

struct clk_hw *n5x_register_periph(const struct n5x_perip_c_clock *clks,
				void __iomem *regbase)
{
	struct clk_hw *hw_clk;
	struct socfpga_periph_clk *periph_clk;
	struct clk_init_data init;
	const char *name = clks->name;
	const char *parent_name = clks->parent_name;
	int ret;
>>>>>>> upstream/android-13

	periph_clk = kzalloc(sizeof(*periph_clk), GFP_KERNEL);
	if (WARN_ON(!periph_clk))
		return NULL;

<<<<<<< HEAD
	if (offset)
		periph_clk->hw.reg = regbase + offset;
	else
		periph_clk->hw.reg = NULL;

	if (bypass_reg)
		periph_clk->bypass_reg = regbase + bypass_reg;
	else
		periph_clk->bypass_reg = NULL;
	periph_clk->bypass_shift = bypass_shift;
	periph_clk->fixed_div = fixed_divider;

	init.name = name;
	init.ops = &peri_cnt_clk_ops;
	init.flags = flags;

	init.num_parents = num_parents;
	init.parent_names = parent_names ? parent_names : &parent_name;

	periph_clk->hw.hw.init = &init;

	clk = clk_register(NULL, &periph_clk->hw.hw);
	if (WARN_ON(IS_ERR(clk))) {
		kfree(periph_clk);
		return NULL;
	}
	return clk;
=======
	periph_clk->hw.reg = regbase + clks->offset;
	periph_clk->shift = clks->shift;

	init.name = name;
	init.ops = &n5x_peri_c_clk_ops;
	init.flags = clks->flags;

	init.num_parents = clks->num_parents;
	init.parent_names = parent_name ? &parent_name : NULL;

	periph_clk->hw.hw.init = &init;
	hw_clk = &periph_clk->hw.hw;

	ret = clk_hw_register(NULL, hw_clk);
	if (ret) {
		kfree(periph_clk);
		return ERR_PTR(ret);
	}
	return hw_clk;
}

struct clk_hw *s10_register_cnt_periph(const struct stratix10_perip_cnt_clock *clks,
				    void __iomem *regbase)
{
	struct clk_hw *hw_clk;
	struct socfpga_periph_clk *periph_clk;
	struct clk_init_data init;
	const char *name = clks->name;
	const char *parent_name = clks->parent_name;
	int ret;

	periph_clk = kzalloc(sizeof(*periph_clk), GFP_KERNEL);
	if (WARN_ON(!periph_clk))
		return NULL;

	if (clks->offset)
		periph_clk->hw.reg = regbase + clks->offset;
	else
		periph_clk->hw.reg = NULL;

	if (clks->bypass_reg)
		periph_clk->bypass_reg = regbase + clks->bypass_reg;
	else
		periph_clk->bypass_reg = NULL;
	periph_clk->bypass_shift = clks->bypass_shift;
	periph_clk->fixed_div = clks->fixed_divider;

	init.name = name;
	init.ops = &peri_cnt_clk_ops;
	init.flags = clks->flags;

	init.num_parents = clks->num_parents;
	init.parent_names = parent_name ? &parent_name : NULL;
	if (init.parent_names == NULL)
		init.parent_data = clks->parent_data;

	periph_clk->hw.hw.init = &init;
	hw_clk = &periph_clk->hw.hw;

	ret = clk_hw_register(NULL, hw_clk);
	if (ret) {
		kfree(periph_clk);
		return ERR_PTR(ret);
	}
	return hw_clk;
>>>>>>> upstream/android-13
}
