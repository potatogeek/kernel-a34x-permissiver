<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
>>>>>>> upstream/android-13
 *
 *  Copyright (C) 2010 Thomas Langer <thomas.langer@lantiq.com>
 */

#ifndef _FALCON_IRQ__
#define _FALCON_IRQ__

#define INT_NUM_IRQ0			8
#define INT_NUM_IM0_IRL0		(INT_NUM_IRQ0 + 0)
#define INT_NUM_IM1_IRL0		(INT_NUM_IM0_IRL0 + 32)
#define INT_NUM_IM2_IRL0		(INT_NUM_IM1_IRL0 + 32)
#define INT_NUM_IM3_IRL0		(INT_NUM_IM2_IRL0 + 32)
#define INT_NUM_IM4_IRL0		(INT_NUM_IM3_IRL0 + 32)
#define INT_NUM_EXTRA_START		(INT_NUM_IM4_IRL0 + 32)
#define INT_NUM_IM_OFFSET		(INT_NUM_IM1_IRL0 - INT_NUM_IM0_IRL0)

<<<<<<< HEAD
#define MIPS_CPU_TIMER_IRQ			7

=======
>>>>>>> upstream/android-13
#define MAX_IM			5

#endif /* _FALCON_IRQ__ */
