/* SPDX-License-Identifier: GPL-2.0
 *
 * page_pool.h
 *	Author:	Jesper Dangaard Brouer <netoptimizer@brouer.com>
 *	Copyright (C) 2016 Red Hat, Inc.
 */


#ifndef _NET_PAGE_POOL_H
#define _NET_PAGE_POOL_H

#include <linux/mm.h> 
#include <linux/ptr_ring.h>
#include <linux/dma-direction.h>

#define PP_FLAG_DMA_MAP 1 
#define PP_FLAG_ALL	PP_FLAG_DMA_MAP


#define PP_ALLOC_CACHE_SIZE	128
#define PP_ALLOC_CACHE_REFILL	64
struct pp_alloc_cache {
	u32 count;
	void *cache[PP_ALLOC_CACHE_SIZE];
};

struct page_pool_params {
	unsigned int	flags;
	unsigned int	order;
	unsigned int	pool_size;
	int		nid;  
	struct device	*dev; 
	enum dma_data_direction dma_dir; 
};

struct page_pool {
	struct rcu_head rcu;
	struct page_pool_params p;

	
	struct pp_alloc_cache alloc ____cacheline_aligned_in_smp;

	
	struct ptr_ring ring;
};

struct page *page_pool_alloc_pages(struct page_pool *pool, gfp_t gfp);

static inline struct page *page_pool_dev_alloc_pages(struct page_pool *pool)
{
	gfp_t gfp = (GFP_ATOMIC | __GFP_NOWARN);

	return page_pool_alloc_pages(pool, gfp);
}

struct page_pool *page_pool_create(const struct page_pool_params *params);

void page_pool_destroy(struct page_pool *pool);


void __page_pool_put_page(struct page_pool *pool,
			  struct page *page, bool allow_direct);

static inline void page_pool_put_page(struct page_pool *pool,
				      struct page *page, bool allow_direct)
{
	
#ifdef CONFIG_PAGE_POOL
	__page_pool_put_page(pool, page, allow_direct);
#endif
}

static inline void page_pool_recycle_direct(struct page_pool *pool,
					    struct page *page)
{
	__page_pool_put_page(pool, page, true);
}

static inline bool is_page_pool_compiled_in(void)
{
#ifdef CONFIG_PAGE_POOL
	return true;
#else
	return false;
#endif
}

#endif 
