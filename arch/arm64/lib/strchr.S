<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/lib/strchr.S
 *
 * Copyright (C) 1995-2000 Russell King
 * Copyright (C) 2013 ARM Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#include <linux/linkage.h>
#include <asm/assembler.h>

/*
 * Find the first occurrence of a character in a string.
 *
 * Parameters:
 *	x0 - str
 *	x1 - c
 * Returns:
 *	x0 - address of first occurrence of 'c' or 0
 */
<<<<<<< HEAD
WEAK(strchr)
=======
SYM_FUNC_START_WEAK(strchr)
>>>>>>> upstream/android-13
	and	w1, w1, #0xff
1:	ldrb	w2, [x0], #1
	cmp	w2, w1
	ccmp	w2, wzr, #4, ne
	b.ne	1b
	sub	x0, x0, #1
	cmp	w2, w1
	csel	x0, x0, xzr, eq
	ret
<<<<<<< HEAD
ENDPROC(strchr)
=======
SYM_FUNC_END(strchr)
EXPORT_SYMBOL_NOKASAN(strchr)
>>>>>>> upstream/android-13
