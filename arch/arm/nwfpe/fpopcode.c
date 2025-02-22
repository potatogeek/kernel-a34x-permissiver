<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
    NetWinder Floating Point Emulator
    (c) Rebel.COM, 1998,1999

    Direct questions, comments to Scott Bambrough <scottb@netwinder.org>

<<<<<<< HEAD
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13
*/

#include "fpa11.h"
#include "softfloat.h"
#include "fpopcode.h"
#include "fpsr.h"
#include "fpmodule.h"
#include "fpmodule.inl"

#ifdef CONFIG_FPE_NWFPE_XP
const floatx80 floatx80Constant[] = {
	{ .high = 0x0000, .low = 0x0000000000000000ULL},/* extended 0.0 */
	{ .high = 0x3fff, .low = 0x8000000000000000ULL},/* extended 1.0 */
	{ .high = 0x4000, .low = 0x8000000000000000ULL},/* extended 2.0 */
	{ .high = 0x4000, .low = 0xc000000000000000ULL},/* extended 3.0 */
	{ .high = 0x4001, .low = 0x8000000000000000ULL},/* extended 4.0 */
	{ .high = 0x4001, .low = 0xa000000000000000ULL},/* extended 5.0 */
	{ .high = 0x3ffe, .low = 0x8000000000000000ULL},/* extended 0.5 */
	{ .high = 0x4002, .low = 0xa000000000000000ULL},/* extended 10.0 */
};
#endif

const float64 float64Constant[] = {
	0x0000000000000000ULL,	/* double 0.0 */
	0x3ff0000000000000ULL,	/* double 1.0 */
	0x4000000000000000ULL,	/* double 2.0 */
	0x4008000000000000ULL,	/* double 3.0 */
	0x4010000000000000ULL,	/* double 4.0 */
	0x4014000000000000ULL,	/* double 5.0 */
	0x3fe0000000000000ULL,	/* double 0.5 */
	0x4024000000000000ULL	/* double 10.0 */
};

const float32 float32Constant[] = {
	0x00000000,		/* single 0.0 */
	0x3f800000,		/* single 1.0 */
	0x40000000,		/* single 2.0 */
	0x40400000,		/* single 3.0 */
	0x40800000,		/* single 4.0 */
	0x40a00000,		/* single 5.0 */
	0x3f000000,		/* single 0.5 */
	0x41200000		/* single 10.0 */
};

