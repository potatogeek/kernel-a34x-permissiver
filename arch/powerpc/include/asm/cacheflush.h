<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
>>>>>>> upstream/android-13
 */
#ifndef _ASM_POWERPC_CACHEFLUSH_H
#define _ASM_POWERPC_CACHEFLUSH_H

<<<<<<< HEAD
#ifdef __KERNEL__

#include <linux/mm.h>
#include <asm/cputable.h>

/*
 * No cache flushing is required when address mappings are changed,
 * because the caches on PowerPCs are physically addressed.
 */
#define flush_cache_all()			do { } while (0)
#define flush_cache_mm(mm)			do { } while (0)
#define flush_cache_dup_mm(mm)			do { } while (0)
#define flush_cache_range(vma, start, end)	do { } while (0)
#define flush_cache_page(vma, vmaddr, pfn)	do { } while (0)
#define flush_icache_page(vma, page)		do { } while (0)
#define flush_cache_vunmap(start, end)		do { } while (0)
=======
#include <linux/mm.h>
#include <asm/cputable.h>
#include <asm/cpu_has_feature.h>

/*
 * This flag is used to indicate that the page pointed to by a pte is clean
 * and does not require cleaning before returning it to the user.
 */
#define PG_dcache_clean PG_arch_1
>>>>>>> upstream/android-13

#ifdef CONFIG_PPC_BOOK3S_64
/*
 * Book3s has no ptesync after setting a pte, so without this ptesync it's
 * possible for a kernel virtual mapping access to return a spurious fault
 * if it's accessed right after the pte is set. The page fault handler does
 * not expect this type of fault. flush_cache_vmap is not exactly the right
 * place to put this, but it seems to work well enough.
 */
static inline void flush_cache_vmap(unsigned long start, unsigned long end)
{
	asm volatile("ptesync" ::: "memory");
}
<<<<<<< HEAD
#else
static inline void flush_cache_vmap(unsigned long start, unsigned long end) { }
#endif

#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 1
extern void flush_dcache_page(struct page *page);
#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)

extern void flush_icache_range(unsigned long, unsigned long);
extern void flush_icache_user_range(struct vm_area_struct *vma,
				    struct page *page, unsigned long addr,
				    int len);
extern void __flush_dcache_icache(void *page_va);
extern void flush_dcache_icache_page(struct page *page);
#if defined(CONFIG_PPC32) && !defined(CONFIG_BOOKE)
extern void __flush_dcache_icache_phys(unsigned long physaddr);
#else
static inline void __flush_dcache_icache_phys(unsigned long physaddr)
{
	BUG();
}
#endif

#ifdef CONFIG_PPC32
/*
 * Write any modified data cache blocks out to memory and invalidate them.
 * Does not invalidate the corresponding instruction cache blocks.
 */
static inline void flush_dcache_range(unsigned long start, unsigned long stop)
{
	void *addr = (void *)(start & ~(L1_CACHE_BYTES - 1));
	unsigned long size = stop - (unsigned long)addr + (L1_CACHE_BYTES - 1);
	unsigned long i;

	for (i = 0; i < size >> L1_CACHE_SHIFT; i++, addr += L1_CACHE_BYTES)
		dcbf(addr);
	mb();	/* sync */
=======
#define flush_cache_vmap flush_cache_vmap
#endif /* CONFIG_PPC_BOOK3S_64 */

#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 1
/*
 * This is called when a page has been modified by the kernel.
 * It just marks the page as not i-cache clean.  We do the i-cache
 * flush later when the page is given to a user process, if necessary.
 */
static inline void flush_dcache_page(struct page *page)
{
	if (cpu_has_feature(CPU_FTR_COHERENT_ICACHE))
		return;
	/* avoid an atomic op if possible */
	if (test_bit(PG_dcache_clean, &page->flags))
		clear_bit(PG_dcache_clean, &page->flags);
}

void flush_icache_range(unsigned long start, unsigned long stop);
#define flush_icache_range flush_icache_range

void flush_icache_user_page(struct vm_area_struct *vma, struct page *page,
		unsigned long addr, int len);
#define flush_icache_user_page flush_icache_user_page

void flush_dcache_icache_page(struct page *page);

/**
 * flush_dcache_range(): Write any modified data cache blocks out to memory and
 * invalidate them. Does not invalidate the corresponding instruction cache
 * blocks.
 *
 * @start: the start address
 * @stop: the stop address (exclusive)
 */
static inline void flush_dcache_range(unsigned long start, unsigned long stop)
{
	unsigned long shift = l1_dcache_shift();
	unsigned long bytes = l1_dcache_bytes();
	void *addr = (void *)(start & ~(bytes - 1));
	unsigned long size = stop - (unsigned long)addr + (bytes - 1);
	unsigned long i;

	if (IS_ENABLED(CONFIG_PPC64))
		mb();	/* sync */

	for (i = 0; i < size >> shift; i++, addr += bytes)
		dcbf(addr);
	mb();	/* sync */

>>>>>>> upstream/android-13
}

/*
 * Write any modified data cache blocks out to memory.
 * Does not invalidate the corresponding cache lines (especially for
 * any corresponding instruction cache).
 */
static inline void clean_dcache_range(unsigned long start, unsigned long stop)
{
<<<<<<< HEAD
	void *addr = (void *)(start & ~(L1_CACHE_BYTES - 1));
	unsigned long size = stop - (unsigned long)addr + (L1_CACHE_BYTES - 1);
	unsigned long i;

	for (i = 0; i < size >> L1_CACHE_SHIFT; i++, addr += L1_CACHE_BYTES)
=======
	unsigned long shift = l1_dcache_shift();
	unsigned long bytes = l1_dcache_bytes();
	void *addr = (void *)(start & ~(bytes - 1));
	unsigned long size = stop - (unsigned long)addr + (bytes - 1);
	unsigned long i;

	for (i = 0; i < size >> shift; i++, addr += bytes)
>>>>>>> upstream/android-13
		dcbst(addr);
	mb();	/* sync */
}

/*
 * Like above, but invalidate the D-cache.  This is used by the 8xx
 * to invalidate the cache so the PPC core doesn't get stale data
 * from the CPM (no cache snooping here :-).
 */
static inline void invalidate_dcache_range(unsigned long start,
					   unsigned long stop)
{
<<<<<<< HEAD
	void *addr = (void *)(start & ~(L1_CACHE_BYTES - 1));
	unsigned long size = stop - (unsigned long)addr + (L1_CACHE_BYTES - 1);
	unsigned long i;

	for (i = 0; i < size >> L1_CACHE_SHIFT; i++, addr += L1_CACHE_BYTES)
=======
	unsigned long shift = l1_dcache_shift();
	unsigned long bytes = l1_dcache_bytes();
	void *addr = (void *)(start & ~(bytes - 1));
	unsigned long size = stop - (unsigned long)addr + (bytes - 1);
	unsigned long i;

	for (i = 0; i < size >> shift; i++, addr += bytes)
>>>>>>> upstream/android-13
		dcbi(addr);
	mb();	/* sync */
}

<<<<<<< HEAD
#endif /* CONFIG_PPC32 */
#ifdef CONFIG_PPC64
extern void flush_dcache_range(unsigned long start, unsigned long stop);
extern void flush_inval_dcache_range(unsigned long start, unsigned long stop);
#endif

#define copy_to_user_page(vma, page, vaddr, dst, src, len) \
	do { \
		memcpy(dst, src, len); \
		flush_icache_user_range(vma, page, vaddr, len); \
	} while (0)
#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)

#endif /* __KERNEL__ */
=======
#ifdef CONFIG_4xx
static inline void flush_instruction_cache(void)
{
	iccci((void *)KERNELBASE);
	isync();
}
#else
void flush_instruction_cache(void);
#endif

#include <asm-generic/cacheflush.h>
>>>>>>> upstream/android-13

#endif /* _ASM_POWERPC_CACHEFLUSH_H */
