/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_IA64_DMA_H
#define _ASM_IA64_DMA_H

/*
 * Copyright (C) 1998-2002 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */


#include <asm/io.h>		/* need byte IO */

extern unsigned long MAX_DMA_ADDRESS;

<<<<<<< HEAD
#ifdef CONFIG_PCI
  extern int isa_dma_bridge_buggy;
#else
# define isa_dma_bridge_buggy 	(0)
#endif
=======
extern int isa_dma_bridge_buggy;
>>>>>>> upstream/android-13

#define free_dma(x)

#endif /* _ASM_IA64_DMA_H */
