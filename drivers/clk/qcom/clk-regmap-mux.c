<<<<<<< HEAD
/*
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/bitops.h>
#include <linux/regmap.h>
#include <linux/export.h>

#include "clk-regmap-mux.h"

static inline struct clk_regmap_mux *to_clk_regmap_mux(struct clk_hw *hw)
{
	return container_of(to_clk_regmap(hw), struct clk_regmap_mux, clkr);
}

static u8 mux_get_parent(struct clk_hw *hw)
{
	struct clk_regmap_mux *mux = to_clk_regmap_mux(hw);
	struct clk_regmap *clkr = to_clk_regmap(hw);
	unsigned int mask = GENMASK(mux->width - 1, 0);
	unsigned int val;

	regmap_read(clkr->regmap, mux->reg, &val);

	val >>= mux->shift;
	val &= mask;

	if (mux->parent_map)
<<<<<<< HEAD
		return qcom_find_src_index(hw, mux->parent_map, val);
=======
		return qcom_find_cfg_index(hw, mux->parent_map, val);
>>>>>>> upstream/android-13

	return val;
}

static int mux_set_parent(struct clk_hw *hw, u8 index)
{
	struct clk_regmap_mux *mux = to_clk_regmap_mux(hw);
	struct clk_regmap *clkr = to_clk_regmap(hw);
	unsigned int mask = GENMASK(mux->width + mux->shift - 1, mux->shift);
	unsigned int val;

	if (mux->parent_map)
		index = mux->parent_map[index].cfg;

	val = index;
	val <<= mux->shift;

	return regmap_update_bits(clkr->regmap, mux->reg, mask, val);
}

const struct clk_ops clk_regmap_mux_closest_ops = {
	.get_parent = mux_get_parent,
	.set_parent = mux_set_parent,
	.determine_rate = __clk_mux_determine_rate_closest,
};
EXPORT_SYMBOL_GPL(clk_regmap_mux_closest_ops);
