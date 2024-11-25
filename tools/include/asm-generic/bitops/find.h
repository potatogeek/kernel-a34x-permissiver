/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _TOOLS_LINUX_ASM_GENERIC_BITOPS_FIND_H_
#define _TOOLS_LINUX_ASM_GENERIC_BITOPS_FIND_H_

<<<<<<< HEAD
=======
extern unsigned long _find_next_bit(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long nbits,
		unsigned long start, unsigned long invert, unsigned long le);
extern unsigned long _find_first_bit(const unsigned long *addr, unsigned long size);
extern unsigned long _find_first_zero_bit(const unsigned long *addr, unsigned long size);
extern unsigned long _find_last_bit(const unsigned long *addr, unsigned long size);

>>>>>>> upstream/android-13
#ifndef find_next_bit
/**
 * find_next_bit - find the next set bit in a memory region
 * @addr: The address to base the search on
 * @offset: The bitnumber to start searching at
 * @size: The bitmap size in bits
 *
 * Returns the bit number for the next set bit
 * If no bits are set, returns @size.
 */
<<<<<<< HEAD
extern unsigned long find_next_bit(const unsigned long *addr, unsigned long
		size, unsigned long offset);
=======
static inline
unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
			    unsigned long offset)
{
	if (small_const_nbits(size)) {
		unsigned long val;

		if (unlikely(offset >= size))
			return size;

		val = *addr & GENMASK(size - 1, offset);
		return val ? __ffs(val) : size;
	}

	return _find_next_bit(addr, NULL, size, offset, 0UL, 0);
}
>>>>>>> upstream/android-13
#endif

#ifndef find_next_and_bit
/**
 * find_next_and_bit - find the next set bit in both memory regions
 * @addr1: The first address to base the search on
 * @addr2: The second address to base the search on
 * @offset: The bitnumber to start searching at
 * @size: The bitmap size in bits
 *
 * Returns the bit number for the next set bit
 * If no bits are set, returns @size.
 */
<<<<<<< HEAD
extern unsigned long find_next_and_bit(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long size,
		unsigned long offset);
#endif

#ifndef find_next_zero_bit

=======
static inline
unsigned long find_next_and_bit(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long size,
		unsigned long offset)
{
	if (small_const_nbits(size)) {
		unsigned long val;

		if (unlikely(offset >= size))
			return size;

		val = *addr1 & *addr2 & GENMASK(size - 1, offset);
		return val ? __ffs(val) : size;
	}

	return _find_next_bit(addr1, addr2, size, offset, 0UL, 0);
}
#endif

#ifndef find_next_zero_bit
>>>>>>> upstream/android-13
/**
 * find_next_zero_bit - find the next cleared bit in a memory region
 * @addr: The address to base the search on
 * @offset: The bitnumber to start searching at
 * @size: The bitmap size in bits
 *
 * Returns the bit number of the next zero bit
 * If no bits are zero, returns @size.
 */
<<<<<<< HEAD
unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size,
				 unsigned long offset);
=======
static inline
unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size,
				 unsigned long offset)
{
	if (small_const_nbits(size)) {
		unsigned long val;

		if (unlikely(offset >= size))
			return size;

		val = *addr | ~GENMASK(size - 1, offset);
		return val == ~0UL ? size : ffz(val);
	}

	return _find_next_bit(addr, NULL, size, offset, ~0UL, 0);
}
>>>>>>> upstream/android-13
#endif

#ifndef find_first_bit

/**
 * find_first_bit - find the first set bit in a memory region
 * @addr: The address to start the search at
 * @size: The maximum number of bits to search
 *
 * Returns the bit number of the first set bit.
 * If no bits are set, returns @size.
 */
<<<<<<< HEAD
extern unsigned long find_first_bit(const unsigned long *addr,
				    unsigned long size);
=======
static inline
unsigned long find_first_bit(const unsigned long *addr, unsigned long size)
{
	if (small_const_nbits(size)) {
		unsigned long val = *addr & GENMASK(size - 1, 0);

		return val ? __ffs(val) : size;
	}

	return _find_first_bit(addr, size);
}
>>>>>>> upstream/android-13

#endif /* find_first_bit */

#ifndef find_first_zero_bit

/**
 * find_first_zero_bit - find the first cleared bit in a memory region
 * @addr: The address to start the search at
 * @size: The maximum number of bits to search
 *
 * Returns the bit number of the first cleared bit.
 * If no bits are zero, returns @size.
 */
<<<<<<< HEAD
unsigned long find_first_zero_bit(const unsigned long *addr, unsigned long size);
=======
static inline
unsigned long find_first_zero_bit(const unsigned long *addr, unsigned long size)
{
	if (small_const_nbits(size)) {
		unsigned long val = *addr | ~GENMASK(size - 1, 0);

		return val == ~0UL ? size : ffz(val);
	}

	return _find_first_zero_bit(addr, size);
}
>>>>>>> upstream/android-13
#endif

#endif /*_TOOLS_LINUX_ASM_GENERIC_BITOPS_FIND_H_ */
