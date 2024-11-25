<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2016 Yang Ling <gnaygnil@gmail.com>
 *
 * Loongson 1 RTC timer Register Definitions.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __ASM_MACH_LOONGSON32_REGS_RTC_H
#define __ASM_MACH_LOONGSON32_REGS_RTC_H

#define LS1X_RTC_REG(x) \
		((void __iomem *)KSEG1ADDR(LS1X_RTC_BASE + (x)))

#define LS1X_RTC_CTRL	LS1X_RTC_REG(0x40)

#define RTC_EXTCLK_OK	(BIT(5) | BIT(8))
#define RTC_EXTCLK_EN	BIT(8)

#endif /* __ASM_MACH_LOONGSON32_REGS_RTC_H */
