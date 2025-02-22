<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * omap4-sar-layout.h: OMAP4 SAR RAM layout header file
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 *	Santosh Shilimkar <santosh.shilimkar@ti.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef OMAP_ARCH_OMAP4_SAR_LAYOUT_H
#define OMAP_ARCH_OMAP4_SAR_LAYOUT_H

/*
 * SAR BANK offsets from base address OMAP44XX/54XX_SAR_RAM_BASE
 */
#define SAR_BANK1_OFFSET		0x0000
#define SAR_BANK2_OFFSET		0x1000
#define SAR_BANK3_OFFSET		0x2000
#define SAR_BANK4_OFFSET		0x3000

/* Scratch pad memory offsets from SAR_BANK1 */
#define SCU_OFFSET0				0xfe4
#define SCU_OFFSET1				0xfe8
#define OMAP_TYPE_OFFSET			0xfec
#define L2X0_SAVE_OFFSET0			0xff0
#define L2X0_SAVE_OFFSET1			0xff4
#define L2X0_AUXCTRL_OFFSET			0xff8
#define L2X0_PREFETCH_CTRL_OFFSET		0xffc

/* CPUx Wakeup Non-Secure Physical Address offsets in SAR_BANK1 */
#define CPU0_WAKEUP_NS_PA_ADDR_OFFSET		0xa04
#define CPU1_WAKEUP_NS_PA_ADDR_OFFSET		0xa08
#define OMAP5_CPU0_WAKEUP_NS_PA_ADDR_OFFSET	0xe00
#define OMAP5_CPU1_WAKEUP_NS_PA_ADDR_OFFSET	0xe04

#define SAR_BACKUP_STATUS_OFFSET		(SAR_BANK3_OFFSET + 0x500)
#define SAR_SECURE_RAM_SIZE_OFFSET		(SAR_BANK3_OFFSET + 0x504)
#define SAR_SECRAM_SAVED_AT_OFFSET		(SAR_BANK3_OFFSET + 0x508)

/* WakeUpGen save restore offset from OMAP44XX_SAR_RAM_BASE */
#define WAKEUPGENENB_OFFSET_CPU0		(SAR_BANK3_OFFSET + 0x684)
#define WAKEUPGENENB_SECURE_OFFSET_CPU0		(SAR_BANK3_OFFSET + 0x694)
#define WAKEUPGENENB_OFFSET_CPU1		(SAR_BANK3_OFFSET + 0x6a4)
#define WAKEUPGENENB_SECURE_OFFSET_CPU1		(SAR_BANK3_OFFSET + 0x6b4)
#define AUXCOREBOOT0_OFFSET			(SAR_BANK3_OFFSET + 0x6c4)
#define AUXCOREBOOT1_OFFSET			(SAR_BANK3_OFFSET + 0x6c8)
#define PTMSYNCREQ_MASK_OFFSET			(SAR_BANK3_OFFSET + 0x6cc)
#define PTMSYNCREQ_EN_OFFSET			(SAR_BANK3_OFFSET + 0x6d0)
#define SAR_BACKUP_STATUS_WAKEUPGEN		0x10

/* WakeUpGen save restore offset from OMAP54XX_SAR_RAM_BASE */
#define OMAP5_WAKEUPGENENB_OFFSET_CPU0		(SAR_BANK3_OFFSET + 0x9dc)
#define OMAP5_WAKEUPGENENB_SECURE_OFFSET_CPU0	(SAR_BANK3_OFFSET + 0x9f0)
#define OMAP5_WAKEUPGENENB_OFFSET_CPU1		(SAR_BANK3_OFFSET + 0xa04)
#define OMAP5_WAKEUPGENENB_SECURE_OFFSET_CPU1	(SAR_BANK3_OFFSET + 0xa18)
#define OMAP5_AUXCOREBOOT0_OFFSET		(SAR_BANK3_OFFSET + 0xa2c)
#define OMAP5_AUXCOREBOOT1_OFFSET		(SAR_BANK3_OFFSET + 0x930)
#define OMAP5_AMBA_IF_MODE_OFFSET		(SAR_BANK3_OFFSET + 0xa34)
#define OMAP5_SAR_BACKUP_STATUS_OFFSET		(SAR_BANK3_OFFSET + 0x800)

#endif
