<<<<<<< HEAD
/*
 * Copyright 2016 NXP Semiconductors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright 2016 NXP Semiconductors
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/suspend.h>
#include <linux/io.h>

static int imx25_suspend_enter(suspend_state_t state)
{
	if (!IS_ENABLED(CONFIG_PM))
		return 0;

	switch (state) {
	case PM_SUSPEND_MEM:
		cpu_do_idle();
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static const struct platform_suspend_ops imx25_suspend_ops = {
	.enter = imx25_suspend_enter,
	.valid = suspend_valid_only_mem,
};

void __init imx25_pm_init(void)
{
	suspend_set_ops(&imx25_suspend_ops);
}
