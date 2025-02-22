<<<<<<< HEAD
/*
 * Copyright (C) 2017 Sanechips Technology Co., Ltd.
 * Copyright 2017 Linaro Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2017 Sanechips Technology Co., Ltd.
 * Copyright 2017 Linaro Ltd.
>>>>>>> upstream/android-13
 */

#ifndef __ZX_COMMON_REGS_H__
#define __ZX_COMMON_REGS_H__

/* CSC registers */
#define CSC_CTRL0			0x30
#define CSC_COV_MODE_SHIFT		16
#define CSC_COV_MODE_MASK		(0xffff << CSC_COV_MODE_SHIFT)
#define CSC_BT601_IMAGE_RGB2YCBCR	0
#define CSC_BT601_IMAGE_YCBCR2RGB	1
#define CSC_BT601_VIDEO_RGB2YCBCR	2
#define CSC_BT601_VIDEO_YCBCR2RGB	3
#define CSC_BT709_IMAGE_RGB2YCBCR	4
#define CSC_BT709_IMAGE_YCBCR2RGB	5
#define CSC_BT709_VIDEO_RGB2YCBCR	6
#define CSC_BT709_VIDEO_YCBCR2RGB	7
#define CSC_BT2020_IMAGE_RGB2YCBCR	8
#define CSC_BT2020_IMAGE_YCBCR2RGB	9
#define CSC_BT2020_VIDEO_RGB2YCBCR	10
#define CSC_BT2020_VIDEO_YCBCR2RGB	11
#define CSC_WORK_ENABLE			BIT(0)

#endif /* __ZX_COMMON_REGS_H__ */
