<<<<<<< HEAD
/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
>>>>>>> upstream/android-13
 */

#include <linux/cpuidle.h>
#include <linux/module.h>
#include <asm/cpuidle.h>

#include <soc/imx/cpuidle.h>

#include "common.h"
#include "cpuidle.h"
#include "hardware.h"

static int num_idle_cpus = 0;
<<<<<<< HEAD
static DEFINE_SPINLOCK(cpuidle_lock);
=======
static DEFINE_RAW_SPINLOCK(cpuidle_lock);
>>>>>>> upstream/android-13

static int imx6q_enter_wait(struct cpuidle_device *dev,
			    struct cpuidle_driver *drv, int index)
{
<<<<<<< HEAD
	spin_lock(&cpuidle_lock);
	if (++num_idle_cpus == num_online_cpus())
		imx6_set_lpm(WAIT_UNCLOCKED);
	spin_unlock(&cpuidle_lock);

	cpu_do_idle();

	spin_lock(&cpuidle_lock);
	if (num_idle_cpus-- == num_online_cpus())
		imx6_set_lpm(WAIT_CLOCKED);
	spin_unlock(&cpuidle_lock);
=======
	raw_spin_lock(&cpuidle_lock);
	if (++num_idle_cpus == num_online_cpus())
		imx6_set_lpm(WAIT_UNCLOCKED);
	raw_spin_unlock(&cpuidle_lock);

	rcu_idle_enter();
	cpu_do_idle();
	rcu_idle_exit();

	raw_spin_lock(&cpuidle_lock);
	if (num_idle_cpus-- == num_online_cpus())
		imx6_set_lpm(WAIT_CLOCKED);
	raw_spin_unlock(&cpuidle_lock);
>>>>>>> upstream/android-13

	return index;
}

static struct cpuidle_driver imx6q_cpuidle_driver = {
	.name = "imx6q_cpuidle",
	.owner = THIS_MODULE,
	.states = {
		/* WFI */
		ARM_CPUIDLE_WFI_STATE,
		/* WAIT */
		{
			.exit_latency = 50,
			.target_residency = 75,
<<<<<<< HEAD
			.flags = CPUIDLE_FLAG_TIMER_STOP,
=======
			.flags = CPUIDLE_FLAG_TIMER_STOP | CPUIDLE_FLAG_RCU_IDLE,
>>>>>>> upstream/android-13
			.enter = imx6q_enter_wait,
			.name = "WAIT",
			.desc = "Clock off",
		},
	},
	.state_count = 2,
	.safe_state_index = 0,
};

/*
 * i.MX6 Q/DL has an erratum (ERR006687) that prevents the FEC from waking the
 * CPUs when they are in wait(unclocked) state. As the hardware workaround isn't
 * applicable to all boards, disable the deeper idle state when the workaround
 * isn't present and the FEC is in use.
 */
void imx6q_cpuidle_fec_irqs_used(void)
{
<<<<<<< HEAD
	imx6q_cpuidle_driver.states[1].disabled = true;
=======
	cpuidle_driver_state_disabled(&imx6q_cpuidle_driver, 1, true);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(imx6q_cpuidle_fec_irqs_used);

void imx6q_cpuidle_fec_irqs_unused(void)
{
<<<<<<< HEAD
	imx6q_cpuidle_driver.states[1].disabled = false;
=======
	cpuidle_driver_state_disabled(&imx6q_cpuidle_driver, 1, false);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(imx6q_cpuidle_fec_irqs_unused);

int __init imx6q_cpuidle_init(void)
{
	/* Set INT_MEM_CLK_LPM bit to get a reliable WAIT mode support */
	imx6_set_int_mem_clk_lpm(true);

	return cpuidle_register(&imx6q_cpuidle_driver, NULL);
}
