<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * fs/dax.c - Direct Access filesystem code
 * Copyright (c) 2013-2014 Intel Corporation
 * Author: Matthew Wilcox <matthew.r.wilcox@intel.com>
 * Author: Ross Zwisler <ross.zwisler@linux.intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/atomic.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>
#include <linux/dax.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/highmem.h>
#include <linux/memcontrol.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/pagevec.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/uio.h>
#include <linux/vmstat.h>
#include <linux/pfn_t.h>
#include <linux/sizes.h>
#include <linux/mmu_notifier.h>
#include <linux/iomap.h>
<<<<<<< HEAD
#include "internal.h"
=======
#include <asm/pgalloc.h>
>>>>>>> upstream/android-13

#define CREATE_TRACE_POINTS
#include <trace/events/fs_dax.h>

<<<<<<< HEAD
=======
static inline unsigned int pe_order(enum page_entry_size pe_size)
{
	if (pe_size == PE_SIZE_PTE)
		return PAGE_SHIFT - PAGE_SHIFT;
	if (pe_size == PE_SIZE_PMD)
		return PMD_SHIFT - PAGE_SHIFT;
	if (pe_size == PE_SIZE_PUD)
		return PUD_SHIFT - PAGE_SHIFT;
	return ~0;
}

>>>>>>> upstream/android-13
/* We choose 4096 entries - same as per-zone page wait tables */
#define DAX_WAIT_TABLE_BITS 12
#define DAX_WAIT_TABLE_ENTRIES (1 << DAX_WAIT_TABLE_BITS)

/* The 'colour' (ie low bits) within a PMD of a page offset.  */
#define PG_PMD_COLOUR	((PMD_SIZE >> PAGE_SHIFT) - 1)
#define PG_PMD_NR	(PMD_SIZE >> PAGE_SHIFT)

<<<<<<< HEAD
=======
/* The order of a PMD entry */
#define PMD_ORDER	(PMD_SHIFT - PAGE_SHIFT)

>>>>>>> upstream/android-13
static wait_queue_head_t wait_table[DAX_WAIT_TABLE_ENTRIES];

static int __init init_dax_wait_table(void)
{
	int i;

	for (i = 0; i < DAX_WAIT_TABLE_ENTRIES; i++)
		init_waitqueue_head(wait_table + i);
	return 0;
}
fs_initcall(init_dax_wait_table);

/*
<<<<<<< HEAD
 * We use lowest available bit in exceptional entry for locking, one bit for
 * the entry size (PMD) and two more to tell us if the entry is a zero page or
 * an empty entry that is just used for locking.  In total four special bits.
=======
 * DAX pagecache entries use XArray value entries so they can't be mistaken
 * for pages.  We use one bit for locking, one bit for the entry size (PMD)
 * and two more to tell us if the entry is a zero page or an empty entry that
 * is just used for locking.  In total four special bits.
>>>>>>> upstream/android-13
 *
 * If the PMD bit isn't set the entry has size PAGE_SIZE, and if the ZERO_PAGE
 * and EMPTY bits aren't set the entry is a normal DAX entry with a filesystem
 * block allocation.
 */
<<<<<<< HEAD
#define RADIX_DAX_SHIFT		(RADIX_TREE_EXCEPTIONAL_SHIFT + 4)
#define RADIX_DAX_ENTRY_LOCK	(1 << RADIX_TREE_EXCEPTIONAL_SHIFT)
#define RADIX_DAX_PMD		(1 << (RADIX_TREE_EXCEPTIONAL_SHIFT + 1))
#define RADIX_DAX_ZERO_PAGE	(1 << (RADIX_TREE_EXCEPTIONAL_SHIFT + 2))
#define RADIX_DAX_EMPTY		(1 << (RADIX_TREE_EXCEPTIONAL_SHIFT + 3))

static unsigned long dax_radix_pfn(void *entry)
{
	return (unsigned long)entry >> RADIX_DAX_SHIFT;
}

static void *dax_radix_locked_entry(unsigned long pfn, unsigned long flags)
{
	return (void *)(RADIX_TREE_EXCEPTIONAL_ENTRY | flags |
			(pfn << RADIX_DAX_SHIFT) | RADIX_DAX_ENTRY_LOCK);
}

static unsigned int dax_radix_order(void *entry)
{
	if ((unsigned long)entry & RADIX_DAX_PMD)
		return PMD_SHIFT - PAGE_SHIFT;
	return 0;
}

static int dax_is_pmd_entry(void *entry)
{
	return (unsigned long)entry & RADIX_DAX_PMD;
}

static int dax_is_pte_entry(void *entry)
{
	return !((unsigned long)entry & RADIX_DAX_PMD);
=======
#define DAX_SHIFT	(4)
#define DAX_LOCKED	(1UL << 0)
#define DAX_PMD		(1UL << 1)
#define DAX_ZERO_PAGE	(1UL << 2)
#define DAX_EMPTY	(1UL << 3)

static unsigned long dax_to_pfn(void *entry)
{
	return xa_to_value(entry) >> DAX_SHIFT;
}

static void *dax_make_entry(pfn_t pfn, unsigned long flags)
{
	return xa_mk_value(flags | (pfn_t_to_pfn(pfn) << DAX_SHIFT));
}

static bool dax_is_locked(void *entry)
{
	return xa_to_value(entry) & DAX_LOCKED;
}

static unsigned int dax_entry_order(void *entry)
{
	if (xa_to_value(entry) & DAX_PMD)
		return PMD_ORDER;
	return 0;
}

static unsigned long dax_is_pmd_entry(void *entry)
{
	return xa_to_value(entry) & DAX_PMD;
}

static bool dax_is_pte_entry(void *entry)
{
	return !(xa_to_value(entry) & DAX_PMD);
>>>>>>> upstream/android-13
}

static int dax_is_zero_entry(void *entry)
{
<<<<<<< HEAD
	return (unsigned long)entry & RADIX_DAX_ZERO_PAGE;
=======
	return xa_to_value(entry) & DAX_ZERO_PAGE;
>>>>>>> upstream/android-13
}

static int dax_is_empty_entry(void *entry)
{
<<<<<<< HEAD
	return (unsigned long)entry & RADIX_DAX_EMPTY;
}

/*
 * DAX radix tree locking
 */
struct exceptional_entry_key {
	struct address_space *mapping;
=======
	return xa_to_value(entry) & DAX_EMPTY;
}

/*
 * true if the entry that was found is of a smaller order than the entry
 * we were looking for
 */
static bool dax_is_conflict(void *entry)
{
	return entry == XA_RETRY_ENTRY;
}

/*
 * DAX page cache entry locking
 */
struct exceptional_entry_key {
	struct xarray *xa;
>>>>>>> upstream/android-13
	pgoff_t entry_start;
};

struct wait_exceptional_entry_queue {
	wait_queue_entry_t wait;
	struct exceptional_entry_key key;
};

<<<<<<< HEAD
static wait_queue_head_t *dax_entry_waitqueue(struct address_space *mapping,
		pgoff_t index, void *entry, struct exceptional_entry_key *key)
{
	unsigned long hash;
=======
/**
 * enum dax_wake_mode: waitqueue wakeup behaviour
 * @WAKE_ALL: wake all waiters in the waitqueue
 * @WAKE_NEXT: wake only the first waiter in the waitqueue
 */
enum dax_wake_mode {
	WAKE_ALL,
	WAKE_NEXT,
};

static wait_queue_head_t *dax_entry_waitqueue(struct xa_state *xas,
		void *entry, struct exceptional_entry_key *key)
{
	unsigned long hash;
	unsigned long index = xas->xa_index;
>>>>>>> upstream/android-13

	/*
	 * If 'entry' is a PMD, align the 'index' that we use for the wait
	 * queue to the start of that PMD.  This ensures that all offsets in
	 * the range covered by the PMD map to the same bit lock.
	 */
	if (dax_is_pmd_entry(entry))
		index &= ~PG_PMD_COLOUR;
<<<<<<< HEAD

	key->mapping = mapping;
	key->entry_start = index;

	hash = hash_long((unsigned long)mapping ^ index, DAX_WAIT_TABLE_BITS);
	return wait_table + hash;
}

static int wake_exceptional_entry_func(wait_queue_entry_t *wait, unsigned int mode,
				       int sync, void *keyp)
=======
	key->xa = xas->xa;
	key->entry_start = index;

	hash = hash_long((unsigned long)xas->xa ^ index, DAX_WAIT_TABLE_BITS);
	return wait_table + hash;
}

static int wake_exceptional_entry_func(wait_queue_entry_t *wait,
		unsigned int mode, int sync, void *keyp)
>>>>>>> upstream/android-13
{
	struct exceptional_entry_key *key = keyp;
	struct wait_exceptional_entry_queue *ewait =
		container_of(wait, struct wait_exceptional_entry_queue, wait);

<<<<<<< HEAD
	if (key->mapping != ewait->key.mapping ||
=======
	if (key->xa != ewait->key.xa ||
>>>>>>> upstream/android-13
	    key->entry_start != ewait->key.entry_start)
		return 0;
	return autoremove_wake_function(wait, mode, sync, NULL);
}

/*
 * @entry may no longer be the entry at the index in the mapping.
 * The important information it's conveying is whether the entry at
 * this index used to be a PMD entry.
 */
<<<<<<< HEAD
static void dax_wake_mapping_entry_waiter(struct address_space *mapping,
		pgoff_t index, void *entry, bool wake_all)
=======
static void dax_wake_entry(struct xa_state *xas, void *entry,
			   enum dax_wake_mode mode)
>>>>>>> upstream/android-13
{
	struct exceptional_entry_key key;
	wait_queue_head_t *wq;

<<<<<<< HEAD
	wq = dax_entry_waitqueue(mapping, index, entry, &key);
=======
	wq = dax_entry_waitqueue(xas, entry, &key);
>>>>>>> upstream/android-13

	/*
	 * Checking for locked entry and prepare_to_wait_exclusive() happens
	 * under the i_pages lock, ditto for entry handling in our callers.
	 * So at this point all tasks that could have seen our entry locked
	 * must be in the waitqueue and the following check will see them.
	 */
	if (waitqueue_active(wq))
<<<<<<< HEAD
		__wake_up(wq, TASK_NORMAL, wake_all ? 0 : 1, &key);
}

/*
 * Check whether the given slot is locked.  Must be called with the i_pages
 * lock held.
 */
static inline int slot_locked(struct address_space *mapping, void **slot)
{
	unsigned long entry = (unsigned long)
		radix_tree_deref_slot_protected(slot, &mapping->i_pages.xa_lock);
	return entry & RADIX_DAX_ENTRY_LOCK;
}

/*
 * Mark the given slot as locked.  Must be called with the i_pages lock held.
 */
static inline void *lock_slot(struct address_space *mapping, void **slot)
{
	unsigned long entry = (unsigned long)
		radix_tree_deref_slot_protected(slot, &mapping->i_pages.xa_lock);

	entry |= RADIX_DAX_ENTRY_LOCK;
	radix_tree_replace_slot(&mapping->i_pages, slot, (void *)entry);
	return (void *)entry;
}

/*
 * Mark the given slot as unlocked.  Must be called with the i_pages lock held.
 */
static inline void *unlock_slot(struct address_space *mapping, void **slot)
{
	unsigned long entry = (unsigned long)
		radix_tree_deref_slot_protected(slot, &mapping->i_pages.xa_lock);

	entry &= ~(unsigned long)RADIX_DAX_ENTRY_LOCK;
	radix_tree_replace_slot(&mapping->i_pages, slot, (void *)entry);
	return (void *)entry;
}

static void put_unlocked_mapping_entry(struct address_space *mapping,
				       pgoff_t index, void *entry);

/*
 * Lookup entry in radix tree, wait for it to become unlocked if it is
 * exceptional entry and return it. The caller must call
 * put_unlocked_mapping_entry() when he decided not to lock the entry or
 * put_locked_mapping_entry() when he locked the entry and now wants to
 * unlock it.
 *
 * Must be called with the i_pages lock held.
 */
static void *get_unlocked_mapping_entry(struct address_space *mapping,
		pgoff_t index, void ***slotp)
{
	void *entry, **slot;
=======
		__wake_up(wq, TASK_NORMAL, mode == WAKE_ALL ? 0 : 1, &key);
}

/*
 * Look up entry in page cache, wait for it to become unlocked if it
 * is a DAX entry and return it.  The caller must subsequently call
 * put_unlocked_entry() if it did not lock the entry or dax_unlock_entry()
 * if it did.  The entry returned may have a larger order than @order.
 * If @order is larger than the order of the entry found in i_pages, this
 * function returns a dax_is_conflict entry.
 *
 * Must be called with the i_pages lock held.
 */
static void *get_unlocked_entry(struct xa_state *xas, unsigned int order)
{
	void *entry;
>>>>>>> upstream/android-13
	struct wait_exceptional_entry_queue ewait;
	wait_queue_head_t *wq;

	init_wait(&ewait.wait);
	ewait.wait.func = wake_exceptional_entry_func;

	for (;;) {
<<<<<<< HEAD
		entry = __radix_tree_lookup(&mapping->i_pages, index, NULL,
					  &slot);
		if (!entry ||
		    WARN_ON_ONCE(!radix_tree_exceptional_entry(entry)) ||
		    !slot_locked(mapping, slot)) {
			if (slotp)
				*slotp = slot;
			return entry;
		}

		wq = dax_entry_waitqueue(mapping, index, entry, &ewait.key);
		prepare_to_wait_exclusive(wq, &ewait.wait,
					  TASK_UNINTERRUPTIBLE);
		xa_unlock_irq(&mapping->i_pages);
		schedule();
		finish_wait(wq, &ewait.wait);
		xa_lock_irq(&mapping->i_pages);
=======
		entry = xas_find_conflict(xas);
		if (!entry || WARN_ON_ONCE(!xa_is_value(entry)))
			return entry;
		if (dax_entry_order(entry) < order)
			return XA_RETRY_ENTRY;
		if (!dax_is_locked(entry))
			return entry;

		wq = dax_entry_waitqueue(xas, entry, &ewait.key);
		prepare_to_wait_exclusive(wq, &ewait.wait,
					  TASK_UNINTERRUPTIBLE);
		xas_unlock_irq(xas);
		xas_reset(xas);
		schedule();
		finish_wait(wq, &ewait.wait);
		xas_lock_irq(xas);
>>>>>>> upstream/android-13
	}
}

/*
 * The only thing keeping the address space around is the i_pages lock
 * (it's cycled in clear_inode() after removing the entries from i_pages)
 * After we call xas_unlock_irq(), we cannot touch xas->xa.
 */
<<<<<<< HEAD
static void wait_entry_unlocked(struct address_space *mapping, pgoff_t index,
		void ***slotp, void *entry)
=======
static void wait_entry_unlocked(struct xa_state *xas, void *entry)
>>>>>>> upstream/android-13
{
	struct wait_exceptional_entry_queue ewait;
	wait_queue_head_t *wq;

	init_wait(&ewait.wait);
	ewait.wait.func = wake_exceptional_entry_func;

<<<<<<< HEAD
	wq = dax_entry_waitqueue(mapping, index, entry, &ewait.key);
=======
	wq = dax_entry_waitqueue(xas, entry, &ewait.key);
>>>>>>> upstream/android-13
	/*
	 * Unlike get_unlocked_entry() there is no guarantee that this
	 * path ever successfully retrieves an unlocked entry before an
	 * inode dies. Perform a non-exclusive wait in case this path
	 * never successfully performs its own wake up.
	 */
	prepare_to_wait(wq, &ewait.wait, TASK_UNINTERRUPTIBLE);
<<<<<<< HEAD
	xa_unlock_irq(&mapping->i_pages);
=======
	xas_unlock_irq(xas);
>>>>>>> upstream/android-13
	schedule();
	finish_wait(wq, &ewait.wait);
}

<<<<<<< HEAD
static void unlock_mapping_entry(struct address_space *mapping, pgoff_t index)
{
	void *entry, **slot;

	xa_lock_irq(&mapping->i_pages);
	entry = __radix_tree_lookup(&mapping->i_pages, index, NULL, &slot);
	if (WARN_ON_ONCE(!entry || !radix_tree_exceptional_entry(entry) ||
			 !slot_locked(mapping, slot))) {
		xa_unlock_irq(&mapping->i_pages);
		return;
	}
	unlock_slot(mapping, slot);
	xa_unlock_irq(&mapping->i_pages);
	dax_wake_mapping_entry_waiter(mapping, index, entry, false);
}

static void put_locked_mapping_entry(struct address_space *mapping,
		pgoff_t index)
{
	unlock_mapping_entry(mapping, index);
}

/*
 * Called when we are done with radix tree entry we looked up via
 * get_unlocked_mapping_entry() and which we didn't lock in the end.
 */
static void put_unlocked_mapping_entry(struct address_space *mapping,
				       pgoff_t index, void *entry)
{
	if (!entry)
		return;

	/* We have to wake up next waiter for the radix tree entry lock */
	dax_wake_mapping_entry_waiter(mapping, index, entry, false);
=======
static void put_unlocked_entry(struct xa_state *xas, void *entry,
			       enum dax_wake_mode mode)
{
	if (entry && !dax_is_conflict(entry))
		dax_wake_entry(xas, entry, mode);
}

/*
 * We used the xa_state to get the entry, but then we locked the entry and
 * dropped the xa_lock, so we know the xa_state is stale and must be reset
 * before use.
 */
static void dax_unlock_entry(struct xa_state *xas, void *entry)
{
	void *old;

	BUG_ON(dax_is_locked(entry));
	xas_reset(xas);
	xas_lock_irq(xas);
	old = xas_store(xas, entry);
	xas_unlock_irq(xas);
	BUG_ON(!dax_is_locked(old));
	dax_wake_entry(xas, entry, WAKE_NEXT);
}

/*
 * Return: The entry stored at this location before it was locked.
 */
static void *dax_lock_entry(struct xa_state *xas, void *entry)
{
	unsigned long v = xa_to_value(entry);
	return xas_store(xas, xa_mk_value(v | DAX_LOCKED));
>>>>>>> upstream/android-13
}

static unsigned long dax_entry_size(void *entry)
{
	if (dax_is_zero_entry(entry))
		return 0;
	else if (dax_is_empty_entry(entry))
		return 0;
	else if (dax_is_pmd_entry(entry))
		return PMD_SIZE;
	else
		return PAGE_SIZE;
}

<<<<<<< HEAD
static unsigned long dax_radix_end_pfn(void *entry)
{
	return dax_radix_pfn(entry) + dax_entry_size(entry) / PAGE_SIZE;
=======
static unsigned long dax_end_pfn(void *entry)
{
	return dax_to_pfn(entry) + dax_entry_size(entry) / PAGE_SIZE;
>>>>>>> upstream/android-13
}

/*
 * Iterate through all mapped pfns represented by an entry, i.e. skip
 * 'empty' and 'zero' entries.
 */
#define for_each_mapped_pfn(entry, pfn) \
<<<<<<< HEAD
	for (pfn = dax_radix_pfn(entry); \
			pfn < dax_radix_end_pfn(entry); pfn++)
=======
	for (pfn = dax_to_pfn(entry); \
			pfn < dax_end_pfn(entry); pfn++)
>>>>>>> upstream/android-13

/*
 * TODO: for reflink+dax we need a way to associate a single page with
 * multiple address_space instances at different linear_page_index()
 * offsets.
 */
static void dax_associate_entry(void *entry, struct address_space *mapping,
		struct vm_area_struct *vma, unsigned long address)
{
	unsigned long size = dax_entry_size(entry), pfn, index;
	int i = 0;

	if (IS_ENABLED(CONFIG_FS_DAX_LIMITED))
		return;

	index = linear_page_index(vma, address & ~(size - 1));
	for_each_mapped_pfn(entry, pfn) {
		struct page *page = pfn_to_page(pfn);

		WARN_ON_ONCE(page->mapping);
		page->mapping = mapping;
		page->index = index + i++;
	}
}

static void dax_disassociate_entry(void *entry, struct address_space *mapping,
		bool trunc)
{
	unsigned long pfn;

	if (IS_ENABLED(CONFIG_FS_DAX_LIMITED))
		return;

	for_each_mapped_pfn(entry, pfn) {
		struct page *page = pfn_to_page(pfn);

		WARN_ON_ONCE(trunc && page_ref_count(page) > 1);
		WARN_ON_ONCE(page->mapping && page->mapping != mapping);
		page->mapping = NULL;
		page->index = 0;
	}
}

static struct page *dax_busy_page(void *entry)
{
	unsigned long pfn;

	for_each_mapped_pfn(entry, pfn) {
		struct page *page = pfn_to_page(pfn);

		if (page_ref_count(page) > 1)
			return page;
	}
	return NULL;
}

<<<<<<< HEAD
bool dax_lock_mapping_entry(struct page *page)
{
	pgoff_t index;
	struct inode *inode;
	bool did_lock = false;
	void *entry = NULL, **slot;
	struct address_space *mapping;

	rcu_read_lock();
	for (;;) {
		mapping = READ_ONCE(page->mapping);

=======
/*
 * dax_lock_mapping_entry - Lock the DAX entry corresponding to a page
 * @page: The page whose entry we want to lock
 *
 * Context: Process context.
 * Return: A cookie to pass to dax_unlock_page() or 0 if the entry could
 * not be locked.
 */
dax_entry_t dax_lock_page(struct page *page)
{
	XA_STATE(xas, NULL, 0);
	void *entry;

	/* Ensure page->mapping isn't freed while we look at it */
	rcu_read_lock();
	for (;;) {
		struct address_space *mapping = READ_ONCE(page->mapping);

		entry = NULL;
>>>>>>> upstream/android-13
		if (!mapping || !dax_mapping(mapping))
			break;

		/*
		 * In the device-dax case there's no need to lock, a
		 * struct dev_pagemap pin is sufficient to keep the
		 * inode alive, and we assume we have dev_pagemap pin
		 * otherwise we would not have a valid pfn_to_page()
		 * translation.
		 */
<<<<<<< HEAD
		inode = mapping->host;
		if (S_ISCHR(inode->i_mode)) {
			did_lock = true;
			break;
		}

		xa_lock_irq(&mapping->i_pages);
		if (mapping != page->mapping) {
			xa_unlock_irq(&mapping->i_pages);
			continue;
		}
		index = page->index;

		entry = __radix_tree_lookup(&mapping->i_pages, index,
						NULL, &slot);
		if (!entry) {
			xa_unlock_irq(&mapping->i_pages);
			break;
		} else if (slot_locked(mapping, slot)) {
			rcu_read_unlock();
			wait_entry_unlocked(mapping, index, &slot, entry);
			rcu_read_lock();
			continue;
		}
		lock_slot(mapping, slot);
		did_lock = true;
		xa_unlock_irq(&mapping->i_pages);
		break;
	}
	rcu_read_unlock();

	return did_lock;
}

void dax_unlock_mapping_entry(struct page *page)
{
	struct address_space *mapping = page->mapping;
	struct inode *inode = mapping->host;

	if (S_ISCHR(inode->i_mode))
		return;

	unlock_mapping_entry(mapping, page->index);
}

/*
 * Find radix tree entry at given index. If it points to an exceptional entry,
 * return it with the radix tree entry locked. If the radix tree doesn't
 * contain given index, create an empty exceptional entry for the index and
 * return with it locked.
 *
 * When requesting an entry with size RADIX_DAX_PMD, grab_mapping_entry() will
 * either return that locked entry or will return an error.  This error will
 * happen if there are any 4k entries within the 2MiB range that we are
 * requesting.
 *
 * We always favor 4k entries over 2MiB entries. There isn't a flow where we
 * evict 4k entries in order to 'upgrade' them to a 2MiB entry.  A 2MiB
 * insertion will fail if it finds any 4k entries already in the tree, and a
 * 4k insertion will cause an existing 2MiB entry to be unmapped and
 * downgraded to 4k entries.  This happens for both 2MiB huge zero pages as
 * well as 2MiB empty entries.
 *
 * The exception to this downgrade path is for 2MiB DAX PMD entries that have
 * real storage backing them.  We will leave these real 2MiB DAX entries in
 * the tree, and PTE writes will simply dirty the entire 2MiB DAX entry.
=======
		entry = (void *)~0UL;
		if (S_ISCHR(mapping->host->i_mode))
			break;

		xas.xa = &mapping->i_pages;
		xas_lock_irq(&xas);
		if (mapping != page->mapping) {
			xas_unlock_irq(&xas);
			continue;
		}
		xas_set(&xas, page->index);
		entry = xas_load(&xas);
		if (dax_is_locked(entry)) {
			rcu_read_unlock();
			wait_entry_unlocked(&xas, entry);
			rcu_read_lock();
			continue;
		}
		dax_lock_entry(&xas, entry);
		xas_unlock_irq(&xas);
		break;
	}
	rcu_read_unlock();
	return (dax_entry_t)entry;
}

void dax_unlock_page(struct page *page, dax_entry_t cookie)
{
	struct address_space *mapping = page->mapping;
	XA_STATE(xas, &mapping->i_pages, page->index);

	if (S_ISCHR(mapping->host->i_mode))
		return;

	dax_unlock_entry(&xas, (void *)cookie);
}

/*
 * Find page cache entry at given index. If it is a DAX entry, return it
 * with the entry locked. If the page cache doesn't contain an entry at
 * that index, add a locked empty entry.
 *
 * When requesting an entry with size DAX_PMD, grab_mapping_entry() will
 * either return that locked entry or will return VM_FAULT_FALLBACK.
 * This will happen if there are any PTE entries within the PMD range
 * that we are requesting.
 *
 * We always favor PTE entries over PMD entries. There isn't a flow where we
 * evict PTE entries in order to 'upgrade' them to a PMD entry.  A PMD
 * insertion will fail if it finds any PTE entries already in the tree, and a
 * PTE insertion will cause an existing PMD entry to be unmapped and
 * downgraded to PTE entries.  This happens for both PMD zero pages as
 * well as PMD empty entries.
 *
 * The exception to this downgrade path is for PMD entries that have
 * real storage backing them.  We will leave these real PMD entries in
 * the tree, and PTE writes will simply dirty the entire PMD entry.
>>>>>>> upstream/android-13
 *
 * Note: Unlike filemap_fault() we don't honor FAULT_FLAG_RETRY flags. For
 * persistent memory the benefit is doubtful. We can add that later if we can
 * show it helps.
<<<<<<< HEAD
 */
static void *grab_mapping_entry(struct address_space *mapping, pgoff_t index,
		unsigned long size_flag)
{
	bool pmd_downgrade = false; /* splitting 2MiB entry into 4k entries? */
	void *entry, **slot;

restart:
	xa_lock_irq(&mapping->i_pages);
	entry = get_unlocked_mapping_entry(mapping, index, &slot);

	if (WARN_ON_ONCE(entry && !radix_tree_exceptional_entry(entry))) {
		entry = ERR_PTR(-EIO);
		goto out_unlock;
	}

	if (entry) {
		if (size_flag & RADIX_DAX_PMD) {
			if (dax_is_pte_entry(entry)) {
				put_unlocked_mapping_entry(mapping, index,
						entry);
				entry = ERR_PTR(-EEXIST);
				goto out_unlock;
			}
		} else { /* trying to grab a PTE entry */
=======
 *
 * On error, this function does not return an ERR_PTR.  Instead it returns
 * a VM_FAULT code, encoded as an xarray internal entry.  The ERR_PTR values
 * overlap with xarray value entries.
 */
static void *grab_mapping_entry(struct xa_state *xas,
		struct address_space *mapping, unsigned int order)
{
	unsigned long index = xas->xa_index;
	bool pmd_downgrade;	/* splitting PMD entry into PTE entries? */
	void *entry;

retry:
	pmd_downgrade = false;
	xas_lock_irq(xas);
	entry = get_unlocked_entry(xas, order);

	if (entry) {
		if (dax_is_conflict(entry))
			goto fallback;
		if (!xa_is_value(entry)) {
			xas_set_err(xas, -EIO);
			goto out_unlock;
		}

		if (order == 0) {
>>>>>>> upstream/android-13
			if (dax_is_pmd_entry(entry) &&
			    (dax_is_zero_entry(entry) ||
			     dax_is_empty_entry(entry))) {
				pmd_downgrade = true;
			}
		}
	}

<<<<<<< HEAD
	/* No entry for given index? Make sure radix tree is big enough. */
	if (!entry || pmd_downgrade) {
		int err;

		if (pmd_downgrade) {
			/*
			 * Make sure 'entry' remains valid while we drop
			 * the i_pages lock.
			 */
			entry = lock_slot(mapping, slot);
		}

		xa_unlock_irq(&mapping->i_pages);
=======
	if (pmd_downgrade) {
		/*
		 * Make sure 'entry' remains valid while we drop
		 * the i_pages lock.
		 */
		dax_lock_entry(xas, entry);

>>>>>>> upstream/android-13
		/*
		 * Besides huge zero pages the only other thing that gets
		 * downgraded are empty entries which don't need to be
		 * unmapped.
		 */
<<<<<<< HEAD
		if (pmd_downgrade && dax_is_zero_entry(entry))
			unmap_mapping_pages(mapping, index & ~PG_PMD_COLOUR,
							PG_PMD_NR, false);

		err = radix_tree_preload(
				mapping_gfp_mask(mapping) & ~__GFP_HIGHMEM);
		if (err) {
			if (pmd_downgrade)
				put_locked_mapping_entry(mapping, index);
			return ERR_PTR(err);
		}
		xa_lock_irq(&mapping->i_pages);

		if (!entry) {
			/*
			 * We needed to drop the i_pages lock while calling
			 * radix_tree_preload() and we didn't have an entry to
			 * lock.  See if another thread inserted an entry at
			 * our index during this time.
			 */
			entry = __radix_tree_lookup(&mapping->i_pages, index,
					NULL, &slot);
			if (entry) {
				radix_tree_preload_end();
				xa_unlock_irq(&mapping->i_pages);
				goto restart;
			}
		}

		if (pmd_downgrade) {
			dax_disassociate_entry(entry, mapping, false);
			radix_tree_delete(&mapping->i_pages, index);
			mapping->nrexceptional--;
			dax_wake_mapping_entry_waiter(mapping, index, entry,
					true);
		}

		entry = dax_radix_locked_entry(0, size_flag | RADIX_DAX_EMPTY);

		err = __radix_tree_insert(&mapping->i_pages, index,
				dax_radix_order(entry), entry);
		radix_tree_preload_end();
		if (err) {
			xa_unlock_irq(&mapping->i_pages);
			/*
			 * Our insertion of a DAX entry failed, most likely
			 * because we were inserting a PMD entry and it
			 * collided with a PTE sized entry at a different
			 * index in the PMD range.  We haven't inserted
			 * anything into the radix tree and have no waiters to
			 * wake.
			 */
			return ERR_PTR(err);
		}
		/* Good, we have inserted empty locked entry into the tree. */
		mapping->nrexceptional++;
		xa_unlock_irq(&mapping->i_pages);
		return entry;
	}
	entry = lock_slot(mapping, slot);
 out_unlock:
	xa_unlock_irq(&mapping->i_pages);
	return entry;
}

/**
 * dax_layout_busy_page - find first pinned page in @mapping
 * @mapping: address space to scan for a page with ref count > 1
=======
		if (dax_is_zero_entry(entry)) {
			xas_unlock_irq(xas);
			unmap_mapping_pages(mapping,
					xas->xa_index & ~PG_PMD_COLOUR,
					PG_PMD_NR, false);
			xas_reset(xas);
			xas_lock_irq(xas);
		}

		dax_disassociate_entry(entry, mapping, false);
		xas_store(xas, NULL);	/* undo the PMD join */
		dax_wake_entry(xas, entry, WAKE_ALL);
		mapping->nrpages -= PG_PMD_NR;
		entry = NULL;
		xas_set(xas, index);
	}

	if (entry) {
		dax_lock_entry(xas, entry);
	} else {
		unsigned long flags = DAX_EMPTY;

		if (order > 0)
			flags |= DAX_PMD;
		entry = dax_make_entry(pfn_to_pfn_t(0), flags);
		dax_lock_entry(xas, entry);
		if (xas_error(xas))
			goto out_unlock;
		mapping->nrpages += 1UL << order;
	}

out_unlock:
	xas_unlock_irq(xas);
	if (xas_nomem(xas, mapping_gfp_mask(mapping) & ~__GFP_HIGHMEM))
		goto retry;
	if (xas->xa_node == XA_ERROR(-ENOMEM))
		return xa_mk_internal(VM_FAULT_OOM);
	if (xas_error(xas))
		return xa_mk_internal(VM_FAULT_SIGBUS);
	return entry;
fallback:
	xas_unlock_irq(xas);
	return xa_mk_internal(VM_FAULT_FALLBACK);
}

/**
 * dax_layout_busy_page_range - find first pinned page in @mapping
 * @mapping: address space to scan for a page with ref count > 1
 * @start: Starting offset. Page containing 'start' is included.
 * @end: End offset. Page containing 'end' is included. If 'end' is LLONG_MAX,
 *       pages from 'start' till the end of file are included.
>>>>>>> upstream/android-13
 *
 * DAX requires ZONE_DEVICE mapped pages. These pages are never
 * 'onlined' to the page allocator so they are considered idle when
 * page->count == 1. A filesystem uses this interface to determine if
 * any page in the mapping is busy, i.e. for DMA, or other
 * get_user_pages() usages.
 *
 * It is expected that the filesystem is holding locks to block the
 * establishment of new mappings in this address_space. I.e. it expects
 * to be able to run unmap_mapping_range() and subsequently not race
 * mapping_mapped() becoming true.
 */
<<<<<<< HEAD
struct page *dax_layout_busy_page(struct address_space *mapping)
{
	pgoff_t	indices[PAGEVEC_SIZE];
	struct page *page = NULL;
	struct pagevec pvec;
	pgoff_t	index, end;
	unsigned i;
=======
struct page *dax_layout_busy_page_range(struct address_space *mapping,
					loff_t start, loff_t end)
{
	void *entry;
	unsigned int scanned = 0;
	struct page *page = NULL;
	pgoff_t start_idx = start >> PAGE_SHIFT;
	pgoff_t end_idx;
	XA_STATE(xas, &mapping->i_pages, start_idx);
>>>>>>> upstream/android-13

	/*
	 * In the 'limited' case get_user_pages() for dax is disabled.
	 */
	if (IS_ENABLED(CONFIG_FS_DAX_LIMITED))
		return NULL;

	if (!dax_mapping(mapping) || !mapping_mapped(mapping))
		return NULL;

<<<<<<< HEAD
	pagevec_init(&pvec);
	index = 0;
	end = -1;

	/*
	 * If we race get_user_pages_fast() here either we'll see the
	 * elevated page count in the pagevec_lookup and wait, or
=======
	/* If end == LLONG_MAX, all pages from start to till end of file */
	if (end == LLONG_MAX)
		end_idx = ULONG_MAX;
	else
		end_idx = end >> PAGE_SHIFT;
	/*
	 * If we race get_user_pages_fast() here either we'll see the
	 * elevated page count in the iteration and wait, or
>>>>>>> upstream/android-13
	 * get_user_pages_fast() will see that the page it took a reference
	 * against is no longer mapped in the page tables and bail to the
	 * get_user_pages() slow path.  The slow path is protected by
	 * pte_lock() and pmd_lock(). New references are not taken without
<<<<<<< HEAD
	 * holding those locks, and unmap_mapping_range() will not zero the
=======
	 * holding those locks, and unmap_mapping_pages() will not zero the
>>>>>>> upstream/android-13
	 * pte or pmd without holding the respective lock, so we are
	 * guaranteed to either see new references or prevent new
	 * references from being established.
	 */
<<<<<<< HEAD
	unmap_mapping_range(mapping, 0, 0, 0);

	while (index < end && pagevec_lookup_entries(&pvec, mapping, index,
				min(end - index, (pgoff_t)PAGEVEC_SIZE),
				indices)) {
		pgoff_t nr_pages = 1;

		for (i = 0; i < pagevec_count(&pvec); i++) {
			struct page *pvec_ent = pvec.pages[i];
			void *entry;

			index = indices[i];
			if (index >= end)
				break;

			if (WARN_ON_ONCE(
			     !radix_tree_exceptional_entry(pvec_ent)))
				continue;

			xa_lock_irq(&mapping->i_pages);
			entry = get_unlocked_mapping_entry(mapping, index, NULL);
			if (entry) {
				page = dax_busy_page(entry);
				/*
				 * Account for multi-order entries at
				 * the end of the pagevec.
				 */
				if (i + 1 >= pagevec_count(&pvec))
					nr_pages = 1UL << dax_radix_order(entry);
			}
			put_unlocked_mapping_entry(mapping, index, entry);
			xa_unlock_irq(&mapping->i_pages);
			if (page)
				break;
		}

		/*
		 * We don't expect normal struct page entries to exist in our
		 * tree, but we keep these pagevec calls so that this code is
		 * consistent with the common pattern for handling pagevecs
		 * throughout the kernel.
		 */
		pagevec_remove_exceptionals(&pvec);
		pagevec_release(&pvec);
		index += nr_pages;

		if (page)
			break;
	}
	return page;
}
EXPORT_SYMBOL_GPL(dax_layout_busy_page);

static int __dax_invalidate_mapping_entry(struct address_space *mapping,
					  pgoff_t index, bool trunc)
{
	int ret = 0;
	void *entry;
	struct radix_tree_root *pages = &mapping->i_pages;

	xa_lock_irq(pages);
	entry = get_unlocked_mapping_entry(mapping, index, NULL);
	if (!entry || WARN_ON_ONCE(!radix_tree_exceptional_entry(entry)))
		goto out;
	if (!trunc &&
	    (radix_tree_tag_get(pages, index, PAGECACHE_TAG_DIRTY) ||
	     radix_tree_tag_get(pages, index, PAGECACHE_TAG_TOWRITE)))
		goto out;
	dax_disassociate_entry(entry, mapping, trunc);
	radix_tree_delete(pages, index);
	mapping->nrexceptional--;
	ret = 1;
out:
	put_unlocked_mapping_entry(mapping, index, entry);
	xa_unlock_irq(pages);
	return ret;
}
/*
 * Delete exceptional DAX entry at @index from @mapping. Wait for radix tree
 * entry to get unlocked before deleting it.
 */
int dax_delete_mapping_entry(struct address_space *mapping, pgoff_t index)
{
	int ret = __dax_invalidate_mapping_entry(mapping, index, true);
=======
	unmap_mapping_pages(mapping, start_idx, end_idx - start_idx + 1, 0);

	xas_lock_irq(&xas);
	xas_for_each(&xas, entry, end_idx) {
		if (WARN_ON_ONCE(!xa_is_value(entry)))
			continue;
		if (unlikely(dax_is_locked(entry)))
			entry = get_unlocked_entry(&xas, 0);
		if (entry)
			page = dax_busy_page(entry);
		put_unlocked_entry(&xas, entry, WAKE_NEXT);
		if (page)
			break;
		if (++scanned % XA_CHECK_SCHED)
			continue;

		xas_pause(&xas);
		xas_unlock_irq(&xas);
		cond_resched();
		xas_lock_irq(&xas);
	}
	xas_unlock_irq(&xas);
	return page;
}
EXPORT_SYMBOL_GPL(dax_layout_busy_page_range);

struct page *dax_layout_busy_page(struct address_space *mapping)
{
	return dax_layout_busy_page_range(mapping, 0, LLONG_MAX);
}
EXPORT_SYMBOL_GPL(dax_layout_busy_page);

static int __dax_invalidate_entry(struct address_space *mapping,
					  pgoff_t index, bool trunc)
{
	XA_STATE(xas, &mapping->i_pages, index);
	int ret = 0;
	void *entry;

	xas_lock_irq(&xas);
	entry = get_unlocked_entry(&xas, 0);
	if (!entry || WARN_ON_ONCE(!xa_is_value(entry)))
		goto out;
	if (!trunc &&
	    (xas_get_mark(&xas, PAGECACHE_TAG_DIRTY) ||
	     xas_get_mark(&xas, PAGECACHE_TAG_TOWRITE)))
		goto out;
	dax_disassociate_entry(entry, mapping, trunc);
	xas_store(&xas, NULL);
	mapping->nrpages -= 1UL << dax_entry_order(entry);
	ret = 1;
out:
	put_unlocked_entry(&xas, entry, WAKE_ALL);
	xas_unlock_irq(&xas);
	return ret;
}

/*
 * Delete DAX entry at @index from @mapping.  Wait for it
 * to be unlocked before deleting it.
 */
int dax_delete_mapping_entry(struct address_space *mapping, pgoff_t index)
{
	int ret = __dax_invalidate_entry(mapping, index, true);
>>>>>>> upstream/android-13

	/*
	 * This gets called from truncate / punch_hole path. As such, the caller
	 * must hold locks protecting against concurrent modifications of the
<<<<<<< HEAD
	 * radix tree (usually fs-private i_mmap_sem for writing). Since the
	 * caller has seen exceptional entry for this index, we better find it
=======
	 * page cache (usually fs-private i_mmap_sem for writing). Since the
	 * caller has seen a DAX entry for this index, we better find it
>>>>>>> upstream/android-13
	 * at that index as well...
	 */
	WARN_ON_ONCE(!ret);
	return ret;
}

/*
<<<<<<< HEAD
 * Invalidate exceptional DAX entry if it is clean.
=======
 * Invalidate DAX entry if it is clean.
>>>>>>> upstream/android-13
 */
int dax_invalidate_mapping_entry_sync(struct address_space *mapping,
				      pgoff_t index)
{
<<<<<<< HEAD
	return __dax_invalidate_mapping_entry(mapping, index, false);
}

static int copy_user_dax(struct block_device *bdev, struct dax_device *dax_dev,
		sector_t sector, size_t size, struct page *to,
		unsigned long vaddr)
=======
	return __dax_invalidate_entry(mapping, index, false);
}

static int copy_cow_page_dax(struct block_device *bdev, struct dax_device *dax_dev,
			     sector_t sector, struct page *to, unsigned long vaddr)
>>>>>>> upstream/android-13
{
	void *vto, *kaddr;
	pgoff_t pgoff;
	long rc;
	int id;

<<<<<<< HEAD
	rc = bdev_dax_pgoff(bdev, sector, size, &pgoff);
=======
	rc = bdev_dax_pgoff(bdev, sector, PAGE_SIZE, &pgoff);
>>>>>>> upstream/android-13
	if (rc)
		return rc;

	id = dax_read_lock();
<<<<<<< HEAD
	rc = dax_direct_access(dax_dev, pgoff, PHYS_PFN(size), &kaddr, NULL);
=======
	rc = dax_direct_access(dax_dev, pgoff, 1, &kaddr, NULL);
>>>>>>> upstream/android-13
	if (rc < 0) {
		dax_read_unlock(id);
		return rc;
	}
	vto = kmap_atomic(to);
	copy_user_page(vto, (void __force *)kaddr, vaddr, to);
	kunmap_atomic(vto);
	dax_read_unlock(id);
	return 0;
}

/*
 * By this point grab_mapping_entry() has ensured that we have a locked entry
 * of the appropriate size so we don't have to worry about downgrading PMDs to
 * PTEs.  If we happen to be trying to insert a PTE and there is a PMD
 * already in the tree, we will skip the insertion and just dirty the PMD as
 * appropriate.
 */
<<<<<<< HEAD
static void *dax_insert_mapping_entry(struct address_space *mapping,
				      struct vm_fault *vmf,
				      void *entry, pfn_t pfn_t,
				      unsigned long flags, bool dirty)
{
	struct radix_tree_root *pages = &mapping->i_pages;
	unsigned long pfn = pfn_t_to_pfn(pfn_t);
	pgoff_t index = vmf->pgoff;
	void *new_entry;
=======
static void *dax_insert_entry(struct xa_state *xas,
		struct address_space *mapping, struct vm_fault *vmf,
		void *entry, pfn_t pfn, unsigned long flags, bool dirty)
{
	void *new_entry = dax_make_entry(pfn, flags);
>>>>>>> upstream/android-13

	if (dirty)
		__mark_inode_dirty(mapping->host, I_DIRTY_PAGES);

<<<<<<< HEAD
	if (dax_is_zero_entry(entry) && !(flags & RADIX_DAX_ZERO_PAGE)) {
		/* we are replacing a zero page with block mapping */
		if (dax_is_pmd_entry(entry))
			unmap_mapping_pages(mapping, index & ~PG_PMD_COLOUR,
							PG_PMD_NR, false);
		else /* pte entry */
			unmap_mapping_pages(mapping, vmf->pgoff, 1, false);
	}

	xa_lock_irq(pages);
	new_entry = dax_radix_locked_entry(pfn, flags);
	if (dax_entry_size(entry) != dax_entry_size(new_entry)) {
		dax_disassociate_entry(entry, mapping, false);
		dax_associate_entry(new_entry, mapping, vmf->vma, vmf->address);
	}

	if (dax_is_zero_entry(entry) || dax_is_empty_entry(entry)) {
		/*
		 * Only swap our new entry into the radix tree if the current
		 * entry is a zero page or an empty entry.  If a normal PTE or
		 * PMD entry is already in the tree, we leave it alone.  This
=======
	if (dax_is_zero_entry(entry) && !(flags & DAX_ZERO_PAGE)) {
		unsigned long index = xas->xa_index;
		/* we are replacing a zero page with block mapping */
		if (dax_is_pmd_entry(entry))
			unmap_mapping_pages(mapping, index & ~PG_PMD_COLOUR,
					PG_PMD_NR, false);
		else /* pte entry */
			unmap_mapping_pages(mapping, index, 1, false);
	}

	xas_reset(xas);
	xas_lock_irq(xas);
	if (dax_is_zero_entry(entry) || dax_is_empty_entry(entry)) {
		void *old;

		dax_disassociate_entry(entry, mapping, false);
		dax_associate_entry(new_entry, mapping, vmf->vma, vmf->address);
		/*
		 * Only swap our new entry into the page cache if the current
		 * entry is a zero page or an empty entry.  If a normal PTE or
		 * PMD entry is already in the cache, we leave it alone.  This
>>>>>>> upstream/android-13
		 * means that if we are trying to insert a PTE and the
		 * existing entry is a PMD, we will just leave the PMD in the
		 * tree and dirty it if necessary.
		 */
<<<<<<< HEAD
		struct radix_tree_node *node;
		void **slot;
		void *ret;

		ret = __radix_tree_lookup(pages, index, &node, &slot);
		WARN_ON_ONCE(ret != entry);
		__radix_tree_replace(pages, node, slot,
				     new_entry, NULL);
		entry = new_entry;
	}

	if (dirty)
		radix_tree_tag_set(pages, index, PAGECACHE_TAG_DIRTY);

	xa_unlock_irq(pages);
	return entry;
}

static inline unsigned long
pgoff_address(pgoff_t pgoff, struct vm_area_struct *vma)
=======
		old = dax_lock_entry(xas, new_entry);
		WARN_ON_ONCE(old != xa_mk_value(xa_to_value(entry) |
					DAX_LOCKED));
		entry = new_entry;
	} else {
		xas_load(xas);	/* Walk the xa_state */
	}

	if (dirty)
		xas_set_mark(xas, PAGECACHE_TAG_DIRTY);

	xas_unlock_irq(xas);
	return entry;
}

static inline
unsigned long pgoff_address(pgoff_t pgoff, struct vm_area_struct *vma)
>>>>>>> upstream/android-13
{
	unsigned long address;

	address = vma->vm_start + ((pgoff - vma->vm_pgoff) << PAGE_SHIFT);
	VM_BUG_ON_VMA(address < vma->vm_start || address >= vma->vm_end, vma);
	return address;
}

/* Walk all mappings of a given index of a file and writeprotect them */
<<<<<<< HEAD
static void dax_mapping_entry_mkclean(struct address_space *mapping,
				      pgoff_t index, unsigned long pfn)
=======
static void dax_entry_mkclean(struct address_space *mapping, pgoff_t index,
		unsigned long pfn)
>>>>>>> upstream/android-13
{
	struct vm_area_struct *vma;
	pte_t pte, *ptep = NULL;
	pmd_t *pmdp = NULL;
	spinlock_t *ptl;

	i_mmap_lock_read(mapping);
	vma_interval_tree_foreach(vma, &mapping->i_mmap, index, index) {
<<<<<<< HEAD
		unsigned long address, start, end;
=======
		struct mmu_notifier_range range;
		unsigned long address;
>>>>>>> upstream/android-13

		cond_resched();

		if (!(vma->vm_flags & VM_SHARED))
			continue;

		address = pgoff_address(index, vma);

		/*
<<<<<<< HEAD
		 * Note because we provide start/end to follow_pte_pmd it will
		 * call mmu_notifier_invalidate_range_start() on our behalf
		 * before taking any lock.
		 */
		if (follow_pte_pmd(vma->vm_mm, address, &start, &end, &ptep, &pmdp, &ptl))
=======
		 * follow_invalidate_pte() will use the range to call
		 * mmu_notifier_invalidate_range_start() on our behalf before
		 * taking any lock.
		 */
		if (follow_invalidate_pte(vma->vm_mm, address, &range, &ptep,
					  &pmdp, &ptl))
>>>>>>> upstream/android-13
			continue;

		/*
		 * No need to call mmu_notifier_invalidate_range() as we are
		 * downgrading page table protection not changing it to point
		 * to a new page.
		 *
		 * See Documentation/vm/mmu_notifier.rst
		 */
		if (pmdp) {
#ifdef CONFIG_FS_DAX_PMD
			pmd_t pmd;

			if (pfn != pmd_pfn(*pmdp))
				goto unlock_pmd;
			if (!pmd_dirty(*pmdp) && !pmd_write(*pmdp))
				goto unlock_pmd;

			flush_cache_page(vma, address, pfn);
			pmd = pmdp_invalidate(vma, address, pmdp);
			pmd = pmd_wrprotect(pmd);
			pmd = pmd_mkclean(pmd);
			set_pmd_at(vma->vm_mm, address, pmdp, pmd);
unlock_pmd:
#endif
			spin_unlock(ptl);
		} else {
			if (pfn != pte_pfn(*ptep))
				goto unlock_pte;
			if (!pte_dirty(*ptep) && !pte_write(*ptep))
				goto unlock_pte;

			flush_cache_page(vma, address, pfn);
			pte = ptep_clear_flush(vma, address, ptep);
			pte = pte_wrprotect(pte);
			pte = pte_mkclean(pte);
			set_pte_at(vma->vm_mm, address, ptep, pte);
unlock_pte:
			pte_unmap_unlock(ptep, ptl);
		}

<<<<<<< HEAD
		mmu_notifier_invalidate_range_end(vma->vm_mm, start, end);
=======
		mmu_notifier_invalidate_range_end(&range);
>>>>>>> upstream/android-13
	}
	i_mmap_unlock_read(mapping);
}

<<<<<<< HEAD
static int dax_writeback_one(struct dax_device *dax_dev,
		struct address_space *mapping, pgoff_t index, void *entry)
{
	struct radix_tree_root *pages = &mapping->i_pages;
	void *entry2, **slot;
	unsigned long pfn;
	long ret = 0;
	size_t size;
=======
static int dax_writeback_one(struct xa_state *xas, struct dax_device *dax_dev,
		struct address_space *mapping, void *entry)
{
	unsigned long pfn, index, count;
	long ret = 0;
>>>>>>> upstream/android-13

	/*
	 * A page got tagged dirty in DAX mapping? Something is seriously
	 * wrong.
	 */
<<<<<<< HEAD
	if (WARN_ON(!radix_tree_exceptional_entry(entry)))
		return -EIO;

	xa_lock_irq(pages);
	entry2 = get_unlocked_mapping_entry(mapping, index, &slot);
	/* Entry got punched out / reallocated? */
	if (!entry2 || WARN_ON_ONCE(!radix_tree_exceptional_entry(entry2)))
		goto put_unlocked;
	/*
	 * Entry got reallocated elsewhere? No need to writeback. We have to
	 * compare pfns as we must not bail out due to difference in lockbit
	 * or entry type.
	 */
	if (dax_radix_pfn(entry2) != dax_radix_pfn(entry))
		goto put_unlocked;
	if (WARN_ON_ONCE(dax_is_empty_entry(entry) ||
				dax_is_zero_entry(entry))) {
		ret = -EIO;
		goto put_unlocked;
	}

	/* Another fsync thread may have already written back this entry */
	if (!radix_tree_tag_get(pages, index, PAGECACHE_TAG_TOWRITE))
		goto put_unlocked;
	/* Lock the entry to serialize with page faults */
	entry = lock_slot(mapping, slot);
=======
	if (WARN_ON(!xa_is_value(entry)))
		return -EIO;

	if (unlikely(dax_is_locked(entry))) {
		void *old_entry = entry;

		entry = get_unlocked_entry(xas, 0);

		/* Entry got punched out / reallocated? */
		if (!entry || WARN_ON_ONCE(!xa_is_value(entry)))
			goto put_unlocked;
		/*
		 * Entry got reallocated elsewhere? No need to writeback.
		 * We have to compare pfns as we must not bail out due to
		 * difference in lockbit or entry type.
		 */
		if (dax_to_pfn(old_entry) != dax_to_pfn(entry))
			goto put_unlocked;
		if (WARN_ON_ONCE(dax_is_empty_entry(entry) ||
					dax_is_zero_entry(entry))) {
			ret = -EIO;
			goto put_unlocked;
		}

		/* Another fsync thread may have already done this entry */
		if (!xas_get_mark(xas, PAGECACHE_TAG_TOWRITE))
			goto put_unlocked;
	}

	/* Lock the entry to serialize with page faults */
	dax_lock_entry(xas, entry);

>>>>>>> upstream/android-13
	/*
	 * We can clear the tag now but we have to be careful so that concurrent
	 * dax_writeback_one() calls for the same index cannot finish before we
	 * actually flush the caches. This is achieved as the calls will look
	 * at the entry only under the i_pages lock and once they do that
	 * they will see the entry locked and wait for it to unlock.
	 */
<<<<<<< HEAD
	radix_tree_tag_clear(pages, index, PAGECACHE_TAG_TOWRITE);
	xa_unlock_irq(pages);

	/*
	 * Even if dax_writeback_mapping_range() was given a wbc->range_start
	 * in the middle of a PMD, the 'index' we are given will be aligned to
	 * the start index of the PMD, as will the pfn we pull from 'entry'.
	 * This allows us to flush for PMD_SIZE and not have to worry about
	 * partial PMD writebacks.
	 */
	pfn = dax_radix_pfn(entry);
	size = PAGE_SIZE << dax_radix_order(entry);

	dax_mapping_entry_mkclean(mapping, index, pfn);
	dax_flush(dax_dev, page_address(pfn_to_page(pfn)), size);
=======
	xas_clear_mark(xas, PAGECACHE_TAG_TOWRITE);
	xas_unlock_irq(xas);

	/*
	 * If dax_writeback_mapping_range() was given a wbc->range_start
	 * in the middle of a PMD, the 'index' we use needs to be
	 * aligned to the start of the PMD.
	 * This allows us to flush for PMD_SIZE and not have to worry about
	 * partial PMD writebacks.
	 */
	pfn = dax_to_pfn(entry);
	count = 1UL << dax_entry_order(entry);
	index = xas->xa_index & ~(count - 1);

	dax_entry_mkclean(mapping, index, pfn);
	dax_flush(dax_dev, page_address(pfn_to_page(pfn)), count * PAGE_SIZE);
>>>>>>> upstream/android-13
	/*
	 * After we have flushed the cache, we can clear the dirty tag. There
	 * cannot be new dirty data in the pfn after the flush has completed as
	 * the pfn mappings are writeprotected and fault waits for mapping
	 * entry lock.
	 */
<<<<<<< HEAD
	xa_lock_irq(pages);
	radix_tree_tag_clear(pages, index, PAGECACHE_TAG_DIRTY);
	xa_unlock_irq(pages);
	trace_dax_writeback_one(mapping->host, index, size >> PAGE_SHIFT);
	put_locked_mapping_entry(mapping, index);
	return ret;

 put_unlocked:
	put_unlocked_mapping_entry(mapping, index, entry2);
	xa_unlock_irq(pages);
=======
	xas_reset(xas);
	xas_lock_irq(xas);
	xas_store(xas, entry);
	xas_clear_mark(xas, PAGECACHE_TAG_DIRTY);
	dax_wake_entry(xas, entry, WAKE_NEXT);

	trace_dax_writeback_one(mapping->host, index, count);
	return ret;

 put_unlocked:
	put_unlocked_entry(xas, entry, WAKE_NEXT);
>>>>>>> upstream/android-13
	return ret;
}

/*
 * Flush the mapping to the persistent domain within the byte range of [start,
 * end]. This is required by data integrity operations to ensure file data is
 * on persistent storage prior to completion of the operation.
 */
int dax_writeback_mapping_range(struct address_space *mapping,
<<<<<<< HEAD
		struct block_device *bdev, struct writeback_control *wbc)
{
	struct inode *inode = mapping->host;
	pgoff_t start_index, end_index;
	pgoff_t indices[PAGEVEC_SIZE];
	struct dax_device *dax_dev;
	struct pagevec pvec;
	bool done = false;
	int i, ret = 0;
=======
		struct dax_device *dax_dev, struct writeback_control *wbc)
{
	XA_STATE(xas, &mapping->i_pages, wbc->range_start >> PAGE_SHIFT);
	struct inode *inode = mapping->host;
	pgoff_t end_index = wbc->range_end >> PAGE_SHIFT;
	void *entry;
	int ret = 0;
	unsigned int scanned = 0;
>>>>>>> upstream/android-13

	if (WARN_ON_ONCE(inode->i_blkbits != PAGE_SHIFT))
		return -EIO;

<<<<<<< HEAD
	if (!mapping->nrexceptional || wbc->sync_mode != WB_SYNC_ALL)
		return 0;

	dax_dev = dax_get_by_host(bdev->bd_disk->disk_name);
	if (!dax_dev)
		return -EIO;

	start_index = wbc->range_start >> PAGE_SHIFT;
	end_index = wbc->range_end >> PAGE_SHIFT;

	trace_dax_writeback_range(inode, start_index, end_index);

	tag_pages_for_writeback(mapping, start_index, end_index);

	pagevec_init(&pvec);
	while (!done) {
		pvec.nr = find_get_entries_tag(mapping, start_index,
				PAGECACHE_TAG_TOWRITE, PAGEVEC_SIZE,
				pvec.pages, indices);

		if (pvec.nr == 0)
			break;

		for (i = 0; i < pvec.nr; i++) {
			if (indices[i] > end_index) {
				done = true;
				break;
			}

			ret = dax_writeback_one(dax_dev, mapping, indices[i],
					pvec.pages[i]);
			if (ret < 0) {
				mapping_set_error(mapping, ret);
				goto out;
			}
		}
		start_index = indices[pvec.nr - 1] + 1;
	}
out:
	put_dax(dax_dev);
	trace_dax_writeback_range_done(inode, start_index, end_index);
	return (ret < 0 ? ret : 0);
}
EXPORT_SYMBOL_GPL(dax_writeback_mapping_range);

static sector_t dax_iomap_sector(struct iomap *iomap, loff_t pos)
=======
	if (mapping_empty(mapping) || wbc->sync_mode != WB_SYNC_ALL)
		return 0;

	trace_dax_writeback_range(inode, xas.xa_index, end_index);

	tag_pages_for_writeback(mapping, xas.xa_index, end_index);

	xas_lock_irq(&xas);
	xas_for_each_marked(&xas, entry, end_index, PAGECACHE_TAG_TOWRITE) {
		ret = dax_writeback_one(&xas, dax_dev, mapping, entry);
		if (ret < 0) {
			mapping_set_error(mapping, ret);
			break;
		}
		if (++scanned % XA_CHECK_SCHED)
			continue;

		xas_pause(&xas);
		xas_unlock_irq(&xas);
		cond_resched();
		xas_lock_irq(&xas);
	}
	xas_unlock_irq(&xas);
	trace_dax_writeback_range_done(inode, xas.xa_index, end_index);
	return ret;
}
EXPORT_SYMBOL_GPL(dax_writeback_mapping_range);

static sector_t dax_iomap_sector(const struct iomap *iomap, loff_t pos)
>>>>>>> upstream/android-13
{
	return (iomap->addr + (pos & PAGE_MASK) - iomap->offset) >> 9;
}

<<<<<<< HEAD
static int dax_iomap_pfn(struct iomap *iomap, loff_t pos, size_t size,
=======
static int dax_iomap_pfn(const struct iomap *iomap, loff_t pos, size_t size,
>>>>>>> upstream/android-13
			 pfn_t *pfnp)
{
	const sector_t sector = dax_iomap_sector(iomap, pos);
	pgoff_t pgoff;
	int id, rc;
	long length;

	rc = bdev_dax_pgoff(iomap->bdev, sector, size, &pgoff);
	if (rc)
		return rc;
	id = dax_read_lock();
	length = dax_direct_access(iomap->dax_dev, pgoff, PHYS_PFN(size),
				   NULL, pfnp);
	if (length < 0) {
		rc = length;
		goto out;
	}
	rc = -EINVAL;
	if (PFN_PHYS(length) < size)
		goto out;
	if (pfn_t_to_pfn(*pfnp) & (PHYS_PFN(size)-1))
		goto out;
	/* For larger pages we need devmap */
	if (length > 1 && !pfn_t_devmap(*pfnp))
		goto out;
	rc = 0;
out:
	dax_read_unlock(id);
	return rc;
}

/*
 * The user has performed a load from a hole in the file.  Allocating a new
 * page in the file would cause excessive storage usage for workloads with
 * sparse files.  Instead we insert a read-only mapping of the 4k zero page.
 * If this page is ever written to we will re-fault and change the mapping to
 * point to real DAX storage instead.
 */
<<<<<<< HEAD
static vm_fault_t dax_load_hole(struct address_space *mapping, void *entry,
			 struct vm_fault *vmf)
=======
static vm_fault_t dax_load_hole(struct xa_state *xas,
		struct address_space *mapping, void **entry,
		struct vm_fault *vmf)
>>>>>>> upstream/android-13
{
	struct inode *inode = mapping->host;
	unsigned long vaddr = vmf->address;
	pfn_t pfn = pfn_to_pfn_t(my_zero_pfn(vaddr));
	vm_fault_t ret;

<<<<<<< HEAD
	dax_insert_mapping_entry(mapping, vmf, entry, pfn, RADIX_DAX_ZERO_PAGE,
			false);
=======
	*entry = dax_insert_entry(xas, mapping, vmf, *entry, pfn,
			DAX_ZERO_PAGE, false);

>>>>>>> upstream/android-13
	ret = vmf_insert_mixed(vmf->vma, vaddr, pfn);
	trace_dax_load_hole(inode, vmf, ret);
	return ret;
}

<<<<<<< HEAD
static bool dax_range_is_aligned(struct block_device *bdev,
				 unsigned int offset, unsigned int length)
{
	unsigned short sector_size = bdev_logical_block_size(bdev);

	if (!IS_ALIGNED(offset, sector_size))
		return false;
	if (!IS_ALIGNED(length, sector_size))
		return false;

	return true;
}

int __dax_zero_page_range(struct block_device *bdev,
		struct dax_device *dax_dev, sector_t sector,
		unsigned int offset, unsigned int size)
{
	if (dax_range_is_aligned(bdev, offset, size)) {
		sector_t start_sector = sector + (offset >> 9);

		return blkdev_issue_zeroout(bdev, start_sector,
				size >> 9, GFP_NOFS, 0);
	} else {
		pgoff_t pgoff;
		long rc, id;
		void *kaddr;

		rc = bdev_dax_pgoff(bdev, sector, PAGE_SIZE, &pgoff);
		if (rc)
			return rc;

		id = dax_read_lock();
		rc = dax_direct_access(dax_dev, pgoff, 1, &kaddr, NULL);
		if (rc < 0) {
			dax_read_unlock(id);
			return rc;
		}
		memset(kaddr + offset, 0, size);
		dax_flush(dax_dev, kaddr + offset, size);
		dax_read_unlock(id);
	}
	return 0;
}
EXPORT_SYMBOL_GPL(__dax_zero_page_range);

static loff_t
dax_iomap_actor(struct inode *inode, loff_t pos, loff_t length, void *data,
		struct iomap *iomap)
{
	struct block_device *bdev = iomap->bdev;
	struct dax_device *dax_dev = iomap->dax_dev;
	struct iov_iter *iter = data;
=======
#ifdef CONFIG_FS_DAX_PMD
static vm_fault_t dax_pmd_load_hole(struct xa_state *xas, struct vm_fault *vmf,
		const struct iomap *iomap, void **entry)
{
	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
	unsigned long pmd_addr = vmf->address & PMD_MASK;
	struct vm_area_struct *vma = vmf->vma;
	struct inode *inode = mapping->host;
	pgtable_t pgtable = NULL;
	struct page *zero_page;
	spinlock_t *ptl;
	pmd_t pmd_entry;
	pfn_t pfn;

	zero_page = mm_get_huge_zero_page(vmf->vma->vm_mm);

	if (unlikely(!zero_page))
		goto fallback;

	pfn = page_to_pfn_t(zero_page);
	*entry = dax_insert_entry(xas, mapping, vmf, *entry, pfn,
			DAX_PMD | DAX_ZERO_PAGE, false);

	if (arch_needs_pgtable_deposit()) {
		pgtable = pte_alloc_one(vma->vm_mm);
		if (!pgtable)
			return VM_FAULT_OOM;
	}

	ptl = pmd_lock(vmf->vma->vm_mm, vmf->pmd);
	if (!pmd_none(*(vmf->pmd))) {
		spin_unlock(ptl);
		goto fallback;
	}

	if (pgtable) {
		pgtable_trans_huge_deposit(vma->vm_mm, vmf->pmd, pgtable);
		mm_inc_nr_ptes(vma->vm_mm);
	}
	pmd_entry = mk_pmd(zero_page, vmf->vma->vm_page_prot);
	pmd_entry = pmd_mkhuge(pmd_entry);
	set_pmd_at(vmf->vma->vm_mm, pmd_addr, vmf->pmd, pmd_entry);
	spin_unlock(ptl);
	trace_dax_pmd_load_hole(inode, vmf, zero_page, *entry);
	return VM_FAULT_NOPAGE;

fallback:
	if (pgtable)
		pte_free(vma->vm_mm, pgtable);
	trace_dax_pmd_load_hole_fallback(inode, vmf, zero_page, *entry);
	return VM_FAULT_FALLBACK;
}
#else
static vm_fault_t dax_pmd_load_hole(struct xa_state *xas, struct vm_fault *vmf,
		const struct iomap *iomap, void **entry)
{
	return VM_FAULT_FALLBACK;
}
#endif /* CONFIG_FS_DAX_PMD */

s64 dax_iomap_zero(loff_t pos, u64 length, struct iomap *iomap)
{
	sector_t sector = iomap_sector(iomap, pos & PAGE_MASK);
	pgoff_t pgoff;
	long rc, id;
	void *kaddr;
	bool page_aligned = false;
	unsigned offset = offset_in_page(pos);
	unsigned size = min_t(u64, PAGE_SIZE - offset, length);

	if (IS_ALIGNED(sector << SECTOR_SHIFT, PAGE_SIZE) &&
	    (size == PAGE_SIZE))
		page_aligned = true;

	rc = bdev_dax_pgoff(iomap->bdev, sector, PAGE_SIZE, &pgoff);
	if (rc)
		return rc;

	id = dax_read_lock();

	if (page_aligned)
		rc = dax_zero_page_range(iomap->dax_dev, pgoff, 1);
	else
		rc = dax_direct_access(iomap->dax_dev, pgoff, 1, &kaddr, NULL);
	if (rc < 0) {
		dax_read_unlock(id);
		return rc;
	}

	if (!page_aligned) {
		memset(kaddr + offset, 0, size);
		dax_flush(iomap->dax_dev, kaddr + offset, size);
	}
	dax_read_unlock(id);
	return size;
}

static loff_t dax_iomap_iter(const struct iomap_iter *iomi,
		struct iov_iter *iter)
{
	const struct iomap *iomap = &iomi->iomap;
	loff_t length = iomap_length(iomi);
	loff_t pos = iomi->pos;
	struct block_device *bdev = iomap->bdev;
	struct dax_device *dax_dev = iomap->dax_dev;
>>>>>>> upstream/android-13
	loff_t end = pos + length, done = 0;
	ssize_t ret = 0;
	size_t xfer;
	int id;

	if (iov_iter_rw(iter) == READ) {
<<<<<<< HEAD
		end = min(end, i_size_read(inode));
=======
		end = min(end, i_size_read(iomi->inode));
>>>>>>> upstream/android-13
		if (pos >= end)
			return 0;

		if (iomap->type == IOMAP_HOLE || iomap->type == IOMAP_UNWRITTEN)
			return iov_iter_zero(min(length, end - pos), iter);
	}

	if (WARN_ON_ONCE(iomap->type != IOMAP_MAPPED))
		return -EIO;

	/*
	 * Write can allocate block for an area which has a hole page mapped
	 * into page tables. We have to tear down these mappings so that data
	 * written by write(2) is visible in mmap.
	 */
	if (iomap->flags & IOMAP_F_NEW) {
<<<<<<< HEAD
		invalidate_inode_pages2_range(inode->i_mapping,
=======
		invalidate_inode_pages2_range(iomi->inode->i_mapping,
>>>>>>> upstream/android-13
					      pos >> PAGE_SHIFT,
					      (end - 1) >> PAGE_SHIFT);
	}

	id = dax_read_lock();
	while (pos < end) {
		unsigned offset = pos & (PAGE_SIZE - 1);
		const size_t size = ALIGN(length + offset, PAGE_SIZE);
		const sector_t sector = dax_iomap_sector(iomap, pos);
		ssize_t map_len;
		pgoff_t pgoff;
		void *kaddr;

		if (fatal_signal_pending(current)) {
			ret = -EINTR;
			break;
		}

		ret = bdev_dax_pgoff(bdev, sector, size, &pgoff);
		if (ret)
			break;

		map_len = dax_direct_access(dax_dev, pgoff, PHYS_PFN(size),
				&kaddr, NULL);
		if (map_len < 0) {
			ret = map_len;
			break;
		}

		map_len = PFN_PHYS(map_len);
		kaddr += offset;
		map_len -= offset;
		if (map_len > end - pos)
			map_len = end - pos;

		/*
		 * The userspace address for the memory copy has already been
		 * validated via access_ok() in either vfs_read() or
		 * vfs_write(), depending on which operation we are doing.
		 */
		if (iov_iter_rw(iter) == WRITE)
			xfer = dax_copy_from_iter(dax_dev, pgoff, kaddr,
					map_len, iter);
		else
			xfer = dax_copy_to_iter(dax_dev, pgoff, kaddr,
					map_len, iter);

		pos += xfer;
		length -= xfer;
		done += xfer;

		if (xfer == 0)
			ret = -EFAULT;
		if (xfer < map_len)
			break;
	}
	dax_read_unlock(id);

	return done ? done : ret;
}

/**
 * dax_iomap_rw - Perform I/O to a DAX file
 * @iocb:	The control block for this I/O
 * @iter:	The addresses to do I/O from or to
 * @ops:	iomap ops passed from the file system
 *
 * This function performs read and write operations to directly mapped
 * persistent memory.  The callers needs to take care of read/write exclusion
 * and evicting any page cache pages in the region under I/O.
 */
ssize_t
dax_iomap_rw(struct kiocb *iocb, struct iov_iter *iter,
		const struct iomap_ops *ops)
{
<<<<<<< HEAD
	struct address_space *mapping = iocb->ki_filp->f_mapping;
	struct inode *inode = mapping->host;
	loff_t pos = iocb->ki_pos, ret = 0, done = 0;
	unsigned flags = 0;

	if (iov_iter_rw(iter) == WRITE) {
		lockdep_assert_held_exclusive(&inode->i_rwsem);
		flags |= IOMAP_WRITE;
	} else {
		lockdep_assert_held(&inode->i_rwsem);
	}

	if (iocb->ki_flags & IOCB_NOWAIT)
		flags |= IOMAP_NOWAIT;

	while (iov_iter_count(iter)) {
		ret = iomap_apply(inode, pos, iov_iter_count(iter), flags, ops,
				iter, dax_iomap_actor);
		if (ret <= 0)
			break;
		pos += ret;
		done += ret;
	}

	iocb->ki_pos += done;
=======
	struct iomap_iter iomi = {
		.inode		= iocb->ki_filp->f_mapping->host,
		.pos		= iocb->ki_pos,
		.len		= iov_iter_count(iter),
	};
	loff_t done = 0;
	int ret;

	if (iov_iter_rw(iter) == WRITE) {
		lockdep_assert_held_write(&iomi.inode->i_rwsem);
		iomi.flags |= IOMAP_WRITE;
	} else {
		lockdep_assert_held(&iomi.inode->i_rwsem);
	}

	if (iocb->ki_flags & IOCB_NOWAIT)
		iomi.flags |= IOMAP_NOWAIT;

	while ((ret = iomap_iter(&iomi, ops)) > 0)
		iomi.processed = dax_iomap_iter(&iomi, iter);

	done = iomi.pos - iocb->ki_pos;
	iocb->ki_pos = iomi.pos;
>>>>>>> upstream/android-13
	return done ? done : ret;
}
EXPORT_SYMBOL_GPL(dax_iomap_rw);

static vm_fault_t dax_fault_return(int error)
{
	if (error == 0)
		return VM_FAULT_NOPAGE;
<<<<<<< HEAD
	if (error == -ENOMEM)
		return VM_FAULT_OOM;
	return VM_FAULT_SIGBUS;
=======
	return vmf_error(error);
>>>>>>> upstream/android-13
}

/*
 * MAP_SYNC on a dax mapping guarantees dirty metadata is
 * flushed on write-faults (non-cow), but not read-faults.
 */
static bool dax_fault_is_synchronous(unsigned long flags,
<<<<<<< HEAD
		struct vm_area_struct *vma, struct iomap *iomap)
=======
		struct vm_area_struct *vma, const struct iomap *iomap)
>>>>>>> upstream/android-13
{
	return (flags & IOMAP_WRITE) && (vma->vm_flags & VM_SYNC)
		&& (iomap->flags & IOMAP_F_DIRTY);
}

<<<<<<< HEAD
static vm_fault_t dax_iomap_pte_fault(struct vm_fault *vmf, pfn_t *pfnp,
			       int *iomap_errp, const struct iomap_ops *ops)
{
	struct vm_area_struct *vma = vmf->vma;
	struct address_space *mapping = vma->vm_file->f_mapping;
	struct inode *inode = mapping->host;
	unsigned long vaddr = vmf->address;
	loff_t pos = (loff_t)vmf->pgoff << PAGE_SHIFT;
	struct iomap iomap = { 0 };
	unsigned flags = IOMAP_FAULT;
	int error, major = 0;
	bool write = vmf->flags & FAULT_FLAG_WRITE;
	bool sync;
	vm_fault_t ret = 0;
	void *entry;
	pfn_t pfn;

	trace_dax_pte_fault(inode, vmf, ret);
=======
/*
 * When handling a synchronous page fault and the inode need a fsync, we can
 * insert the PTE/PMD into page tables only after that fsync happened. Skip
 * insertion for now and return the pfn so that caller can insert it after the
 * fsync is done.
 */
static vm_fault_t dax_fault_synchronous_pfnp(pfn_t *pfnp, pfn_t pfn)
{
	if (WARN_ON_ONCE(!pfnp))
		return VM_FAULT_SIGBUS;
	*pfnp = pfn;
	return VM_FAULT_NEEDDSYNC;
}

static vm_fault_t dax_fault_cow_page(struct vm_fault *vmf,
		const struct iomap_iter *iter)
{
	sector_t sector = dax_iomap_sector(&iter->iomap, iter->pos);
	unsigned long vaddr = vmf->address;
	vm_fault_t ret;
	int error = 0;

	switch (iter->iomap.type) {
	case IOMAP_HOLE:
	case IOMAP_UNWRITTEN:
		clear_user_highpage(vmf->cow_page, vaddr);
		break;
	case IOMAP_MAPPED:
		error = copy_cow_page_dax(iter->iomap.bdev, iter->iomap.dax_dev,
					  sector, vmf->cow_page, vaddr);
		break;
	default:
		WARN_ON_ONCE(1);
		error = -EIO;
		break;
	}

	if (error)
		return dax_fault_return(error);

	__SetPageUptodate(vmf->cow_page);
	ret = finish_fault(vmf);
	if (!ret)
		return VM_FAULT_DONE_COW;
	return ret;
}

/**
 * dax_fault_iter - Common actor to handle pfn insertion in PTE/PMD fault.
 * @vmf:	vm fault instance
 * @iter:	iomap iter
 * @pfnp:	pfn to be returned
 * @xas:	the dax mapping tree of a file
 * @entry:	an unlocked dax entry to be inserted
 * @pmd:	distinguish whether it is a pmd fault
 */
static vm_fault_t dax_fault_iter(struct vm_fault *vmf,
		const struct iomap_iter *iter, pfn_t *pfnp,
		struct xa_state *xas, void **entry, bool pmd)
{
	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
	const struct iomap *iomap = &iter->iomap;
	size_t size = pmd ? PMD_SIZE : PAGE_SIZE;
	loff_t pos = (loff_t)xas->xa_index << PAGE_SHIFT;
	bool write = vmf->flags & FAULT_FLAG_WRITE;
	bool sync = dax_fault_is_synchronous(iter->flags, vmf->vma, iomap);
	unsigned long entry_flags = pmd ? DAX_PMD : 0;
	int err = 0;
	pfn_t pfn;

	if (!pmd && vmf->cow_page)
		return dax_fault_cow_page(vmf, iter);

	/* if we are reading UNWRITTEN and HOLE, return a hole. */
	if (!write &&
	    (iomap->type == IOMAP_UNWRITTEN || iomap->type == IOMAP_HOLE)) {
		if (!pmd)
			return dax_load_hole(xas, mapping, entry, vmf);
		return dax_pmd_load_hole(xas, vmf, iomap, entry);
	}

	if (iomap->type != IOMAP_MAPPED) {
		WARN_ON_ONCE(1);
		return pmd ? VM_FAULT_FALLBACK : VM_FAULT_SIGBUS;
	}

	err = dax_iomap_pfn(&iter->iomap, pos, size, &pfn);
	if (err)
		return pmd ? VM_FAULT_FALLBACK : dax_fault_return(err);

	*entry = dax_insert_entry(xas, mapping, vmf, *entry, pfn, entry_flags,
				  write && !sync);

	if (sync)
		return dax_fault_synchronous_pfnp(pfnp, pfn);

	/* insert PMD pfn */
	if (pmd)
		return vmf_insert_pfn_pmd(vmf, pfn, write);

	/* insert PTE pfn */
	if (write)
		return vmf_insert_mixed_mkwrite(vmf->vma, vmf->address, pfn);
	return vmf_insert_mixed(vmf->vma, vmf->address, pfn);
}

static vm_fault_t dax_iomap_pte_fault(struct vm_fault *vmf, pfn_t *pfnp,
			       int *iomap_errp, const struct iomap_ops *ops)
{
	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
	XA_STATE(xas, &mapping->i_pages, vmf->pgoff);
	struct iomap_iter iter = {
		.inode		= mapping->host,
		.pos		= (loff_t)vmf->pgoff << PAGE_SHIFT,
		.len		= PAGE_SIZE,
		.flags		= IOMAP_FAULT,
	};
	vm_fault_t ret = 0;
	void *entry;
	int error;

	trace_dax_pte_fault(iter.inode, vmf, ret);
>>>>>>> upstream/android-13
	/*
	 * Check whether offset isn't beyond end of file now. Caller is supposed
	 * to hold locks serializing us with truncate / punch hole so this is
	 * a reliable test.
	 */
<<<<<<< HEAD
	if (pos >= i_size_read(inode)) {
=======
	if (iter.pos >= i_size_read(iter.inode)) {
>>>>>>> upstream/android-13
		ret = VM_FAULT_SIGBUS;
		goto out;
	}

<<<<<<< HEAD
	if (write && !vmf->cow_page)
		flags |= IOMAP_WRITE;

	entry = grab_mapping_entry(mapping, vmf->pgoff, 0);
	if (IS_ERR(entry)) {
		ret = dax_fault_return(PTR_ERR(entry));
=======
	if ((vmf->flags & FAULT_FLAG_WRITE) && !vmf->cow_page)
		iter.flags |= IOMAP_WRITE;

	entry = grab_mapping_entry(&xas, mapping, 0);
	if (xa_is_internal(entry)) {
		ret = xa_to_internal(entry);
>>>>>>> upstream/android-13
		goto out;
	}

	/*
	 * It is possible, particularly with mixed reads & writes to private
	 * mappings, that we have raced with a PMD fault that overlaps with
	 * the PTE we need to set up.  If so just return and the fault will be
	 * retried.
	 */
	if (pmd_trans_huge(*vmf->pmd) || pmd_devmap(*vmf->pmd)) {
		ret = VM_FAULT_NOPAGE;
		goto unlock_entry;
	}

<<<<<<< HEAD
	/*
	 * Note that we don't bother to use iomap_apply here: DAX required
	 * the file system block size to be equal the page size, which means
	 * that we never have to deal with more than a single extent here.
	 */
	error = ops->iomap_begin(inode, pos, PAGE_SIZE, flags, &iomap);
	if (iomap_errp)
		*iomap_errp = error;
	if (error) {
		ret = dax_fault_return(error);
		goto unlock_entry;
	}
	if (WARN_ON_ONCE(iomap.offset + iomap.length < pos + PAGE_SIZE)) {
		error = -EIO;	/* fs corruption? */
		goto error_finish_iomap;
	}

	if (vmf->cow_page) {
		sector_t sector = dax_iomap_sector(&iomap, pos);

		switch (iomap.type) {
		case IOMAP_HOLE:
		case IOMAP_UNWRITTEN:
			clear_user_highpage(vmf->cow_page, vaddr);
			break;
		case IOMAP_MAPPED:
			error = copy_user_dax(iomap.bdev, iomap.dax_dev,
					sector, PAGE_SIZE, vmf->cow_page, vaddr);
			break;
		default:
			WARN_ON_ONCE(1);
			error = -EIO;
			break;
		}

		if (error)
			goto error_finish_iomap;

		__SetPageUptodate(vmf->cow_page);
		ret = finish_fault(vmf);
		if (!ret)
			ret = VM_FAULT_DONE_COW;
		goto finish_iomap;
	}

	sync = dax_fault_is_synchronous(flags, vma, &iomap);

	switch (iomap.type) {
	case IOMAP_MAPPED:
		if (iomap.flags & IOMAP_F_NEW) {
			count_vm_event(PGMAJFAULT);
			count_memcg_event_mm(vma->vm_mm, PGMAJFAULT);
			major = VM_FAULT_MAJOR;
		}
		error = dax_iomap_pfn(&iomap, pos, PAGE_SIZE, &pfn);
		if (error < 0)
			goto error_finish_iomap;

		entry = dax_insert_mapping_entry(mapping, vmf, entry, pfn,
						 0, write && !sync);

		/*
		 * If we are doing synchronous page fault and inode needs fsync,
		 * we can insert PTE into page tables only after that happens.
		 * Skip insertion for now and return the pfn so that caller can
		 * insert it after fsync is done.
		 */
		if (sync) {
			if (WARN_ON_ONCE(!pfnp)) {
				error = -EIO;
				goto error_finish_iomap;
			}
			*pfnp = pfn;
			ret = VM_FAULT_NEEDDSYNC | major;
			goto finish_iomap;
		}
		trace_dax_insert_mapping(inode, vmf, entry);
		if (write)
			ret = vmf_insert_mixed_mkwrite(vma, vaddr, pfn);
		else
			ret = vmf_insert_mixed(vma, vaddr, pfn);

		goto finish_iomap;
	case IOMAP_UNWRITTEN:
	case IOMAP_HOLE:
		if (!write) {
			ret = dax_load_hole(mapping, entry, vmf);
			goto finish_iomap;
		}
		/*FALLTHRU*/
	default:
		WARN_ON_ONCE(1);
		error = -EIO;
		break;
	}

 error_finish_iomap:
	ret = dax_fault_return(error);
 finish_iomap:
	if (ops->iomap_end) {
		int copied = PAGE_SIZE;

		if (ret & VM_FAULT_ERROR)
			copied = 0;
		/*
		 * The fault is done by now and there's no way back (other
		 * thread may be already happily using PTE we have installed).
		 * Just ignore error from ->iomap_end since we cannot do much
		 * with it.
		 */
		ops->iomap_end(inode, pos, PAGE_SIZE, copied, flags, &iomap);
	}
 unlock_entry:
	put_locked_mapping_entry(mapping, vmf->pgoff);
 out:
	trace_dax_pte_fault_done(inode, vmf, ret);
	return ret | major;
}

#ifdef CONFIG_FS_DAX_PMD
static vm_fault_t dax_pmd_load_hole(struct vm_fault *vmf, struct iomap *iomap,
		void *entry)
{
	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
	unsigned long pmd_addr = vmf->address & PMD_MASK;
	struct inode *inode = mapping->host;
	struct page *zero_page;
	void *ret = NULL;
	spinlock_t *ptl;
	pmd_t pmd_entry;
	pfn_t pfn;

	zero_page = mm_get_huge_zero_page(vmf->vma->vm_mm);

	if (unlikely(!zero_page))
		goto fallback;

	pfn = page_to_pfn_t(zero_page);
	ret = dax_insert_mapping_entry(mapping, vmf, entry, pfn,
			RADIX_DAX_PMD | RADIX_DAX_ZERO_PAGE, false);

	ptl = pmd_lock(vmf->vma->vm_mm, vmf->pmd);
	if (!pmd_none(*(vmf->pmd))) {
		spin_unlock(ptl);
		goto fallback;
	}

	pmd_entry = mk_pmd(zero_page, vmf->vma->vm_page_prot);
	pmd_entry = pmd_mkhuge(pmd_entry);
	set_pmd_at(vmf->vma->vm_mm, pmd_addr, vmf->pmd, pmd_entry);
	spin_unlock(ptl);
	trace_dax_pmd_load_hole(inode, vmf, zero_page, ret);
	return VM_FAULT_NOPAGE;

fallback:
	trace_dax_pmd_load_hole_fallback(inode, vmf, zero_page, ret);
	return VM_FAULT_FALLBACK;
=======
	while ((error = iomap_iter(&iter, ops)) > 0) {
		if (WARN_ON_ONCE(iomap_length(&iter) < PAGE_SIZE)) {
			iter.processed = -EIO;	/* fs corruption? */
			continue;
		}

		ret = dax_fault_iter(vmf, &iter, pfnp, &xas, &entry, false);
		if (ret != VM_FAULT_SIGBUS &&
		    (iter.iomap.flags & IOMAP_F_NEW)) {
			count_vm_event(PGMAJFAULT);
			count_memcg_event_mm(vmf->vma->vm_mm, PGMAJFAULT);
			ret |= VM_FAULT_MAJOR;
		}

		if (!(ret & VM_FAULT_ERROR))
			iter.processed = PAGE_SIZE;
	}

	if (iomap_errp)
		*iomap_errp = error;
	if (!ret && error)
		ret = dax_fault_return(error);

unlock_entry:
	dax_unlock_entry(&xas, entry);
out:
	trace_dax_pte_fault_done(iter.inode, vmf, ret);
	return ret;
}

#ifdef CONFIG_FS_DAX_PMD
static bool dax_fault_check_fallback(struct vm_fault *vmf, struct xa_state *xas,
		pgoff_t max_pgoff)
{
	unsigned long pmd_addr = vmf->address & PMD_MASK;
	bool write = vmf->flags & FAULT_FLAG_WRITE;

	/*
	 * Make sure that the faulting address's PMD offset (color) matches
	 * the PMD offset from the start of the file.  This is necessary so
	 * that a PMD range in the page table overlaps exactly with a PMD
	 * range in the page cache.
	 */
	if ((vmf->pgoff & PG_PMD_COLOUR) !=
	    ((vmf->address >> PAGE_SHIFT) & PG_PMD_COLOUR))
		return true;

	/* Fall back to PTEs if we're going to COW */
	if (write && !(vmf->vma->vm_flags & VM_SHARED))
		return true;

	/* If the PMD would extend outside the VMA */
	if (pmd_addr < vmf->vma->vm_start)
		return true;
	if ((pmd_addr + PMD_SIZE) > vmf->vma->vm_end)
		return true;

	/* If the PMD would extend beyond the file size */
	if ((xas->xa_index | PG_PMD_COLOUR) >= max_pgoff)
		return true;

	return false;
>>>>>>> upstream/android-13
}

static vm_fault_t dax_iomap_pmd_fault(struct vm_fault *vmf, pfn_t *pfnp,
			       const struct iomap_ops *ops)
{
<<<<<<< HEAD
	struct vm_area_struct *vma = vmf->vma;
	struct address_space *mapping = vma->vm_file->f_mapping;
	unsigned long pmd_addr = vmf->address & PMD_MASK;
	bool write = vmf->flags & FAULT_FLAG_WRITE;
	bool sync;
	unsigned int iomap_flags = (write ? IOMAP_WRITE : 0) | IOMAP_FAULT;
	struct inode *inode = mapping->host;
	vm_fault_t result = VM_FAULT_FALLBACK;
	struct iomap iomap = { 0 };
	pgoff_t max_pgoff, pgoff;
	void *entry;
	loff_t pos;
	int error;
	pfn_t pfn;
=======
	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
	XA_STATE_ORDER(xas, &mapping->i_pages, vmf->pgoff, PMD_ORDER);
	struct iomap_iter iter = {
		.inode		= mapping->host,
		.len		= PMD_SIZE,
		.flags		= IOMAP_FAULT,
	};
	vm_fault_t ret = VM_FAULT_FALLBACK;
	pgoff_t max_pgoff;
	void *entry;
	int error;

	if (vmf->flags & FAULT_FLAG_WRITE)
		iter.flags |= IOMAP_WRITE;
>>>>>>> upstream/android-13

	/*
	 * Check whether offset isn't beyond end of file now. Caller is
	 * supposed to hold locks serializing us with truncate / punch hole so
	 * this is a reliable test.
	 */
<<<<<<< HEAD
	pgoff = linear_page_index(vma, pmd_addr);
	max_pgoff = DIV_ROUND_UP(i_size_read(inode), PAGE_SIZE);

	trace_dax_pmd_fault(inode, vmf, max_pgoff, 0);

	/*
	 * Make sure that the faulting address's PMD offset (color) matches
	 * the PMD offset from the start of the file.  This is necessary so
	 * that a PMD range in the page table overlaps exactly with a PMD
	 * range in the radix tree.
	 */
	if ((vmf->pgoff & PG_PMD_COLOUR) !=
	    ((vmf->address >> PAGE_SHIFT) & PG_PMD_COLOUR))
		goto fallback;

	/* Fall back to PTEs if we're going to COW */
	if (write && !(vma->vm_flags & VM_SHARED))
		goto fallback;

	/* If the PMD would extend outside the VMA */
	if (pmd_addr < vma->vm_start)
		goto fallback;
	if ((pmd_addr + PMD_SIZE) > vma->vm_end)
		goto fallback;

	if (pgoff >= max_pgoff) {
		result = VM_FAULT_SIGBUS;
		goto out;
	}

	/* If the PMD would extend beyond the file size */
	if ((pgoff | PG_PMD_COLOUR) >= max_pgoff)
		goto fallback;

	/*
	 * grab_mapping_entry() will make sure we get a 2MiB empty entry, a
	 * 2MiB zero page entry or a DAX PMD.  If it can't (because a 4k page
	 * is already in the tree, for instance), it will return -EEXIST and
	 * we just fall back to 4k entries.
	 */
	entry = grab_mapping_entry(mapping, pgoff, RADIX_DAX_PMD);
	if (IS_ERR(entry))
		goto fallback;
=======
	max_pgoff = DIV_ROUND_UP(i_size_read(iter.inode), PAGE_SIZE);

	trace_dax_pmd_fault(iter.inode, vmf, max_pgoff, 0);

	if (xas.xa_index >= max_pgoff) {
		ret = VM_FAULT_SIGBUS;
		goto out;
	}

	if (dax_fault_check_fallback(vmf, &xas, max_pgoff))
		goto fallback;

	/*
	 * grab_mapping_entry() will make sure we get an empty PMD entry,
	 * a zero PMD entry or a DAX PMD.  If it can't (because a PTE
	 * entry is already in the array, for instance), it will return
	 * VM_FAULT_FALLBACK.
	 */
	entry = grab_mapping_entry(&xas, mapping, PMD_ORDER);
	if (xa_is_internal(entry)) {
		ret = xa_to_internal(entry);
		goto fallback;
	}
>>>>>>> upstream/android-13

	/*
	 * It is possible, particularly with mixed reads & writes to private
	 * mappings, that we have raced with a PTE fault that overlaps with
	 * the PMD we need to set up.  If so just return and the fault will be
	 * retried.
	 */
	if (!pmd_none(*vmf->pmd) && !pmd_trans_huge(*vmf->pmd) &&
			!pmd_devmap(*vmf->pmd)) {
<<<<<<< HEAD
		result = 0;
		goto unlock_entry;
	}

	/*
	 * Note that we don't use iomap_apply here.  We aren't doing I/O, only
	 * setting up a mapping, so really we're using iomap_begin() as a way
	 * to look up our filesystem block.
	 */
	pos = (loff_t)pgoff << PAGE_SHIFT;
	error = ops->iomap_begin(inode, pos, PMD_SIZE, iomap_flags, &iomap);
	if (error)
		goto unlock_entry;

	if (iomap.offset + iomap.length < pos + PMD_SIZE)
		goto finish_iomap;

	sync = dax_fault_is_synchronous(iomap_flags, vma, &iomap);

	switch (iomap.type) {
	case IOMAP_MAPPED:
		error = dax_iomap_pfn(&iomap, pos, PMD_SIZE, &pfn);
		if (error < 0)
			goto finish_iomap;

		entry = dax_insert_mapping_entry(mapping, vmf, entry, pfn,
						RADIX_DAX_PMD, write && !sync);

		/*
		 * If we are doing synchronous page fault and inode needs fsync,
		 * we can insert PMD into page tables only after that happens.
		 * Skip insertion for now and return the pfn so that caller can
		 * insert it after fsync is done.
		 */
		if (sync) {
			if (WARN_ON_ONCE(!pfnp))
				goto finish_iomap;
			*pfnp = pfn;
			result = VM_FAULT_NEEDDSYNC;
			goto finish_iomap;
		}

		trace_dax_pmd_insert_mapping(inode, vmf, PMD_SIZE, pfn, entry);
		result = vmf_insert_pfn_pmd(vmf, pfn, write);
		break;
	case IOMAP_UNWRITTEN:
	case IOMAP_HOLE:
		if (WARN_ON_ONCE(write))
			break;
		result = dax_pmd_load_hole(vmf, &iomap, entry);
		break;
	default:
		WARN_ON_ONCE(1);
		break;
	}

 finish_iomap:
	if (ops->iomap_end) {
		int copied = PMD_SIZE;

		if (result == VM_FAULT_FALLBACK)
			copied = 0;
		/*
		 * The fault is done by now and there's no way back (other
		 * thread may be already happily using PMD we have installed).
		 * Just ignore error from ->iomap_end since we cannot do much
		 * with it.
		 */
		ops->iomap_end(inode, pos, PMD_SIZE, copied, iomap_flags,
				&iomap);
	}
 unlock_entry:
	put_locked_mapping_entry(mapping, pgoff);
 fallback:
	if (result == VM_FAULT_FALLBACK) {
		split_huge_pmd(vma, vmf->pmd, vmf->address);
		count_vm_event(THP_FAULT_FALLBACK);
	}
out:
	trace_dax_pmd_fault_done(inode, vmf, max_pgoff, result);
	return result;
=======
		ret = 0;
		goto unlock_entry;
	}

	iter.pos = (loff_t)xas.xa_index << PAGE_SHIFT;
	while ((error = iomap_iter(&iter, ops)) > 0) {
		if (iomap_length(&iter) < PMD_SIZE)
			continue; /* actually breaks out of the loop */

		ret = dax_fault_iter(vmf, &iter, pfnp, &xas, &entry, true);
		if (ret != VM_FAULT_FALLBACK)
			iter.processed = PMD_SIZE;
	}

unlock_entry:
	dax_unlock_entry(&xas, entry);
fallback:
	if (ret == VM_FAULT_FALLBACK) {
		split_huge_pmd(vmf->vma, vmf->pmd, vmf->address);
		count_vm_event(THP_FAULT_FALLBACK);
	}
out:
	trace_dax_pmd_fault_done(iter.inode, vmf, max_pgoff, ret);
	return ret;
>>>>>>> upstream/android-13
}
#else
static vm_fault_t dax_iomap_pmd_fault(struct vm_fault *vmf, pfn_t *pfnp,
			       const struct iomap_ops *ops)
{
	return VM_FAULT_FALLBACK;
}
#endif /* CONFIG_FS_DAX_PMD */

/**
 * dax_iomap_fault - handle a page fault on a DAX file
 * @vmf: The description of the fault
 * @pe_size: Size of the page to fault in
 * @pfnp: PFN to insert for synchronous faults if fsync is required
 * @iomap_errp: Storage for detailed error code in case of error
 * @ops: Iomap ops passed from the file system
 *
 * When a page fault occurs, filesystems may call this helper in
 * their fault handler for DAX files. dax_iomap_fault() assumes the caller
 * has done all the necessary locking for page fault to proceed
 * successfully.
 */
vm_fault_t dax_iomap_fault(struct vm_fault *vmf, enum page_entry_size pe_size,
		    pfn_t *pfnp, int *iomap_errp, const struct iomap_ops *ops)
{
	switch (pe_size) {
	case PE_SIZE_PTE:
		return dax_iomap_pte_fault(vmf, pfnp, iomap_errp, ops);
	case PE_SIZE_PMD:
		return dax_iomap_pmd_fault(vmf, pfnp, ops);
	default:
		return VM_FAULT_FALLBACK;
	}
}
EXPORT_SYMBOL_GPL(dax_iomap_fault);

<<<<<<< HEAD
/**
 * dax_insert_pfn_mkwrite - insert PTE or PMD entry into page tables
 * @vmf: The description of the fault
 * @pe_size: Size of entry to be inserted
 * @pfn: PFN to insert
 *
 * This function inserts writeable PTE or PMD entry into page tables for mmaped
 * DAX file.  It takes care of marking corresponding radix tree entry as dirty
 * as well.
 */
static vm_fault_t dax_insert_pfn_mkwrite(struct vm_fault *vmf,
				  enum page_entry_size pe_size,
				  pfn_t pfn)
{
	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
	void *entry, **slot;
	pgoff_t index = vmf->pgoff;
	vm_fault_t ret;

	xa_lock_irq(&mapping->i_pages);
	entry = get_unlocked_mapping_entry(mapping, index, &slot);
	/* Did we race with someone splitting entry or so? */
	if (!entry ||
	    (pe_size == PE_SIZE_PTE && !dax_is_pte_entry(entry)) ||
	    (pe_size == PE_SIZE_PMD && !dax_is_pmd_entry(entry))) {
		put_unlocked_mapping_entry(mapping, index, entry);
		xa_unlock_irq(&mapping->i_pages);
=======
/*
 * dax_insert_pfn_mkwrite - insert PTE or PMD entry into page tables
 * @vmf: The description of the fault
 * @pfn: PFN to insert
 * @order: Order of entry to insert.
 *
 * This function inserts a writeable PTE or PMD entry into the page tables
 * for an mmaped DAX file.  It also marks the page cache entry as dirty.
 */
static vm_fault_t
dax_insert_pfn_mkwrite(struct vm_fault *vmf, pfn_t pfn, unsigned int order)
{
	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
	XA_STATE_ORDER(xas, &mapping->i_pages, vmf->pgoff, order);
	void *entry;
	vm_fault_t ret;

	xas_lock_irq(&xas);
	entry = get_unlocked_entry(&xas, order);
	/* Did we race with someone splitting entry or so? */
	if (!entry || dax_is_conflict(entry) ||
	    (order == 0 && !dax_is_pte_entry(entry))) {
		put_unlocked_entry(&xas, entry, WAKE_NEXT);
		xas_unlock_irq(&xas);
>>>>>>> upstream/android-13
		trace_dax_insert_pfn_mkwrite_no_entry(mapping->host, vmf,
						      VM_FAULT_NOPAGE);
		return VM_FAULT_NOPAGE;
	}
<<<<<<< HEAD
	radix_tree_tag_set(&mapping->i_pages, index, PAGECACHE_TAG_DIRTY);
	entry = lock_slot(mapping, slot);
	xa_unlock_irq(&mapping->i_pages);
	switch (pe_size) {
	case PE_SIZE_PTE:
		ret = vmf_insert_mixed_mkwrite(vmf->vma, vmf->address, pfn);
		break;
#ifdef CONFIG_FS_DAX_PMD
	case PE_SIZE_PMD:
		ret = vmf_insert_pfn_pmd(vmf, pfn, FAULT_FLAG_WRITE);
		break;
#endif
	default:
		ret = VM_FAULT_FALLBACK;
	}
	put_locked_mapping_entry(mapping, index);
=======
	xas_set_mark(&xas, PAGECACHE_TAG_DIRTY);
	dax_lock_entry(&xas, entry);
	xas_unlock_irq(&xas);
	if (order == 0)
		ret = vmf_insert_mixed_mkwrite(vmf->vma, vmf->address, pfn);
#ifdef CONFIG_FS_DAX_PMD
	else if (order == PMD_ORDER)
		ret = vmf_insert_pfn_pmd(vmf, pfn, FAULT_FLAG_WRITE);
#endif
	else
		ret = VM_FAULT_FALLBACK;
	dax_unlock_entry(&xas, entry);
>>>>>>> upstream/android-13
	trace_dax_insert_pfn_mkwrite(mapping->host, vmf, ret);
	return ret;
}

/**
 * dax_finish_sync_fault - finish synchronous page fault
 * @vmf: The description of the fault
 * @pe_size: Size of entry to be inserted
 * @pfn: PFN to insert
 *
 * This function ensures that the file range touched by the page fault is
 * stored persistently on the media and handles inserting of appropriate page
 * table entry.
 */
vm_fault_t dax_finish_sync_fault(struct vm_fault *vmf,
		enum page_entry_size pe_size, pfn_t pfn)
{
	int err;
	loff_t start = ((loff_t)vmf->pgoff) << PAGE_SHIFT;
<<<<<<< HEAD
	size_t len = 0;

	if (pe_size == PE_SIZE_PTE)
		len = PAGE_SIZE;
	else if (pe_size == PE_SIZE_PMD)
		len = PMD_SIZE;
	else
		WARN_ON_ONCE(1);
	err = vfs_fsync_range(vmf->vma->vm_file, start, start + len - 1, 1);
	if (err)
		return VM_FAULT_SIGBUS;
	return dax_insert_pfn_mkwrite(vmf, pe_size, pfn);
=======
	unsigned int order = pe_order(pe_size);
	size_t len = PAGE_SIZE << order;

	err = vfs_fsync_range(vmf->vma->vm_file, start, start + len - 1, 1);
	if (err)
		return VM_FAULT_SIGBUS;
	return dax_insert_pfn_mkwrite(vmf, pfn, order);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(dax_finish_sync_fault);
