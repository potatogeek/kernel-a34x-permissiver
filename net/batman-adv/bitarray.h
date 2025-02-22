/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2006-2018  B.A.T.M.A.N. contributors:
 *
 * Simon Wunderlich, Marek Lindner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Simon Wunderlich, Marek Lindner
>>>>>>> upstream/android-13
 */

#ifndef _NET_BATMAN_ADV_BITARRAY_H_
#define _NET_BATMAN_ADV_BITARRAY_H_

#include "main.h"

#include <linux/bitops.h>
#include <linux/compiler.h>
#include <linux/stddef.h>
#include <linux/types.h>

/**
 * batadv_test_bit() - check if bit is set in the current window
 *
 * @seq_bits: pointer to the sequence number receive packet
 * @last_seqno: latest sequence number in seq_bits
 * @curr_seqno: sequence number to test for
 *
 * Return: true if the corresponding bit in the given seq_bits indicates true
 * and curr_seqno is within range of last_seqno. Otherwise returns false.
 */
static inline bool batadv_test_bit(const unsigned long *seq_bits,
				   u32 last_seqno, u32 curr_seqno)
{
	s32 diff;

	diff = last_seqno - curr_seqno;
	if (diff < 0 || diff >= BATADV_TQ_LOCAL_WINDOW_SIZE)
		return false;
	return test_bit(diff, seq_bits) != 0;
}

/**
 * batadv_set_bit() - Turn corresponding bit on, so we can remember that we got
 *  the packet
 * @seq_bits: bitmap of the packet receive window
 * @n: relative sequence number of newly received packet
 */
static inline void batadv_set_bit(unsigned long *seq_bits, s32 n)
{
	/* if too old, just drop it */
	if (n < 0 || n >= BATADV_TQ_LOCAL_WINDOW_SIZE)
		return;

	set_bit(n, seq_bits); /* turn the position on */
}

bool batadv_bit_get_packet(void *priv, unsigned long *seq_bits,
			   s32 seq_num_diff, int set_mark);

#endif /* _NET_BATMAN_ADV_BITARRAY_H_ */
