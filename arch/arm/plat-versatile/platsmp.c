<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/plat-versatile/platsmp.c
 *
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
 * This code is specific to the hardware found on ARM Realview and
 * Versatile Express platforms where the CPUs are unable to be individually
 * woken, and where there is no way to hot-unplug CPUs.  Real platforms
 * should not copy this code.
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <linux/smp.h>

#include <asm/cacheflush.h>
#include <asm/smp_plat.h>

#include <plat/platsmp.h>

/*
<<<<<<< HEAD
 * Write pen_release in a way that is guaranteed to be visible to all
 * observers, irrespective of whether they're taking part in coherency
 * or not.  This is necessary for the hotplug code to work reliably.
 */
static void write_pen_release(int val)
{
	pen_release = val;
	smp_wmb();
	sync_cache_w(&pen_release);
}

static DEFINE_SPINLOCK(boot_lock);
=======
 * versatile_cpu_release controls the release of CPUs from the holding
 * pen in headsmp.S, which exists because we are not always able to
 * control the release of individual CPUs from the board firmware.
 * Production platforms do not need this.
 */
volatile int versatile_cpu_release = -1;

/*
 * Write versatile_cpu_release in a way that is guaranteed to be visible to
 * all observers, irrespective of whether they're taking part in coherency
 * or not.  This is necessary for the hotplug code to work reliably.
 */
static void versatile_write_cpu_release(int val)
{
	versatile_cpu_release = val;
	smp_wmb();
	sync_cache_w(&versatile_cpu_release);
}

/*
 * versatile_lock exists to avoid running the loops_per_jiffy delay loop
 * calibrations on the secondary CPU while the requesting CPU is using
 * the limited-bandwidth bus - which affects the calibration value.
 * Production platforms do not need this.
 */
static DEFINE_RAW_SPINLOCK(versatile_lock);
>>>>>>> upstream/android-13

void versatile_secondary_init(unsigned int cpu)
{
	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
<<<<<<< HEAD
	write_pen_release(-1);
=======
	versatile_write_cpu_release(-1);
>>>>>>> upstream/android-13

	/*
	 * Synchronise with the boot thread.
	 */
<<<<<<< HEAD
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
=======
	raw_spin_lock(&versatile_lock);
	raw_spin_unlock(&versatile_lock);
>>>>>>> upstream/android-13
}

int versatile_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;

	/*
	 * Set synchronisation state between this boot processor
	 * and the secondary one
	 */
<<<<<<< HEAD
	spin_lock(&boot_lock);
=======
	raw_spin_lock(&versatile_lock);
>>>>>>> upstream/android-13

	/*
	 * This is really belt and braces; we hold unintended secondary
	 * CPUs in the holding pen until we're ready for them.  However,
	 * since we haven't sent them a soft interrupt, they shouldn't
	 * be there.
	 */
<<<<<<< HEAD
	write_pen_release(cpu_logical_map(cpu));
=======
	versatile_write_cpu_release(cpu_logical_map(cpu));
>>>>>>> upstream/android-13

	/*
	 * Send the secondary CPU a soft interrupt, thereby causing
	 * the boot monitor to read the system wide flags register,
	 * and branch to the address found there.
	 */
	arch_send_wakeup_ipi_mask(cpumask_of(cpu));

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		smp_rmb();
<<<<<<< HEAD
		if (pen_release == -1)
=======
		if (versatile_cpu_release == -1)
>>>>>>> upstream/android-13
			break;

		udelay(10);
	}

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
<<<<<<< HEAD
	spin_unlock(&boot_lock);

	return pen_release != -1 ? -ENOSYS : 0;
=======
	raw_spin_unlock(&versatile_lock);

	return versatile_cpu_release != -1 ? -ENOSYS : 0;
>>>>>>> upstream/android-13
}
