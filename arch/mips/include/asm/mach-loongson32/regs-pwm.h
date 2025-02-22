<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2014 Zhang, Keguang <keguang.zhang@gmail.com>
 *
 * Loongson 1 PWM Register Definitions.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute	it and/or modify it
 * under  the terms of	the GNU General	 Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __ASM_MACH_LOONGSON32_REGS_PWM_H
#define __ASM_MACH_LOONGSON32_REGS_PWM_H

/* Loongson 1 PWM Timer Register Definitions */
#define PWM_CNT			0x0
#define PWM_HRC			0x4
#define PWM_LRC			0x8
#define PWM_CTRL		0xc

/* PWM Control Register Bits */
#define CNT_RST			BIT(7)
#define INT_SR			BIT(6)
#define INT_EN			BIT(5)
#define PWM_SINGLE		BIT(4)
#define PWM_OE			BIT(3)
#define CNT_EN			BIT(0)

#endif /* __ASM_MACH_LOONGSON32_REGS_PWM_H */
