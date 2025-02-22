/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BITOPS_NON_ATOMIC_H_
#define _ASM_GENERIC_BITOPS_NON_ATOMIC_H_

#include <asm/types.h>

/**
<<<<<<< HEAD
 * __set_bit - Set a bit in memory
=======
 * arch___set_bit - Set a bit in memory
>>>>>>> upstream/android-13
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
<<<<<<< HEAD
static inline void __set_bit(int nr, volatile unsigned long *addr)
=======
static __always_inline void
arch___set_bit(unsigned int nr, volatile unsigned long *addr)
>>>>>>> upstream/android-13
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p  |= mask;
}
<<<<<<< HEAD

static inline void __clear_bit(int nr, volatile unsigned long *addr)
=======
#define __set_bit arch___set_bit

static __always_inline void
arch___clear_bit(unsigned int nr, volatile unsigned long *addr)
>>>>>>> upstream/android-13
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p &= ~mask;
}
<<<<<<< HEAD

/**
 * __change_bit - Toggle a bit in memory
=======
#define __clear_bit arch___clear_bit

/**
 * arch___change_bit - Toggle a bit in memory
>>>>>>> upstream/android-13
 * @nr: the bit to change
 * @addr: the address to start counting from
 *
 * Unlike change_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
<<<<<<< HEAD
static inline void __change_bit(int nr, volatile unsigned long *addr)
=======
static __always_inline
void arch___change_bit(unsigned int nr, volatile unsigned long *addr)
>>>>>>> upstream/android-13
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p ^= mask;
}
<<<<<<< HEAD

/**
 * __test_and_set_bit - Set a bit and return its old value
=======
#define __change_bit arch___change_bit

/**
 * arch___test_and_set_bit - Set a bit and return its old value
>>>>>>> upstream/android-13
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
<<<<<<< HEAD
static inline int __test_and_set_bit(int nr, volatile unsigned long *addr)
=======
static __always_inline int
arch___test_and_set_bit(unsigned int nr, volatile unsigned long *addr)
>>>>>>> upstream/android-13
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old = *p;

	*p = old | mask;
	return (old & mask) != 0;
}
<<<<<<< HEAD

/**
 * __test_and_clear_bit - Clear a bit and return its old value
=======
#define __test_and_set_bit arch___test_and_set_bit

/**
 * arch___test_and_clear_bit - Clear a bit and return its old value
>>>>>>> upstream/android-13
 * @nr: Bit to clear
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
<<<<<<< HEAD
static inline int __test_and_clear_bit(int nr, volatile unsigned long *addr)
=======
static __always_inline int
arch___test_and_clear_bit(unsigned int nr, volatile unsigned long *addr)
>>>>>>> upstream/android-13
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old = *p;

	*p = old & ~mask;
	return (old & mask) != 0;
}
<<<<<<< HEAD

/* WARNING: non atomic and it can be reordered! */
static inline int __test_and_change_bit(int nr,
					    volatile unsigned long *addr)
=======
#define __test_and_clear_bit arch___test_and_clear_bit

/* WARNING: non atomic and it can be reordered! */
static __always_inline int
arch___test_and_change_bit(unsigned int nr, volatile unsigned long *addr)
>>>>>>> upstream/android-13
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old = *p;

	*p = old ^ mask;
	return (old & mask) != 0;
}
<<<<<<< HEAD

/**
 * test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
static inline int test_bit(int nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}
=======
#define __test_and_change_bit arch___test_and_change_bit

/**
 * arch_test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
static __always_inline int
arch_test_bit(unsigned int nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}
#define test_bit arch_test_bit
>>>>>>> upstream/android-13

#endif /* _ASM_GENERIC_BITOPS_NON_ATOMIC_H_ */
