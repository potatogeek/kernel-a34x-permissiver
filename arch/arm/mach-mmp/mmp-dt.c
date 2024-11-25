<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/mach-mmp/mmp-dt.c
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
static const char *const pxa168_dt_board_compat[] __initconst = {
	"mrvl,pxa168-aspenite",
	NULL,
};

static const char *const pxa910_dt_board_compat[] __initconst = {
	"mrvl,pxa910-dkb",
	NULL,
};

static void __init mmp_init_time(void)
{
#ifdef CONFIG_CACHE_TAUROS2
	tauros2_init(0);
#endif
<<<<<<< HEAD
	mmp_dt_init_timer();
	of_clk_init(NULL);
=======
	of_clk_init(NULL);
	timer_probe();
>>>>>>> upstream/android-13
}

DT_MACHINE_START(PXA168_DT, "Marvell PXA168 (Device Tree Support)")
	.map_io		= mmp_map_io,
	.init_time	= mmp_init_time,
	.dt_compat	= pxa168_dt_board_compat,
MACHINE_END

DT_MACHINE_START(PXA910_DT, "Marvell PXA910 (Device Tree Support)")
	.map_io		= mmp_map_io,
	.init_time	= mmp_init_time,
	.dt_compat	= pxa910_dt_board_compat,
MACHINE_END
