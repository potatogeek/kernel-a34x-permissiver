<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/* IEEE754 floating point arithmetic
 * single precision
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

#include "ieee754sp.h"
#include "ieee754dp.h"

static inline union ieee754sp ieee754sp_nan_fdp(int xs, u64 xm)
{
	return buildsp(xs, SP_EMAX + 1 + SP_EBIAS,
		       xm >> (DP_FBITS - SP_FBITS));
}

union ieee754sp ieee754sp_fdp(union ieee754dp x)
{
	union ieee754sp y;
	u32 rm;

	COMPXDP;
	COMPYSP;

	EXPLODEXDP;

	ieee754_clearcx();

	FLUSHXDP;

	switch (xc) {
	case IEEE754_CLASS_SNAN:
		x = ieee754dp_nanxcpt(x);
		EXPLODEXDP;
<<<<<<< HEAD
		/* fall through */

=======
		fallthrough;
>>>>>>> upstream/android-13
	case IEEE754_CLASS_QNAN:
		y = ieee754sp_nan_fdp(xs, xm);
		if (!ieee754_csr.nan2008) {
			EXPLODEYSP;
			if (!ieee754_class_nan(yc))
				y = ieee754sp_indef();
		}
		return y;

	case IEEE754_CLASS_INF:
		return ieee754sp_inf(xs);

	case IEEE754_CLASS_ZERO:
		return ieee754sp_zero(xs);

	case IEEE754_CLASS_DNORM:
		/* can't possibly be sp representable */
		ieee754_setcx(IEEE754_UNDERFLOW);
		ieee754_setcx(IEEE754_INEXACT);
		if ((ieee754_csr.rm == FPU_CSR_RU && !xs) ||
				(ieee754_csr.rm == FPU_CSR_RD && xs))
			return ieee754sp_mind(xs);
		return ieee754sp_zero(xs);

	case IEEE754_CLASS_NORM:
		break;
	}

	/*
	 * Convert from DP_FBITS to SP_FBITS+3 with sticky right shift.
	 */
	rm = (xm >> (DP_FBITS - (SP_FBITS + 3))) |
	     ((xm << (64 - (DP_FBITS - (SP_FBITS + 3)))) != 0);

	return ieee754sp_format(xs, xe, rm);
}
