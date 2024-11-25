/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_POWERPC_SPINLOCK_TYPES_H
#define _ASM_POWERPC_SPINLOCK_TYPES_H

#ifndef __LINUX_SPINLOCK_TYPES_H
# error "please don't include this file directly"
#endif

<<<<<<< HEAD
typedef struct {
	volatile unsigned int slock;
} arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED	{ 0 }

typedef struct {
	volatile signed int lock;
} arch_rwlock_t;

#define __ARCH_RW_LOCK_UNLOCKED		{ 0 }
=======
#ifdef CONFIG_PPC_QUEUED_SPINLOCKS
#include <asm-generic/qspinlock_types.h>
#include <asm-generic/qrwlock_types.h>
#else
#include <asm/simple_spinlock_types.h>
#endif
>>>>>>> upstream/android-13

#endif
