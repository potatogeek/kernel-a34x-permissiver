<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Generic hugetlb support.
 * (C) Nadia Yvette Chambers, April 2004
 */
#include <linux/list.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/seq_file.h>
#include <linux/sysctl.h>
#include <linux/highmem.h>
#include <linux/mmu_notifier.h>
#include <linux/nodemask.h>
#include <linux/pagemap.h>
#include <linux/mempolicy.h>
#include <linux/compiler.h>
#include <linux/cpuset.h>
#include <linux/mutex.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
=======
#include <linux/memblock.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/sched/mm.h>
>>>>>>> upstream/android-13
#include <linux/mmdebug.h>
#include <linux/sched/signal.h>
#include <linux/rmap.h>
#include <linux/string_helpers.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <linux/jhash.h>
<<<<<<< HEAD

#include <asm/page.h>
#include <asm/pgtable.h>
=======
#include <linux/numa.h>
#include <linux/llist.h>
#include <linux/cma.h>
#include <linux/migrate.h>

#include <asm/page.h>
#include <asm/pgalloc.h>
>>>>>>> upstream/android-13
#include <asm/tlb.h>

#include <linux/io.h>
#include <linux/hugetlb.h>
#include <linux/hugetlb_cgroup.h>
#include <linux/node.h>
<<<<<<< HEAD
#include <linux/userfaultfd_k.h>
#include <linux/page_owner.h>
#include "internal.h"
=======
#include <linux/page_owner.h>
#include "internal.h"
#include "hugetlb_vmemmap.h"
>>>>>>> upstream/android-13

int hugetlb_max_hstate __read_mostly;
unsigned int default_hstate_idx;
struct hstate hstates[HUGE_MAX_HSTATE];
<<<<<<< HEAD
=======

#ifdef CONFIG_CMA
static struct cma *hugetlb_cma[MAX_NUMNODES];
#endif
static unsigned long hugetlb_cma_size __initdata;

>>>>>>> upstream/android-13
/*
 * Minimum page order among possible hugepage sizes, set to a proper value
 * at boot time.
 */
static unsigned int minimum_order __read_mostly = UINT_MAX;

__initdata LIST_HEAD(huge_boot_pages);

/* for command line parsing */
static struct hstate * __initdata parsed_hstate;
static unsigned long __initdata default_hstate_max_huge_pages;
<<<<<<< HEAD
static unsigned long __initdata default_hstate_size;
static bool __initdata parsed_valid_hugepagesz = true;
=======
static bool __initdata parsed_valid_hugepagesz = true;
static bool __initdata parsed_default_hugepagesz;
>>>>>>> upstream/android-13

/*
 * Protects updates to hugepage_freelists, hugepage_activelist, nr_huge_pages,
 * free_huge_pages, and surplus_huge_pages.
 */
DEFINE_SPINLOCK(hugetlb_lock);

/*
 * Serializes faults on the same logical page.  This is used to
 * prevent spurious OOMs when the hugepage pool is fully utilized.
 */
static int num_fault_mutexes;
struct mutex *hugetlb_fault_mutex_table ____cacheline_aligned_in_smp;

<<<<<<< HEAD
static inline bool PageHugeFreed(struct page *head)
{
	return page_private(head + 4) == -1UL;
}

static inline void SetPageHugeFreed(struct page *head)
{
	set_page_private(head + 4, -1UL);
}

static inline void ClearPageHugeFreed(struct page *head)
{
	set_page_private(head + 4, 0);
}

/* Forward declaration */
static int hugetlb_acct_memory(struct hstate *h, long delta);

static inline void unlock_or_release_subpool(struct hugepage_subpool *spool)
{
	bool free = (spool->count == 0) && (spool->used_hpages == 0);

	spin_unlock(&spool->lock);

	/* If no pages are used, and no other handles to the subpool
	 * remain, give up any reservations mased on minimum size and
	 * free the subpool */
	if (free) {
=======
/* Forward declaration */
static int hugetlb_acct_memory(struct hstate *h, long delta);

static inline bool subpool_is_free(struct hugepage_subpool *spool)
{
	if (spool->count)
		return false;
	if (spool->max_hpages != -1)
		return spool->used_hpages == 0;
	if (spool->min_hpages != -1)
		return spool->rsv_hpages == spool->min_hpages;

	return true;
}

static inline void unlock_or_release_subpool(struct hugepage_subpool *spool,
						unsigned long irq_flags)
{
	spin_unlock_irqrestore(&spool->lock, irq_flags);

	/* If no pages are used, and no other handles to the subpool
	 * remain, give up any reservations based on minimum size and
	 * free the subpool */
	if (subpool_is_free(spool)) {
>>>>>>> upstream/android-13
		if (spool->min_hpages != -1)
			hugetlb_acct_memory(spool->hstate,
						-spool->min_hpages);
		kfree(spool);
	}
}

struct hugepage_subpool *hugepage_new_subpool(struct hstate *h, long max_hpages,
						long min_hpages)
{
	struct hugepage_subpool *spool;

	spool = kzalloc(sizeof(*spool), GFP_KERNEL);
	if (!spool)
		return NULL;

	spin_lock_init(&spool->lock);
	spool->count = 1;
	spool->max_hpages = max_hpages;
	spool->hstate = h;
	spool->min_hpages = min_hpages;

	if (min_hpages != -1 && hugetlb_acct_memory(h, min_hpages)) {
		kfree(spool);
		return NULL;
	}
	spool->rsv_hpages = min_hpages;

	return spool;
}

void hugepage_put_subpool(struct hugepage_subpool *spool)
{
<<<<<<< HEAD
	spin_lock(&spool->lock);
	BUG_ON(!spool->count);
	spool->count--;
	unlock_or_release_subpool(spool);
=======
	unsigned long flags;

	spin_lock_irqsave(&spool->lock, flags);
	BUG_ON(!spool->count);
	spool->count--;
	unlock_or_release_subpool(spool, flags);
>>>>>>> upstream/android-13
}

/*
 * Subpool accounting for allocating and reserving pages.
 * Return -ENOMEM if there are not enough resources to satisfy the
<<<<<<< HEAD
 * the request.  Otherwise, return the number of pages by which the
 * global pools must be adjusted (upward).  The returned value may
 * only be different than the passed value (delta) in the case where
 * a subpool minimum size must be manitained.
=======
 * request.  Otherwise, return the number of pages by which the
 * global pools must be adjusted (upward).  The returned value may
 * only be different than the passed value (delta) in the case where
 * a subpool minimum size must be maintained.
>>>>>>> upstream/android-13
 */
static long hugepage_subpool_get_pages(struct hugepage_subpool *spool,
				      long delta)
{
	long ret = delta;

	if (!spool)
		return ret;

<<<<<<< HEAD
	spin_lock(&spool->lock);
=======
	spin_lock_irq(&spool->lock);
>>>>>>> upstream/android-13

	if (spool->max_hpages != -1) {		/* maximum size accounting */
		if ((spool->used_hpages + delta) <= spool->max_hpages)
			spool->used_hpages += delta;
		else {
			ret = -ENOMEM;
			goto unlock_ret;
		}
	}

	/* minimum size accounting */
	if (spool->min_hpages != -1 && spool->rsv_hpages) {
		if (delta > spool->rsv_hpages) {
			/*
			 * Asking for more reserves than those already taken on
			 * behalf of subpool.  Return difference.
			 */
			ret = delta - spool->rsv_hpages;
			spool->rsv_hpages = 0;
		} else {
			ret = 0;	/* reserves already accounted for */
			spool->rsv_hpages -= delta;
		}
	}

unlock_ret:
<<<<<<< HEAD
	spin_unlock(&spool->lock);
=======
	spin_unlock_irq(&spool->lock);
>>>>>>> upstream/android-13
	return ret;
}

/*
 * Subpool accounting for freeing and unreserving pages.
 * Return the number of global page reservations that must be dropped.
 * The return value may only be different than the passed value (delta)
 * in the case where a subpool minimum size must be maintained.
 */
static long hugepage_subpool_put_pages(struct hugepage_subpool *spool,
				       long delta)
{
	long ret = delta;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> upstream/android-13

	if (!spool)
		return delta;

<<<<<<< HEAD
	spin_lock(&spool->lock);
=======
	spin_lock_irqsave(&spool->lock, flags);
>>>>>>> upstream/android-13

	if (spool->max_hpages != -1)		/* maximum size accounting */
		spool->used_hpages -= delta;

	 /* minimum size accounting */
	if (spool->min_hpages != -1 && spool->used_hpages < spool->min_hpages) {
		if (spool->rsv_hpages + delta <= spool->min_hpages)
			ret = 0;
		else
			ret = spool->rsv_hpages + delta - spool->min_hpages;

		spool->rsv_hpages += delta;
		if (spool->rsv_hpages > spool->min_hpages)
			spool->rsv_hpages = spool->min_hpages;
	}

	/*
	 * If hugetlbfs_put_super couldn't free spool due to an outstanding
	 * quota reference, free it now.
	 */
<<<<<<< HEAD
	unlock_or_release_subpool(spool);
=======
	unlock_or_release_subpool(spool, flags);
>>>>>>> upstream/android-13

	return ret;
}

static inline struct hugepage_subpool *subpool_inode(struct inode *inode)
{
	return HUGETLBFS_SB(inode->i_sb)->spool;
}

static inline struct hugepage_subpool *subpool_vma(struct vm_area_struct *vma)
{
	return subpool_inode(file_inode(vma->vm_file));
}

<<<<<<< HEAD
/*
 * Region tracking -- allows tracking of reservations and instantiated pages
 *                    across the pages in a mapping.
 *
 * The region data structures are embedded into a resv_map and protected
 * by a resv_map's lock.  The set of regions within the resv_map represent
 * reservations for huge pages, or huge pages that have already been
 * instantiated within the map.  The from and to elements are huge page
 * indicies into the associated mapping.  from indicates the starting index
 * of the region.  to represents the first index past the end of  the region.
 *
 * For example, a file region structure with from == 0 and to == 4 represents
 * four huge pages in a mapping.  It is important to note that the to element
 * represents the first element past the end of the region. This is used in
 * arithmetic as 4(to) - 0(from) = 4 huge pages in the region.
 *
 * Interval notation of the form [from, to) will be used to indicate that
 * the endpoint from is inclusive and to is exclusive.
 */
struct file_region {
	struct list_head link;
	long from;
	long to;
};

/*
 * Add the huge page range represented by [f, t) to the reserve
 * map.  In the normal case, existing regions will be expanded
 * to accommodate the specified range.  Sufficient regions should
 * exist for expansion due to the previous call to region_chg
 * with the same range.  However, it is possible that region_del
 * could have been called after region_chg and modifed the map
 * in such a way that no region exists to be expanded.  In this
 * case, pull a region descriptor from the cache associated with
 * the map and use that for the new range.
 *
 * Return the number of new huge pages added to the map.  This
 * number is greater than or equal to zero.
 */
static long region_add(struct resv_map *resv, long f, long t)
{
	struct list_head *head = &resv->regions;
	struct file_region *rg, *nrg, *trg;
	long add = 0;

	spin_lock(&resv->lock);
	/* Locate the region we are either in or before. */
	list_for_each_entry(rg, head, link)
		if (f <= rg->to)
			break;

	/*
	 * If no region exists which can be expanded to include the
	 * specified range, the list must have been modified by an
	 * interleving call to region_del().  Pull a region descriptor
	 * from the cache and use it for this range.
	 */
	if (&rg->link == head || t < rg->from) {
		VM_BUG_ON(resv->region_cache_count <= 0);

		resv->region_cache_count--;
		nrg = list_first_entry(&resv->region_cache, struct file_region,
					link);
		list_del(&nrg->link);

		nrg->from = f;
		nrg->to = t;
		list_add(&nrg->link, rg->link.prev);

		add += t - f;
		goto out_locked;
	}

	/* Round our left edge to the current segment if it encloses us. */
	if (f > rg->from)
		f = rg->from;

	/* Check for and consume any regions we now overlap with. */
	nrg = rg;
	list_for_each_entry_safe(rg, trg, rg->link.prev, link) {
		if (&rg->link == head)
			break;
		if (rg->from > t)
			break;

		/* If this area reaches higher then extend our area to
		 * include it completely.  If this is not the first area
		 * which we intend to reuse, free it. */
		if (rg->to > t)
			t = rg->to;
		if (rg != nrg) {
			/* Decrement return value by the deleted range.
			 * Another range will span this area so that by
			 * end of routine add will be >= zero
			 */
			add -= (rg->to - rg->from);
			list_del(&rg->link);
			kfree(rg);
		}
	}

	add += (nrg->from - f);		/* Added to beginning of region */
	nrg->from = f;
	add += t - nrg->to;		/* Added to end of region */
	nrg->to = t;

out_locked:
	resv->adds_in_progress--;
	spin_unlock(&resv->lock);
	VM_BUG_ON(add < 0);
=======
/* Helper that removes a struct file_region from the resv_map cache and returns
 * it for use.
 */
static struct file_region *
get_file_region_entry_from_cache(struct resv_map *resv, long from, long to)
{
	struct file_region *nrg = NULL;

	VM_BUG_ON(resv->region_cache_count <= 0);

	resv->region_cache_count--;
	nrg = list_first_entry(&resv->region_cache, struct file_region, link);
	list_del(&nrg->link);

	nrg->from = from;
	nrg->to = to;

	return nrg;
}

static void copy_hugetlb_cgroup_uncharge_info(struct file_region *nrg,
					      struct file_region *rg)
{
#ifdef CONFIG_CGROUP_HUGETLB
	nrg->reservation_counter = rg->reservation_counter;
	nrg->css = rg->css;
	if (rg->css)
		css_get(rg->css);
#endif
}

/* Helper that records hugetlb_cgroup uncharge info. */
static void record_hugetlb_cgroup_uncharge_info(struct hugetlb_cgroup *h_cg,
						struct hstate *h,
						struct resv_map *resv,
						struct file_region *nrg)
{
#ifdef CONFIG_CGROUP_HUGETLB
	if (h_cg) {
		nrg->reservation_counter =
			&h_cg->rsvd_hugepage[hstate_index(h)];
		nrg->css = &h_cg->css;
		/*
		 * The caller will hold exactly one h_cg->css reference for the
		 * whole contiguous reservation region. But this area might be
		 * scattered when there are already some file_regions reside in
		 * it. As a result, many file_regions may share only one css
		 * reference. In order to ensure that one file_region must hold
		 * exactly one h_cg->css reference, we should do css_get for
		 * each file_region and leave the reference held by caller
		 * untouched.
		 */
		css_get(&h_cg->css);
		if (!resv->pages_per_hpage)
			resv->pages_per_hpage = pages_per_huge_page(h);
		/* pages_per_hpage should be the same for all entries in
		 * a resv_map.
		 */
		VM_BUG_ON(resv->pages_per_hpage != pages_per_huge_page(h));
	} else {
		nrg->reservation_counter = NULL;
		nrg->css = NULL;
	}
#endif
}

static void put_uncharge_info(struct file_region *rg)
{
#ifdef CONFIG_CGROUP_HUGETLB
	if (rg->css)
		css_put(rg->css);
#endif
}

static bool has_same_uncharge_info(struct file_region *rg,
				   struct file_region *org)
{
#ifdef CONFIG_CGROUP_HUGETLB
	return rg && org &&
	       rg->reservation_counter == org->reservation_counter &&
	       rg->css == org->css;

#else
	return true;
#endif
}

static void coalesce_file_region(struct resv_map *resv, struct file_region *rg)
{
	struct file_region *nrg = NULL, *prg = NULL;

	prg = list_prev_entry(rg, link);
	if (&prg->link != &resv->regions && prg->to == rg->from &&
	    has_same_uncharge_info(prg, rg)) {
		prg->to = rg->to;

		list_del(&rg->link);
		put_uncharge_info(rg);
		kfree(rg);

		rg = prg;
	}

	nrg = list_next_entry(rg, link);
	if (&nrg->link != &resv->regions && nrg->from == rg->to &&
	    has_same_uncharge_info(nrg, rg)) {
		nrg->from = rg->from;

		list_del(&rg->link);
		put_uncharge_info(rg);
		kfree(rg);
	}
}

static inline long
hugetlb_resv_map_add(struct resv_map *map, struct file_region *rg, long from,
		     long to, struct hstate *h, struct hugetlb_cgroup *cg,
		     long *regions_needed)
{
	struct file_region *nrg;

	if (!regions_needed) {
		nrg = get_file_region_entry_from_cache(map, from, to);
		record_hugetlb_cgroup_uncharge_info(cg, h, map, nrg);
		list_add(&nrg->link, rg->link.prev);
		coalesce_file_region(map, nrg);
	} else
		*regions_needed += 1;

	return to - from;
}

/*
 * Must be called with resv->lock held.
 *
 * Calling this with regions_needed != NULL will count the number of pages
 * to be added but will not modify the linked list. And regions_needed will
 * indicate the number of file_regions needed in the cache to carry out to add
 * the regions for this range.
 */
static long add_reservation_in_range(struct resv_map *resv, long f, long t,
				     struct hugetlb_cgroup *h_cg,
				     struct hstate *h, long *regions_needed)
{
	long add = 0;
	struct list_head *head = &resv->regions;
	long last_accounted_offset = f;
	struct file_region *rg = NULL, *trg = NULL;

	if (regions_needed)
		*regions_needed = 0;

	/* In this loop, we essentially handle an entry for the range
	 * [last_accounted_offset, rg->from), at every iteration, with some
	 * bounds checking.
	 */
	list_for_each_entry_safe(rg, trg, head, link) {
		/* Skip irrelevant regions that start before our range. */
		if (rg->from < f) {
			/* If this region ends after the last accounted offset,
			 * then we need to update last_accounted_offset.
			 */
			if (rg->to > last_accounted_offset)
				last_accounted_offset = rg->to;
			continue;
		}

		/* When we find a region that starts beyond our range, we've
		 * finished.
		 */
		if (rg->from >= t)
			break;

		/* Add an entry for last_accounted_offset -> rg->from, and
		 * update last_accounted_offset.
		 */
		if (rg->from > last_accounted_offset)
			add += hugetlb_resv_map_add(resv, rg,
						    last_accounted_offset,
						    rg->from, h, h_cg,
						    regions_needed);

		last_accounted_offset = rg->to;
	}

	/* Handle the case where our range extends beyond
	 * last_accounted_offset.
	 */
	if (last_accounted_offset < t)
		add += hugetlb_resv_map_add(resv, rg, last_accounted_offset,
					    t, h, h_cg, regions_needed);

	VM_BUG_ON(add < 0);
	return add;
}

/* Must be called with resv->lock acquired. Will drop lock to allocate entries.
 */
static int allocate_file_region_entries(struct resv_map *resv,
					int regions_needed)
	__must_hold(&resv->lock)
{
	struct list_head allocated_regions;
	int to_allocate = 0, i = 0;
	struct file_region *trg = NULL, *rg = NULL;

	VM_BUG_ON(regions_needed < 0);

	INIT_LIST_HEAD(&allocated_regions);

	/*
	 * Check for sufficient descriptors in the cache to accommodate
	 * the number of in progress add operations plus regions_needed.
	 *
	 * This is a while loop because when we drop the lock, some other call
	 * to region_add or region_del may have consumed some region_entries,
	 * so we keep looping here until we finally have enough entries for
	 * (adds_in_progress + regions_needed).
	 */
	while (resv->region_cache_count <
	       (resv->adds_in_progress + regions_needed)) {
		to_allocate = resv->adds_in_progress + regions_needed -
			      resv->region_cache_count;

		/* At this point, we should have enough entries in the cache
		 * for all the existing adds_in_progress. We should only be
		 * needing to allocate for regions_needed.
		 */
		VM_BUG_ON(resv->region_cache_count < resv->adds_in_progress);

		spin_unlock(&resv->lock);
		for (i = 0; i < to_allocate; i++) {
			trg = kmalloc(sizeof(*trg), GFP_KERNEL);
			if (!trg)
				goto out_of_memory;
			list_add(&trg->link, &allocated_regions);
		}

		spin_lock(&resv->lock);

		list_splice(&allocated_regions, &resv->region_cache);
		resv->region_cache_count += to_allocate;
	}

	return 0;

out_of_memory:
	list_for_each_entry_safe(rg, trg, &allocated_regions, link) {
		list_del(&rg->link);
		kfree(rg);
	}
	return -ENOMEM;
}

/*
 * Add the huge page range represented by [f, t) to the reserve
 * map.  Regions will be taken from the cache to fill in this range.
 * Sufficient regions should exist in the cache due to the previous
 * call to region_chg with the same range, but in some cases the cache will not
 * have sufficient entries due to races with other code doing region_add or
 * region_del.  The extra needed entries will be allocated.
 *
 * regions_needed is the out value provided by a previous call to region_chg.
 *
 * Return the number of new huge pages added to the map.  This number is greater
 * than or equal to zero.  If file_region entries needed to be allocated for
 * this operation and we were not able to allocate, it returns -ENOMEM.
 * region_add of regions of length 1 never allocate file_regions and cannot
 * fail; region_chg will always allocate at least 1 entry and a region_add for
 * 1 page will only require at most 1 entry.
 */
static long region_add(struct resv_map *resv, long f, long t,
		       long in_regions_needed, struct hstate *h,
		       struct hugetlb_cgroup *h_cg)
{
	long add = 0, actual_regions_needed = 0;

	spin_lock(&resv->lock);
retry:

	/* Count how many regions are actually needed to execute this add. */
	add_reservation_in_range(resv, f, t, NULL, NULL,
				 &actual_regions_needed);

	/*
	 * Check for sufficient descriptors in the cache to accommodate
	 * this add operation. Note that actual_regions_needed may be greater
	 * than in_regions_needed, as the resv_map may have been modified since
	 * the region_chg call. In this case, we need to make sure that we
	 * allocate extra entries, such that we have enough for all the
	 * existing adds_in_progress, plus the excess needed for this
	 * operation.
	 */
	if (actual_regions_needed > in_regions_needed &&
	    resv->region_cache_count <
		    resv->adds_in_progress +
			    (actual_regions_needed - in_regions_needed)) {
		/* region_add operation of range 1 should never need to
		 * allocate file_region entries.
		 */
		VM_BUG_ON(t - f <= 1);

		if (allocate_file_region_entries(
			    resv, actual_regions_needed - in_regions_needed)) {
			return -ENOMEM;
		}

		goto retry;
	}

	add = add_reservation_in_range(resv, f, t, h_cg, h, NULL);

	resv->adds_in_progress -= in_regions_needed;

	spin_unlock(&resv->lock);
>>>>>>> upstream/android-13
	return add;
}

/*
 * Examine the existing reserve map and determine how many
 * huge pages in the specified range [f, t) are NOT currently
 * represented.  This routine is called before a subsequent
 * call to region_add that will actually modify the reserve
 * map to add the specified range [f, t).  region_chg does
 * not change the number of huge pages represented by the
<<<<<<< HEAD
 * map.  However, if the existing regions in the map can not
 * be expanded to represent the new range, a new file_region
 * structure is added to the map as a placeholder.  This is
 * so that the subsequent region_add call will have all the
 * regions it needs and will not fail.
 *
 * Upon entry, region_chg will also examine the cache of region descriptors
 * associated with the map.  If there are not enough descriptors cached, one
 * will be allocated for the in progress add operation.
=======
 * map.  A number of new file_region structures is added to the cache as a
 * placeholder, for the subsequent region_add call to use. At least 1
 * file_region structure is added.
 *
 * out_regions_needed is the number of regions added to the
 * resv->adds_in_progress.  This value needs to be provided to a follow up call
 * to region_add or region_abort for proper accounting.
>>>>>>> upstream/android-13
 *
 * Returns the number of huge pages that need to be added to the existing
 * reservation map for the range [f, t).  This number is greater or equal to
 * zero.  -ENOMEM is returned if a new file_region structure or cache entry
 * is needed and can not be allocated.
 */
<<<<<<< HEAD
static long region_chg(struct resv_map *resv, long f, long t)
{
	struct list_head *head = &resv->regions;
	struct file_region *rg, *nrg = NULL;
	long chg = 0;

retry:
	spin_lock(&resv->lock);
retry_locked:
	resv->adds_in_progress++;

	/*
	 * Check for sufficient descriptors in the cache to accommodate
	 * the number of in progress add operations.
	 */
	if (resv->adds_in_progress > resv->region_cache_count) {
		struct file_region *trg;

		VM_BUG_ON(resv->adds_in_progress - resv->region_cache_count > 1);
		/* Must drop lock to allocate a new descriptor. */
		resv->adds_in_progress--;
		spin_unlock(&resv->lock);

		trg = kmalloc(sizeof(*trg), GFP_KERNEL);
		if (!trg) {
			kfree(nrg);
			return -ENOMEM;
		}

		spin_lock(&resv->lock);
		list_add(&trg->link, &resv->region_cache);
		resv->region_cache_count++;
		goto retry_locked;
	}

	/* Locate the region we are before or in. */
	list_for_each_entry(rg, head, link)
		if (f <= rg->to)
			break;

	/* If we are below the current region then a new region is required.
	 * Subtle, allocate a new region at the position but make it zero
	 * size such that we can guarantee to record the reservation. */
	if (&rg->link == head || t < rg->from) {
		if (!nrg) {
			resv->adds_in_progress--;
			spin_unlock(&resv->lock);
			nrg = kmalloc(sizeof(*nrg), GFP_KERNEL);
			if (!nrg)
				return -ENOMEM;

			nrg->from = f;
			nrg->to   = f;
			INIT_LIST_HEAD(&nrg->link);
			goto retry;
		}

		list_add(&nrg->link, rg->link.prev);
		chg = t - f;
		goto out_nrg;
	}

	/* Round our left edge to the current segment if it encloses us. */
	if (f > rg->from)
		f = rg->from;
	chg = t - f;

	/* Check for and consume any regions we now overlap with. */
	list_for_each_entry(rg, rg->link.prev, link) {
		if (&rg->link == head)
			break;
		if (rg->from > t)
			goto out;

		/* We overlap with this area, if it extends further than
		 * us then we must extend ourselves.  Account for its
		 * existing reservation. */
		if (rg->to > t) {
			chg += rg->to - t;
			t = rg->to;
		}
		chg -= rg->to - rg->from;
	}

out:
	spin_unlock(&resv->lock);
	/*  We already know we raced and no longer need the new region */
	kfree(nrg);
	return chg;
out_nrg:
=======
static long region_chg(struct resv_map *resv, long f, long t,
		       long *out_regions_needed)
{
	long chg = 0;

	spin_lock(&resv->lock);

	/* Count how many hugepages in this range are NOT represented. */
	chg = add_reservation_in_range(resv, f, t, NULL, NULL,
				       out_regions_needed);

	if (*out_regions_needed == 0)
		*out_regions_needed = 1;

	if (allocate_file_region_entries(resv, *out_regions_needed))
		return -ENOMEM;

	resv->adds_in_progress += *out_regions_needed;

>>>>>>> upstream/android-13
	spin_unlock(&resv->lock);
	return chg;
}

/*
 * Abort the in progress add operation.  The adds_in_progress field
 * of the resv_map keeps track of the operations in progress between
 * calls to region_chg and region_add.  Operations are sometimes
 * aborted after the call to region_chg.  In such cases, region_abort
<<<<<<< HEAD
 * is called to decrement the adds_in_progress counter.
=======
 * is called to decrement the adds_in_progress counter. regions_needed
 * is the value returned by the region_chg call, it is used to decrement
 * the adds_in_progress counter.
>>>>>>> upstream/android-13
 *
 * NOTE: The range arguments [f, t) are not needed or used in this
 * routine.  They are kept to make reading the calling code easier as
 * arguments will match the associated region_chg call.
 */
<<<<<<< HEAD
static void region_abort(struct resv_map *resv, long f, long t)
{
	spin_lock(&resv->lock);
	VM_BUG_ON(!resv->region_cache_count);
	resv->adds_in_progress--;
=======
static void region_abort(struct resv_map *resv, long f, long t,
			 long regions_needed)
{
	spin_lock(&resv->lock);
	VM_BUG_ON(!resv->region_cache_count);
	resv->adds_in_progress -= regions_needed;
>>>>>>> upstream/android-13
	spin_unlock(&resv->lock);
}

/*
 * Delete the specified range [f, t) from the reserve map.  If the
 * t parameter is LONG_MAX, this indicates that ALL regions after f
 * should be deleted.  Locate the regions which intersect [f, t)
 * and either trim, delete or split the existing regions.
 *
 * Returns the number of huge pages deleted from the reserve map.
 * In the normal case, the return value is zero or more.  In the
 * case where a region must be split, a new region descriptor must
 * be allocated.  If the allocation fails, -ENOMEM will be returned.
 * NOTE: If the parameter t == LONG_MAX, then we will never split
 * a region and possibly return -ENOMEM.  Callers specifying
 * t == LONG_MAX do not need to check for -ENOMEM error.
 */
static long region_del(struct resv_map *resv, long f, long t)
{
	struct list_head *head = &resv->regions;
	struct file_region *rg, *trg;
	struct file_region *nrg = NULL;
	long del = 0;

retry:
	spin_lock(&resv->lock);
	list_for_each_entry_safe(rg, trg, head, link) {
		/*
		 * Skip regions before the range to be deleted.  file_region
		 * ranges are normally of the form [from, to).  However, there
		 * may be a "placeholder" entry in the map which is of the form
		 * (from, to) with from == to.  Check for placeholder entries
		 * at the beginning of the range to be deleted.
		 */
		if (rg->to <= f && (rg->to != rg->from || rg->to != f))
			continue;

		if (rg->from >= t)
			break;

		if (f > rg->from && t < rg->to) { /* Must split region */
			/*
			 * Check for an entry in the cache before dropping
			 * lock and attempting allocation.
			 */
			if (!nrg &&
			    resv->region_cache_count > resv->adds_in_progress) {
				nrg = list_first_entry(&resv->region_cache,
							struct file_region,
							link);
				list_del(&nrg->link);
				resv->region_cache_count--;
			}

			if (!nrg) {
				spin_unlock(&resv->lock);
				nrg = kmalloc(sizeof(*nrg), GFP_KERNEL);
				if (!nrg)
					return -ENOMEM;
				goto retry;
			}

			del += t - f;
<<<<<<< HEAD
=======
			hugetlb_cgroup_uncharge_file_region(
				resv, rg, t - f, false);
>>>>>>> upstream/android-13

			/* New entry for end of split region */
			nrg->from = t;
			nrg->to = rg->to;
<<<<<<< HEAD
=======

			copy_hugetlb_cgroup_uncharge_info(nrg, rg);

>>>>>>> upstream/android-13
			INIT_LIST_HEAD(&nrg->link);

			/* Original entry is trimmed */
			rg->to = f;

			list_add(&nrg->link, &rg->link);
			nrg = NULL;
			break;
		}

		if (f <= rg->from && t >= rg->to) { /* Remove entire region */
			del += rg->to - rg->from;
<<<<<<< HEAD
=======
			hugetlb_cgroup_uncharge_file_region(resv, rg,
							    rg->to - rg->from, true);
>>>>>>> upstream/android-13
			list_del(&rg->link);
			kfree(rg);
			continue;
		}

		if (f <= rg->from) {	/* Trim beginning of region */
<<<<<<< HEAD
			del += t - rg->from;
			rg->from = t;
		} else {		/* Trim end of region */
=======
			hugetlb_cgroup_uncharge_file_region(resv, rg,
							    t - rg->from, false);

			del += t - rg->from;
			rg->from = t;
		} else {		/* Trim end of region */
			hugetlb_cgroup_uncharge_file_region(resv, rg,
							    rg->to - f, false);

>>>>>>> upstream/android-13
			del += rg->to - f;
			rg->to = f;
		}
	}

	spin_unlock(&resv->lock);
	kfree(nrg);
	return del;
}

/*
 * A rare out of memory error was encountered which prevented removal of
 * the reserve map region for a page.  The huge page itself was free'ed
 * and removed from the page cache.  This routine will adjust the subpool
 * usage count, and the global reserve count if needed.  By incrementing
 * these counts, the reserve map entry which could not be deleted will
 * appear as a "reserved" entry instead of simply dangling with incorrect
 * counts.
 */
void hugetlb_fix_reserve_counts(struct inode *inode)
{
	struct hugepage_subpool *spool = subpool_inode(inode);
	long rsv_adjust;
	bool reserved = false;

	rsv_adjust = hugepage_subpool_get_pages(spool, 1);
	if (rsv_adjust > 0) {
		struct hstate *h = hstate_inode(inode);

		if (!hugetlb_acct_memory(h, 1))
			reserved = true;
	} else if (!rsv_adjust) {
		reserved = true;
	}

	if (!reserved)
		pr_warn("hugetlb: Huge Page Reserved count may go negative.\n");
}

/*
 * Count and return the number of huge pages in the reserve map
 * that intersect with the range [f, t).
 */
static long region_count(struct resv_map *resv, long f, long t)
{
	struct list_head *head = &resv->regions;
	struct file_region *rg;
	long chg = 0;

	spin_lock(&resv->lock);
	/* Locate each segment we overlap with, and count that overlap. */
	list_for_each_entry(rg, head, link) {
		long seg_from;
		long seg_to;

		if (rg->to <= f)
			continue;
		if (rg->from >= t)
			break;

		seg_from = max(rg->from, f);
		seg_to = min(rg->to, t);

		chg += seg_to - seg_from;
	}
	spin_unlock(&resv->lock);

	return chg;
}

/*
 * Convert the address within this vma to the page offset within
 * the mapping, in pagecache page units; huge pages here.
 */
static pgoff_t vma_hugecache_offset(struct hstate *h,
			struct vm_area_struct *vma, unsigned long address)
{
	return ((address - vma->vm_start) >> huge_page_shift(h)) +
			(vma->vm_pgoff >> huge_page_order(h));
}

pgoff_t linear_hugepage_index(struct vm_area_struct *vma,
				     unsigned long address)
{
	return vma_hugecache_offset(hstate_vma(vma), vma, address);
}
EXPORT_SYMBOL_GPL(linear_hugepage_index);

/*
 * Return the size of the pages allocated when backing a VMA. In the majority
 * cases this will be same size as used by the page table entries.
 */
unsigned long vma_kernel_pagesize(struct vm_area_struct *vma)
{
	if (vma->vm_ops && vma->vm_ops->pagesize)
		return vma->vm_ops->pagesize(vma);
	return PAGE_SIZE;
}
EXPORT_SYMBOL_GPL(vma_kernel_pagesize);

/*
 * Return the page size being used by the MMU to back a VMA. In the majority
 * of cases, the page size used by the kernel matches the MMU size. On
 * architectures where it differs, an architecture-specific 'strong'
 * version of this symbol is required.
 */
__weak unsigned long vma_mmu_pagesize(struct vm_area_struct *vma)
{
	return vma_kernel_pagesize(vma);
}

/*
 * Flags for MAP_PRIVATE reservations.  These are stored in the bottom
 * bits of the reservation map pointer, which are always clear due to
 * alignment.
 */
#define HPAGE_RESV_OWNER    (1UL << 0)
#define HPAGE_RESV_UNMAPPED (1UL << 1)
#define HPAGE_RESV_MASK (HPAGE_RESV_OWNER | HPAGE_RESV_UNMAPPED)

/*
 * These helpers are used to track how many pages are reserved for
 * faults in a MAP_PRIVATE mapping. Only the process that called mmap()
 * is guaranteed to have their future faults succeed.
 *
 * With the exception of reset_vma_resv_huge_pages() which is called at fork(),
 * the reserve counters are updated with the hugetlb_lock held. It is safe
 * to reset the VMA at fork() time as it is not in use yet and there is no
 * chance of the global counters getting corrupted as a result of the values.
 *
 * The private mapping reservation is represented in a subtly different
 * manner to a shared mapping.  A shared mapping has a region map associated
 * with the underlying file, this region map represents the backing file
 * pages which have ever had a reservation assigned which this persists even
 * after the page is instantiated.  A private mapping has a region map
 * associated with the original mmap which is attached to all VMAs which
 * reference it, this region map represents those offsets which have consumed
 * reservation ie. where pages have been instantiated.
 */
static unsigned long get_vma_private_data(struct vm_area_struct *vma)
{
	return (unsigned long)vma->vm_private_data;
}

static void set_vma_private_data(struct vm_area_struct *vma,
							unsigned long value)
{
	vma->vm_private_data = (void *)value;
}

<<<<<<< HEAD
=======
static void
resv_map_set_hugetlb_cgroup_uncharge_info(struct resv_map *resv_map,
					  struct hugetlb_cgroup *h_cg,
					  struct hstate *h)
{
#ifdef CONFIG_CGROUP_HUGETLB
	if (!h_cg || !h) {
		resv_map->reservation_counter = NULL;
		resv_map->pages_per_hpage = 0;
		resv_map->css = NULL;
	} else {
		resv_map->reservation_counter =
			&h_cg->rsvd_hugepage[hstate_index(h)];
		resv_map->pages_per_hpage = pages_per_huge_page(h);
		resv_map->css = &h_cg->css;
	}
#endif
}

>>>>>>> upstream/android-13
struct resv_map *resv_map_alloc(void)
{
	struct resv_map *resv_map = kmalloc(sizeof(*resv_map), GFP_KERNEL);
	struct file_region *rg = kmalloc(sizeof(*rg), GFP_KERNEL);

	if (!resv_map || !rg) {
		kfree(resv_map);
		kfree(rg);
		return NULL;
	}

	kref_init(&resv_map->refs);
	spin_lock_init(&resv_map->lock);
	INIT_LIST_HEAD(&resv_map->regions);

	resv_map->adds_in_progress = 0;
<<<<<<< HEAD
=======
	/*
	 * Initialize these to 0. On shared mappings, 0's here indicate these
	 * fields don't do cgroup accounting. On private mappings, these will be
	 * re-initialized to the proper values, to indicate that hugetlb cgroup
	 * reservations are to be un-charged from here.
	 */
	resv_map_set_hugetlb_cgroup_uncharge_info(resv_map, NULL, NULL);
>>>>>>> upstream/android-13

	INIT_LIST_HEAD(&resv_map->region_cache);
	list_add(&rg->link, &resv_map->region_cache);
	resv_map->region_cache_count = 1;

	return resv_map;
}

void resv_map_release(struct kref *ref)
{
	struct resv_map *resv_map = container_of(ref, struct resv_map, refs);
	struct list_head *head = &resv_map->region_cache;
	struct file_region *rg, *trg;

	/* Clear out any active regions before we release the map. */
	region_del(resv_map, 0, LONG_MAX);

	/* ... and any entries left in the cache */
	list_for_each_entry_safe(rg, trg, head, link) {
		list_del(&rg->link);
		kfree(rg);
	}

	VM_BUG_ON(resv_map->adds_in_progress);

	kfree(resv_map);
}

static inline struct resv_map *inode_resv_map(struct inode *inode)
{
<<<<<<< HEAD
	return inode->i_mapping->private_data;
=======
	/*
	 * At inode evict time, i_mapping may not point to the original
	 * address space within the inode.  This original address space
	 * contains the pointer to the resv_map.  So, always use the
	 * address space embedded within the inode.
	 * The VERY common case is inode->mapping == &inode->i_data but,
	 * this may not be true for device special inodes.
	 */
	return (struct resv_map *)(&inode->i_data)->private_data;
>>>>>>> upstream/android-13
}

static struct resv_map *vma_resv_map(struct vm_area_struct *vma)
{
	VM_BUG_ON_VMA(!is_vm_hugetlb_page(vma), vma);
	if (vma->vm_flags & VM_MAYSHARE) {
		struct address_space *mapping = vma->vm_file->f_mapping;
		struct inode *inode = mapping->host;

		return inode_resv_map(inode);

	} else {
		return (struct resv_map *)(get_vma_private_data(vma) &
							~HPAGE_RESV_MASK);
	}
}

static void set_vma_resv_map(struct vm_area_struct *vma, struct resv_map *map)
{
	VM_BUG_ON_VMA(!is_vm_hugetlb_page(vma), vma);
	VM_BUG_ON_VMA(vma->vm_flags & VM_MAYSHARE, vma);

	set_vma_private_data(vma, (get_vma_private_data(vma) &
				HPAGE_RESV_MASK) | (unsigned long)map);
}

static void set_vma_resv_flags(struct vm_area_struct *vma, unsigned long flags)
{
	VM_BUG_ON_VMA(!is_vm_hugetlb_page(vma), vma);
	VM_BUG_ON_VMA(vma->vm_flags & VM_MAYSHARE, vma);

	set_vma_private_data(vma, get_vma_private_data(vma) | flags);
}

static int is_vma_resv_set(struct vm_area_struct *vma, unsigned long flag)
{
	VM_BUG_ON_VMA(!is_vm_hugetlb_page(vma), vma);

	return (get_vma_private_data(vma) & flag) != 0;
}

/* Reset counters to 0 and clear all HPAGE_RESV_* flags */
void reset_vma_resv_huge_pages(struct vm_area_struct *vma)
{
	VM_BUG_ON_VMA(!is_vm_hugetlb_page(vma), vma);
	if (!(vma->vm_flags & VM_MAYSHARE))
		vma->vm_private_data = (void *)0;
}

/* Returns true if the VMA has associated reserve pages */
static bool vma_has_reserves(struct vm_area_struct *vma, long chg)
{
	if (vma->vm_flags & VM_NORESERVE) {
		/*
		 * This address is already reserved by other process(chg == 0),
		 * so, we should decrement reserved count. Without decrementing,
		 * reserve count remains after releasing inode, because this
		 * allocated page will go into page cache and is regarded as
		 * coming from reserved pool in releasing step.  Currently, we
		 * don't have any other solution to deal with this situation
		 * properly, so add work-around here.
		 */
		if (vma->vm_flags & VM_MAYSHARE && chg == 0)
			return true;
		else
			return false;
	}

	/* Shared mappings always use reserves */
	if (vma->vm_flags & VM_MAYSHARE) {
		/*
		 * We know VM_NORESERVE is not set.  Therefore, there SHOULD
		 * be a region map for all pages.  The only situation where
		 * there is no region map is if a hole was punched via
<<<<<<< HEAD
		 * fallocate.  In this case, there really are no reverves to
=======
		 * fallocate.  In this case, there really are no reserves to
>>>>>>> upstream/android-13
		 * use.  This situation is indicated if chg != 0.
		 */
		if (chg)
			return false;
		else
			return true;
	}

	/*
	 * Only the process that called mmap() has reserves for
	 * private mappings.
	 */
	if (is_vma_resv_set(vma, HPAGE_RESV_OWNER)) {
		/*
		 * Like the shared case above, a hole punch or truncate
		 * could have been performed on the private mapping.
		 * Examine the value of chg to determine if reserves
		 * actually exist or were previously consumed.
		 * Very Subtle - The value of chg comes from a previous
		 * call to vma_needs_reserves().  The reserve map for
		 * private mappings has different (opposite) semantics
		 * than that of shared mappings.  vma_needs_reserves()
		 * has already taken this difference in semantics into
		 * account.  Therefore, the meaning of chg is the same
		 * as in the shared case above.  Code could easily be
		 * combined, but keeping it separate draws attention to
		 * subtle differences.
		 */
		if (chg)
			return false;
		else
			return true;
	}

	return false;
}

static void enqueue_huge_page(struct hstate *h, struct page *page)
{
	int nid = page_to_nid(page);
<<<<<<< HEAD
	list_move(&page->lru, &h->hugepage_freelists[nid]);
	h->free_huge_pages++;
	h->free_huge_pages_node[nid]++;
	SetPageHugeFreed(page);
=======

	lockdep_assert_held(&hugetlb_lock);
	VM_BUG_ON_PAGE(page_count(page), page);

	list_move(&page->lru, &h->hugepage_freelists[nid]);
	h->free_huge_pages++;
	h->free_huge_pages_node[nid]++;
	SetHPageFreed(page);
>>>>>>> upstream/android-13
}

static struct page *dequeue_huge_page_node_exact(struct hstate *h, int nid)
{
	struct page *page;
<<<<<<< HEAD

	list_for_each_entry(page, &h->hugepage_freelists[nid], lru)
		if (!PageHWPoison(page))
			break;
	/*
	 * if 'non-isolated free hugepage' not found on the list,
	 * the allocation fails.
	 */
	if (&h->hugepage_freelists[nid] == &page->lru)
		return NULL;
	list_move(&page->lru, &h->hugepage_activelist);
	set_page_refcounted(page);
	ClearPageHugeFreed(page);
	h->free_huge_pages--;
	h->free_huge_pages_node[nid]--;
	return page;
=======
	bool pin = !!(current->flags & PF_MEMALLOC_PIN);

	lockdep_assert_held(&hugetlb_lock);
	list_for_each_entry(page, &h->hugepage_freelists[nid], lru) {
		if (pin && !is_pinnable_page(page))
			continue;

		if (PageHWPoison(page))
			continue;

		list_move(&page->lru, &h->hugepage_activelist);
		set_page_refcounted(page);
		ClearHPageFreed(page);
		h->free_huge_pages--;
		h->free_huge_pages_node[nid]--;
		return page;
	}

	return NULL;
>>>>>>> upstream/android-13
}

static struct page *dequeue_huge_page_nodemask(struct hstate *h, gfp_t gfp_mask, int nid,
		nodemask_t *nmask)
{
	unsigned int cpuset_mems_cookie;
	struct zonelist *zonelist;
	struct zone *zone;
	struct zoneref *z;
<<<<<<< HEAD
	int node = -1;
=======
	int node = NUMA_NO_NODE;
>>>>>>> upstream/android-13

	zonelist = node_zonelist(nid, gfp_mask);

retry_cpuset:
	cpuset_mems_cookie = read_mems_allowed_begin();
	for_each_zone_zonelist_nodemask(zone, z, zonelist, gfp_zone(gfp_mask), nmask) {
		struct page *page;

		if (!cpuset_zone_allowed(zone, gfp_mask))
			continue;
		/*
		 * no need to ask again on the same node. Pool is node rather than
		 * zone aware
		 */
		if (zone_to_nid(zone) == node)
			continue;
		node = zone_to_nid(zone);

		page = dequeue_huge_page_node_exact(h, node);
		if (page)
			return page;
	}
	if (unlikely(read_mems_allowed_retry(cpuset_mems_cookie)))
		goto retry_cpuset;

	return NULL;
}

<<<<<<< HEAD
/* Movability of hugepages depends on migration support. */
static inline gfp_t htlb_alloc_mask(struct hstate *h)
{
	if (hugepage_migration_supported(h))
		return GFP_HIGHUSER_MOVABLE;
	else
		return GFP_HIGHUSER;
}

=======
>>>>>>> upstream/android-13
static struct page *dequeue_huge_page_vma(struct hstate *h,
				struct vm_area_struct *vma,
				unsigned long address, int avoid_reserve,
				long chg)
{
<<<<<<< HEAD
	struct page *page;
=======
	struct page *page = NULL;
>>>>>>> upstream/android-13
	struct mempolicy *mpol;
	gfp_t gfp_mask;
	nodemask_t *nodemask;
	int nid;

	/*
	 * A child process with MAP_PRIVATE mappings created by their parent
	 * have no page reserves. This check ensures that reservations are
	 * not "stolen". The child may still get SIGKILLed
	 */
	if (!vma_has_reserves(vma, chg) &&
			h->free_huge_pages - h->resv_huge_pages == 0)
		goto err;

	/* If reserves cannot be used, ensure enough pages are in the pool */
	if (avoid_reserve && h->free_huge_pages - h->resv_huge_pages == 0)
		goto err;

	gfp_mask = htlb_alloc_mask(h);
	nid = huge_node(vma, address, gfp_mask, &mpol, &nodemask);
<<<<<<< HEAD
	page = dequeue_huge_page_nodemask(h, gfp_mask, nid, nodemask);
	if (page && !avoid_reserve && vma_has_reserves(vma, chg)) {
		SetPagePrivate(page);
=======

	if (mpol_is_preferred_many(mpol)) {
		page = dequeue_huge_page_nodemask(h, gfp_mask, nid, nodemask);

		/* Fallback to all nodes if page==NULL */
		nodemask = NULL;
	}

	if (!page)
		page = dequeue_huge_page_nodemask(h, gfp_mask, nid, nodemask);

	if (page && !avoid_reserve && vma_has_reserves(vma, chg)) {
		SetHPageRestoreReserve(page);
>>>>>>> upstream/android-13
		h->resv_huge_pages--;
	}

	mpol_cond_put(mpol);
	return page;

err:
	return NULL;
}

/*
 * common helper functions for hstate_next_node_to_{alloc|free}.
 * We may have allocated or freed a huge page based on a different
 * nodes_allowed previously, so h->next_node_to_{alloc|free} might
 * be outside of *nodes_allowed.  Ensure that we use an allowed
 * node for alloc or free.
 */
static int next_node_allowed(int nid, nodemask_t *nodes_allowed)
{
	nid = next_node_in(nid, *nodes_allowed);
	VM_BUG_ON(nid >= MAX_NUMNODES);

	return nid;
}

static int get_valid_node_allowed(int nid, nodemask_t *nodes_allowed)
{
	if (!node_isset(nid, *nodes_allowed))
		nid = next_node_allowed(nid, nodes_allowed);
	return nid;
}

/*
 * returns the previously saved node ["this node"] from which to
 * allocate a persistent huge page for the pool and advance the
 * next node from which to allocate, handling wrap at end of node
 * mask.
 */
static int hstate_next_node_to_alloc(struct hstate *h,
					nodemask_t *nodes_allowed)
{
	int nid;

	VM_BUG_ON(!nodes_allowed);

	nid = get_valid_node_allowed(h->next_nid_to_alloc, nodes_allowed);
	h->next_nid_to_alloc = next_node_allowed(nid, nodes_allowed);

	return nid;
}

/*
<<<<<<< HEAD
 * helper for free_pool_huge_page() - return the previously saved
=======
 * helper for remove_pool_huge_page() - return the previously saved
>>>>>>> upstream/android-13
 * node ["this node"] from which to free a huge page.  Advance the
 * next node id whether or not we find a free huge page to free so
 * that the next attempt to free addresses the next node.
 */
static int hstate_next_node_to_free(struct hstate *h, nodemask_t *nodes_allowed)
{
	int nid;

	VM_BUG_ON(!nodes_allowed);

	nid = get_valid_node_allowed(h->next_nid_to_free, nodes_allowed);
	h->next_nid_to_free = next_node_allowed(nid, nodes_allowed);

	return nid;
}

#define for_each_node_mask_to_alloc(hs, nr_nodes, node, mask)		\
	for (nr_nodes = nodes_weight(*mask);				\
		nr_nodes > 0 &&						\
		((node = hstate_next_node_to_alloc(hs, mask)) || 1);	\
		nr_nodes--)

#define for_each_node_mask_to_free(hs, nr_nodes, node, mask)		\
	for (nr_nodes = nodes_weight(*mask);				\
		nr_nodes > 0 &&						\
		((node = hstate_next_node_to_free(hs, mask)) || 1);	\
		nr_nodes--)

#ifdef CONFIG_ARCH_HAS_GIGANTIC_PAGE
static void destroy_compound_gigantic_page(struct page *page,
					unsigned int order)
{
	int i;
	int nr_pages = 1 << order;
	struct page *p = page + 1;

	atomic_set(compound_mapcount_ptr(page), 0);
<<<<<<< HEAD
=======
	atomic_set(compound_pincount_ptr(page), 0);

>>>>>>> upstream/android-13
	for (i = 1; i < nr_pages; i++, p = mem_map_next(p, page, i)) {
		clear_compound_head(p);
		set_page_refcounted(p);
	}

	set_compound_order(page, 0);
<<<<<<< HEAD
=======
	page[1].compound_nr = 0;
>>>>>>> upstream/android-13
	__ClearPageHead(page);
}

static void free_gigantic_page(struct page *page, unsigned int order)
{
<<<<<<< HEAD
	free_contig_range(page_to_pfn(page), 1 << order);
}

static int __alloc_gigantic_page(unsigned long start_pfn,
				unsigned long nr_pages, gfp_t gfp_mask)
{
	unsigned long end_pfn = start_pfn + nr_pages;
	return alloc_contig_range(start_pfn, end_pfn, MIGRATE_MOVABLE,
				  gfp_mask);
}

static bool pfn_range_valid_gigantic(struct zone *z,
			unsigned long start_pfn, unsigned long nr_pages)
{
	unsigned long i, end_pfn = start_pfn + nr_pages;
	struct page *page;

	for (i = start_pfn; i < end_pfn; i++) {
		page = pfn_to_online_page(i);
		if (!page)
			return false;

		if (page_zone(page) != z)
			return false;

		if (PageReserved(page))
			return false;

		if (page_count(page) > 0)
			return false;

		if (PageHuge(page))
			return false;
	}

	return true;
}

static bool zone_spans_last_pfn(const struct zone *zone,
			unsigned long start_pfn, unsigned long nr_pages)
{
	unsigned long last_pfn = start_pfn + nr_pages - 1;
	return zone_spans_pfn(zone, last_pfn);
}

static struct page *alloc_gigantic_page(struct hstate *h, gfp_t gfp_mask,
		int nid, nodemask_t *nodemask)
{
	unsigned int order = huge_page_order(h);
	unsigned long nr_pages = 1 << order;
	unsigned long ret, pfn, flags;
	struct zonelist *zonelist;
	struct zone *zone;
	struct zoneref *z;

	zonelist = node_zonelist(nid, gfp_mask);
	for_each_zone_zonelist_nodemask(zone, z, zonelist, gfp_zone(gfp_mask), nodemask) {
		spin_lock_irqsave(&zone->lock, flags);

		pfn = ALIGN(zone->zone_start_pfn, nr_pages);
		while (zone_spans_last_pfn(zone, pfn, nr_pages)) {
			if (pfn_range_valid_gigantic(zone, pfn, nr_pages)) {
				/*
				 * We release the zone lock here because
				 * alloc_contig_range() will also lock the zone
				 * at some point. If there's an allocation
				 * spinning on this lock, it may win the race
				 * and cause alloc_contig_range() to fail...
				 */
				spin_unlock_irqrestore(&zone->lock, flags);
				ret = __alloc_gigantic_page(pfn, nr_pages, gfp_mask);
				if (!ret)
					return pfn_to_page(pfn);
				spin_lock_irqsave(&zone->lock, flags);
			}
			pfn += nr_pages;
		}

		spin_unlock_irqrestore(&zone->lock, flags);
	}

	return NULL;
}

static void prep_new_huge_page(struct hstate *h, struct page *page, int nid);
static void prep_compound_gigantic_page(struct page *page, unsigned int order);

#else /* !CONFIG_ARCH_HAS_GIGANTIC_PAGE */
static inline bool gigantic_page_supported(void) { return false; }
static struct page *alloc_gigantic_page(struct hstate *h, gfp_t gfp_mask,
		int nid, nodemask_t *nodemask) { return NULL; }
=======
	/*
	 * If the page isn't allocated using the cma allocator,
	 * cma_release() returns false.
	 */
#ifdef CONFIG_CMA
	if (cma_release(hugetlb_cma[page_to_nid(page)], page, 1 << order))
		return;
#endif

	free_contig_range(page_to_pfn(page), 1 << order);
}

#ifdef CONFIG_CONTIG_ALLOC
static struct page *alloc_gigantic_page(struct hstate *h, gfp_t gfp_mask,
		int nid, nodemask_t *nodemask)
{
	unsigned long nr_pages = pages_per_huge_page(h);
	if (nid == NUMA_NO_NODE)
		nid = numa_mem_id();

#ifdef CONFIG_CMA
	{
		struct page *page;
		int node;

		if (hugetlb_cma[nid]) {
			page = cma_alloc(hugetlb_cma[nid], nr_pages,
					huge_page_order(h), true);
			if (page)
				return page;
		}

		if (!(gfp_mask & __GFP_THISNODE)) {
			for_each_node_mask(node, *nodemask) {
				if (node == nid || !hugetlb_cma[node])
					continue;

				page = cma_alloc(hugetlb_cma[node], nr_pages,
						huge_page_order(h), true);
				if (page)
					return page;
			}
		}
	}
#endif

	return alloc_contig_pages(nr_pages, gfp_mask, nid, nodemask);
}

#else /* !CONFIG_CONTIG_ALLOC */
static struct page *alloc_gigantic_page(struct hstate *h, gfp_t gfp_mask,
					int nid, nodemask_t *nodemask)
{
	return NULL;
}
#endif /* CONFIG_CONTIG_ALLOC */

#else /* !CONFIG_ARCH_HAS_GIGANTIC_PAGE */
static struct page *alloc_gigantic_page(struct hstate *h, gfp_t gfp_mask,
					int nid, nodemask_t *nodemask)
{
	return NULL;
}
>>>>>>> upstream/android-13
static inline void free_gigantic_page(struct page *page, unsigned int order) { }
static inline void destroy_compound_gigantic_page(struct page *page,
						unsigned int order) { }
#endif

<<<<<<< HEAD
static void update_and_free_page(struct hstate *h, struct page *page)
=======
/*
 * Remove hugetlb page from lists, and update dtor so that page appears
 * as just a compound page.  A reference is held on the page.
 *
 * Must be called with hugetlb lock held.
 */
static void remove_hugetlb_page(struct hstate *h, struct page *page,
							bool adjust_surplus)
{
	int nid = page_to_nid(page);

	VM_BUG_ON_PAGE(hugetlb_cgroup_from_page(page), page);
	VM_BUG_ON_PAGE(hugetlb_cgroup_from_page_rsvd(page), page);

	lockdep_assert_held(&hugetlb_lock);
	if (hstate_is_gigantic(h) && !gigantic_page_runtime_supported())
		return;

	list_del(&page->lru);

	if (HPageFreed(page)) {
		h->free_huge_pages--;
		h->free_huge_pages_node[nid]--;
	}
	if (adjust_surplus) {
		h->surplus_huge_pages--;
		h->surplus_huge_pages_node[nid]--;
	}

	/*
	 * Very subtle
	 *
	 * For non-gigantic pages set the destructor to the normal compound
	 * page dtor.  This is needed in case someone takes an additional
	 * temporary ref to the page, and freeing is delayed until they drop
	 * their reference.
	 *
	 * For gigantic pages set the destructor to the null dtor.  This
	 * destructor will never be called.  Before freeing the gigantic
	 * page destroy_compound_gigantic_page will turn the compound page
	 * into a simple group of pages.  After this the destructor does not
	 * apply.
	 *
	 * This handles the case where more than one ref is held when and
	 * after update_and_free_page is called.
	 */
	set_page_refcounted(page);
	if (hstate_is_gigantic(h))
		set_compound_page_dtor(page, NULL_COMPOUND_DTOR);
	else
		set_compound_page_dtor(page, COMPOUND_PAGE_DTOR);

	h->nr_huge_pages--;
	h->nr_huge_pages_node[nid]--;
}

static void add_hugetlb_page(struct hstate *h, struct page *page,
			     bool adjust_surplus)
{
	int zeroed;
	int nid = page_to_nid(page);

	VM_BUG_ON_PAGE(!HPageVmemmapOptimized(page), page);

	lockdep_assert_held(&hugetlb_lock);

	INIT_LIST_HEAD(&page->lru);
	h->nr_huge_pages++;
	h->nr_huge_pages_node[nid]++;

	if (adjust_surplus) {
		h->surplus_huge_pages++;
		h->surplus_huge_pages_node[nid]++;
	}

	set_compound_page_dtor(page, HUGETLB_PAGE_DTOR);
	set_page_private(page, 0);
	SetHPageVmemmapOptimized(page);

	/*
	 * This page is about to be managed by the hugetlb allocator and
	 * should have no users.  Drop our reference, and check for others
	 * just in case.
	 */
	zeroed = put_page_testzero(page);
	if (!zeroed)
		/*
		 * It is VERY unlikely soneone else has taken a ref on
		 * the page.  In this case, we simply return as the
		 * hugetlb destructor (free_huge_page) will be called
		 * when this other ref is dropped.
		 */
		return;

	arch_clear_hugepage_flags(page);
	enqueue_huge_page(h, page);
}

static void __update_and_free_page(struct hstate *h, struct page *page)
>>>>>>> upstream/android-13
{
	int i;
	struct page *subpage = page;

<<<<<<< HEAD
	if (hstate_is_gigantic(h) && !gigantic_page_supported())
		return;

	h->nr_huge_pages--;
	h->nr_huge_pages_node[page_to_nid(page)]--;
=======
	if (hstate_is_gigantic(h) && !gigantic_page_runtime_supported())
		return;

	if (alloc_huge_page_vmemmap(h, page)) {
		spin_lock_irq(&hugetlb_lock);
		/*
		 * If we cannot allocate vmemmap pages, just refuse to free the
		 * page and put the page back on the hugetlb free list and treat
		 * as a surplus page.
		 */
		add_hugetlb_page(h, page, true);
		spin_unlock_irq(&hugetlb_lock);
		return;
	}

>>>>>>> upstream/android-13
	for (i = 0; i < pages_per_huge_page(h);
	     i++, subpage = mem_map_next(subpage, page, i)) {
		subpage->flags &= ~(1 << PG_locked | 1 << PG_error |
				1 << PG_referenced | 1 << PG_dirty |
				1 << PG_active | 1 << PG_private |
				1 << PG_writeback);
	}
<<<<<<< HEAD
	VM_BUG_ON_PAGE(hugetlb_cgroup_from_page(page), page);
	set_compound_page_dtor(page, NULL_COMPOUND_DTOR);
	set_page_refcounted(page);
=======
>>>>>>> upstream/android-13
	if (hstate_is_gigantic(h)) {
		destroy_compound_gigantic_page(page, huge_page_order(h));
		free_gigantic_page(page, huge_page_order(h));
	} else {
		__free_pages(page, huge_page_order(h));
	}
}

<<<<<<< HEAD
=======
/*
 * As update_and_free_page() can be called under any context, so we cannot
 * use GFP_KERNEL to allocate vmemmap pages. However, we can defer the
 * actual freeing in a workqueue to prevent from using GFP_ATOMIC to allocate
 * the vmemmap pages.
 *
 * free_hpage_workfn() locklessly retrieves the linked list of pages to be
 * freed and frees them one-by-one. As the page->mapping pointer is going
 * to be cleared in free_hpage_workfn() anyway, it is reused as the llist_node
 * structure of a lockless linked list of huge pages to be freed.
 */
static LLIST_HEAD(hpage_freelist);

static void free_hpage_workfn(struct work_struct *work)
{
	struct llist_node *node;

	node = llist_del_all(&hpage_freelist);

	while (node) {
		struct page *page;
		struct hstate *h;

		page = container_of((struct address_space **)node,
				     struct page, mapping);
		node = node->next;
		page->mapping = NULL;
		/*
		 * The VM_BUG_ON_PAGE(!PageHuge(page), page) in page_hstate()
		 * is going to trigger because a previous call to
		 * remove_hugetlb_page() will set_compound_page_dtor(page,
		 * NULL_COMPOUND_DTOR), so do not use page_hstate() directly.
		 */
		h = size_to_hstate(page_size(page));

		__update_and_free_page(h, page);

		cond_resched();
	}
}
static DECLARE_WORK(free_hpage_work, free_hpage_workfn);

static inline void flush_free_hpage_work(struct hstate *h)
{
	if (free_vmemmap_pages_per_hpage(h))
		flush_work(&free_hpage_work);
}

static void update_and_free_page(struct hstate *h, struct page *page,
				 bool atomic)
{
	if (!HPageVmemmapOptimized(page) || !atomic) {
		__update_and_free_page(h, page);
		return;
	}

	/*
	 * Defer freeing to avoid using GFP_ATOMIC to allocate vmemmap pages.
	 *
	 * Only call schedule_work() if hpage_freelist is previously
	 * empty. Otherwise, schedule_work() had been called but the workfn
	 * hasn't retrieved the list yet.
	 */
	if (llist_add((struct llist_node *)&page->mapping, &hpage_freelist))
		schedule_work(&free_hpage_work);
}

static void update_and_free_pages_bulk(struct hstate *h, struct list_head *list)
{
	struct page *page, *t_page;

	list_for_each_entry_safe(page, t_page, list, lru) {
		update_and_free_page(h, page, false);
		cond_resched();
	}
}

>>>>>>> upstream/android-13
struct hstate *size_to_hstate(unsigned long size)
{
	struct hstate *h;

	for_each_hstate(h) {
		if (huge_page_size(h) == size)
			return h;
	}
	return NULL;
}

<<<<<<< HEAD
/*
 * Test to determine whether the hugepage is "active/in-use" (i.e. being linked
 * to hstate->hugepage_activelist.)
 *
 * This function can be called for tail pages, but never returns true for them.
 */
bool page_huge_active(struct page *page)
{
	return PageHeadHuge(page) && PagePrivate(&page[1]);
}

/* never called for tail page */
void set_page_huge_active(struct page *page)
{
	VM_BUG_ON_PAGE(!PageHeadHuge(page), page);
	SetPagePrivate(&page[1]);
}

static void clear_page_huge_active(struct page *page)
{
	VM_BUG_ON_PAGE(!PageHeadHuge(page), page);
	ClearPagePrivate(&page[1]);
}

/*
 * Internal hugetlb specific page flag. Do not use outside of the hugetlb
 * code
 */
static inline bool PageHugeTemporary(struct page *page)
{
	if (!PageHuge(page))
		return false;

	return (unsigned long)page[2].mapping == -1U;
}

static inline void SetPageHugeTemporary(struct page *page)
{
	page[2].mapping = (void *)-1U;
}

static inline void ClearPageHugeTemporary(struct page *page)
{
	page[2].mapping = NULL;
}

=======
>>>>>>> upstream/android-13
void free_huge_page(struct page *page)
{
	/*
	 * Can't pass hstate in here because it is called from the
	 * compound page destructor.
	 */
	struct hstate *h = page_hstate(page);
	int nid = page_to_nid(page);
<<<<<<< HEAD
	struct hugepage_subpool *spool =
		(struct hugepage_subpool *)page_private(page);
	bool restore_reserve;

	set_page_private(page, 0);
	page->mapping = NULL;
	VM_BUG_ON_PAGE(page_count(page), page);
	VM_BUG_ON_PAGE(page_mapcount(page), page);
	restore_reserve = PagePrivate(page);
	ClearPagePrivate(page);

	/*
	 * If PagePrivate() was set on page, page allocation consumed a
	 * reservation.  If the page was associated with a subpool, there
	 * would have been a page reserved in the subpool before allocation
	 * via hugepage_subpool_get_pages().  Since we are 'restoring' the
	 * reservtion, do not call hugepage_subpool_put_pages() as this will
=======
	struct hugepage_subpool *spool = hugetlb_page_subpool(page);
	bool restore_reserve;
	unsigned long flags;

	VM_BUG_ON_PAGE(page_count(page), page);
	VM_BUG_ON_PAGE(page_mapcount(page), page);

	hugetlb_set_page_subpool(page, NULL);
	page->mapping = NULL;
	restore_reserve = HPageRestoreReserve(page);
	ClearHPageRestoreReserve(page);

	/*
	 * If HPageRestoreReserve was set on page, page allocation consumed a
	 * reservation.  If the page was associated with a subpool, there
	 * would have been a page reserved in the subpool before allocation
	 * via hugepage_subpool_get_pages().  Since we are 'restoring' the
	 * reservation, do not call hugepage_subpool_put_pages() as this will
>>>>>>> upstream/android-13
	 * remove the reserved page from the subpool.
	 */
	if (!restore_reserve) {
		/*
		 * A return code of zero implies that the subpool will be
		 * under its minimum size if the reservation is not restored
		 * after page is free.  Therefore, force restore_reserve
		 * operation.
		 */
		if (hugepage_subpool_put_pages(spool, 1) == 0)
			restore_reserve = true;
	}

<<<<<<< HEAD
	spin_lock(&hugetlb_lock);
	clear_page_huge_active(page);
	hugetlb_cgroup_uncharge_page(hstate_index(h),
				     pages_per_huge_page(h), page);
	if (restore_reserve)
		h->resv_huge_pages++;

	if (PageHugeTemporary(page)) {
		list_del(&page->lru);
		ClearPageHugeTemporary(page);
		update_and_free_page(h, page);
	} else if (h->surplus_huge_pages_node[nid]) {
		/* remove the page from active list */
		list_del(&page->lru);
		update_and_free_page(h, page);
		h->surplus_huge_pages--;
		h->surplus_huge_pages_node[nid]--;
	} else {
		arch_clear_hugepage_flags(page);
		enqueue_huge_page(h, page);
	}
	spin_unlock(&hugetlb_lock);
=======
	spin_lock_irqsave(&hugetlb_lock, flags);
	ClearHPageMigratable(page);
	hugetlb_cgroup_uncharge_page(hstate_index(h),
				     pages_per_huge_page(h), page);
	hugetlb_cgroup_uncharge_page_rsvd(hstate_index(h),
					  pages_per_huge_page(h), page);
	if (restore_reserve)
		h->resv_huge_pages++;

	if (HPageTemporary(page)) {
		remove_hugetlb_page(h, page, false);
		spin_unlock_irqrestore(&hugetlb_lock, flags);
		update_and_free_page(h, page, true);
	} else if (h->surplus_huge_pages_node[nid]) {
		/* remove the page from active list */
		remove_hugetlb_page(h, page, true);
		spin_unlock_irqrestore(&hugetlb_lock, flags);
		update_and_free_page(h, page, true);
	} else {
		arch_clear_hugepage_flags(page);
		enqueue_huge_page(h, page);
		spin_unlock_irqrestore(&hugetlb_lock, flags);
	}
}

/*
 * Must be called with the hugetlb lock held
 */
static void __prep_account_new_huge_page(struct hstate *h, int nid)
{
	lockdep_assert_held(&hugetlb_lock);
	h->nr_huge_pages++;
	h->nr_huge_pages_node[nid]++;
}

static void __prep_new_huge_page(struct hstate *h, struct page *page)
{
	free_huge_page_vmemmap(h, page);
	INIT_LIST_HEAD(&page->lru);
	set_compound_page_dtor(page, HUGETLB_PAGE_DTOR);
	hugetlb_set_page_subpool(page, NULL);
	set_hugetlb_cgroup(page, NULL);
	set_hugetlb_cgroup_rsvd(page, NULL);
>>>>>>> upstream/android-13
}

static void prep_new_huge_page(struct hstate *h, struct page *page, int nid)
{
<<<<<<< HEAD
	INIT_LIST_HEAD(&page->lru);
	set_compound_page_dtor(page, HUGETLB_PAGE_DTOR);
	spin_lock(&hugetlb_lock);
	set_hugetlb_cgroup(page, NULL);
	h->nr_huge_pages++;
	h->nr_huge_pages_node[nid]++;
	ClearPageHugeFreed(page);
	spin_unlock(&hugetlb_lock);
}

static void prep_compound_gigantic_page(struct page *page, unsigned int order)
{
	int i;
=======
	__prep_new_huge_page(h, page);
	spin_lock_irq(&hugetlb_lock);
	__prep_account_new_huge_page(h, nid);
	spin_unlock_irq(&hugetlb_lock);
}

static bool prep_compound_gigantic_page(struct page *page, unsigned int order)
{
	int i, j;
>>>>>>> upstream/android-13
	int nr_pages = 1 << order;
	struct page *p = page + 1;

	/* we rely on prep_new_huge_page to set the destructor */
	set_compound_order(page, order);
	__ClearPageReserved(page);
	__SetPageHead(page);
	for (i = 1; i < nr_pages; i++, p = mem_map_next(p, page, i)) {
		/*
		 * For gigantic hugepages allocated through bootmem at
		 * boot, it's safer to be consistent with the not-gigantic
		 * hugepages and clear the PG_reserved bit from all tail pages
<<<<<<< HEAD
		 * too.  Otherwse drivers using get_user_pages() to access tail
=======
		 * too.  Otherwise drivers using get_user_pages() to access tail
>>>>>>> upstream/android-13
		 * pages may get the reference counting wrong if they see
		 * PG_reserved set on a tail page (despite the head page not
		 * having PG_reserved set).  Enforcing this consistency between
		 * head and tail pages allows drivers to optimize away a check
		 * on the head page when they need know if put_page() is needed
		 * after get_user_pages().
		 */
		__ClearPageReserved(p);
<<<<<<< HEAD
=======
		/*
		 * Subtle and very unlikely
		 *
		 * Gigantic 'page allocators' such as memblock or cma will
		 * return a set of pages with each page ref counted.  We need
		 * to turn this set of pages into a compound page with tail
		 * page ref counts set to zero.  Code such as speculative page
		 * cache adding could take a ref on a 'to be' tail page.
		 * We need to respect any increased ref count, and only set
		 * the ref count to zero if count is currently 1.  If count
		 * is not 1, we return an error.  An error return indicates
		 * the set of pages can not be converted to a gigantic page.
		 * The caller who allocated the pages should then discard the
		 * pages using the appropriate free interface.
		 */
		if (!page_ref_freeze(p, 1)) {
			pr_warn("HugeTLB page can not be used due to unexpected inflated ref count\n");
			goto out_error;
		}
>>>>>>> upstream/android-13
		set_page_count(p, 0);
		set_compound_head(p, page);
	}
	atomic_set(compound_mapcount_ptr(page), -1);
<<<<<<< HEAD
=======
	atomic_set(compound_pincount_ptr(page), 0);
	return true;

out_error:
	/* undo tail page modifications made above */
	p = page + 1;
	for (j = 1; j < i; j++, p = mem_map_next(p, page, j)) {
		clear_compound_head(p);
		set_page_refcounted(p);
	}
	/* need to clear PG_reserved on remaining tail pages  */
	for (; j < nr_pages; j++, p = mem_map_next(p, page, j))
		__ClearPageReserved(p);
	set_compound_order(page, 0);
	page[1].compound_nr = 0;
	__ClearPageHead(page);
	return false;
>>>>>>> upstream/android-13
}

/*
 * PageHuge() only returns true for hugetlbfs pages, but not for normal or
 * transparent huge pages.  See the PageTransHuge() documentation for more
 * details.
 */
int PageHuge(struct page *page)
{
	if (!PageCompound(page))
		return 0;

	page = compound_head(page);
	return page[1].compound_dtor == HUGETLB_PAGE_DTOR;
}
EXPORT_SYMBOL_GPL(PageHuge);

/*
 * PageHeadHuge() only returns true for hugetlbfs head page, but not for
 * normal or transparent huge pages.
 */
int PageHeadHuge(struct page *page_head)
{
	if (!PageHead(page_head))
		return 0;

<<<<<<< HEAD
	return get_compound_page_dtor(page_head) == free_huge_page;
}

pgoff_t __basepage_index(struct page *page)
=======
	return page_head[1].compound_dtor == HUGETLB_PAGE_DTOR;
}

/*
 * Find and lock address space (mapping) in write mode.
 *
 * Upon entry, the page is locked which means that page_mapping() is
 * stable.  Due to locking order, we can only trylock_write.  If we can
 * not get the lock, simply return NULL to caller.
 */
struct address_space *hugetlb_page_mapping_lock_write(struct page *hpage)
{
	struct address_space *mapping = page_mapping(hpage);

	if (!mapping)
		return mapping;

	if (i_mmap_trylock_write(mapping))
		return mapping;

	return NULL;
}

pgoff_t hugetlb_basepage_index(struct page *page)
>>>>>>> upstream/android-13
{
	struct page *page_head = compound_head(page);
	pgoff_t index = page_index(page_head);
	unsigned long compound_idx;

<<<<<<< HEAD
	if (!PageHuge(page_head))
		return page_index(page);

=======
>>>>>>> upstream/android-13
	if (compound_order(page_head) >= MAX_ORDER)
		compound_idx = page_to_pfn(page) - page_to_pfn(page_head);
	else
		compound_idx = page - page_head;

	return (index << compound_order(page_head)) + compound_idx;
}

static struct page *alloc_buddy_huge_page(struct hstate *h,
<<<<<<< HEAD
		gfp_t gfp_mask, int nid, nodemask_t *nmask)
{
	int order = huge_page_order(h);
	struct page *page;

	gfp_mask |= __GFP_COMP|__GFP_RETRY_MAYFAIL|__GFP_NOWARN;
	if (nid == NUMA_NO_NODE)
		nid = numa_mem_id();
	page = __alloc_pages_nodemask(gfp_mask, order, nid, nmask);
=======
		gfp_t gfp_mask, int nid, nodemask_t *nmask,
		nodemask_t *node_alloc_noretry)
{
	int order = huge_page_order(h);
	struct page *page;
	bool alloc_try_hard = true;

	/*
	 * By default we always try hard to allocate the page with
	 * __GFP_RETRY_MAYFAIL flag.  However, if we are allocating pages in
	 * a loop (to adjust global huge page counts) and previous allocation
	 * failed, do not continue to try hard on the same node.  Use the
	 * node_alloc_noretry bitmap to manage this state information.
	 */
	if (node_alloc_noretry && node_isset(nid, *node_alloc_noretry))
		alloc_try_hard = false;
	gfp_mask |= __GFP_COMP|__GFP_NOWARN;
	if (alloc_try_hard)
		gfp_mask |= __GFP_RETRY_MAYFAIL;
	if (nid == NUMA_NO_NODE)
		nid = numa_mem_id();
	page = __alloc_pages(gfp_mask, order, nid, nmask);
>>>>>>> upstream/android-13
	if (page)
		__count_vm_event(HTLB_BUDDY_PGALLOC);
	else
		__count_vm_event(HTLB_BUDDY_PGALLOC_FAIL);

<<<<<<< HEAD
=======
	/*
	 * If we did not specify __GFP_RETRY_MAYFAIL, but still got a page this
	 * indicates an overall state change.  Clear bit so that we resume
	 * normal 'try hard' allocations.
	 */
	if (node_alloc_noretry && page && !alloc_try_hard)
		node_clear(nid, *node_alloc_noretry);

	/*
	 * If we tried hard to get a page but failed, set bit so that
	 * subsequent attempts will not try as hard until there is an
	 * overall state change.
	 */
	if (node_alloc_noretry && !page && alloc_try_hard)
		node_set(nid, *node_alloc_noretry);

>>>>>>> upstream/android-13
	return page;
}

/*
 * Common helper to allocate a fresh hugetlb page. All specific allocators
 * should use this function to get new hugetlb pages
 */
static struct page *alloc_fresh_huge_page(struct hstate *h,
<<<<<<< HEAD
		gfp_t gfp_mask, int nid, nodemask_t *nmask)
{
	struct page *page;

=======
		gfp_t gfp_mask, int nid, nodemask_t *nmask,
		nodemask_t *node_alloc_noretry)
{
	struct page *page;
	bool retry = false;

retry:
>>>>>>> upstream/android-13
	if (hstate_is_gigantic(h))
		page = alloc_gigantic_page(h, gfp_mask, nid, nmask);
	else
		page = alloc_buddy_huge_page(h, gfp_mask,
<<<<<<< HEAD
				nid, nmask);
	if (!page)
		return NULL;

	if (hstate_is_gigantic(h))
		prep_compound_gigantic_page(page, huge_page_order(h));
=======
				nid, nmask, node_alloc_noretry);
	if (!page)
		return NULL;

	if (hstate_is_gigantic(h)) {
		if (!prep_compound_gigantic_page(page, huge_page_order(h))) {
			/*
			 * Rare failure to convert pages to compound page.
			 * Free pages and try again - ONCE!
			 */
			free_gigantic_page(page, huge_page_order(h));
			if (!retry) {
				retry = true;
				goto retry;
			}
			return NULL;
		}
	}
>>>>>>> upstream/android-13
	prep_new_huge_page(h, page, page_to_nid(page));

	return page;
}

/*
 * Allocates a fresh page to the hugetlb allocator pool in the node interleaved
 * manner.
 */
<<<<<<< HEAD
static int alloc_pool_huge_page(struct hstate *h, nodemask_t *nodes_allowed)
=======
static int alloc_pool_huge_page(struct hstate *h, nodemask_t *nodes_allowed,
				nodemask_t *node_alloc_noretry)
>>>>>>> upstream/android-13
{
	struct page *page;
	int nr_nodes, node;
	gfp_t gfp_mask = htlb_alloc_mask(h) | __GFP_THISNODE;

	for_each_node_mask_to_alloc(h, nr_nodes, node, nodes_allowed) {
<<<<<<< HEAD
		page = alloc_fresh_huge_page(h, gfp_mask, node, nodes_allowed);
=======
		page = alloc_fresh_huge_page(h, gfp_mask, node, nodes_allowed,
						node_alloc_noretry);
>>>>>>> upstream/android-13
		if (page)
			break;
	}

	if (!page)
		return 0;

	put_page(page); /* free it into the hugepage allocator */

	return 1;
}

/*
<<<<<<< HEAD
 * Free huge page from pool from next node to free.
 * Attempt to keep persistent huge pages more or less
 * balanced over allowed nodes.
 * Called with hugetlb_lock locked.
 */
static int free_pool_huge_page(struct hstate *h, nodemask_t *nodes_allowed,
							 bool acct_surplus)
{
	int nr_nodes, node;
	int ret = 0;

=======
 * Remove huge page from pool from next node to free.  Attempt to keep
 * persistent huge pages more or less balanced over allowed nodes.
 * This routine only 'removes' the hugetlb page.  The caller must make
 * an additional call to free the page to low level allocators.
 * Called with hugetlb_lock locked.
 */
static struct page *remove_pool_huge_page(struct hstate *h,
						nodemask_t *nodes_allowed,
						 bool acct_surplus)
{
	int nr_nodes, node;
	struct page *page = NULL;

	lockdep_assert_held(&hugetlb_lock);
>>>>>>> upstream/android-13
	for_each_node_mask_to_free(h, nr_nodes, node, nodes_allowed) {
		/*
		 * If we're returning unused surplus pages, only examine
		 * nodes with surplus pages.
		 */
		if ((!acct_surplus || h->surplus_huge_pages_node[node]) &&
		    !list_empty(&h->hugepage_freelists[node])) {
<<<<<<< HEAD
			struct page *page =
				list_entry(h->hugepage_freelists[node].next,
					  struct page, lru);
			list_del(&page->lru);
			h->free_huge_pages--;
			h->free_huge_pages_node[node]--;
			if (acct_surplus) {
				h->surplus_huge_pages--;
				h->surplus_huge_pages_node[node]--;
			}
			update_and_free_page(h, page);
			ret = 1;
=======
			page = list_entry(h->hugepage_freelists[node].next,
					  struct page, lru);
			remove_hugetlb_page(h, page, acct_surplus);
>>>>>>> upstream/android-13
			break;
		}
	}

<<<<<<< HEAD
	return ret;
=======
	return page;
>>>>>>> upstream/android-13
}

/*
 * Dissolve a given free hugepage into free buddy pages. This function does
 * nothing for in-use hugepages and non-hugepages.
 * This function returns values like below:
 *
<<<<<<< HEAD
 *  -EBUSY: failed to dissolved free hugepages or the hugepage is in-use
 *          (allocated or reserved.)
 *       0: successfully dissolved free hugepages or the page is not a
 *          hugepage (considered as already dissolved)
=======
 *  -ENOMEM: failed to allocate vmemmap pages to free the freed hugepages
 *           when the system is under memory pressure and the feature of
 *           freeing unused vmemmap pages associated with each hugetlb page
 *           is enabled.
 *  -EBUSY:  failed to dissolved free hugepages or the hugepage is in-use
 *           (allocated or reserved.)
 *       0:  successfully dissolved free hugepages or the page is not a
 *           hugepage (considered as already dissolved)
>>>>>>> upstream/android-13
 */
int dissolve_free_huge_page(struct page *page)
{
	int rc = -EBUSY;

retry:
	/* Not to disrupt normal path by vainly holding hugetlb_lock */
	if (!PageHuge(page))
		return 0;

<<<<<<< HEAD
	spin_lock(&hugetlb_lock);
=======
	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	if (!PageHuge(page)) {
		rc = 0;
		goto out;
	}

	if (!page_count(page)) {
		struct page *head = compound_head(page);
		struct hstate *h = page_hstate(head);
<<<<<<< HEAD
		int nid = page_to_nid(head);
=======
>>>>>>> upstream/android-13
		if (h->free_huge_pages - h->resv_huge_pages == 0)
			goto out;

		/*
		 * We should make sure that the page is already on the free list
		 * when it is dissolved.
		 */
<<<<<<< HEAD
		if (unlikely(!PageHugeFreed(head))) {
			spin_unlock(&hugetlb_lock);
=======
		if (unlikely(!HPageFreed(head))) {
			spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
			cond_resched();

			/*
			 * Theoretically, we should return -EBUSY when we
			 * encounter this race. In fact, we have a chance
			 * to successfully dissolve the page if we do a
			 * retry. Because the race window is quite small.
			 * If we seize this opportunity, it is an optimization
			 * for increasing the success rate of dissolving page.
			 */
			goto retry;
		}

<<<<<<< HEAD
		/*
		 * Move PageHWPoison flag from head page to the raw error page,
		 * which makes any subpages rather than the error page reusable.
		 */
		if (PageHWPoison(head) && page != head) {
			SetPageHWPoison(page);
			ClearPageHWPoison(head);
		}
		list_del(&head->lru);
		h->free_huge_pages--;
		h->free_huge_pages_node[nid]--;
		h->max_huge_pages--;
		update_and_free_page(h, head);
		rc = 0;
	}
out:
	spin_unlock(&hugetlb_lock);
=======
		remove_hugetlb_page(h, head, false);
		h->max_huge_pages--;
		spin_unlock_irq(&hugetlb_lock);

		/*
		 * Normally update_and_free_page will allocate required vmemmmap
		 * before freeing the page.  update_and_free_page will fail to
		 * free the page if it can not allocate required vmemmap.  We
		 * need to adjust max_huge_pages if the page is not freed.
		 * Attempt to allocate vmemmmap here so that we can take
		 * appropriate action on failure.
		 */
		rc = alloc_huge_page_vmemmap(h, head);
		if (!rc) {
			/*
			 * Move PageHWPoison flag from head page to the raw
			 * error page, which makes any subpages rather than
			 * the error page reusable.
			 */
			if (PageHWPoison(head) && page != head) {
				SetPageHWPoison(page);
				ClearPageHWPoison(head);
			}
			update_and_free_page(h, head, false);
		} else {
			spin_lock_irq(&hugetlb_lock);
			add_hugetlb_page(h, head, false);
			h->max_huge_pages++;
			spin_unlock_irq(&hugetlb_lock);
		}

		return rc;
	}
out:
	spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	return rc;
}

/*
 * Dissolve free hugepages in a given pfn range. Used by memory hotplug to
 * make specified memory blocks removable from the system.
 * Note that this will dissolve a free gigantic hugepage completely, if any
 * part of it lies within the given range.
 * Also note that if dissolve_free_huge_page() returns with an error, all
 * free hugepages that were dissolved before that error are lost.
 */
int dissolve_free_huge_pages(unsigned long start_pfn, unsigned long end_pfn)
{
	unsigned long pfn;
	struct page *page;
	int rc = 0;

	if (!hugepages_supported())
		return rc;

	for (pfn = start_pfn; pfn < end_pfn; pfn += 1 << minimum_order) {
		page = pfn_to_page(pfn);
		rc = dissolve_free_huge_page(page);
		if (rc)
			break;
	}

	return rc;
}

/*
 * Allocates a fresh surplus page from the page allocator.
 */
static struct page *alloc_surplus_huge_page(struct hstate *h, gfp_t gfp_mask,
<<<<<<< HEAD
		int nid, nodemask_t *nmask)
{
	struct page *page = NULL;
=======
		int nid, nodemask_t *nmask, bool zero_ref)
{
	struct page *page = NULL;
	bool retry = false;
>>>>>>> upstream/android-13

	if (hstate_is_gigantic(h))
		return NULL;

<<<<<<< HEAD
	spin_lock(&hugetlb_lock);
	if (h->surplus_huge_pages >= h->nr_overcommit_huge_pages)
		goto out_unlock;
	spin_unlock(&hugetlb_lock);

	page = alloc_fresh_huge_page(h, gfp_mask, nid, nmask);
	if (!page)
		return NULL;

	spin_lock(&hugetlb_lock);
=======
	spin_lock_irq(&hugetlb_lock);
	if (h->surplus_huge_pages >= h->nr_overcommit_huge_pages)
		goto out_unlock;
	spin_unlock_irq(&hugetlb_lock);

retry:
	page = alloc_fresh_huge_page(h, gfp_mask, nid, nmask, NULL);
	if (!page)
		return NULL;

	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	/*
	 * We could have raced with the pool size change.
	 * Double check that and simply deallocate the new page
	 * if we would end up overcommiting the surpluses. Abuse
	 * temporary page to workaround the nasty free_huge_page
	 * codeflow
	 */
	if (h->surplus_huge_pages >= h->nr_overcommit_huge_pages) {
<<<<<<< HEAD
		SetPageHugeTemporary(page);
		spin_unlock(&hugetlb_lock);
		put_page(page);
		return NULL;
	} else {
		h->surplus_huge_pages++;
		h->surplus_huge_pages_node[page_to_nid(page)]++;
	}

out_unlock:
	spin_unlock(&hugetlb_lock);
=======
		SetHPageTemporary(page);
		spin_unlock_irq(&hugetlb_lock);
		put_page(page);
		return NULL;
	}

	if (zero_ref) {
		/*
		 * Caller requires a page with zero ref count.
		 * We will drop ref count here.  If someone else is holding
		 * a ref, the page will be freed when they drop it.  Abuse
		 * temporary page flag to accomplish this.
		 */
		SetHPageTemporary(page);
		if (!put_page_testzero(page)) {
			/*
			 * Unexpected inflated ref count on freshly allocated
			 * huge.  Retry once.
			 */
			pr_info("HugeTLB unexpected inflated ref count on freshly allocated page\n");
			spin_unlock_irq(&hugetlb_lock);
			if (retry)
				return NULL;

			retry = true;
			goto retry;
		}
		ClearHPageTemporary(page);
	}

	h->surplus_huge_pages++;
	h->surplus_huge_pages_node[page_to_nid(page)]++;

out_unlock:
	spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13

	return page;
}

<<<<<<< HEAD
struct page *alloc_migrate_huge_page(struct hstate *h, gfp_t gfp_mask,
=======
static struct page *alloc_migrate_huge_page(struct hstate *h, gfp_t gfp_mask,
>>>>>>> upstream/android-13
				     int nid, nodemask_t *nmask)
{
	struct page *page;

	if (hstate_is_gigantic(h))
		return NULL;

<<<<<<< HEAD
	page = alloc_fresh_huge_page(h, gfp_mask, nid, nmask);
=======
	page = alloc_fresh_huge_page(h, gfp_mask, nid, nmask, NULL);
>>>>>>> upstream/android-13
	if (!page)
		return NULL;

	/*
	 * We do not account these pages as surplus because they are only
	 * temporary and will be released properly on the last reference
	 */
<<<<<<< HEAD
	SetPageHugeTemporary(page);
=======
	SetHPageTemporary(page);
>>>>>>> upstream/android-13

	return page;
}

/*
 * Use the VMA's mpolicy to allocate a huge page from the buddy.
 */
static
struct page *alloc_buddy_huge_page_with_mpol(struct hstate *h,
		struct vm_area_struct *vma, unsigned long addr)
{
<<<<<<< HEAD
	struct page *page;
=======
	struct page *page = NULL;
>>>>>>> upstream/android-13
	struct mempolicy *mpol;
	gfp_t gfp_mask = htlb_alloc_mask(h);
	int nid;
	nodemask_t *nodemask;

	nid = huge_node(vma, addr, gfp_mask, &mpol, &nodemask);
<<<<<<< HEAD
	page = alloc_surplus_huge_page(h, gfp_mask, nid, nodemask);
	mpol_cond_put(mpol);

	return page;
}

/* page migration callback function */
struct page *alloc_huge_page_node(struct hstate *h, int nid)
{
	gfp_t gfp_mask = htlb_alloc_mask(h);
	struct page *page = NULL;

	if (nid != NUMA_NO_NODE)
		gfp_mask |= __GFP_THISNODE;

	spin_lock(&hugetlb_lock);
	if (h->free_huge_pages - h->resv_huge_pages > 0)
		page = dequeue_huge_page_nodemask(h, gfp_mask, nid, NULL);
	spin_unlock(&hugetlb_lock);

	if (!page)
		page = alloc_migrate_huge_page(h, gfp_mask, nid, NULL);

=======
	if (mpol_is_preferred_many(mpol)) {
		gfp_t gfp = gfp_mask | __GFP_NOWARN;

		gfp &=  ~(__GFP_DIRECT_RECLAIM | __GFP_NOFAIL);
		page = alloc_surplus_huge_page(h, gfp, nid, nodemask, false);

		/* Fallback to all nodes if page==NULL */
		nodemask = NULL;
	}

	if (!page)
		page = alloc_surplus_huge_page(h, gfp_mask, nid, nodemask, false);
	mpol_cond_put(mpol);
>>>>>>> upstream/android-13
	return page;
}

/* page migration callback function */
struct page *alloc_huge_page_nodemask(struct hstate *h, int preferred_nid,
<<<<<<< HEAD
		nodemask_t *nmask)
{
	gfp_t gfp_mask = htlb_alloc_mask(h);

	spin_lock(&hugetlb_lock);
=======
		nodemask_t *nmask, gfp_t gfp_mask)
{
	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	if (h->free_huge_pages - h->resv_huge_pages > 0) {
		struct page *page;

		page = dequeue_huge_page_nodemask(h, gfp_mask, preferred_nid, nmask);
		if (page) {
<<<<<<< HEAD
			spin_unlock(&hugetlb_lock);
			return page;
		}
	}
	spin_unlock(&hugetlb_lock);
=======
			spin_unlock_irq(&hugetlb_lock);
			return page;
		}
	}
	spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13

	return alloc_migrate_huge_page(h, gfp_mask, preferred_nid, nmask);
}

/* mempolicy aware migration callback */
struct page *alloc_huge_page_vma(struct hstate *h, struct vm_area_struct *vma,
		unsigned long address)
{
	struct mempolicy *mpol;
	nodemask_t *nodemask;
	struct page *page;
	gfp_t gfp_mask;
	int node;

	gfp_mask = htlb_alloc_mask(h);
	node = huge_node(vma, address, gfp_mask, &mpol, &nodemask);
<<<<<<< HEAD
	page = alloc_huge_page_nodemask(h, node, nodemask);
=======
	page = alloc_huge_page_nodemask(h, node, nodemask, gfp_mask);
>>>>>>> upstream/android-13
	mpol_cond_put(mpol);

	return page;
}

/*
 * Increase the hugetlb pool such that it can accommodate a reservation
 * of size 'delta'.
 */
<<<<<<< HEAD
static int gather_surplus_pages(struct hstate *h, int delta)
{
	struct list_head surplus_list;
	struct page *page, *tmp;
	int ret, i;
	int needed, allocated;
	bool alloc_ok = true;

=======
static int gather_surplus_pages(struct hstate *h, long delta)
	__must_hold(&hugetlb_lock)
{
	struct list_head surplus_list;
	struct page *page, *tmp;
	int ret;
	long i;
	long needed, allocated;
	bool alloc_ok = true;

	lockdep_assert_held(&hugetlb_lock);
>>>>>>> upstream/android-13
	needed = (h->resv_huge_pages + delta) - h->free_huge_pages;
	if (needed <= 0) {
		h->resv_huge_pages += delta;
		return 0;
	}

	allocated = 0;
	INIT_LIST_HEAD(&surplus_list);

	ret = -ENOMEM;
retry:
<<<<<<< HEAD
	spin_unlock(&hugetlb_lock);
	for (i = 0; i < needed; i++) {
		page = alloc_surplus_huge_page(h, htlb_alloc_mask(h),
				NUMA_NO_NODE, NULL);
=======
	spin_unlock_irq(&hugetlb_lock);
	for (i = 0; i < needed; i++) {
		page = alloc_surplus_huge_page(h, htlb_alloc_mask(h),
				NUMA_NO_NODE, NULL, true);
>>>>>>> upstream/android-13
		if (!page) {
			alloc_ok = false;
			break;
		}
		list_add(&page->lru, &surplus_list);
		cond_resched();
	}
	allocated += i;

	/*
	 * After retaking hugetlb_lock, we need to recalculate 'needed'
	 * because either resv_huge_pages or free_huge_pages may have changed.
	 */
<<<<<<< HEAD
	spin_lock(&hugetlb_lock);
=======
	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	needed = (h->resv_huge_pages + delta) -
			(h->free_huge_pages + allocated);
	if (needed > 0) {
		if (alloc_ok)
			goto retry;
		/*
		 * We were not able to allocate enough pages to
		 * satisfy the entire reservation so we free what
		 * we've allocated so far.
		 */
		goto free;
	}
	/*
	 * The surplus_list now contains _at_least_ the number of extra pages
	 * needed to accommodate the reservation.  Add the appropriate number
	 * of pages to the hugetlb pool and free the extras back to the buddy
	 * allocator.  Commit the entire reservation here to prevent another
	 * process from stealing the pages as they are added to the pool but
	 * before they are reserved.
	 */
	needed += allocated;
	h->resv_huge_pages += delta;
	ret = 0;

	/* Free the needed pages to the hugetlb pool */
	list_for_each_entry_safe(page, tmp, &surplus_list, lru) {
		if ((--needed) < 0)
			break;
<<<<<<< HEAD
		/*
		 * This page is now managed by the hugetlb allocator and has
		 * no users -- drop the buddy allocator's reference.
		 */
		put_page_testzero(page);
		VM_BUG_ON_PAGE(page_count(page), page);
		enqueue_huge_page(h, page);
	}
free:
	spin_unlock(&hugetlb_lock);

	/* Free unnecessary surplus pages to the buddy allocator */
	list_for_each_entry_safe(page, tmp, &surplus_list, lru)
		put_page(page);
	spin_lock(&hugetlb_lock);
=======
		/* Add the page to the hugetlb allocator */
		enqueue_huge_page(h, page);
	}
free:
	spin_unlock_irq(&hugetlb_lock);

	/*
	 * Free unnecessary surplus pages to the buddy allocator.
	 * Pages have no ref count, call free_huge_page directly.
	 */
	list_for_each_entry_safe(page, tmp, &surplus_list, lru)
		free_huge_page(page);
	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13

	return ret;
}

/*
 * This routine has two main purposes:
 * 1) Decrement the reservation count (resv_huge_pages) by the value passed
 *    in unused_resv_pages.  This corresponds to the prior adjustments made
 *    to the associated reservation map.
 * 2) Free any unused surplus pages that may have been allocated to satisfy
 *    the reservation.  As many as unused_resv_pages may be freed.
<<<<<<< HEAD
 *
 * Called with hugetlb_lock held.  However, the lock could be dropped (and
 * reacquired) during calls to cond_resched_lock.  Whenever dropping the lock,
 * we must make sure nobody else can claim pages we are in the process of
 * freeing.  Do this by ensuring resv_huge_page always is greater than the
 * number of huge pages we plan to free when dropping the lock.
=======
>>>>>>> upstream/android-13
 */
static void return_unused_surplus_pages(struct hstate *h,
					unsigned long unused_resv_pages)
{
	unsigned long nr_pages;
<<<<<<< HEAD
=======
	struct page *page;
	LIST_HEAD(page_list);

	lockdep_assert_held(&hugetlb_lock);
	/* Uncommit the reservation */
	h->resv_huge_pages -= unused_resv_pages;
>>>>>>> upstream/android-13

	/* Cannot return gigantic pages currently */
	if (hstate_is_gigantic(h))
		goto out;

	/*
	 * Part (or even all) of the reservation could have been backed
	 * by pre-allocated pages. Only free surplus pages.
	 */
	nr_pages = min(unused_resv_pages, h->surplus_huge_pages);

	/*
	 * We want to release as many surplus pages as possible, spread
	 * evenly across all nodes with memory. Iterate across these nodes
	 * until we can no longer free unreserved surplus pages. This occurs
	 * when the nodes with surplus pages have no free pages.
<<<<<<< HEAD
	 * free_pool_huge_page() will balance the the freed pages across the
	 * on-line nodes with memory and will handle the hstate accounting.
	 *
	 * Note that we decrement resv_huge_pages as we free the pages.  If
	 * we drop the lock, resv_huge_pages will still be sufficiently large
	 * to cover subsequent pages we may free.
	 */
	while (nr_pages--) {
		h->resv_huge_pages--;
		unused_resv_pages--;
		if (!free_pool_huge_page(h, &node_states[N_MEMORY], 1))
			goto out;
		cond_resched_lock(&hugetlb_lock);
	}

out:
	/* Fully uncommit the reservation */
	h->resv_huge_pages -= unused_resv_pages;
=======
	 * remove_pool_huge_page() will balance the freed pages across the
	 * on-line nodes with memory and will handle the hstate accounting.
	 */
	while (nr_pages--) {
		page = remove_pool_huge_page(h, &node_states[N_MEMORY], 1);
		if (!page)
			goto out;

		list_add(&page->lru, &page_list);
	}

out:
	spin_unlock_irq(&hugetlb_lock);
	update_and_free_pages_bulk(h, &page_list);
	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
}


/*
 * vma_needs_reservation, vma_commit_reservation and vma_end_reservation
 * are used by the huge page allocation routines to manage reservations.
 *
 * vma_needs_reservation is called to determine if the huge page at addr
 * within the vma has an associated reservation.  If a reservation is
 * needed, the value 1 is returned.  The caller is then responsible for
 * managing the global reservation and subpool usage counts.  After
 * the huge page has been allocated, vma_commit_reservation is called
 * to add the page to the reservation map.  If the page allocation fails,
 * the reservation must be ended instead of committed.  vma_end_reservation
 * is called in such cases.
 *
 * In the normal case, vma_commit_reservation returns the same value
 * as the preceding vma_needs_reservation call.  The only time this
 * is not the case is if a reserve map was changed between calls.  It
 * is the responsibility of the caller to notice the difference and
 * take appropriate action.
 *
 * vma_add_reservation is used in error paths where a reservation must
 * be restored when a newly allocated huge page must be freed.  It is
 * to be called after calling vma_needs_reservation to determine if a
 * reservation exists.
<<<<<<< HEAD
=======
 *
 * vma_del_reservation is used in error paths where an entry in the reserve
 * map was created during huge page allocation and must be removed.  It is to
 * be called after calling vma_needs_reservation to determine if a reservation
 * exists.
>>>>>>> upstream/android-13
 */
enum vma_resv_mode {
	VMA_NEEDS_RESV,
	VMA_COMMIT_RESV,
	VMA_END_RESV,
	VMA_ADD_RESV,
<<<<<<< HEAD
=======
	VMA_DEL_RESV,
>>>>>>> upstream/android-13
};
static long __vma_reservation_common(struct hstate *h,
				struct vm_area_struct *vma, unsigned long addr,
				enum vma_resv_mode mode)
{
	struct resv_map *resv;
	pgoff_t idx;
	long ret;
<<<<<<< HEAD
=======
	long dummy_out_regions_needed;
>>>>>>> upstream/android-13

	resv = vma_resv_map(vma);
	if (!resv)
		return 1;

	idx = vma_hugecache_offset(h, vma, addr);
	switch (mode) {
	case VMA_NEEDS_RESV:
<<<<<<< HEAD
		ret = region_chg(resv, idx, idx + 1);
		break;
	case VMA_COMMIT_RESV:
		ret = region_add(resv, idx, idx + 1);
		break;
	case VMA_END_RESV:
		region_abort(resv, idx, idx + 1);
		ret = 0;
		break;
	case VMA_ADD_RESV:
		if (vma->vm_flags & VM_MAYSHARE)
			ret = region_add(resv, idx, idx + 1);
		else {
			region_abort(resv, idx, idx + 1);
			ret = region_del(resv, idx, idx + 1);
		}
		break;
=======
		ret = region_chg(resv, idx, idx + 1, &dummy_out_regions_needed);
		/* We assume that vma_reservation_* routines always operate on
		 * 1 page, and that adding to resv map a 1 page entry can only
		 * ever require 1 region.
		 */
		VM_BUG_ON(dummy_out_regions_needed != 1);
		break;
	case VMA_COMMIT_RESV:
		ret = region_add(resv, idx, idx + 1, 1, NULL, NULL);
		/* region_add calls of range 1 should never fail. */
		VM_BUG_ON(ret < 0);
		break;
	case VMA_END_RESV:
		region_abort(resv, idx, idx + 1, 1);
		ret = 0;
		break;
	case VMA_ADD_RESV:
		if (vma->vm_flags & VM_MAYSHARE) {
			ret = region_add(resv, idx, idx + 1, 1, NULL, NULL);
			/* region_add calls of range 1 should never fail. */
			VM_BUG_ON(ret < 0);
		} else {
			region_abort(resv, idx, idx + 1, 1);
			ret = region_del(resv, idx, idx + 1);
		}
		break;
	case VMA_DEL_RESV:
		if (vma->vm_flags & VM_MAYSHARE) {
			region_abort(resv, idx, idx + 1, 1);
			ret = region_del(resv, idx, idx + 1);
		} else {
			ret = region_add(resv, idx, idx + 1, 1, NULL, NULL);
			/* region_add calls of range 1 should never fail. */
			VM_BUG_ON(ret < 0);
		}
		break;
>>>>>>> upstream/android-13
	default:
		BUG();
	}

<<<<<<< HEAD
	if (vma->vm_flags & VM_MAYSHARE)
		return ret;
	else if (is_vma_resv_set(vma, HPAGE_RESV_OWNER) && ret >= 0) {
		/*
		 * In most cases, reserves always exist for private mappings.
		 * However, a file associated with mapping could have been
		 * hole punched or truncated after reserves were consumed.
		 * As subsequent fault on such a range will not use reserves.
		 * Subtle - The reserve map for private mappings has the
		 * opposite meaning than that of shared mappings.  If NO
		 * entry is in the reserve map, it means a reservation exists.
		 * If an entry exists in the reserve map, it means the
		 * reservation has already been consumed.  As a result, the
		 * return value of this routine is the opposite of the
		 * value returned from reserve map manipulation routines above.
		 */
		if (ret)
			return 0;
		else
			return 1;
	}
	else
		return ret < 0 ? ret : 0;
=======
	if (vma->vm_flags & VM_MAYSHARE || mode == VMA_DEL_RESV)
		return ret;
	/*
	 * We know private mapping must have HPAGE_RESV_OWNER set.
	 *
	 * In most cases, reserves always exist for private mappings.
	 * However, a file associated with mapping could have been
	 * hole punched or truncated after reserves were consumed.
	 * As subsequent fault on such a range will not use reserves.
	 * Subtle - The reserve map for private mappings has the
	 * opposite meaning than that of shared mappings.  If NO
	 * entry is in the reserve map, it means a reservation exists.
	 * If an entry exists in the reserve map, it means the
	 * reservation has already been consumed.  As a result, the
	 * return value of this routine is the opposite of the
	 * value returned from reserve map manipulation routines above.
	 */
	if (ret > 0)
		return 0;
	if (ret == 0)
		return 1;
	return ret;
>>>>>>> upstream/android-13
}

static long vma_needs_reservation(struct hstate *h,
			struct vm_area_struct *vma, unsigned long addr)
{
	return __vma_reservation_common(h, vma, addr, VMA_NEEDS_RESV);
}

static long vma_commit_reservation(struct hstate *h,
			struct vm_area_struct *vma, unsigned long addr)
{
	return __vma_reservation_common(h, vma, addr, VMA_COMMIT_RESV);
}

static void vma_end_reservation(struct hstate *h,
			struct vm_area_struct *vma, unsigned long addr)
{
	(void)__vma_reservation_common(h, vma, addr, VMA_END_RESV);
}

static long vma_add_reservation(struct hstate *h,
			struct vm_area_struct *vma, unsigned long addr)
{
	return __vma_reservation_common(h, vma, addr, VMA_ADD_RESV);
}

<<<<<<< HEAD
/*
 * This routine is called to restore a reservation on error paths.  In the
 * specific error paths, a huge page was allocated (via alloc_huge_page)
 * and is about to be freed.  If a reservation for the page existed,
 * alloc_huge_page would have consumed the reservation and set PagePrivate
 * in the newly allocated page.  When the page is freed via free_huge_page,
 * the global reservation count will be incremented if PagePrivate is set.
 * However, free_huge_page can not adjust the reserve map.  Adjust the
 * reserve map here to be consistent with global reserve count adjustments
 * to be made by free_huge_page.
 */
static void restore_reserve_on_error(struct hstate *h,
			struct vm_area_struct *vma, unsigned long address,
			struct page *page)
{
	if (unlikely(PagePrivate(page))) {
		long rc = vma_needs_reservation(h, vma, address);

		if (unlikely(rc < 0)) {
			/*
			 * Rare out of memory condition in reserve map
			 * manipulation.  Clear PagePrivate so that
=======
static long vma_del_reservation(struct hstate *h,
			struct vm_area_struct *vma, unsigned long addr)
{
	return __vma_reservation_common(h, vma, addr, VMA_DEL_RESV);
}

/*
 * This routine is called to restore reservation information on error paths.
 * It should ONLY be called for pages allocated via alloc_huge_page(), and
 * the hugetlb mutex should remain held when calling this routine.
 *
 * It handles two specific cases:
 * 1) A reservation was in place and the page consumed the reservation.
 *    HPageRestoreReserve is set in the page.
 * 2) No reservation was in place for the page, so HPageRestoreReserve is
 *    not set.  However, alloc_huge_page always updates the reserve map.
 *
 * In case 1, free_huge_page later in the error path will increment the
 * global reserve count.  But, free_huge_page does not have enough context
 * to adjust the reservation map.  This case deals primarily with private
 * mappings.  Adjust the reserve map here to be consistent with global
 * reserve count adjustments to be made by free_huge_page.  Make sure the
 * reserve map indicates there is a reservation present.
 *
 * In case 2, simply undo reserve map modifications done by alloc_huge_page.
 */
void restore_reserve_on_error(struct hstate *h, struct vm_area_struct *vma,
			unsigned long address, struct page *page)
{
	long rc = vma_needs_reservation(h, vma, address);

	if (HPageRestoreReserve(page)) {
		if (unlikely(rc < 0))
			/*
			 * Rare out of memory condition in reserve map
			 * manipulation.  Clear HPageRestoreReserve so that
>>>>>>> upstream/android-13
			 * global reserve count will not be incremented
			 * by free_huge_page.  This will make it appear
			 * as though the reservation for this page was
			 * consumed.  This may prevent the task from
			 * faulting in the page at a later time.  This
			 * is better than inconsistent global huge page
			 * accounting of reserve counts.
			 */
<<<<<<< HEAD
			ClearPagePrivate(page);
		} else if (rc) {
			rc = vma_add_reservation(h, vma, address);
			if (unlikely(rc < 0))
				/*
				 * See above comment about rare out of
				 * memory condition.
				 */
				ClearPagePrivate(page);
		} else
			vma_end_reservation(h, vma, address);
	}
}

=======
			ClearHPageRestoreReserve(page);
		else if (rc)
			(void)vma_add_reservation(h, vma, address);
		else
			vma_end_reservation(h, vma, address);
	} else {
		if (!rc) {
			/*
			 * This indicates there is an entry in the reserve map
			 * not added by alloc_huge_page.  We know it was added
			 * before the alloc_huge_page call, otherwise
			 * HPageRestoreReserve would be set on the page.
			 * Remove the entry so that a subsequent allocation
			 * does not consume a reservation.
			 */
			rc = vma_del_reservation(h, vma, address);
			if (rc < 0)
				/*
				 * VERY rare out of memory condition.  Since
				 * we can not delete the entry, set
				 * HPageRestoreReserve so that the reserve
				 * count will be incremented when the page
				 * is freed.  This reserve will be consumed
				 * on a subsequent allocation.
				 */
				SetHPageRestoreReserve(page);
		} else if (rc < 0) {
			/*
			 * Rare out of memory condition from
			 * vma_needs_reservation call.  Memory allocation is
			 * only attempted if a new entry is needed.  Therefore,
			 * this implies there is not an entry in the
			 * reserve map.
			 *
			 * For shared mappings, no entry in the map indicates
			 * no reservation.  We are done.
			 */
			if (!(vma->vm_flags & VM_MAYSHARE))
				/*
				 * For private mappings, no entry indicates
				 * a reservation is present.  Since we can
				 * not add an entry, set SetHPageRestoreReserve
				 * on the page so reserve count will be
				 * incremented when freed.  This reserve will
				 * be consumed on a subsequent allocation.
				 */
				SetHPageRestoreReserve(page);
		} else
			/*
			 * No reservation present, do nothing
			 */
			 vma_end_reservation(h, vma, address);
	}
}

/*
 * alloc_and_dissolve_huge_page - Allocate a new page and dissolve the old one
 * @h: struct hstate old page belongs to
 * @old_page: Old page to dissolve
 * @list: List to isolate the page in case we need to
 * Returns 0 on success, otherwise negated error.
 */
static int alloc_and_dissolve_huge_page(struct hstate *h, struct page *old_page,
					struct list_head *list)
{
	gfp_t gfp_mask = htlb_alloc_mask(h) | __GFP_THISNODE;
	int nid = page_to_nid(old_page);
	bool alloc_retry = false;
	struct page *new_page;
	int ret = 0;

	/*
	 * Before dissolving the page, we need to allocate a new one for the
	 * pool to remain stable.  Here, we allocate the page and 'prep' it
	 * by doing everything but actually updating counters and adding to
	 * the pool.  This simplifies and let us do most of the processing
	 * under the lock.
	 */
alloc_retry:
	new_page = alloc_buddy_huge_page(h, gfp_mask, nid, NULL, NULL);
	if (!new_page)
		return -ENOMEM;
	/*
	 * If all goes well, this page will be directly added to the free
	 * list in the pool.  For this the ref count needs to be zero.
	 * Attempt to drop now, and retry once if needed.  It is VERY
	 * unlikely there is another ref on the page.
	 *
	 * If someone else has a reference to the page, it will be freed
	 * when they drop their ref.  Abuse temporary page flag to accomplish
	 * this.  Retry once if there is an inflated ref count.
	 */
	SetHPageTemporary(new_page);
	if (!put_page_testzero(new_page)) {
		if (alloc_retry)
			return -EBUSY;

		alloc_retry = true;
		goto alloc_retry;
	}
	ClearHPageTemporary(new_page);

	__prep_new_huge_page(h, new_page);

retry:
	spin_lock_irq(&hugetlb_lock);
	if (!PageHuge(old_page)) {
		/*
		 * Freed from under us. Drop new_page too.
		 */
		goto free_new;
	} else if (page_count(old_page)) {
		/*
		 * Someone has grabbed the page, try to isolate it here.
		 * Fail with -EBUSY if not possible.
		 */
		spin_unlock_irq(&hugetlb_lock);
		if (!isolate_huge_page(old_page, list))
			ret = -EBUSY;
		spin_lock_irq(&hugetlb_lock);
		goto free_new;
	} else if (!HPageFreed(old_page)) {
		/*
		 * Page's refcount is 0 but it has not been enqueued in the
		 * freelist yet. Race window is small, so we can succeed here if
		 * we retry.
		 */
		spin_unlock_irq(&hugetlb_lock);
		cond_resched();
		goto retry;
	} else {
		/*
		 * Ok, old_page is still a genuine free hugepage. Remove it from
		 * the freelist and decrease the counters. These will be
		 * incremented again when calling __prep_account_new_huge_page()
		 * and enqueue_huge_page() for new_page. The counters will remain
		 * stable since this happens under the lock.
		 */
		remove_hugetlb_page(h, old_page, false);

		/*
		 * Ref count on new page is already zero as it was dropped
		 * earlier.  It can be directly added to the pool free list.
		 */
		__prep_account_new_huge_page(h, nid);
		enqueue_huge_page(h, new_page);

		/*
		 * Pages have been replaced, we can safely free the old one.
		 */
		spin_unlock_irq(&hugetlb_lock);
		update_and_free_page(h, old_page, false);
	}

	return ret;

free_new:
	spin_unlock_irq(&hugetlb_lock);
	/* Page has a zero ref count, but needs a ref to be freed */
	set_page_refcounted(new_page);
	update_and_free_page(h, new_page, false);

	return ret;
}

int isolate_or_dissolve_huge_page(struct page *page, struct list_head *list)
{
	struct hstate *h;
	struct page *head;
	int ret = -EBUSY;

	/*
	 * The page might have been dissolved from under our feet, so make sure
	 * to carefully check the state under the lock.
	 * Return success when racing as if we dissolved the page ourselves.
	 */
	spin_lock_irq(&hugetlb_lock);
	if (PageHuge(page)) {
		head = compound_head(page);
		h = page_hstate(head);
	} else {
		spin_unlock_irq(&hugetlb_lock);
		return 0;
	}
	spin_unlock_irq(&hugetlb_lock);

	/*
	 * Fence off gigantic pages as there is a cyclic dependency between
	 * alloc_contig_range and them. Return -ENOMEM as this has the effect
	 * of bailing out right away without further retrying.
	 */
	if (hstate_is_gigantic(h))
		return -ENOMEM;

	if (page_count(head) && isolate_huge_page(head, list))
		ret = 0;
	else if (!page_count(head))
		ret = alloc_and_dissolve_huge_page(h, head, list);

	return ret;
}

>>>>>>> upstream/android-13
struct page *alloc_huge_page(struct vm_area_struct *vma,
				    unsigned long addr, int avoid_reserve)
{
	struct hugepage_subpool *spool = subpool_vma(vma);
	struct hstate *h = hstate_vma(vma);
	struct page *page;
	long map_chg, map_commit;
	long gbl_chg;
	int ret, idx;
	struct hugetlb_cgroup *h_cg;
<<<<<<< HEAD
=======
	bool deferred_reserve;
>>>>>>> upstream/android-13

	idx = hstate_index(h);
	/*
	 * Examine the region/reserve map to determine if the process
	 * has a reservation for the page to be allocated.  A return
	 * code of zero indicates a reservation exists (no change).
	 */
	map_chg = gbl_chg = vma_needs_reservation(h, vma, addr);
	if (map_chg < 0)
		return ERR_PTR(-ENOMEM);

	/*
	 * Processes that did not create the mapping will have no
	 * reserves as indicated by the region/reserve map. Check
	 * that the allocation will not exceed the subpool limit.
	 * Allocations for MAP_NORESERVE mappings also need to be
	 * checked against any subpool limit.
	 */
	if (map_chg || avoid_reserve) {
		gbl_chg = hugepage_subpool_get_pages(spool, 1);
		if (gbl_chg < 0) {
			vma_end_reservation(h, vma, addr);
			return ERR_PTR(-ENOSPC);
		}

		/*
		 * Even though there was no reservation in the region/reserve
		 * map, there could be reservations associated with the
		 * subpool that can be used.  This would be indicated if the
		 * return value of hugepage_subpool_get_pages() is zero.
		 * However, if avoid_reserve is specified we still avoid even
		 * the subpool reservations.
		 */
		if (avoid_reserve)
			gbl_chg = 1;
	}

<<<<<<< HEAD
	ret = hugetlb_cgroup_charge_cgroup(idx, pages_per_huge_page(h), &h_cg);
	if (ret)
		goto out_subpool_put;

	spin_lock(&hugetlb_lock);
=======
	/* If this allocation is not consuming a reservation, charge it now.
	 */
	deferred_reserve = map_chg || avoid_reserve;
	if (deferred_reserve) {
		ret = hugetlb_cgroup_charge_cgroup_rsvd(
			idx, pages_per_huge_page(h), &h_cg);
		if (ret)
			goto out_subpool_put;
	}

	ret = hugetlb_cgroup_charge_cgroup(idx, pages_per_huge_page(h), &h_cg);
	if (ret)
		goto out_uncharge_cgroup_reservation;

	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	/*
	 * glb_chg is passed to indicate whether or not a page must be taken
	 * from the global free pool (global change).  gbl_chg == 0 indicates
	 * a reservation exists for the allocation.
	 */
	page = dequeue_huge_page_vma(h, vma, addr, avoid_reserve, gbl_chg);
	if (!page) {
<<<<<<< HEAD
		spin_unlock(&hugetlb_lock);
=======
		spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
		page = alloc_buddy_huge_page_with_mpol(h, vma, addr);
		if (!page)
			goto out_uncharge_cgroup;
		if (!avoid_reserve && vma_has_reserves(vma, gbl_chg)) {
<<<<<<< HEAD
			SetPagePrivate(page);
			h->resv_huge_pages--;
		}
		spin_lock(&hugetlb_lock);
		list_move(&page->lru, &h->hugepage_activelist);
		/* Fall through */
	}
	hugetlb_cgroup_commit_charge(idx, pages_per_huge_page(h), h_cg, page);
	spin_unlock(&hugetlb_lock);

	set_page_private(page, (unsigned long)spool);
=======
			SetHPageRestoreReserve(page);
			h->resv_huge_pages--;
		}
		spin_lock_irq(&hugetlb_lock);
		list_add(&page->lru, &h->hugepage_activelist);
		/* Fall through */
	}
	hugetlb_cgroup_commit_charge(idx, pages_per_huge_page(h), h_cg, page);
	/* If allocation is not consuming a reservation, also store the
	 * hugetlb_cgroup pointer on the page.
	 */
	if (deferred_reserve) {
		hugetlb_cgroup_commit_charge_rsvd(idx, pages_per_huge_page(h),
						  h_cg, page);
	}

	spin_unlock_irq(&hugetlb_lock);

	hugetlb_set_page_subpool(page, spool);
>>>>>>> upstream/android-13

	map_commit = vma_commit_reservation(h, vma, addr);
	if (unlikely(map_chg > map_commit)) {
		/*
		 * The page was added to the reservation map between
		 * vma_needs_reservation and vma_commit_reservation.
		 * This indicates a race with hugetlb_reserve_pages.
		 * Adjust for the subpool count incremented above AND
		 * in hugetlb_reserve_pages for the same page.  Also,
		 * the reservation count added in hugetlb_reserve_pages
		 * no longer applies.
		 */
		long rsv_adjust;

		rsv_adjust = hugepage_subpool_put_pages(spool, 1);
		hugetlb_acct_memory(h, -rsv_adjust);
<<<<<<< HEAD
=======
		if (deferred_reserve)
			hugetlb_cgroup_uncharge_page_rsvd(hstate_index(h),
					pages_per_huge_page(h), page);
>>>>>>> upstream/android-13
	}
	return page;

out_uncharge_cgroup:
	hugetlb_cgroup_uncharge_cgroup(idx, pages_per_huge_page(h), h_cg);
<<<<<<< HEAD
=======
out_uncharge_cgroup_reservation:
	if (deferred_reserve)
		hugetlb_cgroup_uncharge_cgroup_rsvd(idx, pages_per_huge_page(h),
						    h_cg);
>>>>>>> upstream/android-13
out_subpool_put:
	if (map_chg || avoid_reserve)
		hugepage_subpool_put_pages(spool, 1);
	vma_end_reservation(h, vma, addr);
	return ERR_PTR(-ENOSPC);
}

int alloc_bootmem_huge_page(struct hstate *h)
	__attribute__ ((weak, alias("__alloc_bootmem_huge_page")));
int __alloc_bootmem_huge_page(struct hstate *h)
{
	struct huge_bootmem_page *m;
	int nr_nodes, node;

	for_each_node_mask_to_alloc(h, nr_nodes, node, &node_states[N_MEMORY]) {
		void *addr;

<<<<<<< HEAD
		addr = memblock_virt_alloc_try_nid_raw(
				huge_page_size(h), huge_page_size(h),
				0, BOOTMEM_ALLOC_ACCESSIBLE, node);
=======
		addr = memblock_alloc_try_nid_raw(
				huge_page_size(h), huge_page_size(h),
				0, MEMBLOCK_ALLOC_ACCESSIBLE, node);
>>>>>>> upstream/android-13
		if (addr) {
			/*
			 * Use the beginning of the huge page to store the
			 * huge_bootmem_page struct (until gather_bootmem
			 * puts them into the mem_map).
			 */
			m = addr;
			goto found;
		}
	}
	return 0;

found:
	BUG_ON(!IS_ALIGNED(virt_to_phys(m), huge_page_size(h)));
	/* Put them into a private list first because mem_map is not up yet */
	INIT_LIST_HEAD(&m->list);
	list_add(&m->list, &huge_boot_pages);
	m->hstate = h;
	return 1;
}

<<<<<<< HEAD
static void __init prep_compound_huge_page(struct page *page,
		unsigned int order)
{
	if (unlikely(order > (MAX_ORDER - 1)))
		prep_compound_gigantic_page(page, order);
	else
		prep_compound_page(page, order);
}

/* Put bootmem huge pages into the standard lists after mem_map is up */
=======
/*
 * Put bootmem huge pages into the standard lists after mem_map is up.
 * Note: This only applies to gigantic (order > MAX_ORDER) pages.
 */
>>>>>>> upstream/android-13
static void __init gather_bootmem_prealloc(void)
{
	struct huge_bootmem_page *m;

	list_for_each_entry(m, &huge_boot_pages, list) {
		struct page *page = virt_to_page(m);
		struct hstate *h = m->hstate;

<<<<<<< HEAD
		WARN_ON(page_count(page) != 1);
		prep_compound_huge_page(page, h->order);
		WARN_ON(PageReserved(page));
		prep_new_huge_page(h, page, page_to_nid(page));
		put_page(page); /* free it into the hugepage allocator */

		/*
		 * If we had gigantic hugepages allocated at boot time, we need
		 * to restore the 'stolen' pages to totalram_pages in order to
		 * fix confusing memory reports from free(1) and another
		 * side-effects, like CommitLimit going negative.
		 */
		if (hstate_is_gigantic(h))
			adjust_managed_page_count(page, 1 << h->order);
=======
		VM_BUG_ON(!hstate_is_gigantic(h));
		WARN_ON(page_count(page) != 1);
		if (prep_compound_gigantic_page(page, huge_page_order(h))) {
			WARN_ON(PageReserved(page));
			prep_new_huge_page(h, page, page_to_nid(page));
			put_page(page); /* add to the hugepage allocator */
		} else {
			/* VERY unlikely inflated ref count on a tail page */
			free_gigantic_page(page, huge_page_order(h));
		}

		/*
		 * We need to restore the 'stolen' pages to totalram_pages
		 * in order to fix confusing memory reports from free(1) and
		 * other side-effects, like CommitLimit going negative.
		 */
		adjust_managed_page_count(page, pages_per_huge_page(h));
>>>>>>> upstream/android-13
		cond_resched();
	}
}

static void __init hugetlb_hstate_alloc_pages(struct hstate *h)
{
	unsigned long i;
<<<<<<< HEAD

	for (i = 0; i < h->max_huge_pages; ++i) {
		if (hstate_is_gigantic(h)) {
			if (!alloc_bootmem_huge_page(h))
				break;
		} else if (!alloc_pool_huge_page(h,
					 &node_states[N_MEMORY]))
=======
	nodemask_t *node_alloc_noretry;

	if (!hstate_is_gigantic(h)) {
		/*
		 * Bit mask controlling how hard we retry per-node allocations.
		 * Ignore errors as lower level routines can deal with
		 * node_alloc_noretry == NULL.  If this kmalloc fails at boot
		 * time, we are likely in bigger trouble.
		 */
		node_alloc_noretry = kmalloc(sizeof(*node_alloc_noretry),
						GFP_KERNEL);
	} else {
		/* allocations done at boot time */
		node_alloc_noretry = NULL;
	}

	/* bit mask controlling how hard we retry per-node allocations */
	if (node_alloc_noretry)
		nodes_clear(*node_alloc_noretry);

	for (i = 0; i < h->max_huge_pages; ++i) {
		if (hstate_is_gigantic(h)) {
			if (hugetlb_cma_size) {
				pr_warn_once("HugeTLB: hugetlb_cma is enabled, skip boot time allocation\n");
				goto free;
			}
			if (!alloc_bootmem_huge_page(h))
				break;
		} else if (!alloc_pool_huge_page(h,
					 &node_states[N_MEMORY],
					 node_alloc_noretry))
>>>>>>> upstream/android-13
			break;
		cond_resched();
	}
	if (i < h->max_huge_pages) {
		char buf[32];

		string_get_size(huge_page_size(h), 1, STRING_UNITS_2, buf, 32);
		pr_warn("HugeTLB: allocating %lu of page size %s failed.  Only allocated %lu hugepages.\n",
			h->max_huge_pages, buf, i);
		h->max_huge_pages = i;
	}
<<<<<<< HEAD
=======
free:
	kfree(node_alloc_noretry);
>>>>>>> upstream/android-13
}

static void __init hugetlb_init_hstates(void)
{
	struct hstate *h;

	for_each_hstate(h) {
		if (minimum_order > huge_page_order(h))
			minimum_order = huge_page_order(h);

		/* oversize hugepages were init'ed in early boot */
		if (!hstate_is_gigantic(h))
			hugetlb_hstate_alloc_pages(h);
	}
	VM_BUG_ON(minimum_order == UINT_MAX);
}

static void __init report_hugepages(void)
{
	struct hstate *h;

	for_each_hstate(h) {
		char buf[32];

		string_get_size(huge_page_size(h), 1, STRING_UNITS_2, buf, 32);
		pr_info("HugeTLB registered %s page size, pre-allocated %ld pages\n",
			buf, h->free_huge_pages);
	}
}

#ifdef CONFIG_HIGHMEM
static void try_to_free_low(struct hstate *h, unsigned long count,
						nodemask_t *nodes_allowed)
{
	int i;
<<<<<<< HEAD

	if (hstate_is_gigantic(h))
		return;

=======
	LIST_HEAD(page_list);

	lockdep_assert_held(&hugetlb_lock);
	if (hstate_is_gigantic(h))
		return;

	/*
	 * Collect pages to be freed on a list, and free after dropping lock
	 */
>>>>>>> upstream/android-13
	for_each_node_mask(i, *nodes_allowed) {
		struct page *page, *next;
		struct list_head *freel = &h->hugepage_freelists[i];
		list_for_each_entry_safe(page, next, freel, lru) {
			if (count >= h->nr_huge_pages)
<<<<<<< HEAD
				return;
			if (PageHighMem(page))
				continue;
			list_del(&page->lru);
			update_and_free_page(h, page);
			h->free_huge_pages--;
			h->free_huge_pages_node[page_to_nid(page)]--;
		}
	}
=======
				goto out;
			if (PageHighMem(page))
				continue;
			remove_hugetlb_page(h, page, false);
			list_add(&page->lru, &page_list);
		}
	}

out:
	spin_unlock_irq(&hugetlb_lock);
	update_and_free_pages_bulk(h, &page_list);
	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
}
#else
static inline void try_to_free_low(struct hstate *h, unsigned long count,
						nodemask_t *nodes_allowed)
{
}
#endif

/*
 * Increment or decrement surplus_huge_pages.  Keep node-specific counters
 * balanced by operating on them in a round-robin fashion.
 * Returns 1 if an adjustment was made.
 */
static int adjust_pool_surplus(struct hstate *h, nodemask_t *nodes_allowed,
				int delta)
{
	int nr_nodes, node;

<<<<<<< HEAD
=======
	lockdep_assert_held(&hugetlb_lock);
>>>>>>> upstream/android-13
	VM_BUG_ON(delta != -1 && delta != 1);

	if (delta < 0) {
		for_each_node_mask_to_alloc(h, nr_nodes, node, nodes_allowed) {
			if (h->surplus_huge_pages_node[node])
				goto found;
		}
	} else {
		for_each_node_mask_to_free(h, nr_nodes, node, nodes_allowed) {
			if (h->surplus_huge_pages_node[node] <
					h->nr_huge_pages_node[node])
				goto found;
		}
	}
	return 0;

found:
	h->surplus_huge_pages += delta;
	h->surplus_huge_pages_node[node] += delta;
	return 1;
}

#define persistent_huge_pages(h) (h->nr_huge_pages - h->surplus_huge_pages)
<<<<<<< HEAD
static unsigned long set_max_huge_pages(struct hstate *h, unsigned long count,
						nodemask_t *nodes_allowed)
{
	unsigned long min_count, ret;

	if (hstate_is_gigantic(h) && !gigantic_page_supported())
		return h->max_huge_pages;
=======
static int set_max_huge_pages(struct hstate *h, unsigned long count, int nid,
			      nodemask_t *nodes_allowed)
{
	unsigned long min_count, ret;
	struct page *page;
	LIST_HEAD(page_list);
	NODEMASK_ALLOC(nodemask_t, node_alloc_noretry, GFP_KERNEL);

	/*
	 * Bit mask controlling how hard we retry per-node allocations.
	 * If we can not allocate the bit mask, do not attempt to allocate
	 * the requested huge pages.
	 */
	if (node_alloc_noretry)
		nodes_clear(*node_alloc_noretry);
	else
		return -ENOMEM;

	/*
	 * resize_lock mutex prevents concurrent adjustments to number of
	 * pages in hstate via the proc/sysfs interfaces.
	 */
	mutex_lock(&h->resize_lock);
	flush_free_hpage_work(h);
	spin_lock_irq(&hugetlb_lock);

	/*
	 * Check for a node specific request.
	 * Changing node specific huge page count may require a corresponding
	 * change to the global count.  In any case, the passed node mask
	 * (nodes_allowed) will restrict alloc/free to the specified node.
	 */
	if (nid != NUMA_NO_NODE) {
		unsigned long old_count = count;

		count += h->nr_huge_pages - h->nr_huge_pages_node[nid];
		/*
		 * User may have specified a large count value which caused the
		 * above calculation to overflow.  In this case, they wanted
		 * to allocate as many huge pages as possible.  Set count to
		 * largest possible value to align with their intention.
		 */
		if (count < old_count)
			count = ULONG_MAX;
	}

	/*
	 * Gigantic pages runtime allocation depend on the capability for large
	 * page range allocation.
	 * If the system does not provide this feature, return an error when
	 * the user tries to allocate gigantic pages but let the user free the
	 * boottime allocated gigantic pages.
	 */
	if (hstate_is_gigantic(h) && !IS_ENABLED(CONFIG_CONTIG_ALLOC)) {
		if (count > persistent_huge_pages(h)) {
			spin_unlock_irq(&hugetlb_lock);
			mutex_unlock(&h->resize_lock);
			NODEMASK_FREE(node_alloc_noretry);
			return -EINVAL;
		}
		/* Fall through to decrease pool */
	}
>>>>>>> upstream/android-13

	/*
	 * Increase the pool size
	 * First take pages out of surplus state.  Then make up the
	 * remaining difference by allocating fresh huge pages.
	 *
	 * We might race with alloc_surplus_huge_page() here and be unable
	 * to convert a surplus huge page to a normal huge page. That is
	 * not critical, though, it just means the overall size of the
	 * pool might be one hugepage larger than it needs to be, but
	 * within all the constraints specified by the sysctls.
	 */
<<<<<<< HEAD
	spin_lock(&hugetlb_lock);
=======
>>>>>>> upstream/android-13
	while (h->surplus_huge_pages && count > persistent_huge_pages(h)) {
		if (!adjust_pool_surplus(h, nodes_allowed, -1))
			break;
	}

	while (count > persistent_huge_pages(h)) {
		/*
		 * If this allocation races such that we no longer need the
		 * page, free_huge_page will handle it by freeing the page
		 * and reducing the surplus.
		 */
<<<<<<< HEAD
		spin_unlock(&hugetlb_lock);
=======
		spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13

		/* yield cpu to avoid soft lockup */
		cond_resched();

<<<<<<< HEAD
		ret = alloc_pool_huge_page(h, nodes_allowed);
		spin_lock(&hugetlb_lock);
=======
		ret = alloc_pool_huge_page(h, nodes_allowed,
						node_alloc_noretry);
		spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
		if (!ret)
			goto out;

		/* Bail for signals. Probably ctrl-c from user */
		if (signal_pending(current))
			goto out;
	}

	/*
	 * Decrease the pool size
	 * First return free pages to the buddy allocator (being careful
	 * to keep enough around to satisfy reservations).  Then place
	 * pages into surplus state as needed so the pool will shrink
	 * to the desired size as pages become free.
	 *
	 * By placing pages into the surplus state independent of the
	 * overcommit value, we are allowing the surplus pool size to
	 * exceed overcommit. There are few sane options here. Since
	 * alloc_surplus_huge_page() is checking the global counter,
	 * though, we'll note that we're not allowed to exceed surplus
	 * and won't grow the pool anywhere else. Not until one of the
	 * sysctls are changed, or the surplus pages go out of use.
	 */
	min_count = h->resv_huge_pages + h->nr_huge_pages - h->free_huge_pages;
	min_count = max(count, min_count);
	try_to_free_low(h, min_count, nodes_allowed);
<<<<<<< HEAD
	while (min_count < persistent_huge_pages(h)) {
		if (!free_pool_huge_page(h, nodes_allowed, 0))
			break;
		cond_resched_lock(&hugetlb_lock);
	}
=======

	/*
	 * Collect pages to be removed on list without dropping lock
	 */
	while (min_count < persistent_huge_pages(h)) {
		page = remove_pool_huge_page(h, nodes_allowed, 0);
		if (!page)
			break;

		list_add(&page->lru, &page_list);
	}
	/* free the pages after dropping lock */
	spin_unlock_irq(&hugetlb_lock);
	update_and_free_pages_bulk(h, &page_list);
	flush_free_hpage_work(h);
	spin_lock_irq(&hugetlb_lock);

>>>>>>> upstream/android-13
	while (count < persistent_huge_pages(h)) {
		if (!adjust_pool_surplus(h, nodes_allowed, 1))
			break;
	}
out:
<<<<<<< HEAD
	ret = persistent_huge_pages(h);
	spin_unlock(&hugetlb_lock);
	return ret;
=======
	h->max_huge_pages = persistent_huge_pages(h);
	spin_unlock_irq(&hugetlb_lock);
	mutex_unlock(&h->resize_lock);

	NODEMASK_FREE(node_alloc_noretry);

	return 0;
>>>>>>> upstream/android-13
}

#define HSTATE_ATTR_RO(_name) \
	static struct kobj_attribute _name##_attr = __ATTR_RO(_name)

#define HSTATE_ATTR(_name) \
	static struct kobj_attribute _name##_attr = \
		__ATTR(_name, 0644, _name##_show, _name##_store)

static struct kobject *hugepages_kobj;
static struct kobject *hstate_kobjs[HUGE_MAX_HSTATE];

static struct hstate *kobj_to_node_hstate(struct kobject *kobj, int *nidp);

static struct hstate *kobj_to_hstate(struct kobject *kobj, int *nidp)
{
	int i;

	for (i = 0; i < HUGE_MAX_HSTATE; i++)
		if (hstate_kobjs[i] == kobj) {
			if (nidp)
				*nidp = NUMA_NO_NODE;
			return &hstates[i];
		}

	return kobj_to_node_hstate(kobj, nidp);
}

static ssize_t nr_hugepages_show_common(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	struct hstate *h;
	unsigned long nr_huge_pages;
	int nid;

	h = kobj_to_hstate(kobj, &nid);
	if (nid == NUMA_NO_NODE)
		nr_huge_pages = h->nr_huge_pages;
	else
		nr_huge_pages = h->nr_huge_pages_node[nid];

<<<<<<< HEAD
	return sprintf(buf, "%lu\n", nr_huge_pages);
=======
	return sysfs_emit(buf, "%lu\n", nr_huge_pages);
>>>>>>> upstream/android-13
}

static ssize_t __nr_hugepages_store_common(bool obey_mempolicy,
					   struct hstate *h, int nid,
					   unsigned long count, size_t len)
{
	int err;
<<<<<<< HEAD
	NODEMASK_ALLOC(nodemask_t, nodes_allowed, GFP_KERNEL | __GFP_NORETRY);

	if (hstate_is_gigantic(h) && !gigantic_page_supported()) {
		err = -EINVAL;
		goto out;
	}
=======
	nodemask_t nodes_allowed, *n_mask;

	if (hstate_is_gigantic(h) && !gigantic_page_runtime_supported())
		return -EINVAL;
>>>>>>> upstream/android-13

	if (nid == NUMA_NO_NODE) {
		/*
		 * global hstate attribute
		 */
		if (!(obey_mempolicy &&
<<<<<<< HEAD
				init_nodemask_of_mempolicy(nodes_allowed))) {
			NODEMASK_FREE(nodes_allowed);
			nodes_allowed = &node_states[N_MEMORY];
		}
	} else if (nodes_allowed) {
		/*
		 * per node hstate attribute: adjust count to global,
		 * but restrict alloc/free to the specified node.
		 */
		count += h->nr_huge_pages - h->nr_huge_pages_node[nid];
		init_nodemask_of_node(nodes_allowed, nid);
	} else
		nodes_allowed = &node_states[N_MEMORY];

	h->max_huge_pages = set_max_huge_pages(h, count, nodes_allowed);

	if (nodes_allowed != &node_states[N_MEMORY])
		NODEMASK_FREE(nodes_allowed);

	return len;
out:
	NODEMASK_FREE(nodes_allowed);
	return err;
=======
				init_nodemask_of_mempolicy(&nodes_allowed)))
			n_mask = &node_states[N_MEMORY];
		else
			n_mask = &nodes_allowed;
	} else {
		/*
		 * Node specific request.  count adjustment happens in
		 * set_max_huge_pages() after acquiring hugetlb_lock.
		 */
		init_nodemask_of_node(&nodes_allowed, nid);
		n_mask = &nodes_allowed;
	}

	err = set_max_huge_pages(h, count, nid, n_mask);

	return err ? err : len;
>>>>>>> upstream/android-13
}

static ssize_t nr_hugepages_store_common(bool obey_mempolicy,
					 struct kobject *kobj, const char *buf,
					 size_t len)
{
	struct hstate *h;
	unsigned long count;
	int nid;
	int err;

	err = kstrtoul(buf, 10, &count);
	if (err)
		return err;

	h = kobj_to_hstate(kobj, &nid);
	return __nr_hugepages_store_common(obey_mempolicy, h, nid, count, len);
}

static ssize_t nr_hugepages_show(struct kobject *kobj,
				       struct kobj_attribute *attr, char *buf)
{
	return nr_hugepages_show_common(kobj, attr, buf);
}

static ssize_t nr_hugepages_store(struct kobject *kobj,
	       struct kobj_attribute *attr, const char *buf, size_t len)
{
	return nr_hugepages_store_common(false, kobj, buf, len);
}
HSTATE_ATTR(nr_hugepages);

#ifdef CONFIG_NUMA

/*
 * hstate attribute for optionally mempolicy-based constraint on persistent
 * huge page alloc/free.
 */
static ssize_t nr_hugepages_mempolicy_show(struct kobject *kobj,
<<<<<<< HEAD
				       struct kobj_attribute *attr, char *buf)
=======
					   struct kobj_attribute *attr,
					   char *buf)
>>>>>>> upstream/android-13
{
	return nr_hugepages_show_common(kobj, attr, buf);
}

static ssize_t nr_hugepages_mempolicy_store(struct kobject *kobj,
	       struct kobj_attribute *attr, const char *buf, size_t len)
{
	return nr_hugepages_store_common(true, kobj, buf, len);
}
HSTATE_ATTR(nr_hugepages_mempolicy);
#endif


static ssize_t nr_overcommit_hugepages_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	struct hstate *h = kobj_to_hstate(kobj, NULL);
<<<<<<< HEAD
	return sprintf(buf, "%lu\n", h->nr_overcommit_huge_pages);
=======
	return sysfs_emit(buf, "%lu\n", h->nr_overcommit_huge_pages);
>>>>>>> upstream/android-13
}

static ssize_t nr_overcommit_hugepages_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	int err;
	unsigned long input;
	struct hstate *h = kobj_to_hstate(kobj, NULL);

	if (hstate_is_gigantic(h))
		return -EINVAL;

	err = kstrtoul(buf, 10, &input);
	if (err)
		return err;

<<<<<<< HEAD
	spin_lock(&hugetlb_lock);
	h->nr_overcommit_huge_pages = input;
	spin_unlock(&hugetlb_lock);
=======
	spin_lock_irq(&hugetlb_lock);
	h->nr_overcommit_huge_pages = input;
	spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13

	return count;
}
HSTATE_ATTR(nr_overcommit_hugepages);

static ssize_t free_hugepages_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	struct hstate *h;
	unsigned long free_huge_pages;
	int nid;

	h = kobj_to_hstate(kobj, &nid);
	if (nid == NUMA_NO_NODE)
		free_huge_pages = h->free_huge_pages;
	else
		free_huge_pages = h->free_huge_pages_node[nid];

<<<<<<< HEAD
	return sprintf(buf, "%lu\n", free_huge_pages);
=======
	return sysfs_emit(buf, "%lu\n", free_huge_pages);
>>>>>>> upstream/android-13
}
HSTATE_ATTR_RO(free_hugepages);

static ssize_t resv_hugepages_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	struct hstate *h = kobj_to_hstate(kobj, NULL);
<<<<<<< HEAD
	return sprintf(buf, "%lu\n", h->resv_huge_pages);
=======
	return sysfs_emit(buf, "%lu\n", h->resv_huge_pages);
>>>>>>> upstream/android-13
}
HSTATE_ATTR_RO(resv_hugepages);

static ssize_t surplus_hugepages_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	struct hstate *h;
	unsigned long surplus_huge_pages;
	int nid;

	h = kobj_to_hstate(kobj, &nid);
	if (nid == NUMA_NO_NODE)
		surplus_huge_pages = h->surplus_huge_pages;
	else
		surplus_huge_pages = h->surplus_huge_pages_node[nid];

<<<<<<< HEAD
	return sprintf(buf, "%lu\n", surplus_huge_pages);
=======
	return sysfs_emit(buf, "%lu\n", surplus_huge_pages);
>>>>>>> upstream/android-13
}
HSTATE_ATTR_RO(surplus_hugepages);

static struct attribute *hstate_attrs[] = {
	&nr_hugepages_attr.attr,
	&nr_overcommit_hugepages_attr.attr,
	&free_hugepages_attr.attr,
	&resv_hugepages_attr.attr,
	&surplus_hugepages_attr.attr,
#ifdef CONFIG_NUMA
	&nr_hugepages_mempolicy_attr.attr,
#endif
	NULL,
};

static const struct attribute_group hstate_attr_group = {
	.attrs = hstate_attrs,
};

static int hugetlb_sysfs_add_hstate(struct hstate *h, struct kobject *parent,
				    struct kobject **hstate_kobjs,
				    const struct attribute_group *hstate_attr_group)
{
	int retval;
	int hi = hstate_index(h);

	hstate_kobjs[hi] = kobject_create_and_add(h->name, parent);
	if (!hstate_kobjs[hi])
		return -ENOMEM;

	retval = sysfs_create_group(hstate_kobjs[hi], hstate_attr_group);
	if (retval) {
		kobject_put(hstate_kobjs[hi]);
		hstate_kobjs[hi] = NULL;
	}

	return retval;
}

static void __init hugetlb_sysfs_init(void)
{
	struct hstate *h;
	int err;

	hugepages_kobj = kobject_create_and_add("hugepages", mm_kobj);
	if (!hugepages_kobj)
		return;

	for_each_hstate(h) {
		err = hugetlb_sysfs_add_hstate(h, hugepages_kobj,
					 hstate_kobjs, &hstate_attr_group);
		if (err)
<<<<<<< HEAD
			pr_err("Hugetlb: Unable to add hstate %s", h->name);
=======
			pr_err("HugeTLB: Unable to add hstate %s", h->name);
>>>>>>> upstream/android-13
	}
}

#ifdef CONFIG_NUMA

/*
 * node_hstate/s - associate per node hstate attributes, via their kobjects,
 * with node devices in node_devices[] using a parallel array.  The array
 * index of a node device or _hstate == node id.
 * This is here to avoid any static dependency of the node device driver, in
 * the base kernel, on the hugetlb module.
 */
struct node_hstate {
	struct kobject		*hugepages_kobj;
	struct kobject		*hstate_kobjs[HUGE_MAX_HSTATE];
};
static struct node_hstate node_hstates[MAX_NUMNODES];

/*
 * A subset of global hstate attributes for node devices
 */
static struct attribute *per_node_hstate_attrs[] = {
	&nr_hugepages_attr.attr,
	&free_hugepages_attr.attr,
	&surplus_hugepages_attr.attr,
	NULL,
};

static const struct attribute_group per_node_hstate_attr_group = {
	.attrs = per_node_hstate_attrs,
};

/*
 * kobj_to_node_hstate - lookup global hstate for node device hstate attr kobj.
 * Returns node id via non-NULL nidp.
 */
static struct hstate *kobj_to_node_hstate(struct kobject *kobj, int *nidp)
{
	int nid;

	for (nid = 0; nid < nr_node_ids; nid++) {
		struct node_hstate *nhs = &node_hstates[nid];
		int i;
		for (i = 0; i < HUGE_MAX_HSTATE; i++)
			if (nhs->hstate_kobjs[i] == kobj) {
				if (nidp)
					*nidp = nid;
				return &hstates[i];
			}
	}

	BUG();
	return NULL;
}

/*
 * Unregister hstate attributes from a single node device.
 * No-op if no hstate attributes attached.
 */
static void hugetlb_unregister_node(struct node *node)
{
	struct hstate *h;
	struct node_hstate *nhs = &node_hstates[node->dev.id];

	if (!nhs->hugepages_kobj)
		return;		/* no hstate attributes */

	for_each_hstate(h) {
		int idx = hstate_index(h);
		if (nhs->hstate_kobjs[idx]) {
			kobject_put(nhs->hstate_kobjs[idx]);
			nhs->hstate_kobjs[idx] = NULL;
		}
	}

	kobject_put(nhs->hugepages_kobj);
	nhs->hugepages_kobj = NULL;
}


/*
 * Register hstate attributes for a single node device.
 * No-op if attributes already registered.
 */
static void hugetlb_register_node(struct node *node)
{
	struct hstate *h;
	struct node_hstate *nhs = &node_hstates[node->dev.id];
	int err;

	if (nhs->hugepages_kobj)
		return;		/* already allocated */

	nhs->hugepages_kobj = kobject_create_and_add("hugepages",
							&node->dev.kobj);
	if (!nhs->hugepages_kobj)
		return;

	for_each_hstate(h) {
		err = hugetlb_sysfs_add_hstate(h, nhs->hugepages_kobj,
						nhs->hstate_kobjs,
						&per_node_hstate_attr_group);
		if (err) {
<<<<<<< HEAD
			pr_err("Hugetlb: Unable to add hstate %s for node %d\n",
=======
			pr_err("HugeTLB: Unable to add hstate %s for node %d\n",
>>>>>>> upstream/android-13
				h->name, node->dev.id);
			hugetlb_unregister_node(node);
			break;
		}
	}
}

/*
 * hugetlb init time:  register hstate attributes for all registered node
 * devices of nodes that have memory.  All on-line nodes should have
 * registered their associated device by this time.
 */
static void __init hugetlb_register_all_nodes(void)
{
	int nid;

	for_each_node_state(nid, N_MEMORY) {
		struct node *node = node_devices[nid];
		if (node->dev.id == nid)
			hugetlb_register_node(node);
	}

	/*
	 * Let the node device driver know we're here so it can
	 * [un]register hstate attributes on node hotplug.
	 */
	register_hugetlbfs_with_node(hugetlb_register_node,
				     hugetlb_unregister_node);
}
#else	/* !CONFIG_NUMA */

static struct hstate *kobj_to_node_hstate(struct kobject *kobj, int *nidp)
{
	BUG();
	if (nidp)
		*nidp = -1;
	return NULL;
}

static void hugetlb_register_all_nodes(void) { }

#endif

static int __init hugetlb_init(void)
{
	int i;

<<<<<<< HEAD
	if (!hugepages_supported())
		return 0;

	if (!size_to_hstate(default_hstate_size)) {
		if (default_hstate_size != 0) {
			pr_err("HugeTLB: unsupported default_hugepagesz %lu. Reverting to %lu\n",
			       default_hstate_size, HPAGE_SIZE);
		}

		default_hstate_size = HPAGE_SIZE;
		if (!size_to_hstate(default_hstate_size))
			hugetlb_add_hstate(HUGETLB_PAGE_ORDER);
	}
	default_hstate_idx = hstate_index(size_to_hstate(default_hstate_size));
	if (default_hstate_max_huge_pages) {
		if (!default_hstate.max_huge_pages)
			default_hstate.max_huge_pages = default_hstate_max_huge_pages;
	}

=======
	BUILD_BUG_ON(sizeof_field(struct page, private) * BITS_PER_BYTE <
			__NR_HPAGEFLAGS);

	if (!hugepages_supported()) {
		if (hugetlb_max_hstate || default_hstate_max_huge_pages)
			pr_warn("HugeTLB: huge pages not supported, ignoring associated command-line parameters\n");
		return 0;
	}

	/*
	 * Make sure HPAGE_SIZE (HUGETLB_PAGE_ORDER) hstate exists.  Some
	 * architectures depend on setup being done here.
	 */
	hugetlb_add_hstate(HUGETLB_PAGE_ORDER);
	if (!parsed_default_hugepagesz) {
		/*
		 * If we did not parse a default huge page size, set
		 * default_hstate_idx to HPAGE_SIZE hstate. And, if the
		 * number of huge pages for this default size was implicitly
		 * specified, set that here as well.
		 * Note that the implicit setting will overwrite an explicit
		 * setting.  A warning will be printed in this case.
		 */
		default_hstate_idx = hstate_index(size_to_hstate(HPAGE_SIZE));
		if (default_hstate_max_huge_pages) {
			if (default_hstate.max_huge_pages) {
				char buf[32];

				string_get_size(huge_page_size(&default_hstate),
					1, STRING_UNITS_2, buf, 32);
				pr_warn("HugeTLB: Ignoring hugepages=%lu associated with %s page size\n",
					default_hstate.max_huge_pages, buf);
				pr_warn("HugeTLB: Using hugepages=%lu for number of default huge pages\n",
					default_hstate_max_huge_pages);
			}
			default_hstate.max_huge_pages =
				default_hstate_max_huge_pages;
		}
	}

	hugetlb_cma_check();
>>>>>>> upstream/android-13
	hugetlb_init_hstates();
	gather_bootmem_prealloc();
	report_hugepages();

	hugetlb_sysfs_init();
	hugetlb_register_all_nodes();
	hugetlb_cgroup_file_init();

#ifdef CONFIG_SMP
	num_fault_mutexes = roundup_pow_of_two(8 * num_possible_cpus());
#else
	num_fault_mutexes = 1;
#endif
	hugetlb_fault_mutex_table =
		kmalloc_array(num_fault_mutexes, sizeof(struct mutex),
			      GFP_KERNEL);
	BUG_ON(!hugetlb_fault_mutex_table);

	for (i = 0; i < num_fault_mutexes; i++)
		mutex_init(&hugetlb_fault_mutex_table[i]);
	return 0;
}
subsys_initcall(hugetlb_init);

<<<<<<< HEAD
/* Should be called on processing a hugepagesz=... option */
void __init hugetlb_bad_size(void)
{
	parsed_valid_hugepagesz = false;
=======
/* Overwritten by architectures with more huge page sizes */
bool __init __attribute((weak)) arch_hugetlb_valid_size(unsigned long size)
{
	return size == HPAGE_SIZE;
>>>>>>> upstream/android-13
}

void __init hugetlb_add_hstate(unsigned int order)
{
	struct hstate *h;
	unsigned long i;

	if (size_to_hstate(PAGE_SIZE << order)) {
<<<<<<< HEAD
		pr_warn("hugepagesz= specified twice, ignoring\n");
=======
>>>>>>> upstream/android-13
		return;
	}
	BUG_ON(hugetlb_max_hstate >= HUGE_MAX_HSTATE);
	BUG_ON(order == 0);
	h = &hstates[hugetlb_max_hstate++];
<<<<<<< HEAD
	h->order = order;
	h->mask = ~((1ULL << (order + PAGE_SHIFT)) - 1);
	h->nr_huge_pages = 0;
	h->free_huge_pages = 0;
=======
	mutex_init(&h->resize_lock);
	h->order = order;
	h->mask = ~(huge_page_size(h) - 1);
>>>>>>> upstream/android-13
	for (i = 0; i < MAX_NUMNODES; ++i)
		INIT_LIST_HEAD(&h->hugepage_freelists[i]);
	INIT_LIST_HEAD(&h->hugepage_activelist);
	h->next_nid_to_alloc = first_memory_node;
	h->next_nid_to_free = first_memory_node;
	snprintf(h->name, HSTATE_NAME_LEN, "hugepages-%lukB",
					huge_page_size(h)/1024);
<<<<<<< HEAD
=======
	hugetlb_vmemmap_init(h);
>>>>>>> upstream/android-13

	parsed_hstate = h;
}

<<<<<<< HEAD
static int __init hugetlb_nrpages_setup(char *s)
=======
/*
 * hugepages command line processing
 * hugepages normally follows a valid hugepagsz or default_hugepagsz
 * specification.  If not, ignore the hugepages value.  hugepages can also
 * be the first huge page command line  option in which case it implicitly
 * specifies the number of huge pages for the default size.
 */
static int __init hugepages_setup(char *s)
>>>>>>> upstream/android-13
{
	unsigned long *mhp;
	static unsigned long *last_mhp;

	if (!parsed_valid_hugepagesz) {
<<<<<<< HEAD
		pr_warn("hugepages = %s preceded by "
			"an unsupported hugepagesz, ignoring\n", s);
		parsed_valid_hugepagesz = true;
		return 1;
	}
	/*
	 * !hugetlb_max_hstate means we haven't parsed a hugepagesz= parameter yet,
	 * so this hugepages= parameter goes to the "default hstate".
=======
		pr_warn("HugeTLB: hugepages=%s does not follow a valid hugepagesz, ignoring\n", s);
		parsed_valid_hugepagesz = true;
		return 0;
	}

	/*
	 * !hugetlb_max_hstate means we haven't parsed a hugepagesz= parameter
	 * yet, so this hugepages= parameter goes to the "default hstate".
	 * Otherwise, it goes with the previously parsed hugepagesz or
	 * default_hugepagesz.
>>>>>>> upstream/android-13
	 */
	else if (!hugetlb_max_hstate)
		mhp = &default_hstate_max_huge_pages;
	else
		mhp = &parsed_hstate->max_huge_pages;

	if (mhp == last_mhp) {
<<<<<<< HEAD
		pr_warn("hugepages= specified twice without interleaving hugepagesz=, ignoring\n");
		return 1;
=======
		pr_warn("HugeTLB: hugepages= specified twice without interleaving hugepagesz=, ignoring hugepages=%s\n", s);
		return 0;
>>>>>>> upstream/android-13
	}

	if (sscanf(s, "%lu", mhp) <= 0)
		*mhp = 0;

	/*
	 * Global state is always initialized later in hugetlb_init.
<<<<<<< HEAD
	 * But we need to allocate >= MAX_ORDER hstates here early to still
	 * use the bootmem allocator.
	 */
	if (hugetlb_max_hstate && parsed_hstate->order >= MAX_ORDER)
=======
	 * But we need to allocate gigantic hstates here early to still
	 * use the bootmem allocator.
	 */
	if (hugetlb_max_hstate && hstate_is_gigantic(parsed_hstate))
>>>>>>> upstream/android-13
		hugetlb_hstate_alloc_pages(parsed_hstate);

	last_mhp = mhp;

	return 1;
}
<<<<<<< HEAD
__setup("hugepages=", hugetlb_nrpages_setup);

static int __init hugetlb_default_setup(char *s)
{
	default_hstate_size = memparse(s, &s);
	return 1;
}
__setup("default_hugepagesz=", hugetlb_default_setup);

static unsigned int cpuset_mems_nr(unsigned int *array)
{
	int node;
	unsigned int nr = 0;

	for_each_node_mask(node, cpuset_current_mems_allowed)
		nr += array[node];
=======
__setup("hugepages=", hugepages_setup);

/*
 * hugepagesz command line processing
 * A specific huge page size can only be specified once with hugepagesz.
 * hugepagesz is followed by hugepages on the command line.  The global
 * variable 'parsed_valid_hugepagesz' is used to determine if prior
 * hugepagesz argument was valid.
 */
static int __init hugepagesz_setup(char *s)
{
	unsigned long size;
	struct hstate *h;

	parsed_valid_hugepagesz = false;
	size = (unsigned long)memparse(s, NULL);

	if (!arch_hugetlb_valid_size(size)) {
		pr_err("HugeTLB: unsupported hugepagesz=%s\n", s);
		return 0;
	}

	h = size_to_hstate(size);
	if (h) {
		/*
		 * hstate for this size already exists.  This is normally
		 * an error, but is allowed if the existing hstate is the
		 * default hstate.  More specifically, it is only allowed if
		 * the number of huge pages for the default hstate was not
		 * previously specified.
		 */
		if (!parsed_default_hugepagesz ||  h != &default_hstate ||
		    default_hstate.max_huge_pages) {
			pr_warn("HugeTLB: hugepagesz=%s specified twice, ignoring\n", s);
			return 0;
		}

		/*
		 * No need to call hugetlb_add_hstate() as hstate already
		 * exists.  But, do set parsed_hstate so that a following
		 * hugepages= parameter will be applied to this hstate.
		 */
		parsed_hstate = h;
		parsed_valid_hugepagesz = true;
		return 1;
	}

	hugetlb_add_hstate(ilog2(size) - PAGE_SHIFT);
	parsed_valid_hugepagesz = true;
	return 1;
}
__setup("hugepagesz=", hugepagesz_setup);

/*
 * default_hugepagesz command line input
 * Only one instance of default_hugepagesz allowed on command line.
 */
static int __init default_hugepagesz_setup(char *s)
{
	unsigned long size;

	parsed_valid_hugepagesz = false;
	if (parsed_default_hugepagesz) {
		pr_err("HugeTLB: default_hugepagesz previously specified, ignoring %s\n", s);
		return 0;
	}

	size = (unsigned long)memparse(s, NULL);

	if (!arch_hugetlb_valid_size(size)) {
		pr_err("HugeTLB: unsupported default_hugepagesz=%s\n", s);
		return 0;
	}

	hugetlb_add_hstate(ilog2(size) - PAGE_SHIFT);
	parsed_valid_hugepagesz = true;
	parsed_default_hugepagesz = true;
	default_hstate_idx = hstate_index(size_to_hstate(size));

	/*
	 * The number of default huge pages (for this size) could have been
	 * specified as the first hugetlb parameter: hugepages=X.  If so,
	 * then default_hstate_max_huge_pages is set.  If the default huge
	 * page size is gigantic (>= MAX_ORDER), then the pages must be
	 * allocated here from bootmem allocator.
	 */
	if (default_hstate_max_huge_pages) {
		default_hstate.max_huge_pages = default_hstate_max_huge_pages;
		if (hstate_is_gigantic(&default_hstate))
			hugetlb_hstate_alloc_pages(&default_hstate);
		default_hstate_max_huge_pages = 0;
	}

	return 1;
}
__setup("default_hugepagesz=", default_hugepagesz_setup);

static unsigned int allowed_mems_nr(struct hstate *h)
{
	int node;
	unsigned int nr = 0;
	nodemask_t *mpol_allowed;
	unsigned int *array = h->free_huge_pages_node;
	gfp_t gfp_mask = htlb_alloc_mask(h);

	mpol_allowed = policy_nodemask_current(gfp_mask);

	for_each_node_mask(node, cpuset_current_mems_allowed) {
		if (!mpol_allowed || node_isset(node, *mpol_allowed))
			nr += array[node];
	}
>>>>>>> upstream/android-13

	return nr;
}

#ifdef CONFIG_SYSCTL
static int proc_hugetlb_doulongvec_minmax(struct ctl_table *table, int write,
					  void *buffer, size_t *length,
					  loff_t *ppos, unsigned long *out)
{
	struct ctl_table dup_table;

	/*
	 * In order to avoid races with __do_proc_doulongvec_minmax(), we
	 * can duplicate the @table and alter the duplicate of it.
	 */
	dup_table = *table;
	dup_table.data = out;

	return proc_doulongvec_minmax(&dup_table, write, buffer, length, ppos);
}

static int hugetlb_sysctl_handler_common(bool obey_mempolicy,
			 struct ctl_table *table, int write,
<<<<<<< HEAD
			 void __user *buffer, size_t *length, loff_t *ppos)
=======
			 void *buffer, size_t *length, loff_t *ppos)
>>>>>>> upstream/android-13
{
	struct hstate *h = &default_hstate;
	unsigned long tmp = h->max_huge_pages;
	int ret;

	if (!hugepages_supported())
		return -EOPNOTSUPP;

	ret = proc_hugetlb_doulongvec_minmax(table, write, buffer, length, ppos,
					     &tmp);
	if (ret)
		goto out;

	if (write)
		ret = __nr_hugepages_store_common(obey_mempolicy, h,
						  NUMA_NO_NODE, tmp, *length);
out:
	return ret;
}

int hugetlb_sysctl_handler(struct ctl_table *table, int write,
<<<<<<< HEAD
			  void __user *buffer, size_t *length, loff_t *ppos)
=======
			  void *buffer, size_t *length, loff_t *ppos)
>>>>>>> upstream/android-13
{

	return hugetlb_sysctl_handler_common(false, table, write,
							buffer, length, ppos);
}

#ifdef CONFIG_NUMA
int hugetlb_mempolicy_sysctl_handler(struct ctl_table *table, int write,
<<<<<<< HEAD
			  void __user *buffer, size_t *length, loff_t *ppos)
=======
			  void *buffer, size_t *length, loff_t *ppos)
>>>>>>> upstream/android-13
{
	return hugetlb_sysctl_handler_common(true, table, write,
							buffer, length, ppos);
}
#endif /* CONFIG_NUMA */

int hugetlb_overcommit_handler(struct ctl_table *table, int write,
<<<<<<< HEAD
			void __user *buffer,
			size_t *length, loff_t *ppos)
=======
		void *buffer, size_t *length, loff_t *ppos)
>>>>>>> upstream/android-13
{
	struct hstate *h = &default_hstate;
	unsigned long tmp;
	int ret;

	if (!hugepages_supported())
		return -EOPNOTSUPP;

	tmp = h->nr_overcommit_huge_pages;

	if (write && hstate_is_gigantic(h))
		return -EINVAL;

	ret = proc_hugetlb_doulongvec_minmax(table, write, buffer, length, ppos,
					     &tmp);
	if (ret)
		goto out;

	if (write) {
<<<<<<< HEAD
		spin_lock(&hugetlb_lock);
		h->nr_overcommit_huge_pages = tmp;
		spin_unlock(&hugetlb_lock);
=======
		spin_lock_irq(&hugetlb_lock);
		h->nr_overcommit_huge_pages = tmp;
		spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	}
out:
	return ret;
}

#endif /* CONFIG_SYSCTL */

void hugetlb_report_meminfo(struct seq_file *m)
{
	struct hstate *h;
	unsigned long total = 0;

	if (!hugepages_supported())
		return;

	for_each_hstate(h) {
		unsigned long count = h->nr_huge_pages;

<<<<<<< HEAD
		total += (PAGE_SIZE << huge_page_order(h)) * count;
=======
		total += huge_page_size(h) * count;
>>>>>>> upstream/android-13

		if (h == &default_hstate)
			seq_printf(m,
				   "HugePages_Total:   %5lu\n"
				   "HugePages_Free:    %5lu\n"
				   "HugePages_Rsvd:    %5lu\n"
				   "HugePages_Surp:    %5lu\n"
				   "Hugepagesize:   %8lu kB\n",
				   count,
				   h->free_huge_pages,
				   h->resv_huge_pages,
				   h->surplus_huge_pages,
<<<<<<< HEAD
				   (PAGE_SIZE << huge_page_order(h)) / 1024);
	}

	seq_printf(m, "Hugetlb:        %8lu kB\n", total / 1024);
}

int hugetlb_report_node_meminfo(int nid, char *buf)
{
	struct hstate *h = &default_hstate;
	if (!hugepages_supported())
		return 0;
	return sprintf(buf,
		"Node %d HugePages_Total: %5u\n"
		"Node %d HugePages_Free:  %5u\n"
		"Node %d HugePages_Surp:  %5u\n",
		nid, h->nr_huge_pages_node[nid],
		nid, h->free_huge_pages_node[nid],
		nid, h->surplus_huge_pages_node[nid]);
=======
				   huge_page_size(h) / SZ_1K);
	}

	seq_printf(m, "Hugetlb:        %8lu kB\n", total / SZ_1K);
}

int hugetlb_report_node_meminfo(char *buf, int len, int nid)
{
	struct hstate *h = &default_hstate;

	if (!hugepages_supported())
		return 0;

	return sysfs_emit_at(buf, len,
			     "Node %d HugePages_Total: %5u\n"
			     "Node %d HugePages_Free:  %5u\n"
			     "Node %d HugePages_Surp:  %5u\n",
			     nid, h->nr_huge_pages_node[nid],
			     nid, h->free_huge_pages_node[nid],
			     nid, h->surplus_huge_pages_node[nid]);
>>>>>>> upstream/android-13
}

void hugetlb_show_meminfo(void)
{
	struct hstate *h;
	int nid;

	if (!hugepages_supported())
		return;

	for_each_node_state(nid, N_MEMORY)
		for_each_hstate(h)
			pr_info("Node %d hugepages_total=%u hugepages_free=%u hugepages_surp=%u hugepages_size=%lukB\n",
				nid,
				h->nr_huge_pages_node[nid],
				h->free_huge_pages_node[nid],
				h->surplus_huge_pages_node[nid],
<<<<<<< HEAD
				1UL << (huge_page_order(h) + PAGE_SHIFT - 10));
=======
				huge_page_size(h) / SZ_1K);
>>>>>>> upstream/android-13
}

void hugetlb_report_usage(struct seq_file *m, struct mm_struct *mm)
{
	seq_printf(m, "HugetlbPages:\t%8lu kB\n",
		   atomic_long_read(&mm->hugetlb_usage) << (PAGE_SHIFT - 10));
}

/* Return the number pages of memory we physically have, in PAGE_SIZE units. */
unsigned long hugetlb_total_pages(void)
{
	struct hstate *h;
	unsigned long nr_total_pages = 0;

	for_each_hstate(h)
		nr_total_pages += h->nr_huge_pages * pages_per_huge_page(h);
	return nr_total_pages;
}

static int hugetlb_acct_memory(struct hstate *h, long delta)
{
	int ret = -ENOMEM;

<<<<<<< HEAD
	spin_lock(&hugetlb_lock);
=======
	if (!delta)
		return 0;

	spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	/*
	 * When cpuset is configured, it breaks the strict hugetlb page
	 * reservation as the accounting is done on a global variable. Such
	 * reservation is completely rubbish in the presence of cpuset because
	 * the reservation is not checked against page availability for the
	 * current cpuset. Application can still potentially OOM'ed by kernel
	 * with lack of free htlb page in cpuset that the task is in.
	 * Attempt to enforce strict accounting with cpuset is almost
	 * impossible (or too ugly) because cpuset is too fluid that
	 * task or memory node can be dynamically moved between cpusets.
	 *
	 * The change of semantics for shared hugetlb mapping with cpuset is
	 * undesirable. However, in order to preserve some of the semantics,
	 * we fall back to check against current free page availability as
	 * a best attempt and hopefully to minimize the impact of changing
	 * semantics that cpuset has.
<<<<<<< HEAD
=======
	 *
	 * Apart from cpuset, we also have memory policy mechanism that
	 * also determines from which node the kernel will allocate memory
	 * in a NUMA system. So similar to cpuset, we also should consider
	 * the memory policy of the current task. Similar to the description
	 * above.
>>>>>>> upstream/android-13
	 */
	if (delta > 0) {
		if (gather_surplus_pages(h, delta) < 0)
			goto out;

<<<<<<< HEAD
		if (delta > cpuset_mems_nr(h->free_huge_pages_node)) {
=======
		if (delta > allowed_mems_nr(h)) {
>>>>>>> upstream/android-13
			return_unused_surplus_pages(h, delta);
			goto out;
		}
	}

	ret = 0;
	if (delta < 0)
		return_unused_surplus_pages(h, (unsigned long) -delta);

out:
<<<<<<< HEAD
	spin_unlock(&hugetlb_lock);
=======
	spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	return ret;
}

static void hugetlb_vm_op_open(struct vm_area_struct *vma)
{
	struct resv_map *resv = vma_resv_map(vma);

	/*
	 * This new VMA should share its siblings reservation map if present.
	 * The VMA will only ever have a valid reservation map pointer where
	 * it is being copied for another still existing VMA.  As that VMA
	 * has a reference to the reservation map it cannot disappear until
	 * after this open call completes.  It is therefore safe to take a
	 * new reference here without additional locking.
	 */
<<<<<<< HEAD
	if (resv && is_vma_resv_set(vma, HPAGE_RESV_OWNER))
		kref_get(&resv->refs);
=======
	if (resv && is_vma_resv_set(vma, HPAGE_RESV_OWNER)) {
		resv_map_dup_hugetlb_cgroup_uncharge_info(resv);
		kref_get(&resv->refs);
	}
>>>>>>> upstream/android-13
}

static void hugetlb_vm_op_close(struct vm_area_struct *vma)
{
	struct hstate *h = hstate_vma(vma);
	struct resv_map *resv = vma_resv_map(vma);
	struct hugepage_subpool *spool = subpool_vma(vma);
	unsigned long reserve, start, end;
	long gbl_reserve;

	if (!resv || !is_vma_resv_set(vma, HPAGE_RESV_OWNER))
		return;

	start = vma_hugecache_offset(h, vma, vma->vm_start);
	end = vma_hugecache_offset(h, vma, vma->vm_end);

	reserve = (end - start) - region_count(resv, start, end);
<<<<<<< HEAD

	kref_put(&resv->refs, resv_map_release);

=======
	hugetlb_cgroup_uncharge_counter(resv, start, end);
>>>>>>> upstream/android-13
	if (reserve) {
		/*
		 * Decrement reserve counts.  The global reserve count may be
		 * adjusted if the subpool has a minimum size.
		 */
		gbl_reserve = hugepage_subpool_put_pages(spool, reserve);
		hugetlb_acct_memory(h, -gbl_reserve);
	}
<<<<<<< HEAD
=======

	kref_put(&resv->refs, resv_map_release);
>>>>>>> upstream/android-13
}

static int hugetlb_vm_op_split(struct vm_area_struct *vma, unsigned long addr)
{
	if (addr & ~(huge_page_mask(hstate_vma(vma))))
		return -EINVAL;
	return 0;
}

static unsigned long hugetlb_vm_op_pagesize(struct vm_area_struct *vma)
{
<<<<<<< HEAD
	struct hstate *hstate = hstate_vma(vma);

	return 1UL << huge_page_shift(hstate);
=======
	return huge_page_size(hstate_vma(vma));
>>>>>>> upstream/android-13
}

/*
 * We cannot handle pagefaults against hugetlb pages at all.  They cause
 * handle_mm_fault() to try to instantiate regular-sized pages in the
<<<<<<< HEAD
 * hugegpage VMA.  do_page_fault() is supposed to trap this, so BUG is we get
=======
 * hugepage VMA.  do_page_fault() is supposed to trap this, so BUG is we get
>>>>>>> upstream/android-13
 * this far.
 */
static vm_fault_t hugetlb_vm_op_fault(struct vm_fault *vmf)
{
	BUG();
	return 0;
}

/*
 * When a new function is introduced to vm_operations_struct and added
 * to hugetlb_vm_ops, please consider adding the function to shm_vm_ops.
 * This is because under System V memory model, mappings created via
 * shmget/shmat with "huge page" specified are backed by hugetlbfs files,
 * their original vm_ops are overwritten with shm_vm_ops.
 */
const struct vm_operations_struct hugetlb_vm_ops = {
	.fault = hugetlb_vm_op_fault,
	.open = hugetlb_vm_op_open,
	.close = hugetlb_vm_op_close,
<<<<<<< HEAD
	.split = hugetlb_vm_op_split,
=======
	.may_split = hugetlb_vm_op_split,
>>>>>>> upstream/android-13
	.pagesize = hugetlb_vm_op_pagesize,
};

static pte_t make_huge_pte(struct vm_area_struct *vma, struct page *page,
				int writable)
{
	pte_t entry;
<<<<<<< HEAD
=======
	unsigned int shift = huge_page_shift(hstate_vma(vma));
>>>>>>> upstream/android-13

	if (writable) {
		entry = huge_pte_mkwrite(huge_pte_mkdirty(mk_huge_pte(page,
					 vma->vm_page_prot)));
	} else {
		entry = huge_pte_wrprotect(mk_huge_pte(page,
					   vma->vm_page_prot));
	}
	entry = pte_mkyoung(entry);
	entry = pte_mkhuge(entry);
<<<<<<< HEAD
	entry = arch_make_huge_pte(entry, vma, page, writable);
=======
	entry = arch_make_huge_pte(entry, shift, vma->vm_flags);
>>>>>>> upstream/android-13

	return entry;
}

static void set_huge_ptep_writable(struct vm_area_struct *vma,
				   unsigned long address, pte_t *ptep)
{
	pte_t entry;

	entry = huge_pte_mkwrite(huge_pte_mkdirty(huge_ptep_get(ptep)));
	if (huge_ptep_set_access_flags(vma, address, ptep, entry, 1))
		update_mmu_cache(vma, address, ptep);
}

bool is_hugetlb_entry_migration(pte_t pte)
{
	swp_entry_t swp;

	if (huge_pte_none(pte) || pte_present(pte))
		return false;
	swp = pte_to_swp_entry(pte);
<<<<<<< HEAD
	if (non_swap_entry(swp) && is_migration_entry(swp))
=======
	if (is_migration_entry(swp))
>>>>>>> upstream/android-13
		return true;
	else
		return false;
}

<<<<<<< HEAD
static int is_hugetlb_entry_hwpoisoned(pte_t pte)
=======
static bool is_hugetlb_entry_hwpoisoned(pte_t pte)
>>>>>>> upstream/android-13
{
	swp_entry_t swp;

	if (huge_pte_none(pte) || pte_present(pte))
<<<<<<< HEAD
		return 0;
	swp = pte_to_swp_entry(pte);
	if (non_swap_entry(swp) && is_hwpoison_entry(swp))
		return 1;
	else
		return 0;
=======
		return false;
	swp = pte_to_swp_entry(pte);
	if (is_hwpoison_entry(swp))
		return true;
	else
		return false;
}

static void
hugetlb_install_page(struct vm_area_struct *vma, pte_t *ptep, unsigned long addr,
		     struct page *new_page)
{
	__SetPageUptodate(new_page);
	set_huge_pte_at(vma->vm_mm, addr, ptep, make_huge_pte(vma, new_page, 1));
	hugepage_add_new_anon_rmap(new_page, vma, addr);
	hugetlb_count_add(pages_per_huge_page(hstate_vma(vma)), vma->vm_mm);
	ClearHPageRestoreReserve(new_page);
	SetHPageMigratable(new_page);
>>>>>>> upstream/android-13
}

int copy_hugetlb_page_range(struct mm_struct *dst, struct mm_struct *src,
			    struct vm_area_struct *vma)
{
	pte_t *src_pte, *dst_pte, entry, dst_entry;
	struct page *ptepage;
	unsigned long addr;
<<<<<<< HEAD
	int cow;
	struct hstate *h = hstate_vma(vma);
	unsigned long sz = huge_page_size(h);
	unsigned long mmun_start;	/* For mmu_notifiers */
	unsigned long mmun_end;		/* For mmu_notifiers */
	int ret = 0;

	cow = (vma->vm_flags & (VM_SHARED | VM_MAYWRITE)) == VM_MAYWRITE;

	mmun_start = vma->vm_start;
	mmun_end = vma->vm_end;
	if (cow)
		mmu_notifier_invalidate_range_start(src, mmun_start, mmun_end);
=======
	bool cow = is_cow_mapping(vma->vm_flags);
	struct hstate *h = hstate_vma(vma);
	unsigned long sz = huge_page_size(h);
	unsigned long npages = pages_per_huge_page(h);
	struct address_space *mapping = vma->vm_file->f_mapping;
	struct mmu_notifier_range range;
	int ret = 0;

	if (cow) {
		mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, vma, src,
					vma->vm_start,
					vma->vm_end);
		mmu_notifier_invalidate_range_start(&range);
	} else {
		/*
		 * For shared mappings i_mmap_rwsem must be held to call
		 * huge_pte_alloc, otherwise the returned ptep could go
		 * away if part of a shared pmd and another thread calls
		 * huge_pmd_unshare.
		 */
		i_mmap_lock_read(mapping);
	}
>>>>>>> upstream/android-13

	for (addr = vma->vm_start; addr < vma->vm_end; addr += sz) {
		spinlock_t *src_ptl, *dst_ptl;
		src_pte = huge_pte_offset(src, addr, sz);
		if (!src_pte)
			continue;
<<<<<<< HEAD
		dst_pte = huge_pte_alloc(dst, addr, sz);
=======
		dst_pte = huge_pte_alloc(dst, vma, addr, sz);
>>>>>>> upstream/android-13
		if (!dst_pte) {
			ret = -ENOMEM;
			break;
		}

		/*
		 * If the pagetables are shared don't copy or take references.
		 * dst_pte == src_pte is the common case of src/dest sharing.
		 *
		 * However, src could have 'unshared' and dst shares with
		 * another vma.  If dst_pte !none, this implies sharing.
		 * Check here before taking page table lock, and once again
		 * after taking the lock below.
		 */
		dst_entry = huge_ptep_get(dst_pte);
		if ((dst_pte == src_pte) || !huge_pte_none(dst_entry))
			continue;

		dst_ptl = huge_pte_lock(h, dst, dst_pte);
		src_ptl = huge_pte_lockptr(h, src, src_pte);
		spin_lock_nested(src_ptl, SINGLE_DEPTH_NESTING);
		entry = huge_ptep_get(src_pte);
		dst_entry = huge_ptep_get(dst_pte);
<<<<<<< HEAD
=======
again:
>>>>>>> upstream/android-13
		if (huge_pte_none(entry) || !huge_pte_none(dst_entry)) {
			/*
			 * Skip if src entry none.  Also, skip in the
			 * unlikely case dst entry !none as this implies
			 * sharing with another vma.
			 */
			;
		} else if (unlikely(is_hugetlb_entry_migration(entry) ||
				    is_hugetlb_entry_hwpoisoned(entry))) {
			swp_entry_t swp_entry = pte_to_swp_entry(entry);

<<<<<<< HEAD
			if (is_write_migration_entry(swp_entry) && cow) {
=======
			if (is_writable_migration_entry(swp_entry) && cow) {
>>>>>>> upstream/android-13
				/*
				 * COW mappings require pages in both
				 * parent and child to be set to read.
				 */
<<<<<<< HEAD
				make_migration_entry_read(&swp_entry);
=======
				swp_entry = make_readable_migration_entry(
							swp_offset(swp_entry));
>>>>>>> upstream/android-13
				entry = swp_entry_to_pte(swp_entry);
				set_huge_swap_pte_at(src, addr, src_pte,
						     entry, sz);
			}
			set_huge_swap_pte_at(dst, addr, dst_pte, entry, sz);
		} else {
<<<<<<< HEAD
=======
			entry = huge_ptep_get(src_pte);
			ptepage = pte_page(entry);
			get_page(ptepage);

			/*
			 * This is a rare case where we see pinned hugetlb
			 * pages while they're prone to COW.  We need to do the
			 * COW earlier during fork.
			 *
			 * When pre-allocating the page or copying data, we
			 * need to be without the pgtable locks since we could
			 * sleep during the process.
			 */
			if (unlikely(page_needs_cow_for_dma(vma, ptepage))) {
				pte_t src_pte_old = entry;
				struct page *new;

				spin_unlock(src_ptl);
				spin_unlock(dst_ptl);
				/* Do not use reserve as it's private owned */
				new = alloc_huge_page(vma, addr, 1);
				if (IS_ERR(new)) {
					put_page(ptepage);
					ret = PTR_ERR(new);
					break;
				}
				copy_user_huge_page(new, ptepage, addr, vma,
						    npages);
				put_page(ptepage);

				/* Install the new huge page if src pte stable */
				dst_ptl = huge_pte_lock(h, dst, dst_pte);
				src_ptl = huge_pte_lockptr(h, src, src_pte);
				spin_lock_nested(src_ptl, SINGLE_DEPTH_NESTING);
				entry = huge_ptep_get(src_pte);
				if (!pte_same(src_pte_old, entry)) {
					restore_reserve_on_error(h, vma, addr,
								new);
					put_page(new);
					/* dst_entry won't change as in child */
					goto again;
				}
				hugetlb_install_page(vma, dst_pte, addr, new);
				spin_unlock(src_ptl);
				spin_unlock(dst_ptl);
				continue;
			}

>>>>>>> upstream/android-13
			if (cow) {
				/*
				 * No need to notify as we are downgrading page
				 * table protection not changing it to point
				 * to a new page.
				 *
				 * See Documentation/vm/mmu_notifier.rst
				 */
				huge_ptep_set_wrprotect(src, addr, src_pte);
<<<<<<< HEAD
			}
			entry = huge_ptep_get(src_pte);
			ptepage = pte_page(entry);
			get_page(ptepage);
			page_dup_rmap(ptepage, true);
			set_huge_pte_at(dst, addr, dst_pte, entry);
			hugetlb_count_add(pages_per_huge_page(h), dst);
=======
				entry = huge_pte_wrprotect(entry);
			}

			page_dup_rmap(ptepage, true);
			set_huge_pte_at(dst, addr, dst_pte, entry);
			hugetlb_count_add(npages, dst);
>>>>>>> upstream/android-13
		}
		spin_unlock(src_ptl);
		spin_unlock(dst_ptl);
	}

	if (cow)
<<<<<<< HEAD
		mmu_notifier_invalidate_range_end(src, mmun_start, mmun_end);
=======
		mmu_notifier_invalidate_range_end(&range);
	else
		i_mmap_unlock_read(mapping);
>>>>>>> upstream/android-13

	return ret;
}

void __unmap_hugepage_range(struct mmu_gather *tlb, struct vm_area_struct *vma,
			    unsigned long start, unsigned long end,
			    struct page *ref_page)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned long address;
	pte_t *ptep;
	pte_t pte;
	spinlock_t *ptl;
	struct page *page;
	struct hstate *h = hstate_vma(vma);
	unsigned long sz = huge_page_size(h);
<<<<<<< HEAD
	unsigned long mmun_start = start;	/* For mmu_notifiers */
	unsigned long mmun_end   = end;		/* For mmu_notifiers */
=======
	struct mmu_notifier_range range;
	bool force_flush = false;
>>>>>>> upstream/android-13

	WARN_ON(!is_vm_hugetlb_page(vma));
	BUG_ON(start & ~huge_page_mask(h));
	BUG_ON(end & ~huge_page_mask(h));

	/*
	 * This is a hugetlb vma, all the pte entries should point
	 * to huge page.
	 */
<<<<<<< HEAD
	tlb_remove_check_page_size_change(tlb, sz);
=======
	tlb_change_page_size(tlb, sz);
>>>>>>> upstream/android-13
	tlb_start_vma(tlb, vma);

	/*
	 * If sharing possible, alert mmu notifiers of worst case.
	 */
<<<<<<< HEAD
	adjust_range_if_pmd_sharing_possible(vma, &mmun_start, &mmun_end);
	mmu_notifier_invalidate_range_start(mm, mmun_start, mmun_end);
=======
	mmu_notifier_range_init(&range, MMU_NOTIFY_UNMAP, 0, vma, mm, start,
				end);
	adjust_range_if_pmd_sharing_possible(vma, &range.start, &range.end);
	mmu_notifier_invalidate_range_start(&range);
>>>>>>> upstream/android-13
	address = start;
	for (; address < end; address += sz) {
		ptep = huge_pte_offset(mm, address, sz);
		if (!ptep)
			continue;

		ptl = huge_pte_lock(h, mm, ptep);
<<<<<<< HEAD
		if (huge_pmd_unshare(mm, &address, ptep)) {
			spin_unlock(ptl);
			/*
			 * We just unmapped a page of PMDs by clearing a PUD.
			 * The caller's TLB flush range should cover this area.
			 */
=======
		if (huge_pmd_unshare(mm, vma, &address, ptep)) {
			spin_unlock(ptl);
			tlb_flush_pmd_range(tlb, address & PUD_MASK, PUD_SIZE);
			force_flush = true;
>>>>>>> upstream/android-13
			continue;
		}

		pte = huge_ptep_get(ptep);
		if (huge_pte_none(pte)) {
			spin_unlock(ptl);
			continue;
		}

		/*
		 * Migrating hugepage or HWPoisoned hugepage is already
		 * unmapped and its refcount is dropped, so just clear pte here.
		 */
		if (unlikely(!pte_present(pte))) {
			huge_pte_clear(mm, address, ptep, sz);
			spin_unlock(ptl);
			continue;
		}

		page = pte_page(pte);
		/*
		 * If a reference page is supplied, it is because a specific
		 * page is being unmapped, not a range. Ensure the page we
		 * are about to unmap is the actual page of interest.
		 */
		if (ref_page) {
			if (page != ref_page) {
				spin_unlock(ptl);
				continue;
			}
			/*
			 * Mark the VMA as having unmapped its page so that
			 * future faults in this VMA will fail rather than
			 * looking like data was lost
			 */
			set_vma_resv_flags(vma, HPAGE_RESV_UNMAPPED);
		}

		pte = huge_ptep_get_and_clear(mm, address, ptep);
		tlb_remove_huge_tlb_entry(h, tlb, ptep, address);
		if (huge_pte_dirty(pte))
			set_page_dirty(page);

		hugetlb_count_sub(pages_per_huge_page(h), mm);
		page_remove_rmap(page, true);

		spin_unlock(ptl);
		tlb_remove_page_size(tlb, page, huge_page_size(h));
		/*
		 * Bail out after unmapping reference page if supplied
		 */
		if (ref_page)
			break;
	}
<<<<<<< HEAD
	mmu_notifier_invalidate_range_end(mm, mmun_start, mmun_end);
	tlb_end_vma(tlb, vma);
=======
	mmu_notifier_invalidate_range_end(&range);
	tlb_end_vma(tlb, vma);

	/*
	 * If we unshared PMDs, the TLB flush was not recorded in mmu_gather. We
	 * could defer the flush until now, since by holding i_mmap_rwsem we
	 * guaranteed that the last refernece would not be dropped. But we must
	 * do the flushing before we return, as otherwise i_mmap_rwsem will be
	 * dropped and the last reference to the shared PMDs page might be
	 * dropped as well.
	 *
	 * In theory we could defer the freeing of the PMD pages as well, but
	 * huge_pmd_unshare() relies on the exact page_count for the PMD page to
	 * detect sharing, so we cannot defer the release of the page either.
	 * Instead, do flush now.
	 */
	if (force_flush)
		tlb_flush_mmu_tlbonly(tlb);
>>>>>>> upstream/android-13
}

void __unmap_hugepage_range_final(struct mmu_gather *tlb,
			  struct vm_area_struct *vma, unsigned long start,
			  unsigned long end, struct page *ref_page)
{
	__unmap_hugepage_range(tlb, vma, start, end, ref_page);

	/*
	 * Clear this flag so that x86's huge_pmd_share page_table_shareable
	 * test will fail on a vma being torn down, and not grab a page table
	 * on its way out.  We're lucky that the flag has such an appropriate
	 * name, and can in fact be safely cleared here. We could clear it
	 * before the __unmap_hugepage_range above, but all that's necessary
	 * is to clear it before releasing the i_mmap_rwsem. This works
	 * because in the context this is called, the VMA is about to be
	 * destroyed and the i_mmap_rwsem is held.
	 */
	vma->vm_flags &= ~VM_MAYSHARE;
}

void unmap_hugepage_range(struct vm_area_struct *vma, unsigned long start,
			  unsigned long end, struct page *ref_page)
{
<<<<<<< HEAD
	struct mm_struct *mm;
	struct mmu_gather tlb;
	unsigned long tlb_start = start;
	unsigned long tlb_end = end;

	/*
	 * If shared PMDs were possibly used within this vma range, adjust
	 * start/end for worst case tlb flushing.
	 * Note that we can not be sure if PMDs are shared until we try to
	 * unmap pages.  However, we want to make sure TLB flushing covers
	 * the largest possible range.
	 */
	adjust_range_if_pmd_sharing_possible(vma, &tlb_start, &tlb_end);

	mm = vma->vm_mm;

	tlb_gather_mmu(&tlb, mm, tlb_start, tlb_end);
	__unmap_hugepage_range(&tlb, vma, start, end, ref_page);
	tlb_finish_mmu(&tlb, tlb_start, tlb_end);
=======
	struct mmu_gather tlb;

	tlb_gather_mmu(&tlb, vma->vm_mm);
	__unmap_hugepage_range(&tlb, vma, start, end, ref_page);
	tlb_finish_mmu(&tlb);
>>>>>>> upstream/android-13
}

/*
 * This is called when the original mapper is failing to COW a MAP_PRIVATE
<<<<<<< HEAD
 * mappping it owns the reserve page for. The intention is to unmap the page
=======
 * mapping it owns the reserve page for. The intention is to unmap the page
>>>>>>> upstream/android-13
 * from other VMAs and let the children be SIGKILLed if they are faulting the
 * same region.
 */
static void unmap_ref_private(struct mm_struct *mm, struct vm_area_struct *vma,
			      struct page *page, unsigned long address)
{
	struct hstate *h = hstate_vma(vma);
	struct vm_area_struct *iter_vma;
	struct address_space *mapping;
	pgoff_t pgoff;

	/*
	 * vm_pgoff is in PAGE_SIZE units, hence the different calculation
	 * from page cache lookup which is in HPAGE_SIZE units.
	 */
	address = address & huge_page_mask(h);
	pgoff = ((address - vma->vm_start) >> PAGE_SHIFT) +
			vma->vm_pgoff;
	mapping = vma->vm_file->f_mapping;

	/*
	 * Take the mapping lock for the duration of the table walk. As
	 * this mapping should be shared between all the VMAs,
	 * __unmap_hugepage_range() is called as the lock is already held
	 */
	i_mmap_lock_write(mapping);
	vma_interval_tree_foreach(iter_vma, &mapping->i_mmap, pgoff, pgoff) {
		/* Do not unmap the current VMA */
		if (iter_vma == vma)
			continue;

		/*
		 * Shared VMAs have their own reserves and do not affect
		 * MAP_PRIVATE accounting but it is possible that a shared
		 * VMA is using the same page so check and skip such VMAs.
		 */
		if (iter_vma->vm_flags & VM_MAYSHARE)
			continue;

		/*
		 * Unmap the page from other VMAs without their own reserves.
		 * They get marked to be SIGKILLed if they fault in these
		 * areas. This is because a future no-page fault on this VMA
		 * could insert a zeroed page instead of the data existing
		 * from the time of fork. This would look like data corruption
		 */
		if (!is_vma_resv_set(iter_vma, HPAGE_RESV_OWNER))
			unmap_hugepage_range(iter_vma, address,
					     address + huge_page_size(h), page);
	}
	i_mmap_unlock_write(mapping);
}

/*
 * Hugetlb_cow() should be called with page lock of the original hugepage held.
 * Called with hugetlb_instantiation_mutex held and pte_page locked so we
 * cannot race with other handlers or page migration.
 * Keep the pte_same checks anyway to make transition from the mutex easier.
 */
static vm_fault_t hugetlb_cow(struct mm_struct *mm, struct vm_area_struct *vma,
		       unsigned long address, pte_t *ptep,
		       struct page *pagecache_page, spinlock_t *ptl)
{
	pte_t pte;
	struct hstate *h = hstate_vma(vma);
	struct page *old_page, *new_page;
	int outside_reserve = 0;
	vm_fault_t ret = 0;
<<<<<<< HEAD
	unsigned long mmun_start;	/* For mmu_notifiers */
	unsigned long mmun_end;		/* For mmu_notifiers */
	unsigned long haddr = address & huge_page_mask(h);
=======
	unsigned long haddr = address & huge_page_mask(h);
	struct mmu_notifier_range range;
>>>>>>> upstream/android-13

	pte = huge_ptep_get(ptep);
	old_page = pte_page(pte);

retry_avoidcopy:
	/* If no-one else is actually using this page, avoid the copy
	 * and just make the page writable */
	if (page_mapcount(old_page) == 1 && PageAnon(old_page)) {
		page_move_anon_rmap(old_page, vma);
		set_huge_ptep_writable(vma, haddr, ptep);
		return 0;
	}

	/*
	 * If the process that created a MAP_PRIVATE mapping is about to
	 * perform a COW due to a shared page count, attempt to satisfy
	 * the allocation without using the existing reserves. The pagecache
	 * page is used to determine if the reserve at this address was
	 * consumed or not. If reserves were used, a partial faulted mapping
	 * at the time of fork() could consume its reserves on COW instead
	 * of the full address range.
	 */
	if (is_vma_resv_set(vma, HPAGE_RESV_OWNER) &&
			old_page != pagecache_page)
		outside_reserve = 1;

	get_page(old_page);

	/*
	 * Drop page table lock as buddy allocator may be called. It will
	 * be acquired again before returning to the caller, as expected.
	 */
	spin_unlock(ptl);
	new_page = alloc_huge_page(vma, haddr, outside_reserve);

	if (IS_ERR(new_page)) {
		/*
		 * If a process owning a MAP_PRIVATE mapping fails to COW,
		 * it is due to references held by a child and an insufficient
		 * huge page pool. To guarantee the original mappers
		 * reliability, unmap the page from child processes. The child
		 * may get SIGKILLed if it later faults.
		 */
		if (outside_reserve) {
<<<<<<< HEAD
			put_page(old_page);
			BUG_ON(huge_pte_none(pte));
			unmap_ref_private(mm, vma, old_page, haddr);
			BUG_ON(huge_pte_none(pte));
=======
			struct address_space *mapping = vma->vm_file->f_mapping;
			pgoff_t idx;
			u32 hash;

			put_page(old_page);
			BUG_ON(huge_pte_none(pte));
			/*
			 * Drop hugetlb_fault_mutex and i_mmap_rwsem before
			 * unmapping.  unmapping needs to hold i_mmap_rwsem
			 * in write mode.  Dropping i_mmap_rwsem in read mode
			 * here is OK as COW mappings do not interact with
			 * PMD sharing.
			 *
			 * Reacquire both after unmap operation.
			 */
			idx = vma_hugecache_offset(h, vma, haddr);
			hash = hugetlb_fault_mutex_hash(mapping, idx);
			mutex_unlock(&hugetlb_fault_mutex_table[hash]);
			i_mmap_unlock_read(mapping);

			unmap_ref_private(mm, vma, old_page, haddr);

			i_mmap_lock_read(mapping);
			mutex_lock(&hugetlb_fault_mutex_table[hash]);
>>>>>>> upstream/android-13
			spin_lock(ptl);
			ptep = huge_pte_offset(mm, haddr, huge_page_size(h));
			if (likely(ptep &&
				   pte_same(huge_ptep_get(ptep), pte)))
				goto retry_avoidcopy;
			/*
			 * race occurs while re-acquiring page table
			 * lock, and our job is done.
			 */
			return 0;
		}

		ret = vmf_error(PTR_ERR(new_page));
		goto out_release_old;
	}

	/*
	 * When the original hugepage is shared one, it does not have
	 * anon_vma prepared.
	 */
	if (unlikely(anon_vma_prepare(vma))) {
		ret = VM_FAULT_OOM;
		goto out_release_all;
	}

	copy_user_huge_page(new_page, old_page, address, vma,
			    pages_per_huge_page(h));
	__SetPageUptodate(new_page);

<<<<<<< HEAD
	mmun_start = haddr;
	mmun_end = mmun_start + huge_page_size(h);
	mmu_notifier_invalidate_range_start(mm, mmun_start, mmun_end);
=======
	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, vma, mm, haddr,
				haddr + huge_page_size(h));
	mmu_notifier_invalidate_range_start(&range);
>>>>>>> upstream/android-13

	/*
	 * Retake the page table lock to check for racing updates
	 * before the page tables are altered
	 */
	spin_lock(ptl);
	ptep = huge_pte_offset(mm, haddr, huge_page_size(h));
	if (likely(ptep && pte_same(huge_ptep_get(ptep), pte))) {
<<<<<<< HEAD
		ClearPagePrivate(new_page);

		/* Break COW */
		huge_ptep_clear_flush(vma, haddr, ptep);
		mmu_notifier_invalidate_range(mm, mmun_start, mmun_end);
=======
		ClearHPageRestoreReserve(new_page);

		/* Break COW */
		huge_ptep_clear_flush(vma, haddr, ptep);
		mmu_notifier_invalidate_range(mm, range.start, range.end);
>>>>>>> upstream/android-13
		set_huge_pte_at(mm, haddr, ptep,
				make_huge_pte(vma, new_page, 1));
		page_remove_rmap(old_page, true);
		hugepage_add_new_anon_rmap(new_page, vma, haddr);
<<<<<<< HEAD
		set_page_huge_active(new_page);
=======
		SetHPageMigratable(new_page);
>>>>>>> upstream/android-13
		/* Make the old page be freed below */
		new_page = old_page;
	}
	spin_unlock(ptl);
<<<<<<< HEAD
	mmu_notifier_invalidate_range_end(mm, mmun_start, mmun_end);
out_release_all:
	restore_reserve_on_error(h, vma, haddr, new_page);
=======
	mmu_notifier_invalidate_range_end(&range);
out_release_all:
	/* No restore in case of successful pagetable update (Break COW) */
	if (new_page != old_page)
		restore_reserve_on_error(h, vma, haddr, new_page);
>>>>>>> upstream/android-13
	put_page(new_page);
out_release_old:
	put_page(old_page);

	spin_lock(ptl); /* Caller expects lock to be held */
	return ret;
}

/* Return the pagecache page at a given address within a VMA */
static struct page *hugetlbfs_pagecache_page(struct hstate *h,
			struct vm_area_struct *vma, unsigned long address)
{
	struct address_space *mapping;
	pgoff_t idx;

	mapping = vma->vm_file->f_mapping;
	idx = vma_hugecache_offset(h, vma, address);

	return find_lock_page(mapping, idx);
}

/*
 * Return whether there is a pagecache page to back given address within VMA.
 * Caller follow_hugetlb_page() holds page_table_lock so we cannot lock_page.
 */
static bool hugetlbfs_pagecache_present(struct hstate *h,
			struct vm_area_struct *vma, unsigned long address)
{
	struct address_space *mapping;
	pgoff_t idx;
	struct page *page;

	mapping = vma->vm_file->f_mapping;
	idx = vma_hugecache_offset(h, vma, address);

	page = find_get_page(mapping, idx);
	if (page)
		put_page(page);
	return page != NULL;
}

int huge_add_to_page_cache(struct page *page, struct address_space *mapping,
			   pgoff_t idx)
{
	struct inode *inode = mapping->host;
	struct hstate *h = hstate_inode(inode);
	int err = add_to_page_cache(page, mapping, idx, GFP_KERNEL);

	if (err)
		return err;
<<<<<<< HEAD
	ClearPagePrivate(page);
=======
	ClearHPageRestoreReserve(page);
>>>>>>> upstream/android-13

	/*
	 * set page dirty so that it will not be removed from cache/file
	 * by non-hugetlbfs specific code paths.
	 */
	set_page_dirty(page);

	spin_lock(&inode->i_lock);
	inode->i_blocks += blocks_per_huge_page(h);
	spin_unlock(&inode->i_lock);
	return 0;
}

<<<<<<< HEAD
=======
static inline vm_fault_t hugetlb_handle_userfault(struct vm_area_struct *vma,
						  struct address_space *mapping,
						  pgoff_t idx,
						  unsigned int flags,
						  unsigned long haddr,
						  unsigned long reason)
{
	vm_fault_t ret;
	u32 hash;
	struct vm_fault vmf = {
		.vma = vma,
		.address = haddr,
		.flags = flags,

		/*
		 * Hard to debug if it ends up being
		 * used by a callee that assumes
		 * something about the other
		 * uninitialized fields... same as in
		 * memory.c
		 */
	};

	/*
	 * hugetlb_fault_mutex and i_mmap_rwsem must be
	 * dropped before handling userfault.  Reacquire
	 * after handling fault to make calling code simpler.
	 */
	hash = hugetlb_fault_mutex_hash(mapping, idx);
	mutex_unlock(&hugetlb_fault_mutex_table[hash]);
	i_mmap_unlock_read(mapping);
	ret = handle_userfault(&vmf, reason);
	i_mmap_lock_read(mapping);
	mutex_lock(&hugetlb_fault_mutex_table[hash]);

	return ret;
}

>>>>>>> upstream/android-13
static vm_fault_t hugetlb_no_page(struct mm_struct *mm,
			struct vm_area_struct *vma,
			struct address_space *mapping, pgoff_t idx,
			unsigned long address, pte_t *ptep, unsigned int flags)
{
	struct hstate *h = hstate_vma(vma);
	vm_fault_t ret = VM_FAULT_SIGBUS;
	int anon_rmap = 0;
	unsigned long size;
	struct page *page;
	pte_t new_pte;
	spinlock_t *ptl;
	unsigned long haddr = address & huge_page_mask(h);
<<<<<<< HEAD
	bool new_page = false;
=======
	bool new_page, new_pagecache_page = false;
>>>>>>> upstream/android-13

	/*
	 * Currently, we are forced to kill the process in the event the
	 * original mapper has unmapped pages from the child due to a failed
	 * COW. Warn that such a situation has occurred as it may not be obvious
	 */
	if (is_vma_resv_set(vma, HPAGE_RESV_UNMAPPED)) {
		pr_warn_ratelimited("PID %d killed due to inadequate hugepage pool\n",
			   current->pid);
		return ret;
	}

	/*
<<<<<<< HEAD
	 * Use page lock to guard against racing truncation
	 * before we get page_table_lock.
	 */
retry:
	page = find_lock_page(mapping, idx);
	if (!page) {
		size = i_size_read(mapping->host) >> huge_page_shift(h);
		if (idx >= size)
			goto out;

		/*
		 * Check for page in userfault range
		 */
		if (userfaultfd_missing(vma)) {
			u32 hash;
			struct vm_fault vmf = {
				.vma = vma,
				.address = haddr,
				.flags = flags,
				.vma_flags = vma->vm_flags,
				.vma_page_prot = vma->vm_page_prot,
				/*
				 * Hard to debug if it ends up being
				 * used by a callee that assumes
				 * something about the other
				 * uninitialized fields... same as in
				 * memory.c
				 */
			};

			/*
			 * hugetlb_fault_mutex must be dropped before
			 * handling userfault.  Reacquire after handling
			 * fault to make calling code simpler.
			 */
			hash = hugetlb_fault_mutex_hash(h, mapping, idx, haddr);
			mutex_unlock(&hugetlb_fault_mutex_table[hash]);
			ret = handle_userfault(&vmf, VM_UFFD_MISSING);
			mutex_lock(&hugetlb_fault_mutex_table[hash]);
=======
	 * We can not race with truncation due to holding i_mmap_rwsem.
	 * i_size is modified when holding i_mmap_rwsem, so check here
	 * once for faults beyond end of file.
	 */
	size = i_size_read(mapping->host) >> huge_page_shift(h);
	if (idx >= size)
		goto out;

retry:
	new_page = false;
	page = find_lock_page(mapping, idx);
	if (!page) {
		/* Check for page in userfault range */
		if (userfaultfd_missing(vma)) {
			ret = hugetlb_handle_userfault(vma, mapping, idx,
						       flags, haddr,
						       VM_UFFD_MISSING);
>>>>>>> upstream/android-13
			goto out;
		}

		page = alloc_huge_page(vma, haddr, 0);
		if (IS_ERR(page)) {
<<<<<<< HEAD
			ret = vmf_error(PTR_ERR(page));
=======
			/*
			 * Returning error will result in faulting task being
			 * sent SIGBUS.  The hugetlb fault mutex prevents two
			 * tasks from racing to fault in the same page which
			 * could result in false unable to allocate errors.
			 * Page migration does not take the fault mutex, but
			 * does a clear then write of pte's under page table
			 * lock.  Page fault code could race with migration,
			 * notice the clear pte and try to allocate a page
			 * here.  Before returning error, get ptl and make
			 * sure there really is no pte entry.
			 */
			ptl = huge_pte_lock(h, mm, ptep);
			ret = 0;
			if (huge_pte_none(huge_ptep_get(ptep)))
				ret = vmf_error(PTR_ERR(page));
			spin_unlock(ptl);
>>>>>>> upstream/android-13
			goto out;
		}
		clear_huge_page(page, address, pages_per_huge_page(h));
		__SetPageUptodate(page);
		new_page = true;

		if (vma->vm_flags & VM_MAYSHARE) {
			int err = huge_add_to_page_cache(page, mapping, idx);
			if (err) {
				put_page(page);
				if (err == -EEXIST)
					goto retry;
				goto out;
			}
<<<<<<< HEAD
=======
			new_pagecache_page = true;
>>>>>>> upstream/android-13
		} else {
			lock_page(page);
			if (unlikely(anon_vma_prepare(vma))) {
				ret = VM_FAULT_OOM;
				goto backout_unlocked;
			}
			anon_rmap = 1;
		}
	} else {
		/*
		 * If memory error occurs between mmap() and fault, some process
		 * don't have hwpoisoned swap entry for errored virtual address.
		 * So we need to block hugepage fault by PG_hwpoison bit check.
		 */
		if (unlikely(PageHWPoison(page))) {
			ret = VM_FAULT_HWPOISON_LARGE |
				VM_FAULT_SET_HINDEX(hstate_index(h));
			goto backout_unlocked;
		}
<<<<<<< HEAD
=======

		/* Check for page in userfault range. */
		if (userfaultfd_minor(vma)) {
			unlock_page(page);
			put_page(page);
			ret = hugetlb_handle_userfault(vma, mapping, idx,
						       flags, haddr,
						       VM_UFFD_MINOR);
			goto out;
		}
>>>>>>> upstream/android-13
	}

	/*
	 * If we are going to COW a private mapping later, we examine the
	 * pending reservations for this page now. This will ensure that
	 * any allocations necessary to record that reservation occur outside
	 * the spinlock.
	 */
	if ((flags & FAULT_FLAG_WRITE) && !(vma->vm_flags & VM_SHARED)) {
		if (vma_needs_reservation(h, vma, haddr) < 0) {
			ret = VM_FAULT_OOM;
			goto backout_unlocked;
		}
		/* Just decrements count, does not deallocate */
		vma_end_reservation(h, vma, haddr);
	}

	ptl = huge_pte_lock(h, mm, ptep);
<<<<<<< HEAD
	size = i_size_read(mapping->host) >> huge_page_shift(h);
	if (idx >= size)
		goto backout;

=======
>>>>>>> upstream/android-13
	ret = 0;
	if (!huge_pte_none(huge_ptep_get(ptep)))
		goto backout;

	if (anon_rmap) {
<<<<<<< HEAD
		ClearPagePrivate(page);
=======
		ClearHPageRestoreReserve(page);
>>>>>>> upstream/android-13
		hugepage_add_new_anon_rmap(page, vma, haddr);
	} else
		page_dup_rmap(page, true);
	new_pte = make_huge_pte(vma, page, ((vma->vm_flags & VM_WRITE)
				&& (vma->vm_flags & VM_SHARED)));
	set_huge_pte_at(mm, haddr, ptep, new_pte);

	hugetlb_count_add(pages_per_huge_page(h), mm);
	if ((flags & FAULT_FLAG_WRITE) && !(vma->vm_flags & VM_SHARED)) {
		/* Optimization, do the COW without a second fault */
		ret = hugetlb_cow(mm, vma, address, ptep, page, ptl);
	}

	spin_unlock(ptl);

	/*
<<<<<<< HEAD
	 * Only make newly allocated pages active.  Existing pages found
	 * in the pagecache could be !page_huge_active() if they have been
	 * isolated for migration.
	 */
	if (new_page)
		set_page_huge_active(page);
=======
	 * Only set HPageMigratable in newly allocated pages.  Existing pages
	 * found in the pagecache may not have HPageMigratableset if they have
	 * been isolated for migration.
	 */
	if (new_page)
		SetHPageMigratable(page);
>>>>>>> upstream/android-13

	unlock_page(page);
out:
	return ret;

backout:
	spin_unlock(ptl);
backout_unlocked:
	unlock_page(page);
<<<<<<< HEAD
	restore_reserve_on_error(h, vma, haddr, page);
=======
	/* restore reserve for newly allocated pages not in page cache */
	if (new_page && !new_pagecache_page)
		restore_reserve_on_error(h, vma, haddr, page);
>>>>>>> upstream/android-13
	put_page(page);
	goto out;
}

#ifdef CONFIG_SMP
<<<<<<< HEAD
u32 hugetlb_fault_mutex_hash(struct hstate *h, struct address_space *mapping,
			    pgoff_t idx, unsigned long address)
=======
u32 hugetlb_fault_mutex_hash(struct address_space *mapping, pgoff_t idx)
>>>>>>> upstream/android-13
{
	unsigned long key[2];
	u32 hash;

	key[0] = (unsigned long) mapping;
	key[1] = idx;

	hash = jhash2((u32 *)&key, sizeof(key)/(sizeof(u32)), 0);

	return hash & (num_fault_mutexes - 1);
}
#else
/*
<<<<<<< HEAD
 * For uniprocesor systems we always use a single mutex, so just
 * return 0 and avoid the hashing overhead.
 */
u32 hugetlb_fault_mutex_hash(struct hstate *h, struct address_space *mapping,
			    pgoff_t idx, unsigned long address)
=======
 * For uniprocessor systems we always use a single mutex, so just
 * return 0 and avoid the hashing overhead.
 */
u32 hugetlb_fault_mutex_hash(struct address_space *mapping, pgoff_t idx)
>>>>>>> upstream/android-13
{
	return 0;
}
#endif

vm_fault_t hugetlb_fault(struct mm_struct *mm, struct vm_area_struct *vma,
			unsigned long address, unsigned int flags)
{
	pte_t *ptep, entry;
	spinlock_t *ptl;
	vm_fault_t ret;
	u32 hash;
	pgoff_t idx;
	struct page *page = NULL;
	struct page *pagecache_page = NULL;
	struct hstate *h = hstate_vma(vma);
	struct address_space *mapping;
	int need_wait_lock = 0;
	unsigned long haddr = address & huge_page_mask(h);

	ptep = huge_pte_offset(mm, haddr, huge_page_size(h));
	if (ptep) {
<<<<<<< HEAD
=======
		/*
		 * Since we hold no locks, ptep could be stale.  That is
		 * OK as we are only making decisions based on content and
		 * not actually modifying content here.
		 */
>>>>>>> upstream/android-13
		entry = huge_ptep_get(ptep);
		if (unlikely(is_hugetlb_entry_migration(entry))) {
			migration_entry_wait_huge(vma, mm, ptep);
			return 0;
		} else if (unlikely(is_hugetlb_entry_hwpoisoned(entry)))
			return VM_FAULT_HWPOISON_LARGE |
				VM_FAULT_SET_HINDEX(hstate_index(h));
<<<<<<< HEAD
	} else {
		ptep = huge_pte_alloc(mm, haddr, huge_page_size(h));
		if (!ptep)
			return VM_FAULT_OOM;
	}

	mapping = vma->vm_file->f_mapping;
	idx = vma_hugecache_offset(h, vma, haddr);
=======
	}

	/*
	 * Acquire i_mmap_rwsem before calling huge_pte_alloc and hold
	 * until finished with ptep.  This serves two purposes:
	 * 1) It prevents huge_pmd_unshare from being called elsewhere
	 *    and making the ptep no longer valid.
	 * 2) It synchronizes us with i_size modifications during truncation.
	 *
	 * ptep could have already be assigned via huge_pte_offset.  That
	 * is OK, as huge_pte_alloc will return the same value unless
	 * something has changed.
	 */
	mapping = vma->vm_file->f_mapping;
	i_mmap_lock_read(mapping);
	ptep = huge_pte_alloc(mm, vma, haddr, huge_page_size(h));
	if (!ptep) {
		i_mmap_unlock_read(mapping);
		return VM_FAULT_OOM;
	}
>>>>>>> upstream/android-13

	/*
	 * Serialize hugepage allocation and instantiation, so that we don't
	 * get spurious allocation failures if two CPUs race to instantiate
	 * the same page in the page cache.
	 */
<<<<<<< HEAD
	hash = hugetlb_fault_mutex_hash(h, mapping, idx, haddr);
=======
	idx = vma_hugecache_offset(h, vma, haddr);
	hash = hugetlb_fault_mutex_hash(mapping, idx);
>>>>>>> upstream/android-13
	mutex_lock(&hugetlb_fault_mutex_table[hash]);

	entry = huge_ptep_get(ptep);
	if (huge_pte_none(entry)) {
		ret = hugetlb_no_page(mm, vma, mapping, idx, address, ptep, flags);
		goto out_mutex;
	}

	ret = 0;

	/*
	 * entry could be a migration/hwpoison entry at this point, so this
	 * check prevents the kernel from going below assuming that we have
<<<<<<< HEAD
	 * a active hugepage in pagecache. This goto expects the 2nd page fault,
	 * and is_hugetlb_entry_(migration|hwpoisoned) check will properly
	 * handle it.
=======
	 * an active hugepage in pagecache. This goto expects the 2nd page
	 * fault, and is_hugetlb_entry_(migration|hwpoisoned) check will
	 * properly handle it.
>>>>>>> upstream/android-13
	 */
	if (!pte_present(entry))
		goto out_mutex;

	/*
	 * If we are going to COW the mapping later, we examine the pending
	 * reservations for this page now. This will ensure that any
	 * allocations necessary to record that reservation occur outside the
	 * spinlock. For private mappings, we also lookup the pagecache
	 * page now as it is used to determine if a reservation has been
	 * consumed.
	 */
	if ((flags & FAULT_FLAG_WRITE) && !huge_pte_write(entry)) {
		if (vma_needs_reservation(h, vma, haddr) < 0) {
			ret = VM_FAULT_OOM;
			goto out_mutex;
		}
		/* Just decrements count, does not deallocate */
		vma_end_reservation(h, vma, haddr);

		if (!(vma->vm_flags & VM_MAYSHARE))
			pagecache_page = hugetlbfs_pagecache_page(h,
								vma, haddr);
	}

	ptl = huge_pte_lock(h, mm, ptep);

	/* Check for a racing update before calling hugetlb_cow */
	if (unlikely(!pte_same(entry, huge_ptep_get(ptep))))
		goto out_ptl;

	/*
	 * hugetlb_cow() requires page locks of pte_page(entry) and
	 * pagecache_page, so here we need take the former one
	 * when page != pagecache_page or !pagecache_page.
	 */
	page = pte_page(entry);
	if (page != pagecache_page)
		if (!trylock_page(page)) {
			need_wait_lock = 1;
			goto out_ptl;
		}

	get_page(page);

	if (flags & FAULT_FLAG_WRITE) {
		if (!huge_pte_write(entry)) {
			ret = hugetlb_cow(mm, vma, address, ptep,
					  pagecache_page, ptl);
			goto out_put_page;
		}
		entry = huge_pte_mkdirty(entry);
	}
	entry = pte_mkyoung(entry);
	if (huge_ptep_set_access_flags(vma, haddr, ptep, entry,
						flags & FAULT_FLAG_WRITE))
		update_mmu_cache(vma, haddr, ptep);
out_put_page:
	if (page != pagecache_page)
		unlock_page(page);
	put_page(page);
out_ptl:
	spin_unlock(ptl);

	if (pagecache_page) {
		unlock_page(pagecache_page);
		put_page(pagecache_page);
	}
out_mutex:
	mutex_unlock(&hugetlb_fault_mutex_table[hash]);
<<<<<<< HEAD
=======
	i_mmap_unlock_read(mapping);
>>>>>>> upstream/android-13
	/*
	 * Generally it's safe to hold refcount during waiting page lock. But
	 * here we just wait to defer the next page fault to avoid busy loop and
	 * the page is not used after unlocked before returning from the current
	 * page fault. So we are safe from accessing freed page, even if we wait
	 * here without taking refcount.
	 */
	if (need_wait_lock)
		wait_on_page_locked(page);
	return ret;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_USERFAULTFD
>>>>>>> upstream/android-13
/*
 * Used by userfaultfd UFFDIO_COPY.  Based on mcopy_atomic_pte with
 * modifications for huge pages.
 */
int hugetlb_mcopy_atomic_pte(struct mm_struct *dst_mm,
			    pte_t *dst_pte,
			    struct vm_area_struct *dst_vma,
			    unsigned long dst_addr,
			    unsigned long src_addr,
<<<<<<< HEAD
			    struct page **pagep)
{
	struct address_space *mapping;
	pgoff_t idx;
	unsigned long size;
	int vm_shared = dst_vma->vm_flags & VM_SHARED;
	struct hstate *h = hstate_vma(dst_vma);
	pte_t _dst_pte;
	spinlock_t *ptl;
	int ret;
	struct page *page;

	if (!*pagep) {
		ret = -ENOMEM;
		page = alloc_huge_page(dst_vma, dst_addr, 0);
		if (IS_ERR(page))
			goto out;
=======
			    enum mcopy_atomic_mode mode,
			    struct page **pagep)
{
	bool is_continue = (mode == MCOPY_ATOMIC_CONTINUE);
	struct hstate *h = hstate_vma(dst_vma);
	struct address_space *mapping = dst_vma->vm_file->f_mapping;
	pgoff_t idx = vma_hugecache_offset(h, dst_vma, dst_addr);
	unsigned long size;
	int vm_shared = dst_vma->vm_flags & VM_SHARED;
	pte_t _dst_pte;
	spinlock_t *ptl;
	int ret = -ENOMEM;
	struct page *page;
	int writable;
	bool page_in_pagecache = false;

	if (is_continue) {
		ret = -EFAULT;
		page = find_lock_page(mapping, idx);
		if (!page)
			goto out;
		page_in_pagecache = true;
	} else if (!*pagep) {
		/* If a page already exists, then it's UFFDIO_COPY for
		 * a non-missing case. Return -EEXIST.
		 */
		if (vm_shared &&
		    hugetlbfs_pagecache_present(h, dst_vma, dst_addr)) {
			ret = -EEXIST;
			goto out;
		}

		page = alloc_huge_page(dst_vma, dst_addr, 0);
		if (IS_ERR(page)) {
			ret = -ENOMEM;
			goto out;
		}
>>>>>>> upstream/android-13

		ret = copy_huge_page_from_user(page,
						(const void __user *) src_addr,
						pages_per_huge_page(h), false);

<<<<<<< HEAD
		/* fallback to copy_from_user outside mmap_sem */
		if (unlikely(ret)) {
			ret = -ENOENT;
			*pagep = page;
			/* don't free the page */
			goto out;
		}
	} else {
		page = *pagep;
=======
		/* fallback to copy_from_user outside mmap_lock */
		if (unlikely(ret)) {
			ret = -ENOENT;
			/* Free the allocated page which may have
			 * consumed a reservation.
			 */
			restore_reserve_on_error(h, dst_vma, dst_addr, page);
			put_page(page);

			/* Allocate a temporary page to hold the copied
			 * contents.
			 */
			page = alloc_huge_page_vma(h, dst_vma, dst_addr);
			if (!page) {
				ret = -ENOMEM;
				goto out;
			}
			*pagep = page;
			/* Set the outparam pagep and return to the caller to
			 * copy the contents outside the lock. Don't free the
			 * page.
			 */
			goto out;
		}
	} else {
		if (vm_shared &&
		    hugetlbfs_pagecache_present(h, dst_vma, dst_addr)) {
			put_page(*pagep);
			ret = -EEXIST;
			*pagep = NULL;
			goto out;
		}

		page = alloc_huge_page(dst_vma, dst_addr, 0);
		if (IS_ERR(page)) {
			ret = -ENOMEM;
			*pagep = NULL;
			goto out;
		}
		copy_huge_page(page, *pagep);
		put_page(*pagep);
>>>>>>> upstream/android-13
		*pagep = NULL;
	}

	/*
	 * The memory barrier inside __SetPageUptodate makes sure that
	 * preceding stores to the page contents become visible before
	 * the set_pte_at() write.
	 */
	__SetPageUptodate(page);

<<<<<<< HEAD
	mapping = dst_vma->vm_file->f_mapping;
	idx = vma_hugecache_offset(h, dst_vma, dst_addr);

	/*
	 * If shared, add to page cache
	 */
	if (vm_shared) {
=======
	/* Add shared, newly allocated pages to the page cache. */
	if (vm_shared && !is_continue) {
>>>>>>> upstream/android-13
		size = i_size_read(mapping->host) >> huge_page_shift(h);
		ret = -EFAULT;
		if (idx >= size)
			goto out_release_nounlock;

		/*
		 * Serialization between remove_inode_hugepages() and
		 * huge_add_to_page_cache() below happens through the
		 * hugetlb_fault_mutex_table that here must be hold by
		 * the caller.
		 */
		ret = huge_add_to_page_cache(page, mapping, idx);
		if (ret)
			goto out_release_nounlock;
<<<<<<< HEAD
=======
		page_in_pagecache = true;
>>>>>>> upstream/android-13
	}

	ptl = huge_pte_lockptr(h, dst_mm, dst_pte);
	spin_lock(ptl);

	/*
	 * Recheck the i_size after holding PT lock to make sure not
	 * to leave any page mapped (as page_mapped()) beyond the end
	 * of the i_size (remove_inode_hugepages() is strict about
	 * enforcing that). If we bail out here, we'll also leave a
	 * page in the radix tree in the vm_shared case beyond the end
	 * of the i_size, but remove_inode_hugepages() will take care
	 * of it as soon as we drop the hugetlb_fault_mutex_table.
	 */
	size = i_size_read(mapping->host) >> huge_page_shift(h);
	ret = -EFAULT;
	if (idx >= size)
		goto out_release_unlock;

	ret = -EEXIST;
	if (!huge_pte_none(huge_ptep_get(dst_pte)))
		goto out_release_unlock;

	if (vm_shared) {
		page_dup_rmap(page, true);
	} else {
<<<<<<< HEAD
		ClearPagePrivate(page);
		hugepage_add_new_anon_rmap(page, dst_vma, dst_addr);
	}

	_dst_pte = make_huge_pte(dst_vma, page, dst_vma->vm_flags & VM_WRITE);
	if (dst_vma->vm_flags & VM_WRITE)
=======
		ClearHPageRestoreReserve(page);
		hugepage_add_new_anon_rmap(page, dst_vma, dst_addr);
	}

	/* For CONTINUE on a non-shared VMA, don't set VM_WRITE for CoW. */
	if (is_continue && !vm_shared)
		writable = 0;
	else
		writable = dst_vma->vm_flags & VM_WRITE;

	_dst_pte = make_huge_pte(dst_vma, page, writable);
	if (writable)
>>>>>>> upstream/android-13
		_dst_pte = huge_pte_mkdirty(_dst_pte);
	_dst_pte = pte_mkyoung(_dst_pte);

	set_huge_pte_at(dst_mm, dst_addr, dst_pte, _dst_pte);

	(void)huge_ptep_set_access_flags(dst_vma, dst_addr, dst_pte, _dst_pte,
					dst_vma->vm_flags & VM_WRITE);
	hugetlb_count_add(pages_per_huge_page(h), dst_mm);

	/* No need to invalidate - it was non-present before */
	update_mmu_cache(dst_vma, dst_addr, dst_pte);

	spin_unlock(ptl);
<<<<<<< HEAD
	set_page_huge_active(page);
	if (vm_shared)
=======
	if (!is_continue)
		SetHPageMigratable(page);
	if (vm_shared || is_continue)
>>>>>>> upstream/android-13
		unlock_page(page);
	ret = 0;
out:
	return ret;
out_release_unlock:
	spin_unlock(ptl);
<<<<<<< HEAD
	if (vm_shared)
		unlock_page(page);
out_release_nounlock:
	put_page(page);
	goto out;
}
=======
	if (vm_shared || is_continue)
		unlock_page(page);
out_release_nounlock:
	if (!page_in_pagecache)
		restore_reserve_on_error(h, dst_vma, dst_addr, page);
	put_page(page);
	goto out;
}
#endif /* CONFIG_USERFAULTFD */

static void record_subpages_vmas(struct page *page, struct vm_area_struct *vma,
				 int refs, struct page **pages,
				 struct vm_area_struct **vmas)
{
	int nr;

	for (nr = 0; nr < refs; nr++) {
		if (likely(pages))
			pages[nr] = mem_map_offset(page, nr);
		if (vmas)
			vmas[nr] = vma;
	}
}
>>>>>>> upstream/android-13

long follow_hugetlb_page(struct mm_struct *mm, struct vm_area_struct *vma,
			 struct page **pages, struct vm_area_struct **vmas,
			 unsigned long *position, unsigned long *nr_pages,
<<<<<<< HEAD
			 long i, unsigned int flags, int *nonblocking)
=======
			 long i, unsigned int flags, int *locked)
>>>>>>> upstream/android-13
{
	unsigned long pfn_offset;
	unsigned long vaddr = *position;
	unsigned long remainder = *nr_pages;
	struct hstate *h = hstate_vma(vma);
<<<<<<< HEAD
	int err = -EFAULT;
=======
	int err = -EFAULT, refs;
>>>>>>> upstream/android-13

	while (vaddr < vma->vm_end && remainder) {
		pte_t *pte;
		spinlock_t *ptl = NULL;
		int absent;
		struct page *page;

		/*
		 * If we have a pending SIGKILL, don't keep faulting pages and
		 * potentially allocating memory.
		 */
<<<<<<< HEAD
		if (unlikely(fatal_signal_pending(current))) {
=======
		if (fatal_signal_pending(current)) {
>>>>>>> upstream/android-13
			remainder = 0;
			break;
		}

		/*
		 * Some archs (sparc64, sh*) have multiple pte_ts to
		 * each hugepage.  We have to make sure we get the
		 * first, for the page indexing below to work.
		 *
		 * Note that page table lock is not held when pte is null.
		 */
		pte = huge_pte_offset(mm, vaddr & huge_page_mask(h),
				      huge_page_size(h));
		if (pte)
			ptl = huge_pte_lock(h, mm, pte);
		absent = !pte || huge_pte_none(huge_ptep_get(pte));

		/*
		 * When coredumping, it suits get_dump_page if we just return
		 * an error where there's an empty slot with no huge pagecache
		 * to back it.  This way, we avoid allocating a hugepage, and
		 * the sparse dumpfile avoids allocating disk blocks, but its
		 * huge holes still show up with zeroes where they need to be.
		 */
		if (absent && (flags & FOLL_DUMP) &&
		    !hugetlbfs_pagecache_present(h, vma, vaddr)) {
			if (pte)
				spin_unlock(ptl);
			remainder = 0;
			break;
		}

		/*
		 * We need call hugetlb_fault for both hugepages under migration
		 * (in which case hugetlb_fault waits for the migration,) and
		 * hwpoisoned hugepages (in which case we need to prevent the
		 * caller from accessing to them.) In order to do this, we use
		 * here is_swap_pte instead of is_hugetlb_entry_migration and
		 * is_hugetlb_entry_hwpoisoned. This is because it simply covers
		 * both cases, and because we can't follow correct pages
		 * directly from any kind of swap entries.
		 */
		if (absent || is_swap_pte(huge_ptep_get(pte)) ||
		    ((flags & FOLL_WRITE) &&
		      !huge_pte_write(huge_ptep_get(pte)))) {
			vm_fault_t ret;
			unsigned int fault_flags = 0;

			if (pte)
				spin_unlock(ptl);
			if (flags & FOLL_WRITE)
				fault_flags |= FAULT_FLAG_WRITE;
<<<<<<< HEAD
			if (nonblocking)
				fault_flags |= FAULT_FLAG_ALLOW_RETRY;
=======
			if (locked)
				fault_flags |= FAULT_FLAG_ALLOW_RETRY |
					FAULT_FLAG_KILLABLE;
>>>>>>> upstream/android-13
			if (flags & FOLL_NOWAIT)
				fault_flags |= FAULT_FLAG_ALLOW_RETRY |
					FAULT_FLAG_RETRY_NOWAIT;
			if (flags & FOLL_TRIED) {
<<<<<<< HEAD
				VM_WARN_ON_ONCE(fault_flags &
						FAULT_FLAG_ALLOW_RETRY);
=======
				/*
				 * Note: FAULT_FLAG_ALLOW_RETRY and
				 * FAULT_FLAG_TRIED can co-exist
				 */
>>>>>>> upstream/android-13
				fault_flags |= FAULT_FLAG_TRIED;
			}
			ret = hugetlb_fault(mm, vma, vaddr, fault_flags);
			if (ret & VM_FAULT_ERROR) {
				err = vm_fault_to_errno(ret, flags);
				remainder = 0;
				break;
			}
			if (ret & VM_FAULT_RETRY) {
<<<<<<< HEAD
				if (nonblocking &&
				    !(fault_flags & FAULT_FLAG_RETRY_NOWAIT))
					*nonblocking = 0;
=======
				if (locked &&
				    !(fault_flags & FAULT_FLAG_RETRY_NOWAIT))
					*locked = 0;
>>>>>>> upstream/android-13
				*nr_pages = 0;
				/*
				 * VM_FAULT_RETRY must not return an
				 * error, it will return zero
				 * instead.
				 *
				 * No need to update "position" as the
				 * caller will not check it after
				 * *nr_pages is set to 0.
				 */
				return i;
			}
			continue;
		}

		pfn_offset = (vaddr & ~huge_page_mask(h)) >> PAGE_SHIFT;
		page = pte_page(huge_ptep_get(pte));

		/*
<<<<<<< HEAD
		 * Instead of doing 'try_get_page()' below in the same_page
		 * loop, just check the count once here.
		 */
		if (unlikely(page_count(page) <= 0)) {
			if (pages) {
=======
		 * If subpage information not requested, update counters
		 * and skip the same_page loop below.
		 */
		if (!pages && !vmas && !pfn_offset &&
		    (vaddr + huge_page_size(h) < vma->vm_end) &&
		    (remainder >= pages_per_huge_page(h))) {
			vaddr += huge_page_size(h);
			remainder -= pages_per_huge_page(h);
			i += pages_per_huge_page(h);
			spin_unlock(ptl);
			continue;
		}

		/* vaddr may not be aligned to PAGE_SIZE */
		refs = min3(pages_per_huge_page(h) - pfn_offset, remainder,
		    (vma->vm_end - ALIGN_DOWN(vaddr, PAGE_SIZE)) >> PAGE_SHIFT);

		if (pages || vmas)
			record_subpages_vmas(mem_map_offset(page, pfn_offset),
					     vma, refs,
					     likely(pages) ? pages + i : NULL,
					     vmas ? vmas + i : NULL);

		if (pages) {
			/*
			 * try_grab_compound_head() should always succeed here,
			 * because: a) we hold the ptl lock, and b) we've just
			 * checked that the huge page is present in the page
			 * tables. If the huge page is present, then the tail
			 * pages must also be present. The ptl prevents the
			 * head page and tail pages from being rearranged in
			 * any way. So this page must be available at this
			 * point, unless the page refcount overflowed:
			 */
			if (WARN_ON_ONCE(!try_grab_compound_head(pages[i],
								 refs,
								 flags))) {
>>>>>>> upstream/android-13
				spin_unlock(ptl);
				remainder = 0;
				err = -ENOMEM;
				break;
			}
		}
<<<<<<< HEAD
same_page:
		if (pages) {
			pages[i] = mem_map_offset(page, pfn_offset);
			get_page(pages[i]);
		}

		if (vmas)
			vmas[i] = vma;

		vaddr += PAGE_SIZE;
		++pfn_offset;
		--remainder;
		++i;
		if (vaddr < vma->vm_end && remainder &&
				pfn_offset < pages_per_huge_page(h)) {
			/*
			 * We use pfn_offset to avoid touching the pageframes
			 * of this compound page.
			 */
			goto same_page;
		}
=======

		vaddr += (refs << PAGE_SHIFT);
		remainder -= refs;
		i += refs;

>>>>>>> upstream/android-13
		spin_unlock(ptl);
	}
	*nr_pages = remainder;
	/*
	 * setting position is actually required only if remainder is
	 * not zero but it's faster not to add a "if (remainder)"
	 * branch.
	 */
	*position = vaddr;

	return i ? i : err;
}

<<<<<<< HEAD
#ifndef __HAVE_ARCH_FLUSH_HUGETLB_TLB_RANGE
/*
 * ARCHes with special requirements for evicting HUGETLB backing TLB entries can
 * implement this.
 */
#define flush_hugetlb_tlb_range(vma, addr, end)	flush_tlb_range(vma, addr, end)
#endif

=======
>>>>>>> upstream/android-13
unsigned long hugetlb_change_protection(struct vm_area_struct *vma,
		unsigned long address, unsigned long end, pgprot_t newprot)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned long start = address;
	pte_t *ptep;
	pte_t pte;
	struct hstate *h = hstate_vma(vma);
	unsigned long pages = 0;
<<<<<<< HEAD
	unsigned long f_start = start;
	unsigned long f_end = end;
	bool shared_pmd = false;

	/*
	 * In the case of shared PMDs, the area to flush could be beyond
	 * start/end.  Set f_start/f_end to cover the maximum possible
	 * range if PMD sharing is possible.
	 */
	adjust_range_if_pmd_sharing_possible(vma, &f_start, &f_end);

	BUG_ON(address >= end);
	flush_cache_range(vma, f_start, f_end);

	mmu_notifier_invalidate_range_start(mm, f_start, f_end);
=======
	bool shared_pmd = false;
	struct mmu_notifier_range range;

	/*
	 * In the case of shared PMDs, the area to flush could be beyond
	 * start/end.  Set range.start/range.end to cover the maximum possible
	 * range if PMD sharing is possible.
	 */
	mmu_notifier_range_init(&range, MMU_NOTIFY_PROTECTION_VMA,
				0, vma, mm, start, end);
	adjust_range_if_pmd_sharing_possible(vma, &range.start, &range.end);

	BUG_ON(address >= end);
	flush_cache_range(vma, range.start, range.end);

	mmu_notifier_invalidate_range_start(&range);
>>>>>>> upstream/android-13
	i_mmap_lock_write(vma->vm_file->f_mapping);
	for (; address < end; address += huge_page_size(h)) {
		spinlock_t *ptl;
		ptep = huge_pte_offset(mm, address, huge_page_size(h));
		if (!ptep)
			continue;
		ptl = huge_pte_lock(h, mm, ptep);
<<<<<<< HEAD
		if (huge_pmd_unshare(mm, &address, ptep)) {
=======
		if (huge_pmd_unshare(mm, vma, &address, ptep)) {
>>>>>>> upstream/android-13
			pages++;
			spin_unlock(ptl);
			shared_pmd = true;
			continue;
		}
		pte = huge_ptep_get(ptep);
		if (unlikely(is_hugetlb_entry_hwpoisoned(pte))) {
			spin_unlock(ptl);
			continue;
		}
		if (unlikely(is_hugetlb_entry_migration(pte))) {
			swp_entry_t entry = pte_to_swp_entry(pte);

<<<<<<< HEAD
			if (is_write_migration_entry(entry)) {
				pte_t newpte;

				make_migration_entry_read(&entry);
=======
			if (is_writable_migration_entry(entry)) {
				pte_t newpte;

				entry = make_readable_migration_entry(
							swp_offset(entry));
>>>>>>> upstream/android-13
				newpte = swp_entry_to_pte(entry);
				set_huge_swap_pte_at(mm, address, ptep,
						     newpte, huge_page_size(h));
				pages++;
			}
			spin_unlock(ptl);
			continue;
		}
		if (!huge_pte_none(pte)) {
<<<<<<< HEAD
			pte = huge_ptep_get_and_clear(mm, address, ptep);
			pte = pte_mkhuge(huge_pte_modify(pte, newprot));
			pte = arch_make_huge_pte(pte, vma, NULL, 0);
			set_huge_pte_at(mm, address, ptep, pte);
=======
			pte_t old_pte;
			unsigned int shift = huge_page_shift(hstate_vma(vma));

			old_pte = huge_ptep_modify_prot_start(vma, address, ptep);
			pte = pte_mkhuge(huge_pte_modify(old_pte, newprot));
			pte = arch_make_huge_pte(pte, shift, vma->vm_flags);
			huge_ptep_modify_prot_commit(vma, address, ptep, old_pte, pte);
>>>>>>> upstream/android-13
			pages++;
		}
		spin_unlock(ptl);
	}
	/*
	 * Must flush TLB before releasing i_mmap_rwsem: x86's huge_pmd_unshare
	 * may have cleared our pud entry and done put_page on the page table:
	 * once we release i_mmap_rwsem, another task can do the final put_page
	 * and that page table be reused and filled with junk.  If we actually
	 * did unshare a page of pmds, flush the range corresponding to the pud.
	 */
	if (shared_pmd)
<<<<<<< HEAD
		flush_hugetlb_tlb_range(vma, f_start, f_end);
=======
		flush_hugetlb_tlb_range(vma, range.start, range.end);
>>>>>>> upstream/android-13
	else
		flush_hugetlb_tlb_range(vma, start, end);
	/*
	 * No need to call mmu_notifier_invalidate_range() we are downgrading
	 * page table protection not changing it to point to a new page.
	 *
	 * See Documentation/vm/mmu_notifier.rst
	 */
	i_mmap_unlock_write(vma->vm_file->f_mapping);
<<<<<<< HEAD
	mmu_notifier_invalidate_range_end(mm, f_start, f_end);
=======
	mmu_notifier_invalidate_range_end(&range);
>>>>>>> upstream/android-13

	return pages << h->order;
}

<<<<<<< HEAD
int hugetlb_reserve_pages(struct inode *inode,
=======
/* Return true if reservation was successful, false otherwise.  */
bool hugetlb_reserve_pages(struct inode *inode,
>>>>>>> upstream/android-13
					long from, long to,
					struct vm_area_struct *vma,
					vm_flags_t vm_flags)
{
<<<<<<< HEAD
	long ret, chg;
	struct hstate *h = hstate_inode(inode);
	struct hugepage_subpool *spool = subpool_inode(inode);
	struct resv_map *resv_map;
	long gbl_reserve;
=======
	long chg, add = -1;
	struct hstate *h = hstate_inode(inode);
	struct hugepage_subpool *spool = subpool_inode(inode);
	struct resv_map *resv_map;
	struct hugetlb_cgroup *h_cg = NULL;
	long gbl_reserve, regions_needed = 0;
>>>>>>> upstream/android-13

	/* This should never happen */
	if (from > to) {
		VM_WARN(1, "%s called with a negative range\n", __func__);
<<<<<<< HEAD
		return -EINVAL;
=======
		return false;
>>>>>>> upstream/android-13
	}

	/*
	 * Only apply hugepage reservation if asked. At fault time, an
	 * attempt will be made for VM_NORESERVE to allocate a page
	 * without using reserves
	 */
	if (vm_flags & VM_NORESERVE)
<<<<<<< HEAD
		return 0;
=======
		return true;
>>>>>>> upstream/android-13

	/*
	 * Shared mappings base their reservation on the number of pages that
	 * are already allocated on behalf of the file. Private mappings need
	 * to reserve the full area even if read-only as mprotect() may be
	 * called to make the mapping read-write. Assume !vma is a shm mapping
	 */
	if (!vma || vma->vm_flags & VM_MAYSHARE) {
<<<<<<< HEAD
		resv_map = inode_resv_map(inode);

		chg = region_chg(resv_map, from, to);

	} else {
		resv_map = resv_map_alloc();
		if (!resv_map)
			return -ENOMEM;
=======
		/*
		 * resv_map can not be NULL as hugetlb_reserve_pages is only
		 * called for inodes for which resv_maps were created (see
		 * hugetlbfs_get_inode).
		 */
		resv_map = inode_resv_map(inode);

		chg = region_chg(resv_map, from, to, &regions_needed);

	} else {
		/* Private mapping. */
		resv_map = resv_map_alloc();
		if (!resv_map)
			return false;
>>>>>>> upstream/android-13

		chg = to - from;

		set_vma_resv_map(vma, resv_map);
		set_vma_resv_flags(vma, HPAGE_RESV_OWNER);
	}

<<<<<<< HEAD
	if (chg < 0) {
		ret = chg;
		goto out_err;
=======
	if (chg < 0)
		goto out_err;

	if (hugetlb_cgroup_charge_cgroup_rsvd(hstate_index(h),
				chg * pages_per_huge_page(h), &h_cg) < 0)
		goto out_err;

	if (vma && !(vma->vm_flags & VM_MAYSHARE) && h_cg) {
		/* For private mappings, the hugetlb_cgroup uncharge info hangs
		 * of the resv_map.
		 */
		resv_map_set_hugetlb_cgroup_uncharge_info(resv_map, h_cg, h);
>>>>>>> upstream/android-13
	}

	/*
	 * There must be enough pages in the subpool for the mapping. If
	 * the subpool has a minimum size, there may be some global
	 * reservations already in place (gbl_reserve).
	 */
	gbl_reserve = hugepage_subpool_get_pages(spool, chg);
<<<<<<< HEAD
	if (gbl_reserve < 0) {
		ret = -ENOSPC;
		goto out_err;
	}
=======
	if (gbl_reserve < 0)
		goto out_uncharge_cgroup;
>>>>>>> upstream/android-13

	/*
	 * Check enough hugepages are available for the reservation.
	 * Hand the pages back to the subpool if there are not
	 */
<<<<<<< HEAD
	ret = hugetlb_acct_memory(h, gbl_reserve);
	if (ret < 0) {
		/* put back original number of pages, chg */
		(void)hugepage_subpool_put_pages(spool, chg);
		goto out_err;
	}
=======
	if (hugetlb_acct_memory(h, gbl_reserve) < 0)
		goto out_put_pages;
>>>>>>> upstream/android-13

	/*
	 * Account for the reservations made. Shared mappings record regions
	 * that have reservations as they are shared by multiple VMAs.
	 * When the last VMA disappears, the region map says how much
	 * the reservation was and the page cache tells how much of
	 * the reservation was consumed. Private mappings are per-VMA and
	 * only the consumed reservations are tracked. When the VMA
	 * disappears, the original reservation is the VMA size and the
	 * consumed reservations are stored in the map. Hence, nothing
	 * else has to be done for private mappings here
	 */
	if (!vma || vma->vm_flags & VM_MAYSHARE) {
<<<<<<< HEAD
		long add = region_add(resv_map, from, to);

		if (unlikely(chg > add)) {
=======
		add = region_add(resv_map, from, to, regions_needed, h, h_cg);

		if (unlikely(add < 0)) {
			hugetlb_acct_memory(h, -gbl_reserve);
			goto out_put_pages;
		} else if (unlikely(chg > add)) {
>>>>>>> upstream/android-13
			/*
			 * pages in this range were added to the reserve
			 * map between region_chg and region_add.  This
			 * indicates a race with alloc_huge_page.  Adjust
			 * the subpool and reserve counts modified above
			 * based on the difference.
			 */
			long rsv_adjust;

<<<<<<< HEAD
			rsv_adjust = hugepage_subpool_put_pages(spool,
								chg - add);
			hugetlb_acct_memory(h, -rsv_adjust);
		}
	}
	return 0;
out_err:
	if (!vma || vma->vm_flags & VM_MAYSHARE)
		/* Don't call region_abort if region_chg failed */
		if (chg >= 0)
			region_abort(resv_map, from, to);
	if (vma && is_vma_resv_set(vma, HPAGE_RESV_OWNER))
		kref_put(&resv_map->refs, resv_map_release);
	return ret;
=======
			/*
			 * hugetlb_cgroup_uncharge_cgroup_rsvd() will put the
			 * reference to h_cg->css. See comment below for detail.
			 */
			hugetlb_cgroup_uncharge_cgroup_rsvd(
				hstate_index(h),
				(chg - add) * pages_per_huge_page(h), h_cg);

			rsv_adjust = hugepage_subpool_put_pages(spool,
								chg - add);
			hugetlb_acct_memory(h, -rsv_adjust);
		} else if (h_cg) {
			/*
			 * The file_regions will hold their own reference to
			 * h_cg->css. So we should release the reference held
			 * via hugetlb_cgroup_charge_cgroup_rsvd() when we are
			 * done.
			 */
			hugetlb_cgroup_put_rsvd_cgroup(h_cg);
		}
	}
	return true;

out_put_pages:
	/* put back original number of pages, chg */
	(void)hugepage_subpool_put_pages(spool, chg);
out_uncharge_cgroup:
	hugetlb_cgroup_uncharge_cgroup_rsvd(hstate_index(h),
					    chg * pages_per_huge_page(h), h_cg);
out_err:
	if (!vma || vma->vm_flags & VM_MAYSHARE)
		/* Only call region_abort if the region_chg succeeded but the
		 * region_add failed or didn't run.
		 */
		if (chg >= 0 && add < 0)
			region_abort(resv_map, from, to, regions_needed);
	if (vma && is_vma_resv_set(vma, HPAGE_RESV_OWNER))
		kref_put(&resv_map->refs, resv_map_release);
	return false;
>>>>>>> upstream/android-13
}

long hugetlb_unreserve_pages(struct inode *inode, long start, long end,
								long freed)
{
	struct hstate *h = hstate_inode(inode);
	struct resv_map *resv_map = inode_resv_map(inode);
	long chg = 0;
	struct hugepage_subpool *spool = subpool_inode(inode);
	long gbl_reserve;

<<<<<<< HEAD
=======
	/*
	 * Since this routine can be called in the evict inode path for all
	 * hugetlbfs inodes, resv_map could be NULL.
	 */
>>>>>>> upstream/android-13
	if (resv_map) {
		chg = region_del(resv_map, start, end);
		/*
		 * region_del() can fail in the rare case where a region
		 * must be split and another region descriptor can not be
		 * allocated.  If end == LONG_MAX, it will not fail.
		 */
		if (chg < 0)
			return chg;
	}

	spin_lock(&inode->i_lock);
	inode->i_blocks -= (blocks_per_huge_page(h) * freed);
	spin_unlock(&inode->i_lock);

	/*
	 * If the subpool has a minimum size, the number of global
	 * reservations to be released may be adjusted.
<<<<<<< HEAD
=======
	 *
	 * Note that !resv_map implies freed == 0. So (chg - freed)
	 * won't go negative.
>>>>>>> upstream/android-13
	 */
	gbl_reserve = hugepage_subpool_put_pages(spool, (chg - freed));
	hugetlb_acct_memory(h, -gbl_reserve);

	return 0;
}

#ifdef CONFIG_ARCH_WANT_HUGE_PMD_SHARE
static unsigned long page_table_shareable(struct vm_area_struct *svma,
				struct vm_area_struct *vma,
				unsigned long addr, pgoff_t idx)
{
	unsigned long saddr = ((idx - svma->vm_pgoff) << PAGE_SHIFT) +
				svma->vm_start;
	unsigned long sbase = saddr & PUD_MASK;
	unsigned long s_end = sbase + PUD_SIZE;

	/* Allow segments to share if only one is marked locked */
	unsigned long vm_flags = vma->vm_flags & VM_LOCKED_CLEAR_MASK;
	unsigned long svm_flags = svma->vm_flags & VM_LOCKED_CLEAR_MASK;

	/*
	 * match the virtual addresses, permission and the alignment of the
	 * page table page.
	 */
	if (pmd_index(addr) != pmd_index(saddr) ||
	    vm_flags != svm_flags ||
<<<<<<< HEAD
	    sbase < svma->vm_start || svma->vm_end < s_end)
=======
	    !range_in_vma(svma, sbase, s_end))
>>>>>>> upstream/android-13
		return 0;

	return saddr;
}

static bool vma_shareable(struct vm_area_struct *vma, unsigned long addr)
{
	unsigned long base = addr & PUD_MASK;
	unsigned long end = base + PUD_SIZE;

	/*
	 * check on proper vm_flags and page table alignment
	 */
	if (vma->vm_flags & VM_MAYSHARE && range_in_vma(vma, base, end))
		return true;
	return false;
}

<<<<<<< HEAD
=======
bool want_pmd_share(struct vm_area_struct *vma, unsigned long addr)
{
#ifdef CONFIG_USERFAULTFD
	if (uffd_disable_huge_pmd_share(vma))
		return false;
#endif
	return vma_shareable(vma, addr);
}

>>>>>>> upstream/android-13
/*
 * Determine if start,end range within vma could be mapped by shared pmd.
 * If yes, adjust start and end to cover range associated with possible
 * shared pmd mappings.
 */
void adjust_range_if_pmd_sharing_possible(struct vm_area_struct *vma,
				unsigned long *start, unsigned long *end)
{
	unsigned long v_start = ALIGN(vma->vm_start, PUD_SIZE),
		v_end = ALIGN_DOWN(vma->vm_end, PUD_SIZE);

	/*
<<<<<<< HEAD
	 * vma need span at least one aligned PUD size and the start,end range
	 * must at least partialy within it.
=======
	 * vma needs to span at least one aligned PUD size, and the range
	 * must be at least partially within in.
>>>>>>> upstream/android-13
	 */
	if (!(vma->vm_flags & VM_MAYSHARE) || !(v_end > v_start) ||
		(*end <= v_start) || (*start >= v_end))
		return;

	/* Extend the range to be PUD aligned for a worst case scenario */
	if (*start > v_start)
		*start = ALIGN_DOWN(*start, PUD_SIZE);

	if (*end < v_end)
		*end = ALIGN(*end, PUD_SIZE);
}

/*
 * Search for a shareable pmd page for hugetlb. In any case calls pmd_alloc()
 * and returns the corresponding pte. While this is not necessary for the
 * !shared pmd case because we can allocate the pmd later as well, it makes the
<<<<<<< HEAD
 * code much cleaner. pmd allocation is essential for the shared case because
 * pud has to be populated inside the same i_mmap_rwsem section - otherwise
 * racing tasks could either miss the sharing (see huge_pte_offset) or select a
 * bad pmd for sharing.
 */
pte_t *huge_pmd_share(struct mm_struct *mm, unsigned long addr, pud_t *pud)
{
	struct vm_area_struct *vma = find_vma(mm, addr);
=======
 * code much cleaner.
 *
 * This routine must be called with i_mmap_rwsem held in at least read mode if
 * sharing is possible.  For hugetlbfs, this prevents removal of any page
 * table entries associated with the address space.  This is important as we
 * are setting up sharing based on existing page table entries (mappings).
 *
 * NOTE: This routine is only called from huge_pte_alloc.  Some callers of
 * huge_pte_alloc know that sharing is not possible and do not take
 * i_mmap_rwsem as a performance optimization.  This is handled by the
 * if !vma_shareable check at the beginning of the routine. i_mmap_rwsem is
 * only required for subsequent processing.
 */
pte_t *huge_pmd_share(struct mm_struct *mm, struct vm_area_struct *vma,
		      unsigned long addr, pud_t *pud)
{
>>>>>>> upstream/android-13
	struct address_space *mapping = vma->vm_file->f_mapping;
	pgoff_t idx = ((addr - vma->vm_start) >> PAGE_SHIFT) +
			vma->vm_pgoff;
	struct vm_area_struct *svma;
	unsigned long saddr;
	pte_t *spte = NULL;
	pte_t *pte;
	spinlock_t *ptl;

<<<<<<< HEAD
	if (!vma_shareable(vma, addr))
		return (pte_t *)pmd_alloc(mm, pud, addr);

	i_mmap_lock_write(mapping);
=======
	i_mmap_assert_locked(mapping);
>>>>>>> upstream/android-13
	vma_interval_tree_foreach(svma, &mapping->i_mmap, idx, idx) {
		if (svma == vma)
			continue;

		saddr = page_table_shareable(svma, vma, addr, idx);
		if (saddr) {
			spte = huge_pte_offset(svma->vm_mm, saddr,
					       vma_mmu_pagesize(svma));
			if (spte) {
				get_page(virt_to_page(spte));
				break;
			}
		}
	}

	if (!spte)
		goto out;

	ptl = huge_pte_lock(hstate_vma(vma), mm, spte);
	if (pud_none(*pud)) {
		pud_populate(mm, pud,
				(pmd_t *)((unsigned long)spte & PAGE_MASK));
		mm_inc_nr_pmds(mm);
	} else {
		put_page(virt_to_page(spte));
	}
	spin_unlock(ptl);
out:
	pte = (pte_t *)pmd_alloc(mm, pud, addr);
<<<<<<< HEAD
	i_mmap_unlock_write(mapping);
=======
>>>>>>> upstream/android-13
	return pte;
}

/*
 * unmap huge page backed by shared pte.
 *
 * Hugetlb pte page is ref counted at the time of mapping.  If pte is shared
 * indicated by page_count > 1, unmap is achieved by clearing pud and
 * decrementing the ref count. If count == 1, the pte page is not shared.
 *
<<<<<<< HEAD
 * called with page table lock held.
=======
 * Called with page table lock held and i_mmap_rwsem held in write mode.
>>>>>>> upstream/android-13
 *
 * returns: 1 successfully unmapped a shared pte page
 *	    0 the underlying pte page is not shared, or it is the last user
 */
<<<<<<< HEAD
int huge_pmd_unshare(struct mm_struct *mm, unsigned long *addr, pte_t *ptep)
=======
int huge_pmd_unshare(struct mm_struct *mm, struct vm_area_struct *vma,
					unsigned long *addr, pte_t *ptep)
>>>>>>> upstream/android-13
{
	pgd_t *pgd = pgd_offset(mm, *addr);
	p4d_t *p4d = p4d_offset(pgd, *addr);
	pud_t *pud = pud_offset(p4d, *addr);

<<<<<<< HEAD
=======
	i_mmap_assert_write_locked(vma->vm_file->f_mapping);
>>>>>>> upstream/android-13
	BUG_ON(page_count(virt_to_page(ptep)) == 0);
	if (page_count(virt_to_page(ptep)) == 1)
		return 0;

	pud_clear(pud);
	put_page(virt_to_page(ptep));
	mm_dec_nr_pmds(mm);
	*addr = ALIGN(*addr, HPAGE_SIZE * PTRS_PER_PTE) - HPAGE_SIZE;
	return 1;
}
<<<<<<< HEAD
#define want_pmd_share()	(1)
#else /* !CONFIG_ARCH_WANT_HUGE_PMD_SHARE */
pte_t *huge_pmd_share(struct mm_struct *mm, unsigned long addr, pud_t *pud)
=======

#else /* !CONFIG_ARCH_WANT_HUGE_PMD_SHARE */
pte_t *huge_pmd_share(struct mm_struct *mm, struct vm_area_struct *vma,
		      unsigned long addr, pud_t *pud)
>>>>>>> upstream/android-13
{
	return NULL;
}

<<<<<<< HEAD
int huge_pmd_unshare(struct mm_struct *mm, unsigned long *addr, pte_t *ptep)
=======
int huge_pmd_unshare(struct mm_struct *mm, struct vm_area_struct *vma,
				unsigned long *addr, pte_t *ptep)
>>>>>>> upstream/android-13
{
	return 0;
}

void adjust_range_if_pmd_sharing_possible(struct vm_area_struct *vma,
				unsigned long *start, unsigned long *end)
{
}
<<<<<<< HEAD
#define want_pmd_share()	(0)
#endif /* CONFIG_ARCH_WANT_HUGE_PMD_SHARE */

#ifdef CONFIG_ARCH_WANT_GENERAL_HUGETLB
pte_t *huge_pte_alloc(struct mm_struct *mm,
=======

bool want_pmd_share(struct vm_area_struct *vma, unsigned long addr)
{
	return false;
}
#endif /* CONFIG_ARCH_WANT_HUGE_PMD_SHARE */

#ifdef CONFIG_ARCH_WANT_GENERAL_HUGETLB
pte_t *huge_pte_alloc(struct mm_struct *mm, struct vm_area_struct *vma,
>>>>>>> upstream/android-13
			unsigned long addr, unsigned long sz)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pte_t *pte = NULL;

	pgd = pgd_offset(mm, addr);
	p4d = p4d_alloc(mm, pgd, addr);
	if (!p4d)
		return NULL;
	pud = pud_alloc(mm, p4d, addr);
	if (pud) {
		if (sz == PUD_SIZE) {
			pte = (pte_t *)pud;
		} else {
			BUG_ON(sz != PMD_SIZE);
<<<<<<< HEAD
			if (want_pmd_share() && pud_none(*pud))
				pte = huge_pmd_share(mm, addr, pud);
=======
			if (want_pmd_share(vma, addr) && pud_none(*pud))
				pte = huge_pmd_share(mm, vma, addr, pud);
>>>>>>> upstream/android-13
			else
				pte = (pte_t *)pmd_alloc(mm, pud, addr);
		}
	}
	BUG_ON(pte && pte_present(*pte) && !pte_huge(*pte));

	return pte;
}

/*
 * huge_pte_offset() - Walk the page table to resolve the hugepage
 * entry at address @addr
 *
<<<<<<< HEAD
 * Return: Pointer to page table or swap entry (PUD or PMD) for
 * address @addr, or NULL if a p*d_none() entry is encountered and the
=======
 * Return: Pointer to page table entry (PUD or PMD) for
 * address @addr, or NULL if a !p*d_present() entry is encountered and the
>>>>>>> upstream/android-13
 * size @sz doesn't match the hugepage size at this level of the page
 * table.
 */
pte_t *huge_pte_offset(struct mm_struct *mm,
		       unsigned long addr, unsigned long sz)
{
	pgd_t *pgd;
	p4d_t *p4d;
<<<<<<< HEAD
	pud_t *pud, pud_entry;
	pmd_t *pmd, pmd_entry;
=======
	pud_t *pud;
	pmd_t *pmd;
>>>>>>> upstream/android-13

	pgd = pgd_offset(mm, addr);
	if (!pgd_present(*pgd))
		return NULL;
	p4d = p4d_offset(pgd, addr);
	if (!p4d_present(*p4d))
		return NULL;

	pud = pud_offset(p4d, addr);
<<<<<<< HEAD
	pud_entry = READ_ONCE(*pud);
	if (sz != PUD_SIZE && pud_none(pud_entry))
		return NULL;
	/* hugepage or swap? */
	if (pud_huge(pud_entry) || !pud_present(pud_entry))
		return (pte_t *)pud;

	pmd = pmd_offset(pud, addr);
	pmd_entry = READ_ONCE(*pmd);
	if (sz != PMD_SIZE && pmd_none(pmd_entry))
		return NULL;
	/* hugepage or swap? */
	if (pmd_huge(pmd_entry) || !pmd_present(pmd_entry))
		return (pte_t *)pmd;

	return NULL;
=======
	if (sz == PUD_SIZE)
		/* must be pud huge, non-present or none */
		return (pte_t *)pud;
	if (!pud_present(*pud))
		return NULL;
	/* must have a valid entry and size to go further */

	pmd = pmd_offset(pud, addr);
	/* must be pmd huge, non-present or none */
	return (pte_t *)pmd;
>>>>>>> upstream/android-13
}

#endif /* CONFIG_ARCH_WANT_GENERAL_HUGETLB */

/*
 * These functions are overwritable if your architecture needs its own
 * behavior.
 */
struct page * __weak
follow_huge_addr(struct mm_struct *mm, unsigned long address,
			      int write)
{
	return ERR_PTR(-EINVAL);
}

struct page * __weak
follow_huge_pd(struct vm_area_struct *vma,
	       unsigned long address, hugepd_t hpd, int flags, int pdshift)
{
	WARN(1, "hugepd follow called with no support for hugepage directory format\n");
	return NULL;
}

struct page * __weak
follow_huge_pmd(struct mm_struct *mm, unsigned long address,
		pmd_t *pmd, int flags)
{
	struct page *page = NULL;
	spinlock_t *ptl;
	pte_t pte;
<<<<<<< HEAD
=======

	/* FOLL_GET and FOLL_PIN are mutually exclusive. */
	if (WARN_ON_ONCE((flags & (FOLL_PIN | FOLL_GET)) ==
			 (FOLL_PIN | FOLL_GET)))
		return NULL;

>>>>>>> upstream/android-13
retry:
	ptl = pmd_lockptr(mm, pmd);
	spin_lock(ptl);
	/*
	 * make sure that the address range covered by this pmd is not
	 * unmapped from other threads.
	 */
	if (!pmd_huge(*pmd))
		goto out;
	pte = huge_ptep_get((pte_t *)pmd);
	if (pte_present(pte)) {
		page = pmd_page(*pmd) + ((address & ~PMD_MASK) >> PAGE_SHIFT);
<<<<<<< HEAD
		if (flags & FOLL_GET)
			get_page(page);
=======
		/*
		 * try_grab_page() should always succeed here, because: a) we
		 * hold the pmd (ptl) lock, and b) we've just checked that the
		 * huge pmd (head) page is present in the page tables. The ptl
		 * prevents the head page and tail pages from being rearranged
		 * in any way. So this page must be available at this point,
		 * unless the page refcount overflowed:
		 */
		if (WARN_ON_ONCE(!try_grab_page(page, flags))) {
			page = NULL;
			goto out;
		}
>>>>>>> upstream/android-13
	} else {
		if (is_hugetlb_entry_migration(pte)) {
			spin_unlock(ptl);
			__migration_entry_wait(mm, (pte_t *)pmd, ptl);
			goto retry;
		}
		/*
		 * hwpoisoned entry is treated as no_page_table in
		 * follow_page_mask().
		 */
	}
out:
	spin_unlock(ptl);
	return page;
}

struct page * __weak
follow_huge_pud(struct mm_struct *mm, unsigned long address,
		pud_t *pud, int flags)
{
<<<<<<< HEAD
	if (flags & FOLL_GET)
=======
	if (flags & (FOLL_GET | FOLL_PIN))
>>>>>>> upstream/android-13
		return NULL;

	return pte_page(*(pte_t *)pud) + ((address & ~PUD_MASK) >> PAGE_SHIFT);
}

struct page * __weak
follow_huge_pgd(struct mm_struct *mm, unsigned long address, pgd_t *pgd, int flags)
{
<<<<<<< HEAD
	if (flags & FOLL_GET)
=======
	if (flags & (FOLL_GET | FOLL_PIN))
>>>>>>> upstream/android-13
		return NULL;

	return pte_page(*(pte_t *)pgd) + ((address & ~PGDIR_MASK) >> PAGE_SHIFT);
}

bool isolate_huge_page(struct page *page, struct list_head *list)
{
	bool ret = true;

<<<<<<< HEAD
	spin_lock(&hugetlb_lock);
	if (!PageHeadHuge(page) || !page_huge_active(page) ||
=======
	spin_lock_irq(&hugetlb_lock);
	if (!PageHeadHuge(page) ||
	    !HPageMigratable(page) ||
>>>>>>> upstream/android-13
	    !get_page_unless_zero(page)) {
		ret = false;
		goto unlock;
	}
<<<<<<< HEAD
	clear_page_huge_active(page);
	list_move_tail(&page->lru, list);
unlock:
	spin_unlock(&hugetlb_lock);
=======
	ClearHPageMigratable(page);
	list_move_tail(&page->lru, list);
unlock:
	spin_unlock_irq(&hugetlb_lock);
	return ret;
}

int get_hwpoison_huge_page(struct page *page, bool *hugetlb)
{
	int ret = 0;

	*hugetlb = false;
	spin_lock_irq(&hugetlb_lock);
	if (PageHeadHuge(page)) {
		*hugetlb = true;
		if (HPageFreed(page) || HPageMigratable(page))
			ret = get_page_unless_zero(page);
		else
			ret = -EBUSY;
	}
	spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	return ret;
}

void putback_active_hugepage(struct page *page)
{
<<<<<<< HEAD
	VM_BUG_ON_PAGE(!PageHead(page), page);
	spin_lock(&hugetlb_lock);
	set_page_huge_active(page);
	list_move_tail(&page->lru, &(page_hstate(page))->hugepage_activelist);
	spin_unlock(&hugetlb_lock);
=======
	spin_lock_irq(&hugetlb_lock);
	SetHPageMigratable(page);
	list_move_tail(&page->lru, &(page_hstate(page))->hugepage_activelist);
	spin_unlock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
	put_page(page);
}

void move_hugetlb_state(struct page *oldpage, struct page *newpage, int reason)
{
	struct hstate *h = page_hstate(oldpage);

	hugetlb_cgroup_migrate(oldpage, newpage);
	set_page_owner_migrate_reason(newpage, reason);

	/*
	 * transfer temporary state of the new huge page. This is
	 * reverse to other transitions because the newpage is going to
	 * be final while the old one will be freed so it takes over
	 * the temporary status.
	 *
	 * Also note that we have to transfer the per-node surplus state
	 * here as well otherwise the global surplus count will not match
	 * the per-node's.
	 */
<<<<<<< HEAD
	if (PageHugeTemporary(newpage)) {
		int old_nid = page_to_nid(oldpage);
		int new_nid = page_to_nid(newpage);

		SetPageHugeTemporary(oldpage);
		ClearPageHugeTemporary(newpage);

		spin_lock(&hugetlb_lock);
=======
	if (HPageTemporary(newpage)) {
		int old_nid = page_to_nid(oldpage);
		int new_nid = page_to_nid(newpage);

		SetHPageTemporary(oldpage);
		ClearHPageTemporary(newpage);

		/*
		 * There is no need to transfer the per-node surplus state
		 * when we do not cross the node.
		 */
		if (new_nid == old_nid)
			return;
		spin_lock_irq(&hugetlb_lock);
>>>>>>> upstream/android-13
		if (h->surplus_huge_pages_node[old_nid]) {
			h->surplus_huge_pages_node[old_nid]--;
			h->surplus_huge_pages_node[new_nid]++;
		}
<<<<<<< HEAD
		spin_unlock(&hugetlb_lock);
	}
}
=======
		spin_unlock_irq(&hugetlb_lock);
	}
}

/*
 * This function will unconditionally remove all the shared pmd pgtable entries
 * within the specific vma for a hugetlbfs memory range.
 */
void hugetlb_unshare_all_pmds(struct vm_area_struct *vma)
{
	struct hstate *h = hstate_vma(vma);
	unsigned long sz = huge_page_size(h);
	struct mm_struct *mm = vma->vm_mm;
	struct mmu_notifier_range range;
	unsigned long address, start, end;
	spinlock_t *ptl;
	pte_t *ptep;

	if (!(vma->vm_flags & VM_MAYSHARE))
		return;

	start = ALIGN(vma->vm_start, PUD_SIZE);
	end = ALIGN_DOWN(vma->vm_end, PUD_SIZE);

	if (start >= end)
		return;

	/*
	 * No need to call adjust_range_if_pmd_sharing_possible(), because
	 * we have already done the PUD_SIZE alignment.
	 */
	mmu_notifier_range_init(&range, MMU_NOTIFY_CLEAR, 0, vma, mm,
				start, end);
	mmu_notifier_invalidate_range_start(&range);
	i_mmap_lock_write(vma->vm_file->f_mapping);
	for (address = start; address < end; address += PUD_SIZE) {
		unsigned long tmp = address;

		ptep = huge_pte_offset(mm, address, sz);
		if (!ptep)
			continue;
		ptl = huge_pte_lock(h, mm, ptep);
		/* We don't want 'address' to be changed */
		huge_pmd_unshare(mm, vma, &tmp, ptep);
		spin_unlock(ptl);
	}
	flush_hugetlb_tlb_range(vma, start, end);
	i_mmap_unlock_write(vma->vm_file->f_mapping);
	/*
	 * No need to call mmu_notifier_invalidate_range(), see
	 * Documentation/vm/mmu_notifier.rst.
	 */
	mmu_notifier_invalidate_range_end(&range);
}

#ifdef CONFIG_CMA
static bool cma_reserve_called __initdata;

static int __init cmdline_parse_hugetlb_cma(char *p)
{
	hugetlb_cma_size = memparse(p, &p);
	return 0;
}

early_param("hugetlb_cma", cmdline_parse_hugetlb_cma);

void __init hugetlb_cma_reserve(int order)
{
	unsigned long size, reserved, per_node;
	int nid;

	cma_reserve_called = true;

	if (!hugetlb_cma_size)
		return;

	if (hugetlb_cma_size < (PAGE_SIZE << order)) {
		pr_warn("hugetlb_cma: cma area should be at least %lu MiB\n",
			(PAGE_SIZE << order) / SZ_1M);
		return;
	}

	/*
	 * If 3 GB area is requested on a machine with 4 numa nodes,
	 * let's allocate 1 GB on first three nodes and ignore the last one.
	 */
	per_node = DIV_ROUND_UP(hugetlb_cma_size, nr_online_nodes);
	pr_info("hugetlb_cma: reserve %lu MiB, up to %lu MiB per node\n",
		hugetlb_cma_size / SZ_1M, per_node / SZ_1M);

	reserved = 0;
	for_each_node_state(nid, N_ONLINE) {
		int res;
		char name[CMA_MAX_NAME];

		size = min(per_node, hugetlb_cma_size - reserved);
		size = round_up(size, PAGE_SIZE << order);

		snprintf(name, sizeof(name), "hugetlb%d", nid);
		res = cma_declare_contiguous_nid(0, size, 0, PAGE_SIZE << order,
						 0, false, name,
						 &hugetlb_cma[nid], nid);
		if (res) {
			pr_warn("hugetlb_cma: reservation failed: err %d, node %d",
				res, nid);
			continue;
		}

		reserved += size;
		pr_info("hugetlb_cma: reserved %lu MiB on node %d\n",
			size / SZ_1M, nid);

		if (reserved >= hugetlb_cma_size)
			break;
	}
}

void __init hugetlb_cma_check(void)
{
	if (!hugetlb_cma_size || cma_reserve_called)
		return;

	pr_warn("hugetlb_cma: the option isn't supported by current arch\n");
}

#endif /* CONFIG_CMA */
>>>>>>> upstream/android-13
