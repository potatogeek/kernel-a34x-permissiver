<<<<<<< HEAD
/*
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2006 Atmark Techno, Inc.
>>>>>>> upstream/android-13
 */

#ifndef _ASM_MICROBLAZE_DMA_H
#define _ASM_MICROBLAZE_DMA_H

<<<<<<< HEAD
#ifndef CONFIG_MMU
/* we don't have dma address limit. define it as zero to be
 * unlimited. */
#define MAX_DMA_ADDRESS		(0)
#else
/* Virtual address corresponding to last available physical memory address.  */
#define MAX_DMA_ADDRESS (CONFIG_KERNEL_START + memory_size - 1)
#endif
=======
/* Virtual address corresponding to last available physical memory address.  */
#define MAX_DMA_ADDRESS (CONFIG_KERNEL_START + memory_size - 1)
>>>>>>> upstream/android-13

#ifdef CONFIG_PCI
extern int isa_dma_bridge_buggy;
#else
#define isa_dma_bridge_buggy     (0)
#endif

#endif /* _ASM_MICROBLAZE_DMA_H */
