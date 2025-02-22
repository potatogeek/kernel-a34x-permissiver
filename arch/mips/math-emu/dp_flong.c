<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/* IEEE754 floating point arithmetic
 * double precision: common utilities
 */
/*
 * MIPS floating point support
 * Copyright (C) 1994-2000 Algorithmics Ltd.
<<<<<<< HEAD
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
=======
>>>>>>> upstream/android-13
 */

#include "ieee754dp.h"

union ieee754dp ieee754dp_flong(s64 x)
{
	u64 xm;
	int xe;
	int xs;

	ieee754_clearcx();

	if (x == 0)
		return ieee754dp_zero(0);
	if (x == 1 || x == -1)
		return ieee754dp_one(x < 0);
	if (x == 10 || x == -10)
		return ieee754dp_ten(x < 0);

	xs = (x < 0);
	if (xs) {
		if (x == (1ULL << 63))
			xm = (1ULL << 63);	/* max neg can't be safely negated */
		else
			xm = -x;
	} else {
		xm = x;
	}

	/* normalize */
	xe = DP_FBITS + 3;
	if (xm >> (DP_FBITS + 1 + 3)) {
		/* shunt out overflow bits */
		while (xm >> (DP_FBITS + 1 + 3)) {
			XDPSRSX1();
		}
	} else {
		/* normalize in grs extended double precision */
		while ((xm >> (DP_FBITS + 3)) == 0) {
			xm <<= 1;
			xe--;
		}
	}

	return ieee754dp_format(xs, xe, xm);
}
