/*
 * include/asm-xtensa/highmem.h
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file "COPYING" in the main directory of
 * this archive for more details.
 *
 * Copyright (C) 2003 - 2005 Tensilica Inc.
 * Copyright (C) 2014 Cadence Design Systems Inc.
 */

#ifndef _XTENSA_HIGHMEM_H
#define _XTENSA_HIGHMEM_H

<<<<<<< HEAD
#include <linux/wait.h>
#include <asm/cacheflush.h>
#include <asm/fixmap.h>
#include <asm/kmap_types.h>
#include <asm/pgtable.h>

#define PKMAP_BASE		((FIXADDR_START - \
=======
#ifdef CONFIG_HIGHMEM
#include <linux/wait.h>
#include <linux/pgtable.h>
#include <asm/cacheflush.h>
#include <asm/fixmap.h>

#define PKMAP_BASE		((FIXADDR_START -			\
>>>>>>> upstream/android-13
				  (LAST_PKMAP + 1) * PAGE_SIZE) & PMD_MASK)
#define LAST_PKMAP		(PTRS_PER_PTE * DCACHE_N_COLORS)
#define LAST_PKMAP_MASK		(LAST_PKMAP - 1)
#define PKMAP_NR(virt)		(((virt) - PKMAP_BASE) >> PAGE_SHIFT)
#define PKMAP_ADDR(nr)		(PKMAP_BASE + ((nr) << PAGE_SHIFT))

#define kmap_prot		PAGE_KERNEL_EXEC

#if DCACHE_WAY_SIZE > PAGE_SIZE
#define get_pkmap_color get_pkmap_color
static inline int get_pkmap_color(struct page *page)
{
	return DCACHE_ALIAS(page_to_phys(page));
}

extern unsigned int last_pkmap_nr_arr[];

static inline unsigned int get_next_pkmap_nr(unsigned int color)
{
	last_pkmap_nr_arr[color] =
		(last_pkmap_nr_arr[color] + DCACHE_N_COLORS) & LAST_PKMAP_MASK;
	return last_pkmap_nr_arr[color] + color;
}

static inline int no_more_pkmaps(unsigned int pkmap_nr, unsigned int color)
{
	return pkmap_nr < DCACHE_N_COLORS;
}

static inline int get_pkmap_entries_count(unsigned int color)
{
	return LAST_PKMAP / DCACHE_N_COLORS;
}

extern wait_queue_head_t pkmap_map_wait_arr[];

static inline wait_queue_head_t *get_pkmap_wait_queue_head(unsigned int color)
{
	return pkmap_map_wait_arr + color;
}
<<<<<<< HEAD
=======

enum fixed_addresses kmap_local_map_idx(int type, unsigned long pfn);
#define arch_kmap_local_map_idx		kmap_local_map_idx

enum fixed_addresses kmap_local_unmap_idx(int type, unsigned long addr);
#define arch_kmap_local_unmap_idx	kmap_local_unmap_idx

>>>>>>> upstream/android-13
#endif

extern pte_t *pkmap_page_table;

<<<<<<< HEAD
void *kmap_high(struct page *page);
void kunmap_high(struct page *page);

static inline void *kmap(struct page *page)
{
	/* Check if this memory layout is broken because PKMAP overlaps
	 * page table.
	 */
	BUILD_BUG_ON(PKMAP_BASE <
		     TLBTEMP_BASE_1 + TLBTEMP_SIZE);
	BUG_ON(in_interrupt());
	if (!PageHighMem(page))
		return page_address(page);
	return kmap_high(page);
}

static inline void kunmap(struct page *page)
{
	BUG_ON(in_interrupt());
	if (!PageHighMem(page))
		return;
	kunmap_high(page);
}

=======
>>>>>>> upstream/android-13
static inline void flush_cache_kmaps(void)
{
	flush_cache_all();
}

<<<<<<< HEAD
void *kmap_atomic(struct page *page);
void __kunmap_atomic(void *kvaddr);

void kmap_init(void);

=======
#define arch_kmap_local_post_unmap(vaddr)	\
	local_flush_tlb_kernel_range(vaddr, vaddr + PAGE_SIZE)

void kmap_init(void);

#endif /* CONFIG_HIGHMEM */
>>>>>>> upstream/android-13
#endif
