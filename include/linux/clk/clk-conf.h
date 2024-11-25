<<<<<<< HEAD
/*
 * Copyright (C) 2014 Samsung Electronics Co., Ltd.
 * Sylwester Nawrocki <s.nawrocki@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2014 Samsung Electronics Co., Ltd.
 * Sylwester Nawrocki <s.nawrocki@samsung.com>
 */

#ifndef __CLK_CONF_H
#define __CLK_CONF_H

>>>>>>> upstream/android-13
#include <linux/types.h>

struct device_node;

#if defined(CONFIG_OF) && defined(CONFIG_COMMON_CLK)
int of_clk_set_defaults(struct device_node *node, bool clk_supplier);
#else
static inline int of_clk_set_defaults(struct device_node *node,
				      bool clk_supplier)
{
	return 0;
}
#endif
<<<<<<< HEAD
=======

#endif /* __CLK_CONF_H */
>>>>>>> upstream/android-13
