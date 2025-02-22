<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * PowerPC 64-bit swsusp implementation
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
<<<<<<< HEAD
 *
 * GPLv2
=======
>>>>>>> upstream/android-13
 */

#include <asm/iommu.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/nmi.h>

void do_after_copyback(void)
{
	iommu_restore();
	touch_softlockup_watchdog();
	mb();
}

void _iommu_save(void)
{
	iommu_save();
}
