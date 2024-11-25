/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_HIGHMEM_H
#define _ASM_HIGHMEM_H

<<<<<<< HEAD
#include <asm/kmap_types.h>
=======
#include <asm/cachetype.h>
#include <asm/fixmap.h>
>>>>>>> upstream/android-13

#define PKMAP_BASE		(PAGE_OFFSET - PMD_SIZE)
#define LAST_PKMAP		PTRS_PER_PTE
#define LAST_PKMAP_MASK		(LAST_PKMAP - 1)
#define PKMAP_NR(virt)		(((virt) - PKMAP_BASE) >> PAGE_SHIFT)
#define PKMAP_ADDR(nr)		(PKMAP_BASE + ((nr) << PAGE_SHIFT))

<<<<<<< HEAD
#define kmap_prot		PAGE_KERNEL

=======
>>>>>>> upstream/android-13
#define flush_cache_kmaps() \
	do { \
		if (cache_is_vivt()) \
			flush_cache_all(); \
	} while (0)

extern pte_t *pkmap_page_table;

<<<<<<< HEAD
extern void *kmap_high(struct page *page);
extern void kunmap_high(struct page *page);

=======
>>>>>>> upstream/android-13
/*
 * The reason for kmap_high_get() is to ensure that the currently kmap'd
 * page usage count does not decrease to zero while we're using its
 * existing virtual mapping in an atomic context.  With a VIVT cache this
 * is essential to do, but with a VIPT cache this is only an optimization
 * so not to pay the price of establishing a second mapping if an existing
 * one can be used.  However, on platforms without hardware TLB maintenance
 * broadcast, we simply cannot use ARCH_NEEDS_KMAP_HIGH_GET at all since
 * the locking involved must also disable IRQs which is incompatible with
 * the IPI mechanism used by global TLB operations.
 */
#define ARCH_NEEDS_KMAP_HIGH_GET
#if defined(CONFIG_SMP) && defined(CONFIG_CPU_TLB_V6)
#undef ARCH_NEEDS_KMAP_HIGH_GET
#if defined(CONFIG_HIGHMEM) && defined(CONFIG_CPU_CACHE_VIVT)
#error "The sum of features in your kernel config cannot be supported together"
#endif
#endif

/*
 * Needed to be able to broadcast the TLB invalidation for kmap.
 */
#ifdef CONFIG_ARM_ERRATA_798181
#undef ARCH_NEEDS_KMAP_HIGH_GET
#endif

#ifdef ARCH_NEEDS_KMAP_HIGH_GET
extern void *kmap_high_get(struct page *page);
<<<<<<< HEAD
#else
=======

static inline void *arch_kmap_local_high_get(struct page *page)
{
	if (IS_ENABLED(CONFIG_DEBUG_HIGHMEM) && !cache_is_vivt())
		return NULL;
	return kmap_high_get(page);
}
#define arch_kmap_local_high_get arch_kmap_local_high_get

#else /* ARCH_NEEDS_KMAP_HIGH_GET */
>>>>>>> upstream/android-13
static inline void *kmap_high_get(struct page *page)
{
	return NULL;
}
<<<<<<< HEAD
#endif

/*
 * The following functions are already defined by <linux/highmem.h>
 * when CONFIG_HIGHMEM is not set.
 */
#ifdef CONFIG_HIGHMEM
extern void *kmap(struct page *page);
extern void kunmap(struct page *page);
extern void *kmap_atomic(struct page *page);
extern void __kunmap_atomic(void *kvaddr);
extern void *kmap_atomic_pfn(unsigned long pfn);
#endif
=======
#endif /* !ARCH_NEEDS_KMAP_HIGH_GET */

#define arch_kmap_local_post_map(vaddr, pteval)				\
	local_flush_tlb_kernel_page(vaddr)

#define arch_kmap_local_pre_unmap(vaddr)				\
do {									\
	if (cache_is_vivt())						\
		__cpuc_flush_dcache_area((void *)vaddr, PAGE_SIZE);	\
} while (0)

#define arch_kmap_local_post_unmap(vaddr)				\
	local_flush_tlb_kernel_page(vaddr)
>>>>>>> upstream/android-13

#endif
