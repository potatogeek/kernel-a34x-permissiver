<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/* Hisilicon Hibmc SoC drm driver
 *
 * Based on the bochs drm driver.
 *
 * Copyright (c) 2016 Huawei Limited.
 *
 * Author:
 *	Rongrong Zou <zourongrong@huawei.com>
 *	Rongrong Zou <zourongrong@gmail.com>
 *	Jianhua Li <lijianhua@huawei.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef HIBMC_DRM_HW_H
#define HIBMC_DRM_HW_H

/* register definition */
#define HIBMC_MISC_CTRL				0x4

#define HIBMC_MSCCTL_LOCALMEM_RESET(x)		((x) << 6)
#define HIBMC_MSCCTL_LOCALMEM_RESET_MASK	0x40

#define HIBMC_CURRENT_GATE			0x000040
#define HIBMC_CURR_GATE_DISPLAY(x)		((x) << 2)
#define HIBMC_CURR_GATE_DISPLAY_MASK		0x4

#define HIBMC_CURR_GATE_LOCALMEM(x)		((x) << 1)
#define HIBMC_CURR_GATE_LOCALMEM_MASK		0x2

#define HIBMC_MODE0_GATE			0x000044
#define HIBMC_MODE1_GATE			0x000048
#define HIBMC_POWER_MODE_CTRL			0x00004C

#define HIBMC_PW_MODE_CTL_OSC_INPUT(x)		((x) << 3)
#define HIBMC_PW_MODE_CTL_OSC_INPUT_MASK	0x8

#define HIBMC_PW_MODE_CTL_MODE(x)		((x) << 0)
#define HIBMC_PW_MODE_CTL_MODE_MASK		0x03
#define HIBMC_PW_MODE_CTL_MODE_SHIFT		0

#define HIBMC_PW_MODE_CTL_MODE_MODE0		0
#define HIBMC_PW_MODE_CTL_MODE_MODE1		1
#define HIBMC_PW_MODE_CTL_MODE_SLEEP		2

#define HIBMC_PANEL_PLL_CTRL			0x00005C
#define HIBMC_CRT_PLL_CTRL			0x000060

#define HIBMC_PLL_CTRL_BYPASS(x)		((x) << 18)
#define HIBMC_PLL_CTRL_BYPASS_MASK		0x40000

#define HIBMC_PLL_CTRL_POWER(x)			((x) << 17)
#define HIBMC_PLL_CTRL_POWER_MASK		0x20000

#define HIBMC_PLL_CTRL_INPUT(x)			((x) << 16)
#define HIBMC_PLL_CTRL_INPUT_MASK		0x10000

#define HIBMC_PLL_CTRL_POD(x)			((x) << 14)
#define HIBMC_PLL_CTRL_POD_MASK			0xC000

#define HIBMC_PLL_CTRL_OD(x)			((x) << 12)
#define HIBMC_PLL_CTRL_OD_MASK			0x3000

#define HIBMC_PLL_CTRL_N(x)			((x) << 8)
#define HIBMC_PLL_CTRL_N_MASK			0xF00

#define HIBMC_PLL_CTRL_M(x)			((x) << 0)
#define HIBMC_PLL_CTRL_M_MASK			0xFF

#define HIBMC_CRT_DISP_CTL			0x80200

<<<<<<< HEAD
=======
#define HIBMC_CRT_DISP_CTL_DPMS(x)		((x) << 30)
#define HIBMC_CRT_DISP_CTL_DPMS_MASK		0xc0000000

#define HIBMC_CRT_DPMS_ON			0
#define HIBMC_CRT_DPMS_OFF			3

>>>>>>> upstream/android-13
#define HIBMC_CRT_DISP_CTL_CRTSELECT(x)		((x) << 25)
#define HIBMC_CRT_DISP_CTL_CRTSELECT_MASK	0x2000000

#define HIBMC_CRTSELECT_CRT			1

#define HIBMC_CRT_DISP_CTL_CLOCK_PHASE(x)	((x) << 14)
#define HIBMC_CRT_DISP_CTL_CLOCK_PHASE_MASK	0x4000

#define HIBMC_CRT_DISP_CTL_VSYNC_PHASE(x)	((x) << 13)
#define HIBMC_CRT_DISP_CTL_VSYNC_PHASE_MASK	0x2000

#define HIBMC_CRT_DISP_CTL_HSYNC_PHASE(x)	((x) << 12)
#define HIBMC_CRT_DISP_CTL_HSYNC_PHASE_MASK	0x1000

#define HIBMC_CRT_DISP_CTL_TIMING(x)		((x) << 8)
#define HIBMC_CRT_DISP_CTL_TIMING_MASK		0x100

<<<<<<< HEAD
=======
#define HIBMC_CTL_DISP_CTL_GAMMA(x)		((x) << 3)
#define HIBMC_CTL_DISP_CTL_GAMMA_MASK		0x08

>>>>>>> upstream/android-13
#define HIBMC_CRT_DISP_CTL_PLANE(x)		((x) << 2)
#define HIBMC_CRT_DISP_CTL_PLANE_MASK		4

#define HIBMC_CRT_DISP_CTL_FORMAT(x)		((x) << 0)
#define HIBMC_CRT_DISP_CTL_FORMAT_MASK		0x03

#define HIBMC_CRT_FB_ADDRESS			0x080204

#define HIBMC_CRT_FB_WIDTH			0x080208
#define HIBMC_CRT_FB_WIDTH_WIDTH(x)		((x) << 16)
#define HIBMC_CRT_FB_WIDTH_WIDTH_MASK		0x3FFF0000
#define HIBMC_CRT_FB_WIDTH_OFFS(x)		((x) << 0)
#define HIBMC_CRT_FB_WIDTH_OFFS_MASK		0x3FFF

#define HIBMC_CRT_HORZ_TOTAL			0x08020C
#define HIBMC_CRT_HORZ_TOTAL_TOTAL(x)		((x) << 16)
#define HIBMC_CRT_HORZ_TOTAL_TOTAL_MASK		0xFFF0000

#define HIBMC_CRT_HORZ_TOTAL_DISP_END(x)	((x) << 0)
#define HIBMC_CRT_HORZ_TOTAL_DISP_END_MASK	0xFFF

#define HIBMC_CRT_HORZ_SYNC			0x080210
#define HIBMC_CRT_HORZ_SYNC_WIDTH(x)		((x) << 16)
#define HIBMC_CRT_HORZ_SYNC_WIDTH_MASK		0xFF0000

#define HIBMC_CRT_HORZ_SYNC_START(x)		((x) << 0)
#define HIBMC_CRT_HORZ_SYNC_START_MASK		0xFFF

#define HIBMC_CRT_VERT_TOTAL			0x080214
#define HIBMC_CRT_VERT_TOTAL_TOTAL(x)		((x) << 16)
#define HIBMC_CRT_VERT_TOTAL_TOTAL_MASK		0x7FFF0000

#define HIBMC_CRT_VERT_TOTAL_DISP_END(x)	((x) << 0)
#define HIBMC_CRT_VERT_TOTAL_DISP_END_MASK	0x7FF

#define HIBMC_CRT_VERT_SYNC			0x080218
#define HIBMC_CRT_VERT_SYNC_HEIGHT(x)		((x) << 16)
#define HIBMC_CRT_VERT_SYNC_HEIGHT_MASK		0x3F0000

#define HIBMC_CRT_VERT_SYNC_START(x)		((x) << 0)
#define HIBMC_CRT_VERT_SYNC_START_MASK		0x7FF

/* Auto Centering */
#define HIBMC_CRT_AUTO_CENTERING_TL		0x080280
#define HIBMC_CRT_AUTO_CENTERING_TL_TOP(x)	((x) << 16)
#define HIBMC_CRT_AUTO_CENTERING_TL_TOP_MASK	0x7FF0000

#define HIBMC_CRT_AUTO_CENTERING_TL_LEFT(x)	((x) << 0)
#define HIBMC_CRT_AUTO_CENTERING_TL_LEFT_MASK	0x7FF

#define HIBMC_CRT_AUTO_CENTERING_BR		0x080284
#define HIBMC_CRT_AUTO_CENTERING_BR_BOTTOM(x)	((x) << 16)
#define HIBMC_CRT_AUTO_CENTERING_BR_BOTTOM_MASK	0x7FF0000

#define HIBMC_CRT_AUTO_CENTERING_BR_RIGHT(x)	((x) << 0)
#define HIBMC_CRT_AUTO_CENTERING_BR_RIGHT_MASK	0x7FF

/* register to control panel output */
#define HIBMC_DISPLAY_CONTROL_HISILE		0x80288
#define HIBMC_DISPLAY_CONTROL_FPVDDEN(x)	((x) << 0)
#define HIBMC_DISPLAY_CONTROL_PANELDATE(x)	((x) << 1)
#define HIBMC_DISPLAY_CONTROL_FPEN(x)		((x) << 2)
#define HIBMC_DISPLAY_CONTROL_VBIASEN(x)	((x) << 3)

#define HIBMC_RAW_INTERRUPT			0x80290
#define HIBMC_RAW_INTERRUPT_VBLANK(x)		((x) << 2)
#define HIBMC_RAW_INTERRUPT_VBLANK_MASK		0x4

#define HIBMC_RAW_INTERRUPT_EN			0x80298
#define HIBMC_RAW_INTERRUPT_EN_VBLANK(x)	((x) << 2)
#define HIBMC_RAW_INTERRUPT_EN_VBLANK_MASK	0x4

/* register and values for PLL control */
#define CRT_PLL1_HS				0x802a8
#define CRT_PLL1_HS_OUTER_BYPASS(x)		((x) << 30)
#define CRT_PLL1_HS_INTER_BYPASS(x)		((x) << 29)
#define CRT_PLL1_HS_POWERON(x)			((x) << 24)

#define CRT_PLL1_HS_25MHZ			0x23d40f02
#define CRT_PLL1_HS_40MHZ			0x23940801
#define CRT_PLL1_HS_65MHZ			0x23940d01
#define CRT_PLL1_HS_78MHZ			0x23540F82
#define CRT_PLL1_HS_74MHZ			0x23941dc2
#define CRT_PLL1_HS_80MHZ			0x23941001
#define CRT_PLL1_HS_80MHZ_1152			0x23540fc2
<<<<<<< HEAD
=======
#define CRT_PLL1_HS_106MHZ			0x237C1641
>>>>>>> upstream/android-13
#define CRT_PLL1_HS_108MHZ			0x23b41b01
#define CRT_PLL1_HS_162MHZ			0x23480681
#define CRT_PLL1_HS_148MHZ			0x23541dc2
#define CRT_PLL1_HS_193MHZ			0x234807c1

#define CRT_PLL2_HS				0x802ac
#define CRT_PLL2_HS_25MHZ			0x206B851E
#define CRT_PLL2_HS_40MHZ			0x30000000
#define CRT_PLL2_HS_65MHZ			0x40000000
#define CRT_PLL2_HS_78MHZ			0x50E147AE
#define CRT_PLL2_HS_74MHZ			0x602B6AE7
#define CRT_PLL2_HS_80MHZ			0x70000000
<<<<<<< HEAD
=======
#define CRT_PLL2_HS_106MHZ			0x0075c28f
>>>>>>> upstream/android-13
#define CRT_PLL2_HS_108MHZ			0x80000000
#define CRT_PLL2_HS_162MHZ			0xA0000000
#define CRT_PLL2_HS_148MHZ			0xB0CCCCCD
#define CRT_PLL2_HS_193MHZ			0xC0872B02

<<<<<<< HEAD
=======
#define HIBMC_CRT_PALETTE                       0x80C00

>>>>>>> upstream/android-13
#define HIBMC_FIELD(field, value) (field(value) & field##_MASK)
#endif
