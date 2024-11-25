<<<<<<< HEAD
/*
 * am33xx-restart.c - Code common to all AM33xx machines.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * am33xx-restart.c - Code common to all AM33xx machines.
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/reboot.h>

#include "common.h"
#include "prm.h"

/**
 * am3xx_restart - trigger a software restart of the SoC
 * @mode: the "reboot mode", see arch/arm/kernel/{setup,process}.c
 * @cmd: passed from the userspace program rebooting the system (if provided)
 *
 * Resets the SoC.  For @cmd, see the 'reboot' syscall in
 * kernel/sys.c.  No return value.
 */
void am33xx_restart(enum reboot_mode mode, const char *cmd)
{
	/* TODO: Handle mode and cmd if necessary */

	omap_prm_reset_system();
}
