// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2018 HUAWEI, Inc.
<<<<<<< HEAD
 *             http://www.huawei.com/
 * Created by Gao Xiang <gaoxiang25@huawei.com>
 */
#include "internal.h"
#include <linux/pagevec.h>
#include <linux/mm_inline.h>

struct page *erofs_allocpage(struct list_head *pool, gfp_t gfp, bool nofail)
{
	struct page *page;

	if (!list_empty(pool)) {
		page = lru_to_page(pool);
		DBG_BUGON(page_ref_count(page) != 1);
		list_del(&page->lru);
	} else {
		page = alloc_pages(gfp | (nofail ? __GFP_NOFAIL : 0), 0);
=======
 *             https://www.huawei.com/
 */
#include "internal.h"
#include <linux/pagevec.h>

struct page *erofs_allocpage(struct page **pagepool, gfp_t gfp)
{
	struct page *page = *pagepool;

	if (page) {
		DBG_BUGON(page_ref_count(page) != 1);
		*pagepool = (struct page *)page_private(page);
	} else {
		page = alloc_page(gfp);
>>>>>>> upstream/android-13
	}
	return page;
}

<<<<<<< HEAD
#if (EROFS_PCPUBUF_NR_PAGES > 0)
static struct {
	u8 data[PAGE_SIZE * EROFS_PCPUBUF_NR_PAGES];
} ____cacheline_aligned_in_smp erofs_pcpubuf[NR_CPUS];

void *erofs_get_pcpubuf(unsigned int pagenr)
{
	preempt_disable();
	return &erofs_pcpubuf[smp_processor_id()].data[pagenr * PAGE_SIZE];
}
#endif
=======
void erofs_release_pages(struct page **pagepool)
{
	while (*pagepool) {
		struct page *page = *pagepool;

		*pagepool = (struct page *)page_private(page);
		put_page(page);
	}
}
>>>>>>> upstream/android-13

#ifdef CONFIG_EROFS_FS_ZIP
/* global shrink count (for all mounted EROFS instances) */
static atomic_long_t erofs_global_shrink_cnt;

<<<<<<< HEAD
#define __erofs_workgroup_get(grp)	atomic_inc(&(grp)->refcount)
#define __erofs_workgroup_put(grp)	atomic_dec(&(grp)->refcount)

=======
>>>>>>> upstream/android-13
static int erofs_workgroup_get(struct erofs_workgroup *grp)
{
	int o;

repeat:
	o = erofs_wait_on_workgroup_freezed(grp);
	if (o <= 0)
		return -1;

	if (atomic_cmpxchg(&grp->refcount, o, o + 1) != o)
		goto repeat;

	/* decrease refcount paired by erofs_workgroup_put */
	if (o == 1)
		atomic_long_dec(&erofs_global_shrink_cnt);
	return 0;
}

struct erofs_workgroup *erofs_find_workgroup(struct super_block *sb,
<<<<<<< HEAD
					     pgoff_t index, bool *tag)
=======
					     pgoff_t index)
>>>>>>> upstream/android-13
{
	struct erofs_sb_info *sbi = EROFS_SB(sb);
	struct erofs_workgroup *grp;

repeat:
	rcu_read_lock();
<<<<<<< HEAD
	grp = radix_tree_lookup(&sbi->workstn_tree, index);
	if (grp) {
		*tag = radix_tree_exceptional_entry(grp);
	 	grp = (void *)((unsigned long)grp &
                       ~RADIX_TREE_EXCEPTIONAL_ENTRY);

=======
	grp = xa_load(&sbi->managed_pslots, index);
	if (grp) {
>>>>>>> upstream/android-13
		if (erofs_workgroup_get(grp)) {
			/* prefer to relax rcu read side */
			rcu_read_unlock();
			goto repeat;
		}

		DBG_BUGON(index != grp->index);
	}
	rcu_read_unlock();
	return grp;
}

<<<<<<< HEAD
int erofs_register_workgroup(struct super_block *sb,
			     struct erofs_workgroup *grp,
			     bool tag)
{
	struct erofs_sb_info *sbi;
	int err;

	/* grp shouldn't be broken or used before */
	if (atomic_read(&grp->refcount) != 1) {
		DBG_BUGON(1);
		return -EINVAL;
	}

	err = radix_tree_preload(GFP_NOFS);
	if (err)
		return err;

	sbi = EROFS_SB(sb);
	xa_lock(&sbi->workstn_tree);

	if (tag)
		grp = (void *)((unsigned long)grp |
			1UL << RADIX_TREE_EXCEPTIONAL_SHIFT);

	/*
	 * Bump up reference count before making this workgroup
	 * visible to other users in order to avoid potential UAF
 	 * without serialized by erofs_workstn_lock.
	 */
	__erofs_workgroup_get(grp);	

	err = radix_tree_insert(&sbi->workstn_tree, grp->index, grp);
	if (err)
		/*
		 * it's safe to decrease since the workgroup isn't visible
		 * and refcount >= 2 (cannot be freezed).
		 */
		__erofs_workgroup_put(grp);
 
	xa_unlock(&sbi->workstn_tree);
	radix_tree_preload_end();
	return err;
=======
struct erofs_workgroup *erofs_insert_workgroup(struct super_block *sb,
					       struct erofs_workgroup *grp)
{
	struct erofs_sb_info *const sbi = EROFS_SB(sb);
	struct erofs_workgroup *pre;

	/*
	 * Bump up a reference count before making this visible
	 * to others for the XArray in order to avoid potential
	 * UAF without serialized by xa_lock.
	 */
	atomic_inc(&grp->refcount);

repeat:
	xa_lock(&sbi->managed_pslots);
	pre = __xa_cmpxchg(&sbi->managed_pslots, grp->index,
			   NULL, grp, GFP_NOFS);
	if (pre) {
		if (xa_is_err(pre)) {
			pre = ERR_PTR(xa_err(pre));
		} else if (erofs_workgroup_get(pre)) {
			/* try to legitimize the current in-tree one */
			xa_unlock(&sbi->managed_pslots);
			cond_resched();
			goto repeat;
		}
		atomic_dec(&grp->refcount);
		grp = pre;
	}
	xa_unlock(&sbi->managed_pslots);
	return grp;
>>>>>>> upstream/android-13
}

static void  __erofs_workgroup_free(struct erofs_workgroup *grp)
{
	atomic_long_dec(&erofs_global_shrink_cnt);
	erofs_workgroup_free_rcu(grp);
}

int erofs_workgroup_put(struct erofs_workgroup *grp)
{
	int count = atomic_dec_return(&grp->refcount);

	if (count == 1)
		atomic_long_inc(&erofs_global_shrink_cnt);
	else if (!count)
<<<<<<< HEAD
		__erofs_workgroup_free(grp);	
	return count;
}

/* for cache-managed case, customized reclaim paths exist */
static void erofs_workgroup_unfreeze_final(struct erofs_workgroup *grp)
{
	erofs_workgroup_unfreeze(grp, 0);
	__erofs_workgroup_free(grp);
}

bool erofs_try_to_release_workgroup(struct erofs_sb_info *sbi,
		   struct erofs_workgroup *grp,
 		   bool cleanup)
{
	/*
 	 * for managed cache enabled, the refcount of workgroups
 	 * themselves could be < 0 (freezed). So there is no guarantee
 	 * that all refcount > 0 if managed cache is enabled.
 	 */
=======
		__erofs_workgroup_free(grp);
	return count;
}

static bool erofs_try_to_release_workgroup(struct erofs_sb_info *sbi,
					   struct erofs_workgroup *grp)
{
	/*
	 * If managed cache is on, refcount of workgroups
	 * themselves could be < 0 (freezed). In other words,
	 * there is no guarantee that all refcounts > 0.
	 */
>>>>>>> upstream/android-13
	if (!erofs_workgroup_try_to_freeze(grp, 1))
		return false;

	/*
<<<<<<< HEAD
 	 * note that all cached pages should be unlinked
 	 * before delete it from the radix tree.
 	 * Otherwise some cached pages of an orphan old workgroup
 	 * could be still linked after the new one is available.
 	 */
=======
	 * Note that all cached pages should be unattached
	 * before deleted from the XArray. Otherwise some
	 * cached pages could be still attached to the orphan
	 * old workgroup when the new one is available in the tree.
	 */
>>>>>>> upstream/android-13
	if (erofs_try_to_free_all_cached_pages(sbi, grp)) {
		erofs_workgroup_unfreeze(grp, 1);
		return false;
	}

	/*
<<<<<<< HEAD
 	 * it is impossible to fail after the workgroup is freezed,
	 * however in order to avoid some race conditions, add a
 	 * DBG_BUGON to observe this in advance.
 	 */
	DBG_BUGON((void *)((unsigned long)radix_tree_delete(&sbi->workstn_tree,
			  grp->index) & ~RADIX_TREE_EXCEPTIONAL_ENTRY) != grp);

	/*
 	 * if managed cache is enable, the last refcount
	 * should indicate the related workstation.
	 */
	erofs_workgroup_unfreeze_final(grp);
	return true;
}


static unsigned long erofs_shrink_workstation(struct erofs_sb_info *sbi,
					      unsigned long nr_shrink,
					      bool cleanup)
{
	pgoff_t first_index = 0;
	void *batch[PAGEVEC_SIZE];
	unsigned int freed = 0;

	int i, found;
repeat:
	xa_lock(&sbi->workstn_tree);

	found = radix_tree_gang_lookup(&sbi->workstn_tree,
				       batch, first_index, PAGEVEC_SIZE);

	for (i = 0; i < found; ++i) {
                struct erofs_workgroup *grp = (void *)
                       ((unsigned long)batch[i] &
                               ~RADIX_TREE_EXCEPTIONAL_ENTRY);

		first_index = grp->index + 1;

		/* try to shrink each valid workgroup */
		if (!erofs_try_to_release_workgroup(sbi, grp, cleanup))
			continue;

		++freed;
		if (!--nr_shrink)
			break;
	}
	xa_unlock(&sbi->workstn_tree);

	if (i && nr_shrink)
		goto repeat;
=======
	 * It's impossible to fail after the workgroup is freezed,
	 * however in order to avoid some race conditions, add a
	 * DBG_BUGON to observe this in advance.
	 */
	DBG_BUGON(__xa_erase(&sbi->managed_pslots, grp->index) != grp);

	/* last refcount should be connected with its managed pslot.  */
	erofs_workgroup_unfreeze(grp, 0);
	__erofs_workgroup_free(grp);
	return true;
}

static unsigned long erofs_shrink_workstation(struct erofs_sb_info *sbi,
					      unsigned long nr_shrink)
{
	struct erofs_workgroup *grp;
	unsigned int freed = 0;
	unsigned long index;

	xa_lock(&sbi->managed_pslots);
	xa_for_each(&sbi->managed_pslots, index, grp) {
		/* try to shrink each valid workgroup */
		if (!erofs_try_to_release_workgroup(sbi, grp))
			continue;
		xa_unlock(&sbi->managed_pslots);

		++freed;
		if (!--nr_shrink)
			return freed;
		xa_lock(&sbi->managed_pslots);
	}
	xa_unlock(&sbi->managed_pslots);
>>>>>>> upstream/android-13
	return freed;
}

/* protected by 'erofs_sb_list_lock' */
static unsigned int shrinker_run_no;

/* protects the mounted 'erofs_sb_list' */
static DEFINE_SPINLOCK(erofs_sb_list_lock);
static LIST_HEAD(erofs_sb_list);

void erofs_shrinker_register(struct super_block *sb)
{
	struct erofs_sb_info *sbi = EROFS_SB(sb);

	mutex_init(&sbi->umount_mutex);

	spin_lock(&erofs_sb_list_lock);
	list_add(&sbi->list, &erofs_sb_list);
	spin_unlock(&erofs_sb_list_lock);
}

void erofs_shrinker_unregister(struct super_block *sb)
{
	struct erofs_sb_info *const sbi = EROFS_SB(sb);

	mutex_lock(&sbi->umount_mutex);
<<<<<<< HEAD
	erofs_shrink_workstation(sbi, ~0UL, true);
=======
	/* clean up all remaining workgroups in memory */
	erofs_shrink_workstation(sbi, ~0UL);
>>>>>>> upstream/android-13

	spin_lock(&erofs_sb_list_lock);
	list_del(&sbi->list);
	spin_unlock(&erofs_sb_list_lock);
	mutex_unlock(&sbi->umount_mutex);
}

static unsigned long erofs_shrink_count(struct shrinker *shrink,
					struct shrink_control *sc)
{
	return atomic_long_read(&erofs_global_shrink_cnt);
}

static unsigned long erofs_shrink_scan(struct shrinker *shrink,
				       struct shrink_control *sc)
{
	struct erofs_sb_info *sbi;
	struct list_head *p;

	unsigned long nr = sc->nr_to_scan;
	unsigned int run_no;
	unsigned long freed = 0;

	spin_lock(&erofs_sb_list_lock);
	do {
		run_no = ++shrinker_run_no;
	} while (run_no == 0);

	/* Iterate over all mounted superblocks and try to shrink them */
	p = erofs_sb_list.next;
	while (p != &erofs_sb_list) {
		sbi = list_entry(p, struct erofs_sb_info, list);

		/*
		 * We move the ones we do to the end of the list, so we stop
		 * when we see one we have already done.
		 */
		if (sbi->shrinker_run_no == run_no)
			break;

		if (!mutex_trylock(&sbi->umount_mutex)) {
			p = p->next;
			continue;
		}

		spin_unlock(&erofs_sb_list_lock);
		sbi->shrinker_run_no = run_no;

<<<<<<< HEAD
		freed += erofs_shrink_workstation(sbi, nr, false);
=======
		freed += erofs_shrink_workstation(sbi, nr - freed);
>>>>>>> upstream/android-13

		spin_lock(&erofs_sb_list_lock);
		/* Get the next list element before we move this one */
		p = p->next;

		/*
		 * Move this one to the end of the list to provide some
		 * fairness.
		 */
		list_move_tail(&sbi->list, &erofs_sb_list);
		mutex_unlock(&sbi->umount_mutex);

		if (freed >= nr)
			break;
	}
	spin_unlock(&erofs_sb_list_lock);
	return freed;
}

static struct shrinker erofs_shrinker_info = {
	.scan_objects = erofs_shrink_scan,
	.count_objects = erofs_shrink_count,
	.seeks = DEFAULT_SEEKS,
};

int __init erofs_init_shrinker(void)
{
	return register_shrinker(&erofs_shrinker_info);
}

void erofs_exit_shrinker(void)
{
	unregister_shrinker(&erofs_shrinker_info);
}
#endif	/* !CONFIG_EROFS_FS_ZIP */
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
