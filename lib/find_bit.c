<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* bit search implementation
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * Copyright (C) 2008 IBM Corporation
 * 'find_last_bit' is written by Rusty Russell <rusty@rustcorp.com.au>
 * (Inspired by David Howell's find_next_bit implementation)
 *
 * Rewritten by Yury Norov <yury.norov@gmail.com> to decrease
 * size and improve performance, 2015.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/bitops.h>
#include <linux/bitmap.h>
#include <linux/export.h>
<<<<<<< HEAD
#include <linux/kernel.h>

#if !defined(find_next_bit) || !defined(find_next_zero_bit) || \
		!defined(find_next_and_bit)

=======
#include <linux/math.h>
#include <linux/minmax.h>
#include <linux/swab.h>

#if !defined(find_next_bit) || !defined(find_next_zero_bit) ||			\
	!defined(find_next_bit_le) || !defined(find_next_zero_bit_le) ||	\
	!defined(find_next_and_bit)
>>>>>>> upstream/android-13
/*
 * This is a common helper function for find_next_bit, find_next_zero_bit, and
 * find_next_and_bit. The differences are:
 *  - The "invert" argument, which is XORed with each fetched word before
 *    searching it for one bits.
 *  - The optional "addr2", which is anded with "addr1" if present.
 */
<<<<<<< HEAD
static inline unsigned long _find_next_bit(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long nbits,
		unsigned long start, unsigned long invert)
{
	unsigned long tmp;
=======
unsigned long _find_next_bit(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long nbits,
		unsigned long start, unsigned long invert, unsigned long le)
{
	unsigned long tmp, mask;
>>>>>>> upstream/android-13

	if (unlikely(start >= nbits))
		return nbits;

	tmp = addr1[start / BITS_PER_LONG];
	if (addr2)
		tmp &= addr2[start / BITS_PER_LONG];
	tmp ^= invert;

	/* Handle 1st word. */
<<<<<<< HEAD
	tmp &= BITMAP_FIRST_WORD_MASK(start);
=======
	mask = BITMAP_FIRST_WORD_MASK(start);
	if (le)
		mask = swab(mask);

	tmp &= mask;

>>>>>>> upstream/android-13
	start = round_down(start, BITS_PER_LONG);

	while (!tmp) {
		start += BITS_PER_LONG;
		if (start >= nbits)
			return nbits;

		tmp = addr1[start / BITS_PER_LONG];
		if (addr2)
			tmp &= addr2[start / BITS_PER_LONG];
		tmp ^= invert;
	}

<<<<<<< HEAD
	return min(start + __ffs(tmp), nbits);
}
#endif

#ifndef find_next_bit
/*
 * Find the next set bit in a memory region.
 */
unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
			    unsigned long offset)
{
	return _find_next_bit(addr, NULL, size, offset, 0UL);
}
EXPORT_SYMBOL(find_next_bit);
#endif

#ifndef find_next_zero_bit
unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size,
				 unsigned long offset)
{
	return _find_next_bit(addr, NULL, size, offset, ~0UL);
}
EXPORT_SYMBOL(find_next_zero_bit);
#endif

#if !defined(find_next_and_bit)
unsigned long find_next_and_bit(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long size,
		unsigned long offset)
{
	return _find_next_bit(addr1, addr2, size, offset, 0UL);
}
EXPORT_SYMBOL(find_next_and_bit);
=======
	if (le)
		tmp = swab(tmp);

	return min(start + __ffs(tmp), nbits);
}
EXPORT_SYMBOL(_find_next_bit);
>>>>>>> upstream/android-13
#endif

#ifndef find_first_bit
/*
 * Find the first set bit in a memory region.
 */
<<<<<<< HEAD
unsigned long find_first_bit(const unsigned long *addr, unsigned long size)
=======
unsigned long _find_first_bit(const unsigned long *addr, unsigned long size)
>>>>>>> upstream/android-13
{
	unsigned long idx;

	for (idx = 0; idx * BITS_PER_LONG < size; idx++) {
		if (addr[idx])
			return min(idx * BITS_PER_LONG + __ffs(addr[idx]), size);
	}

	return size;
}
<<<<<<< HEAD
EXPORT_SYMBOL(find_first_bit);
=======
EXPORT_SYMBOL(_find_first_bit);
>>>>>>> upstream/android-13
#endif

#ifndef find_first_zero_bit
/*
 * Find the first cleared bit in a memory region.
 */
<<<<<<< HEAD
unsigned long find_first_zero_bit(const unsigned long *addr, unsigned long size)
=======
unsigned long _find_first_zero_bit(const unsigned long *addr, unsigned long size)
>>>>>>> upstream/android-13
{
	unsigned long idx;

	for (idx = 0; idx * BITS_PER_LONG < size; idx++) {
		if (addr[idx] != ~0UL)
			return min(idx * BITS_PER_LONG + ffz(addr[idx]), size);
	}

	return size;
}
<<<<<<< HEAD
EXPORT_SYMBOL(find_first_zero_bit);
#endif

#ifndef find_last_bit
unsigned long find_last_bit(const unsigned long *addr, unsigned long size)
=======
EXPORT_SYMBOL(_find_first_zero_bit);
#endif

#ifndef find_last_bit
unsigned long _find_last_bit(const unsigned long *addr, unsigned long size)
>>>>>>> upstream/android-13
{
	if (size) {
		unsigned long val = BITMAP_LAST_WORD_MASK(size);
		unsigned long idx = (size-1) / BITS_PER_LONG;

		do {
			val &= addr[idx];
			if (val)
				return idx * BITS_PER_LONG + __fls(val);

			val = ~0ul;
		} while (idx--);
	}
	return size;
}
<<<<<<< HEAD
EXPORT_SYMBOL(find_last_bit);
#endif

#ifdef __BIG_ENDIAN

#if !defined(find_next_bit_le) || !defined(find_next_zero_bit_le)
static inline unsigned long _find_next_bit_le(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long nbits,
		unsigned long start, unsigned long invert)
{
	unsigned long tmp;

	if (unlikely(start >= nbits))
		return nbits;

	tmp = addr1[start / BITS_PER_LONG];
	if (addr2)
		tmp &= addr2[start / BITS_PER_LONG];
	tmp ^= invert;

	/* Handle 1st word. */
	tmp &= swab(BITMAP_FIRST_WORD_MASK(start));
	start = round_down(start, BITS_PER_LONG);

	while (!tmp) {
		start += BITS_PER_LONG;
		if (start >= nbits)
			return nbits;

		tmp = addr1[start / BITS_PER_LONG];
		if (addr2)
			tmp &= addr2[start / BITS_PER_LONG];
		tmp ^= invert;
	}

	return min(start + __ffs(swab(tmp)), nbits);
}
#endif

#ifndef find_next_zero_bit_le
unsigned long find_next_zero_bit_le(const void *addr, unsigned
		long size, unsigned long offset)
{
	return _find_next_bit_le(addr, NULL, size, offset, ~0UL);
}
EXPORT_SYMBOL(find_next_zero_bit_le);
#endif

#ifndef find_next_bit_le
unsigned long find_next_bit_le(const void *addr, unsigned
		long size, unsigned long offset)
{
	return _find_next_bit_le(addr, NULL, size, offset, 0UL);
}
EXPORT_SYMBOL(find_next_bit_le);
#endif

#endif /* __BIG_ENDIAN */
=======
EXPORT_SYMBOL(_find_last_bit);
#endif

unsigned long find_next_clump8(unsigned long *clump, const unsigned long *addr,
			       unsigned long size, unsigned long offset)
{
	offset = find_next_bit(addr, size, offset);
	if (offset == size)
		return size;

	offset = round_down(offset, 8);
	*clump = bitmap_get_value8(addr, offset);

	return offset;
}
EXPORT_SYMBOL(find_next_clump8);
>>>>>>> upstream/android-13
