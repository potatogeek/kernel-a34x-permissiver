<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * arch/arm/include/asm/mach/time.h
 *
 * Copyright (C) 2004 MontaVista Software, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef __ASM_ARM_MACH_TIME_H
#define __ASM_ARM_MACH_TIME_H

<<<<<<< HEAD
extern void timer_tick(void);

=======
>>>>>>> upstream/android-13
typedef void (*clock_access_fn)(struct timespec64 *);
extern int register_persistent_clock(clock_access_fn read_persistent);

#endif
