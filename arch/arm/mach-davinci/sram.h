<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * mach/sram.h - DaVinci simple SRAM allocator
 *
 * Copyright (C) 2009 David Brownell
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef __MACH_SRAM_H
#define __MACH_SRAM_H

/* ARBITRARY:  SRAM allocations are multiples of this 2^N size */
#define SRAM_GRANULARITY	512

/*
 * SRAM allocations return a CPU virtual address, or NULL on error.
 * If a DMA address is requested and the SRAM supports DMA, its
 * mapped address is also returned.
 *
 * Errors include SRAM memory not being available, and requesting
 * DMA mapped SRAM on systems which don't allow that.
 */
extern void *sram_alloc(size_t len, dma_addr_t *dma);
extern void sram_free(void *addr, size_t len);

/* Get the struct gen_pool * for use in platform data */
extern struct gen_pool *sram_get_gen_pool(void);

#endif /* __MACH_SRAM_H */
