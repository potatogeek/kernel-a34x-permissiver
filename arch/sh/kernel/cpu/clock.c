<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * arch/sh/kernel/cpu/clock.c - SuperH clock framework
 *
 *  Copyright (C) 2005 - 2009  Paul Mundt
 *
 * This clock framework is derived from the OMAP version by:
 *
 *	Copyright (C) 2004 - 2008 Nokia Corporation
 *	Written by Tuukka Tikkanen <tuukka.tikkanen@elektrobit.com>
 *
 *  Modified for omap shared clock framework by Tony Lindgren <tony@atomide.com>
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <asm/clock.h>
#include <asm/machvec.h>

int __init clk_init(void)
{
	int ret;

#ifndef CONFIG_COMMON_CLK
	ret = arch_clk_init();
	if (unlikely(ret)) {
		pr_err("%s: CPU clock registration failed.\n", __func__);
		return ret;
	}
#endif

	if (sh_mv.mv_clk_init) {
		ret = sh_mv.mv_clk_init();
		if (unlikely(ret)) {
			pr_err("%s: machvec clock initialization failed.\n",
			       __func__);
			return ret;
		}
	}

#ifndef CONFIG_COMMON_CLK
	/* Kick the child clocks.. */
	recalculate_root_clocks();

	/* Enable the necessary init clocks */
	clk_enable_init_clocks();
#endif

	return ret;
}


