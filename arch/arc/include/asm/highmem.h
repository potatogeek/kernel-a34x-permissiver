<<<<<<< HEAD
/*
 * Copyright (C) 2015 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2015 Synopsys, Inc. (www.synopsys.com)
>>>>>>> upstream/android-13
 */

#ifndef _ASM_HIGHMEM_H
#define _ASM_HIGHMEM_H

#ifdef CONFIG_HIGHMEM

#include <uapi/asm/page.h>
<<<<<<< HEAD
#include <asm/kmap_types.h>

/* start after vmalloc area */
#define FIXMAP_BASE		(PAGE_OFFSET - FIXMAP_SIZE - PKMAP_SIZE)
#define FIXMAP_SIZE		PGDIR_SIZE	/* only 1 PGD worth */
#define KM_TYPE_NR		((FIXMAP_SIZE >> PAGE_SHIFT)/NR_CPUS)
#define FIXMAP_ADDR(nr)		(FIXMAP_BASE + ((nr) << PAGE_SHIFT))

/* start after fixmap area */
#define PKMAP_BASE		(FIXMAP_BASE + FIXMAP_SIZE)
#define PKMAP_SIZE		PGDIR_SIZE
=======
#include <asm/kmap_size.h>

#define FIXMAP_SIZE		PGDIR_SIZE
#define PKMAP_SIZE		PGDIR_SIZE

/* start after vmalloc area */
#define FIXMAP_BASE		(PAGE_OFFSET - FIXMAP_SIZE - PKMAP_SIZE)

#define FIX_KMAP_SLOTS		(KM_MAX_IDX * NR_CPUS)
#define FIX_KMAP_BEGIN		(0UL)
#define FIX_KMAP_END		((FIX_KMAP_BEGIN + FIX_KMAP_SLOTS) - 1)

#define FIXADDR_TOP		(FIXMAP_BASE + (FIX_KMAP_END << PAGE_SHIFT))

/*
 * This should be converted to the asm-generic version, but of course this
 * is needlessly different from all other architectures. Sigh - tglx
 */
#define __fix_to_virt(x)	(FIXADDR_TOP - ((x) << PAGE_SHIFT))
#define __virt_to_fix(x)	(((FIXADDR_TOP - ((x) & PAGE_MASK))) >> PAGE_SHIFT)

/* start after fixmap area */
#define PKMAP_BASE		(FIXMAP_BASE + FIXMAP_SIZE)
>>>>>>> upstream/android-13
#define LAST_PKMAP		(PKMAP_SIZE >> PAGE_SHIFT)
#define LAST_PKMAP_MASK		(LAST_PKMAP - 1)
#define PKMAP_ADDR(nr)		(PKMAP_BASE + ((nr) << PAGE_SHIFT))
#define PKMAP_NR(virt)		(((virt) - PKMAP_BASE) >> PAGE_SHIFT)

<<<<<<< HEAD
#define kmap_prot		PAGE_KERNEL


#include <asm/cacheflush.h>

extern void *kmap(struct page *page);
extern void *kmap_high(struct page *page);
extern void *kmap_atomic(struct page *page);
extern void __kunmap_atomic(void *kvaddr);
extern void kunmap_high(struct page *page);

extern void kmap_init(void);

=======
#include <asm/cacheflush.h>

extern void kmap_init(void);

#define arch_kmap_local_post_unmap(vaddr)			\
	local_flush_tlb_kernel_range(vaddr, vaddr + PAGE_SIZE)

>>>>>>> upstream/android-13
static inline void flush_cache_kmaps(void)
{
	flush_cache_all();
}
<<<<<<< HEAD

static inline void kunmap(struct page *page)
{
	BUG_ON(in_interrupt());
	if (!PageHighMem(page))
		return;
	kunmap_high(page);
}


=======
>>>>>>> upstream/android-13
#endif

#endif
