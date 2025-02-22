<<<<<<< HEAD
/*
 * Copyright (C) 2017 Cogent Embedded Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2017 Cogent Embedded Inc.
>>>>>>> upstream/android-13
 */
#ifndef __DT_BINDINGS_POWER_R8A77970_SYSC_H__
#define __DT_BINDINGS_POWER_R8A77970_SYSC_H__

/*
 * These power domain indices match the numbers of the interrupt bits
 * representing the power areas in the various Interrupt Registers
 * (e.g. SYSCISR, Interrupt Status Register)
 */

#define R8A77970_PD_CA53_CPU0		 5
#define R8A77970_PD_CA53_CPU1		 6
<<<<<<< HEAD
#define R8A77970_PD_CR7			13
=======
>>>>>>> upstream/android-13
#define R8A77970_PD_CA53_SCU		21
#define R8A77970_PD_A2IR0		23
#define R8A77970_PD_A3IR		24
#define R8A77970_PD_A2IR1		27
#define R8A77970_PD_A2DP		28
#define R8A77970_PD_A2CN		29
#define R8A77970_PD_A2SC0		30
#define R8A77970_PD_A2SC1		31

/* Always-on power area */
#define R8A77970_PD_ALWAYS_ON		32

#endif /* __DT_BINDINGS_POWER_R8A77970_SYSC_H__ */
