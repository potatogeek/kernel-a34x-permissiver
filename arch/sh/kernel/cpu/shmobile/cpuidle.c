<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * arch/sh/kernel/cpu/shmobile/cpuidle.c
 *
 * Cpuidle support code for SuperH Mobile
 *
 *  Copyright (C) 2009 Magnus Damm
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/suspend.h>
#include <linux/cpuidle.h>
#include <linux/export.h>
#include <asm/suspend.h>
#include <linux/uaccess.h>

static unsigned long cpuidle_mode[] = {
	SUSP_SH_SLEEP, /* regular sleep mode */
	SUSP_SH_SLEEP | SUSP_SH_SF, /* sleep mode + self refresh */
	SUSP_SH_STANDBY | SUSP_SH_SF, /* software standby mode + self refresh */
};

static int cpuidle_sleep_enter(struct cpuidle_device *dev,
				struct cpuidle_driver *drv,
				int index)
{
	unsigned long allowed_mode = SUSP_SH_SLEEP;
	int requested_state = index;
	int allowed_state;
	int k;

	/* convert allowed mode to allowed state */
	for (k = ARRAY_SIZE(cpuidle_mode) - 1; k > 0; k--)
		if (cpuidle_mode[k] == allowed_mode)
			break;

	allowed_state = k;

	/* take the following into account for sleep mode selection:
	 * - allowed_state: best mode allowed by hardware (clock deps)
	 * - requested_state: best mode allowed by software (latencies)
	 */
	k = min_t(int, allowed_state, requested_state);

	sh_mobile_call_standby(cpuidle_mode[k]);

	return k;
}

static struct cpuidle_driver cpuidle_driver = {
	.name   = "sh_idle",
	.owner  = THIS_MODULE,
	.states = {
		{
			.exit_latency = 1,
			.target_residency = 1 * 2,
			.power_usage = 3,
			.enter = cpuidle_sleep_enter,
			.name = "C1",
			.desc = "SuperH Sleep Mode",
		},
		{
			.exit_latency = 100,
			.target_residency = 1 * 2,
			.power_usage = 1,
			.enter = cpuidle_sleep_enter,
			.name = "C2",
			.desc = "SuperH Sleep Mode [SF]",
<<<<<<< HEAD
			.disabled = true,
=======
			.flags = CPUIDLE_FLAG_UNUSABLE,
>>>>>>> upstream/android-13
		},
		{
			.exit_latency = 2300,
			.target_residency = 1 * 2,
			.power_usage = 1,
			.enter = cpuidle_sleep_enter,
			.name = "C3",
			.desc = "SuperH Mobile Standby Mode [SF]",
<<<<<<< HEAD
			.disabled = true,
=======
			.flags = CPUIDLE_FLAG_UNUSABLE,
>>>>>>> upstream/android-13
		},
	},
	.safe_state_index = 0,
	.state_count = 3,
};

int __init sh_mobile_setup_cpuidle(void)
{
	if (sh_mobile_sleep_supported & SUSP_SH_SF)
<<<<<<< HEAD
		cpuidle_driver.states[1].disabled = false;

	if (sh_mobile_sleep_supported & SUSP_SH_STANDBY)
		cpuidle_driver.states[2].disabled = false;
=======
		cpuidle_driver.states[1].flags = CPUIDLE_FLAG_NONE;

	if (sh_mobile_sleep_supported & SUSP_SH_STANDBY)
		cpuidle_driver.states[2].flags = CPUIDLE_FLAG_NONE;
>>>>>>> upstream/android-13

	return cpuidle_register(&cpuidle_driver, NULL);
}
