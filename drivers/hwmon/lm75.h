<<<<<<< HEAD
/*
    lm75.h - Part of lm_sensors, Linux kernel modules for hardware
	      monitoring
    Copyright (c) 2003 Mark M. Hoffman <mhoffman@lightlink.com>

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
*/

/*
    This file contains common code for encoding/decoding LM75 type
    temperature readings, which are emulated by many of the chips
    we support.  As the user is unlikely to load more than one driver
    which contains this code, we don't worry about the wasted space.
*/
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * lm75.h - Part of lm_sensors, Linux kernel modules for hardware monitoring
 * Copyright (c) 2003 Mark M. Hoffman <mhoffman@lightlink.com>
 */

/*
 * This file contains common code for encoding/decoding LM75 type
 * temperature readings, which are emulated by many of the chips
 * we support.  As the user is unlikely to load more than one driver
 * which contains this code, we don't worry about the wasted space.
 */
>>>>>>> upstream/android-13

#include <linux/kernel.h>

/* straight from the datasheet */
#define LM75_TEMP_MIN (-55000)
#define LM75_TEMP_MAX 125000
#define LM75_SHUTDOWN 0x01

<<<<<<< HEAD
/* TEMP: 0.001C/bit (-55C to +125C)
   REG: (0.5C/bit, two's complement) << 7 */
static inline u16 LM75_TEMP_TO_REG(long temp)
{
	int ntemp = clamp_val(temp, LM75_TEMP_MIN, LM75_TEMP_MAX);
=======
/*
 * TEMP: 0.001C/bit (-55C to +125C)
 * REG: (0.5C/bit, two's complement) << 7
 */
static inline u16 LM75_TEMP_TO_REG(long temp)
{
	int ntemp = clamp_val(temp, LM75_TEMP_MIN, LM75_TEMP_MAX);

>>>>>>> upstream/android-13
	ntemp += (ntemp < 0 ? -250 : 250);
	return (u16)((ntemp / 500) << 7);
}

static inline int LM75_TEMP_FROM_REG(u16 reg)
{
<<<<<<< HEAD
	/* use integer division instead of equivalent right shift to
	   guarantee arithmetic shift and preserve the sign */
=======
	/*
	 * use integer division instead of equivalent right shift to
	 * guarantee arithmetic shift and preserve the sign
	 */
>>>>>>> upstream/android-13
	return ((s16)reg / 128) * 500;
}
