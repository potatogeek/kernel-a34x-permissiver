<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * drivers/video/geode/video_cs5530.h
 *   -- CS5530 video device
 *
 * Copyright (C) 2005 Arcom Control Systems Ltd.
 *
 * Based on AMD's original 2.4 driver:
 *   Copyright (C) 2004 Advanced Micro Devices, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#ifndef __VIDEO_CS5530_H__
#define __VIDEO_CS5530_H__

extern const struct geode_vid_ops cs5530_vid_ops;

/* CS5530 Video device registers */

#define CS5530_VIDEO_CONFIG	0x0000
#  define CS5530_VCFG_VID_EN			0x00000001
#  define CS5530_VCFG_VID_REG_UPDATE		0x00000002
#  define CS5530_VCFG_VID_INP_FORMAT		0x0000000C
#  define CS5530_VCFG_8_BIT_4_2_0		0x00000004
#  define CS5530_VCFG_16_BIT_4_2_0		0x00000008
#  define CS5530_VCFG_GV_SEL			0x00000010
#  define CS5530_VCFG_CSC_BYPASS		0x00000020
#  define CS5530_VCFG_X_FILTER_EN		0x00000040
#  define CS5530_VCFG_Y_FILTER_EN		0x00000080
#  define CS5530_VCFG_LINE_SIZE_LOWER_MASK	0x0000FF00
#  define CS5530_VCFG_INIT_READ_MASK		0x01FF0000
#  define CS5530_VCFG_EARLY_VID_RDY		0x02000000
#  define CS5530_VCFG_LINE_SIZE_UPPER		0x08000000
#  define CS5530_VCFG_4_2_0_MODE		0x10000000
#  define CS5530_VCFG_16_BIT_EN			0x20000000
#  define CS5530_VCFG_HIGH_SPD_INT		0x40000000

#define CS5530_DISPLAY_CONFIG	0x0004
#  define CS5530_DCFG_DIS_EN			0x00000001
#  define CS5530_DCFG_HSYNC_EN			0x00000002
#  define CS5530_DCFG_VSYNC_EN			0x00000004
#  define CS5530_DCFG_DAC_BL_EN			0x00000008
#  define CS5530_DCFG_DAC_PWR_EN		0x00000020
#  define CS5530_DCFG_FP_PWR_EN			0x00000040
#  define CS5530_DCFG_FP_DATA_EN		0x00000080
#  define CS5530_DCFG_CRT_HSYNC_POL		0x00000100
#  define CS5530_DCFG_CRT_VSYNC_POL		0x00000200
#  define CS5530_DCFG_FP_HSYNC_POL		0x00000400
#  define CS5530_DCFG_FP_VSYNC_POL		0x00000800
#  define CS5530_DCFG_XGA_FP			0x00001000
#  define CS5530_DCFG_FP_DITH_EN		0x00002000
#  define CS5530_DCFG_CRT_SYNC_SKW_MASK		0x0001C000
#  define CS5530_DCFG_CRT_SYNC_SKW_INIT		0x00010000
#  define CS5530_DCFG_PWR_SEQ_DLY_MASK		0x000E0000
#  define CS5530_DCFG_PWR_SEQ_DLY_INIT		0x00080000
#  define CS5530_DCFG_VG_CK			0x00100000
#  define CS5530_DCFG_GV_PAL_BYP		0x00200000
#  define CS5530_DCFG_DDC_SCL			0x00400000
#  define CS5530_DCFG_DDC_SDA			0x00800000
#  define CS5530_DCFG_DDC_OE			0x01000000
#  define CS5530_DCFG_16_BIT_EN			0x02000000

#define CS5530_VIDEO_X_POS	0x0008
#define CS5530_VIDEO_Y_POS	0x000C
#define CS5530_VIDEO_SCALE	0x0010
#define CS5530_VIDEO_COLOR_KEY	0x0014
#define CS5530_VIDEO_COLOR_MASK 0x0018
#define CS5530_PALETTE_ADDRESS	0x001C
#define CS5530_PALETTE_DATA	0x0020
#define CS5530_DOT_CLK_CONFIG	0x0024
#define CS5530_CRCSIG_TFT_TV	0x0028

#endif /* !__VIDEO_CS5530_H__ */
