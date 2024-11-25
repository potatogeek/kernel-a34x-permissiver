/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BITOPS_ATOMIC_H_
#define _ASM_GENERIC_BITOPS_ATOMIC_H_

#include <linux/atomic.h>
#include <linux/compiler.h>
#include <asm/barrier.h>

/*
 * Implementation of atomic bitops using atomic-fetch ops.
 * See Documentation/atomic_bitops.txt for details.
 */

<<<<<<< HEAD
static inline void set_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	atomic_long_or(BIT_MASK(nr), (atomic_long_t *)p);
}

static inline void clear_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	atomic_long_andnot(BIT_MASK(nr), (atomic_long_t *)p);
}

static inline void change_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	atomic_long_xor(BIT_MASK(nr), (atomic_long_t *)p);
}

static inline int test_and_set_bit(unsigned int nr, volatile unsigned long *p)
=======
static __always_inline void
arch_set_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	arch_atomic_long_or(BIT_MASK(nr), (atomic_long_t *)p);
}

static __always_inline void
arch_clear_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	arch_atomic_long_andnot(BIT_MASK(nr), (atomic_long_t *)p);
}

static __always_inline void
arch_change_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	arch_atomic_long_xor(BIT_MASK(nr), (atomic_long_t *)p);
}

static __always_inline int
arch_test_and_set_bit(unsigned int nr, volatile unsigned long *p)
>>>>>>> upstream/android-13
{
	long old;
	unsigned long mask = BIT_MASK(nr);

	p += BIT_WORD(nr);
	if (READ_ONCE(*p) & mask)
		return 1;

<<<<<<< HEAD
	old = atomic_long_fetch_or(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

static inline int test_and_clear_bit(unsigned int nr, volatile unsigned long *p)
=======
	old = arch_atomic_long_fetch_or(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

static __always_inline int
arch_test_and_clear_bit(unsigned int nr, volatile unsigned long *p)
>>>>>>> upstream/android-13
{
	long old;
	unsigned long mask = BIT_MASK(nr);

	p += BIT_WORD(nr);
	if (!(READ_ONCE(*p) & mask))
		return 0;

<<<<<<< HEAD
	old = atomic_long_fetch_andnot(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

static inline int test_and_change_bit(unsigned int nr, volatile unsigned long *p)
=======
	old = arch_atomic_long_fetch_andnot(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

static __always_inline int
arch_test_and_change_bit(unsigned int nr, volatile unsigned long *p)
>>>>>>> upstream/android-13
{
	long old;
	unsigned long mask = BIT_MASK(nr);

	p += BIT_WORD(nr);
<<<<<<< HEAD
	old = atomic_long_fetch_xor(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

=======
	old = arch_atomic_long_fetch_xor(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

#include <asm-generic/bitops/instrumented-atomic.h>

>>>>>>> upstream/android-13
#endif /* _ASM_GENERIC_BITOPS_ATOMIC_H */
