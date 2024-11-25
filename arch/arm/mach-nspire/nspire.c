<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *	linux/arch/arm/mach-nspire/nspire.c
 *
 *	Copyright (C) 2013 Daniel Tang <tangrs@tangrs.id.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/irqchip.h>
#include <linux/irqchip/arm-vic.h>
#include <linux/clkdev.h>
#include <linux/amba/bus.h>
<<<<<<< HEAD
#include <linux/amba/clcd.h>
=======
>>>>>>> upstream/android-13

#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <asm/mach/map.h>

#include "mmio.h"
<<<<<<< HEAD
#include "clcd.h"
=======
>>>>>>> upstream/android-13

static const char *const nspire_dt_match[] __initconst = {
	"ti,nspire",
	"ti,nspire-cx",
	"ti,nspire-tp",
	"ti,nspire-clp",
	NULL,
};

<<<<<<< HEAD
static struct clcd_board nspire_clcd_data = {
	.name		= "LCD",
	.caps		= CLCD_CAP_5551 | CLCD_CAP_565,
	.check		= clcdfb_check,
	.decode		= clcdfb_decode,
	.setup		= nspire_clcd_setup,
	.mmap		= nspire_clcd_mmap,
	.remove		= nspire_clcd_remove,
};


static struct of_dev_auxdata nspire_auxdata[] __initdata = {
	OF_DEV_AUXDATA("arm,pl111", NSPIRE_LCD_PHYS_BASE,
			NULL, &nspire_clcd_data),
	{ }
};

static void __init nspire_init(void)
{
	of_platform_default_populate(NULL, nspire_auxdata, NULL);
}

=======
>>>>>>> upstream/android-13
static void nspire_restart(enum reboot_mode mode, const char *cmd)
{
	void __iomem *base = ioremap(NSPIRE_MISC_PHYS_BASE, SZ_4K);
	if (!base)
		return;

	writel(2, base + NSPIRE_MISC_HWRESET);
}

DT_MACHINE_START(NSPIRE, "TI-NSPIRE")
	.dt_compat	= nspire_dt_match,
<<<<<<< HEAD
	.init_machine	= nspire_init,
=======
>>>>>>> upstream/android-13
	.restart	= nspire_restart,
MACHINE_END
