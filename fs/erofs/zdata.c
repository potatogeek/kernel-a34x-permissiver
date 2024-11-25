// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2018 HUAWEI, Inc.
<<<<<<< HEAD
 *             http://www.huawei.com/
 * Created by Gao Xiang <gaoxiang25@huawei.com>
=======
 *             https://www.huawei.com/
>>>>>>> upstream/android-13
 */
#include "zdata.h"
#include "compress.h"
#include <linux/prefetch.h>
<<<<<<< HEAD
#include <linux/mm_inline.h>
=======
>>>>>>> upstream/android-13

#include <trace/events/erofs.h>

/*
<<<<<<< HEAD
 * a compressed_pages[] placeholder in order to avoid
 * being filled with file pages for in-place decompression.
 */
#define PAGE_UNALLOCATED     ((void *)0x5F0E4B1D)
=======
 * since pclustersize is variable for big pcluster feature, introduce slab
 * pools implementation for different pcluster sizes.
 */
struct z_erofs_pcluster_slab {
	struct kmem_cache *slab;
	unsigned int maxpages;
	char name[48];
};

#define _PCLP(n) { .maxpages = n }

static struct z_erofs_pcluster_slab pcluster_pool[] __read_mostly = {
	_PCLP(1), _PCLP(4), _PCLP(16), _PCLP(64), _PCLP(128),
	_PCLP(Z_EROFS_PCLUSTER_MAX_PAGES)
};

static void z_erofs_destroy_pcluster_pool(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(pcluster_pool); ++i) {
		if (!pcluster_pool[i].slab)
			continue;
		kmem_cache_destroy(pcluster_pool[i].slab);
		pcluster_pool[i].slab = NULL;
	}
}

static int z_erofs_create_pcluster_pool(void)
{
	struct z_erofs_pcluster_slab *pcs;
	struct z_erofs_pcluster *a;
	unsigned int size;

	for (pcs = pcluster_pool;
	     pcs < pcluster_pool + ARRAY_SIZE(pcluster_pool); ++pcs) {
		size = struct_size(a, compressed_pages, pcs->maxpages);

		sprintf(pcs->name, "erofs_pcluster-%u", pcs->maxpages);
		pcs->slab = kmem_cache_create(pcs->name, size, 0,
					      SLAB_RECLAIM_ACCOUNT, NULL);
		if (pcs->slab)
			continue;

		z_erofs_destroy_pcluster_pool();
		return -ENOMEM;
	}
	return 0;
}

static struct z_erofs_pcluster *z_erofs_alloc_pcluster(unsigned int nrpages)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(pcluster_pool); ++i) {
		struct z_erofs_pcluster_slab *pcs = pcluster_pool + i;
		struct z_erofs_pcluster *pcl;

		if (nrpages > pcs->maxpages)
			continue;

		pcl = kmem_cache_zalloc(pcs->slab, GFP_NOFS);
		if (!pcl)
			return ERR_PTR(-ENOMEM);
		pcl->pclusterpages = nrpages;
		return pcl;
	}
	return ERR_PTR(-EINVAL);
}

static void z_erofs_free_pcluster(struct z_erofs_pcluster *pcl)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(pcluster_pool); ++i) {
		struct z_erofs_pcluster_slab *pcs = pcluster_pool + i;

		if (pcl->pclusterpages > pcs->maxpages)
			continue;

		kmem_cache_free(pcs->slab, pcl);
		return;
	}
	DBG_BUGON(1);
}
>>>>>>> upstream/android-13

/* how to allocate cached pages for a pcluster */
enum z_erofs_cache_alloctype {
	DONTALLOC,	/* don't allocate any cached pages */
<<<<<<< HEAD
	DELAYEDALLOC,	/* delayed allocation (at the time of submitting io) */
=======
	/*
	 * try to use cached I/O if page allocation succeeds or fallback
	 * to in-place I/O instead to avoid any direct reclaim.
	 */
	TRYALLOC,
>>>>>>> upstream/android-13
};

/*
 * tagged pointer with 1-bit tag for all compressed pages
 * tag 0 - the page is just found with an extra page reference
 */
typedef tagptr1_t compressed_page_t;

#define tag_compressed_page_justfound(page) \
	tagptr_fold(compressed_page_t, page, 1)

static struct workqueue_struct *z_erofs_workqueue __read_mostly;
<<<<<<< HEAD
static struct kmem_cache *pcluster_cachep __read_mostly;
=======
>>>>>>> upstream/android-13

void z_erofs_exit_zip_subsystem(void)
{
	destroy_workqueue(z_erofs_workqueue);
<<<<<<< HEAD
	kmem_cache_destroy(pcluster_cachep);
=======
	z_erofs_destroy_pcluster_pool();
>>>>>>> upstream/android-13
}

static inline int z_erofs_init_workqueue(void)
{
	const unsigned int onlinecpus = num_possible_cpus();
<<<<<<< HEAD
	const unsigned int flags = WQ_UNBOUND | WQ_HIGHPRI | WQ_CPU_INTENSIVE;
=======
>>>>>>> upstream/android-13

	/*
	 * no need to spawn too many threads, limiting threads could minimum
	 * scheduling overhead, perhaps per-CPU threads should be better?
	 */
<<<<<<< HEAD
	z_erofs_workqueue = alloc_workqueue("erofs_unzipd", flags,
=======
	z_erofs_workqueue = alloc_workqueue("erofs_unzipd",
					    WQ_UNBOUND | WQ_HIGHPRI,
>>>>>>> upstream/android-13
					    onlinecpus + onlinecpus / 4);
	return z_erofs_workqueue ? 0 : -ENOMEM;
}

<<<<<<< HEAD
static void z_erofs_pcluster_init_once(void *ptr)
{
	struct z_erofs_pcluster *pcl = ptr;
	struct z_erofs_collection *cl = z_erofs_primarycollection(pcl);
	unsigned int i;

	mutex_init(&cl->lock);
	cl->nr_pages = 0;
	cl->vcnt = 0;
	for (i = 0; i < Z_EROFS_CLUSTER_MAX_PAGES; ++i)
		pcl->compressed_pages[i] = NULL;
}

static void z_erofs_pcluster_init_always(struct z_erofs_pcluster *pcl)
{
	struct z_erofs_collection *cl = z_erofs_primarycollection(pcl);

	atomic_set(&pcl->obj.refcount, 1);

	DBG_BUGON(cl->nr_pages);
	DBG_BUGON(cl->vcnt);
}

int __init z_erofs_init_zip_subsystem(void)
{
	pcluster_cachep = kmem_cache_create("erofs_compress",
					    Z_EROFS_WORKGROUP_SIZE, 0,
					    SLAB_RECLAIM_ACCOUNT,
					    z_erofs_pcluster_init_once);
	if (pcluster_cachep) {
		if (!z_erofs_init_workqueue())
			return 0;

		kmem_cache_destroy(pcluster_cachep);
	}
	return -ENOMEM;
=======
int __init z_erofs_init_zip_subsystem(void)
{
	int err = z_erofs_create_pcluster_pool();

	if (err)
		return err;
	err = z_erofs_init_workqueue();
	if (err)
		z_erofs_destroy_pcluster_pool();
	return err;
>>>>>>> upstream/android-13
}

enum z_erofs_collectmode {
	COLLECT_SECONDARY,
	COLLECT_PRIMARY,
	/*
	 * The current collection was the tail of an exist chain, in addition
	 * that the previous processed chained collections are all decided to
	 * be hooked up to it.
	 * A new chain will be created for the remaining collections which are
	 * not processed yet, therefore different from COLLECT_PRIMARY_FOLLOWED,
	 * the next collection cannot reuse the whole page safely in
	 * the following scenario:
	 *  ________________________________________________________________
	 * |      tail (partial) page     |       head (partial) page       |
	 * |   (belongs to the next cl)   |   (belongs to the current cl)   |
	 * |_______PRIMARY_FOLLOWED_______|________PRIMARY_HOOKED___________|
	 */
	COLLECT_PRIMARY_HOOKED,
<<<<<<< HEAD
=======
	/*
	 * a weak form of COLLECT_PRIMARY_FOLLOWED, the difference is that it
	 * could be dispatched into bypass queue later due to uptodated managed
	 * pages. All related online pages cannot be reused for inplace I/O (or
	 * pagevec) since it can be directly decoded without I/O submission.
	 */
>>>>>>> upstream/android-13
	COLLECT_PRIMARY_FOLLOWED_NOINPLACE,
	/*
	 * The current collection has been linked with the owned chain, and
	 * could also be linked with the remaining collections, which means
	 * if the processing page is the tail page of the collection, thus
	 * the current collection can safely use the whole page (since
	 * the previous collection is under control) for in-place I/O, as
	 * illustrated below:
	 *  ________________________________________________________________
	 * |  tail (partial) page |          head (partial) page           |
	 * |  (of the current cl) |      (of the previous collection)      |
	 * |  PRIMARY_FOLLOWED or |                                        |
	 * |_____PRIMARY_HOOKED___|____________PRIMARY_FOLLOWED____________|
	 *
	 * [  (*) the above page can be used as inplace I/O.               ]
	 */
	COLLECT_PRIMARY_FOLLOWED,
};

struct z_erofs_collector {
	struct z_erofs_pagevec_ctor vector;

	struct z_erofs_pcluster *pcl, *tailpcl;
	struct z_erofs_collection *cl;
<<<<<<< HEAD
	struct page **compressedpages;
=======
	/* a pointer used to pick up inplace I/O pages */
	struct page **icpage_ptr;
>>>>>>> upstream/android-13
	z_erofs_next_pcluster_t owned_head;

	enum z_erofs_collectmode mode;
};

struct z_erofs_decompress_frontend {
	struct inode *const inode;

	struct z_erofs_collector clt;
	struct erofs_map_blocks map;

<<<<<<< HEAD
=======
	bool readahead;
>>>>>>> upstream/android-13
	/* used for applying cache strategy on the fly */
	bool backmost;
	erofs_off_t headoffset;
};

#define COLLECTOR_INIT() { \
	.owned_head = Z_EROFS_PCLUSTER_TAIL, \
	.mode = COLLECT_PRIMARY_FOLLOWED }

#define DECOMPRESS_FRONTEND_INIT(__i) { \
	.inode = __i, .clt = COLLECTOR_INIT(), \
	.backmost = true, }

static struct page *z_pagemap_global[Z_EROFS_VMAP_GLOBAL_PAGES];
static DEFINE_MUTEX(z_pagemap_global_lock);

static void preload_compressed_pages(struct z_erofs_collector *clt,
				     struct address_space *mc,
				     enum z_erofs_cache_alloctype type,
<<<<<<< HEAD
				     struct list_head *pagepool)
{
	const struct z_erofs_pcluster *pcl = clt->pcl;
	const unsigned int clusterpages = BIT(pcl->clusterbits);
	struct page **pages = clt->compressedpages;
	pgoff_t index = pcl->obj.index + (pages - pcl->compressed_pages);
	bool standalone = true;
=======
				     struct page **pagepool)
{
	struct z_erofs_pcluster *pcl = clt->pcl;
	bool standalone = true;
	gfp_t gfp = (mapping_gfp_mask(mc) & ~__GFP_DIRECT_RECLAIM) |
			__GFP_NOMEMALLOC | __GFP_NORETRY | __GFP_NOWARN;
	struct page **pages;
	pgoff_t index;
>>>>>>> upstream/android-13

	if (clt->mode < COLLECT_PRIMARY_FOLLOWED)
		return;

<<<<<<< HEAD
	for (; pages < pcl->compressed_pages + clusterpages; ++pages) {
		struct page *page;
		compressed_page_t t;
=======
	pages = pcl->compressed_pages;
	index = pcl->obj.index;
	for (; index < pcl->obj.index + pcl->pclusterpages; ++index, ++pages) {
		struct page *page;
		compressed_page_t t;
		struct page *newpage = NULL;
>>>>>>> upstream/android-13

		/* the compressed page was loaded before */
		if (READ_ONCE(*pages))
			continue;

		page = find_get_page(mc, index);

		if (page) {
			t = tag_compressed_page_justfound(page);
<<<<<<< HEAD
		} else if (type == DELAYEDALLOC) {
			t = tagptr_init(compressed_page_t, PAGE_UNALLOCATED);
		} else {	/* DONTALLOC */
			if (standalone)
				clt->compressedpages = pages;
			standalone = false;
			continue;
=======
		} else {
			/* I/O is needed, no possible to decompress directly */
			standalone = false;
			switch (type) {
			case TRYALLOC:
				newpage = erofs_allocpage(pagepool, gfp);
				if (!newpage)
					continue;
				set_page_private(newpage,
						 Z_EROFS_PREALLOCATED_PAGE);
				t = tag_compressed_page_justfound(newpage);
				break;
			default:        /* DONTALLOC */
				continue;
			}
>>>>>>> upstream/android-13
		}

		if (!cmpxchg_relaxed(pages, NULL, tagptr_cast_ptr(t)))
			continue;

		if (page)
			put_page(page);
<<<<<<< HEAD
	}

	if (standalone)		/* downgrade to PRIMARY_FOLLOWED_NOINPLACE */
=======
		else if (newpage)
			erofs_pagepool_add(pagepool, newpage);
	}

	/*
	 * don't do inplace I/O if all compressed pages are available in
	 * managed cache since it can be moved to the bypass queue instead.
	 */
	if (standalone)
>>>>>>> upstream/android-13
		clt->mode = COLLECT_PRIMARY_FOLLOWED_NOINPLACE;
}

/* called by erofs_shrinker to get rid of all compressed_pages */
int erofs_try_to_free_all_cached_pages(struct erofs_sb_info *sbi,
				       struct erofs_workgroup *grp)
{
	struct z_erofs_pcluster *const pcl =
		container_of(grp, struct z_erofs_pcluster, obj);
<<<<<<< HEAD
	struct address_space *const mapping = MNGD_MAPPING(sbi);
	const unsigned int clusterpages = BIT(pcl->clusterbits);
=======
>>>>>>> upstream/android-13
	int i;

	/*
	 * refcount of workgroup is now freezed as 1,
	 * therefore no need to worry about available decompression users.
	 */
<<<<<<< HEAD
	for (i = 0; i < clusterpages; ++i) {
=======
	for (i = 0; i < pcl->pclusterpages; ++i) {
>>>>>>> upstream/android-13
		struct page *page = pcl->compressed_pages[i];

		if (!page)
			continue;

		/* block other users from reclaiming or migrating the page */
		if (!trylock_page(page))
			return -EBUSY;

<<<<<<< HEAD
		if (page->mapping != mapping)
=======
		if (!erofs_page_is_managed(sbi, page))
>>>>>>> upstream/android-13
			continue;

		/* barrier is implied in the following 'unlock_page' */
		WRITE_ONCE(pcl->compressed_pages[i], NULL);
<<<<<<< HEAD
		set_page_private(page, 0);
		ClearPagePrivate(page);

		unlock_page(page);
		put_page(page);
=======
		detach_page_private(page);
		unlock_page(page);
>>>>>>> upstream/android-13
	}
	return 0;
}

<<<<<<< HEAD
int erofs_try_to_free_cached_page(struct address_space *mapping,
				  struct page *page)
{
	struct z_erofs_pcluster *const pcl = (void *)page_private(page);
	const unsigned int clusterpages = BIT(pcl->clusterbits);
=======
int erofs_try_to_free_cached_page(struct page *page)
{
	struct z_erofs_pcluster *const pcl = (void *)page_private(page);
>>>>>>> upstream/android-13
	int ret = 0;	/* 0 - busy */

	if (erofs_workgroup_try_to_freeze(&pcl->obj, 1)) {
		unsigned int i;

<<<<<<< HEAD
		for (i = 0; i < clusterpages; ++i) {
=======
		for (i = 0; i < pcl->pclusterpages; ++i) {
>>>>>>> upstream/android-13
			if (pcl->compressed_pages[i] == page) {
				WRITE_ONCE(pcl->compressed_pages[i], NULL);
				ret = 1;
				break;
			}
		}
		erofs_workgroup_unfreeze(&pcl->obj, 1);

<<<<<<< HEAD
		if (ret) {
			ClearPagePrivate(page);
			put_page(page);
		}
=======
		if (ret)
			detach_page_private(page);
>>>>>>> upstream/android-13
	}
	return ret;
}

/* page_type must be Z_EROFS_PAGE_TYPE_EXCLUSIVE */
<<<<<<< HEAD
static inline bool z_erofs_try_inplace_io(struct z_erofs_collector *clt,
					  struct page *page)
{
	struct z_erofs_pcluster *const pcl = clt->pcl;
	const unsigned int clusterpages = BIT(pcl->clusterbits);

	while (clt->compressedpages < pcl->compressed_pages + clusterpages) {
		if (!cmpxchg(clt->compressedpages++, NULL, page))
			return true;
	}
=======
static bool z_erofs_try_inplace_io(struct z_erofs_collector *clt,
				   struct page *page)
{
	struct z_erofs_pcluster *const pcl = clt->pcl;

	while (clt->icpage_ptr > pcl->compressed_pages)
		if (!cmpxchg(--clt->icpage_ptr, NULL, page))
			return true;
>>>>>>> upstream/android-13
	return false;
}

/* callers must be with collection lock held */
static int z_erofs_attach_page(struct z_erofs_collector *clt,
<<<<<<< HEAD
			       struct page *page,
			       enum z_erofs_page_type type)
{
	int ret;
	bool occupied;
=======
			       struct page *page, enum z_erofs_page_type type,
			       bool pvec_safereuse)
{
	int ret;
>>>>>>> upstream/android-13

	/* give priority for inplaceio */
	if (clt->mode >= COLLECT_PRIMARY &&
	    type == Z_EROFS_PAGE_TYPE_EXCLUSIVE &&
	    z_erofs_try_inplace_io(clt, page))
		return 0;

<<<<<<< HEAD
	ret = z_erofs_pagevec_enqueue(&clt->vector,
				      page, type, &occupied);
	clt->cl->vcnt += (unsigned int)ret;

	return ret ? 0 : -EAGAIN;
}

static enum z_erofs_collectmode
try_to_claim_pcluster(struct z_erofs_pcluster *pcl,
		      z_erofs_next_pcluster_t *owned_head)
{
	/* let's claim these following types of pclusters */
retry:
	if (pcl->next == Z_EROFS_PCLUSTER_NIL) {
		/* type 1, nil pcluster */
		if (cmpxchg(&pcl->next, Z_EROFS_PCLUSTER_NIL,
			    *owned_head) != Z_EROFS_PCLUSTER_NIL)
			goto retry;

		*owned_head = &pcl->next;
		/* lucky, I am the followee :) */
		return COLLECT_PRIMARY_FOLLOWED;
	} else if (pcl->next == Z_EROFS_PCLUSTER_TAIL) {
		/*
		 * type 2, link to the end of a existing open chain,
		 * be careful that its submission itself is governed
		 * by the original owned chain.
		 */
		if (cmpxchg(&pcl->next, Z_EROFS_PCLUSTER_TAIL,
			    *owned_head) != Z_EROFS_PCLUSTER_TAIL)
			goto retry;
		*owned_head = Z_EROFS_PCLUSTER_TAIL;
		return COLLECT_PRIMARY_HOOKED;
	}
	return COLLECT_PRIMARY;	/* :( better luck next time */
}

static struct z_erofs_collection *cllookup(struct z_erofs_collector *clt,
					   struct inode *inode,
					   struct erofs_map_blocks *map)
{
	struct erofs_workgroup *grp;
	struct z_erofs_pcluster *pcl;
	struct z_erofs_collection *cl;
	unsigned int length;
	bool tag;

	grp = erofs_find_workgroup(inode->i_sb, map->m_pa >> PAGE_SHIFT, &tag);
	if (!grp)
		return NULL;

	pcl = container_of(grp, struct z_erofs_pcluster, obj);
	if (clt->owned_head == &pcl->next || pcl == clt->tailpcl) {
		DBG_BUGON(1);
		erofs_workgroup_put(grp);
		return ERR_PTR(-EFSCORRUPTED);
=======
	ret = z_erofs_pagevec_enqueue(&clt->vector, page, type,
				      pvec_safereuse);
	clt->cl->vcnt += (unsigned int)ret;
	return ret ? 0 : -EAGAIN;
}

static void z_erofs_try_to_claim_pcluster(struct z_erofs_collector *clt)
{
	struct z_erofs_pcluster *pcl = clt->pcl;
	z_erofs_next_pcluster_t *owned_head = &clt->owned_head;

	/* type 1, nil pcluster (this pcluster doesn't belong to any chain.) */
	if (cmpxchg(&pcl->next, Z_EROFS_PCLUSTER_NIL,
		    *owned_head) == Z_EROFS_PCLUSTER_NIL) {
		*owned_head = &pcl->next;
		/* so we can attach this pcluster to our submission chain. */
		clt->mode = COLLECT_PRIMARY_FOLLOWED;
		return;
	}

	/*
	 * type 2, link to the end of an existing open chain, be careful
	 * that its submission is controlled by the original attached chain.
	 */
	if (cmpxchg(&pcl->next, Z_EROFS_PCLUSTER_TAIL,
		    *owned_head) == Z_EROFS_PCLUSTER_TAIL) {
		*owned_head = Z_EROFS_PCLUSTER_TAIL;
		clt->mode = COLLECT_PRIMARY_HOOKED;
		clt->tailpcl = NULL;
		return;
	}
	/* type 3, it belongs to a chain, but it isn't the end of the chain */
	clt->mode = COLLECT_PRIMARY;
}

static int z_erofs_lookup_collection(struct z_erofs_collector *clt,
				     struct inode *inode,
				     struct erofs_map_blocks *map)
{
	struct z_erofs_pcluster *pcl = clt->pcl;
	struct z_erofs_collection *cl;
	unsigned int length;

	/* to avoid unexpected loop formed by corrupted images */
	if (clt->owned_head == &pcl->next || pcl == clt->tailpcl) {
		DBG_BUGON(1);
		return -EFSCORRUPTED;
>>>>>>> upstream/android-13
	}

	cl = z_erofs_primarycollection(pcl);
	if (cl->pageofs != (map->m_la & ~PAGE_MASK)) {
		DBG_BUGON(1);
<<<<<<< HEAD
		erofs_workgroup_put(grp);
		return ERR_PTR(-EFSCORRUPTED);
=======
		return -EFSCORRUPTED;
>>>>>>> upstream/android-13
	}

	length = READ_ONCE(pcl->length);
	if (length & Z_EROFS_PCLUSTER_FULL_LENGTH) {
		if ((map->m_llen << Z_EROFS_PCLUSTER_LENGTH_BIT) > length) {
			DBG_BUGON(1);
<<<<<<< HEAD
			erofs_workgroup_put(grp);
			return ERR_PTR(-EFSCORRUPTED);
=======
			return -EFSCORRUPTED;
>>>>>>> upstream/android-13
		}
	} else {
		unsigned int llen = map->m_llen << Z_EROFS_PCLUSTER_LENGTH_BIT;

		if (map->m_flags & EROFS_MAP_FULL_MAPPED)
			llen |= Z_EROFS_PCLUSTER_FULL_LENGTH;

		while (llen > length &&
		       length != cmpxchg_relaxed(&pcl->length, length, llen)) {
			cpu_relax();
			length = READ_ONCE(pcl->length);
		}
	}
	mutex_lock(&cl->lock);
	/* used to check tail merging loop due to corrupted images */
	if (clt->owned_head == Z_EROFS_PCLUSTER_TAIL)
		clt->tailpcl = pcl;
<<<<<<< HEAD
	clt->mode = try_to_claim_pcluster(pcl, &clt->owned_head);
	/* clean tailpcl if the current owned_head is Z_EROFS_PCLUSTER_TAIL */
	if (clt->owned_head == Z_EROFS_PCLUSTER_TAIL)
		clt->tailpcl = NULL;
	clt->pcl = pcl;
	clt->cl = cl;
	return cl;
}

static struct z_erofs_collection *clregister(struct z_erofs_collector *clt,
					     struct inode *inode,
					     struct erofs_map_blocks *map)
{
	struct z_erofs_pcluster *pcl;
	struct z_erofs_collection *cl;
	int err;

	/* no available workgroup, let's allocate one */
	pcl = kmem_cache_alloc(pcluster_cachep, GFP_NOFS);
	if (!pcl)
		return ERR_PTR(-ENOMEM);

	z_erofs_pcluster_init_always(pcl);
	pcl->obj.index = map->m_pa >> PAGE_SHIFT;

=======

	z_erofs_try_to_claim_pcluster(clt);
	clt->cl = cl;
	return 0;
}

static int z_erofs_register_collection(struct z_erofs_collector *clt,
				       struct inode *inode,
				       struct erofs_map_blocks *map)
{
	struct z_erofs_pcluster *pcl;
	struct z_erofs_collection *cl;
	struct erofs_workgroup *grp;
	int err;

	if (!(map->m_flags & EROFS_MAP_ENCODED)) {
		DBG_BUGON(1);
		return -EFSCORRUPTED;
	}

	/* no available pcluster, let's allocate one */
	pcl = z_erofs_alloc_pcluster(map->m_plen >> PAGE_SHIFT);
	if (IS_ERR(pcl))
		return PTR_ERR(pcl);

	atomic_set(&pcl->obj.refcount, 1);
	pcl->obj.index = map->m_pa >> PAGE_SHIFT;
	pcl->algorithmformat = map->m_algorithmformat;
>>>>>>> upstream/android-13
	pcl->length = (map->m_llen << Z_EROFS_PCLUSTER_LENGTH_BIT) |
		(map->m_flags & EROFS_MAP_FULL_MAPPED ?
			Z_EROFS_PCLUSTER_FULL_LENGTH : 0);

<<<<<<< HEAD
	if (map->m_flags & EROFS_MAP_ZIPPED)
		pcl->algorithmformat = Z_EROFS_COMPRESSION_LZ4;
	else
		pcl->algorithmformat = Z_EROFS_COMPRESSION_SHIFTED;

	pcl->clusterbits = EROFS_I(inode)->z_physical_clusterbits[0];
	pcl->clusterbits -= PAGE_SHIFT;

=======
>>>>>>> upstream/android-13
	/* new pclusters should be claimed as type 1, primary and followed */
	pcl->next = clt->owned_head;
	clt->mode = COLLECT_PRIMARY_FOLLOWED;

	cl = z_erofs_primarycollection(pcl);
	cl->pageofs = map->m_la & ~PAGE_MASK;

	/*
	 * lock all primary followed works before visible to others
	 * and mutex_trylock *never* fails for a new pcluster.
	 */
<<<<<<< HEAD
	mutex_trylock(&cl->lock);

	err = erofs_register_workgroup(inode->i_sb, &pcl->obj, 0);
	if (err) {
		mutex_unlock(&cl->lock);
		kmem_cache_free(pcluster_cachep, pcl);
		return ERR_PTR(-EAGAIN);
=======
	mutex_init(&cl->lock);
	DBG_BUGON(!mutex_trylock(&cl->lock));

	grp = erofs_insert_workgroup(inode->i_sb, &pcl->obj);
	if (IS_ERR(grp)) {
		err = PTR_ERR(grp);
		goto err_out;
	}

	if (grp != &pcl->obj) {
		clt->pcl = container_of(grp, struct z_erofs_pcluster, obj);
		err = -EEXIST;
		goto err_out;
>>>>>>> upstream/android-13
	}
	/* used to check tail merging loop due to corrupted images */
	if (clt->owned_head == Z_EROFS_PCLUSTER_TAIL)
		clt->tailpcl = pcl;
	clt->owned_head = &pcl->next;
	clt->pcl = pcl;
	clt->cl = cl;
<<<<<<< HEAD
	return cl;
=======
	return 0;

err_out:
	mutex_unlock(&cl->lock);
	z_erofs_free_pcluster(pcl);
	return err;
>>>>>>> upstream/android-13
}

static int z_erofs_collector_begin(struct z_erofs_collector *clt,
				   struct inode *inode,
				   struct erofs_map_blocks *map)
{
<<<<<<< HEAD
	struct z_erofs_collection *cl;
=======
	struct erofs_workgroup *grp;
	int ret;
>>>>>>> upstream/android-13

	DBG_BUGON(clt->cl);

	/* must be Z_EROFS_PCLUSTER_TAIL or pointed to previous collection */
	DBG_BUGON(clt->owned_head == Z_EROFS_PCLUSTER_NIL);
	DBG_BUGON(clt->owned_head == Z_EROFS_PCLUSTER_TAIL_CLOSED);

	if (!PAGE_ALIGNED(map->m_pa)) {
		DBG_BUGON(1);
		return -EINVAL;
	}

<<<<<<< HEAD
repeat:
	cl = cllookup(clt, inode, map);
	if (!cl) {
		cl = clregister(clt, inode, map);

		if (cl == ERR_PTR(-EAGAIN))
			goto repeat;
	}

	if (IS_ERR(cl))
		return PTR_ERR(cl);

	z_erofs_pagevec_ctor_init(&clt->vector, Z_EROFS_NR_INLINE_PAGEVECS,
				  cl->pagevec, cl->vcnt);

	clt->compressedpages = clt->pcl->compressed_pages;
	if (clt->mode <= COLLECT_PRIMARY) /* cannot do in-place I/O */
		clt->compressedpages += Z_EROFS_CLUSTER_MAX_PAGES;
=======
	grp = erofs_find_workgroup(inode->i_sb, map->m_pa >> PAGE_SHIFT);
	if (grp) {
		clt->pcl = container_of(grp, struct z_erofs_pcluster, obj);
	} else {
		ret = z_erofs_register_collection(clt, inode, map);

		if (!ret)
			goto out;
		if (ret != -EEXIST)
			return ret;
	}

	ret = z_erofs_lookup_collection(clt, inode, map);
	if (ret) {
		erofs_workgroup_put(&clt->pcl->obj);
		return ret;
	}

out:
	z_erofs_pagevec_ctor_init(&clt->vector, Z_EROFS_NR_INLINE_PAGEVECS,
				  clt->cl->pagevec, clt->cl->vcnt);

	/* since file-backed online pages are traversed in reverse order */
	clt->icpage_ptr = clt->pcl->compressed_pages + clt->pcl->pclusterpages;
>>>>>>> upstream/android-13
	return 0;
}

/*
 * keep in mind that no referenced pclusters will be freed
 * only after a RCU grace period.
 */
static void z_erofs_rcu_callback(struct rcu_head *head)
{
	struct z_erofs_collection *const cl =
		container_of(head, struct z_erofs_collection, rcu);

<<<<<<< HEAD
	kmem_cache_free(pcluster_cachep,
			container_of(cl, struct z_erofs_pcluster,
				     primary_collection));
=======
	z_erofs_free_pcluster(container_of(cl, struct z_erofs_pcluster,
					   primary_collection));
>>>>>>> upstream/android-13
}

void erofs_workgroup_free_rcu(struct erofs_workgroup *grp)
{
	struct z_erofs_pcluster *const pcl =
		container_of(grp, struct z_erofs_pcluster, obj);
	struct z_erofs_collection *const cl = z_erofs_primarycollection(pcl);

	call_rcu(&cl->rcu, z_erofs_rcu_callback);
}

static void z_erofs_collection_put(struct z_erofs_collection *cl)
{
	struct z_erofs_pcluster *const pcl =
		container_of(cl, struct z_erofs_pcluster, primary_collection);

	erofs_workgroup_put(&pcl->obj);
}

static bool z_erofs_collector_end(struct z_erofs_collector *clt)
{
	struct z_erofs_collection *cl = clt->cl;

	if (!cl)
		return false;

	z_erofs_pagevec_ctor_exit(&clt->vector, false);
	mutex_unlock(&cl->lock);

	/*
	 * if all pending pages are added, don't hold its reference
	 * any longer if the pcluster isn't hosted by ourselves.
	 */
	if (clt->mode < COLLECT_PRIMARY_FOLLOWED_NOINPLACE)
		z_erofs_collection_put(cl);

	clt->cl = NULL;
	return true;
}

<<<<<<< HEAD
static inline struct page *__stagingpage_alloc(struct list_head *pagepool,
					       gfp_t gfp)
{
	struct page *page = erofs_allocpage(pagepool, gfp, true);

	page->mapping = Z_EROFS_MAPPING_STAGING;
	return page;
}

=======
>>>>>>> upstream/android-13
static bool should_alloc_managed_pages(struct z_erofs_decompress_frontend *fe,
				       unsigned int cachestrategy,
				       erofs_off_t la)
{
	if (cachestrategy <= EROFS_ZIP_CACHE_DISABLED)
		return false;

	if (fe->backmost)
		return true;

	return cachestrategy >= EROFS_ZIP_CACHE_READAROUND &&
		la < fe->headoffset;
}

static int z_erofs_do_read_page(struct z_erofs_decompress_frontend *fe,
<<<<<<< HEAD
				struct page *page,
				struct list_head *pagepool)
{
	struct inode *const inode = fe->inode;
	struct erofs_sb_info *const sbi __maybe_unused = EROFS_I_SB(inode);
=======
				struct page *page, struct page **pagepool)
{
	struct inode *const inode = fe->inode;
	struct erofs_sb_info *const sbi = EROFS_I_SB(inode);
>>>>>>> upstream/android-13
	struct erofs_map_blocks *const map = &fe->map;
	struct z_erofs_collector *const clt = &fe->clt;
	const loff_t offset = page_offset(page);
	bool tight = true;

	enum z_erofs_cache_alloctype cache_strategy;
	enum z_erofs_page_type page_type;
	unsigned int cur, end, spiltted, index;
	int err = 0;

	/* register locked file pages as online pages in pack */
	z_erofs_onlinepage_init(page);

	if (cleancache_get_page(page) == 0)
		goto out;

	spiltted = 0;
	end = PAGE_SIZE;
repeat:
	cur = end - 1;

	/* lucky, within the range of the current map_blocks */
	if (offset + cur >= map->m_la &&
	    offset + cur < map->m_la + map->m_llen) {
		/* didn't get a valid collection previously (very rare) */
		if (!clt->cl)
			goto restart_now;
		goto hitted;
	}

	/* go ahead the next map_blocks */
	erofs_dbg("%s: [out-of-range] pos %llu", __func__, offset + cur);

	if (z_erofs_collector_end(clt))
		fe->backmost = false;

	map->m_la = offset + cur;
	map->m_llen = 0;
	err = z_erofs_map_blocks_iter(inode, map, 0);
	if (err)
		goto err_out;

restart_now:
	if (!(map->m_flags & EROFS_MAP_MAPPED))
		goto hitted;

	err = z_erofs_collector_begin(clt, inode, map);
	if (err)
		goto err_out;

	/* preload all compressed pages (maybe downgrade role if necessary) */
<<<<<<< HEAD
	if (should_alloc_managed_pages(fe, sbi->cache_strategy, map->m_la))
		cache_strategy = DELAYEDALLOC;
=======
	if (should_alloc_managed_pages(fe, sbi->opt.cache_strategy, map->m_la))
		cache_strategy = TRYALLOC;
>>>>>>> upstream/android-13
	else
		cache_strategy = DONTALLOC;

	preload_compressed_pages(clt, MNGD_MAPPING(sbi),
				 cache_strategy, pagepool);

hitted:
	/*
	 * Ensure the current partial page belongs to this submit chain rather
	 * than other concurrent submit chains or the noio(bypass) chain since
	 * those chains are handled asynchronously thus the page cannot be used
	 * for inplace I/O or pagevec (should be processed in strict order.)
	 */
	tight &= (clt->mode >= COLLECT_PRIMARY_HOOKED &&
		  clt->mode != COLLECT_PRIMARY_FOLLOWED_NOINPLACE);

	cur = end - min_t(unsigned int, offset + end - map->m_la, end);
	if (!(map->m_flags & EROFS_MAP_MAPPED)) {
		zero_user_segment(page, cur, end);
		goto next_part;
	}

	/* let's derive page type */
	page_type = cur ? Z_EROFS_VLE_PAGE_TYPE_HEAD :
		(!spiltted ? Z_EROFS_PAGE_TYPE_EXCLUSIVE :
			(tight ? Z_EROFS_PAGE_TYPE_EXCLUSIVE :
				Z_EROFS_VLE_PAGE_TYPE_TAIL_SHARED));

	if (cur)
		tight &= (clt->mode >= COLLECT_PRIMARY_FOLLOWED);

retry:
<<<<<<< HEAD
	err = z_erofs_attach_page(clt, page, page_type);
	/* should allocate an additional staging page for pagevec */
	if (err == -EAGAIN) {
		struct page *const newpage =
			__stagingpage_alloc(pagepool, GFP_NOFS);

		err = z_erofs_attach_page(clt, newpage,
					  Z_EROFS_PAGE_TYPE_EXCLUSIVE);
=======
	err = z_erofs_attach_page(clt, page, page_type,
				  clt->mode >= COLLECT_PRIMARY_FOLLOWED);
	/* should allocate an additional short-lived page for pagevec */
	if (err == -EAGAIN) {
		struct page *const newpage =
				alloc_page(GFP_NOFS | __GFP_NOFAIL);

		set_page_private(newpage, Z_EROFS_SHORTLIVED_PAGE);
		err = z_erofs_attach_page(clt, newpage,
					  Z_EROFS_PAGE_TYPE_EXCLUSIVE, true);
>>>>>>> upstream/android-13
		if (!err)
			goto retry;
	}

	if (err)
		goto err_out;

	index = page->index - (map->m_la >> PAGE_SHIFT);

	z_erofs_onlinepage_fixup(page, index, true);

	/* bump up the number of spiltted parts of a page */
	++spiltted;
	/* also update nr_pages */
	clt->cl->nr_pages = max_t(pgoff_t, clt->cl->nr_pages, index + 1);
next_part:
	/* can be used for verification */
	map->m_llen = offset + cur - map->m_la;

	end = cur;
	if (end > 0)
		goto repeat;

out:
	z_erofs_onlinepage_endio(page);

	erofs_dbg("%s, finish page: %pK spiltted: %u map->m_llen %llu",
		  __func__, page, spiltted, map->m_llen);
	return err;

	/* if some error occurred while processing this page */
err_out:
	SetPageError(page);
	goto out;
}

<<<<<<< HEAD
static void z_erofs_vle_unzip_kickoff(void *ptr, int bios)
{
	tagptr1_t t = tagptr_init(tagptr1_t, ptr);
	struct z_erofs_unzip_io *io = tagptr_unfold_ptr(t);
	bool background = tagptr_unfold_tags(t);

	if (!background) {
=======
static void z_erofs_decompressqueue_work(struct work_struct *work);
static void z_erofs_decompress_kickoff(struct z_erofs_decompressqueue *io,
				       bool sync, int bios)
{
	struct erofs_sb_info *const sbi = EROFS_SB(io->sb);

	/* wake up the caller thread for sync decompression */
	if (sync) {
>>>>>>> upstream/android-13
		unsigned long flags;

		spin_lock_irqsave(&io->u.wait.lock, flags);
		if (!atomic_add_return(bios, &io->pending_bios))
			wake_up_locked(&io->u.wait);
		spin_unlock_irqrestore(&io->u.wait.lock, flags);
		return;
	}

<<<<<<< HEAD
	if (!atomic_add_return(bios, &io->pending_bios))
		queue_work(z_erofs_workqueue, &io->u.work);
}

static inline void z_erofs_vle_read_endio(struct bio *bio)
{
	struct erofs_sb_info *sbi = NULL;
	blk_status_t err = bio->bi_status;
	struct bio_vec *bvec;
        int i;

	bio_for_each_segment_all(bvec, bio, i) {
		struct page *page = bvec->bv_page;
		bool cachemngd = false;

		DBG_BUGON(PageUptodate(page));
		DBG_BUGON(!page->mapping);

		if (!sbi && !z_erofs_page_is_staging(page))
			sbi = EROFS_SB(page->mapping->host->i_sb);

		/* sbi should already be gotten if the page is managed */
		if (sbi)
			cachemngd = erofs_page_is_managed(sbi, page);

		if (err)
			SetPageError(page);
		else if (cachemngd)
			SetPageUptodate(page);

		if (cachemngd)
			unlock_page(page);
	}

	z_erofs_vle_unzip_kickoff(bio->bi_private, -1);
=======
	if (atomic_add_return(bios, &io->pending_bios))
		return;
	/* Use workqueue and sync decompression for atomic contexts only */
	if (in_atomic() || irqs_disabled()) {
		queue_work(z_erofs_workqueue, &io->u.work);
		sbi->opt.readahead_sync_decompress = true;
		return;
	}
	z_erofs_decompressqueue_work(&io->u.work);
}

static bool z_erofs_page_is_invalidated(struct page *page)
{
	return !page->mapping && !z_erofs_is_shortlived_page(page);
}

static void z_erofs_decompressqueue_endio(struct bio *bio)
{
	tagptr1_t t = tagptr_init(tagptr1_t, bio->bi_private);
	struct z_erofs_decompressqueue *q = tagptr_unfold_ptr(t);
	blk_status_t err = bio->bi_status;
	struct bio_vec *bvec;
	struct bvec_iter_all iter_all;

	bio_for_each_segment_all(bvec, bio, iter_all) {
		struct page *page = bvec->bv_page;

		DBG_BUGON(PageUptodate(page));
		DBG_BUGON(z_erofs_page_is_invalidated(page));

		if (err)
			SetPageError(page);

		if (erofs_page_is_managed(EROFS_SB(q->sb), page)) {
			if (!err)
				SetPageUptodate(page);
			unlock_page(page);
		}
	}
	z_erofs_decompress_kickoff(q, tagptr_unfold_tags(t), -1);
>>>>>>> upstream/android-13
	bio_put(bio);
}

static int z_erofs_decompress_pcluster(struct super_block *sb,
				       struct z_erofs_pcluster *pcl,
<<<<<<< HEAD
				       struct list_head *pagepool)
{
	struct erofs_sb_info *const sbi = EROFS_SB(sb);
	const unsigned int clusterpages = BIT(pcl->clusterbits);
	struct z_erofs_pagevec_ctor ctor;
	unsigned int i, outputsize, llen, nr_pages;
=======
				       struct page **pagepool)
{
	struct erofs_sb_info *const sbi = EROFS_SB(sb);
	struct z_erofs_pagevec_ctor ctor;
	unsigned int i, inputsize, outputsize, llen, nr_pages;
>>>>>>> upstream/android-13
	struct page *pages_onstack[Z_EROFS_VMAP_ONSTACK_PAGES];
	struct page **pages, **compressed_pages, *page;

	enum z_erofs_page_type page_type;
	bool overlapped, partial;
	struct z_erofs_collection *cl;
	int err;

	might_sleep();
	cl = z_erofs_primarycollection(pcl);
	DBG_BUGON(!READ_ONCE(cl->nr_pages));

	mutex_lock(&cl->lock);
	nr_pages = cl->nr_pages;

	if (nr_pages <= Z_EROFS_VMAP_ONSTACK_PAGES) {
		pages = pages_onstack;
	} else if (nr_pages <= Z_EROFS_VMAP_GLOBAL_PAGES &&
		   mutex_trylock(&z_pagemap_global_lock)) {
		pages = z_pagemap_global;
	} else {
		gfp_t gfp_flags = GFP_KERNEL;

		if (nr_pages > Z_EROFS_VMAP_GLOBAL_PAGES)
			gfp_flags |= __GFP_NOFAIL;

		pages = kvmalloc_array(nr_pages, sizeof(struct page *),
				       gfp_flags);

		/* fallback to global pagemap for the lowmem scenario */
		if (!pages) {
			mutex_lock(&z_pagemap_global_lock);
			pages = z_pagemap_global;
		}
	}

	for (i = 0; i < nr_pages; ++i)
		pages[i] = NULL;

	err = 0;
	z_erofs_pagevec_ctor_init(&ctor, Z_EROFS_NR_INLINE_PAGEVECS,
				  cl->pagevec, 0);

	for (i = 0; i < cl->vcnt; ++i) {
		unsigned int pagenr;

		page = z_erofs_pagevec_dequeue(&ctor, &page_type);

		/* all pages in pagevec ought to be valid */
		DBG_BUGON(!page);
<<<<<<< HEAD
		DBG_BUGON(!page->mapping);

		if (z_erofs_put_stagingpage(pagepool, page))
=======
		DBG_BUGON(z_erofs_page_is_invalidated(page));

		if (z_erofs_put_shortlivedpage(pagepool, page))
>>>>>>> upstream/android-13
			continue;

		if (page_type == Z_EROFS_VLE_PAGE_TYPE_HEAD)
			pagenr = 0;
		else
			pagenr = z_erofs_onlinepage_index(page);

		DBG_BUGON(pagenr >= nr_pages);

		/*
		 * currently EROFS doesn't support multiref(dedup),
		 * so here erroring out one multiref page.
		 */
		if (pages[pagenr]) {
			DBG_BUGON(1);
			SetPageError(pages[pagenr]);
			z_erofs_onlinepage_endio(pages[pagenr]);
			err = -EFSCORRUPTED;
		}
		pages[pagenr] = page;
	}
	z_erofs_pagevec_ctor_exit(&ctor, true);

	overlapped = false;
	compressed_pages = pcl->compressed_pages;

<<<<<<< HEAD
	for (i = 0; i < clusterpages; ++i) {
=======
	for (i = 0; i < pcl->pclusterpages; ++i) {
>>>>>>> upstream/android-13
		unsigned int pagenr;

		page = compressed_pages[i];

		/* all compressed pages ought to be valid */
		DBG_BUGON(!page);
<<<<<<< HEAD
		DBG_BUGON(!page->mapping);

		if (!z_erofs_page_is_staging(page)) {
=======
		DBG_BUGON(z_erofs_page_is_invalidated(page));

		if (!z_erofs_is_shortlived_page(page)) {
>>>>>>> upstream/android-13
			if (erofs_page_is_managed(sbi, page)) {
				if (!PageUptodate(page))
					err = -EIO;
				continue;
			}

			/*
			 * only if non-head page can be selected
			 * for inplace decompression
			 */
			pagenr = z_erofs_onlinepage_index(page);

			DBG_BUGON(pagenr >= nr_pages);
			if (pages[pagenr]) {
				DBG_BUGON(1);
				SetPageError(pages[pagenr]);
				z_erofs_onlinepage_endio(pages[pagenr]);
				err = -EFSCORRUPTED;
			}
			pages[pagenr] = page;

			overlapped = true;
		}

<<<<<<< HEAD
		/* PG_error needs checking for inplaced and staging pages */
=======
		/* PG_error needs checking for all non-managed pages */
>>>>>>> upstream/android-13
		if (PageError(page)) {
			DBG_BUGON(PageUptodate(page));
			err = -EIO;
		}
	}

	if (err)
		goto out;

	llen = pcl->length >> Z_EROFS_PCLUSTER_LENGTH_BIT;
	if (nr_pages << PAGE_SHIFT >= cl->pageofs + llen) {
		outputsize = llen;
		partial = !(pcl->length & Z_EROFS_PCLUSTER_FULL_LENGTH);
	} else {
		outputsize = (nr_pages << PAGE_SHIFT) - cl->pageofs;
		partial = true;
	}

<<<<<<< HEAD
=======
	inputsize = pcl->pclusterpages * PAGE_SIZE;
>>>>>>> upstream/android-13
	err = z_erofs_decompress(&(struct z_erofs_decompress_req) {
					.sb = sb,
					.in = compressed_pages,
					.out = pages,
					.pageofs_out = cl->pageofs,
<<<<<<< HEAD
					.inputsize = PAGE_SIZE,
=======
					.inputsize = inputsize,
>>>>>>> upstream/android-13
					.outputsize = outputsize,
					.alg = pcl->algorithmformat,
					.inplace_io = overlapped,
					.partial_decoding = partial
				 }, pagepool);

out:
<<<<<<< HEAD
	/* must handle all compressed pages before endding pages */
	for (i = 0; i < clusterpages; ++i) {
=======
	/* must handle all compressed pages before ending pages */
	for (i = 0; i < pcl->pclusterpages; ++i) {
>>>>>>> upstream/android-13
		page = compressed_pages[i];

		if (erofs_page_is_managed(sbi, page))
			continue;

<<<<<<< HEAD
		/* recycle all individual staging pages */
		(void)z_erofs_put_stagingpage(pagepool, page);
=======
		/* recycle all individual short-lived pages */
		(void)z_erofs_put_shortlivedpage(pagepool, page);
>>>>>>> upstream/android-13

		WRITE_ONCE(compressed_pages[i], NULL);
	}

	for (i = 0; i < nr_pages; ++i) {
		page = pages[i];
		if (!page)
			continue;

<<<<<<< HEAD
		DBG_BUGON(!page->mapping);

		/* recycle all individual staging pages */
		if (z_erofs_put_stagingpage(pagepool, page))
=======
		DBG_BUGON(z_erofs_page_is_invalidated(page));

		/* recycle all individual short-lived pages */
		if (z_erofs_put_shortlivedpage(pagepool, page))
>>>>>>> upstream/android-13
			continue;

		if (err < 0)
			SetPageError(page);

		z_erofs_onlinepage_endio(page);
	}

	if (pages == z_pagemap_global)
		mutex_unlock(&z_pagemap_global_lock);
	else if (pages != pages_onstack)
		kvfree(pages);

	cl->nr_pages = 0;
	cl->vcnt = 0;

	/* all cl locks MUST be taken before the following line */
	WRITE_ONCE(pcl->next, Z_EROFS_PCLUSTER_NIL);

	/* all cl locks SHOULD be released right now */
	mutex_unlock(&cl->lock);

	z_erofs_collection_put(cl);
	return err;
}

<<<<<<< HEAD
static void z_erofs_vle_unzip_all(struct super_block *sb,
				  struct z_erofs_unzip_io *io,
				  struct list_head *pagepool)
=======
static void z_erofs_decompress_queue(const struct z_erofs_decompressqueue *io,
				     struct page **pagepool)
>>>>>>> upstream/android-13
{
	z_erofs_next_pcluster_t owned = io->head;

	while (owned != Z_EROFS_PCLUSTER_TAIL_CLOSED) {
		struct z_erofs_pcluster *pcl;

		/* no possible that 'owned' equals Z_EROFS_WORK_TPTR_TAIL */
		DBG_BUGON(owned == Z_EROFS_PCLUSTER_TAIL);

		/* no possible that 'owned' equals NULL */
		DBG_BUGON(owned == Z_EROFS_PCLUSTER_NIL);

		pcl = container_of(owned, struct z_erofs_pcluster, next);
		owned = READ_ONCE(pcl->next);

<<<<<<< HEAD
		z_erofs_decompress_pcluster(sb, pcl, pagepool);
	}
}

static void z_erofs_vle_unzip_wq(struct work_struct *work)
{
	struct z_erofs_unzip_io_sb *iosb =
		container_of(work, struct z_erofs_unzip_io_sb, io.u.work);
	LIST_HEAD(pagepool);

	DBG_BUGON(iosb->io.head == Z_EROFS_PCLUSTER_TAIL_CLOSED);
	z_erofs_vle_unzip_all(iosb->sb, &iosb->io, &pagepool);

	put_pages_list(&pagepool);
	kvfree(iosb);
=======
		z_erofs_decompress_pcluster(io->sb, pcl, pagepool);
	}
}

static void z_erofs_decompressqueue_work(struct work_struct *work)
{
	struct z_erofs_decompressqueue *bgq =
		container_of(work, struct z_erofs_decompressqueue, u.work);
	struct page *pagepool = NULL;

	DBG_BUGON(bgq->head == Z_EROFS_PCLUSTER_TAIL_CLOSED);
	z_erofs_decompress_queue(bgq, &pagepool);

	erofs_release_pages(&pagepool);
	kvfree(bgq);
>>>>>>> upstream/android-13
}

static struct page *pickup_page_for_submission(struct z_erofs_pcluster *pcl,
					       unsigned int nr,
<<<<<<< HEAD
					       struct list_head *pagepool,
					       struct address_space *mc,
					       gfp_t gfp)
{
	/* determined at compile time to avoid too many #ifdefs */
	const bool nocache = __builtin_constant_p(mc) ? !mc : false;
=======
					       struct page **pagepool,
					       struct address_space *mc,
					       gfp_t gfp)
{
>>>>>>> upstream/android-13
	const pgoff_t index = pcl->obj.index;
	bool tocache = false;

	struct address_space *mapping;
	struct page *oldpage, *page;

	compressed_page_t t;
	int justfound;

repeat:
	page = READ_ONCE(pcl->compressed_pages[nr]);
	oldpage = page;

	if (!page)
		goto out_allocpage;

<<<<<<< HEAD
	/*
	 * the cached page has not been allocated and
	 * an placeholder is out there, prepare it now.
	 */
	if (!nocache && page == PAGE_UNALLOCATED) {
		tocache = true;
		goto out_allocpage;
	}

=======
>>>>>>> upstream/android-13
	/* process the target tagged pointer */
	t = tagptr_init(compressed_page_t, page);
	justfound = tagptr_unfold_tags(t);
	page = tagptr_unfold_ptr(t);

<<<<<<< HEAD
	mapping = READ_ONCE(page->mapping);

	/*
	 * if managed cache is disabled, it's no way to
	 * get such a cached-like page.
	 */
	if (nocache) {
		/* if managed cache is disabled, it is impossible `justfound' */
		DBG_BUGON(justfound);

		/* and it should be locked, not uptodate, and not truncated */
		DBG_BUGON(!PageLocked(page));
		DBG_BUGON(PageUptodate(page));
		DBG_BUGON(!mapping);
		goto out;
	}

	/*
	 * unmanaged (file) pages are all locked solidly,
=======
	/*
	 * preallocated cached pages, which is used to avoid direct reclaim
	 * otherwise, it will go inplace I/O path instead.
	 */
	if (page->private == Z_EROFS_PREALLOCATED_PAGE) {
		WRITE_ONCE(pcl->compressed_pages[nr], page);
		set_page_private(page, 0);
		tocache = true;
		goto out_tocache;
	}
	mapping = READ_ONCE(page->mapping);

	/*
	 * file-backed online pages in plcuster are all locked steady,
>>>>>>> upstream/android-13
	 * therefore it is impossible for `mapping' to be NULL.
	 */
	if (mapping && mapping != mc)
		/* ought to be unmanaged pages */
		goto out;

<<<<<<< HEAD
=======
	/* directly return for shortlived page as well */
	if (z_erofs_is_shortlived_page(page))
		goto out;

>>>>>>> upstream/android-13
	lock_page(page);

	/* only true if page reclaim goes wrong, should never happen */
	DBG_BUGON(justfound && PagePrivate(page));

	/* the page is still in manage cache */
	if (page->mapping == mc) {
		WRITE_ONCE(pcl->compressed_pages[nr], page);

		ClearPageError(page);
		if (!PagePrivate(page)) {
			/*
			 * impossible to be !PagePrivate(page) for
			 * the current restriction as well if
			 * the page is already in compressed_pages[].
			 */
			DBG_BUGON(!justfound);

			justfound = 0;
			set_page_private(page, (unsigned long)pcl);
			SetPagePrivate(page);
		}

		/* no need to submit io if it is already up-to-date */
		if (PageUptodate(page)) {
			unlock_page(page);
			page = NULL;
		}
		goto out;
	}

	/*
	 * the managed page has been truncated, it's unsafe to
	 * reuse this one, let's allocate a new cache-managed page.
	 */
	DBG_BUGON(page->mapping);
	DBG_BUGON(!justfound);

	tocache = true;
	unlock_page(page);
	put_page(page);
out_allocpage:
<<<<<<< HEAD
	page = __stagingpage_alloc(pagepool, gfp);
	if (oldpage != cmpxchg(&pcl->compressed_pages[nr], oldpage, page)) {
		list_add(&page->lru, pagepool);
		cpu_relax();
		goto repeat;
	}
	if (nocache || !tocache)
		goto out;
	if (add_to_page_cache_lru(page, mc, index + nr, gfp)) {
		page->mapping = Z_EROFS_MAPPING_STAGING;
		goto out;
	}

	set_page_private(page, (unsigned long)pcl);
	SetPagePrivate(page);
=======
	page = erofs_allocpage(pagepool, gfp | __GFP_NOFAIL);
	if (oldpage != cmpxchg(&pcl->compressed_pages[nr], oldpage, page)) {
		erofs_pagepool_add(pagepool, page);
		cond_resched();
		goto repeat;
	}
out_tocache:
	if (!tocache || add_to_page_cache_lru(page, mc, index + nr, gfp)) {
		/* turn into temporary page if fails (1 ref) */
		set_page_private(page, Z_EROFS_SHORTLIVED_PAGE);
		goto out;
	}
	attach_page_private(page, pcl);
	/* drop a refcount added by allocpage (then we have 2 refs here) */
	put_page(page);

>>>>>>> upstream/android-13
out:	/* the only exit (for tracing and debugging) */
	return page;
}

<<<<<<< HEAD
static struct z_erofs_unzip_io *jobqueue_init(struct super_block *sb,
					      struct z_erofs_unzip_io *io,
					      bool foreground)
{
	struct z_erofs_unzip_io_sb *iosb;

	if (foreground) {
		/* waitqueue available for foreground io */
		DBG_BUGON(!io);

		init_waitqueue_head(&io->u.wait);
		atomic_set(&io->pending_bios, 0);
		goto out;
	}

	iosb = kvzalloc(sizeof(*iosb), GFP_KERNEL | __GFP_NOFAIL);
	DBG_BUGON(!iosb);

	/* initialize fields in the allocated descriptor */
	io = &iosb->io;
	iosb->sb = sb;
	INIT_WORK(&io->u.work, z_erofs_vle_unzip_wq);
out:
	io->head = Z_EROFS_PCLUSTER_TAIL_CLOSED;
	return io;
=======
static struct z_erofs_decompressqueue *
jobqueue_init(struct super_block *sb,
	      struct z_erofs_decompressqueue *fgq, bool *fg)
{
	struct z_erofs_decompressqueue *q;

	if (fg && !*fg) {
		q = kvzalloc(sizeof(*q), GFP_KERNEL | __GFP_NOWARN);
		if (!q) {
			*fg = true;
			goto fg_out;
		}
		INIT_WORK(&q->u.work, z_erofs_decompressqueue_work);
	} else {
fg_out:
		q = fgq;
		init_waitqueue_head(&fgq->u.wait);
		atomic_set(&fgq->pending_bios, 0);
	}
	q->sb = sb;
	q->head = Z_EROFS_PCLUSTER_TAIL_CLOSED;
	return q;
>>>>>>> upstream/android-13
}

/* define decompression jobqueue types */
enum {
	JQ_BYPASS,
	JQ_SUBMIT,
	NR_JOBQUEUES,
};

static void *jobqueueset_init(struct super_block *sb,
<<<<<<< HEAD
			      z_erofs_next_pcluster_t qtail[],
			      struct z_erofs_unzip_io *q[],
			      struct z_erofs_unzip_io *fgq,
			      bool forcefg)
=======
			      struct z_erofs_decompressqueue *q[],
			      struct z_erofs_decompressqueue *fgq, bool *fg)
>>>>>>> upstream/android-13
{
	/*
	 * if managed cache is enabled, bypass jobqueue is needed,
	 * no need to read from device for all pclusters in this queue.
	 */
<<<<<<< HEAD
	q[JQ_BYPASS] = jobqueue_init(sb, fgq + JQ_BYPASS, true);
	qtail[JQ_BYPASS] = &q[JQ_BYPASS]->head;

	q[JQ_SUBMIT] = jobqueue_init(sb, fgq + JQ_SUBMIT, forcefg);
	qtail[JQ_SUBMIT] = &q[JQ_SUBMIT]->head;

	return tagptr_cast_ptr(tagptr_fold(tagptr1_t, q[JQ_SUBMIT], !forcefg));
=======
	q[JQ_BYPASS] = jobqueue_init(sb, fgq + JQ_BYPASS, NULL);
	q[JQ_SUBMIT] = jobqueue_init(sb, fgq + JQ_SUBMIT, fg);

	return tagptr_cast_ptr(tagptr_fold(tagptr1_t, q[JQ_SUBMIT], *fg));
>>>>>>> upstream/android-13
}

static void move_to_bypass_jobqueue(struct z_erofs_pcluster *pcl,
				    z_erofs_next_pcluster_t qtail[],
				    z_erofs_next_pcluster_t owned_head)
{
	z_erofs_next_pcluster_t *const submit_qtail = qtail[JQ_SUBMIT];
	z_erofs_next_pcluster_t *const bypass_qtail = qtail[JQ_BYPASS];

	DBG_BUGON(owned_head == Z_EROFS_PCLUSTER_TAIL_CLOSED);
	if (owned_head == Z_EROFS_PCLUSTER_TAIL)
		owned_head = Z_EROFS_PCLUSTER_TAIL_CLOSED;

	WRITE_ONCE(pcl->next, Z_EROFS_PCLUSTER_TAIL_CLOSED);

	WRITE_ONCE(*submit_qtail, owned_head);
	WRITE_ONCE(*bypass_qtail, &pcl->next);

	qtail[JQ_BYPASS] = &pcl->next;
}

<<<<<<< HEAD
static bool postsubmit_is_all_bypassed(struct z_erofs_unzip_io *q[],
				       unsigned int nr_bios,
				       bool force_fg)
{
	/*
	 * although background is preferred, no one is pending for submission.
	 * don't issue workqueue for decompression but drop it directly instead.
	 */
	if (force_fg || nr_bios)
		return false;

	kvfree(container_of(q[JQ_SUBMIT], struct z_erofs_unzip_io_sb, io));
	return true;
}

static bool z_erofs_vle_submit_all(struct super_block *sb,
				   z_erofs_next_pcluster_t owned_head,
				   struct list_head *pagepool,
				   struct z_erofs_unzip_io *fgq,
				   bool force_fg)
{
	struct erofs_sb_info *const sbi __maybe_unused = EROFS_SB(sb);
	z_erofs_next_pcluster_t qtail[NR_JOBQUEUES];
	struct z_erofs_unzip_io *q[NR_JOBQUEUES];
	struct bio *bio;
	void *bi_private;
	/* since bio will be NULL, no need to initialize last_index */
	pgoff_t uninitialized_var(last_index);
	bool force_submit = false;
	unsigned int nr_bios;

	if (owned_head == Z_EROFS_PCLUSTER_TAIL)
		return false;

	force_submit = false;
	bio = NULL;
	nr_bios = 0;
	bi_private = jobqueueset_init(sb, qtail, q, fgq, force_fg);
=======
static void z_erofs_submit_queue(struct super_block *sb,
				 struct z_erofs_decompress_frontend *f,
				 struct page **pagepool,
				 struct z_erofs_decompressqueue *fgq,
				 bool *force_fg)
{
	struct erofs_sb_info *const sbi = EROFS_SB(sb);
	z_erofs_next_pcluster_t qtail[NR_JOBQUEUES];
	struct z_erofs_decompressqueue *q[NR_JOBQUEUES];
	void *bi_private;
	z_erofs_next_pcluster_t owned_head = f->clt.owned_head;
	/* bio is NULL initially, so no need to initialize last_{index,bdev} */
	pgoff_t last_index;
	struct block_device *last_bdev;
	unsigned int nr_bios = 0;
	struct bio *bio = NULL;

	bi_private = jobqueueset_init(sb, q, fgq, force_fg);
	qtail[JQ_BYPASS] = &q[JQ_BYPASS]->head;
	qtail[JQ_SUBMIT] = &q[JQ_SUBMIT]->head;
>>>>>>> upstream/android-13

	/* by default, all need io submission */
	q[JQ_SUBMIT]->head = owned_head;

	do {
<<<<<<< HEAD
		struct z_erofs_pcluster *pcl;
		unsigned int clusterpages;
		pgoff_t first_index;
		struct page *page;
		unsigned int i = 0, bypass = 0;
		int err;
=======
		struct erofs_map_dev mdev;
		struct z_erofs_pcluster *pcl;
		pgoff_t cur, end;
		unsigned int i = 0;
		bool bypass = true;
>>>>>>> upstream/android-13

		/* no possible 'owned_head' equals the following */
		DBG_BUGON(owned_head == Z_EROFS_PCLUSTER_TAIL_CLOSED);
		DBG_BUGON(owned_head == Z_EROFS_PCLUSTER_NIL);

		pcl = container_of(owned_head, struct z_erofs_pcluster, next);

<<<<<<< HEAD
		clusterpages = BIT(pcl->clusterbits);
=======
		/* no device id here, thus it will always succeed */
		mdev = (struct erofs_map_dev) {
			.m_pa = blknr_to_addr(pcl->obj.index),
		};
		(void)erofs_map_dev(sb, &mdev);

		cur = erofs_blknr(mdev.m_pa);
		end = cur + pcl->pclusterpages;
>>>>>>> upstream/android-13

		/* close the main owned chain at first */
		owned_head = cmpxchg(&pcl->next, Z_EROFS_PCLUSTER_TAIL,
				     Z_EROFS_PCLUSTER_TAIL_CLOSED);

<<<<<<< HEAD
		first_index = pcl->obj.index;
		force_submit |= (first_index != last_index + 1);

repeat:
		page = pickup_page_for_submission(pcl, i, pagepool,
						  MNGD_MAPPING(sbi),
						  GFP_NOFS);
		if (!page) {
			force_submit = true;
			++bypass;
			goto skippage;
		}

		if (bio && force_submit) {
submit_bio_retry:
			submit_bio(bio);
			bio = NULL;
		}

		if (!bio) {
			bio = bio_alloc(GFP_NOIO, BIO_MAX_PAGES);

			bio->bi_end_io = z_erofs_vle_read_endio;
			bio_set_dev(bio, sb->s_bdev);
			bio->bi_iter.bi_sector = (sector_t)(first_index + i) <<
				LOG_SECTORS_PER_BLOCK;
			bio->bi_private = bi_private;
			bio->bi_opf = REQ_OP_READ;

			++nr_bios;
		}

		err = bio_add_page(bio, page, PAGE_SIZE, 0);
		if (err < PAGE_SIZE)
			goto submit_bio_retry;

		force_submit = false;
		last_index = first_index + i;
skippage:
		if (++i < clusterpages)
			goto repeat;

		if (bypass < clusterpages)
=======
		do {
			struct page *page;

			page = pickup_page_for_submission(pcl, i++, pagepool,
							  MNGD_MAPPING(sbi),
							  GFP_NOFS);
			if (!page)
				continue;

			if (bio && (cur != last_index + 1 ||
				    last_bdev != mdev.m_bdev)) {
submit_bio_retry:
				submit_bio(bio);
				bio = NULL;
			}

			if (!bio) {
				bio = bio_alloc(GFP_NOIO, BIO_MAX_VECS);
				bio->bi_end_io = z_erofs_decompressqueue_endio;

				bio_set_dev(bio, mdev.m_bdev);
				last_bdev = mdev.m_bdev;
				bio->bi_iter.bi_sector = (sector_t)cur <<
					LOG_SECTORS_PER_BLOCK;
				bio->bi_private = bi_private;
				bio->bi_opf = REQ_OP_READ;
				if (f->readahead)
					bio->bi_opf |= REQ_RAHEAD;
				++nr_bios;
			}

			if (bio_add_page(bio, page, PAGE_SIZE, 0) < PAGE_SIZE)
				goto submit_bio_retry;

			last_index = cur;
			bypass = false;
		} while (++cur < end);

		if (!bypass)
>>>>>>> upstream/android-13
			qtail[JQ_SUBMIT] = &pcl->next;
		else
			move_to_bypass_jobqueue(pcl, qtail, owned_head);
	} while (owned_head != Z_EROFS_PCLUSTER_TAIL);

	if (bio)
		submit_bio(bio);

<<<<<<< HEAD
	if (postsubmit_is_all_bypassed(q, nr_bios, force_fg))
		return true;

	z_erofs_vle_unzip_kickoff(bi_private, nr_bios);
	return true;
}

static void z_erofs_submit_and_unzip(struct super_block *sb,
				     struct z_erofs_collector *clt,
				     struct list_head *pagepool,
				     bool force_fg)
{
	struct z_erofs_unzip_io io[NR_JOBQUEUES];

	if (!z_erofs_vle_submit_all(sb, clt->owned_head,
				    pagepool, io, force_fg))
		return;

	/* decompress no I/O pclusters immediately */
	z_erofs_vle_unzip_all(sb, &io[JQ_BYPASS], pagepool);
=======
	/*
	 * although background is preferred, no one is pending for submission.
	 * don't issue workqueue for decompression but drop it directly instead.
	 */
	if (!*force_fg && !nr_bios) {
		kvfree(q[JQ_SUBMIT]);
		return;
	}
	z_erofs_decompress_kickoff(q[JQ_SUBMIT], *force_fg, nr_bios);
}

static void z_erofs_runqueue(struct super_block *sb,
			     struct z_erofs_decompress_frontend *f,
			     struct page **pagepool, bool force_fg)
{
	struct z_erofs_decompressqueue io[NR_JOBQUEUES];

	if (f->clt.owned_head == Z_EROFS_PCLUSTER_TAIL)
		return;
	z_erofs_submit_queue(sb, f, pagepool, io, &force_fg);

	/* handle bypass queue (no i/o pclusters) immediately */
	z_erofs_decompress_queue(&io[JQ_BYPASS], pagepool);
>>>>>>> upstream/android-13

	if (!force_fg)
		return;

	/* wait until all bios are completed */
<<<<<<< HEAD
	wait_event(io[JQ_SUBMIT].u.wait,
		   !atomic_read(&io[JQ_SUBMIT].pending_bios));

	/* let's synchronous decompression */
	z_erofs_vle_unzip_all(sb, &io[JQ_SUBMIT], pagepool);
}

static int z_erofs_vle_normalaccess_readpage(struct file *file,
					     struct page *page)
{
	struct inode *const inode = page->mapping->host;
	struct z_erofs_decompress_frontend f = DECOMPRESS_FRONTEND_INIT(inode);
	int err;
	LIST_HEAD(pagepool);

	trace_erofs_readpage(page, false);

	f.headoffset = (erofs_off_t)page->index << PAGE_SHIFT;

	err = z_erofs_do_read_page(&f, page, &pagepool);
	(void)z_erofs_collector_end(&f.clt);

	/* if some compressed cluster ready, need submit them anyway */
	z_erofs_submit_and_unzip(inode->i_sb, &f.clt, &pagepool, true);
=======
	io_wait_event(io[JQ_SUBMIT].u.wait,
		      !atomic_read(&io[JQ_SUBMIT].pending_bios));

	/* handle synchronous decompress queue in the caller context */
	z_erofs_decompress_queue(&io[JQ_SUBMIT], pagepool);
}

/*
 * Since partial uptodate is still unimplemented for now, we have to use
 * approximate readmore strategies as a start.
 */
static void z_erofs_pcluster_readmore(struct z_erofs_decompress_frontend *f,
				      struct readahead_control *rac,
				      erofs_off_t end,
				      struct page **pagepool,
				      bool backmost)
{
	struct inode *inode = f->inode;
	struct erofs_map_blocks *map = &f->map;
	erofs_off_t cur;
	int err;

	if (backmost) {
		map->m_la = end;
		err = z_erofs_map_blocks_iter(inode, map,
					      EROFS_GET_BLOCKS_READMORE);
		if (err)
			return;

		/* expend ra for the trailing edge if readahead */
		if (rac) {
			loff_t newstart = readahead_pos(rac);

			cur = round_up(map->m_la + map->m_llen, PAGE_SIZE);
			readahead_expand(rac, newstart, cur - newstart);
			return;
		}
		end = round_up(end, PAGE_SIZE);
	} else {
		end = round_up(map->m_la, PAGE_SIZE);

		if (!map->m_llen)
			return;
	}

	cur = map->m_la + map->m_llen - 1;
	while (cur >= end) {
		pgoff_t index = cur >> PAGE_SHIFT;
		struct page *page;

		page = erofs_grab_cache_page_nowait(inode->i_mapping, index);
		if (!page)
			goto skip;

		if (PageUptodate(page)) {
			unlock_page(page);
			put_page(page);
			goto skip;
		}

		err = z_erofs_do_read_page(f, page, pagepool);
		if (err)
			erofs_err(inode->i_sb,
				  "readmore error at page %lu @ nid %llu",
				  index, EROFS_I(inode)->nid);
		put_page(page);
skip:
		if (cur < PAGE_SIZE)
			break;
		cur = (index << PAGE_SHIFT) - 1;
	}
}

static int z_erofs_readpage(struct file *file, struct page *page)
{
	struct inode *const inode = page->mapping->host;
	struct z_erofs_decompress_frontend f = DECOMPRESS_FRONTEND_INIT(inode);
	struct page *pagepool = NULL;
	int err;

	trace_erofs_readpage(page, false);
	f.headoffset = (erofs_off_t)page->index << PAGE_SHIFT;

	z_erofs_pcluster_readmore(&f, NULL, f.headoffset + PAGE_SIZE - 1,
				  &pagepool, true);
	err = z_erofs_do_read_page(&f, page, &pagepool);
	z_erofs_pcluster_readmore(&f, NULL, 0, &pagepool, false);

	(void)z_erofs_collector_end(&f.clt);

	/* if some compressed cluster ready, need submit them anyway */
	z_erofs_runqueue(inode->i_sb, &f, &pagepool, true);
>>>>>>> upstream/android-13

	if (err)
		erofs_err(inode->i_sb, "failed to read, err [%d]", err);

	if (f.map.mpage)
		put_page(f.map.mpage);

<<<<<<< HEAD
	/* clean up the remaining free pages */
	put_pages_list(&pagepool);
	return err;
}

static bool should_decompress_synchronously(struct erofs_sb_info *sbi,
					    unsigned int nr)
{
	return nr <= sbi->max_sync_decompress_pages;
}

static int z_erofs_vle_normalaccess_readpages(struct file *filp,
					      struct address_space *mapping,
					      struct list_head *pages,
					      unsigned int nr_pages)
{
	struct inode *const inode = mapping->host;
	struct erofs_sb_info *const sbi = EROFS_I_SB(inode);

	bool sync = should_decompress_synchronously(sbi, nr_pages);
	struct z_erofs_decompress_frontend f = DECOMPRESS_FRONTEND_INIT(inode);
	gfp_t gfp = mapping_gfp_constraint(mapping, GFP_KERNEL);
	struct page *head = NULL;
	LIST_HEAD(pagepool);

	trace_erofs_readpages(mapping->host, lru_to_page(pages),
			      nr_pages, false);

	f.headoffset = (erofs_off_t)lru_to_page(pages)->index << PAGE_SHIFT;

	for (; nr_pages; --nr_pages) {
		struct page *page = lru_to_page(pages);

		prefetchw(&page->flags);
		list_del(&page->lru);

		/*
		 * A pure asynchronous readahead is indicated if
		 * a PG_readahead marked page is hitted at first.
		 * Let's also do asynchronous decompression for this case.
		 */
		sync &= !(PageReadahead(page) && !head);

		if (add_to_page_cache_lru(page, mapping, page->index, gfp)) {
			list_add(&page->lru, &pagepool);
			continue;
		}

=======
	erofs_release_pages(&pagepool);
	return err;
}

static void z_erofs_readahead(struct readahead_control *rac)
{
	struct inode *const inode = rac->mapping->host;
	struct erofs_sb_info *const sbi = EROFS_I_SB(inode);
	struct z_erofs_decompress_frontend f = DECOMPRESS_FRONTEND_INIT(inode);
	struct page *pagepool = NULL, *head = NULL, *page;
	unsigned int nr_pages;

	f.readahead = true;
	f.headoffset = readahead_pos(rac);

	z_erofs_pcluster_readmore(&f, rac, f.headoffset +
				  readahead_length(rac) - 1, &pagepool, true);
	nr_pages = readahead_count(rac);
	trace_erofs_readpages(inode, readahead_index(rac), nr_pages, false);

	while ((page = readahead_page(rac))) {
>>>>>>> upstream/android-13
		set_page_private(page, (unsigned long)head);
		head = page;
	}

	while (head) {
		struct page *page = head;
		int err;

		/* traversal in reverse order */
		head = (void *)page_private(page);

		err = z_erofs_do_read_page(&f, page, &pagepool);
		if (err)
			erofs_err(inode->i_sb,
				  "readahead error at page %lu @ nid %llu",
				  page->index, EROFS_I(inode)->nid);
		put_page(page);
	}
<<<<<<< HEAD

	(void)z_erofs_collector_end(&f.clt);

	z_erofs_submit_and_unzip(inode->i_sb, &f.clt, &pagepool, sync);

	if (f.map.mpage)
		put_page(f.map.mpage);

	/* clean up the remaining free pages */
	put_pages_list(&pagepool);
	return 0;
}

const struct address_space_operations z_erofs_vle_normalaccess_aops = {
	.readpage = z_erofs_vle_normalaccess_readpage,
	.readpages = z_erofs_vle_normalaccess_readpages,
};

=======
	z_erofs_pcluster_readmore(&f, rac, 0, &pagepool, false);
	(void)z_erofs_collector_end(&f.clt);

	z_erofs_runqueue(inode->i_sb, &f, &pagepool,
			 sbi->opt.readahead_sync_decompress &&
			 nr_pages <= sbi->opt.max_sync_decompress_pages);
	if (f.map.mpage)
		put_page(f.map.mpage);
	erofs_release_pages(&pagepool);
}

const struct address_space_operations z_erofs_aops = {
	.readpage = z_erofs_readpage,
	.readahead = z_erofs_readahead,
};
>>>>>>> upstream/android-13
