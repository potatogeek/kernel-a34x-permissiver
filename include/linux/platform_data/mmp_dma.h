<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  MMP Platform DMA Management
 *
 *  Copyright (c) 2011 Marvell Semiconductors Inc.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef MMP_DMA_H
#define MMP_DMA_H

struct dma_slave_map;

struct mmp_dma_platdata {
	int dma_channels;
	int nb_requestors;
	int slave_map_cnt;
	const struct dma_slave_map *slave_map;
};

#endif /* MMP_DMA_H */
