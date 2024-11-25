<<<<<<< HEAD
/*
 * Copyright (c) 2012-2016 Zhang, Keguang <keguang.zhang@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2012-2016 Zhang, Keguang <keguang.zhang@gmail.com>
>>>>>>> upstream/android-13
 */

#include <linux/clk-provider.h>
#include <linux/slab.h>

<<<<<<< HEAD
=======
#include "clk.h"

>>>>>>> upstream/android-13
struct clk_hw *__init clk_hw_register_pll(struct device *dev,
					  const char *name,
					  const char *parent_name,
					  const struct clk_ops *ops,
					  unsigned long flags)
{
	int ret;
	struct clk_hw *hw;
<<<<<<< HEAD
	struct clk_init_data init = {};
=======
	struct clk_init_data init;
>>>>>>> upstream/android-13

	/* allocate the divider */
	hw = kzalloc(sizeof(*hw), GFP_KERNEL);
	if (!hw)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = ops;
<<<<<<< HEAD
	init.flags = flags | CLK_IS_BASIC;
	init.parent_names = (parent_name ? &parent_name : NULL);
	init.num_parents = (parent_name ? 1 : 0);
=======
	init.flags = flags;
	init.parent_names = parent_name ? &parent_name : NULL;
	init.num_parents = parent_name ? 1 : 0;
>>>>>>> upstream/android-13
	hw->init = &init;

	/* register the clock */
	ret = clk_hw_register(dev, hw);
	if (ret) {
		kfree(hw);
		hw = ERR_PTR(ret);
	}

	return hw;
}
