<<<<<<< HEAD
/*
 * Copyright (C) 2012 Regents of the University of California
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 */

#include <linux/reboot.h>
#include <linux/export.h>
#include <asm/sbi.h>

void (*pm_power_off)(void) = machine_power_off;
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Regents of the University of California
 */

#include <linux/reboot.h>
#include <linux/pm.h>

static void default_power_off(void)
{
	while (1)
		wait_for_interrupt();
}

void (*pm_power_off)(void) = default_power_off;
>>>>>>> upstream/android-13
EXPORT_SYMBOL(pm_power_off);

void machine_restart(char *cmd)
{
	do_kernel_restart(cmd);
	while (1);
}

void machine_halt(void)
{
<<<<<<< HEAD
	machine_power_off();
=======
	pm_power_off();
>>>>>>> upstream/android-13
}

void machine_power_off(void)
{
<<<<<<< HEAD
	sbi_shutdown();
	while (1);
=======
	pm_power_off();
>>>>>>> upstream/android-13
}
