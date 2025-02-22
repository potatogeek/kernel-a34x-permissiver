<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Freescale GPMI NAND Flash Driver
 *
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
 * Copyright 2008 Embedded Alley Solutions, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
=======
>>>>>>> upstream/android-13
 */
#ifndef __GPMI_NAND_BCH_REGS_H
#define __GPMI_NAND_BCH_REGS_H

#define HW_BCH_CTRL				0x00000000
#define HW_BCH_CTRL_SET				0x00000004
#define HW_BCH_CTRL_CLR				0x00000008
#define HW_BCH_CTRL_TOG				0x0000000c

#define BM_BCH_CTRL_COMPLETE_IRQ_EN		(1 << 8)
#define BM_BCH_CTRL_COMPLETE_IRQ		(1 << 0)

#define HW_BCH_STATUS0				0x00000010
#define HW_BCH_MODE				0x00000020
#define HW_BCH_ENCODEPTR			0x00000030
#define HW_BCH_DATAPTR				0x00000040
#define HW_BCH_METAPTR				0x00000050
#define HW_BCH_LAYOUTSELECT			0x00000070

#define HW_BCH_FLASH0LAYOUT0			0x00000080

#define BP_BCH_FLASH0LAYOUT0_NBLOCKS		24
#define BM_BCH_FLASH0LAYOUT0_NBLOCKS	(0xff << BP_BCH_FLASH0LAYOUT0_NBLOCKS)
#define BF_BCH_FLASH0LAYOUT0_NBLOCKS(v)		\
	(((v) << BP_BCH_FLASH0LAYOUT0_NBLOCKS) & BM_BCH_FLASH0LAYOUT0_NBLOCKS)

#define BP_BCH_FLASH0LAYOUT0_META_SIZE		16
#define BM_BCH_FLASH0LAYOUT0_META_SIZE	(0xff << BP_BCH_FLASH0LAYOUT0_META_SIZE)
#define BF_BCH_FLASH0LAYOUT0_META_SIZE(v)	\
	(((v) << BP_BCH_FLASH0LAYOUT0_META_SIZE)\
					 & BM_BCH_FLASH0LAYOUT0_META_SIZE)

#define BP_BCH_FLASH0LAYOUT0_ECC0		12
#define BM_BCH_FLASH0LAYOUT0_ECC0	(0xf << BP_BCH_FLASH0LAYOUT0_ECC0)
#define MX6Q_BP_BCH_FLASH0LAYOUT0_ECC0		11
#define MX6Q_BM_BCH_FLASH0LAYOUT0_ECC0	(0x1f << MX6Q_BP_BCH_FLASH0LAYOUT0_ECC0)
#define BF_BCH_FLASH0LAYOUT0_ECC0(v, x)				\
	(GPMI_IS_MX6(x)					\
		? (((v) << MX6Q_BP_BCH_FLASH0LAYOUT0_ECC0)	\
			& MX6Q_BM_BCH_FLASH0LAYOUT0_ECC0)	\
		: (((v) << BP_BCH_FLASH0LAYOUT0_ECC0)		\
			& BM_BCH_FLASH0LAYOUT0_ECC0)		\
	)

#define MX6Q_BP_BCH_FLASH0LAYOUT0_GF_13_14	10
#define MX6Q_BM_BCH_FLASH0LAYOUT0_GF_13_14			\
				(0x1 << MX6Q_BP_BCH_FLASH0LAYOUT0_GF_13_14)
#define BF_BCH_FLASH0LAYOUT0_GF(v, x)				\
	((GPMI_IS_MX6(x) && ((v) == 14))			\
		? (((1) << MX6Q_BP_BCH_FLASH0LAYOUT0_GF_13_14)	\
			& MX6Q_BM_BCH_FLASH0LAYOUT0_GF_13_14)	\
		: 0						\
	)

#define BP_BCH_FLASH0LAYOUT0_DATA0_SIZE		0
#define BM_BCH_FLASH0LAYOUT0_DATA0_SIZE		\
			(0xfff << BP_BCH_FLASH0LAYOUT0_DATA0_SIZE)
#define MX6Q_BM_BCH_FLASH0LAYOUT0_DATA0_SIZE	\
			(0x3ff << BP_BCH_FLASH0LAYOUT0_DATA0_SIZE)
#define BF_BCH_FLASH0LAYOUT0_DATA0_SIZE(v, x)				\
	(GPMI_IS_MX6(x)						\
		? (((v) >> 2) & MX6Q_BM_BCH_FLASH0LAYOUT0_DATA0_SIZE)	\
		: ((v) & BM_BCH_FLASH0LAYOUT0_DATA0_SIZE)		\
	)

#define HW_BCH_FLASH0LAYOUT1			0x00000090

#define BP_BCH_FLASH0LAYOUT1_PAGE_SIZE		16
#define BM_BCH_FLASH0LAYOUT1_PAGE_SIZE		\
			(0xffff << BP_BCH_FLASH0LAYOUT1_PAGE_SIZE)
#define BF_BCH_FLASH0LAYOUT1_PAGE_SIZE(v)	\
	(((v) << BP_BCH_FLASH0LAYOUT1_PAGE_SIZE) \
					 & BM_BCH_FLASH0LAYOUT1_PAGE_SIZE)

#define BP_BCH_FLASH0LAYOUT1_ECCN		12
#define BM_BCH_FLASH0LAYOUT1_ECCN	(0xf << BP_BCH_FLASH0LAYOUT1_ECCN)
#define MX6Q_BP_BCH_FLASH0LAYOUT1_ECCN		11
#define MX6Q_BM_BCH_FLASH0LAYOUT1_ECCN	(0x1f << MX6Q_BP_BCH_FLASH0LAYOUT1_ECCN)
#define BF_BCH_FLASH0LAYOUT1_ECCN(v, x)				\
	(GPMI_IS_MX6(x)					\
		? (((v) << MX6Q_BP_BCH_FLASH0LAYOUT1_ECCN)	\
			& MX6Q_BM_BCH_FLASH0LAYOUT1_ECCN)	\
		: (((v) << BP_BCH_FLASH0LAYOUT1_ECCN)		\
			& BM_BCH_FLASH0LAYOUT1_ECCN)		\
	)

#define MX6Q_BP_BCH_FLASH0LAYOUT1_GF_13_14	10
#define MX6Q_BM_BCH_FLASH0LAYOUT1_GF_13_14			\
				(0x1 << MX6Q_BP_BCH_FLASH0LAYOUT1_GF_13_14)
#define BF_BCH_FLASH0LAYOUT1_GF(v, x)				\
	((GPMI_IS_MX6(x) && ((v) == 14))			\
		? (((1) << MX6Q_BP_BCH_FLASH0LAYOUT1_GF_13_14)	\
			& MX6Q_BM_BCH_FLASH0LAYOUT1_GF_13_14)	\
		: 0						\
	)

#define BP_BCH_FLASH0LAYOUT1_DATAN_SIZE		0
#define BM_BCH_FLASH0LAYOUT1_DATAN_SIZE		\
			(0xfff << BP_BCH_FLASH0LAYOUT1_DATAN_SIZE)
#define MX6Q_BM_BCH_FLASH0LAYOUT1_DATAN_SIZE	\
			(0x3ff << BP_BCH_FLASH0LAYOUT1_DATAN_SIZE)
#define BF_BCH_FLASH0LAYOUT1_DATAN_SIZE(v, x)				\
	(GPMI_IS_MX6(x)						\
		? (((v) >> 2) & MX6Q_BM_BCH_FLASH0LAYOUT1_DATAN_SIZE)	\
		: ((v) & BM_BCH_FLASH0LAYOUT1_DATAN_SIZE)		\
	)

#define HW_BCH_VERSION				0x00000160
#endif
