<<<<<<< HEAD
/*
 * Copyright (c) 2015, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2015, NVIDIA Corporation.
>>>>>>> upstream/android-13
 */

#ifndef TEGRA_VIC_H
#define TEGRA_VIC_H

/* VIC methods */

<<<<<<< HEAD
#define VIC_SET_APPLICATION_ID			0x00000200
=======
>>>>>>> upstream/android-13
#define VIC_SET_FCE_UCODE_SIZE			0x0000071C
#define VIC_SET_FCE_UCODE_OFFSET		0x0000072C

/* VIC registers */

<<<<<<< HEAD
=======
#define VIC_THI_STREAMID0	0x00000030
#define VIC_THI_STREAMID1	0x00000034

>>>>>>> upstream/android-13
#define NV_PVIC_MISC_PRI_VIC_CG			0x000016d0
#define CG_IDLE_CG_DLY_CNT(val)			((val & 0x3f) << 0)
#define CG_IDLE_CG_EN				(1 << 6)
#define CG_WAKEUP_DLY_CNT(val)			((val & 0xf) << 16)

<<<<<<< HEAD
=======
#define VIC_TFBIF_TRANSCFG	0x00002044
#define  TRANSCFG_ATT(i, v)	(((v) & 0x3) << (i * 4))
#define  TRANSCFG_SID_HW	0
#define  TRANSCFG_SID_PHY	1
#define  TRANSCFG_SID_FALCON	2

>>>>>>> upstream/android-13
/* Firmware offsets */

#define VIC_UCODE_FCE_HEADER_OFFSET		(6*4)
#define VIC_UCODE_FCE_DATA_OFFSET		(7*4)
#define FCE_UCODE_SIZE_OFFSET			(2*4)

#endif /* TEGRA_VIC_H */
