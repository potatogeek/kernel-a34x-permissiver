/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H

#include <asm/barrier.h>
#include <asm/ldcw.h>
#include <asm/processor.h>
#include <asm/spinlock_types.h>

static inline int arch_spin_is_locked(arch_spinlock_t *x)
{
	volatile unsigned int *a = __ldcw_align(x);
<<<<<<< HEAD
	return *a == 0;
}

#define arch_spin_lock(lock) arch_spin_lock_flags(lock, 0)

static inline void arch_spin_lock_flags(arch_spinlock_t *x,
					 unsigned long flags)
=======
	return READ_ONCE(*a) == 0;
}

static inline void arch_spin_lock(arch_spinlock_t *x)
{
	volatile unsigned int *a;

	a = __ldcw_align(x);
	while (__ldcw(a) == 0)
		while (*a == 0)
			continue;
}

static inline void arch_spin_lock_flags(arch_spinlock_t *x,
					unsigned long flags)
>>>>>>> upstream/android-13
{
	volatile unsigned int *a;

	a = __ldcw_align(x);
	while (__ldcw(a) == 0)
		while (*a == 0)
			if (flags & PSW_SM_I) {
				local_irq_enable();
<<<<<<< HEAD
				cpu_relax();
				local_irq_disable();
			} else
				cpu_relax();
=======
				local_irq_disable();
			}
>>>>>>> upstream/android-13
}
#define arch_spin_lock_flags arch_spin_lock_flags

static inline void arch_spin_unlock(arch_spinlock_t *x)
{
	volatile unsigned int *a;

	a = __ldcw_align(x);
<<<<<<< HEAD
	mb();
	*a = 1;
=======
	/* Release with ordered store. */
	__asm__ __volatile__("stw,ma %0,0(%1)" : : "r"(1), "r"(a) : "memory");
>>>>>>> upstream/android-13
}

static inline int arch_spin_trylock(arch_spinlock_t *x)
{
	volatile unsigned int *a;
<<<<<<< HEAD
	int ret;

	a = __ldcw_align(x);
        ret = __ldcw(a) != 0;

	return ret;
=======

	a = __ldcw_align(x);
	return __ldcw(a) != 0;
>>>>>>> upstream/android-13
}

/*
 * Read-write spinlocks, allowing multiple readers but only one writer.
<<<<<<< HEAD
 * Linux rwlocks are unfair to writers; they can be starved for an indefinite
 * time by readers.  With care, they can also be taken in interrupt context.
 *
 * In the PA-RISC implementation, we have a spinlock and a counter.
 * Readers use the lock to serialise their access to the counter (which
 * records how many readers currently hold the lock).
 * Writers hold the spinlock, preventing any readers or other writers from
 * grabbing the rwlock.
 */

/* Note that we have to ensure interrupts are disabled in case we're
 * interrupted by some other code that wants to grab the same read lock */
static  __inline__ void arch_read_lock(arch_rwlock_t *rw)
{
	unsigned long flags;
	local_irq_save(flags);
	arch_spin_lock_flags(&rw->lock, flags);
	rw->counter++;
	arch_spin_unlock(&rw->lock);
	local_irq_restore(flags);
}

/* Note that we have to ensure interrupts are disabled in case we're
 * interrupted by some other code that wants to grab the same read lock */
static  __inline__ void arch_read_unlock(arch_rwlock_t *rw)
{
	unsigned long flags;
	local_irq_save(flags);
	arch_spin_lock_flags(&rw->lock, flags);
	rw->counter--;
	arch_spin_unlock(&rw->lock);
	local_irq_restore(flags);
}

/* Note that we have to ensure interrupts are disabled in case we're
 * interrupted by some other code that wants to grab the same read lock */
static __inline__ int arch_read_trylock(arch_rwlock_t *rw)
{
	unsigned long flags;
 retry:
	local_irq_save(flags);
	if (arch_spin_trylock(&rw->lock)) {
		rw->counter++;
		arch_spin_unlock(&rw->lock);
		local_irq_restore(flags);
		return 1;
	}

	local_irq_restore(flags);
	/* If write-locked, we fail to acquire the lock */
	if (rw->counter < 0)
		return 0;

	/* Wait until we have a realistic chance at the lock */
	while (arch_spin_is_locked(&rw->lock) && rw->counter >= 0)
		cpu_relax();

	goto retry;
}

/* Note that we have to ensure interrupts are disabled in case we're
 * interrupted by some other code that wants to read_trylock() this lock */
static __inline__ void arch_write_lock(arch_rwlock_t *rw)
{
	unsigned long flags;
retry:
	local_irq_save(flags);
	arch_spin_lock_flags(&rw->lock, flags);

	if (rw->counter != 0) {
		arch_spin_unlock(&rw->lock);
		local_irq_restore(flags);

		while (rw->counter != 0)
			cpu_relax();

		goto retry;
	}

	rw->counter = -1; /* mark as write-locked */
	mb();
	local_irq_restore(flags);
}

static __inline__ void arch_write_unlock(arch_rwlock_t *rw)
{
	rw->counter = 0;
	arch_spin_unlock(&rw->lock);
}

/* Note that we have to ensure interrupts are disabled in case we're
 * interrupted by some other code that wants to read_trylock() this lock */
static __inline__ int arch_write_trylock(arch_rwlock_t *rw)
{
	unsigned long flags;
	int result = 0;

	local_irq_save(flags);
	if (arch_spin_trylock(&rw->lock)) {
		if (rw->counter == 0) {
			rw->counter = -1;
			result = 1;
		} else {
			/* Read-locked.  Oh well. */
			arch_spin_unlock(&rw->lock);
		}
	}
	local_irq_restore(flags);

	return result;
=======
 * Unfair locking as Writers could be starved indefinitely by Reader(s)
 *
 * The spinlock itself is contained in @counter and access to it is
 * serialized with @lock_mutex.
 */

/* 1 - lock taken successfully */
static inline int arch_read_trylock(arch_rwlock_t *rw)
{
	int ret = 0;
	unsigned long flags;

	local_irq_save(flags);
	arch_spin_lock(&(rw->lock_mutex));

	/*
	 * zero means writer holds the lock exclusively, deny Reader.
	 * Otherwise grant lock to first/subseq reader
	 */
	if (rw->counter > 0) {
		rw->counter--;
		ret = 1;
	}

	arch_spin_unlock(&(rw->lock_mutex));
	local_irq_restore(flags);

	return ret;
}

/* 1 - lock taken successfully */
static inline int arch_write_trylock(arch_rwlock_t *rw)
{
	int ret = 0;
	unsigned long flags;

	local_irq_save(flags);
	arch_spin_lock(&(rw->lock_mutex));

	/*
	 * If reader(s) hold lock (lock < __ARCH_RW_LOCK_UNLOCKED__),
	 * deny writer. Otherwise if unlocked grant to writer
	 * Hence the claim that Linux rwlocks are unfair to writers.
	 * (can be starved for an indefinite time by readers).
	 */
	if (rw->counter == __ARCH_RW_LOCK_UNLOCKED__) {
		rw->counter = 0;
		ret = 1;
	}
	arch_spin_unlock(&(rw->lock_mutex));
	local_irq_restore(flags);

	return ret;
}

static inline void arch_read_lock(arch_rwlock_t *rw)
{
	while (!arch_read_trylock(rw))
		cpu_relax();
}

static inline void arch_write_lock(arch_rwlock_t *rw)
{
	while (!arch_write_trylock(rw))
		cpu_relax();
}

static inline void arch_read_unlock(arch_rwlock_t *rw)
{
	unsigned long flags;

	local_irq_save(flags);
	arch_spin_lock(&(rw->lock_mutex));
	rw->counter++;
	arch_spin_unlock(&(rw->lock_mutex));
	local_irq_restore(flags);
}

static inline void arch_write_unlock(arch_rwlock_t *rw)
{
	unsigned long flags;

	local_irq_save(flags);
	arch_spin_lock(&(rw->lock_mutex));
	rw->counter = __ARCH_RW_LOCK_UNLOCKED__;
	arch_spin_unlock(&(rw->lock_mutex));
	local_irq_restore(flags);
>>>>>>> upstream/android-13
}

#endif /* __ASM_SPINLOCK_H */
