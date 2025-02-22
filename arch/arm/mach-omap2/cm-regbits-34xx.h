<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
#ifndef __ARCH_ARM_MACH_OMAP2_CM_REGBITS_34XX_H
#define __ARCH_ARM_MACH_OMAP2_CM_REGBITS_34XX_H

/*
 * OMAP3430 Clock Management register bits
 *
 * Copyright (C) 2007-2008 Texas Instruments, Inc.
 * Copyright (C) 2007-2008 Nokia Corporation
 *
 * Written by Paul Walmsley
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#define OMAP3430_CM_FCLKEN_IVA2_EN_IVA2_MASK		(1 << 0)
#define OMAP3430_ST_IVA2_SHIFT				0
#define OMAP3430_CLKTRCTRL_IVA2_MASK			(0x3 << 0)
#define OMAP3430_CLKACTIVITY_IVA2_MASK			(1 << 0)
#define OMAP3430_CLKTRCTRL_MPU_MASK			(0x3 << 0)
#define OMAP3430_ST_AES2_SHIFT				28
#define OMAP3430_ST_SHA12_SHIFT				27
#define AM35XX_ST_UART4_SHIFT				23
#define OMAP3430_ST_HDQ_SHIFT				22
#define OMAP3430ES2_ST_SSI_IDLE_SHIFT			8
#define OMAP3430_ST_MAILBOXES_SHIFT			7
#define OMAP3430_ST_SAD2D_SHIFT				3
#define OMAP3430_ST_SDMA_SHIFT				2
#define OMAP3430ES2_ST_USBTLL_SHIFT			2
#define OMAP3430ES1_CLKTRCTRL_D2D_MASK			(0x3 << 4)
#define OMAP3430_CLKTRCTRL_L4_MASK			(0x3 << 2)
#define OMAP3430_CLKTRCTRL_L3_MASK			(0x3 << 0)
#define OMAP3430ES1_CLKTRCTRL_GFX_MASK			(0x3 << 0)
#define OMAP3430ES2_CLKTRCTRL_SGX_MASK			(0x3 << 0)
#define OMAP3430_ST_WDT2_SHIFT				5
#define OMAP3430_ST_32KSYNC_SHIFT			2
#define OMAP3430_AUTO_PERIPH_DPLL_MASK			(0x7 << 3)
#define OMAP3430ES2_ST_DSS_IDLE_SHIFT			1
#define OMAP3430_CLKTRCTRL_DSS_MASK			(0x3 << 0)
#define OMAP3430_CLKTRCTRL_CAM_MASK			(0x3 << 0)
#define OMAP3430_ST_MCBSP4_SHIFT			2
#define OMAP3430_ST_MCBSP3_SHIFT			1
#define OMAP3430_ST_MCBSP2_SHIFT			0
#define OMAP3430_CLKTRCTRL_PER_MASK			(0x3 << 0)
#define OMAP3430_CLKTRCTRL_EMU_MASK			(0x3 << 0)
#define OMAP3430_CLKTRCTRL_NEON_MASK			(0x3 << 0)
#define OMAP3430ES2_EN_USBHOST2_SHIFT			1
#define OMAP3430ES2_ST_USBHOST_IDLE_SHIFT		1
#define OMAP3430ES2_CLKTRCTRL_USBHOST_MASK		(3 << 0)
#define OMAP34XX_CLKSTCTRL_DISABLE_AUTO		0x0
#define OMAP34XX_CLKSTCTRL_FORCE_SLEEP		0x1
#define OMAP34XX_CLKSTCTRL_FORCE_WAKEUP		0x2
#define OMAP34XX_CLKSTCTRL_ENABLE_AUTO		0x3
#endif
