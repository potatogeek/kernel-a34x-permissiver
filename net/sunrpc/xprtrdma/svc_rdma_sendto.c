// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * Copyright (c) 2016-2018 Oracle. All rights reserved.
 * Copyright (c) 2014 Open Grid Computing, Inc. All rights reserved.
 * Copyright (c) 2005-2006 Network Appliance, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of the Network Appliance, Inc. nor the names of
 *      its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written
 *      permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Tom Tucker <tom@opengridcomputing.com>
 */

/* Operation
 *
 * The main entry point is svc_rdma_sendto. This is called by the
 * RPC server when an RPC Reply is ready to be transmitted to a client.
 *
 * The passed-in svc_rqst contains a struct xdr_buf which holds an
 * XDR-encoded RPC Reply message. sendto must construct the RPC-over-RDMA
 * transport header, post all Write WRs needed for this Reply, then post
 * a Send WR conveying the transport header and the RPC message itself to
 * the client.
 *
 * svc_rdma_sendto must fully transmit the Reply before returning, as
 * the svc_rqst will be recycled as soon as sendto returns. Remaining
 * resources referred to by the svc_rqst are also recycled at that time.
 * Therefore any resources that must remain longer must be detached
 * from the svc_rqst and released later.
 *
 * Page Management
 *
 * The I/O that performs Reply transmission is asynchronous, and may
 * complete well after sendto returns. Thus pages under I/O must be
 * removed from the svc_rqst before sendto returns.
 *
 * The logic here depends on Send Queue and completion ordering. Since
 * the Send WR is always posted last, it will always complete last. Thus
 * when it completes, it is guaranteed that all previous Write WRs have
 * also completed.
 *
 * Write WRs are constructed and posted. Each Write segment gets its own
 * svc_rdma_rw_ctxt, allowing the Write completion handler to find and
 * DMA-unmap the pages under I/O for that Write segment. The Write
 * completion handler does not release any pages.
 *
 * When the Send WR is constructed, it also gets its own svc_rdma_send_ctxt.
 * The ownership of all of the Reply's pages are transferred into that
 * ctxt, the Send WR is posted, and sendto returns.
 *
 * The svc_rdma_send_ctxt is presented when the Send WR completes. The
 * Send completion handler finally releases the Reply's pages.
 *
 * This mechanism also assumes that completions on the transport's Send
 * Completion Queue do not run in parallel. Otherwise a Write completion
 * and Send completion running at the same time could release pages that
 * are still DMA-mapped.
 *
 * Error Handling
 *
 * - If the Send WR is posted successfully, it will either complete
 *   successfully, or get flushed. Either way, the Send completion
 *   handler releases the Reply's pages.
 * - If the Send WR cannot be not posted, the forward path releases
 *   the Reply's pages.
 *
 * This handles the case, without the use of page reference counting,
 * where two different Write segments send portions of the same page.
 */

#include <linux/spinlock.h>
#include <asm/unaligned.h>

#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>

#include <linux/sunrpc/debug.h>
<<<<<<< HEAD
#include <linux/sunrpc/rpc_rdma.h>
=======
>>>>>>> upstream/android-13
#include <linux/sunrpc/svc_rdma.h>

#include "xprt_rdma.h"
#include <trace/events/rpcrdma.h>

<<<<<<< HEAD
#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

static void svc_rdma_wc_send(struct ib_cq *cq, struct ib_wc *wc);

static inline struct svc_rdma_send_ctxt *
svc_rdma_next_send_ctxt(struct list_head *list)
{
	return list_first_entry_or_null(list, struct svc_rdma_send_ctxt,
					sc_list);
=======
static void svc_rdma_wc_send(struct ib_cq *cq, struct ib_wc *wc);

static void svc_rdma_send_cid_init(struct svcxprt_rdma *rdma,
				   struct rpc_rdma_cid *cid)
{
	cid->ci_queue_id = rdma->sc_sq_cq->res.id;
	cid->ci_completion_id = atomic_inc_return(&rdma->sc_completion_ids);
>>>>>>> upstream/android-13
}

static struct svc_rdma_send_ctxt *
svc_rdma_send_ctxt_alloc(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_send_ctxt *ctxt;
	dma_addr_t addr;
	void *buffer;
	size_t size;
	int i;

	size = sizeof(*ctxt);
	size += rdma->sc_max_send_sges * sizeof(struct ib_sge);
	ctxt = kmalloc(size, GFP_KERNEL);
	if (!ctxt)
		goto fail0;
	buffer = kmalloc(rdma->sc_max_req_size, GFP_KERNEL);
	if (!buffer)
		goto fail1;
	addr = ib_dma_map_single(rdma->sc_pd->device, buffer,
				 rdma->sc_max_req_size, DMA_TO_DEVICE);
	if (ib_dma_mapping_error(rdma->sc_pd->device, addr))
		goto fail2;

<<<<<<< HEAD
=======
	svc_rdma_send_cid_init(rdma, &ctxt->sc_cid);

>>>>>>> upstream/android-13
	ctxt->sc_send_wr.next = NULL;
	ctxt->sc_send_wr.wr_cqe = &ctxt->sc_cqe;
	ctxt->sc_send_wr.sg_list = ctxt->sc_sges;
	ctxt->sc_send_wr.send_flags = IB_SEND_SIGNALED;
<<<<<<< HEAD
	ctxt->sc_cqe.done = svc_rdma_wc_send;
	ctxt->sc_xprt_buf = buffer;
=======
	init_completion(&ctxt->sc_done);
	ctxt->sc_cqe.done = svc_rdma_wc_send;
	ctxt->sc_xprt_buf = buffer;
	xdr_buf_init(&ctxt->sc_hdrbuf, ctxt->sc_xprt_buf,
		     rdma->sc_max_req_size);
>>>>>>> upstream/android-13
	ctxt->sc_sges[0].addr = addr;

	for (i = 0; i < rdma->sc_max_send_sges; i++)
		ctxt->sc_sges[i].lkey = rdma->sc_pd->local_dma_lkey;
	return ctxt;

fail2:
	kfree(buffer);
fail1:
	kfree(ctxt);
fail0:
	return NULL;
}

/**
 * svc_rdma_send_ctxts_destroy - Release all send_ctxt's for an xprt
 * @rdma: svcxprt_rdma being torn down
 *
 */
void svc_rdma_send_ctxts_destroy(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_send_ctxt *ctxt;
<<<<<<< HEAD

	while ((ctxt = svc_rdma_next_send_ctxt(&rdma->sc_send_ctxts))) {
		list_del(&ctxt->sc_list);
=======
	struct llist_node *node;

	while ((node = llist_del_first(&rdma->sc_send_ctxts)) != NULL) {
		ctxt = llist_entry(node, struct svc_rdma_send_ctxt, sc_node);
>>>>>>> upstream/android-13
		ib_dma_unmap_single(rdma->sc_pd->device,
				    ctxt->sc_sges[0].addr,
				    rdma->sc_max_req_size,
				    DMA_TO_DEVICE);
		kfree(ctxt->sc_xprt_buf);
		kfree(ctxt);
	}
}

/**
 * svc_rdma_send_ctxt_get - Get a free send_ctxt
 * @rdma: controlling svcxprt_rdma
 *
 * Returns a ready-to-use send_ctxt, or NULL if none are
 * available and a fresh one cannot be allocated.
 */
struct svc_rdma_send_ctxt *svc_rdma_send_ctxt_get(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_send_ctxt *ctxt;
<<<<<<< HEAD

	spin_lock(&rdma->sc_send_lock);
	ctxt = svc_rdma_next_send_ctxt(&rdma->sc_send_ctxts);
	if (!ctxt)
		goto out_empty;
	list_del(&ctxt->sc_list);
	spin_unlock(&rdma->sc_send_lock);

out:
	ctxt->sc_send_wr.num_sge = 0;
	ctxt->sc_cur_sge_no = 0;
	ctxt->sc_page_count = 0;
=======
	struct llist_node *node;

	spin_lock(&rdma->sc_send_lock);
	node = llist_del_first(&rdma->sc_send_ctxts);
	if (!node)
		goto out_empty;
	ctxt = llist_entry(node, struct svc_rdma_send_ctxt, sc_node);
	spin_unlock(&rdma->sc_send_lock);

out:
	rpcrdma_set_xdrlen(&ctxt->sc_hdrbuf, 0);
	xdr_init_encode(&ctxt->sc_stream, &ctxt->sc_hdrbuf,
			ctxt->sc_xprt_buf, NULL);

	ctxt->sc_send_wr.num_sge = 0;
	ctxt->sc_cur_sge_no = 0;
>>>>>>> upstream/android-13
	return ctxt;

out_empty:
	spin_unlock(&rdma->sc_send_lock);
	ctxt = svc_rdma_send_ctxt_alloc(rdma);
	if (!ctxt)
		return NULL;
	goto out;
}

/**
 * svc_rdma_send_ctxt_put - Return send_ctxt to free list
 * @rdma: controlling svcxprt_rdma
 * @ctxt: object to return to the free list
<<<<<<< HEAD
 *
 * Pages left in sc_pages are DMA unmapped and released.
=======
>>>>>>> upstream/android-13
 */
void svc_rdma_send_ctxt_put(struct svcxprt_rdma *rdma,
			    struct svc_rdma_send_ctxt *ctxt)
{
	struct ib_device *device = rdma->sc_cm_id->device;
	unsigned int i;

	/* The first SGE contains the transport header, which
	 * remains mapped until @ctxt is destroyed.
	 */
<<<<<<< HEAD
	for (i = 1; i < ctxt->sc_send_wr.num_sge; i++)
=======
	for (i = 1; i < ctxt->sc_send_wr.num_sge; i++) {
>>>>>>> upstream/android-13
		ib_dma_unmap_page(device,
				  ctxt->sc_sges[i].addr,
				  ctxt->sc_sges[i].length,
				  DMA_TO_DEVICE);
<<<<<<< HEAD

	for (i = 0; i < ctxt->sc_page_count; ++i)
		put_page(ctxt->sc_pages[i]);

	spin_lock(&rdma->sc_send_lock);
	list_add(&ctxt->sc_list, &rdma->sc_send_ctxts);
	spin_unlock(&rdma->sc_send_lock);
=======
		trace_svcrdma_dma_unmap_page(rdma,
					     ctxt->sc_sges[i].addr,
					     ctxt->sc_sges[i].length);
	}

	llist_add(&ctxt->sc_node, &rdma->sc_send_ctxts);
}

/**
 * svc_rdma_wake_send_waiters - manage Send Queue accounting
 * @rdma: controlling transport
 * @avail: Number of additional SQEs that are now available
 *
 */
void svc_rdma_wake_send_waiters(struct svcxprt_rdma *rdma, int avail)
{
	atomic_add(avail, &rdma->sc_sq_avail);
	smp_mb__after_atomic();
	if (unlikely(waitqueue_active(&rdma->sc_send_wait)))
		wake_up(&rdma->sc_send_wait);
>>>>>>> upstream/android-13
}

/**
 * svc_rdma_wc_send - Invoked by RDMA provider for each polled Send WC
 * @cq: Completion Queue context
 * @wc: Work Completion object
 *
 * NB: The svc_xprt/svcxprt_rdma is pinned whenever it's possible that
 * the Send completion handler could be running.
 */
static void svc_rdma_wc_send(struct ib_cq *cq, struct ib_wc *wc)
{
	struct svcxprt_rdma *rdma = cq->cq_context;
	struct ib_cqe *cqe = wc->wr_cqe;
<<<<<<< HEAD
	struct svc_rdma_send_ctxt *ctxt;

	trace_svcrdma_wc_send(wc);

	atomic_inc(&rdma->sc_sq_avail);
	wake_up(&rdma->sc_send_wait);

	ctxt = container_of(cqe, struct svc_rdma_send_ctxt, sc_cqe);
	svc_rdma_send_ctxt_put(rdma, ctxt);

	if (unlikely(wc->status != IB_WC_SUCCESS)) {
		set_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags);
		svc_xprt_enqueue(&rdma->sc_xprt);
		if (wc->status != IB_WC_WR_FLUSH_ERR)
			pr_err("svcrdma: Send: %s (%u/0x%x)\n",
			       ib_wc_status_msg(wc->status),
			       wc->status, wc->vendor_err);
	}

	svc_xprt_put(&rdma->sc_xprt);
=======
	struct svc_rdma_send_ctxt *ctxt =
		container_of(cqe, struct svc_rdma_send_ctxt, sc_cqe);

	trace_svcrdma_wc_send(wc, &ctxt->sc_cid);

	svc_rdma_wake_send_waiters(rdma, 1);
	complete(&ctxt->sc_done);

	if (unlikely(wc->status != IB_WC_SUCCESS))
		svc_xprt_deferred_close(&rdma->sc_xprt);
>>>>>>> upstream/android-13
}

/**
 * svc_rdma_send - Post a single Send WR
 * @rdma: transport on which to post the WR
<<<<<<< HEAD
 * @wr: prepared Send WR to post
 *
 * Returns zero the Send WR was posted successfully. Otherwise, a
 * negative errno is returned.
 */
int svc_rdma_send(struct svcxprt_rdma *rdma, struct ib_send_wr *wr)
{
	int ret;

	might_sleep();
=======
 * @ctxt: send ctxt with a Send WR ready to post
 *
 * Returns zero if the Send WR was posted successfully. Otherwise, a
 * negative errno is returned.
 */
int svc_rdma_send(struct svcxprt_rdma *rdma, struct svc_rdma_send_ctxt *ctxt)
{
	struct ib_send_wr *wr = &ctxt->sc_send_wr;
	int ret;

	reinit_completion(&ctxt->sc_done);

	/* Sync the transport header buffer */
	ib_dma_sync_single_for_device(rdma->sc_pd->device,
				      wr->sg_list[0].addr,
				      wr->sg_list[0].length,
				      DMA_TO_DEVICE);
>>>>>>> upstream/android-13

	/* If the SQ is full, wait until an SQ entry is available */
	while (1) {
		if ((atomic_dec_return(&rdma->sc_sq_avail) < 0)) {
<<<<<<< HEAD
			atomic_inc(&rdma_stat_sq_starve);
=======
			percpu_counter_inc(&svcrdma_stat_sq_starve);
>>>>>>> upstream/android-13
			trace_svcrdma_sq_full(rdma);
			atomic_inc(&rdma->sc_sq_avail);
			wait_event(rdma->sc_send_wait,
				   atomic_read(&rdma->sc_sq_avail) > 1);
			if (test_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags))
				return -ENOTCONN;
			trace_svcrdma_sq_retry(rdma);
			continue;
		}

<<<<<<< HEAD
		svc_xprt_get(&rdma->sc_xprt);
		trace_svcrdma_post_send(wr);
=======
		trace_svcrdma_post_send(ctxt);
>>>>>>> upstream/android-13
		ret = ib_post_send(rdma->sc_qp, wr, NULL);
		if (ret)
			break;
		return 0;
	}

	trace_svcrdma_sq_post_err(rdma, ret);
<<<<<<< HEAD
	set_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags);
	svc_xprt_put(&rdma->sc_xprt);
=======
	svc_xprt_deferred_close(&rdma->sc_xprt);
>>>>>>> upstream/android-13
	wake_up(&rdma->sc_send_wait);
	return ret;
}

<<<<<<< HEAD
static u32 xdr_padsize(u32 len)
{
	return (len & 3) ? (4 - (len & 3)) : 0;
}

/* Returns length of transport header, in bytes.
 */
static unsigned int svc_rdma_reply_hdr_len(__be32 *rdma_resp)
{
	unsigned int nsegs;
	__be32 *p;

	p = rdma_resp;

	/* RPC-over-RDMA V1 replies never have a Read list. */
	p += rpcrdma_fixed_maxsz + 1;

	/* Skip Write list. */
	while (*p++ != xdr_zero) {
		nsegs = be32_to_cpup(p++);
		p += nsegs * rpcrdma_segment_maxsz;
	}

	/* Skip Reply chunk. */
	if (*p++ != xdr_zero) {
		nsegs = be32_to_cpup(p++);
		p += nsegs * rpcrdma_segment_maxsz;
	}

	return (unsigned long)p - (unsigned long)rdma_resp;
}

/* One Write chunk is copied from Call transport header to Reply
 * transport header. Each segment's length field is updated to
 * reflect number of bytes consumed in the segment.
 *
 * Returns number of segments in this chunk.
 */
static unsigned int xdr_encode_write_chunk(__be32 *dst, __be32 *src,
					   unsigned int remaining)
{
	unsigned int i, nsegs;
	u32 seg_len;

	/* Write list discriminator */
	*dst++ = *src++;

	/* number of segments in this chunk */
	nsegs = be32_to_cpup(src);
	*dst++ = *src++;

	for (i = nsegs; i; i--) {
		/* segment's RDMA handle */
		*dst++ = *src++;

		/* bytes returned in this segment */
		seg_len = be32_to_cpu(*src);
		if (remaining >= seg_len) {
			/* entire segment was consumed */
			*dst = *src;
			remaining -= seg_len;
		} else {
			/* segment only partly filled */
			*dst = cpu_to_be32(remaining);
			remaining = 0;
		}
		dst++; src++;

		/* segment's RDMA offset */
		*dst++ = *src++;
		*dst++ = *src++;
	}

	return nsegs;
}

/* The client provided a Write list in the Call message. Fill in
 * the segments in the first Write chunk in the Reply's transport
 * header with the number of bytes consumed in each segment.
 * Remaining chunks are returned unused.
 *
 * Assumptions:
 *  - Client has provided only one Write chunk
 */
static void svc_rdma_xdr_encode_write_list(__be32 *rdma_resp, __be32 *wr_ch,
					   unsigned int consumed)
{
	unsigned int nsegs;
	__be32 *p, *q;

	/* RPC-over-RDMA V1 replies never have a Read list. */
	p = rdma_resp + rpcrdma_fixed_maxsz + 1;

	q = wr_ch;
	while (*q != xdr_zero) {
		nsegs = xdr_encode_write_chunk(p, q, consumed);
		q += 2 + nsegs * rpcrdma_segment_maxsz;
		p += 2 + nsegs * rpcrdma_segment_maxsz;
		consumed = 0;
	}

	/* Terminate Write list */
	*p++ = xdr_zero;

	/* Reply chunk discriminator; may be replaced later */
	*p = xdr_zero;
}

/* The client provided a Reply chunk in the Call message. Fill in
 * the segments in the Reply chunk in the Reply message with the
 * number of bytes consumed in each segment.
 *
 * Assumptions:
 * - Reply can always fit in the provided Reply chunk
 */
static void svc_rdma_xdr_encode_reply_chunk(__be32 *rdma_resp, __be32 *rp_ch,
					    unsigned int consumed)
{
	__be32 *p;

	/* Find the Reply chunk in the Reply's xprt header.
	 * RPC-over-RDMA V1 replies never have a Read list.
	 */
	p = rdma_resp + rpcrdma_fixed_maxsz + 1;

	/* Skip past Write list */
	while (*p++ != xdr_zero)
		p += 1 + be32_to_cpup(p) * rpcrdma_segment_maxsz;

	xdr_encode_write_chunk(p, rp_ch, consumed);
}

/* Parse the RPC Call's transport header.
 */
static void svc_rdma_get_write_arrays(__be32 *rdma_argp,
				      __be32 **write, __be32 **reply)
{
	__be32 *p;

	p = rdma_argp + rpcrdma_fixed_maxsz;

	/* Read list */
	while (*p++ != xdr_zero)
		p += 5;

	/* Write list */
	if (*p != xdr_zero) {
		*write = p;
		while (*p++ != xdr_zero)
			p += 1 + be32_to_cpu(*p) * 4;
	} else {
		*write = NULL;
		p++;
	}

	/* Reply chunk */
	if (*p != xdr_zero)
		*reply = p;
	else
		*reply = NULL;
}

/* RPC-over-RDMA Version One private extension: Remote Invalidation.
 * Responder's choice: requester signals it can handle Send With
 * Invalidate, and responder chooses one rkey to invalidate.
 *
 * Find a candidate rkey to invalidate when sending a reply.  Picks the
 * first R_key it finds in the chunk lists.
 *
 * Returns zero if RPC's chunk lists are empty.
 */
static u32 svc_rdma_get_inv_rkey(__be32 *rdma_argp,
				 __be32 *wr_lst, __be32 *rp_ch)
{
	__be32 *p;

	p = rdma_argp + rpcrdma_fixed_maxsz;
	if (*p != xdr_zero)
		p += 2;
	else if (wr_lst && be32_to_cpup(wr_lst + 1))
		p = wr_lst + 2;
	else if (rp_ch && be32_to_cpup(rp_ch + 1))
		p = rp_ch + 2;
	else
		return 0;
	return be32_to_cpup(p);
}

static int svc_rdma_dma_map_page(struct svcxprt_rdma *rdma,
				 struct svc_rdma_send_ctxt *ctxt,
				 struct page *page,
				 unsigned long offset,
				 unsigned int len)
{
	struct ib_device *dev = rdma->sc_cm_id->device;
	dma_addr_t dma_addr;

=======
/**
 * svc_rdma_encode_read_list - Encode RPC Reply's Read chunk list
 * @sctxt: Send context for the RPC Reply
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Reply Read list
 *   %-EMSGSIZE on XDR buffer overflow
 */
static ssize_t svc_rdma_encode_read_list(struct svc_rdma_send_ctxt *sctxt)
{
	/* RPC-over-RDMA version 1 replies never have a Read list. */
	return xdr_stream_encode_item_absent(&sctxt->sc_stream);
}

/**
 * svc_rdma_encode_write_segment - Encode one Write segment
 * @sctxt: Send context for the RPC Reply
 * @chunk: Write chunk to push
 * @remaining: remaining bytes of the payload left in the Write chunk
 * @segno: which segment in the chunk
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Write segment, and updates @remaining
 *   %-EMSGSIZE on XDR buffer overflow
 */
static ssize_t svc_rdma_encode_write_segment(struct svc_rdma_send_ctxt *sctxt,
					     const struct svc_rdma_chunk *chunk,
					     u32 *remaining, unsigned int segno)
{
	const struct svc_rdma_segment *segment = &chunk->ch_segments[segno];
	const size_t len = rpcrdma_segment_maxsz * sizeof(__be32);
	u32 length;
	__be32 *p;

	p = xdr_reserve_space(&sctxt->sc_stream, len);
	if (!p)
		return -EMSGSIZE;

	length = min_t(u32, *remaining, segment->rs_length);
	*remaining -= length;
	xdr_encode_rdma_segment(p, segment->rs_handle, length,
				segment->rs_offset);
	trace_svcrdma_encode_wseg(sctxt, segno, segment->rs_handle, length,
				  segment->rs_offset);
	return len;
}

/**
 * svc_rdma_encode_write_chunk - Encode one Write chunk
 * @sctxt: Send context for the RPC Reply
 * @chunk: Write chunk to push
 *
 * Copy a Write chunk from the Call transport header to the
 * Reply transport header. Update each segment's length field
 * to reflect the number of bytes written in that segment.
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Write chunk
 *   %-EMSGSIZE on XDR buffer overflow
 */
static ssize_t svc_rdma_encode_write_chunk(struct svc_rdma_send_ctxt *sctxt,
					   const struct svc_rdma_chunk *chunk)
{
	u32 remaining = chunk->ch_payload_length;
	unsigned int segno;
	ssize_t len, ret;

	len = 0;
	ret = xdr_stream_encode_item_present(&sctxt->sc_stream);
	if (ret < 0)
		return ret;
	len += ret;

	ret = xdr_stream_encode_u32(&sctxt->sc_stream, chunk->ch_segcount);
	if (ret < 0)
		return ret;
	len += ret;

	for (segno = 0; segno < chunk->ch_segcount; segno++) {
		ret = svc_rdma_encode_write_segment(sctxt, chunk, &remaining, segno);
		if (ret < 0)
			return ret;
		len += ret;
	}

	return len;
}

/**
 * svc_rdma_encode_write_list - Encode RPC Reply's Write chunk list
 * @rctxt: Reply context with information about the RPC Call
 * @sctxt: Send context for the RPC Reply
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Reply's Write list
 *   %-EMSGSIZE on XDR buffer overflow
 */
static ssize_t svc_rdma_encode_write_list(struct svc_rdma_recv_ctxt *rctxt,
					  struct svc_rdma_send_ctxt *sctxt)
{
	struct svc_rdma_chunk *chunk;
	ssize_t len, ret;

	len = 0;
	pcl_for_each_chunk(chunk, &rctxt->rc_write_pcl) {
		ret = svc_rdma_encode_write_chunk(sctxt, chunk);
		if (ret < 0)
			return ret;
		len += ret;
	}

	/* Terminate the Write list */
	ret = xdr_stream_encode_item_absent(&sctxt->sc_stream);
	if (ret < 0)
		return ret;

	return len + ret;
}

/**
 * svc_rdma_encode_reply_chunk - Encode RPC Reply's Reply chunk
 * @rctxt: Reply context with information about the RPC Call
 * @sctxt: Send context for the RPC Reply
 * @length: size in bytes of the payload in the Reply chunk
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Reply's Reply chunk
 *   %-EMSGSIZE on XDR buffer overflow
 *   %-E2BIG if the RPC message is larger than the Reply chunk
 */
static ssize_t
svc_rdma_encode_reply_chunk(struct svc_rdma_recv_ctxt *rctxt,
			    struct svc_rdma_send_ctxt *sctxt,
			    unsigned int length)
{
	struct svc_rdma_chunk *chunk;

	if (pcl_is_empty(&rctxt->rc_reply_pcl))
		return xdr_stream_encode_item_absent(&sctxt->sc_stream);

	chunk = pcl_first_chunk(&rctxt->rc_reply_pcl);
	if (length > chunk->ch_length)
		return -E2BIG;

	chunk->ch_payload_length = length;
	return svc_rdma_encode_write_chunk(sctxt, chunk);
}

struct svc_rdma_map_data {
	struct svcxprt_rdma		*md_rdma;
	struct svc_rdma_send_ctxt	*md_ctxt;
};

/**
 * svc_rdma_page_dma_map - DMA map one page
 * @data: pointer to arguments
 * @page: struct page to DMA map
 * @offset: offset into the page
 * @len: number of bytes to map
 *
 * Returns:
 *   %0 if DMA mapping was successful
 *   %-EIO if the page cannot be DMA mapped
 */
static int svc_rdma_page_dma_map(void *data, struct page *page,
				 unsigned long offset, unsigned int len)
{
	struct svc_rdma_map_data *args = data;
	struct svcxprt_rdma *rdma = args->md_rdma;
	struct svc_rdma_send_ctxt *ctxt = args->md_ctxt;
	struct ib_device *dev = rdma->sc_cm_id->device;
	dma_addr_t dma_addr;

	++ctxt->sc_cur_sge_no;

>>>>>>> upstream/android-13
	dma_addr = ib_dma_map_page(dev, page, offset, len, DMA_TO_DEVICE);
	if (ib_dma_mapping_error(dev, dma_addr))
		goto out_maperr;

<<<<<<< HEAD
=======
	trace_svcrdma_dma_map_page(rdma, dma_addr, len);
>>>>>>> upstream/android-13
	ctxt->sc_sges[ctxt->sc_cur_sge_no].addr = dma_addr;
	ctxt->sc_sges[ctxt->sc_cur_sge_no].length = len;
	ctxt->sc_send_wr.num_sge++;
	return 0;

out_maperr:
<<<<<<< HEAD
	trace_svcrdma_dma_map_page(rdma, page);
	return -EIO;
}

/* ib_dma_map_page() is used here because svc_rdma_dma_unmap()
 * handles DMA-unmap and it uses ib_dma_unmap_page() exclusively.
 */
static int svc_rdma_dma_map_buf(struct svcxprt_rdma *rdma,
				struct svc_rdma_send_ctxt *ctxt,
				unsigned char *base,
				unsigned int len)
{
	return svc_rdma_dma_map_page(rdma, ctxt, virt_to_page(base),
				     offset_in_page(base), len);
}

/**
 * svc_rdma_sync_reply_hdr - DMA sync the transport header buffer
 * @rdma: controlling transport
 * @ctxt: send_ctxt for the Send WR
 * @len: length of transport header
 *
 */
void svc_rdma_sync_reply_hdr(struct svcxprt_rdma *rdma,
			     struct svc_rdma_send_ctxt *ctxt,
			     unsigned int len)
{
	ctxt->sc_sges[0].length = len;
	ctxt->sc_send_wr.num_sge++;
	ib_dma_sync_single_for_device(rdma->sc_pd->device,
				      ctxt->sc_sges[0].addr, len,
				      DMA_TO_DEVICE);
}

/* If the xdr_buf has more elements than the device can
 * transmit in a single RDMA Send, then the reply will
 * have to be copied into a bounce buffer.
 */
static bool svc_rdma_pull_up_needed(struct svcxprt_rdma *rdma,
				    struct xdr_buf *xdr,
				    __be32 *wr_lst)
{
	int elements;

	/* xdr->head */
	elements = 1;

	/* xdr->pages */
	if (!wr_lst) {
		unsigned int remaining;
		unsigned long pageoff;

		pageoff = xdr->page_base & ~PAGE_MASK;
		remaining = xdr->page_len;
		while (remaining) {
			++elements;
			remaining -= min_t(u32, PAGE_SIZE - pageoff,
					   remaining);
			pageoff = 0;
		}
	}

	/* xdr->tail */
	if (xdr->tail[0].iov_len)
		++elements;

	/* assume 1 SGE is needed for the transport header */
	return elements >= rdma->sc_max_send_sges;
}

/* The device is not capable of sending the reply directly.
 * Assemble the elements of @xdr into the transport header
 * buffer.
 */
static int svc_rdma_pull_up_reply_msg(struct svcxprt_rdma *rdma,
				      struct svc_rdma_send_ctxt *ctxt,
				      struct xdr_buf *xdr, __be32 *wr_lst)
{
	unsigned char *dst, *tailbase;
	unsigned int taillen;

	dst = ctxt->sc_xprt_buf;
	dst += ctxt->sc_sges[0].length;

	memcpy(dst, xdr->head[0].iov_base, xdr->head[0].iov_len);
	dst += xdr->head[0].iov_len;

	tailbase = xdr->tail[0].iov_base;
	taillen = xdr->tail[0].iov_len;
	if (wr_lst) {
		u32 xdrpad;

		xdrpad = xdr_padsize(xdr->page_len);
		if (taillen && xdrpad) {
			tailbase += xdrpad;
			taillen -= xdrpad;
		}
	} else {
		unsigned int len, remaining;
		unsigned long pageoff;
		struct page **ppages;

		ppages = xdr->pages + (xdr->page_base >> PAGE_SHIFT);
		pageoff = xdr->page_base & ~PAGE_MASK;
		remaining = xdr->page_len;
		while (remaining) {
			len = min_t(u32, PAGE_SIZE - pageoff, remaining);

			memcpy(dst, page_address(*ppages) + pageoff, len);
			remaining -= len;
			dst += len;
			pageoff = 0;
			ppages++;
		}
	}

	if (taillen)
		memcpy(dst, tailbase, taillen);

	ctxt->sc_sges[0].length += xdr->len;
	ib_dma_sync_single_for_device(rdma->sc_pd->device,
				      ctxt->sc_sges[0].addr,
				      ctxt->sc_sges[0].length,
				      DMA_TO_DEVICE);

	return 0;
}

/* svc_rdma_map_reply_msg - Map the buffer holding RPC message
 * @rdma: controlling transport
 * @ctxt: send_ctxt for the Send WR
 * @xdr: prepared xdr_buf containing RPC message
 * @wr_lst: pointer to Call header's Write list, or NULL
 *
 * Load the xdr_buf into the ctxt's sge array, and DMA map each
 * element as it is added.
 *
 * Returns zero on success, or a negative errno on failure.
 */
int svc_rdma_map_reply_msg(struct svcxprt_rdma *rdma,
			   struct svc_rdma_send_ctxt *ctxt,
			   struct xdr_buf *xdr, __be32 *wr_lst)
{
	unsigned int len, remaining;
	unsigned long page_off;
	struct page **ppages;
	unsigned char *base;
	u32 xdr_pad;
	int ret;

	if (svc_rdma_pull_up_needed(rdma, xdr, wr_lst))
		return svc_rdma_pull_up_reply_msg(rdma, ctxt, xdr, wr_lst);

	++ctxt->sc_cur_sge_no;
	ret = svc_rdma_dma_map_buf(rdma, ctxt,
				   xdr->head[0].iov_base,
				   xdr->head[0].iov_len);
	if (ret < 0)
		return ret;

	/* If a Write chunk is present, the xdr_buf's page list
	 * is not included inline. However the Upper Layer may
	 * have added XDR padding in the tail buffer, and that
	 * should not be included inline.
	 */
	if (wr_lst) {
		base = xdr->tail[0].iov_base;
		len = xdr->tail[0].iov_len;
		xdr_pad = xdr_padsize(xdr->page_len);

		if (len && xdr_pad) {
			base += xdr_pad;
			len -= xdr_pad;
		}

		goto tail;
	}

	ppages = xdr->pages + (xdr->page_base >> PAGE_SHIFT);
	page_off = xdr->page_base & ~PAGE_MASK;
	remaining = xdr->page_len;
	while (remaining) {
		len = min_t(u32, PAGE_SIZE - page_off, remaining);

		++ctxt->sc_cur_sge_no;
		ret = svc_rdma_dma_map_page(rdma, ctxt, *ppages++,
					    page_off, len);
=======
	trace_svcrdma_dma_map_err(rdma, dma_addr, len);
	return -EIO;
}

/**
 * svc_rdma_iov_dma_map - DMA map an iovec
 * @data: pointer to arguments
 * @iov: kvec to DMA map
 *
 * ib_dma_map_page() is used here because svc_rdma_dma_unmap()
 * handles DMA-unmap and it uses ib_dma_unmap_page() exclusively.
 *
 * Returns:
 *   %0 if DMA mapping was successful
 *   %-EIO if the iovec cannot be DMA mapped
 */
static int svc_rdma_iov_dma_map(void *data, const struct kvec *iov)
{
	if (!iov->iov_len)
		return 0;
	return svc_rdma_page_dma_map(data, virt_to_page(iov->iov_base),
				     offset_in_page(iov->iov_base),
				     iov->iov_len);
}

/**
 * svc_rdma_xb_dma_map - DMA map all segments of an xdr_buf
 * @xdr: xdr_buf containing portion of an RPC message to transmit
 * @data: pointer to arguments
 *
 * Returns:
 *   %0 if DMA mapping was successful
 *   %-EIO if DMA mapping failed
 *
 * On failure, any DMA mappings that have been already done must be
 * unmapped by the caller.
 */
static int svc_rdma_xb_dma_map(const struct xdr_buf *xdr, void *data)
{
	unsigned int len, remaining;
	unsigned long pageoff;
	struct page **ppages;
	int ret;

	ret = svc_rdma_iov_dma_map(data, &xdr->head[0]);
	if (ret < 0)
		return ret;

	ppages = xdr->pages + (xdr->page_base >> PAGE_SHIFT);
	pageoff = offset_in_page(xdr->page_base);
	remaining = xdr->page_len;
	while (remaining) {
		len = min_t(u32, PAGE_SIZE - pageoff, remaining);

		ret = svc_rdma_page_dma_map(data, *ppages++, pageoff, len);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;

		remaining -= len;
<<<<<<< HEAD
		page_off = 0;
	}

	base = xdr->tail[0].iov_base;
	len = xdr->tail[0].iov_len;
tail:
	if (len) {
		++ctxt->sc_cur_sge_no;
		ret = svc_rdma_dma_map_buf(rdma, ctxt, base, len);
		if (ret < 0)
			return ret;
	}

	return 0;
}

/* The svc_rqst and all resources it owns are released as soon as
 * svc_rdma_sendto returns. Transfer pages under I/O to the ctxt
 * so they are released by the Send completion handler.
 */
static void svc_rdma_save_io_pages(struct svc_rqst *rqstp,
				   struct svc_rdma_send_ctxt *ctxt)
{
	int i, pages = rqstp->rq_next_page - rqstp->rq_respages;

	ctxt->sc_page_count += pages;
	for (i = 0; i < pages; i++) {
		ctxt->sc_pages[i] = rqstp->rq_respages[i];
		rqstp->rq_respages[i] = NULL;
	}

	/* Prevent svc_xprt_release from releasing pages in rq_pages */
	rqstp->rq_next_page = rqstp->rq_respages;
=======
		pageoff = 0;
	}

	ret = svc_rdma_iov_dma_map(data, &xdr->tail[0]);
	if (ret < 0)
		return ret;

	return xdr->len;
}

struct svc_rdma_pullup_data {
	u8		*pd_dest;
	unsigned int	pd_length;
	unsigned int	pd_num_sges;
};

/**
 * svc_rdma_xb_count_sges - Count how many SGEs will be needed
 * @xdr: xdr_buf containing portion of an RPC message to transmit
 * @data: pointer to arguments
 *
 * Returns:
 *   Number of SGEs needed to Send the contents of @xdr inline
 */
static int svc_rdma_xb_count_sges(const struct xdr_buf *xdr,
				  void *data)
{
	struct svc_rdma_pullup_data *args = data;
	unsigned int remaining;
	unsigned long offset;

	if (xdr->head[0].iov_len)
		++args->pd_num_sges;

	offset = offset_in_page(xdr->page_base);
	remaining = xdr->page_len;
	while (remaining) {
		++args->pd_num_sges;
		remaining -= min_t(u32, PAGE_SIZE - offset, remaining);
		offset = 0;
	}

	if (xdr->tail[0].iov_len)
		++args->pd_num_sges;

	args->pd_length += xdr->len;
	return 0;
}

/**
 * svc_rdma_pull_up_needed - Determine whether to use pull-up
 * @rdma: controlling transport
 * @sctxt: send_ctxt for the Send WR
 * @rctxt: Write and Reply chunks provided by client
 * @xdr: xdr_buf containing RPC message to transmit
 *
 * Returns:
 *   %true if pull-up must be used
 *   %false otherwise
 */
static bool svc_rdma_pull_up_needed(const struct svcxprt_rdma *rdma,
				    const struct svc_rdma_send_ctxt *sctxt,
				    const struct svc_rdma_recv_ctxt *rctxt,
				    const struct xdr_buf *xdr)
{
	/* Resources needed for the transport header */
	struct svc_rdma_pullup_data args = {
		.pd_length	= sctxt->sc_hdrbuf.len,
		.pd_num_sges	= 1,
	};
	int ret;

	ret = pcl_process_nonpayloads(&rctxt->rc_write_pcl, xdr,
				      svc_rdma_xb_count_sges, &args);
	if (ret < 0)
		return false;

	if (args.pd_length < RPCRDMA_PULLUP_THRESH)
		return true;
	return args.pd_num_sges >= rdma->sc_max_send_sges;
}

/**
 * svc_rdma_xb_linearize - Copy region of xdr_buf to flat buffer
 * @xdr: xdr_buf containing portion of an RPC message to copy
 * @data: pointer to arguments
 *
 * Returns:
 *   Always zero.
 */
static int svc_rdma_xb_linearize(const struct xdr_buf *xdr,
				 void *data)
{
	struct svc_rdma_pullup_data *args = data;
	unsigned int len, remaining;
	unsigned long pageoff;
	struct page **ppages;

	if (xdr->head[0].iov_len) {
		memcpy(args->pd_dest, xdr->head[0].iov_base, xdr->head[0].iov_len);
		args->pd_dest += xdr->head[0].iov_len;
	}

	ppages = xdr->pages + (xdr->page_base >> PAGE_SHIFT);
	pageoff = offset_in_page(xdr->page_base);
	remaining = xdr->page_len;
	while (remaining) {
		len = min_t(u32, PAGE_SIZE - pageoff, remaining);
		memcpy(args->pd_dest, page_address(*ppages) + pageoff, len);
		remaining -= len;
		args->pd_dest += len;
		pageoff = 0;
		ppages++;
	}

	if (xdr->tail[0].iov_len) {
		memcpy(args->pd_dest, xdr->tail[0].iov_base, xdr->tail[0].iov_len);
		args->pd_dest += xdr->tail[0].iov_len;
	}

	args->pd_length += xdr->len;
	return 0;
}

/**
 * svc_rdma_pull_up_reply_msg - Copy Reply into a single buffer
 * @rdma: controlling transport
 * @sctxt: send_ctxt for the Send WR; xprt hdr is already prepared
 * @rctxt: Write and Reply chunks provided by client
 * @xdr: prepared xdr_buf containing RPC message
 *
 * The device is not capable of sending the reply directly.
 * Assemble the elements of @xdr into the transport header buffer.
 *
 * Assumptions:
 *  pull_up_needed has determined that @xdr will fit in the buffer.
 *
 * Returns:
 *   %0 if pull-up was successful
 *   %-EMSGSIZE if a buffer manipulation problem occurred
 */
static int svc_rdma_pull_up_reply_msg(const struct svcxprt_rdma *rdma,
				      struct svc_rdma_send_ctxt *sctxt,
				      const struct svc_rdma_recv_ctxt *rctxt,
				      const struct xdr_buf *xdr)
{
	struct svc_rdma_pullup_data args = {
		.pd_dest	= sctxt->sc_xprt_buf + sctxt->sc_hdrbuf.len,
	};
	int ret;

	ret = pcl_process_nonpayloads(&rctxt->rc_write_pcl, xdr,
				      svc_rdma_xb_linearize, &args);
	if (ret < 0)
		return ret;

	sctxt->sc_sges[0].length = sctxt->sc_hdrbuf.len + args.pd_length;
	trace_svcrdma_send_pullup(sctxt, args.pd_length);
	return 0;
}

/* svc_rdma_map_reply_msg - DMA map the buffer holding RPC message
 * @rdma: controlling transport
 * @sctxt: send_ctxt for the Send WR
 * @rctxt: Write and Reply chunks provided by client
 * @xdr: prepared xdr_buf containing RPC message
 *
 * Returns:
 *   %0 if DMA mapping was successful.
 *   %-EMSGSIZE if a buffer manipulation problem occurred
 *   %-EIO if DMA mapping failed
 *
 * The Send WR's num_sge field is set in all cases.
 */
int svc_rdma_map_reply_msg(struct svcxprt_rdma *rdma,
			   struct svc_rdma_send_ctxt *sctxt,
			   const struct svc_rdma_recv_ctxt *rctxt,
			   const struct xdr_buf *xdr)
{
	struct svc_rdma_map_data args = {
		.md_rdma	= rdma,
		.md_ctxt	= sctxt,
	};

	/* Set up the (persistently-mapped) transport header SGE. */
	sctxt->sc_send_wr.num_sge = 1;
	sctxt->sc_sges[0].length = sctxt->sc_hdrbuf.len;

	/* If there is a Reply chunk, nothing follows the transport
	 * header, and we're done here.
	 */
	if (!pcl_is_empty(&rctxt->rc_reply_pcl))
		return 0;

	/* For pull-up, svc_rdma_send() will sync the transport header.
	 * No additional DMA mapping is necessary.
	 */
	if (svc_rdma_pull_up_needed(rdma, sctxt, rctxt, xdr))
		return svc_rdma_pull_up_reply_msg(rdma, sctxt, rctxt, xdr);

	return pcl_process_nonpayloads(&rctxt->rc_write_pcl, xdr,
				       svc_rdma_xb_dma_map, &args);
>>>>>>> upstream/android-13
}

/* Prepare the portion of the RPC Reply that will be transmitted
 * via RDMA Send. The RPC-over-RDMA transport header is prepared
 * in sc_sges[0], and the RPC xdr_buf is prepared in following sges.
 *
 * Depending on whether a Write list or Reply chunk is present,
 * the server may send all, a portion of, or none of the xdr_buf.
 * In the latter case, only the transport header (sc_sges[0]) is
 * transmitted.
 *
 * RDMA Send is the last step of transmitting an RPC reply. Pages
 * involved in the earlier RDMA Writes are here transferred out
<<<<<<< HEAD
 * of the rqstp and into the ctxt's page array. These pages are
=======
 * of the rqstp and into the sctxt's page array. These pages are
>>>>>>> upstream/android-13
 * DMA unmapped by each Write completion, but the subsequent Send
 * completion finally releases these pages.
 *
 * Assumptions:
 * - The Reply's transport header will never be larger than a page.
 */
static int svc_rdma_send_reply_msg(struct svcxprt_rdma *rdma,
<<<<<<< HEAD
				   struct svc_rdma_send_ctxt *ctxt,
				   __be32 *rdma_argp,
				   struct svc_rqst *rqstp,
				   __be32 *wr_lst, __be32 *rp_ch)
{
	int ret;

	if (!rp_ch) {
		ret = svc_rdma_map_reply_msg(rdma, ctxt,
					     &rqstp->rq_res, wr_lst);
		if (ret < 0)
			return ret;
	}

	svc_rdma_save_io_pages(rqstp, ctxt);

	ctxt->sc_send_wr.opcode = IB_WR_SEND;
	if (rdma->sc_snd_w_inv) {
		ctxt->sc_send_wr.ex.invalidate_rkey =
			svc_rdma_get_inv_rkey(rdma_argp, wr_lst, rp_ch);
		if (ctxt->sc_send_wr.ex.invalidate_rkey)
			ctxt->sc_send_wr.opcode = IB_WR_SEND_WITH_INV;
	}
	dprintk("svcrdma: posting Send WR with %u sge(s)\n",
		ctxt->sc_send_wr.num_sge);
	return svc_rdma_send(rdma, &ctxt->sc_send_wr);
}

/* Given the client-provided Write and Reply chunks, the server was not
 * able to form a complete reply. Return an RDMA_ERROR message so the
 * client can retire this RPC transaction. As above, the Send completion
 * routine releases payload pages that were part of a previous RDMA Write.
 *
 * Remote Invalidation is skipped for simplicity.
 */
static int svc_rdma_send_error_msg(struct svcxprt_rdma *rdma,
				   struct svc_rdma_send_ctxt *ctxt,
				   struct svc_rqst *rqstp)
{
	__be32 *p;
	int ret;

	p = ctxt->sc_xprt_buf;
	trace_svcrdma_err_chunk(*p);
	p += 3;
	*p++ = rdma_error;
	*p   = err_chunk;
	svc_rdma_sync_reply_hdr(rdma, ctxt, RPCRDMA_HDRLEN_ERR);

	svc_rdma_save_io_pages(rqstp, ctxt);

	ctxt->sc_send_wr.opcode = IB_WR_SEND;
	ret = svc_rdma_send(rdma, &ctxt->sc_send_wr);
	if (ret) {
		svc_rdma_send_ctxt_put(rdma, ctxt);
		return ret;
	}

	return 0;
}

void svc_rdma_prep_reply_hdr(struct svc_rqst *rqstp)
{
=======
				   struct svc_rdma_send_ctxt *sctxt,
				   const struct svc_rdma_recv_ctxt *rctxt,
				   struct svc_rqst *rqstp)
{
	int ret;

	ret = svc_rdma_map_reply_msg(rdma, sctxt, rctxt, &rqstp->rq_res);
	if (ret < 0)
		return ret;

	if (rctxt->rc_inv_rkey) {
		sctxt->sc_send_wr.opcode = IB_WR_SEND_WITH_INV;
		sctxt->sc_send_wr.ex.invalidate_rkey = rctxt->rc_inv_rkey;
	} else {
		sctxt->sc_send_wr.opcode = IB_WR_SEND;
	}

	ret = svc_rdma_send(rdma, sctxt);
	if (ret < 0)
		return ret;

	ret = wait_for_completion_killable(&sctxt->sc_done);
	svc_rdma_send_ctxt_put(rdma, sctxt);
	return ret;
}

/**
 * svc_rdma_send_error_msg - Send an RPC/RDMA v1 error response
 * @rdma: controlling transport context
 * @sctxt: Send context for the response
 * @rctxt: Receive context for incoming bad message
 * @status: negative errno indicating error that occurred
 *
 * Given the client-provided Read, Write, and Reply chunks, the
 * server was not able to parse the Call or form a complete Reply.
 * Return an RDMA_ERROR message so the client can retire the RPC
 * transaction.
 *
 * The caller does not have to release @sctxt. It is released by
 * Send completion, or by this function on error.
 */
void svc_rdma_send_error_msg(struct svcxprt_rdma *rdma,
			     struct svc_rdma_send_ctxt *sctxt,
			     struct svc_rdma_recv_ctxt *rctxt,
			     int status)
{
	__be32 *rdma_argp = rctxt->rc_recv_buf;
	__be32 *p;

	rpcrdma_set_xdrlen(&sctxt->sc_hdrbuf, 0);
	xdr_init_encode(&sctxt->sc_stream, &sctxt->sc_hdrbuf,
			sctxt->sc_xprt_buf, NULL);

	p = xdr_reserve_space(&sctxt->sc_stream,
			      rpcrdma_fixed_maxsz * sizeof(*p));
	if (!p)
		goto put_ctxt;

	*p++ = *rdma_argp;
	*p++ = *(rdma_argp + 1);
	*p++ = rdma->sc_fc_credits;
	*p = rdma_error;

	switch (status) {
	case -EPROTONOSUPPORT:
		p = xdr_reserve_space(&sctxt->sc_stream, 3 * sizeof(*p));
		if (!p)
			goto put_ctxt;

		*p++ = err_vers;
		*p++ = rpcrdma_version;
		*p = rpcrdma_version;
		trace_svcrdma_err_vers(*rdma_argp);
		break;
	default:
		p = xdr_reserve_space(&sctxt->sc_stream, sizeof(*p));
		if (!p)
			goto put_ctxt;

		*p = err_chunk;
		trace_svcrdma_err_chunk(*rdma_argp);
	}

	/* Remote Invalidation is skipped for simplicity. */
	sctxt->sc_send_wr.num_sge = 1;
	sctxt->sc_send_wr.opcode = IB_WR_SEND;
	sctxt->sc_sges[0].length = sctxt->sc_hdrbuf.len;
	if (svc_rdma_send(rdma, sctxt))
		goto put_ctxt;

	wait_for_completion_killable(&sctxt->sc_done);

put_ctxt:
	svc_rdma_send_ctxt_put(rdma, sctxt);
>>>>>>> upstream/android-13
}

/**
 * svc_rdma_sendto - Transmit an RPC reply
 * @rqstp: processed RPC request, reply XDR already in ::rq_res
 *
 * Any resources still associated with @rqstp are released upon return.
 * If no reply message was possible, the connection is closed.
 *
 * Returns:
 *	%0 if an RPC reply has been successfully posted,
 *	%-ENOMEM if a resource shortage occurred (connection is lost),
 *	%-ENOTCONN if posting failed (connection is lost).
 */
int svc_rdma_sendto(struct svc_rqst *rqstp)
{
	struct svc_xprt *xprt = rqstp->rq_xprt;
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);
	struct svc_rdma_recv_ctxt *rctxt = rqstp->rq_xprt_ctxt;
<<<<<<< HEAD
	__be32 *p, *rdma_argp, *rdma_resp, *wr_lst, *rp_ch;
	struct xdr_buf *xdr = &rqstp->rq_res;
	struct svc_rdma_send_ctxt *sctxt;
	int ret;

	rdma_argp = rctxt->rc_recv_buf;
	svc_rdma_get_write_arrays(rdma_argp, &wr_lst, &rp_ch);

	/* Create the RDMA response header. xprt->xpt_mutex,
	 * acquired in svc_send(), serializes RPC replies. The
	 * code path below that inserts the credit grant value
	 * into each transport header runs only inside this
	 * critical section.
	 */
	ret = -ENOMEM;
	sctxt = svc_rdma_send_ctxt_get(rdma);
	if (!sctxt)
		goto err0;
	rdma_resp = sctxt->sc_xprt_buf;

	p = rdma_resp;
	*p++ = *rdma_argp;
	*p++ = *(rdma_argp + 1);
	*p++ = rdma->sc_fc_credits;
	*p++ = rp_ch ? rdma_nomsg : rdma_msg;

	/* Start with empty chunks */
	*p++ = xdr_zero;
	*p++ = xdr_zero;
	*p   = xdr_zero;

	if (wr_lst) {
		/* XXX: Presume the client sent only one Write chunk */
		ret = svc_rdma_send_write_chunk(rdma, wr_lst, xdr);
		if (ret < 0)
			goto err2;
		svc_rdma_xdr_encode_write_list(rdma_resp, wr_lst, ret);
	}
	if (rp_ch) {
		ret = svc_rdma_send_reply_chunk(rdma, rp_ch, wr_lst, xdr);
		if (ret < 0)
			goto err2;
		svc_rdma_xdr_encode_reply_chunk(rdma_resp, rp_ch, ret);
	}

	svc_rdma_sync_reply_hdr(rdma, sctxt, svc_rdma_reply_hdr_len(rdma_resp));
	ret = svc_rdma_send_reply_msg(rdma, sctxt, rdma_argp, rqstp,
				      wr_lst, rp_ch);
	if (ret < 0)
		goto err1;
	return 0;

 err2:
	if (ret != -E2BIG && ret != -EINVAL)
		goto err1;

	ret = svc_rdma_send_error_msg(rdma, sctxt, rqstp);
	if (ret < 0)
		goto err1;
	return 0;

 err1:
	svc_rdma_send_ctxt_put(rdma, sctxt);
 err0:
	trace_svcrdma_send_failed(rqstp, ret);
	set_bit(XPT_CLOSE, &xprt->xpt_flags);
	return -ENOTCONN;
}
=======
	__be32 *rdma_argp = rctxt->rc_recv_buf;
	struct svc_rdma_send_ctxt *sctxt;
	unsigned int rc_size;
	__be32 *p;
	int ret;

	ret = -ENOTCONN;
	if (svc_xprt_is_dead(xprt))
		goto drop_connection;

	ret = -ENOMEM;
	sctxt = svc_rdma_send_ctxt_get(rdma);
	if (!sctxt)
		goto drop_connection;

	ret = -EMSGSIZE;
	p = xdr_reserve_space(&sctxt->sc_stream,
			      rpcrdma_fixed_maxsz * sizeof(*p));
	if (!p)
		goto put_ctxt;

	ret = svc_rdma_send_reply_chunk(rdma, rctxt, &rqstp->rq_res);
	if (ret < 0)
		goto reply_chunk;
	rc_size = ret;

	*p++ = *rdma_argp;
	*p++ = *(rdma_argp + 1);
	*p++ = rdma->sc_fc_credits;
	*p = pcl_is_empty(&rctxt->rc_reply_pcl) ? rdma_msg : rdma_nomsg;

	ret = svc_rdma_encode_read_list(sctxt);
	if (ret < 0)
		goto put_ctxt;
	ret = svc_rdma_encode_write_list(rctxt, sctxt);
	if (ret < 0)
		goto put_ctxt;
	ret = svc_rdma_encode_reply_chunk(rctxt, sctxt, rc_size);
	if (ret < 0)
		goto put_ctxt;

	ret = svc_rdma_send_reply_msg(rdma, sctxt, rctxt, rqstp);
	if (ret < 0)
		goto put_ctxt;

	/* Prevent svc_xprt_release() from releasing the page backing
	 * rq_res.head[0].iov_base. It's no longer being accessed by
	 * the I/O device. */
	rqstp->rq_respages++;
	return 0;

reply_chunk:
	if (ret != -E2BIG && ret != -EINVAL)
		goto put_ctxt;

	svc_rdma_send_error_msg(rdma, sctxt, rctxt, ret);
	return 0;

put_ctxt:
	svc_rdma_send_ctxt_put(rdma, sctxt);
drop_connection:
	trace_svcrdma_send_err(rqstp, ret);
	svc_xprt_deferred_close(&rdma->sc_xprt);
	return -ENOTCONN;
}

/**
 * svc_rdma_result_payload - special processing for a result payload
 * @rqstp: svc_rqst to operate on
 * @offset: payload's byte offset in @xdr
 * @length: size of payload, in bytes
 *
 * Return values:
 *   %0 if successful or nothing needed to be done
 *   %-EMSGSIZE on XDR buffer overflow
 *   %-E2BIG if the payload was larger than the Write chunk
 *   %-EINVAL if client provided too many segments
 *   %-ENOMEM if rdma_rw context pool was exhausted
 *   %-ENOTCONN if posting failed (connection is lost)
 *   %-EIO if rdma_rw initialization failed (DMA mapping, etc)
 */
int svc_rdma_result_payload(struct svc_rqst *rqstp, unsigned int offset,
			    unsigned int length)
{
	struct svc_rdma_recv_ctxt *rctxt = rqstp->rq_xprt_ctxt;
	struct svc_rdma_chunk *chunk;
	struct svcxprt_rdma *rdma;
	struct xdr_buf subbuf;
	int ret;

	chunk = rctxt->rc_cur_result_payload;
	if (!length || !chunk)
		return 0;
	rctxt->rc_cur_result_payload =
		pcl_next_chunk(&rctxt->rc_write_pcl, chunk);
	if (length > chunk->ch_length)
		return -E2BIG;

	chunk->ch_position = offset;
	chunk->ch_payload_length = length;

	if (xdr_buf_subsegment(&rqstp->rq_res, &subbuf, offset, length))
		return -EMSGSIZE;

	rdma = container_of(rqstp->rq_xprt, struct svcxprt_rdma, sc_xprt);
	ret = svc_rdma_send_write_chunk(rdma, chunk, &subbuf);
	if (ret < 0)
		return ret;
	return 0;
}
>>>>>>> upstream/android-13
