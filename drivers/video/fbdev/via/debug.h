<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.

<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
=======
>>>>>>> upstream/android-13
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__

<<<<<<< HEAD
=======
#include <linux/printk.h>

>>>>>>> upstream/android-13
#ifndef VIAFB_DEBUG
#define VIAFB_DEBUG 0
#endif

#if VIAFB_DEBUG
#define DEBUG_MSG(f, a...)   printk(f, ## a)
#else
<<<<<<< HEAD
#define DEBUG_MSG(f, a...)
=======
#define DEBUG_MSG(f, a...)   no_printk(f, ## a)
>>>>>>> upstream/android-13
#endif

#define VIAFB_WARN 0
#if VIAFB_WARN
#define WARN_MSG(f, a...)   printk(f, ## a)
#else
<<<<<<< HEAD
#define WARN_MSG(f, a...)
=======
#define WARN_MSG(f, a...)   no_printk(f, ## a)
>>>>>>> upstream/android-13
#endif

#endif /* __DEBUG_H__ */
