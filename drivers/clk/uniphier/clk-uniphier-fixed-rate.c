<<<<<<< HEAD
/*
 * Copyright (C) 2016 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2016 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
>>>>>>> upstream/android-13
 */

#include <linux/clk-provider.h>
#include <linux/device.h>

#include "clk-uniphier.h"

struct clk_hw *uniphier_clk_register_fixed_rate(struct device *dev,
						const char *name,
				const struct uniphier_clk_fixed_rate_data *data)
{
	struct clk_fixed_rate *fixed;
<<<<<<< HEAD
	struct clk_init_data init = {};
=======
	struct clk_init_data init;
>>>>>>> upstream/android-13
	int ret;

	/* allocate fixed-rate clock */
	fixed = devm_kzalloc(dev, sizeof(*fixed), GFP_KERNEL);
	if (!fixed)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &clk_fixed_rate_ops;
<<<<<<< HEAD
=======
	init.flags = 0;
>>>>>>> upstream/android-13
	init.parent_names = NULL;
	init.num_parents = 0;

	fixed->fixed_rate = data->fixed_rate;
	fixed->hw.init = &init;

	ret = devm_clk_hw_register(dev, &fixed->hw);
	if (ret)
		return ERR_PTR(ret);

	return &fixed->hw;
}
