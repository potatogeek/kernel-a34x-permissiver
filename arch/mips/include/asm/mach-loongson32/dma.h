<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2015 Zhang, Keguang <keguang.zhang@gmail.com>
 *
 * Loongson 1 NAND platform support.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute	it and/or modify it
 * under  the terms of	the GNU General	 Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __ASM_MACH_LOONGSON32_DMA_H
#define __ASM_MACH_LOONGSON32_DMA_H

#define LS1X_DMA_CHANNEL0	0
#define LS1X_DMA_CHANNEL1	1
#define LS1X_DMA_CHANNEL2	2

struct plat_ls1x_dma {
	int nr_channels;
};

extern struct plat_ls1x_dma ls1b_dma_pdata;

#endif /* __ASM_MACH_LOONGSON32_DMA_H */
