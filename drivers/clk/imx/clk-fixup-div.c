<<<<<<< HEAD
/*
 * Copyright (C) 2013 Freescale Semiconductor, Inc.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2013 Freescale Semiconductor, Inc.
>>>>>>> upstream/android-13
 */

#include <linux/clk-provider.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/slab.h>
#include "clk.h"

#define div_mask(d)	((1 << (d->width)) - 1)

/**
 * struct clk_fixup_div - imx integer fixup divider clock
 * @divider: the parent class
 * @ops: pointer to clk_ops of parent class
 * @fixup: a hook to fixup the write value
 *
 * The imx fixup divider clock is a subclass of basic clk_divider
 * with an addtional fixup hook.
 */
struct clk_fixup_div {
	struct clk_divider divider;
	const struct clk_ops *ops;
	void (*fixup)(u32 *val);
};

static inline struct clk_fixup_div *to_clk_fixup_div(struct clk_hw *hw)
{
	struct clk_divider *divider = to_clk_divider(hw);

	return container_of(divider, struct clk_fixup_div, divider);
}

static unsigned long clk_fixup_div_recalc_rate(struct clk_hw *hw,
					 unsigned long parent_rate)
{
	struct clk_fixup_div *fixup_div = to_clk_fixup_div(hw);

	return fixup_div->ops->recalc_rate(&fixup_div->divider.hw, parent_rate);
}

static long clk_fixup_div_round_rate(struct clk_hw *hw, unsigned long rate,
			       unsigned long *prate)
{
	struct clk_fixup_div *fixup_div = to_clk_fixup_div(hw);

	return fixup_div->ops->round_rate(&fixup_div->divider.hw, rate, prate);
}

static int clk_fixup_div_set_rate(struct clk_hw *hw, unsigned long rate,
			    unsigned long parent_rate)
{
	struct clk_fixup_div *fixup_div = to_clk_fixup_div(hw);
	struct clk_divider *div = to_clk_divider(hw);
	unsigned int divider, value;
<<<<<<< HEAD
	unsigned long flags = 0;
=======
	unsigned long flags;
>>>>>>> upstream/android-13
	u32 val;

	divider = parent_rate / rate;

	/* Zero based divider */
	value = divider - 1;

	if (value > div_mask(div))
		value = div_mask(div);

	spin_lock_irqsave(div->lock, flags);

	val = readl(div->reg);
	val &= ~(div_mask(div) << div->shift);
	val |= value << div->shift;
	fixup_div->fixup(&val);
	writel(val, div->reg);

	spin_unlock_irqrestore(div->lock, flags);

	return 0;
}

static const struct clk_ops clk_fixup_div_ops = {
	.recalc_rate = clk_fixup_div_recalc_rate,
	.round_rate = clk_fixup_div_round_rate,
	.set_rate = clk_fixup_div_set_rate,
};

<<<<<<< HEAD
struct clk *imx_clk_fixup_divider(const char *name, const char *parent,
=======
struct clk_hw *imx_clk_hw_fixup_divider(const char *name, const char *parent,
>>>>>>> upstream/android-13
				  void __iomem *reg, u8 shift, u8 width,
				  void (*fixup)(u32 *val))
{
	struct clk_fixup_div *fixup_div;
<<<<<<< HEAD
	struct clk *clk;
	struct clk_init_data init = {};
=======
	struct clk_hw *hw;
	struct clk_init_data init;
	int ret;
>>>>>>> upstream/android-13

	if (!fixup)
		return ERR_PTR(-EINVAL);

	fixup_div = kzalloc(sizeof(*fixup_div), GFP_KERNEL);
	if (!fixup_div)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &clk_fixup_div_ops;
	init.flags = CLK_SET_RATE_PARENT;
	init.parent_names = parent ? &parent : NULL;
	init.num_parents = parent ? 1 : 0;

	fixup_div->divider.reg = reg;
	fixup_div->divider.shift = shift;
	fixup_div->divider.width = width;
	fixup_div->divider.lock = &imx_ccm_lock;
	fixup_div->divider.hw.init = &init;
	fixup_div->ops = &clk_divider_ops;
	fixup_div->fixup = fixup;

<<<<<<< HEAD
	clk = clk_register(NULL, &fixup_div->divider.hw);
	if (IS_ERR(clk))
		kfree(fixup_div);

	return clk;
=======
	hw = &fixup_div->divider.hw;

	ret = clk_hw_register(NULL, hw);
	if (ret) {
		kfree(fixup_div);
		return ERR_PTR(ret);
	}

	return hw;
>>>>>>> upstream/android-13
}
