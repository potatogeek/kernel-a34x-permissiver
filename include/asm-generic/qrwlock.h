<<<<<<< HEAD
/*
 * Queue read/write lock
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Queue read/write lock
 *
>>>>>>> upstream/android-13
 * (C) Copyright 2013-2014 Hewlett-Packard Development Company, L.P.
 *
 * Authors: Waiman Long <waiman.long@hp.com>
 */
#ifndef __ASM_GENERIC_QRWLOCK_H
#define __ASM_GENERIC_QRWLOCK_H

#include <linux/atomic.h>
#include <asm/barrier.h>
#include <asm/processor.h>

#include <asm-generic/qrwlock_types.h>

<<<<<<< HEAD
=======
/* Must be included from asm/spinlock.h after defining arch_spin_is_locked.  */

>>>>>>> upstream/android-13
/*
 * Writer states & reader shift and bias.
 */
#define	_QW_WAITING	0x100		/* A writer is waiting	   */
#define	_QW_LOCKED	0x0ff		/* A writer holds the lock */
#define	_QW_WMASK	0x1ff		/* Writer mask		   */
#define	_QR_SHIFT	9		/* Reader count shift	   */
#define _QR_BIAS	(1U << _QR_SHIFT)

/*
 * External function declarations
 */
extern void queued_read_lock_slowpath(struct qrwlock *lock);
extern void queued_write_lock_slowpath(struct qrwlock *lock);

/**
 * queued_read_trylock - try to acquire read lock of a queue rwlock
 * @lock : Pointer to queue rwlock structure
 * Return: 1 if lock acquired, 0 if failed
 */
static inline int queued_read_trylock(struct qrwlock *lock)
{
<<<<<<< HEAD
	u32 cnts;
=======
	int cnts;
>>>>>>> upstream/android-13

	cnts = atomic_read(&lock->cnts);
	if (likely(!(cnts & _QW_WMASK))) {
		cnts = (u32)atomic_add_return_acquire(_QR_BIAS, &lock->cnts);
		if (likely(!(cnts & _QW_WMASK)))
			return 1;
		atomic_sub(_QR_BIAS, &lock->cnts);
	}
	return 0;
}

/**
 * queued_write_trylock - try to acquire write lock of a queue rwlock
 * @lock : Pointer to queue rwlock structure
 * Return: 1 if lock acquired, 0 if failed
 */
static inline int queued_write_trylock(struct qrwlock *lock)
{
<<<<<<< HEAD
	u32 cnts;
=======
	int cnts;
>>>>>>> upstream/android-13

	cnts = atomic_read(&lock->cnts);
	if (unlikely(cnts))
		return 0;

<<<<<<< HEAD
	return likely(atomic_cmpxchg_acquire(&lock->cnts,
					     cnts, cnts | _QW_LOCKED) == cnts);
=======
	return likely(atomic_try_cmpxchg_acquire(&lock->cnts, &cnts,
				_QW_LOCKED));
>>>>>>> upstream/android-13
}
/**
 * queued_read_lock - acquire read lock of a queue rwlock
 * @lock: Pointer to queue rwlock structure
 */
static inline void queued_read_lock(struct qrwlock *lock)
{
<<<<<<< HEAD
	u32 cnts;
=======
	int cnts;
>>>>>>> upstream/android-13

	cnts = atomic_add_return_acquire(_QR_BIAS, &lock->cnts);
	if (likely(!(cnts & _QW_WMASK)))
		return;

	/* The slowpath will decrement the reader count, if necessary. */
	queued_read_lock_slowpath(lock);
}

/**
 * queued_write_lock - acquire write lock of a queue rwlock
 * @lock : Pointer to queue rwlock structure
 */
static inline void queued_write_lock(struct qrwlock *lock)
{
<<<<<<< HEAD
	/* Optimize for the unfair lock case where the fair flag is 0. */
	if (atomic_cmpxchg_acquire(&lock->cnts, 0, _QW_LOCKED) == 0)
=======
	int cnts = 0;
	/* Optimize for the unfair lock case where the fair flag is 0. */
	if (likely(atomic_try_cmpxchg_acquire(&lock->cnts, &cnts, _QW_LOCKED)))
>>>>>>> upstream/android-13
		return;

	queued_write_lock_slowpath(lock);
}

/**
 * queued_read_unlock - release read lock of a queue rwlock
 * @lock : Pointer to queue rwlock structure
 */
static inline void queued_read_unlock(struct qrwlock *lock)
{
	/*
	 * Atomically decrement the reader count
	 */
	(void)atomic_sub_return_release(_QR_BIAS, &lock->cnts);
}

/**
 * queued_write_unlock - release write lock of a queue rwlock
 * @lock : Pointer to queue rwlock structure
 */
static inline void queued_write_unlock(struct qrwlock *lock)
{
	smp_store_release(&lock->wlocked, 0);
}

<<<<<<< HEAD
=======
/**
 * queued_rwlock_is_contended - check if the lock is contended
 * @lock : Pointer to queue rwlock structure
 * Return: 1 if lock contended, 0 otherwise
 */
static inline int queued_rwlock_is_contended(struct qrwlock *lock)
{
	return arch_spin_is_locked(&lock->wait_lock);
}

>>>>>>> upstream/android-13
/*
 * Remapping rwlock architecture specific functions to the corresponding
 * queue rwlock functions.
 */
<<<<<<< HEAD
#define arch_read_lock(l)	queued_read_lock(l)
#define arch_write_lock(l)	queued_write_lock(l)
#define arch_read_trylock(l)	queued_read_trylock(l)
#define arch_write_trylock(l)	queued_write_trylock(l)
#define arch_read_unlock(l)	queued_read_unlock(l)
#define arch_write_unlock(l)	queued_write_unlock(l)
=======
#define arch_read_lock(l)		queued_read_lock(l)
#define arch_write_lock(l)		queued_write_lock(l)
#define arch_read_trylock(l)		queued_read_trylock(l)
#define arch_write_trylock(l)		queued_write_trylock(l)
#define arch_read_unlock(l)		queued_read_unlock(l)
#define arch_write_unlock(l)		queued_write_unlock(l)
#define arch_rwlock_is_contended(l)	queued_rwlock_is_contended(l)
>>>>>>> upstream/android-13

#endif /* __ASM_GENERIC_QRWLOCK_H */
