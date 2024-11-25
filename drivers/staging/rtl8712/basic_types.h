<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/******************************************************************************
 *
 * Copyright(c) 2007 - 2010 Realtek Corporation. All rights reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
=======
>>>>>>> upstream/android-13
 * Modifications for inclusion into the Linux staging tree are
 * Copyright(c) 2010 Larry Finger. All rights reserved.
 *
 * Contact information:
 * WLAN FAE <wlanfae@realtek.com>
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 ******************************************************************************/
#ifndef __BASIC_TYPES_H__
#define __BASIC_TYPES_H__

<<<<<<< HEAD
#define SUCCESS	0
#define FAIL	(-1)

#include <linux/types.h>

#define SIZE_T __kernel_size_t
#define sint signed int
#define FIELD_OFFSET(s, field)	((addr_t)&((s *)(0))->field)
=======
#include <linux/types.h>

#define sint signed int
>>>>>>> upstream/android-13

/* Should we extend this to be host_addr_t and target_addr_t for case:
 *	host : x86_64
 *	target : mips64
 */
#define addr_t unsigned long

<<<<<<< HEAD
#define MEM_ALIGNMENT_OFFSET	(sizeof(SIZE_T))
#define MEM_ALIGNMENT_PADDING	(sizeof(SIZE_T) - 1)

=======
>>>>>>> upstream/android-13
#endif /*__BASIC_TYPES_H__*/

