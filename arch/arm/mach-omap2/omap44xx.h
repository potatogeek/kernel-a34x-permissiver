<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*:
 * Address mappings and base address for OMAP4 interconnects
 * and peripherals.
 *
 * Copyright (C) 2009 Texas Instruments
 *
 * Author: Santosh Shilimkar <santosh.shilimkar@ti.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef __ASM_ARCH_OMAP44XX_H
#define __ASM_ARCH_OMAP44XX_H

/*
 * Please place only base defines here and put the rest in device
 * specific headers.
 */
#define L4_44XX_BASE			0x4a000000
#define L4_WK_44XX_BASE			0x4a300000
#define L4_PER_44XX_BASE		0x48000000
#define L4_EMU_44XX_BASE		0x54000000
#define L3_44XX_BASE			0x44000000
#define OMAP44XX_EMIF1_BASE		0x4c000000
#define OMAP44XX_EMIF2_BASE		0x4d000000
#define OMAP44XX_DMM_BASE		0x4e000000
#define OMAP4430_32KSYNCT_BASE		0x4a304000
#define OMAP4430_CM1_BASE		0x4a004000
#define OMAP4430_CM_BASE		OMAP4430_CM1_BASE
#define OMAP4430_CM2_BASE		0x4a008000
#define OMAP4430_PRM_BASE		0x4a306000
#define OMAP4430_PRCM_MPU_BASE		0x48243000
#define OMAP44XX_GPMC_BASE		0x50000000
#define OMAP443X_SCM_BASE		0x4a002000
#define OMAP443X_CTRL_BASE		0x4a100000
#define OMAP44XX_IC_BASE		0x48200000
#define OMAP44XX_IVA_INTC_BASE		0x40000000
#define IRQ_SIR_IRQ			0x0040
#define OMAP44XX_GIC_DIST_BASE		0x48241000
#define OMAP44XX_GIC_CPU_BASE		0x48240100
#define OMAP44XX_IRQ_GIC_START		32
#define OMAP44XX_LOCAL_TWD_BASE		0x48240600
#define OMAP44XX_L2CACHE_BASE		0x48242000
#define OMAP44XX_WKUPGEN_BASE		0x48281000
#define OMAP44XX_MCPDM_BASE		0x40132000
#define OMAP44XX_SAR_RAM_BASE		0x4a326000

#define OMAP44XX_MAILBOX_BASE		(L4_44XX_BASE + 0xF4000)
#define OMAP44XX_HSUSB_OTG_BASE		(L4_44XX_BASE + 0xAB000)

#define OMAP4_MMU1_BASE			0x55082000
#define OMAP4_MMU2_BASE			0x4A066000

#define OMAP44XX_USBTLL_BASE		(L4_44XX_BASE + 0x62000)
#define OMAP44XX_UHH_CONFIG_BASE	(L4_44XX_BASE + 0x64000)
#define OMAP44XX_HSUSB_OHCI_BASE	(L4_44XX_BASE + 0x64800)
#define OMAP44XX_HSUSB_EHCI_BASE	(L4_44XX_BASE + 0x64C00)

#endif /* __ASM_ARCH_OMAP44XX_H */

