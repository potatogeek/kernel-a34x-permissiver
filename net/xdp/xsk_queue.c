// SPDX-License-Identifier: GPL-2.0
/* XDP user-space ring structure
 * Copyright(c) 2018 Intel Corporation.
 */

<<<<<<< HEAD
#include <linux/slab.h>

#include "xsk_queue.h"

void xskq_set_umem(struct xsk_queue *q, struct xdp_umem_props *umem_props)
{
	if (!q)
		return;

	q->umem_props = *umem_props;
}

static u32 xskq_umem_get_ring_size(struct xsk_queue *q)
{
	return sizeof(struct xdp_umem_ring) + q->nentries * sizeof(u64);
}

static u32 xskq_rxtx_get_ring_size(struct xsk_queue *q)
{
	return sizeof(struct xdp_ring) + q->nentries * sizeof(struct xdp_desc);
=======
#include <linux/log2.h>
#include <linux/slab.h>
#include <linux/overflow.h>
#include <net/xdp_sock_drv.h>

#include "xsk_queue.h"

static size_t xskq_get_ring_size(struct xsk_queue *q, bool umem_queue)
{
	struct xdp_umem_ring *umem_ring;
	struct xdp_rxtx_ring *rxtx_ring;

	if (umem_queue)
		return struct_size(umem_ring, desc, q->nentries);
	return struct_size(rxtx_ring, desc, q->nentries);
>>>>>>> upstream/android-13
}

struct xsk_queue *xskq_create(u32 nentries, bool umem_queue)
{
	struct xsk_queue *q;
	gfp_t gfp_flags;
	size_t size;

	q = kzalloc(sizeof(*q), GFP_KERNEL);
	if (!q)
		return NULL;

	q->nentries = nentries;
	q->ring_mask = nentries - 1;

	gfp_flags = GFP_KERNEL | __GFP_ZERO | __GFP_NOWARN |
		    __GFP_COMP  | __GFP_NORETRY;
<<<<<<< HEAD
	size = umem_queue ? xskq_umem_get_ring_size(q) :
	       xskq_rxtx_get_ring_size(q);
=======
	size = xskq_get_ring_size(q, umem_queue);
>>>>>>> upstream/android-13

	q->ring = (struct xdp_ring *)__get_free_pages(gfp_flags,
						      get_order(size));
	if (!q->ring) {
		kfree(q);
		return NULL;
	}

	return q;
}

void xskq_destroy(struct xsk_queue *q)
{
	if (!q)
		return;

	page_frag_free(q->ring);
	kfree(q);
}
