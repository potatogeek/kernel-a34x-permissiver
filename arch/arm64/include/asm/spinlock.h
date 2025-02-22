<<<<<<< HEAD
/*
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H

<<<<<<< HEAD
#include <asm/qrwlock.h>
#include <asm/qspinlock.h>
=======
#include <asm/qspinlock.h>
#include <asm/qrwlock.h>
>>>>>>> upstream/android-13

/* See include/linux/spinlock.h */
#define smp_mb__after_spinlock()	smp_mb()

<<<<<<< HEAD
=======
/*
 * Changing this will break osq_lock() thanks to the call inside
 * smp_cond_load_relaxed().
 *
 * See:
 * https://lore.kernel.org/lkml/20200110100612.GC2827@hirez.programming.kicks-ass.net
 */
#define vcpu_is_preempted vcpu_is_preempted
static inline bool vcpu_is_preempted(int cpu)
{
	return false;
}

>>>>>>> upstream/android-13
#endif /* __ASM_SPINLOCK_H */
