<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * arch/arm/mach-ep93xx/gesbc9312.c
 * Glomation GESBC-9312-sx support.
 *
 * Copyright (C) 2006 Lennert Buytenhek <buytenh@wantstofly.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/sizes.h>

<<<<<<< HEAD
#include <mach/hardware.h>
=======
#include "hardware.h"
>>>>>>> upstream/android-13

#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include "soc.h"

static struct ep93xx_eth_data __initdata gesbc9312_eth_data = {
	.phy_id		= 1,
};

static void __init gesbc9312_init_machine(void)
{
	ep93xx_init_devices();
	ep93xx_register_flash(4, EP93XX_CS6_PHYS_BASE, SZ_8M);
	ep93xx_register_eth(&gesbc9312_eth_data, 0);
}

MACHINE_START(GESBC9312, "Glomation GESBC-9312-sx")
	/* Maintainer: Lennert Buytenhek <buytenh@wantstofly.org> */
	.atag_offset	= 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
	.init_time	= ep93xx_timer_init,
	.init_machine	= gesbc9312_init_machine,
<<<<<<< HEAD
	.init_late	= ep93xx_init_late,
=======
>>>>>>> upstream/android-13
	.restart	= ep93xx_restart,
MACHINE_END
