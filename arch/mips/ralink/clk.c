<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
>>>>>>> upstream/android-13
 *
 *  Copyright (C) 2011 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2013 John Crispin <john@phrozen.org>
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/export.h>
#include <linux/clkdev.h>
#include <linux/clk.h>
<<<<<<< HEAD
=======
#include <linux/clk-provider.h>
>>>>>>> upstream/android-13

#include <asm/time.h>

#include "common.h"

<<<<<<< HEAD
struct clk {
	struct clk_lookup cl;
	unsigned long rate;
};

void ralink_clk_add(const char *dev, unsigned long rate)
{
	struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
=======
void ralink_clk_add(const char *dev, unsigned long rate)
{
	struct clk *clk = clk_register_fixed_rate(NULL, dev, NULL, 0, rate);
>>>>>>> upstream/android-13

	if (!clk)
		panic("failed to add clock");

<<<<<<< HEAD
	clk->cl.dev_id = dev;
	clk->cl.clk = clk;

	clk->rate = rate;

	clkdev_add(&clk->cl);
}

/*
 * Linux clock API
 */
int clk_enable(struct clk *clk)
{
	return 0;
}
EXPORT_SYMBOL_GPL(clk_enable);

void clk_disable(struct clk *clk)
{
}
EXPORT_SYMBOL_GPL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	if (!clk)
		return 0;

	return clk->rate;
}
EXPORT_SYMBOL_GPL(clk_get_rate);

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	return -1;
}
EXPORT_SYMBOL_GPL(clk_set_rate);

long clk_round_rate(struct clk *clk, unsigned long rate)
{
	return -1;
}
EXPORT_SYMBOL_GPL(clk_round_rate);

=======
	clkdev_create(clk, NULL, "%s", dev);
}

>>>>>>> upstream/android-13
void __init plat_time_init(void)
{
	struct clk *clk;

	ralink_of_remap();

	ralink_clk_init();
	clk = clk_get_sys("cpu", NULL);
	if (IS_ERR(clk))
		panic("unable to get CPU clock, err=%ld", PTR_ERR(clk));
	pr_info("CPU Clock: %ldMHz\n", clk_get_rate(clk) / 1000000);
	mips_hpt_frequency = clk_get_rate(clk) / 2;
	clk_put(clk);
	timer_probe();
}
