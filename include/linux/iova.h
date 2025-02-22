<<<<<<< HEAD
/*
 * Copyright (c) 2006, Intel Corporation.
 *
 * This file is released under the GPLv2.
 *
 * Copyright (C) 2006-2008 Intel Corporation
 * Author: Anil S Keshavamurthy <anil.s.keshavamurthy@intel.com>
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2006, Intel Corporation.
 *
 * Copyright (C) 2006-2008 Intel Corporation
 * Author: Anil S Keshavamurthy <anil.s.keshavamurthy@intel.com>
>>>>>>> upstream/android-13
 */

#ifndef _IOVA_H_
#define _IOVA_H_

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/rbtree.h>
#include <linux/atomic.h>
#include <linux/dma-mapping.h>

/* iova structure */
struct iova {
	struct rb_node	node;
	unsigned long	pfn_hi; /* Highest allocated pfn */
	unsigned long	pfn_lo; /* Lowest allocated pfn */
};

struct iova_magazine;
struct iova_cpu_rcache;

#define IOVA_RANGE_CACHE_MAX_SIZE 6	/* log of max cached IOVA range size (in pages) */
#define MAX_GLOBAL_MAGS 32	/* magazines per bin */

struct iova_rcache {
	spinlock_t lock;
	unsigned long depot_size;
	struct iova_magazine *depot[MAX_GLOBAL_MAGS];
	struct iova_cpu_rcache __percpu *cpu_rcaches;
};

struct iova_domain;

/* Call-Back from IOVA code into IOMMU drivers */
typedef void (* iova_flush_cb)(struct iova_domain *domain);

/* Destructor for per-entry data */
typedef void (* iova_entry_dtor)(unsigned long data);

/* Number of entries per Flush Queue */
#define IOVA_FQ_SIZE	256

/* Timeout (in ms) after which entries are flushed from the Flush-Queue */
#define IOVA_FQ_TIMEOUT	10

/* Flush Queue entry for defered flushing */
struct iova_fq_entry {
	unsigned long iova_pfn;
	unsigned long pages;
	unsigned long data;
	u64 counter; /* Flush counter when this entrie was added */
};

/* Per-CPU Flush Queue structure */
struct iova_fq {
	struct iova_fq_entry entries[IOVA_FQ_SIZE];
	unsigned head, tail;
	spinlock_t lock;
};

/* holds all the iova translations for a domain */
struct iova_domain {
	spinlock_t	iova_rbtree_lock; /* Lock to protect update of rbtree */
	struct rb_root	rbroot;		/* iova domain rbtree root */
	struct rb_node	*cached_node;	/* Save last alloced node */
	struct rb_node	*cached32_node; /* Save last 32-bit alloced node */
	unsigned long	granule;	/* pfn granularity for this domain */
	unsigned long	start_pfn;	/* Lower limit for this domain */
<<<<<<< HEAD
	unsigned long	end_pfn;        /* Upper limit for this domain */
	unsigned long	dma_32bit_pfn;
=======
	unsigned long	dma_32bit_pfn;
	unsigned long	max32_alloc_size; /* Size of last failed allocation */
	struct iova_fq __percpu *fq;	/* Flush Queue */

	atomic64_t	fq_flush_start_cnt;	/* Number of TLB flushes that
						   have been started */

	atomic64_t	fq_flush_finish_cnt;	/* Number of TLB flushes that
						   have been finished */

>>>>>>> upstream/android-13
	struct iova	anchor;		/* rbtree lookup anchor */
	struct iova_rcache rcaches[IOVA_RANGE_CACHE_MAX_SIZE];	/* IOVA range caches */

	iova_flush_cb	flush_cb;	/* Call-Back function to flush IOMMU
					   TLBs */

	iova_entry_dtor entry_dtor;	/* IOMMU driver specific destructor for
					   iova entry */

<<<<<<< HEAD
	struct iova_fq __percpu *fq;	/* Flush Queue */

	atomic64_t	fq_flush_start_cnt;	/* Number of TLB flushes that
						   have been started */

	atomic64_t	fq_flush_finish_cnt;	/* Number of TLB flushes that
						   have been finished */

=======
>>>>>>> upstream/android-13
	struct timer_list fq_timer;		/* Timer to regularily empty the
						   flush-queues */
	atomic_t fq_timer_on;			/* 1 when timer is active, 0
						   when not */
<<<<<<< HEAD
=======
	struct hlist_node	cpuhp_dead;
>>>>>>> upstream/android-13
	bool best_fit;
};

static inline unsigned long iova_size(struct iova *iova)
{
	return iova->pfn_hi - iova->pfn_lo + 1;
}

static inline unsigned long iova_shift(struct iova_domain *iovad)
{
	return __ffs(iovad->granule);
}

static inline unsigned long iova_mask(struct iova_domain *iovad)
{
	return iovad->granule - 1;
}

static inline size_t iova_offset(struct iova_domain *iovad, dma_addr_t iova)
{
	return iova & iova_mask(iovad);
}

static inline size_t iova_align(struct iova_domain *iovad, size_t size)
{
	return ALIGN(size, iovad->granule);
}

static inline dma_addr_t iova_dma_addr(struct iova_domain *iovad, struct iova *iova)
{
	return (dma_addr_t)iova->pfn_lo << iova_shift(iovad);
}

static inline unsigned long iova_pfn(struct iova_domain *iovad, dma_addr_t iova)
{
	return iova >> iova_shift(iovad);
}

#if IS_ENABLED(CONFIG_IOMMU_IOVA)
int iova_cache_get(void);
void iova_cache_put(void);

<<<<<<< HEAD
struct iova *alloc_iova_mem(void);
void free_iova_mem(struct iova *iova);
=======
>>>>>>> upstream/android-13
void free_iova(struct iova_domain *iovad, unsigned long pfn);
void __free_iova(struct iova_domain *iovad, struct iova *iova);
struct iova *alloc_iova(struct iova_domain *iovad, unsigned long size,
	unsigned long limit_pfn,
	bool size_aligned);
void free_iova_fast(struct iova_domain *iovad, unsigned long pfn,
		    unsigned long size);
void queue_iova(struct iova_domain *iovad,
		unsigned long pfn, unsigned long pages,
		unsigned long data);
<<<<<<< HEAD
#ifdef CONFIG_MTK_IOMMU_V2
unsigned long
alloc_iova_fast(struct iova_domain *iovad, unsigned long size,
		unsigned long limit_pfn, bool size_align);
#else
unsigned long alloc_iova_fast(struct iova_domain *iovad, unsigned long size,
			      unsigned long limit_pfn, bool flush_rcache);
#endif
struct iova *reserve_iova(struct iova_domain *iovad, unsigned long pfn_lo,
	unsigned long pfn_hi);
void copy_reserved_iova(struct iova_domain *from, struct iova_domain *to);
#ifdef CONFIG_MTK_IOMMU_V2
void iovad_scan_reserved_iova(void *arg,
		struct iova_domain *iovad,
		void (*f)(void *domain, unsigned long start,
			unsigned long end, unsigned long size,
			unsigned long target),
		unsigned long target);
#endif
void init_iova_domain(struct iova_domain *iovad, unsigned long granule,
	unsigned long start_pfn);
bool has_iova_flush_queue(struct iova_domain *iovad);
=======
unsigned long alloc_iova_fast(struct iova_domain *iovad, unsigned long size,
			      unsigned long limit_pfn, bool flush_rcache);
struct iova *reserve_iova(struct iova_domain *iovad, unsigned long pfn_lo,
	unsigned long pfn_hi);
void init_iova_domain(struct iova_domain *iovad, unsigned long granule,
	unsigned long start_pfn);
>>>>>>> upstream/android-13
int init_iova_flush_queue(struct iova_domain *iovad,
			  iova_flush_cb flush_cb, iova_entry_dtor entry_dtor);
struct iova *find_iova(struct iova_domain *iovad, unsigned long pfn);
void put_iova_domain(struct iova_domain *iovad);
<<<<<<< HEAD
struct iova *split_and_remove_iova(struct iova_domain *iovad,
	struct iova *iova, unsigned long pfn_lo, unsigned long pfn_hi);
void free_cpu_cached_iovas(unsigned int cpu, struct iova_domain *iovad);
=======
>>>>>>> upstream/android-13
#else
static inline int iova_cache_get(void)
{
	return -ENOTSUPP;
}

static inline void iova_cache_put(void)
{
}

<<<<<<< HEAD
static inline struct iova *alloc_iova_mem(void)
{
	return NULL;
}

static inline void free_iova_mem(struct iova *iova)
{
}

=======
>>>>>>> upstream/android-13
static inline void free_iova(struct iova_domain *iovad, unsigned long pfn)
{
}

static inline void __free_iova(struct iova_domain *iovad, struct iova *iova)
{
}

static inline struct iova *alloc_iova(struct iova_domain *iovad,
				      unsigned long size,
				      unsigned long limit_pfn,
				      bool size_aligned)
{
	return NULL;
}

static inline void free_iova_fast(struct iova_domain *iovad,
				  unsigned long pfn,
				  unsigned long size)
{
}

static inline void queue_iova(struct iova_domain *iovad,
			      unsigned long pfn, unsigned long pages,
			      unsigned long data)
{
}

static inline unsigned long alloc_iova_fast(struct iova_domain *iovad,
					    unsigned long size,
					    unsigned long limit_pfn,
					    bool flush_rcache)
{
	return 0;
}

static inline struct iova *reserve_iova(struct iova_domain *iovad,
					unsigned long pfn_lo,
					unsigned long pfn_hi)
{
	return NULL;
}

<<<<<<< HEAD
static inline void copy_reserved_iova(struct iova_domain *from,
				      struct iova_domain *to)
{
}

=======
>>>>>>> upstream/android-13
static inline void init_iova_domain(struct iova_domain *iovad,
				    unsigned long granule,
				    unsigned long start_pfn)
{
}

<<<<<<< HEAD
static inline bool has_iova_flush_queue(struct iova_domain *iovad)
{
	return false;
}

=======
>>>>>>> upstream/android-13
static inline int init_iova_flush_queue(struct iova_domain *iovad,
					iova_flush_cb flush_cb,
					iova_entry_dtor entry_dtor)
{
	return -ENODEV;
}

static inline struct iova *find_iova(struct iova_domain *iovad,
				     unsigned long pfn)
{
	return NULL;
}

static inline void put_iova_domain(struct iova_domain *iovad)
{
}

<<<<<<< HEAD
static inline struct iova *split_and_remove_iova(struct iova_domain *iovad,
						 struct iova *iova,
						 unsigned long pfn_lo,
						 unsigned long pfn_hi)
{
	return NULL;
}

static inline void free_cpu_cached_iovas(unsigned int cpu,
					 struct iova_domain *iovad)
{
}
=======
>>>>>>> upstream/android-13
#endif

#endif
