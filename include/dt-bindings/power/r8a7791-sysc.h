<<<<<<< HEAD
/*
 * Copyright (C) 2016 Glider bvba
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2016 Glider bvba
>>>>>>> upstream/android-13
 */
#ifndef __DT_BINDINGS_POWER_R8A7791_SYSC_H__
#define __DT_BINDINGS_POWER_R8A7791_SYSC_H__

/*
 * These power domain indices match the numbers of the interrupt bits
 * representing the power areas in the various Interrupt Registers
 * (e.g. SYSCISR, Interrupt Status Register)
 */

#define R8A7791_PD_CA15_CPU0		 0
#define R8A7791_PD_CA15_CPU1		 1
#define R8A7791_PD_CA15_SCU		12
#define R8A7791_PD_SH_4A		16
#define R8A7791_PD_SGX			20

/* Always-on power area */
#define R8A7791_PD_ALWAYS_ON		32

#endif /* __DT_BINDINGS_POWER_R8A7791_SYSC_H__ */
