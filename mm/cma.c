<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Contiguous Memory Allocator
 *
 * Copyright (c) 2010-2011 by Samsung Electronics.
 * Copyright IBM Corporation, 2013
 * Copyright LG Electronics Inc., 2014
 * Written by:
 *	Marek Szyprowski <m.szyprowski@samsung.com>
 *	Michal Nazarewicz <mina86@mina86.com>
 *	Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
 *	Joonsoo Kim <iamjoonsoo.kim@lge.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License or (at your optional) any later version of the license.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "cma: " fmt

#ifdef CONFIG_CMA_DEBUG
#ifndef DEBUG
#  define DEBUG
#endif
#endif
#define CREATE_TRACE_POINTS

#include <linux/memblock.h>
#include <linux/err.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/mutex.h>
=======
#include <linux/module.h>
>>>>>>> upstream/android-13
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/log2.h>
#include <linux/cma.h>
#include <linux/highmem.h>
#include <linux/io.h>
#include <linux/kmemleak.h>
<<<<<<< HEAD
=======
#include <linux/sched.h>
#include <linux/jiffies.h>
>>>>>>> upstream/android-13
#include <trace/events/cma.h>

#include "cma.h"

struct cma cma_areas[MAX_CMA_AREAS];
unsigned cma_area_count;
<<<<<<< HEAD
static DEFINE_MUTEX(cma_mutex);
=======
>>>>>>> upstream/android-13

phys_addr_t cma_get_base(const struct cma *cma)
{
	return PFN_PHYS(cma->base_pfn);
}
<<<<<<< HEAD
EXPORT_SYMBOL(cma_get_base);
=======
>>>>>>> upstream/android-13

unsigned long cma_get_size(const struct cma *cma)
{
	return cma->count << PAGE_SHIFT;
}
<<<<<<< HEAD
EXPORT_SYMBOL(cma_get_size);

const char *cma_get_name(const struct cma *cma)
{
	return cma->name ? cma->name : "(undefined)";
=======

const char *cma_get_name(const struct cma *cma)
{
	return cma->name;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(cma_get_name);

static unsigned long cma_bitmap_aligned_mask(const struct cma *cma,
					     unsigned int align_order)
{
	if (align_order <= cma->order_per_bit)
		return 0;
	return (1UL << (align_order - cma->order_per_bit)) - 1;
}

/*
 * Find the offset of the base PFN from the specified align_order.
 * The value returned is represented in order_per_bits.
 */
static unsigned long cma_bitmap_aligned_offset(const struct cma *cma,
					       unsigned int align_order)
{
	return (cma->base_pfn & ((1UL << align_order) - 1))
		>> cma->order_per_bit;
}

static unsigned long cma_bitmap_pages_to_bits(const struct cma *cma,
					      unsigned long pages)
{
	return ALIGN(pages, 1UL << cma->order_per_bit) >> cma->order_per_bit;
}

static void cma_clear_bitmap(struct cma *cma, unsigned long pfn,
<<<<<<< HEAD
			     unsigned int count)
{
	unsigned long bitmap_no, bitmap_count;
=======
			     unsigned long count)
{
	unsigned long bitmap_no, bitmap_count;
	unsigned long flags;
>>>>>>> upstream/android-13

	bitmap_no = (pfn - cma->base_pfn) >> cma->order_per_bit;
	bitmap_count = cma_bitmap_pages_to_bits(cma, count);

<<<<<<< HEAD
	mutex_lock(&cma->lock);
	bitmap_clear(cma->bitmap, bitmap_no, bitmap_count);
	mutex_unlock(&cma->lock);
}

static int __init cma_activate_area(struct cma *cma)
{
	int bitmap_size = BITS_TO_LONGS(cma_bitmap_maxno(cma)) * sizeof(long);
	unsigned long base_pfn = cma->base_pfn, pfn = base_pfn;
	unsigned i = cma->count >> pageblock_order;
	struct zone *zone;

	cma->bitmap = kzalloc(bitmap_size, GFP_KERNEL);

	if (!cma->bitmap) {
		cma->count = 0;
		return -ENOMEM;
	}

	WARN_ON_ONCE(!pfn_valid(pfn));
	zone = page_zone(pfn_to_page(pfn));

	do {
		unsigned j;

		base_pfn = pfn;
		for (j = pageblock_nr_pages; j; --j, pfn++) {
			WARN_ON_ONCE(!pfn_valid(pfn));
			/*
			 * alloc_contig_range requires the pfn range
			 * specified to be in the same zone. Make this
			 * simple by forcing the entire CMA resv range
			 * to be in the same zone.
			 */
			if (page_zone(pfn_to_page(pfn)) != zone)
				goto not_in_zone;
		}
		init_cma_reserved_pageblock(pfn_to_page(base_pfn));
	} while (--i);

	mutex_init(&cma->lock);
=======
	spin_lock_irqsave(&cma->lock, flags);
	bitmap_clear(cma->bitmap, bitmap_no, bitmap_count);
	spin_unlock_irqrestore(&cma->lock, flags);
}

static void __init cma_activate_area(struct cma *cma)
{
	unsigned long base_pfn = cma->base_pfn, pfn;
	struct zone *zone;

	cma->bitmap = bitmap_zalloc(cma_bitmap_maxno(cma), GFP_KERNEL);
	if (!cma->bitmap)
		goto out_error;

	/*
	 * alloc_contig_range() requires the pfn range specified to be in the
	 * same zone. Simplify by forcing the entire CMA resv range to be in the
	 * same zone.
	 */
	WARN_ON_ONCE(!pfn_valid(base_pfn));
	zone = page_zone(pfn_to_page(base_pfn));
	for (pfn = base_pfn + 1; pfn < base_pfn + cma->count; pfn++) {
		WARN_ON_ONCE(!pfn_valid(pfn));
		if (page_zone(pfn_to_page(pfn)) != zone)
			goto not_in_zone;
	}

	for (pfn = base_pfn; pfn < base_pfn + cma->count;
	     pfn += pageblock_nr_pages)
		init_cma_reserved_pageblock(pfn_to_page(pfn));

	spin_lock_init(&cma->lock);
>>>>>>> upstream/android-13

#ifdef CONFIG_CMA_DEBUGFS
	INIT_HLIST_HEAD(&cma->mem_head);
	spin_lock_init(&cma->mem_head_lock);
#endif

<<<<<<< HEAD
	return 0;

not_in_zone:
	pr_err("CMA area %s could not be activated\n", cma->name);
	kfree(cma->bitmap);
	cma->count = 0;
	return -EINVAL;
=======
	return;

not_in_zone:
	bitmap_free(cma->bitmap);
out_error:
	/* Expose all pages to the buddy, they are useless for CMA. */
	for (pfn = base_pfn; pfn < base_pfn + cma->count; pfn++)
		free_reserved_page(pfn_to_page(pfn));
	totalcma_pages -= cma->count;
	cma->count = 0;
	pr_err("CMA area %s could not be activated\n", cma->name);
	return;
>>>>>>> upstream/android-13
}

static int __init cma_init_reserved_areas(void)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < cma_area_count; i++) {
		int ret = cma_activate_area(&cma_areas[i]);

		if (ret)
			return ret;
	}
=======
	for (i = 0; i < cma_area_count; i++)
		cma_activate_area(&cma_areas[i]);
>>>>>>> upstream/android-13

	return 0;
}
core_initcall(cma_init_reserved_areas);

/**
 * cma_init_reserved_mem() - create custom contiguous area from reserved memory
 * @base: Base address of the reserved area
 * @size: Size of the reserved area (in bytes),
 * @order_per_bit: Order of pages represented by one bit on bitmap.
 * @name: The name of the area. If this parameter is NULL, the name of
 *        the area will be set to "cmaN", where N is a running counter of
 *        used areas.
 * @res_cma: Pointer to store the created cma region.
 *
 * This function creates custom contiguous area from already reserved memory.
 */
int __init cma_init_reserved_mem(phys_addr_t base, phys_addr_t size,
				 unsigned int order_per_bit,
				 const char *name,
				 struct cma **res_cma)
{
	struct cma *cma;
	phys_addr_t alignment;

	/* Sanity checks */
	if (cma_area_count == ARRAY_SIZE(cma_areas)) {
		pr_err("Not enough slots for CMA reserved regions!\n");
		return -ENOSPC;
	}

	if (!size || !memblock_is_region_reserved(base, size))
		return -EINVAL;

	/* ensure minimal alignment required by mm core */
	alignment = PAGE_SIZE <<
			max_t(unsigned long, MAX_ORDER - 1, pageblock_order);

	/* alignment should be aligned with order_per_bit */
	if (!IS_ALIGNED(alignment >> PAGE_SHIFT, 1 << order_per_bit))
		return -EINVAL;

	if (ALIGN(base, alignment) != base || ALIGN(size, alignment) != size)
		return -EINVAL;

	/*
	 * Each reserved area must be initialised later, when more kernel
	 * subsystems (like slab allocator) are available.
	 */
	cma = &cma_areas[cma_area_count];
<<<<<<< HEAD
	if (name) {
		cma->name = name;
	} else {
		cma->name = kasprintf(GFP_KERNEL, "cma%d\n", cma_area_count);
		if (!cma->name)
			return -ENOMEM;
	}
=======

	if (name)
		snprintf(cma->name, CMA_MAX_NAME, name);
	else
		snprintf(cma->name, CMA_MAX_NAME,  "cma%d\n", cma_area_count);

>>>>>>> upstream/android-13
	cma->base_pfn = PFN_DOWN(base);
	cma->count = size >> PAGE_SHIFT;
	cma->order_per_bit = order_per_bit;
	*res_cma = cma;
	cma_area_count++;
	totalcma_pages += (size / PAGE_SIZE);

	return 0;
}

/**
<<<<<<< HEAD
 * cma_declare_contiguous() - reserve custom contiguous area
=======
 * cma_declare_contiguous_nid() - reserve custom contiguous area
>>>>>>> upstream/android-13
 * @base: Base address of the reserved area optional, use 0 for any
 * @size: Size of the reserved area (in bytes),
 * @limit: End address of the reserved memory (optional, 0 for any).
 * @alignment: Alignment for the CMA area, should be power of 2 or zero
 * @order_per_bit: Order of pages represented by one bit on bitmap.
 * @fixed: hint about where to place the reserved area
 * @name: The name of the area. See function cma_init_reserved_mem()
 * @res_cma: Pointer to store the created cma region.
<<<<<<< HEAD
=======
 * @nid: nid of the free area to find, %NUMA_NO_NODE for any node
>>>>>>> upstream/android-13
 *
 * This function reserves memory from early allocator. It should be
 * called by arch specific code once the early allocator (memblock or bootmem)
 * has been activated and all other subsystems have already allocated/reserved
 * memory. This function allows to create custom reserved areas.
 *
 * If @fixed is true, reserve contiguous area at exactly @base.  If false,
 * reserve in range from @base to @limit.
 */
<<<<<<< HEAD
int __init cma_declare_contiguous(phys_addr_t base,
			phys_addr_t size, phys_addr_t limit,
			phys_addr_t alignment, unsigned int order_per_bit,
			bool fixed, const char *name, struct cma **res_cma)
=======
int __init cma_declare_contiguous_nid(phys_addr_t base,
			phys_addr_t size, phys_addr_t limit,
			phys_addr_t alignment, unsigned int order_per_bit,
			bool fixed, const char *name, struct cma **res_cma,
			int nid)
>>>>>>> upstream/android-13
{
	phys_addr_t memblock_end = memblock_end_of_DRAM();
	phys_addr_t highmem_start;
	int ret = 0;

	/*
	 * We can't use __pa(high_memory) directly, since high_memory
	 * isn't a valid direct map VA, and DEBUG_VIRTUAL will (validly)
	 * complain. Find the boundary by adding one to the last valid
	 * address.
	 */
	highmem_start = __pa(high_memory - 1) + 1;
	pr_debug("%s(size %pa, base %pa, limit %pa alignment %pa)\n",
		__func__, &size, &base, &limit, &alignment);

	if (cma_area_count == ARRAY_SIZE(cma_areas)) {
		pr_err("Not enough slots for CMA reserved regions!\n");
		return -ENOSPC;
	}

	if (!size)
		return -EINVAL;

	if (alignment && !is_power_of_2(alignment))
		return -EINVAL;

	/*
	 * Sanitise input arguments.
	 * Pages both ends in CMA area could be merged into adjacent unmovable
	 * migratetype page by page allocator's buddy algorithm. In the case,
	 * you couldn't get a contiguous memory, which is not what we want.
	 */
	alignment = max(alignment,  (phys_addr_t)PAGE_SIZE <<
			  max_t(unsigned long, MAX_ORDER - 1, pageblock_order));
	if (fixed && base & (alignment - 1)) {
		ret = -EINVAL;
		pr_err("Region at %pa must be aligned to %pa bytes\n",
			&base, &alignment);
		goto err;
	}
	base = ALIGN(base, alignment);
	size = ALIGN(size, alignment);
	limit &= ~(alignment - 1);

	if (!base)
		fixed = false;

	/* size should be aligned with order_per_bit */
	if (!IS_ALIGNED(size >> PAGE_SHIFT, 1 << order_per_bit))
		return -EINVAL;

	/*
	 * If allocating at a fixed base the request region must not cross the
	 * low/high memory boundary.
	 */
	if (fixed && base < highmem_start && base + size > highmem_start) {
		ret = -EINVAL;
		pr_err("Region at %pa defined on low/high memory boundary (%pa)\n",
			&base, &highmem_start);
		goto err;
	}

	/*
	 * If the limit is unspecified or above the memblock end, its effective
	 * value will be the memblock end. Set it explicitly to simplify further
	 * checks.
	 */
	if (limit == 0 || limit > memblock_end)
		limit = memblock_end;

	if (base + size > limit) {
		ret = -EINVAL;
		pr_err("Size (%pa) of region at %pa exceeds limit (%pa)\n",
			&size, &base, &limit);
		goto err;
	}

	/* Reserve memory */
	if (fixed) {
		if (memblock_is_region_reserved(base, size) ||
		    memblock_reserve(base, size) < 0) {
			ret = -EBUSY;
			goto err;
		}
	} else {
		phys_addr_t addr = 0;

		/*
		 * All pages in the reserved area must come from the same zone.
		 * If the requested region crosses the low/high memory boundary,
		 * try allocating from high memory first and fall back to low
		 * memory in case of failure.
		 */
		if (base < highmem_start && limit > highmem_start) {
<<<<<<< HEAD
			addr = memblock_alloc_range(size, alignment,
						    highmem_start, limit,
						    MEMBLOCK_NONE);
			limit = highmem_start;
		}

		if (!addr) {
			addr = memblock_alloc_range(size, alignment, base,
						    limit,
						    MEMBLOCK_NONE);
=======
			addr = memblock_alloc_range_nid(size, alignment,
					highmem_start, limit, nid, true);
			limit = highmem_start;
		}

		/*
		 * If there is enough memory, try a bottom-up allocation first.
		 * It will place the new cma area close to the start of the node
		 * and guarantee that the compaction is moving pages out of the
		 * cma area and not into it.
		 * Avoid using first 4GB to not interfere with constrained zones
		 * like DMA/DMA32.
		 */
#ifdef CONFIG_PHYS_ADDR_T_64BIT
		if (!memblock_bottom_up() && memblock_end >= SZ_4G + size) {
			memblock_set_bottom_up(true);
			addr = memblock_alloc_range_nid(size, alignment, SZ_4G,
							limit, nid, true);
			memblock_set_bottom_up(false);
		}
#endif

		if (!addr) {
			addr = memblock_alloc_range_nid(size, alignment, base,
					limit, nid, true);
>>>>>>> upstream/android-13
			if (!addr) {
				ret = -ENOMEM;
				goto err;
			}
		}

		/*
		 * kmemleak scans/reads tracked objects for pointers to other
		 * objects but this address isn't mapped and accessible
		 */
		kmemleak_ignore_phys(addr);
		base = addr;
	}

	ret = cma_init_reserved_mem(base, size, order_per_bit, name, res_cma);
	if (ret)
		goto free_mem;

	pr_info("Reserved %ld MiB at %pa\n", (unsigned long)size / SZ_1M,
		&base);
	return 0;

free_mem:
	memblock_free(base, size);
err:
	pr_err("Failed to reserve %ld MiB\n", (unsigned long)size / SZ_1M);
	return ret;
}

#ifdef CONFIG_CMA_DEBUG
static void cma_debug_show_areas(struct cma *cma)
{
	unsigned long next_zero_bit, next_set_bit, nr_zero;
	unsigned long start = 0;
	unsigned long nr_part, nr_total = 0;
	unsigned long nbits = cma_bitmap_maxno(cma);

<<<<<<< HEAD
	mutex_lock(&cma->lock);
=======
	spin_lock_irq(&cma->lock);
>>>>>>> upstream/android-13
	pr_info("number of available pages: ");
	for (;;) {
		next_zero_bit = find_next_zero_bit(cma->bitmap, nbits, start);
		if (next_zero_bit >= nbits)
			break;
		next_set_bit = find_next_bit(cma->bitmap, nbits, next_zero_bit);
		nr_zero = next_set_bit - next_zero_bit;
		nr_part = nr_zero << cma->order_per_bit;
		pr_cont("%s%lu@%lu", nr_total ? "+" : "", nr_part,
			next_zero_bit);
		nr_total += nr_part;
		start = next_zero_bit + nr_zero;
	}
	pr_cont("=> %lu free of %lu total pages\n", nr_total, cma->count);
<<<<<<< HEAD
	mutex_unlock(&cma->lock);
=======
	spin_unlock_irq(&cma->lock);
>>>>>>> upstream/android-13
}
#else
static inline void cma_debug_show_areas(struct cma *cma) { }
#endif

/**
 * cma_alloc() - allocate pages from contiguous area
 * @cma:   Contiguous memory region for which the allocation is performed.
 * @count: Requested number of pages.
 * @align: Requested alignment of pages (in PAGE_SIZE order).
 * @no_warn: Avoid printing message about failed allocation
 *
 * This function allocates part of contiguous memory on specific
 * contiguous memory area.
 */
<<<<<<< HEAD
struct page *cma_alloc(struct cma *cma, size_t count, unsigned int align,
		       bool no_warn)
=======
struct page *cma_alloc(struct cma *cma, unsigned long count,
		       unsigned int align, bool no_warn)
>>>>>>> upstream/android-13
{
	unsigned long mask, offset;
	unsigned long pfn = -1;
	unsigned long start = 0;
	unsigned long bitmap_maxno, bitmap_no, bitmap_count;
<<<<<<< HEAD
	size_t i;
	struct page *page = NULL;
	int ret = -ENOMEM;

	if (!cma || !cma->count)
		return NULL;

	pr_debug("%s(cma %p, count %zu, align %d)\n", __func__, (void *)cma,
		 count, align);

	if (!count)
		return NULL;
=======
	unsigned long i;
	struct page *page = NULL;
	int ret = -ENOMEM;
	int num_attempts = 0;
	int max_retries = 5;

	if (!cma || !cma->count || !cma->bitmap)
		goto out;

	pr_debug("%s(cma %p, count %lu, align %d)\n", __func__, (void *)cma,
		 count, align);

	if (!count)
		goto out;

	trace_cma_alloc_start(cma->name, count, align);
>>>>>>> upstream/android-13

	mask = cma_bitmap_aligned_mask(cma, align);
	offset = cma_bitmap_aligned_offset(cma, align);
	bitmap_maxno = cma_bitmap_maxno(cma);
	bitmap_count = cma_bitmap_pages_to_bits(cma, count);

	if (bitmap_count > bitmap_maxno)
<<<<<<< HEAD
		return NULL;

	for (;;) {
		mutex_lock(&cma->lock);
=======
		goto out;

	for (;;) {
		spin_lock_irq(&cma->lock);
>>>>>>> upstream/android-13
		bitmap_no = bitmap_find_next_zero_area_off(cma->bitmap,
				bitmap_maxno, start, bitmap_count, mask,
				offset);
		if (bitmap_no >= bitmap_maxno) {
<<<<<<< HEAD
			mutex_unlock(&cma->lock);
			break;
=======
			if ((num_attempts < max_retries) && (ret == -EBUSY)) {
				spin_unlock_irq(&cma->lock);

				if (fatal_signal_pending(current))
					break;

				/*
				 * Page may be momentarily pinned by some other
				 * process which has been scheduled out, e.g.
				 * in exit path, during unmap call, or process
				 * fork and so cannot be freed there. Sleep
				 * for 100ms and retry the allocation.
				 */
				start = 0;
				ret = -ENOMEM;
				schedule_timeout_killable(msecs_to_jiffies(100));
				num_attempts++;
				continue;
			} else {
				spin_unlock_irq(&cma->lock);
				break;
			}
>>>>>>> upstream/android-13
		}
		bitmap_set(cma->bitmap, bitmap_no, bitmap_count);
		/*
		 * It's safe to drop the lock here. We've marked this region for
		 * our exclusive use. If the migration fails we will take the
		 * lock again and unmark it.
		 */
<<<<<<< HEAD
		mutex_unlock(&cma->lock);

		pfn = cma->base_pfn + (bitmap_no << cma->order_per_bit);
		mutex_lock(&cma_mutex);
		ret = alloc_contig_range(pfn, pfn + count, MIGRATE_CMA,
				     GFP_KERNEL | (no_warn ? __GFP_NOWARN : 0));
		mutex_unlock(&cma_mutex);
=======
		spin_unlock_irq(&cma->lock);

		pfn = cma->base_pfn + (bitmap_no << cma->order_per_bit);
		ret = alloc_contig_range(pfn, pfn + count, MIGRATE_CMA,
				     GFP_KERNEL | (no_warn ? __GFP_NOWARN : 0));

>>>>>>> upstream/android-13
		if (ret == 0) {
			page = pfn_to_page(pfn);
			break;
		}

		cma_clear_bitmap(cma, pfn, count);
		if (ret != -EBUSY)
			break;

		pr_debug("%s(): memory range at %p is busy, retrying\n",
			 __func__, pfn_to_page(pfn));
<<<<<<< HEAD
=======

		trace_cma_alloc_busy_retry(cma->name, pfn, pfn_to_page(pfn),
					   count, align);
>>>>>>> upstream/android-13
		/* try again with a bit different memory target */
		start = bitmap_no + mask + 1;
	}

<<<<<<< HEAD
	trace_cma_alloc(pfn, page, count, align);
=======
	trace_cma_alloc_finish(cma->name, pfn, page, count, align);
>>>>>>> upstream/android-13

	/*
	 * CMA can allocate multiple page blocks, which results in different
	 * blocks being marked with different tags. Reset the tags to ignore
	 * those page blocks.
	 */
	if (page) {
		for (i = 0; i < count; i++)
			page_kasan_tag_reset(page + i);
	}

	if (ret && !no_warn) {
<<<<<<< HEAD
		pr_err("%s: alloc failed, req-size: %zu pages, ret: %d\n",
			__func__, count, ret);
=======
		pr_err_ratelimited("%s: %s: alloc failed, req-size: %lu pages, ret: %d\n",
				   __func__, cma->name, count, ret);
>>>>>>> upstream/android-13
		cma_debug_show_areas(cma);
	}

	pr_debug("%s(): returned %p\n", __func__, page);
<<<<<<< HEAD
=======
out:
	if (page) {
		count_vm_event(CMA_ALLOC_SUCCESS);
		cma_sysfs_account_success_pages(cma, count);
	} else {
		count_vm_event(CMA_ALLOC_FAIL);
		if (cma)
			cma_sysfs_account_fail_pages(cma, count);
	}

>>>>>>> upstream/android-13
	return page;
}
EXPORT_SYMBOL_GPL(cma_alloc);

/**
 * cma_release() - release allocated pages
 * @cma:   Contiguous memory region for which the allocation is performed.
 * @pages: Allocated pages.
 * @count: Number of allocated pages.
 *
<<<<<<< HEAD
 * This function releases memory allocated by alloc_cma().
 * It returns false when provided pages do not belong to contiguous area and
 * true otherwise.
 */
bool cma_release(struct cma *cma, const struct page *pages, unsigned int count)
=======
 * This function releases memory allocated by cma_alloc().
 * It returns false when provided pages do not belong to contiguous area and
 * true otherwise.
 */
bool cma_release(struct cma *cma, const struct page *pages,
		 unsigned long count)
>>>>>>> upstream/android-13
{
	unsigned long pfn;

	if (!cma || !pages)
		return false;

<<<<<<< HEAD
	pr_debug("%s(page %p)\n", __func__, (void *)pages);
=======
	pr_debug("%s(page %p, count %lu)\n", __func__, (void *)pages, count);
>>>>>>> upstream/android-13

	pfn = page_to_pfn(pages);

	if (pfn < cma->base_pfn || pfn >= cma->base_pfn + cma->count)
		return false;

	VM_BUG_ON(pfn + count > cma->base_pfn + cma->count);

	free_contig_range(pfn, count);
	cma_clear_bitmap(cma, pfn, count);
<<<<<<< HEAD
	trace_cma_release(pfn, pages, count);
=======
	trace_cma_release(cma->name, pfn, pages, count);
>>>>>>> upstream/android-13

	return true;
}
EXPORT_SYMBOL_GPL(cma_release);

int cma_for_each_area(int (*it)(struct cma *cma, void *data), void *data)
{
	int i;

	for (i = 0; i < cma_area_count; i++) {
		int ret = it(&cma_areas[i], data);

		if (ret)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(cma_for_each_area);
