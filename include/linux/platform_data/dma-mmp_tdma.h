<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  SRAM Memory Management
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

#ifndef __DMA_MMP_TDMA_H
#define __DMA_MMP_TDMA_H

#include <linux/genalloc.h>

/* ARBITRARY:  SRAM allocations are multiples of this 2^N size */
#define SRAM_GRANULARITY	512

enum sram_type {
	MMP_SRAM_UNDEFINED = 0,
	MMP_ASRAM,
	MMP_ISRAM,
};

struct sram_platdata {
	char *pool_name;
	int granularity;
};

#ifdef CONFIG_MMP_SRAM
extern struct gen_pool *sram_get_gpool(char *pool_name);
#else
static inline struct gen_pool *sram_get_gpool(char *pool_name)
{
	return NULL;
}
#endif

#endif /* __DMA_MMP_TDMA_H */
