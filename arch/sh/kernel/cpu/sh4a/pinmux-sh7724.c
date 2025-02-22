<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * SH7724 Pinmux
 *
 * Copyright (C) 2009 Renesas Solutions Corp.
 *
 * Kuninori Morimoto <morimoto.kuninori@renesas.com>
 *
 * Based on SH7723 Pinmux
 *  Copyright (C) 2008  Magnus Damm
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/bug.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <cpu/pfc.h>

static struct resource sh7724_pfc_resources[] = {
	[0] = {
		.start	= 0xa4050100,
		.end	= 0xa405016f,
		.flags	= IORESOURCE_MEM,
	},
};

static int __init plat_pinmux_setup(void)
{
	return sh_pfc_register("pfc-sh7724", sh7724_pfc_resources,
			       ARRAY_SIZE(sh7724_pfc_resources));
}
arch_initcall(plat_pinmux_setup);
