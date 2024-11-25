<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Spin and read/write lock operations.
 *
 * Copyright (C) 2001-2004 Paul Mackerras <paulus@au.ibm.com>, IBM
 * Copyright (C) 2001 Anton Blanchard <anton@au.ibm.com>, IBM
 * Copyright (C) 2002 Dave Engebretsen <engebret@us.ibm.com>, IBM
 *   Rework to support virtual processors
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/export.h>
#include <linux/smp.h>

/* waiting for a spinlock... */
#if defined(CONFIG_PPC_SPLPAR)
#include <asm/hvcall.h>
#include <asm/smp.h>

<<<<<<< HEAD
void __spin_yield(arch_spinlock_t *lock)
=======
void splpar_spin_yield(arch_spinlock_t *lock)
>>>>>>> upstream/android-13
{
	unsigned int lock_value, holder_cpu, yield_count;

	lock_value = lock->slock;
	if (lock_value == 0)
		return;
	holder_cpu = lock_value & 0xffff;
	BUG_ON(holder_cpu >= NR_CPUS);
<<<<<<< HEAD
	yield_count = be32_to_cpu(lppaca_of(holder_cpu).yield_count);
=======

	yield_count = yield_count_of(holder_cpu);
>>>>>>> upstream/android-13
	if ((yield_count & 1) == 0)
		return;		/* virtual cpu is currently running */
	rmb();
	if (lock->slock != lock_value)
		return;		/* something has changed */
<<<<<<< HEAD
	plpar_hcall_norets(H_CONFER,
		get_hard_smp_processor_id(holder_cpu), yield_count);
}
EXPORT_SYMBOL_GPL(__spin_yield);
=======
	yield_to_preempted(holder_cpu, yield_count);
}
EXPORT_SYMBOL_GPL(splpar_spin_yield);
>>>>>>> upstream/android-13

/*
 * Waiting for a read lock or a write lock on a rwlock...
 * This turns out to be the same for read and write locks, since
 * we only know the holder if it is write-locked.
 */
<<<<<<< HEAD
void __rw_yield(arch_rwlock_t *rw)
=======
void splpar_rw_yield(arch_rwlock_t *rw)
>>>>>>> upstream/android-13
{
	int lock_value;
	unsigned int holder_cpu, yield_count;

	lock_value = rw->lock;
	if (lock_value >= 0)
		return;		/* no write lock at present */
	holder_cpu = lock_value & 0xffff;
	BUG_ON(holder_cpu >= NR_CPUS);
<<<<<<< HEAD
	yield_count = be32_to_cpu(lppaca_of(holder_cpu).yield_count);
=======

	yield_count = yield_count_of(holder_cpu);
>>>>>>> upstream/android-13
	if ((yield_count & 1) == 0)
		return;		/* virtual cpu is currently running */
	rmb();
	if (rw->lock != lock_value)
		return;		/* something has changed */
<<<<<<< HEAD
	plpar_hcall_norets(H_CONFER,
		get_hard_smp_processor_id(holder_cpu), yield_count);
=======
	yield_to_preempted(holder_cpu, yield_count);
>>>>>>> upstream/android-13
}
#endif
