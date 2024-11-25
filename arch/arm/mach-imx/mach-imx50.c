<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright 2013 Greg Ungerer <gerg@uclinux.org>
 * Copyright 2011 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2011 Linaro Ltd.
<<<<<<< HEAD
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
=======
>>>>>>> upstream/android-13
 */

#include <linux/of_platform.h>
#include <asm/mach/arch.h>

#include "common.h"
<<<<<<< HEAD
=======
#include "hardware.h"

static void __init imx50_init_early(void)
{
	mxc_set_cpu_type(MXC_CPU_MX50);
}
>>>>>>> upstream/android-13

static const char * const imx50_dt_board_compat[] __initconst = {
	"fsl,imx50",
	NULL
};

DT_MACHINE_START(IMX50_DT, "Freescale i.MX50 (Device Tree Support)")
<<<<<<< HEAD
=======
	.init_early	= imx50_init_early,
>>>>>>> upstream/android-13
	.dt_compat	= imx50_dt_board_compat,
MACHINE_END
