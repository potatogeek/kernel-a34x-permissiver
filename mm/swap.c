<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/mm/swap.c
 *
 *  Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 */

/*
 * This file contains the default values for the operation of the
 * Linux VM subsystem. Fine-tuning documentation can be found in
<<<<<<< HEAD
 * Documentation/sysctl/vm.txt.
=======
 * Documentation/admin-guide/sysctl/vm.rst.
>>>>>>> upstream/android-13
 * Started 18.12.91
 * Swap aging added 23.2.95, Stephen Tweedie.
 * Buffermem limits added 12.3.98, Rik van Riel.
 */

#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <linux/swap.h>
#include <linux/mman.h>
#include <linux/pagemap.h>
#include <linux/pagevec.h>
#include <linux/init.h>
#include <linux/export.h>
#include <linux/mm_inline.h>
#include <linux/percpu_counter.h>
#include <linux/memremap.h>
#include <linux/percpu.h>
#include <linux/cpu.h>
#include <linux/notifier.h>
#include <linux/backing-dev.h>
<<<<<<< HEAD
#include <linux/memremap.h>
=======
>>>>>>> upstream/android-13
#include <linux/memcontrol.h>
#include <linux/gfp.h>
#include <linux/uio.h>
#include <linux/hugetlb.h>
#include <linux/page_idle.h>
<<<<<<< HEAD
=======
#include <linux/local_lock.h>
#include <linux/buffer_head.h>
>>>>>>> upstream/android-13

#include "internal.h"

#define CREATE_TRACE_POINTS
#include <trace/events/pagemap.h>

/* How many pages do we try to swap or page in/out together? */
int page_cluster;

<<<<<<< HEAD
static DEFINE_PER_CPU(struct pagevec, lru_add_pvec);
static DEFINE_PER_CPU(struct pagevec, lru_rotate_pvecs);
static DEFINE_PER_CPU(struct pagevec, lru_deactivate_file_pvecs);
static DEFINE_PER_CPU(struct pagevec, lru_lazyfree_pvecs);
#ifdef CONFIG_SMP
static DEFINE_PER_CPU(struct pagevec, activate_page_pvecs);
#endif
=======
/* Protecting only lru_rotate.pvec which requires disabling interrupts */
struct lru_rotate {
	local_lock_t lock;
	struct pagevec pvec;
};
static DEFINE_PER_CPU(struct lru_rotate, lru_rotate) = {
	.lock = INIT_LOCAL_LOCK(lock),
};

/*
 * The following struct pagevec are grouped together because they are protected
 * by disabling preemption (and interrupts remain enabled).
 */
struct lru_pvecs {
	local_lock_t lock;
	struct pagevec lru_add;
	struct pagevec lru_deactivate_file;
	struct pagevec lru_deactivate;
	struct pagevec lru_lazyfree;
#ifdef CONFIG_SMP
	struct pagevec activate_page;
#endif
};
static DEFINE_PER_CPU(struct lru_pvecs, lru_pvecs) = {
	.lock = INIT_LOCAL_LOCK(lock),
};
>>>>>>> upstream/android-13

/*
 * This path almost never happens for VM activity - pages are normally
 * freed via pagevecs.  But it gets used by networking.
 */
static void __page_cache_release(struct page *page)
{
	if (PageLRU(page)) {
<<<<<<< HEAD
		struct zone *zone = page_zone(page);
		struct lruvec *lruvec;
		unsigned long flags;

		spin_lock_irqsave(zone_lru_lock(zone), flags);
		lruvec = mem_cgroup_page_lruvec(page, zone->zone_pgdat);
		VM_BUG_ON_PAGE(!PageLRU(page), page);
		__ClearPageLRU(page);
		del_page_from_lru_list(page, lruvec, page_off_lru(page));
		spin_unlock_irqrestore(zone_lru_lock(zone), flags);
	}
	__ClearPageWaiters(page);
	mem_cgroup_uncharge(page);
=======
		struct lruvec *lruvec;
		unsigned long flags;

		lruvec = lock_page_lruvec_irqsave(page, &flags);
		del_page_from_lru_list(page, lruvec);
		__clear_page_lru_flags(page);
		unlock_page_lruvec_irqrestore(lruvec, flags);
	}
	__ClearPageWaiters(page);
>>>>>>> upstream/android-13
}

static void __put_single_page(struct page *page)
{
	__page_cache_release(page);
<<<<<<< HEAD
	free_unref_page(page);
=======
	mem_cgroup_uncharge(page);
	free_unref_page(page, 0);
>>>>>>> upstream/android-13
}

static void __put_compound_page(struct page *page)
{
<<<<<<< HEAD
	compound_page_dtor *dtor;

=======
>>>>>>> upstream/android-13
	/*
	 * __page_cache_release() is supposed to be called for thp, not for
	 * hugetlb. This is because hugetlb page does never have PageLRU set
	 * (it's never listed to any LRU lists) and no memcg routines should
	 * be called for hugetlb (it has a separate hugetlb_cgroup.)
	 */
	if (!PageHuge(page))
		__page_cache_release(page);
<<<<<<< HEAD
	dtor = get_compound_page_dtor(page);
	(*dtor)(page);
=======
	destroy_compound_page(page);
>>>>>>> upstream/android-13
}

void __put_page(struct page *page)
{
	if (is_zone_device_page(page)) {
		put_dev_pagemap(page->pgmap);

		/*
		 * The page belongs to the device that created pgmap. Do
		 * not return it to page allocator.
		 */
		return;
	}

	if (unlikely(PageCompound(page)))
		__put_compound_page(page);
	else
		__put_single_page(page);
}
EXPORT_SYMBOL(__put_page);

/**
 * put_pages_list() - release a list of pages
 * @pages: list of pages threaded on page->lru
 *
 * Release a list of pages which are strung together on page.lru.  Currently
 * used by read_cache_pages() and related error recovery code.
 */
void put_pages_list(struct list_head *pages)
{
	while (!list_empty(pages)) {
		struct page *victim;

<<<<<<< HEAD
		victim = list_entry(pages->prev, struct page, lru);
=======
		victim = lru_to_page(pages);
>>>>>>> upstream/android-13
		list_del(&victim->lru);
		put_page(victim);
	}
}
EXPORT_SYMBOL(put_pages_list);

/*
 * get_kernel_pages() - pin kernel pages in memory
 * @kiov:	An array of struct kvec structures
 * @nr_segs:	number of segments to pin
 * @write:	pinning for read/write, currently ignored
 * @pages:	array that receives pointers to the pages pinned.
 *		Should be at least nr_segs long.
 *
 * Returns number of pages pinned. This may be fewer than the number
 * requested. If nr_pages is 0 or negative, returns 0. If no pages
 * were pinned, returns -errno. Each page returned must be released
 * with a put_page() call when it is finished with.
 */
int get_kernel_pages(const struct kvec *kiov, int nr_segs, int write,
		struct page **pages)
{
	int seg;

	for (seg = 0; seg < nr_segs; seg++) {
		if (WARN_ON(kiov[seg].iov_len != PAGE_SIZE))
			return seg;

		pages[seg] = kmap_to_page(kiov[seg].iov_base);
		get_page(pages[seg]);
	}

	return seg;
}
EXPORT_SYMBOL_GPL(get_kernel_pages);

<<<<<<< HEAD
/*
 * get_kernel_page() - pin a kernel page in memory
 * @start:	starting kernel address
 * @write:	pinning for read/write, currently ignored
 * @pages:	array that receives pointer to the page pinned.
 *		Must be at least nr_segs long.
 *
 * Returns 1 if page is pinned. If the page was not pinned, returns
 * -errno. The page returned must be released with a put_page() call
 * when it is finished with.
 */
int get_kernel_page(unsigned long start, int write, struct page **pages)
{
	const struct kvec kiov = {
		.iov_base = (void *)start,
		.iov_len = PAGE_SIZE
	};

	return get_kernel_pages(&kiov, 1, write, pages);
}
EXPORT_SYMBOL_GPL(get_kernel_page);

static void pagevec_lru_move_fn(struct pagevec *pvec,
	void (*move_fn)(struct page *page, struct lruvec *lruvec, void *arg),
	void *arg)
{
	int i;
	struct pglist_data *pgdat = NULL;
	struct lruvec *lruvec;
=======
static void pagevec_lru_move_fn(struct pagevec *pvec,
	void (*move_fn)(struct page *page, struct lruvec *lruvec))
{
	int i;
	struct lruvec *lruvec = NULL;
>>>>>>> upstream/android-13
	unsigned long flags = 0;

	for (i = 0; i < pagevec_count(pvec); i++) {
		struct page *page = pvec->pages[i];
<<<<<<< HEAD
		struct pglist_data *pagepgdat = page_pgdat(page);

		if (pagepgdat != pgdat) {
			if (pgdat)
				spin_unlock_irqrestore(&pgdat->lru_lock, flags);
			pgdat = pagepgdat;
			spin_lock_irqsave(&pgdat->lru_lock, flags);
		}

		lruvec = mem_cgroup_page_lruvec(page, pgdat);
		(*move_fn)(page, lruvec, arg);
	}
	if (pgdat)
		spin_unlock_irqrestore(&pgdat->lru_lock, flags);
=======

		/* block memcg migration during page moving between lru */
		if (!TestClearPageLRU(page))
			continue;

		lruvec = relock_page_lruvec_irqsave(page, lruvec, &flags);
		(*move_fn)(page, lruvec);

		SetPageLRU(page);
	}
	if (lruvec)
		unlock_page_lruvec_irqrestore(lruvec, flags);
>>>>>>> upstream/android-13
	release_pages(pvec->pages, pvec->nr);
	pagevec_reinit(pvec);
}

<<<<<<< HEAD
static void pagevec_move_tail_fn(struct page *page, struct lruvec *lruvec,
				 void *arg)
{
	int *pgmoved = arg;

	if (PageLRU(page) && !PageUnevictable(page)) {
		del_page_from_lru_list(page, lruvec, page_lru(page));
		ClearPageActive(page);
		add_page_to_lru_list_tail(page, lruvec, page_lru(page));
		(*pgmoved)++;
	}
}

/*
 * pagevec_move_tail() must be called with IRQ disabled.
 * Otherwise this may cause nasty races.
 */
static void pagevec_move_tail(struct pagevec *pvec)
{
	int pgmoved = 0;

	pagevec_lru_move_fn(pvec, pagevec_move_tail_fn, &pgmoved);
	__count_vm_events(PGROTATED, pgmoved);
=======
static void pagevec_move_tail_fn(struct page *page, struct lruvec *lruvec)
{
	if (!PageUnevictable(page)) {
		del_page_from_lru_list(page, lruvec);
		ClearPageActive(page);
		add_page_to_lru_list_tail(page, lruvec);
		__count_vm_events(PGROTATED, thp_nr_pages(page));
	}
}

/* return true if pagevec needs to drain */
static bool pagevec_add_and_need_flush(struct pagevec *pvec, struct page *page)
{
	bool ret = false;

	if (!pagevec_add(pvec, page) || PageCompound(page) ||
			lru_cache_disabled())
		ret = true;

	return ret;
>>>>>>> upstream/android-13
}

/*
 * Writeback is about to end against a page which has been marked for immediate
 * reclaim.  If it still appears to be reclaimable, move it to the tail of the
 * inactive list.
<<<<<<< HEAD
=======
 *
 * rotate_reclaimable_page() must disable IRQs, to prevent nasty races.
>>>>>>> upstream/android-13
 */
void rotate_reclaimable_page(struct page *page)
{
	if (!PageLocked(page) && !PageDirty(page) &&
	    !PageUnevictable(page) && PageLRU(page)) {
		struct pagevec *pvec;
		unsigned long flags;

		get_page(page);
<<<<<<< HEAD
		local_irq_save(flags);
		pvec = this_cpu_ptr(&lru_rotate_pvecs);
		if (!pagevec_add(pvec, page) || PageCompound(page))
			pagevec_move_tail(pvec);
		local_irq_restore(flags);
	}
}

static void update_page_reclaim_stat(struct lruvec *lruvec,
				     int file, int rotated)
{
	struct zone_reclaim_stat *reclaim_stat = &lruvec->reclaim_stat;

	reclaim_stat->recent_scanned[file]++;
	if (rotated)
		reclaim_stat->recent_rotated[file]++;
}

static void __activate_page(struct page *page, struct lruvec *lruvec,
			    void *arg)
{
	if (PageLRU(page) && !PageActive(page) && !PageUnevictable(page)) {
		int file = page_is_file_cache(page);
		int lru = page_lru_base_type(page);

		del_page_from_lru_list(page, lruvec, lru);
		SetPageActive(page);
		lru += LRU_ACTIVE;
		add_page_to_lru_list(page, lruvec, lru);
		trace_mm_lru_activate(page);

		__count_vm_event(PGACTIVATE);
		update_page_reclaim_stat(lruvec, file, 1);
=======
		local_lock_irqsave(&lru_rotate.lock, flags);
		pvec = this_cpu_ptr(&lru_rotate.pvec);
		if (pagevec_add_and_need_flush(pvec, page))
			pagevec_lru_move_fn(pvec, pagevec_move_tail_fn);
		local_unlock_irqrestore(&lru_rotate.lock, flags);
	}
}

void lru_note_cost(struct lruvec *lruvec, bool file, unsigned int nr_pages)
{
	do {
		unsigned long lrusize;

		/*
		 * Hold lruvec->lru_lock is safe here, since
		 * 1) The pinned lruvec in reclaim, or
		 * 2) From a pre-LRU page during refault (which also holds the
		 *    rcu lock, so would be safe even if the page was on the LRU
		 *    and could move simultaneously to a new lruvec).
		 */
		spin_lock_irq(&lruvec->lru_lock);
		/* Record cost event */
		if (file)
			lruvec->file_cost += nr_pages;
		else
			lruvec->anon_cost += nr_pages;

		/*
		 * Decay previous events
		 *
		 * Because workloads change over time (and to avoid
		 * overflow) we keep these statistics as a floating
		 * average, which ends up weighing recent refaults
		 * more than old ones.
		 */
		lrusize = lruvec_page_state(lruvec, NR_INACTIVE_ANON) +
			  lruvec_page_state(lruvec, NR_ACTIVE_ANON) +
			  lruvec_page_state(lruvec, NR_INACTIVE_FILE) +
			  lruvec_page_state(lruvec, NR_ACTIVE_FILE);

		if (lruvec->file_cost + lruvec->anon_cost > lrusize / 4) {
			lruvec->file_cost /= 2;
			lruvec->anon_cost /= 2;
		}
		spin_unlock_irq(&lruvec->lru_lock);
	} while ((lruvec = parent_lruvec(lruvec)));
}

void lru_note_cost_page(struct page *page)
{
	lru_note_cost(mem_cgroup_page_lruvec(page),
		      page_is_file_lru(page), thp_nr_pages(page));
}

static void __activate_page(struct page *page, struct lruvec *lruvec)
{
	if (!PageActive(page) && !PageUnevictable(page)) {
		int nr_pages = thp_nr_pages(page);

		del_page_from_lru_list(page, lruvec);
		SetPageActive(page);
		add_page_to_lru_list(page, lruvec);
		trace_mm_lru_activate(page);

		__count_vm_events(PGACTIVATE, nr_pages);
		__count_memcg_events(lruvec_memcg(lruvec), PGACTIVATE,
				     nr_pages);
>>>>>>> upstream/android-13
	}
}

#ifdef CONFIG_SMP
static void activate_page_drain(int cpu)
{
<<<<<<< HEAD
	struct pagevec *pvec = &per_cpu(activate_page_pvecs, cpu);

	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, __activate_page, NULL);
=======
	struct pagevec *pvec = &per_cpu(lru_pvecs.activate_page, cpu);

	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, __activate_page);
>>>>>>> upstream/android-13
}

static bool need_activate_page_drain(int cpu)
{
<<<<<<< HEAD
	return pagevec_count(&per_cpu(activate_page_pvecs, cpu)) != 0;
=======
	return pagevec_count(&per_cpu(lru_pvecs.activate_page, cpu)) != 0;
>>>>>>> upstream/android-13
}

void activate_page(struct page *page)
{
	page = compound_head(page);
	if (PageLRU(page) && !PageActive(page) && !PageUnevictable(page)) {
<<<<<<< HEAD
		struct pagevec *pvec = &get_cpu_var(activate_page_pvecs);

		get_page(page);
		if (!pagevec_add(pvec, page) || PageCompound(page))
			pagevec_lru_move_fn(pvec, __activate_page, NULL);
		put_cpu_var(activate_page_pvecs);
=======
		struct pagevec *pvec;

		local_lock(&lru_pvecs.lock);
		pvec = this_cpu_ptr(&lru_pvecs.activate_page);
		get_page(page);
		if (pagevec_add_and_need_flush(pvec, page))
			pagevec_lru_move_fn(pvec, __activate_page);
		local_unlock(&lru_pvecs.lock);
>>>>>>> upstream/android-13
	}
}

#else
static inline void activate_page_drain(int cpu)
{
}

void activate_page(struct page *page)
{
<<<<<<< HEAD
	struct zone *zone = page_zone(page);

	page = compound_head(page);
	spin_lock_irq(zone_lru_lock(zone));
	__activate_page(page, mem_cgroup_page_lruvec(page, zone->zone_pgdat), NULL);
	spin_unlock_irq(zone_lru_lock(zone));
=======
	struct lruvec *lruvec;

	page = compound_head(page);
	if (TestClearPageLRU(page)) {
		lruvec = lock_page_lruvec_irq(page);
		__activate_page(page, lruvec);
		unlock_page_lruvec_irq(lruvec);
		SetPageLRU(page);
	}
>>>>>>> upstream/android-13
}
#endif

static void __lru_cache_activate_page(struct page *page)
{
<<<<<<< HEAD
	struct pagevec *pvec = &get_cpu_var(lru_add_pvec);
	int i;

=======
	struct pagevec *pvec;
	int i;

	local_lock(&lru_pvecs.lock);
	pvec = this_cpu_ptr(&lru_pvecs.lru_add);

>>>>>>> upstream/android-13
	/*
	 * Search backwards on the optimistic assumption that the page being
	 * activated has just been added to this pagevec. Note that only
	 * the local pagevec is examined as a !PageLRU page could be in the
	 * process of being released, reclaimed, migrated or on a remote
	 * pagevec that is currently being drained. Furthermore, marking
	 * a remote pagevec's page PageActive potentially hits a race where
	 * a page is marked PageActive just after it is added to the inactive
	 * list causing accounting errors and BUG_ON checks to trigger.
	 */
	for (i = pagevec_count(pvec) - 1; i >= 0; i--) {
		struct page *pagevec_page = pvec->pages[i];

		if (pagevec_page == page) {
			SetPageActive(page);
			break;
		}
	}

<<<<<<< HEAD
	put_cpu_var(lru_add_pvec);
}

=======
	local_unlock(&lru_pvecs.lock);
}

#ifdef CONFIG_LRU_GEN
static void page_inc_refs(struct page *page)
{
	unsigned long refs;
	unsigned long old_flags, new_flags;

	if (PageUnevictable(page))
		return;

	/* see the comment on MAX_NR_TIERS */
	do {
		new_flags = old_flags = READ_ONCE(page->flags);

		if (!(new_flags & BIT(PG_referenced))) {
			new_flags |= BIT(PG_referenced);
			continue;
		}

		if (!(new_flags & BIT(PG_workingset))) {
			new_flags |= BIT(PG_workingset);
			continue;
		}

		refs = new_flags & LRU_REFS_MASK;
		refs = min(refs + BIT(LRU_REFS_PGOFF), LRU_REFS_MASK);

		new_flags &= ~LRU_REFS_MASK;
		new_flags |= refs;
	} while (new_flags != old_flags &&
		 cmpxchg(&page->flags, old_flags, new_flags) != old_flags);
}
#else
static void page_inc_refs(struct page *page)
{
}
#endif /* CONFIG_LRU_GEN */

>>>>>>> upstream/android-13
/*
 * Mark a page as having seen activity.
 *
 * inactive,unreferenced	->	inactive,referenced
 * inactive,referenced		->	active,unreferenced
 * active,unreferenced		->	active,referenced
 *
 * When a newly allocated page is not yet visible, so safe for non-atomic ops,
 * __SetPageReferenced(page) may be substituted for mark_page_accessed(page).
 */
void mark_page_accessed(struct page *page)
{
	page = compound_head(page);
<<<<<<< HEAD
	if (!PageActive(page) && !PageUnevictable(page) &&
			PageReferenced(page)) {

		/*
		 * If the page is on the LRU, queue it for activation via
		 * activate_page_pvecs. Otherwise, assume the page is on a
=======

	if (lru_gen_enabled()) {
		page_inc_refs(page);
		return;
	}

	trace_android_vh_mark_page_accessed(page);
	if (!PageReferenced(page)) {
		SetPageReferenced(page);
	} else if (PageUnevictable(page)) {
		/*
		 * Unevictable pages are on the "LRU_UNEVICTABLE" list. But,
		 * this list is never rotated or maintained, so marking an
		 * evictable page accessed has no effect.
		 */
	} else if (!PageActive(page)) {
		/*
		 * If the page is on the LRU, queue it for activation via
		 * lru_pvecs.activate_page. Otherwise, assume the page is on a
>>>>>>> upstream/android-13
		 * pagevec, mark it active and it'll be moved to the active
		 * LRU on the next drain.
		 */
		if (PageLRU(page))
			activate_page(page);
		else
			__lru_cache_activate_page(page);
		ClearPageReferenced(page);
<<<<<<< HEAD
		if (page_is_file_cache(page))
			workingset_activation(page);
	} else if (!PageReferenced(page)) {
		SetPageReferenced(page);
=======
		workingset_activation(page);
>>>>>>> upstream/android-13
	}
	if (page_is_idle(page))
		clear_page_idle(page);
}
EXPORT_SYMBOL(mark_page_accessed);

<<<<<<< HEAD
static void __lru_cache_add(struct page *page)
{
	struct pagevec *pvec = &get_cpu_var(lru_add_pvec);

	get_page(page);
	if (!pagevec_add(pvec, page) || PageCompound(page))
		__pagevec_lru_add(pvec);
	put_cpu_var(lru_add_pvec);
}

/**
 * lru_cache_add_anon - add a page to the page lists
 * @page: the page to add
 */
void lru_cache_add_anon(struct page *page)
{
	if (PageActive(page))
		ClearPageActive(page);
	__lru_cache_add(page);
}

void lru_cache_add_file(struct page *page)
{
	if (PageActive(page))
		ClearPageActive(page);
	__lru_cache_add(page);
}
EXPORT_SYMBOL(lru_cache_add_file);

=======
>>>>>>> upstream/android-13
/**
 * lru_cache_add - add a page to a page list
 * @page: the page to be added to the LRU.
 *
 * Queue the page for addition to the LRU via pagevec. The decision on whether
 * to add the page to the [in]active [file|anon] list is deferred until the
 * pagevec is drained. This gives a chance for the caller of lru_cache_add()
 * have the page added to the active list using mark_page_accessed().
 */
void lru_cache_add(struct page *page)
{
<<<<<<< HEAD
	VM_BUG_ON_PAGE(PageActive(page) && PageUnevictable(page), page);
	VM_BUG_ON_PAGE(PageLRU(page), page);
	__lru_cache_add(page);
}

/**
 * lru_cache_add_active_or_unevictable
 * @page:  the page to be added to LRU
 * @vma:   vma in which page is mapped for determining reclaimability
 *
 * Place @page on the active or unevictable LRU list, depending on its
 * evictability.  Note that if the page is not evictable, it goes
 * directly back onto it's zone's unevictable list, it does NOT use a
 * per cpu pagevec.
 */
void __lru_cache_add_active_or_unevictable(struct page *page,
					   unsigned long vma_flags)
{
	VM_BUG_ON_PAGE(PageLRU(page), page);

	if (likely((vma_flags & (VM_LOCKED | VM_SPECIAL)) != VM_LOCKED))
		SetPageActive(page);
	else if (!TestSetPageMlocked(page)) {
		/*
		 * We use the irq-unsafe __mod_zone_page_stat because this
		 * counter is not modified from interrupt context, and the pte
		 * lock is held(spinlock), which implies preemption disabled.
		 */
		__mod_zone_page_state(page_zone(page), NR_MLOCK,
				    hpage_nr_pages(page));
		count_vm_event(UNEVICTABLE_PGMLOCKED);
=======
	struct pagevec *pvec;

	VM_BUG_ON_PAGE(PageActive(page) && PageUnevictable(page), page);
	VM_BUG_ON_PAGE(PageLRU(page), page);

	/* see the comment in lru_gen_add_page() */
	if (lru_gen_enabled() && !PageUnevictable(page) &&
	    lru_gen_in_fault() && !(current->flags & PF_MEMALLOC))
		SetPageActive(page);

	get_page(page);
	local_lock(&lru_pvecs.lock);
	pvec = this_cpu_ptr(&lru_pvecs.lru_add);
	if (pagevec_add_and_need_flush(pvec, page))
		__pagevec_lru_add(pvec);
	local_unlock(&lru_pvecs.lock);
}
EXPORT_SYMBOL(lru_cache_add);

/**
 * lru_cache_add_inactive_or_unevictable
 * @page:  the page to be added to LRU
 * @vma:   vma in which page is mapped for determining reclaimability
 *
 * Place @page on the inactive or unevictable LRU list, depending on its
 * evictability.
 */
void lru_cache_add_inactive_or_unevictable(struct page *page,
					 struct vm_area_struct *vma)
{
	bool unevictable;

	VM_BUG_ON_PAGE(PageLRU(page), page);

	unevictable = (vma->vm_flags & (VM_LOCKED | VM_SPECIAL)) == VM_LOCKED;
	if (unlikely(unevictable) && !TestSetPageMlocked(page)) {
		int nr_pages = thp_nr_pages(page);
		/*
		 * We use the irq-unsafe __mod_zone_page_state because this
		 * counter is not modified from interrupt context, and the pte
		 * lock is held(spinlock), which implies preemption disabled.
		 */
		__mod_zone_page_state(page_zone(page), NR_MLOCK, nr_pages);
		count_vm_events(UNEVICTABLE_PGMLOCKED, nr_pages);
>>>>>>> upstream/android-13
	}
	lru_cache_add(page);
}

/*
 * If the page can not be invalidated, it is moved to the
 * inactive list to speed up its reclaim.  It is moved to the
 * head of the list, rather than the tail, to give the flusher
 * threads some time to write it out, as this is much more
 * effective than the single-page writeout from reclaim.
 *
 * If the page isn't page_mapped and dirty/writeback, the page
 * could reclaim asap using PG_reclaim.
 *
 * 1. active, mapped page -> none
 * 2. active, dirty/writeback page -> inactive, head, PG_reclaim
 * 3. inactive, mapped page -> none
 * 4. inactive, dirty/writeback page -> inactive, head, PG_reclaim
 * 5. inactive, clean -> inactive, tail
 * 6. Others -> none
 *
 * In 4, why it moves inactive's head, the VM expects the page would
 * be write it out by flusher threads as this is much more effective
 * than the single-page writeout from reclaim.
 */
<<<<<<< HEAD
static void lru_deactivate_file_fn(struct page *page, struct lruvec *lruvec,
			      void *arg)
{
	int lru, file;
	bool active;

	if (!PageLRU(page))
		return;
=======
static void lru_deactivate_file_fn(struct page *page, struct lruvec *lruvec)
{
	bool active = PageActive(page);
	int nr_pages = thp_nr_pages(page);
>>>>>>> upstream/android-13

	if (PageUnevictable(page))
		return;

	/* Some processes are using the page */
	if (page_mapped(page))
		return;

<<<<<<< HEAD
	active = PageActive(page);
	file = page_is_file_cache(page);
	lru = page_lru_base_type(page);

	del_page_from_lru_list(page, lruvec, lru + active);
	ClearPageActive(page);
	ClearPageReferenced(page);
	add_page_to_lru_list(page, lruvec, lru);
=======
	del_page_from_lru_list(page, lruvec);
	ClearPageActive(page);
	ClearPageReferenced(page);
>>>>>>> upstream/android-13

	if (PageWriteback(page) || PageDirty(page)) {
		/*
		 * PG_reclaim could be raced with end_page_writeback
		 * It can make readahead confusing.  But race window
		 * is _really_ small and  it's non-critical problem.
		 */
<<<<<<< HEAD
=======
		add_page_to_lru_list(page, lruvec);
>>>>>>> upstream/android-13
		SetPageReclaim(page);
	} else {
		/*
		 * The page's writeback ends up during pagevec
<<<<<<< HEAD
		 * We moves tha page into tail of inactive.
		 */
		list_move_tail(&page->lru, &lruvec->lists[lru]);
		__count_vm_event(PGROTATED);
	}

	if (active)
		__count_vm_event(PGDEACTIVATE);
	update_page_reclaim_stat(lruvec, file, 0);
}


static void lru_lazyfree_fn(struct page *page, struct lruvec *lruvec,
			    void *arg)
{
	if (PageLRU(page) && PageAnon(page) && PageSwapBacked(page) &&
	    !PageSwapCache(page) && !PageUnevictable(page)) {
		bool active = PageActive(page);

		del_page_from_lru_list(page, lruvec,
				       LRU_INACTIVE_ANON + active);
		ClearPageActive(page);
		ClearPageReferenced(page);
		/*
		 * lazyfree pages are clean anonymous pages. They have
		 * SwapBacked flag cleared to distinguish normal anonymous
		 * pages
		 */
		ClearPageSwapBacked(page);
		add_page_to_lru_list(page, lruvec, LRU_INACTIVE_FILE);

		__count_vm_events(PGLAZYFREE, hpage_nr_pages(page));
		count_memcg_page_event(page, PGLAZYFREE);
		update_page_reclaim_stat(lruvec, 1, 0);
=======
		 * We move that page into tail of inactive.
		 */
		add_page_to_lru_list_tail(page, lruvec);
		__count_vm_events(PGROTATED, nr_pages);
	}

	if (active) {
		__count_vm_events(PGDEACTIVATE, nr_pages);
		__count_memcg_events(lruvec_memcg(lruvec), PGDEACTIVATE,
				     nr_pages);
	}
}

static void lru_deactivate_fn(struct page *page, struct lruvec *lruvec)
{
	if (!PageUnevictable(page) && (PageActive(page) || lru_gen_enabled())) {
		int nr_pages = thp_nr_pages(page);

		del_page_from_lru_list(page, lruvec);
		ClearPageActive(page);
		ClearPageReferenced(page);
		add_page_to_lru_list(page, lruvec);

		__count_vm_events(PGDEACTIVATE, nr_pages);
		__count_memcg_events(lruvec_memcg(lruvec), PGDEACTIVATE,
				     nr_pages);
	}
}

static void lru_lazyfree_fn(struct page *page, struct lruvec *lruvec)
{
	if (PageAnon(page) && PageSwapBacked(page) &&
	    !PageSwapCache(page) && !PageUnevictable(page)) {
		int nr_pages = thp_nr_pages(page);

		del_page_from_lru_list(page, lruvec);
		ClearPageActive(page);
		ClearPageReferenced(page);
		/*
		 * Lazyfree pages are clean anonymous pages.  They have
		 * PG_swapbacked flag cleared, to distinguish them from normal
		 * anonymous pages
		 */
		ClearPageSwapBacked(page);
		add_page_to_lru_list(page, lruvec);

		__count_vm_events(PGLAZYFREE, nr_pages);
		__count_memcg_events(lruvec_memcg(lruvec), PGLAZYFREE,
				     nr_pages);
>>>>>>> upstream/android-13
	}
}

/*
 * Drain pages out of the cpu's pagevecs.
 * Either "cpu" is the current CPU, and preemption has already been
 * disabled; or "cpu" is being hot-unplugged, and is already dead.
 */
void lru_add_drain_cpu(int cpu)
{
<<<<<<< HEAD
	struct pagevec *pvec = &per_cpu(lru_add_pvec, cpu);
=======
	struct pagevec *pvec = &per_cpu(lru_pvecs.lru_add, cpu);
>>>>>>> upstream/android-13

	if (pagevec_count(pvec))
		__pagevec_lru_add(pvec);

<<<<<<< HEAD
	pvec = &per_cpu(lru_rotate_pvecs, cpu);
	if (pagevec_count(pvec)) {
		unsigned long flags;

		/* No harm done if a racing interrupt already did this */
		local_irq_save(flags);
		pagevec_move_tail(pvec);
		local_irq_restore(flags);
	}

	pvec = &per_cpu(lru_deactivate_file_pvecs, cpu);
	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, lru_deactivate_file_fn, NULL);

	pvec = &per_cpu(lru_lazyfree_pvecs, cpu);
	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, lru_lazyfree_fn, NULL);
=======
	pvec = &per_cpu(lru_rotate.pvec, cpu);
	/* Disabling interrupts below acts as a compiler barrier. */
	if (data_race(pagevec_count(pvec))) {
		unsigned long flags;

		/* No harm done if a racing interrupt already did this */
		local_lock_irqsave(&lru_rotate.lock, flags);
		pagevec_lru_move_fn(pvec, pagevec_move_tail_fn);
		local_unlock_irqrestore(&lru_rotate.lock, flags);
	}

	pvec = &per_cpu(lru_pvecs.lru_deactivate_file, cpu);
	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, lru_deactivate_file_fn);

	pvec = &per_cpu(lru_pvecs.lru_deactivate, cpu);
	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, lru_deactivate_fn);

	pvec = &per_cpu(lru_pvecs.lru_lazyfree, cpu);
	if (pagevec_count(pvec))
		pagevec_lru_move_fn(pvec, lru_lazyfree_fn);
>>>>>>> upstream/android-13

	activate_page_drain(cpu);
}

/**
 * deactivate_file_page - forcefully deactivate a file page
 * @page: page to deactivate
 *
 * This function hints the VM that @page is a good reclaim candidate,
 * for example if its invalidation fails due to the page being dirty
 * or under writeback.
 */
void deactivate_file_page(struct page *page)
{
	/*
	 * In a workload with many unevictable page such as mprotect,
	 * unevictable page deactivation for accelerating reclaim is pointless.
	 */
	if (PageUnevictable(page))
		return;

	if (likely(get_page_unless_zero(page))) {
<<<<<<< HEAD
		struct pagevec *pvec = &get_cpu_var(lru_deactivate_file_pvecs);

		if (!pagevec_add(pvec, page) || PageCompound(page))
			pagevec_lru_move_fn(pvec, lru_deactivate_file_fn, NULL);
		put_cpu_var(lru_deactivate_file_pvecs);
=======
		struct pagevec *pvec;

		local_lock(&lru_pvecs.lock);
		pvec = this_cpu_ptr(&lru_pvecs.lru_deactivate_file);

		if (pagevec_add_and_need_flush(pvec, page))
			pagevec_lru_move_fn(pvec, lru_deactivate_file_fn);
		local_unlock(&lru_pvecs.lock);
	}
}

/*
 * deactivate_page - deactivate a page
 * @page: page to deactivate
 *
 * deactivate_page() moves @page to the inactive list if @page was on the active
 * list and was not an unevictable page.  This is done to accelerate the reclaim
 * of @page.
 */
void deactivate_page(struct page *page)
{
	if (PageLRU(page) && !PageUnevictable(page) && (PageActive(page) || lru_gen_enabled())) {
		struct pagevec *pvec;

		local_lock(&lru_pvecs.lock);
		pvec = this_cpu_ptr(&lru_pvecs.lru_deactivate);
		get_page(page);
		if (pagevec_add_and_need_flush(pvec, page))
			pagevec_lru_move_fn(pvec, lru_deactivate_fn);
		local_unlock(&lru_pvecs.lock);
>>>>>>> upstream/android-13
	}
}

/**
 * mark_page_lazyfree - make an anon page lazyfree
 * @page: page to deactivate
 *
 * mark_page_lazyfree() moves @page to the inactive file list.
 * This is done to accelerate the reclaim of @page.
 */
void mark_page_lazyfree(struct page *page)
{
	if (PageLRU(page) && PageAnon(page) && PageSwapBacked(page) &&
	    !PageSwapCache(page) && !PageUnevictable(page)) {
<<<<<<< HEAD
		struct pagevec *pvec = &get_cpu_var(lru_lazyfree_pvecs);

		get_page(page);
		if (!pagevec_add(pvec, page) || PageCompound(page))
			pagevec_lru_move_fn(pvec, lru_lazyfree_fn, NULL);
		put_cpu_var(lru_lazyfree_pvecs);
=======
		struct pagevec *pvec;

		local_lock(&lru_pvecs.lock);
		pvec = this_cpu_ptr(&lru_pvecs.lru_lazyfree);
		get_page(page);
		if (pagevec_add_and_need_flush(pvec, page))
			pagevec_lru_move_fn(pvec, lru_lazyfree_fn);
		local_unlock(&lru_pvecs.lock);
>>>>>>> upstream/android-13
	}
}

void lru_add_drain(void)
{
<<<<<<< HEAD
	lru_add_drain_cpu(get_cpu());
	put_cpu();
=======
	local_lock(&lru_pvecs.lock);
	lru_add_drain_cpu(smp_processor_id());
	local_unlock(&lru_pvecs.lock);
}

/*
 * It's called from per-cpu workqueue context in SMP case so
 * lru_add_drain_cpu and invalidate_bh_lrus_cpu should run on
 * the same cpu. It shouldn't be a problem in !SMP case since
 * the core is only one and the locks will disable preemption.
 */
static void lru_add_and_bh_lrus_drain(void)
{
	local_lock(&lru_pvecs.lock);
	lru_add_drain_cpu(smp_processor_id());
	local_unlock(&lru_pvecs.lock);
	invalidate_bh_lrus_cpu();
}

void lru_add_drain_cpu_zone(struct zone *zone)
{
	local_lock(&lru_pvecs.lock);
	lru_add_drain_cpu(smp_processor_id());
	drain_local_pages(zone);
	local_unlock(&lru_pvecs.lock);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_SMP

static DEFINE_PER_CPU(struct work_struct, lru_add_drain_work);

static void lru_add_drain_per_cpu(struct work_struct *dummy)
{
<<<<<<< HEAD
	lru_add_drain();
=======
	lru_add_and_bh_lrus_drain();
>>>>>>> upstream/android-13
}

/*
 * Doesn't need any cpu hotplug locking because we do rely on per-cpu
 * kworkers being shut down before our page_alloc_cpu_dead callback is
 * executed on the offlined cpu.
 * Calling this function with cpu hotplug locks held can actually lead
 * to obscure indirect dependencies via WQ context.
 */
<<<<<<< HEAD
void lru_add_drain_all(void)
{
	static DEFINE_MUTEX(lock);
	static struct cpumask has_work;
	int cpu;
=======
inline void __lru_add_drain_all(bool force_all_cpus)
{
	/*
	 * lru_drain_gen - Global pages generation number
	 *
	 * (A) Definition: global lru_drain_gen = x implies that all generations
	 *     0 < n <= x are already *scheduled* for draining.
	 *
	 * This is an optimization for the highly-contended use case where a
	 * user space workload keeps constantly generating a flow of pages for
	 * each CPU.
	 */
	static unsigned int lru_drain_gen;
	static struct cpumask has_work;
	static DEFINE_MUTEX(lock);
	unsigned cpu, this_gen;
>>>>>>> upstream/android-13

	/*
	 * Make sure nobody triggers this path before mm_percpu_wq is fully
	 * initialized.
	 */
	if (WARN_ON(!mm_percpu_wq))
		return;

<<<<<<< HEAD
	mutex_lock(&lock);
	cpumask_clear(&has_work);

	for_each_online_cpu(cpu) {
		struct work_struct *work = &per_cpu(lru_add_drain_work, cpu);

		if (pagevec_count(&per_cpu(lru_add_pvec, cpu)) ||
		    pagevec_count(&per_cpu(lru_rotate_pvecs, cpu)) ||
		    pagevec_count(&per_cpu(lru_deactivate_file_pvecs, cpu)) ||
		    pagevec_count(&per_cpu(lru_lazyfree_pvecs, cpu)) ||
		    need_activate_page_drain(cpu)) {
			INIT_WORK(work, lru_add_drain_per_cpu);
			queue_work_on(cpu, mm_percpu_wq, work);
			cpumask_set_cpu(cpu, &has_work);
=======
	/*
	 * Guarantee pagevec counter stores visible by this CPU are visible to
	 * other CPUs before loading the current drain generation.
	 */
	smp_mb();

	/*
	 * (B) Locally cache global LRU draining generation number
	 *
	 * The read barrier ensures that the counter is loaded before the mutex
	 * is taken. It pairs with smp_mb() inside the mutex critical section
	 * at (D).
	 */
	this_gen = smp_load_acquire(&lru_drain_gen);

	mutex_lock(&lock);

	/*
	 * (C) Exit the draining operation if a newer generation, from another
	 * lru_add_drain_all(), was already scheduled for draining. Check (A).
	 */
	if (unlikely(this_gen != lru_drain_gen && !force_all_cpus))
		goto done;

	/*
	 * (D) Increment global generation number
	 *
	 * Pairs with smp_load_acquire() at (B), outside of the critical
	 * section. Use a full memory barrier to guarantee that the new global
	 * drain generation number is stored before loading pagevec counters.
	 *
	 * This pairing must be done here, before the for_each_online_cpu loop
	 * below which drains the page vectors.
	 *
	 * Let x, y, and z represent some system CPU numbers, where x < y < z.
	 * Assume CPU #z is in the middle of the for_each_online_cpu loop
	 * below and has already reached CPU #y's per-cpu data. CPU #x comes
	 * along, adds some pages to its per-cpu vectors, then calls
	 * lru_add_drain_all().
	 *
	 * If the paired barrier is done at any later step, e.g. after the
	 * loop, CPU #x will just exit at (C) and miss flushing out all of its
	 * added pages.
	 */
	WRITE_ONCE(lru_drain_gen, lru_drain_gen + 1);
	smp_mb();

	cpumask_clear(&has_work);
	for_each_online_cpu(cpu) {
		struct work_struct *work = &per_cpu(lru_add_drain_work, cpu);

		if (force_all_cpus ||
		    pagevec_count(&per_cpu(lru_pvecs.lru_add, cpu)) ||
		    data_race(pagevec_count(&per_cpu(lru_rotate.pvec, cpu))) ||
		    pagevec_count(&per_cpu(lru_pvecs.lru_deactivate_file, cpu)) ||
		    pagevec_count(&per_cpu(lru_pvecs.lru_deactivate, cpu)) ||
		    pagevec_count(&per_cpu(lru_pvecs.lru_lazyfree, cpu)) ||
		    need_activate_page_drain(cpu) ||
		    has_bh_in_lru(cpu, NULL)) {
			INIT_WORK(work, lru_add_drain_per_cpu);
			queue_work_on(cpu, mm_percpu_wq, work);
			__cpumask_set_cpu(cpu, &has_work);
>>>>>>> upstream/android-13
		}
	}

	for_each_cpu(cpu, &has_work)
		flush_work(&per_cpu(lru_add_drain_work, cpu));

<<<<<<< HEAD
	mutex_unlock(&lock);
}
=======
done:
	mutex_unlock(&lock);
}

void lru_add_drain_all(void)
{
	__lru_add_drain_all(false);
}
>>>>>>> upstream/android-13
#else
void lru_add_drain_all(void)
{
	lru_add_drain();
}
<<<<<<< HEAD
#endif
=======
#endif /* CONFIG_SMP */

atomic_t lru_disable_count = ATOMIC_INIT(0);

/*
 * lru_cache_disable() needs to be called before we start compiling
 * a list of pages to be migrated using isolate_lru_page().
 * It drains pages on LRU cache and then disable on all cpus until
 * lru_cache_enable is called.
 *
 * Must be paired with a call to lru_cache_enable().
 */
void lru_cache_disable(void)
{
	atomic_inc(&lru_disable_count);
#ifdef CONFIG_SMP
	/*
	 * lru_add_drain_all in the force mode will schedule draining on
	 * all online CPUs so any calls of lru_cache_disabled wrapped by
	 * local_lock or preemption disabled would be ordered by that.
	 * The atomic operation doesn't need to have stronger ordering
	 * requirements because that is enforeced by the scheduling
	 * guarantees.
	 */
	__lru_add_drain_all(true);
#else
	lru_add_and_bh_lrus_drain();
#endif
}
>>>>>>> upstream/android-13

/**
 * release_pages - batched put_page()
 * @pages: array of pages to release
 * @nr: number of pages
 *
 * Decrement the reference count on all the pages in @pages.  If it
 * fell to zero, remove the page from the LRU and free it.
 */
void release_pages(struct page **pages, int nr)
{
	int i;
	LIST_HEAD(pages_to_free);
<<<<<<< HEAD
	struct pglist_data *locked_pgdat = NULL;
	struct lruvec *lruvec;
	unsigned long uninitialized_var(flags);
	unsigned int uninitialized_var(lock_batch);
=======
	struct lruvec *lruvec = NULL;
	unsigned long flags;
	unsigned int lock_batch;
>>>>>>> upstream/android-13

	for (i = 0; i < nr; i++) {
		struct page *page = pages[i];

		/*
		 * Make sure the IRQ-safe lock-holding time does not get
		 * excessive with a continuous string of pages from the
<<<<<<< HEAD
		 * same pgdat. The lock is held only if pgdat != NULL.
		 */
		if (locked_pgdat && ++lock_batch == SWAP_CLUSTER_MAX) {
			spin_unlock_irqrestore(&locked_pgdat->lru_lock, flags);
			locked_pgdat = NULL;
		}

=======
		 * same lruvec. The lock is held only if lruvec != NULL.
		 */
		if (lruvec && ++lock_batch == SWAP_CLUSTER_MAX) {
			unlock_page_lruvec_irqrestore(lruvec, flags);
			lruvec = NULL;
		}

		page = compound_head(page);
>>>>>>> upstream/android-13
		if (is_huge_zero_page(page))
			continue;

		if (is_zone_device_page(page)) {
<<<<<<< HEAD
			if (locked_pgdat) {
				spin_unlock_irqrestore(&locked_pgdat->lru_lock,
						       flags);
				locked_pgdat = NULL;
			}
			/*
			 * ZONE_DEVICE pages that return 'false' from
			 * put_devmap_managed_page() do not require special
			 * processing, and instead, expect a call to
			 * put_page_testzero().
			 */
			if (put_devmap_managed_page(page))
				continue;
		}

		page = compound_head(page);
=======
			if (lruvec) {
				unlock_page_lruvec_irqrestore(lruvec, flags);
				lruvec = NULL;
			}
			/*
			 * ZONE_DEVICE pages that return 'false' from
			 * page_is_devmap_managed() do not require special
			 * processing, and instead, expect a call to
			 * put_page_testzero().
			 */
			if (page_is_devmap_managed(page)) {
				put_devmap_managed_page(page);
				continue;
			}
			if (put_page_testzero(page))
				put_dev_pagemap(page->pgmap);
			continue;
		}

>>>>>>> upstream/android-13
		if (!put_page_testzero(page))
			continue;

		if (PageCompound(page)) {
<<<<<<< HEAD
			if (locked_pgdat) {
				spin_unlock_irqrestore(&locked_pgdat->lru_lock, flags);
				locked_pgdat = NULL;
=======
			if (lruvec) {
				unlock_page_lruvec_irqrestore(lruvec, flags);
				lruvec = NULL;
>>>>>>> upstream/android-13
			}
			__put_compound_page(page);
			continue;
		}

		if (PageLRU(page)) {
<<<<<<< HEAD
			struct pglist_data *pgdat = page_pgdat(page);

			if (pgdat != locked_pgdat) {
				if (locked_pgdat)
					spin_unlock_irqrestore(&locked_pgdat->lru_lock,
									flags);
				lock_batch = 0;
				locked_pgdat = pgdat;
				spin_lock_irqsave(&locked_pgdat->lru_lock, flags);
			}

			lruvec = mem_cgroup_page_lruvec(page, locked_pgdat);
			VM_BUG_ON_PAGE(!PageLRU(page), page);
			__ClearPageLRU(page);
			del_page_from_lru_list(page, lruvec, page_off_lru(page));
		}

		/* Clear Active bit in case of parallel mark_page_accessed */
		__ClearPageActive(page);
=======
			struct lruvec *prev_lruvec = lruvec;

			lruvec = relock_page_lruvec_irqsave(page, lruvec,
									&flags);
			if (prev_lruvec != lruvec)
				lock_batch = 0;

			del_page_from_lru_list(page, lruvec);
			__clear_page_lru_flags(page);
		}

>>>>>>> upstream/android-13
		__ClearPageWaiters(page);

		list_add(&page->lru, &pages_to_free);
	}
<<<<<<< HEAD
	if (locked_pgdat)
		spin_unlock_irqrestore(&locked_pgdat->lru_lock, flags);
=======
	if (lruvec)
		unlock_page_lruvec_irqrestore(lruvec, flags);
>>>>>>> upstream/android-13

	mem_cgroup_uncharge_list(&pages_to_free);
	free_unref_page_list(&pages_to_free);
}
EXPORT_SYMBOL(release_pages);

/*
 * The pages which we're about to release may be in the deferred lru-addition
 * queues.  That would prevent them from really being freed right now.  That's
 * OK from a correctness point of view but is inefficient - those pages may be
 * cache-warm and we want to give them back to the page allocator ASAP.
 *
 * So __pagevec_release() will drain those queues here.  __pagevec_lru_add()
 * and __pagevec_lru_add_active() call release_pages() directly to avoid
 * mutual recursion.
 */
void __pagevec_release(struct pagevec *pvec)
{
	if (!pvec->percpu_pvec_drained) {
		lru_add_drain();
		pvec->percpu_pvec_drained = true;
	}
	release_pages(pvec->pages, pagevec_count(pvec));
	pagevec_reinit(pvec);
}
EXPORT_SYMBOL(__pagevec_release);

<<<<<<< HEAD
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
/* used by __split_huge_page_refcount() */
void lru_add_page_tail(struct page *page, struct page *page_tail,
		       struct lruvec *lruvec, struct list_head *list)
{
	const int file = 0;

	VM_BUG_ON_PAGE(!PageHead(page), page);
	VM_BUG_ON_PAGE(PageCompound(page_tail), page);
	VM_BUG_ON_PAGE(PageLRU(page_tail), page);
	VM_BUG_ON(NR_CPUS != 1 &&
		  !spin_is_locked(&lruvec_pgdat(lruvec)->lru_lock));

	if (!list)
		SetPageLRU(page_tail);

	if (likely(PageLRU(page)))
		list_add_tail(&page_tail->lru, &page->lru);
	else if (list) {
		/* page reclaim is reclaiming a huge page */
		get_page(page_tail);
		list_add_tail(&page_tail->lru, list);
	} else {
		struct list_head *list_head;
		/*
		 * Head page has not yet been counted, as an hpage,
		 * so we must account for each subpage individually.
		 *
		 * Use the standard add function to put page_tail on the list,
		 * but then correct its position so they all end up in order.
		 */
		add_page_to_lru_list(page_tail, lruvec, page_lru(page_tail));
		list_head = page_tail->lru.prev;
		list_move_tail(&page_tail->lru, list_head);
	}

	if (!PageUnevictable(page))
		update_page_reclaim_stat(lruvec, file, PageActive(page_tail));
}
#endif /* CONFIG_TRANSPARENT_HUGEPAGE */

static void __pagevec_lru_add_fn(struct page *page, struct lruvec *lruvec,
				 void *arg)
{
	enum lru_list lru;
	int was_unevictable = TestClearPageUnevictable(page);

	VM_BUG_ON_PAGE(PageLRU(page), page);

	SetPageLRU(page);
	/*
	 * Page becomes evictable in two ways:
	 * 1) Within LRU lock [munlock_vma_pages() and __munlock_pagevec()].
=======
static void __pagevec_lru_add_fn(struct page *page, struct lruvec *lruvec)
{
	int was_unevictable = TestClearPageUnevictable(page);
	int nr_pages = thp_nr_pages(page);

	VM_BUG_ON_PAGE(PageLRU(page), page);

	/*
	 * Page becomes evictable in two ways:
	 * 1) Within LRU lock [munlock_vma_page() and __munlock_pagevec()].
>>>>>>> upstream/android-13
	 * 2) Before acquiring LRU lock to put the page to correct LRU and then
	 *   a) do PageLRU check with lock [check_move_unevictable_pages]
	 *   b) do PageLRU check before lock [clear_page_mlock]
	 *
	 * (1) & (2a) are ok as LRU lock will serialize them. For (2b), we need
	 * following strict ordering:
	 *
	 * #0: __pagevec_lru_add_fn		#1: clear_page_mlock
	 *
	 * SetPageLRU()				TestClearPageMlocked()
	 * smp_mb() // explicit ordering	// above provides strict
	 *					// ordering
	 * PageMlocked()			PageLRU()
	 *
	 *
	 * if '#1' does not observe setting of PG_lru by '#0' and fails
	 * isolation, the explicit barrier will make sure that page_evictable
	 * check will put the page in correct LRU. Without smp_mb(), SetPageLRU
	 * can be reordered after PageMlocked check and can make '#1' to fail
	 * the isolation of the page whose Mlocked bit is cleared (#0 is also
	 * looking at the same page) and the evictable page will be stranded
	 * in an unevictable LRU.
	 */
<<<<<<< HEAD
	smp_mb();

	if (page_evictable(page)) {
		lru = page_lru(page);
		update_page_reclaim_stat(lruvec, page_is_file_cache(page),
					 PageActive(page));
		if (was_unevictable)
			count_vm_event(UNEVICTABLE_PGRESCUED);
	} else {
		lru = LRU_UNEVICTABLE;
		ClearPageActive(page);
		SetPageUnevictable(page);
		if (!was_unevictable)
			count_vm_event(UNEVICTABLE_PGCULLED);
	}

	add_page_to_lru_list(page, lruvec, lru);
	trace_mm_lru_insertion(page, lru);
=======
	SetPageLRU(page);
	smp_mb__after_atomic();

	if (page_evictable(page)) {
		if (was_unevictable)
			__count_vm_events(UNEVICTABLE_PGRESCUED, nr_pages);
	} else {
		ClearPageActive(page);
		SetPageUnevictable(page);
		if (!was_unevictable)
			__count_vm_events(UNEVICTABLE_PGCULLED, nr_pages);
	}

	add_page_to_lru_list(page, lruvec);
	trace_mm_lru_insertion(page);
>>>>>>> upstream/android-13
}

/*
 * Add the passed pages to the LRU, then drop the caller's refcount
 * on them.  Reinitialises the caller's pagevec.
 */
void __pagevec_lru_add(struct pagevec *pvec)
{
<<<<<<< HEAD
	pagevec_lru_move_fn(pvec, __pagevec_lru_add_fn, NULL);
}
EXPORT_SYMBOL(__pagevec_lru_add);

/**
 * pagevec_lookup_entries - gang pagecache lookup
 * @pvec:	Where the resulting entries are placed
 * @mapping:	The address_space to search
 * @start:	The starting entry index
 * @nr_entries:	The maximum number of pages
 * @indices:	The cache indices corresponding to the entries in @pvec
 *
 * pagevec_lookup_entries() will search for and return a group of up
 * to @nr_pages pages and shadow entries in the mapping.  All
 * entries are placed in @pvec.  pagevec_lookup_entries() takes a
 * reference against actual pages in @pvec.
 *
 * The search returns a group of mapping-contiguous entries with
 * ascending indexes.  There may be holes in the indices due to
 * not-present entries.
 *
 * pagevec_lookup_entries() returns the number of entries which were
 * found.
 */
unsigned pagevec_lookup_entries(struct pagevec *pvec,
				struct address_space *mapping,
				pgoff_t start, unsigned nr_entries,
				pgoff_t *indices)
{
	pvec->nr = find_get_entries(mapping, start, nr_entries,
				    pvec->pages, indices);
	return pagevec_count(pvec);
=======
	int i;
	struct lruvec *lruvec = NULL;
	unsigned long flags = 0;

	for (i = 0; i < pagevec_count(pvec); i++) {
		struct page *page = pvec->pages[i];

		lruvec = relock_page_lruvec_irqsave(page, lruvec, &flags);
		__pagevec_lru_add_fn(page, lruvec);
	}
	if (lruvec)
		unlock_page_lruvec_irqrestore(lruvec, flags);
	release_pages(pvec->pages, pvec->nr);
	pagevec_reinit(pvec);
>>>>>>> upstream/android-13
}

/**
 * pagevec_remove_exceptionals - pagevec exceptionals pruning
 * @pvec:	The pagevec to prune
 *
<<<<<<< HEAD
 * pagevec_lookup_entries() fills both pages and exceptional radix
 * tree entries into the pagevec.  This function prunes all
=======
 * find_get_entries() fills both pages and XArray value entries (aka
 * exceptional entries) into the pagevec.  This function prunes all
>>>>>>> upstream/android-13
 * exceptionals from @pvec without leaving holes, so that it can be
 * passed on to page-only pagevec operations.
 */
void pagevec_remove_exceptionals(struct pagevec *pvec)
{
	int i, j;

	for (i = 0, j = 0; i < pagevec_count(pvec); i++) {
		struct page *page = pvec->pages[i];
<<<<<<< HEAD
		if (!radix_tree_exceptional_entry(page))
=======
		if (!xa_is_value(page))
>>>>>>> upstream/android-13
			pvec->pages[j++] = page;
	}
	pvec->nr = j;
}

/**
 * pagevec_lookup_range - gang pagecache lookup
 * @pvec:	Where the resulting pages are placed
 * @mapping:	The address_space to search
 * @start:	The starting page index
 * @end:	The final page index
 *
 * pagevec_lookup_range() will search for & return a group of up to PAGEVEC_SIZE
 * pages in the mapping starting from index @start and upto index @end
 * (inclusive).  The pages are placed in @pvec.  pagevec_lookup() takes a
 * reference against the pages in @pvec.
 *
 * The search returns a group of mapping-contiguous pages with ascending
 * indexes.  There may be holes in the indices due to not-present pages. We
 * also update @start to index the next page for the traversal.
 *
 * pagevec_lookup_range() returns the number of pages which were found. If this
 * number is smaller than PAGEVEC_SIZE, the end of specified range has been
 * reached.
 */
unsigned pagevec_lookup_range(struct pagevec *pvec,
		struct address_space *mapping, pgoff_t *start, pgoff_t end)
{
	pvec->nr = find_get_pages_range(mapping, start, end, PAGEVEC_SIZE,
					pvec->pages);
	return pagevec_count(pvec);
}
EXPORT_SYMBOL(pagevec_lookup_range);

unsigned pagevec_lookup_range_tag(struct pagevec *pvec,
		struct address_space *mapping, pgoff_t *index, pgoff_t end,
<<<<<<< HEAD
		int tag)
=======
		xa_mark_t tag)
>>>>>>> upstream/android-13
{
	pvec->nr = find_get_pages_range_tag(mapping, index, end, tag,
					PAGEVEC_SIZE, pvec->pages);
	return pagevec_count(pvec);
}
EXPORT_SYMBOL(pagevec_lookup_range_tag);

<<<<<<< HEAD
unsigned pagevec_lookup_range_nr_tag(struct pagevec *pvec,
		struct address_space *mapping, pgoff_t *index, pgoff_t end,
		int tag, unsigned max_pages)
{
	pvec->nr = find_get_pages_range_tag(mapping, index, end, tag,
		min_t(unsigned int, max_pages, PAGEVEC_SIZE), pvec->pages);
	return pagevec_count(pvec);
}
EXPORT_SYMBOL(pagevec_lookup_range_nr_tag);
=======
>>>>>>> upstream/android-13
/*
 * Perform any setup for the swap system
 */
void __init swap_setup(void)
{
<<<<<<< HEAD
	unsigned long megs = totalram_pages >> (20 - PAGE_SHIFT);
=======
	unsigned long megs = totalram_pages() >> (20 - PAGE_SHIFT);
>>>>>>> upstream/android-13

	/* Use a smaller cluster for small-memory machines */
	if (megs < 16)
		page_cluster = 2;
	else
		page_cluster = 3;
	/*
	 * Right now other parts of the system means that we
	 * _really_ don't want to cluster much more
	 */
}
<<<<<<< HEAD
=======

#ifdef CONFIG_DEV_PAGEMAP_OPS
void put_devmap_managed_page(struct page *page)
{
	int count;

	if (WARN_ON_ONCE(!page_is_devmap_managed(page)))
		return;

	count = page_ref_dec_return(page);

	/*
	 * devmap page refcounts are 1-based, rather than 0-based: if
	 * refcount is 1, then the page is free and the refcount is
	 * stable because nobody holds a reference on the page.
	 */
	if (count == 1)
		free_devmap_managed_page(page);
	else if (!count)
		__put_page(page);
}
EXPORT_SYMBOL(put_devmap_managed_page);
#endif
>>>>>>> upstream/android-13
