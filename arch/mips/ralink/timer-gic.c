<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
>>>>>>> upstream/android-13
 *
 * Copyright (C) 2015 Nikolay Martynov <mar.kolya@gmail.com>
 * Copyright (C) 2015 John Crispin <john@phrozen.org>
 */

#include <linux/init.h>

#include <linux/of.h>
<<<<<<< HEAD
#include <linux/clk-provider.h>
=======
#include <linux/of_clk.h>
>>>>>>> upstream/android-13
#include <linux/clocksource.h>

#include "common.h"

void __init plat_time_init(void)
{
	ralink_of_remap();

	of_clk_init(NULL);
	timer_probe();
}
