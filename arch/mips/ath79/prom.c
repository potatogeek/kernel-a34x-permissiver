<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  Atheros AR71XX/AR724X/AR913X specific prom routines
 *
 *  Copyright (C) 2015 Laurent Fasnacht <l@libres.ch>
 *  Copyright (C) 2008-2010 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/string.h>
#include <linux/initrd.h>

#include <asm/bootinfo.h>
#include <asm/addrspace.h>
#include <asm/fw/fw.h>

#include "common.h"

void __init prom_init(void)
{
	fw_init_cmdline();

#ifdef CONFIG_BLK_DEV_INITRD
	/* Read the initrd address from the firmware environment */
	initrd_start = fw_getenvl("initrd_start");
	if (initrd_start) {
		initrd_start = KSEG0ADDR(initrd_start);
		initrd_end = initrd_start + fw_getenvl("initrd_size");
	}
#endif
}
<<<<<<< HEAD

void __init prom_free_prom_memory(void)
{
	/* We do not have to prom memory to free */
}
=======
>>>>>>> upstream/android-13
