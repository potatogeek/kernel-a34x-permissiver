// SPDX-License-Identifier: GPL-2.0
/*
 * linux/mm/page_isolation.c
 */

#include <linux/mm.h>
#include <linux/page-isolation.h>
#include <linux/pageblock-flags.h>
#include <linux/memory.h>
#include <linux/hugetlb.h>
#include <linux/page_owner.h>
#include <linux/migrate.h>
#include "internal.h"

#define CREATE_TRACE_POINTS
#include <trace/events/page_isolation.h>

<<<<<<< HEAD
static int set_migratetype_isolate(struct page *page, int migratetype,
				bool skip_hwpoisoned_pages)
{
	struct zone *zone;
	unsigned long flags, pfn;
	struct memory_isolate_notify arg;
	int notifier_ret;
	int ret = -EBUSY;

	zone = page_zone(page);
=======
static int set_migratetype_isolate(struct page *page, int migratetype, int isol_flags)
{
	struct zone *zone = page_zone(page);
	struct page *unmovable = NULL;
	unsigned long flags;
>>>>>>> upstream/android-13

	spin_lock_irqsave(&zone->lock, flags);

	/*
	 * We assume the caller intended to SET migrate type to isolate.
	 * If it is already set, then someone else must have raced and
<<<<<<< HEAD
	 * set it before us.  Return -EBUSY
	 */
	if (is_migrate_isolate_page(page))
		goto out;

	pfn = page_to_pfn(page);
	arg.start_pfn = pfn;
	arg.nr_pages = pageblock_nr_pages;
	arg.pages_found = 0;

	/*
	 * It may be possible to isolate a pageblock even if the
	 * migratetype is not MIGRATE_MOVABLE. The memory isolation
	 * notifier chain is used by balloon drivers to return the
	 * number of pages in a range that are held by the balloon
	 * driver to shrink memory. If all the pages are accounted for
	 * by balloons, are free, or on the LRU, isolation can continue.
	 * Later, for example, when memory hotplug notifier runs, these
	 * pages reported as "can be isolated" should be isolated(freed)
	 * by the balloon driver through the memory notifier chain.
	 */
	notifier_ret = memory_isolate_notify(MEM_ISOLATE_COUNT, &arg);
	notifier_ret = notifier_to_errno(notifier_ret);
	if (notifier_ret)
		goto out;
=======
	 * set it before us.
	 */
	if (is_migrate_isolate_page(page)) {
		spin_unlock_irqrestore(&zone->lock, flags);
		return -EBUSY;
	}

#if !defined(CONFIG_HPA)
>>>>>>> upstream/android-13
	/*
	 * FIXME: Now, memory hotplug doesn't call shrink_slab() by itself.
	 * We just check MOVABLE pages.
	 */
<<<<<<< HEAD
	if (!has_unmovable_pages(zone, page, arg.pages_found, migratetype,
				 skip_hwpoisoned_pages))
		ret = 0;

	/*
	 * immobile means "not-on-lru" pages. If immobile is larger than
	 * removable-by-driver pages reported by notifier, we'll fail.
	 */

out:
	if (!ret) {
=======
	unmovable = has_unmovable_pages(zone, page, migratetype, isol_flags);
#endif
	if (!unmovable) {
>>>>>>> upstream/android-13
		unsigned long nr_pages;
		int mt = get_pageblock_migratetype(page);

		set_pageblock_migratetype(page, MIGRATE_ISOLATE);
		zone->nr_isolate_pageblock++;
		nr_pages = move_freepages_block(zone, page, MIGRATE_ISOLATE,
									NULL);

		__mod_zone_freepage_state(zone, -nr_pages, mt);
<<<<<<< HEAD
	}

	spin_unlock_irqrestore(&zone->lock, flags);
	if (!ret)
		drain_all_pages(zone);
	return ret;
=======
		spin_unlock_irqrestore(&zone->lock, flags);
		return 0;
	}

	spin_unlock_irqrestore(&zone->lock, flags);
	if (isol_flags & REPORT_FAILURE) {
		/*
		 * printk() with zone->lock held will likely trigger a
		 * lockdep splat, so defer it here.
		 */
		dump_page(unmovable, "unmovable page");
	}

	return -EBUSY;
>>>>>>> upstream/android-13
}

static void unset_migratetype_isolate(struct page *page, unsigned migratetype)
{
	struct zone *zone;
	unsigned long flags, nr_pages;
	bool isolated_page = false;
	unsigned int order;
	unsigned long pfn, buddy_pfn;
	struct page *buddy;

	zone = page_zone(page);
	spin_lock_irqsave(&zone->lock, flags);
	if (!is_migrate_isolate_page(page))
		goto out;

	/*
	 * Because freepage with more than pageblock_order on isolated
	 * pageblock is restricted to merge due to freepage counting problem,
	 * it is possible that there is free buddy page.
	 * move_freepages_block() doesn't care of merge so we need other
	 * approach in order to merge them. Isolation and free will make
	 * these pages to be merged.
	 */
	if (PageBuddy(page)) {
<<<<<<< HEAD
		order = page_order(page);
		if (order >= pageblock_order) {
=======
		order = buddy_order(page);
		if (order >= pageblock_order && order < MAX_ORDER - 1) {
>>>>>>> upstream/android-13
			pfn = page_to_pfn(page);
			buddy_pfn = __find_buddy_pfn(pfn, order);
			buddy = page + (buddy_pfn - pfn);

<<<<<<< HEAD
			if (pfn_valid_within(buddy_pfn) &&
			    !is_migrate_isolate_page(buddy)) {
=======
			if (!is_migrate_isolate_page(buddy)) {
>>>>>>> upstream/android-13
				__isolate_free_page(page, order);
				isolated_page = true;
			}
		}
	}

	/*
	 * If we isolate freepage with more than pageblock_order, there
	 * should be no freepage in the range, so we could avoid costly
	 * pageblock scanning for freepage moving.
<<<<<<< HEAD
=======
	 *
	 * We didn't actually touch any of the isolated pages, so place them
	 * to the tail of the freelist. This is an optimization for memory
	 * onlining - just onlined memory won't immediately be considered for
	 * allocation.
>>>>>>> upstream/android-13
	 */
	if (!isolated_page) {
		nr_pages = move_freepages_block(zone, page, migratetype, NULL);
		__mod_zone_freepage_state(zone, nr_pages, migratetype);
	}
	set_pageblock_migratetype(page, migratetype);
<<<<<<< HEAD
	zone->nr_isolate_pageblock--;
out:
	spin_unlock_irqrestore(&zone->lock, flags);
	if (isolated_page) {
		post_alloc_hook(page, order, __GFP_MOVABLE);
		__free_pages(page, order);
	}
=======
	if (isolated_page)
		__putback_isolated_page(page, order, migratetype);
	zone->nr_isolate_pageblock--;
out:
	spin_unlock_irqrestore(&zone->lock, flags);
>>>>>>> upstream/android-13
}

static inline struct page *
__first_valid_page(unsigned long pfn, unsigned long nr_pages)
{
	int i;

	for (i = 0; i < nr_pages; i++) {
		struct page *page;

<<<<<<< HEAD
		if (!pfn_valid_within(pfn + i))
			continue;
=======
>>>>>>> upstream/android-13
		page = pfn_to_online_page(pfn + i);
		if (!page)
			continue;
		return page;
	}
	return NULL;
}

<<<<<<< HEAD
/*
 * start_isolate_page_range() -- make page-allocation-type of range of pages
 * to be MIGRATE_ISOLATE.
 * @start_pfn: The lower PFN of the range to be isolated.
 * @end_pfn: The upper PFN of the range to be isolated.
 * @migratetype: migrate type to set in error recovery.
 *
 * Making page-allocation-type to be MIGRATE_ISOLATE means free pages in
 * the range will never be allocated. Any free pages and pages freed in the
 * future will not be allocated again.
 *
 * start_pfn/end_pfn must be aligned to pageblock_order.
 * Return 0 on success and -EBUSY if any part of range cannot be isolated.
 *
 * There is no high level synchronization mechanism that prevents two threads
 * from trying to isolate overlapping ranges.  If this happens, one thread
 * will notice pageblocks in the overlapping range already set to isolate.
 * This happens in set_migratetype_isolate, and set_migratetype_isolate
 * returns an error.  We then clean up by restoring the migration type on
 * pageblocks we may have modified and return -EBUSY to caller.  This
 * prevents two threads from simultaneously working on overlapping ranges.
 */
int start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			     unsigned migratetype, bool skip_hwpoisoned_pages)
=======
/**
 * start_isolate_page_range() - make page-allocation-type of range of pages to
 * be MIGRATE_ISOLATE.
 * @start_pfn:		The lower PFN of the range to be isolated.
 * @end_pfn:		The upper PFN of the range to be isolated.
 *			start_pfn/end_pfn must be aligned to pageblock_order.
 * @migratetype:	Migrate type to set in error recovery.
 * @flags:		The following flags are allowed (they can be combined in
 *			a bit mask)
 *			MEMORY_OFFLINE - isolate to offline (!allocate) memory
 *					 e.g., skip over PageHWPoison() pages
 *					 and PageOffline() pages.
 *			REPORT_FAILURE - report details about the failure to
 *			isolate the range
 *
 * Making page-allocation-type to be MIGRATE_ISOLATE means free pages in
 * the range will never be allocated. Any free pages and pages freed in the
 * future will not be allocated again. If specified range includes migrate types
 * other than MOVABLE or CMA, this will fail with -EBUSY. For isolating all
 * pages in the range finally, the caller have to free all pages in the range.
 * test_page_isolated() can be used for test it.
 *
 * There is no high level synchronization mechanism that prevents two threads
 * from trying to isolate overlapping ranges. If this happens, one thread
 * will notice pageblocks in the overlapping range already set to isolate.
 * This happens in set_migratetype_isolate, and set_migratetype_isolate
 * returns an error. We then clean up by restoring the migration type on
 * pageblocks we may have modified and return -EBUSY to caller. This
 * prevents two threads from simultaneously working on overlapping ranges.
 *
 * Please note that there is no strong synchronization with the page allocator
 * either. Pages might be freed while their page blocks are marked ISOLATED.
 * A call to drain_all_pages() after isolation can flush most of them. However
 * in some cases pages might still end up on pcp lists and that would allow
 * for their allocation even when they are in fact isolated already. Depending
 * on how strong of a guarantee the caller needs, zone_pcp_disable/enable()
 * might be used to flush and disable pcplist before isolation and enable after
 * unisolation.
 *
 * Return: 0 on success and -EBUSY if any part of range cannot be isolated.
 */
int start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			     unsigned migratetype, int flags)
>>>>>>> upstream/android-13
{
	unsigned long pfn;
	unsigned long undo_pfn;
	struct page *page;

	BUG_ON(!IS_ALIGNED(start_pfn, pageblock_nr_pages));
	BUG_ON(!IS_ALIGNED(end_pfn, pageblock_nr_pages));

	for (pfn = start_pfn;
	     pfn < end_pfn;
	     pfn += pageblock_nr_pages) {
		page = __first_valid_page(pfn, pageblock_nr_pages);
<<<<<<< HEAD
		if (page &&
		    set_migratetype_isolate(page, migratetype, skip_hwpoisoned_pages)) {
			undo_pfn = pfn;
			goto undo;
=======
		if (page) {
			if (set_migratetype_isolate(page, migratetype, flags)) {
				undo_pfn = pfn;
				goto undo;
			}
>>>>>>> upstream/android-13
		}
	}
	return 0;
undo:
	for (pfn = start_pfn;
	     pfn < undo_pfn;
	     pfn += pageblock_nr_pages) {
		struct page *page = pfn_to_online_page(pfn);
		if (!page)
			continue;
		unset_migratetype_isolate(page, migratetype);
	}

	return -EBUSY;
}

/*
 * Make isolated pages available again.
 */
<<<<<<< HEAD
int undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
=======
void undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
>>>>>>> upstream/android-13
			    unsigned migratetype)
{
	unsigned long pfn;
	struct page *page;

	BUG_ON(!IS_ALIGNED(start_pfn, pageblock_nr_pages));
	BUG_ON(!IS_ALIGNED(end_pfn, pageblock_nr_pages));

	for (pfn = start_pfn;
	     pfn < end_pfn;
	     pfn += pageblock_nr_pages) {
		page = __first_valid_page(pfn, pageblock_nr_pages);
		if (!page || !is_migrate_isolate_page(page))
			continue;
		unset_migratetype_isolate(page, migratetype);
	}
<<<<<<< HEAD
	return 0;
=======
>>>>>>> upstream/android-13
}
/*
 * Test all pages in the range is free(means isolated) or not.
 * all pages in [start_pfn...end_pfn) must be in the same zone.
 * zone->lock must be held before call this.
 *
 * Returns the last tested pfn.
 */
static unsigned long
__test_page_isolated_in_pageblock(unsigned long pfn, unsigned long end_pfn,
<<<<<<< HEAD
				  bool skip_hwpoisoned_pages)
=======
				  int flags)
>>>>>>> upstream/android-13
{
	struct page *page;

	while (pfn < end_pfn) {
<<<<<<< HEAD
		if (!pfn_valid_within(pfn)) {
			pfn++;
			continue;
		}
=======
>>>>>>> upstream/android-13
		page = pfn_to_page(pfn);
		if (PageBuddy(page))
			/*
			 * If the page is on a free list, it has to be on
			 * the correct MIGRATE_ISOLATE freelist. There is no
			 * simple way to verify that as VM_BUG_ON(), though.
			 */
<<<<<<< HEAD
			pfn += 1 << page_order(page);
		else if (skip_hwpoisoned_pages && PageHWPoison(page))
			/* A HWPoisoned page cannot be also PageBuddy */
			pfn++;
=======
			pfn += 1 << buddy_order(page);
		else if ((flags & MEMORY_OFFLINE) && PageHWPoison(page))
			/* A HWPoisoned page cannot be also PageBuddy */
			pfn++;
		else if ((flags & MEMORY_OFFLINE) && PageOffline(page) &&
			 !page_count(page))
			/*
			 * The responsible driver agreed to skip PageOffline()
			 * pages when offlining memory by dropping its
			 * reference in MEM_GOING_OFFLINE.
			 */
			pfn++;
>>>>>>> upstream/android-13
		else
			break;
	}

	return pfn;
}

/* Caller should ensure that requested range is in a single zone */
int test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn,
<<<<<<< HEAD
			bool skip_hwpoisoned_pages)
=======
			int isol_flags)
>>>>>>> upstream/android-13
{
	unsigned long pfn, flags;
	struct page *page;
	struct zone *zone;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> upstream/android-13

	/*
	 * Note: pageblock_nr_pages != MAX_ORDER. Then, chunks of free pages
	 * are not aligned to pageblock_nr_pages.
	 * Then we just check migratetype first.
	 */
	for (pfn = start_pfn; pfn < end_pfn; pfn += pageblock_nr_pages) {
		page = __first_valid_page(pfn, pageblock_nr_pages);
		if (page && !is_migrate_isolate_page(page))
			break;
	}
	page = __first_valid_page(start_pfn, end_pfn - start_pfn);
<<<<<<< HEAD
	if ((pfn < end_pfn) || !page)
		return -EBUSY;
	/* Check all pages are free or marked as ISOLATED */
	zone = page_zone(page);
	spin_lock_irqsave(&zone->lock, flags);
	pfn = __test_page_isolated_in_pageblock(start_pfn, end_pfn,
						skip_hwpoisoned_pages);
	spin_unlock_irqrestore(&zone->lock, flags);

	trace_test_pages_isolated(start_pfn, end_pfn, pfn);

	return pfn < end_pfn ? -EBUSY : 0;
}

struct page *alloc_migrate_target(struct page *page, unsigned long private)
{
	return new_page_nodemask(page, numa_node_id(), &node_states[N_MEMORY]);
=======
	if ((pfn < end_pfn) || !page) {
		ret = -EBUSY;
		goto out;
	}

	/* Check all pages are free or marked as ISOLATED */
	zone = page_zone(page);
	spin_lock_irqsave(&zone->lock, flags);
	pfn = __test_page_isolated_in_pageblock(start_pfn, end_pfn, isol_flags);
	spin_unlock_irqrestore(&zone->lock, flags);

	ret = pfn < end_pfn ? -EBUSY : 0;

out:
	trace_test_pages_isolated(start_pfn, end_pfn, pfn);

	return ret;
>>>>>>> upstream/android-13
}
