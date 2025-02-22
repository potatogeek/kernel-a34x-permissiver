<<<<<<< HEAD
/*
 * Copyright 2016 Linaro Ltd.
 * Copyright 2016 ZTE Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2016 Linaro Ltd.
 * Copyright 2016 ZTE Corporation.
>>>>>>> upstream/android-13
 */

#ifndef __ZX_VOU_REGS_H__
#define __ZX_VOU_REGS_H__

/* Sub-module offset */
#define MAIN_GL_OFFSET			0x130
#define MAIN_GL_CSC_OFFSET		0x580
#define MAIN_CHN_CSC_OFFSET		0x6c0
#define MAIN_HBSC_OFFSET		0x820
#define MAIN_DITHER_OFFSET		0x960
#define MAIN_RSZ_OFFSET			0x600 /* OTFPPU sub-module */

#define AUX_GL_OFFSET			0x200
#define AUX_GL_CSC_OFFSET		0x5d0
#define AUX_CHN_CSC_OFFSET		0x710
#define AUX_HBSC_OFFSET			0x860
#define AUX_DITHER_OFFSET		0x970
#define AUX_RSZ_OFFSET			0x800

#define OSD_VL0_OFFSET			0x040
#define OSD_VL_OFFSET(i)		(OSD_VL0_OFFSET + 0x050 * (i))

#define HBSC_VL0_OFFSET			0x760
#define HBSC_VL_OFFSET(i)		(HBSC_VL0_OFFSET + 0x040 * (i))

#define RSZ_VL1_U0			0xa00
#define RSZ_VL_OFFSET(i)		(RSZ_VL1_U0 + 0x200 * (i))

/* OSD (GPC_GLOBAL) registers */
#define OSD_INT_STA			0x04
#define OSD_INT_CLRSTA			0x08
#define OSD_INT_MSK			0x0c
#define OSD_INT_AUX_UPT			BIT(14)
#define OSD_INT_MAIN_UPT		BIT(13)
#define OSD_INT_GL1_LBW			BIT(10)
#define OSD_INT_GL0_LBW			BIT(9)
#define OSD_INT_VL2_LBW			BIT(8)
#define OSD_INT_VL1_LBW			BIT(7)
#define OSD_INT_VL0_LBW			BIT(6)
#define OSD_INT_BUS_ERR			BIT(3)
#define OSD_INT_CFG_ERR			BIT(2)
#define OSD_INT_ERROR (\
	OSD_INT_GL1_LBW | OSD_INT_GL0_LBW | \
	OSD_INT_VL2_LBW | OSD_INT_VL1_LBW | OSD_INT_VL0_LBW | \
	OSD_INT_BUS_ERR | OSD_INT_CFG_ERR \
)
#define OSD_INT_ENABLE (OSD_INT_ERROR | OSD_INT_AUX_UPT | OSD_INT_MAIN_UPT)
#define OSD_CTRL0			0x10
#define OSD_CTRL0_VL0_EN		BIT(13)
#define OSD_CTRL0_VL0_SEL		BIT(12)
#define OSD_CTRL0_VL1_EN		BIT(11)
#define OSD_CTRL0_VL1_SEL		BIT(10)
#define OSD_CTRL0_VL2_EN		BIT(9)
#define OSD_CTRL0_VL2_SEL		BIT(8)
#define OSD_CTRL0_GL0_EN		BIT(7)
#define OSD_CTRL0_GL0_SEL		BIT(6)
#define OSD_CTRL0_GL1_EN		BIT(5)
#define OSD_CTRL0_GL1_SEL		BIT(4)
#define OSD_RST_CLR			0x1c
#define RST_PER_FRAME			BIT(19)

/* Main/Aux channel registers */
#define OSD_MAIN_CHN			0x470
#define OSD_AUX_CHN			0x4d0
#define CHN_CTRL0			0x00
#define CHN_ENABLE			BIT(0)
#define CHN_CTRL1			0x04
#define CHN_SCREEN_W_SHIFT		18
#define CHN_SCREEN_W_MASK		(0x1fff << CHN_SCREEN_W_SHIFT)
#define CHN_SCREEN_H_SHIFT		5
#define CHN_SCREEN_H_MASK		(0x1fff << CHN_SCREEN_H_SHIFT)
#define CHN_UPDATE			0x08
#define CHN_INTERLACE_BUF_CTRL		0x24
#define CHN_INTERLACE_EN		BIT(2)

/* Dither registers */
#define OSD_DITHER_CTRL0		0x00
#define DITHER_BYSPASS			BIT(31)

/* TIMING_CTRL registers */
#define TIMING_TC_ENABLE		0x04
#define AUX_TC_EN			BIT(1)
#define MAIN_TC_EN			BIT(0)
#define FIR_MAIN_ACTIVE			0x08
#define FIR_AUX_ACTIVE			0x0c
#define V_ACTIVE_SHIFT			16
#define V_ACTIVE_MASK			(0xffff << V_ACTIVE_SHIFT)
#define H_ACTIVE_SHIFT			0
#define H_ACTIVE_MASK			(0xffff << H_ACTIVE_SHIFT)
#define FIR_MAIN_H_TIMING		0x10
#define FIR_MAIN_V_TIMING		0x14
#define FIR_AUX_H_TIMING		0x18
#define FIR_AUX_V_TIMING		0x1c
#define SYNC_WIDE_SHIFT			22
#define SYNC_WIDE_MASK			(0x3ff << SYNC_WIDE_SHIFT)
#define BACK_PORCH_SHIFT		11
#define BACK_PORCH_MASK			(0x7ff << BACK_PORCH_SHIFT)
#define FRONT_PORCH_SHIFT		0
#define FRONT_PORCH_MASK		(0x7ff << FRONT_PORCH_SHIFT)
#define TIMING_CTRL			0x20
#define AUX_POL_SHIFT			3
#define AUX_POL_MASK			(0x7 << AUX_POL_SHIFT)
#define MAIN_POL_SHIFT			0
#define MAIN_POL_MASK			(0x7 << MAIN_POL_SHIFT)
#define POL_DE_SHIFT			2
#define POL_VSYNC_SHIFT			1
#define POL_HSYNC_SHIFT			0
#define TIMING_INT_CTRL			0x24
#define TIMING_INT_STATE		0x28
#define TIMING_INT_AUX_FRAME		BIT(3)
#define TIMING_INT_MAIN_FRAME		BIT(1)
#define TIMING_INT_AUX_FRAME_SEL_VSW	(0x2 << 10)
#define TIMING_INT_MAIN_FRAME_SEL_VSW	(0x2 << 6)
#define TIMING_INT_ENABLE (\
	TIMING_INT_MAIN_FRAME_SEL_VSW | TIMING_INT_AUX_FRAME_SEL_VSW | \
	TIMING_INT_MAIN_FRAME | TIMING_INT_AUX_FRAME \
)
#define TIMING_MAIN_SHIFT		0x2c
#define TIMING_AUX_SHIFT		0x30
#define H_SHIFT_VAL			0x0048
#define V_SHIFT_VAL			0x0001
#define SCAN_CTRL			0x34
#define AUX_PI_EN			BIT(19)
#define MAIN_PI_EN			BIT(18)
#define AUX_INTERLACE_SEL		BIT(1)
#define MAIN_INTERLACE_SEL		BIT(0)
#define SEC_V_ACTIVE			0x38
#define SEC_VACT_MAIN_SHIFT		0
#define SEC_VACT_MAIN_MASK		(0xffff << SEC_VACT_MAIN_SHIFT)
#define SEC_VACT_AUX_SHIFT		16
#define SEC_VACT_AUX_MASK		(0xffff << SEC_VACT_AUX_SHIFT)
#define SEC_MAIN_V_TIMING		0x3c
#define SEC_AUX_V_TIMING		0x40
#define TIMING_MAIN_PI_SHIFT		0x68
#define TIMING_AUX_PI_SHIFT		0x6c
#define H_PI_SHIFT_VAL			0x000f

#define V_ACTIVE(x)	(((x) << V_ACTIVE_SHIFT) & V_ACTIVE_MASK)
#define H_ACTIVE(x)	(((x) << H_ACTIVE_SHIFT) & H_ACTIVE_MASK)

#define SYNC_WIDE(x)	(((x) << SYNC_WIDE_SHIFT) & SYNC_WIDE_MASK)
#define BACK_PORCH(x)	(((x) << BACK_PORCH_SHIFT) & BACK_PORCH_MASK)
#define FRONT_PORCH(x)	(((x) << FRONT_PORCH_SHIFT) & FRONT_PORCH_MASK)

/* DTRC registers */
#define DTRC_F0_CTRL			0x2c
#define DTRC_F1_CTRL			0x5c
#define DTRC_DECOMPRESS_BYPASS		BIT(17)
#define DTRC_DETILE_CTRL		0x68
#define TILE2RASTESCAN_BYPASS_MODE	BIT(30)
#define DETILE_ARIDR_MODE_MASK		(0x3 << 0)
#define DETILE_ARID_ALL			0
#define DETILE_ARID_IN_ARIDR		1
#define DETILE_ARID_BYP_BUT_ARIDR	2
#define DETILE_ARID_IN_ARIDR2		3
#define DTRC_ARID			0x6c
#define DTRC_ARID3_SHIFT		24
#define DTRC_ARID3_MASK			(0xff << DTRC_ARID3_SHIFT)
#define DTRC_ARID2_SHIFT		16
#define DTRC_ARID2_MASK			(0xff << DTRC_ARID2_SHIFT)
#define DTRC_ARID1_SHIFT		8
#define DTRC_ARID1_MASK			(0xff << DTRC_ARID1_SHIFT)
#define DTRC_ARID0_SHIFT		0
#define DTRC_ARID0_MASK			(0xff << DTRC_ARID0_SHIFT)
#define DTRC_DEC2DDR_ARID		0x70

#define DTRC_ARID3(x)	(((x) << DTRC_ARID3_SHIFT) & DTRC_ARID3_MASK)
#define DTRC_ARID2(x)	(((x) << DTRC_ARID2_SHIFT) & DTRC_ARID2_MASK)
#define DTRC_ARID1(x)	(((x) << DTRC_ARID1_SHIFT) & DTRC_ARID1_MASK)
#define DTRC_ARID0(x)	(((x) << DTRC_ARID0_SHIFT) & DTRC_ARID0_MASK)

/* VOU_CTRL registers */
#define VOU_INF_EN			0x00
#define VOU_INF_CH_SEL			0x04
#define VOU_INF_DATA_SEL		0x08
#define VOU_SOFT_RST			0x14
#define VOU_CLK_SEL			0x18
#define VGA_AUX_DIV_SHIFT		29
#define VGA_MAIN_DIV_SHIFT		26
#define PIC_MAIN_DIV_SHIFT		23
#define PIC_AUX_DIV_SHIFT		20
#define VOU_CLK_VL2_SEL			BIT(8)
#define VOU_CLK_VL1_SEL			BIT(7)
#define VOU_CLK_VL0_SEL			BIT(6)
#define VOU_CLK_GL1_SEL			BIT(5)
#define VOU_CLK_GL0_SEL			BIT(4)
#define VOU_DIV_PARA			0x1c
#define DIV_PARA_UPDATE			BIT(31)
#define TVENC_AUX_DIV_SHIFT		28
#define HDMI_AUX_PNX_DIV_SHIFT		25
#define HDMI_MAIN_PNX_DIV_SHIFT		22
#define HDMI_AUX_DIV_SHIFT		19
#define HDMI_MAIN_DIV_SHIFT		16
#define TVENC_MAIN_DIV_SHIFT		13
#define INF_AUX_DIV_SHIFT		9
#define INF_MAIN_DIV_SHIFT		6
#define LAYER_AUX_DIV_SHIFT		3
#define LAYER_MAIN_DIV_SHIFT		0
#define VOU_CLK_REQEN			0x20
#define VOU_CLK_EN			0x24
#define VOU_INF_HDMI_CTRL		0x30
#define VOU_HDMI_AUD_MASK		0x1f

/* OTFPPU_CTRL registers */
#define OTFPPU_RSZ_DATA_SOURCE		0x04

#endif /* __ZX_VOU_REGS_H__ */
