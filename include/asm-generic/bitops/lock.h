/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BITOPS_LOCK_H_
#define _ASM_GENERIC_BITOPS_LOCK_H_

#include <linux/atomic.h>
#include <linux/compiler.h>
#include <asm/barrier.h>

/**
<<<<<<< HEAD
 * test_and_set_bit_lock - Set a bit and return its old value, for lock
=======
 * arch_test_and_set_bit_lock - Set a bit and return its old value, for lock
>>>>>>> upstream/android-13
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and provides acquire barrier semantics if
 * the returned value is 0.
 * It can be used to implement bit locks.
 */
<<<<<<< HEAD
static inline int test_and_set_bit_lock(unsigned int nr,
					volatile unsigned long *p)
=======
static __always_inline int
arch_test_and_set_bit_lock(unsigned int nr, volatile unsigned long *p)
>>>>>>> upstream/android-13
{
	long old;
	unsigned long mask = BIT_MASK(nr);

	p += BIT_WORD(nr);
	if (READ_ONCE(*p) & mask)
		return 1;

<<<<<<< HEAD
	old = atomic_long_fetch_or_acquire(mask, (atomic_long_t *)p);
=======
	old = arch_atomic_long_fetch_or_acquire(mask, (atomic_long_t *)p);
>>>>>>> upstream/android-13
	return !!(old & mask);
}


/**
<<<<<<< HEAD
 * clear_bit_unlock - Clear a bit in memory, for unlock
=======
 * arch_clear_bit_unlock - Clear a bit in memory, for unlock
>>>>>>> upstream/android-13
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * This operation is atomic and provides release barrier semantics.
 */
<<<<<<< HEAD
static inline void clear_bit_unlock(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	atomic_long_fetch_andnot_release(BIT_MASK(nr), (atomic_long_t *)p);
}

/**
 * __clear_bit_unlock - Clear a bit in memory, for unlock
=======
static __always_inline void
arch_clear_bit_unlock(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	arch_atomic_long_fetch_andnot_release(BIT_MASK(nr), (atomic_long_t *)p);
}

/**
 * arch___clear_bit_unlock - Clear a bit in memory, for unlock
>>>>>>> upstream/android-13
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * A weaker form of clear_bit_unlock() as used by __bit_lock_unlock(). If all
 * the bits in the word are protected by this lock some archs can use weaker
 * ops to safely unlock.
 *
 * See for example x86's implementation.
 */
<<<<<<< HEAD
static inline void __clear_bit_unlock(unsigned int nr,
				      volatile unsigned long *p)
=======
static inline void
arch___clear_bit_unlock(unsigned int nr, volatile unsigned long *p)
>>>>>>> upstream/android-13
{
	unsigned long old;

	p += BIT_WORD(nr);
	old = READ_ONCE(*p);
	old &= ~BIT_MASK(nr);
<<<<<<< HEAD
	atomic_long_set_release((atomic_long_t *)p, old);
}

/**
 * clear_bit_unlock_is_negative_byte - Clear a bit in memory and test if bottom
 *                                     byte is negative, for unlock.
=======
	arch_atomic_long_set_release((atomic_long_t *)p, old);
}

/**
 * arch_clear_bit_unlock_is_negative_byte - Clear a bit in memory and test if bottom
 *                                          byte is negative, for unlock.
>>>>>>> upstream/android-13
 * @nr: the bit to clear
 * @addr: the address to start counting from
 *
 * This is a bit of a one-trick-pony for the filemap code, which clears
 * PG_locked and tests PG_waiters,
 */
<<<<<<< HEAD
#ifndef clear_bit_unlock_is_negative_byte
static inline bool clear_bit_unlock_is_negative_byte(unsigned int nr,
						     volatile unsigned long *p)
=======
#ifndef arch_clear_bit_unlock_is_negative_byte
static inline bool arch_clear_bit_unlock_is_negative_byte(unsigned int nr,
							  volatile unsigned long *p)
>>>>>>> upstream/android-13
{
	long old;
	unsigned long mask = BIT_MASK(nr);

	p += BIT_WORD(nr);
<<<<<<< HEAD
	old = atomic_long_fetch_andnot_release(mask, (atomic_long_t *)p);
	return !!(old & BIT(7));
}
#define clear_bit_unlock_is_negative_byte clear_bit_unlock_is_negative_byte
#endif

=======
	old = arch_atomic_long_fetch_andnot_release(mask, (atomic_long_t *)p);
	return !!(old & BIT(7));
}
#define arch_clear_bit_unlock_is_negative_byte arch_clear_bit_unlock_is_negative_byte
#endif

#include <asm-generic/bitops/instrumented-lock.h>

>>>>>>> upstream/android-13
#endif /* _ASM_GENERIC_BITOPS_LOCK_H_ */
