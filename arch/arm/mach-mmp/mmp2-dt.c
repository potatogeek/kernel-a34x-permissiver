<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/mach-mmp/mmp2-dt.c
 *
 *  Copyright (C) 2012 Marvell Technology Group Ltd.
 *  Author: Haojian Zhuang <haojian.zhuang@marvell.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  publishhed by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/io.h>
#include <linux/irqchip.h>
#include <linux/of_platform.h>
<<<<<<< HEAD
#include <linux/clk-provider.h>
=======
#include <linux/of_clk.h>
#include <linux/clocksource.h>
>>>>>>> upstream/android-13
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/hardware/cache-tauros2.h>

#include "common.h"

<<<<<<< HEAD
extern void __init mmp_dt_init_timer(void);

=======
>>>>>>> upstream/android-13
static void __init mmp_init_time(void)
{
#ifdef CONFIG_CACHE_TAUROS2
	tauros2_init(0);
#endif
<<<<<<< HEAD
	mmp_dt_init_timer();
	of_clk_init(NULL);
}

static const char *const mmp2_dt_board_compat[] __initconst = {
	"mrvl,mmp2-brownstone",
=======
	of_clk_init(NULL);
	timer_probe();
}

static const char *const mmp2_dt_board_compat[] __initconst = {
	"mrvl,mmp2",
>>>>>>> upstream/android-13
	NULL,
};

DT_MACHINE_START(MMP2_DT, "Marvell MMP2 (Device Tree Support)")
<<<<<<< HEAD
	.map_io		= mmp_map_io,
=======
	.map_io		= mmp2_map_io,
>>>>>>> upstream/android-13
	.init_time	= mmp_init_time,
	.dt_compat	= mmp2_dt_board_compat,
MACHINE_END
