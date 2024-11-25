<<<<<<< HEAD
/*
 * Copyright (c) 2014 Mellanox Technologies. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
=======
/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */
/*
 * Copyright (c) 2014 Mellanox Technologies. All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef IB_UMEM_ODP_H
#define IB_UMEM_ODP_H

#include <rdma/ib_umem.h>
#include <rdma/ib_verbs.h>
<<<<<<< HEAD
#include <linux/interval_tree.h>

struct umem_odp_node {
	u64 __subtree_last;
	struct rb_node rb;
};

struct ib_umem_odp {
	/*
	 * An array of the pages included in the on-demand paging umem.
	 * Indices of pages that are currently not mapped into the device will
	 * contain NULL.
	 */
	struct page		**page_list;
	/*
	 * An array of the same size as page_list, with DMA addresses mapped
	 * for pages the pages in page_list. The lower two bits designate
	 * access permissions. See ODP_READ_ALLOWED_BIT and
	 * ODP_WRITE_ALLOWED_BIT.
=======

struct ib_umem_odp {
	struct ib_umem umem;
	struct mmu_interval_notifier notifier;
	struct pid *tgid;

	/* An array of the pfns included in the on-demand paging umem. */
	unsigned long *pfn_list;

	/*
	 * An array with DMA addresses mapped for pfns in pfn_list.
	 * The lower two bits designate access permissions.
	 * See ODP_READ_ALLOWED_BIT and ODP_WRITE_ALLOWED_BIT.
>>>>>>> upstream/android-13
	 */
	dma_addr_t		*dma_list;
	/*
	 * The umem_mutex protects the page_list and dma_list fields of an ODP
	 * umem, allowing only a single thread to map/unmap pages. The mutex
	 * also protects access to the mmu notifier counters.
	 */
	struct mutex		umem_mutex;
	void			*private; /* for the HW driver to use. */

<<<<<<< HEAD
	/* When false, use the notifier counter in the ucontext struct. */
	bool mn_counters_active;
	int notifiers_seq;
	int notifiers_count;

	/* A linked list of umems that don't have private mmu notifier
	 * counters yet. */
	struct list_head no_private_counters;
	struct ib_umem		*umem;

	/* Tree tracking */
	struct umem_odp_node	interval_tree;

	struct completion	notifier_completion;
	int			dying;
	struct work_struct	work;
};

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING

int ib_umem_odp_get(struct ib_ucontext *context, struct ib_umem *umem,
		    int access);
struct ib_umem *ib_alloc_odp_umem(struct ib_ucontext *context,
				  unsigned long addr,
				  size_t size);

void ib_umem_odp_release(struct ib_umem *umem);
=======
	int npages;

	/*
	 * An implicit odp umem cannot be DMA mapped, has 0 length, and serves
	 * only as an anchor for the driver to hold onto the per_mm. FIXME:
	 * This should be removed and drivers should work with the per_mm
	 * directly.
	 */
	bool is_implicit_odp;

	unsigned int		page_shift;
};

static inline struct ib_umem_odp *to_ib_umem_odp(struct ib_umem *umem)
{
	return container_of(umem, struct ib_umem_odp, umem);
}

/* Returns the first page of an ODP umem. */
static inline unsigned long ib_umem_start(struct ib_umem_odp *umem_odp)
{
	return umem_odp->notifier.interval_tree.start;
}

/* Returns the address of the page after the last one of an ODP umem. */
static inline unsigned long ib_umem_end(struct ib_umem_odp *umem_odp)
{
	return umem_odp->notifier.interval_tree.last + 1;
}

static inline size_t ib_umem_odp_num_pages(struct ib_umem_odp *umem_odp)
{
	return (ib_umem_end(umem_odp) - ib_umem_start(umem_odp)) >>
	       umem_odp->page_shift;
}
>>>>>>> upstream/android-13

/*
 * The lower 2 bits of the DMA address signal the R/W permissions for
 * the entry. To upgrade the permissions, provide the appropriate
 * bitmask to the map_dma_pages function.
 *
 * Be aware that upgrading a mapped address might result in change of
 * the DMA address for the page.
 */
#define ODP_READ_ALLOWED_BIT  (1<<0ULL)
#define ODP_WRITE_ALLOWED_BIT (1<<1ULL)

#define ODP_DMA_ADDR_MASK (~(ODP_READ_ALLOWED_BIT | ODP_WRITE_ALLOWED_BIT))

<<<<<<< HEAD
int ib_umem_odp_map_dma_pages(struct ib_umem *umem, u64 start_offset, u64 bcnt,
			      u64 access_mask, unsigned long current_seq);

void ib_umem_odp_unmap_dma_pages(struct ib_umem *umem, u64 start_offset,
				 u64 bound);

typedef int (*umem_call_back)(struct ib_umem *item, u64 start, u64 end,
			      void *cookie);
/*
 * Call the callback on each ib_umem in the range. Returns the logical or of
 * the return values of the functions called.
 */
int rbt_ib_umem_for_each_in_range(struct rb_root_cached *root,
				  u64 start, u64 end,
				  umem_call_back cb,
				  bool blockable, void *cookie);

/*
 * Find first region intersecting with address range.
 * Return NULL if not found
 */
struct ib_umem_odp *rbt_ib_umem_lookup(struct rb_root_cached *root,
				       u64 addr, u64 length);

static inline int ib_umem_mmu_notifier_retry(struct ib_umem *item,
					     unsigned long mmu_seq)
{
	/*
	 * This code is strongly based on the KVM code from
	 * mmu_notifier_retry. Should be called with
	 * the relevant locks taken (item->odp_data->umem_mutex
	 * and the ucontext umem_mutex semaphore locked for read).
	 */

	/* Do not allow page faults while the new ib_umem hasn't seen a state
	 * with zero notifiers yet, and doesn't have its own valid set of
	 * private counters. */
	if (!item->odp_data->mn_counters_active)
		return 1;

	if (unlikely(item->odp_data->notifiers_count))
		return 1;
	if (item->odp_data->notifiers_seq != mmu_seq)
		return 1;
	return 0;
}

#else /* CONFIG_INFINIBAND_ON_DEMAND_PAGING */

static inline int ib_umem_odp_get(struct ib_ucontext *context,
				  struct ib_umem *umem,
				  int access)
{
	return -EINVAL;
}

static inline struct ib_umem *ib_alloc_odp_umem(struct ib_ucontext *context,
						unsigned long addr,
						size_t size)
=======
#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING

struct ib_umem_odp *
ib_umem_odp_get(struct ib_device *device, unsigned long addr, size_t size,
		int access, const struct mmu_interval_notifier_ops *ops);
struct ib_umem_odp *ib_umem_odp_alloc_implicit(struct ib_device *device,
					       int access);
struct ib_umem_odp *
ib_umem_odp_alloc_child(struct ib_umem_odp *root_umem, unsigned long addr,
			size_t size,
			const struct mmu_interval_notifier_ops *ops);
void ib_umem_odp_release(struct ib_umem_odp *umem_odp);

int ib_umem_odp_map_dma_and_lock(struct ib_umem_odp *umem_odp, u64 start_offset,
				 u64 bcnt, u64 access_mask, bool fault);

void ib_umem_odp_unmap_dma_pages(struct ib_umem_odp *umem_odp, u64 start_offset,
				 u64 bound);

#else /* CONFIG_INFINIBAND_ON_DEMAND_PAGING */

static inline struct ib_umem_odp *
ib_umem_odp_get(struct ib_device *device, unsigned long addr, size_t size,
		int access, const struct mmu_interval_notifier_ops *ops)
>>>>>>> upstream/android-13
{
	return ERR_PTR(-EINVAL);
}

<<<<<<< HEAD
static inline void ib_umem_odp_release(struct ib_umem *umem) {}
=======
static inline void ib_umem_odp_release(struct ib_umem_odp *umem_odp) {}
>>>>>>> upstream/android-13

#endif /* CONFIG_INFINIBAND_ON_DEMAND_PAGING */

#endif /* IB_UMEM_ODP_H */
