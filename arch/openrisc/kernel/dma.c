<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * OpenRISC Linux
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the OpenRISC architecture:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 *
<<<<<<< HEAD
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 * DMA mapping callbacks...
 * As alloc_coherent is the only DMA callback being used currently, that's
 * the only thing implemented properly.  The rest need looking into...
 */

#include <linux/dma-noncoherent.h>
=======
 * DMA mapping callbacks...
 */

#include <linux/dma-map-ops.h>
#include <linux/pagewalk.h>
>>>>>>> upstream/android-13

#include <asm/cpuinfo.h>
#include <asm/spr_defs.h>
#include <asm/tlbflush.h>

static int
page_set_nocache(pte_t *pte, unsigned long addr,
		 unsigned long next, struct mm_walk *walk)
{
	unsigned long cl;
	struct cpuinfo_or1k *cpuinfo = &cpuinfo_or1k[smp_processor_id()];

	pte_val(*pte) |= _PAGE_CI;

	/*
	 * Flush the page out of the TLB so that the new page flags get
	 * picked up next time there's an access
	 */
<<<<<<< HEAD
	flush_tlb_page(NULL, addr);
=======
	flush_tlb_kernel_range(addr, addr + PAGE_SIZE);
>>>>>>> upstream/android-13

	/* Flush page out of dcache */
	for (cl = __pa(addr); cl < __pa(next); cl += cpuinfo->dcache_block_size)
		mtspr(SPR_DCBFR, cl);

	return 0;
}

<<<<<<< HEAD
=======
static const struct mm_walk_ops set_nocache_walk_ops = {
	.pte_entry		= page_set_nocache,
};

>>>>>>> upstream/android-13
static int
page_clear_nocache(pte_t *pte, unsigned long addr,
		   unsigned long next, struct mm_walk *walk)
{
	pte_val(*pte) &= ~_PAGE_CI;

	/*
	 * Flush the page out of the TLB so that the new page flags get
	 * picked up next time there's an access
	 */
<<<<<<< HEAD
	flush_tlb_page(NULL, addr);
=======
	flush_tlb_kernel_range(addr, addr + PAGE_SIZE);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/*
 * Alloc "coherent" memory, which for OpenRISC means simply uncached.
 *
 * This function effectively just calls __get_free_pages, sets the
 * cache-inhibit bit on those pages, and makes sure that the pages are
 * flushed out of the cache before they are used.
 *
 * If the NON_CONSISTENT attribute is set, then this function just
 * returns "normal", cachable memory.
 *
 * There are additional flags WEAK_ORDERING and WRITE_COMBINE to take
 * into consideration here, too.  All current known implementations of
 * the OR1K support only strongly ordered memory accesses, so that flag
 * is being ignored for now; uncached but write-combined memory is a
 * missing feature of the OR1K.
 */
void *
arch_dma_alloc(struct device *dev, size_t size, dma_addr_t *dma_handle,
		gfp_t gfp, unsigned long attrs)
{
	unsigned long va;
	void *page;
	struct mm_walk walk = {
		.pte_entry = page_set_nocache,
		.mm = &init_mm
	};

	page = alloc_pages_exact(size, gfp);
	if (!page)
		return NULL;

	/* This gives us the real physical address of the first page. */
	*dma_handle = __pa(page);

	va = (unsigned long)page;

	if ((attrs & DMA_ATTR_NON_CONSISTENT) == 0) {
		/*
		 * We need to iterate through the pages, clearing the dcache for
		 * them and setting the cache-inhibit bit.
		 */
		if (walk_page_range(va, va + size, &walk)) {
			free_pages_exact(page, size);
			return NULL;
		}
	}

	return (void *)va;
}

void
arch_dma_free(struct device *dev, size_t size, void *vaddr,
		dma_addr_t dma_handle, unsigned long attrs)
{
	unsigned long va = (unsigned long)vaddr;
	struct mm_walk walk = {
		.pte_entry = page_clear_nocache,
		.mm = &init_mm
	};

	if ((attrs & DMA_ATTR_NON_CONSISTENT) == 0) {
		/* walk_page_range shouldn't be able to fail here */
		WARN_ON(walk_page_range(va, va + size, &walk));
	}

	free_pages_exact(vaddr, size);
}

void arch_sync_dma_for_device(struct device *dev, phys_addr_t addr, size_t size,
=======
static const struct mm_walk_ops clear_nocache_walk_ops = {
	.pte_entry		= page_clear_nocache,
};

void *arch_dma_set_uncached(void *cpu_addr, size_t size)
{
	unsigned long va = (unsigned long)cpu_addr;
	int error;

	/*
	 * We need to iterate through the pages, clearing the dcache for
	 * them and setting the cache-inhibit bit.
	 */
	mmap_read_lock(&init_mm);
	error = walk_page_range(&init_mm, va, va + size, &set_nocache_walk_ops,
			NULL);
	mmap_read_unlock(&init_mm);

	if (error)
		return ERR_PTR(error);
	return cpu_addr;
}

void arch_dma_clear_uncached(void *cpu_addr, size_t size)
{
	unsigned long va = (unsigned long)cpu_addr;

	mmap_read_lock(&init_mm);
	/* walk_page_range shouldn't be able to fail here */
	WARN_ON(walk_page_range(&init_mm, va, va + size,
			&clear_nocache_walk_ops, NULL));
	mmap_read_unlock(&init_mm);
}

void arch_sync_dma_for_device(phys_addr_t addr, size_t size,
>>>>>>> upstream/android-13
		enum dma_data_direction dir)
{
	unsigned long cl;
	struct cpuinfo_or1k *cpuinfo = &cpuinfo_or1k[smp_processor_id()];

	switch (dir) {
	case DMA_TO_DEVICE:
		/* Flush the dcache for the requested range */
		for (cl = addr; cl < addr + size;
		     cl += cpuinfo->dcache_block_size)
			mtspr(SPR_DCBFR, cl);
		break;
	case DMA_FROM_DEVICE:
		/* Invalidate the dcache for the requested range */
		for (cl = addr; cl < addr + size;
		     cl += cpuinfo->dcache_block_size)
			mtspr(SPR_DCBIR, cl);
		break;
	default:
		/*
		 * NOTE: If dir == DMA_BIDIRECTIONAL then there's no need to
		 * flush nor invalidate the cache here as the area will need
		 * to be manually synced anyway.
		 */
		break;
	}
}
