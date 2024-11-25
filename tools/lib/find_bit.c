<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* bit search implementation
 *
 * Copied from lib/find_bit.c to tools/lib/find_bit.c
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
#include <linux/kernel.h>

#if !defined(find_next_bit) || !defined(find_next_zero_bit) || \
		!defined(find_next_and_bit)

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
	(void) le;
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

	/*
	 * Due to the lack of swab() in tools, and the fact that it doesn't
	 * need little-endian support, just comment it out
	 */
#if (0)
	if (le)
		mask = swab(mask);
#endif

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
=======
#if (0)
	if (le)
		tmp = swab(tmp);
#endif

	return min(start + __ffs(tmp), nbits);
>>>>>>> upstream/android-13
}
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
#endif
<<<<<<< HEAD

#ifndef find_next_zero_bit
unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size,
				 unsigned long offset)
{
	return _find_next_bit(addr, NULL, size, offset, ~0UL);
}
#endif

#ifndef find_next_and_bit
unsigned long find_next_and_bit(const unsigned long *addr1,
		const unsigned long *addr2, unsigned long size,
		unsigned long offset)
{
	return _find_next_bit(addr1, addr2, size, offset, 0UL);
}
#endif
=======
>>>>>>> upstream/android-13
