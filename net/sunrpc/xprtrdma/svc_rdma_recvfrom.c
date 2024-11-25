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
 * The main entry point is svc_rdma_recvfrom. This is called from
 * svc_recv when the transport indicates there is incoming data to
 * be read. "Data Ready" is signaled when an RDMA Receive completes,
 * or when a set of RDMA Reads complete.
 *
 * An svc_rqst is passed in. This structure contains an array of
 * free pages (rq_pages) that will contain the incoming RPC message.
 *
 * Short messages are moved directly into svc_rqst::rq_arg, and
 * the RPC Call is ready to be processed by the Upper Layer.
 * svc_rdma_recvfrom returns the length of the RPC Call message,
 * completing the reception of the RPC Call.
 *
 * However, when an incoming message has Read chunks,
 * svc_rdma_recvfrom must post RDMA Reads to pull the RPC Call's
 * data payload from the client. svc_rdma_recvfrom sets up the
 * RDMA Reads using pages in svc_rqst::rq_pages, which are
 * transferred to an svc_rdma_recv_ctxt for the duration of the
 * I/O. svc_rdma_recvfrom then returns zero, since the RPC message
 * is still not yet ready.
 *
 * When the Read chunk payloads have become available on the
 * server, "Data Ready" is raised again, and svc_recv calls
 * svc_rdma_recvfrom again. This second call may use a different
 * svc_rqst than the first one, thus any information that needs
 * to be preserved across these two calls is kept in an
 * svc_rdma_recv_ctxt.
 *
 * The second call to svc_rdma_recvfrom performs final assembly
 * of the RPC Call message, using the RDMA Read sink pages kept in
 * the svc_rdma_recv_ctxt. The xdr_buf is copied from the
 * svc_rdma_recv_ctxt to the second svc_rqst. The second call returns
 * the length of the completed RPC Call message.
 *
 * Page Management
 *
 * Pages under I/O must be transferred from the first svc_rqst to an
 * svc_rdma_recv_ctxt before the first svc_rdma_recvfrom call returns.
 *
 * The first svc_rqst supplies pages for RDMA Reads. These are moved
 * from rqstp::rq_pages into ctxt::pages. The consumed elements of
 * the rq_pages array are set to NULL and refilled with the first
 * svc_rdma_recvfrom call returns.
 *
 * During the second svc_rdma_recvfrom call, RDMA Read sink pages
<<<<<<< HEAD
 * are transferred from the svc_rdma_recv_ctxt to the second svc_rqst
 * (see rdma_read_complete() below).
 */

=======
 * are transferred from the svc_rdma_recv_ctxt to the second svc_rqst.
 */

#include <linux/slab.h>
>>>>>>> upstream/android-13
#include <linux/spinlock.h>
#include <asm/unaligned.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>

#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/debug.h>
#include <linux/sunrpc/rpc_rdma.h>
#include <linux/sunrpc/svc_rdma.h>

#include "xprt_rdma.h"
#include <trace/events/rpcrdma.h>

<<<<<<< HEAD
#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

=======
>>>>>>> upstream/android-13
static void svc_rdma_wc_receive(struct ib_cq *cq, struct ib_wc *wc);

static inline struct svc_rdma_recv_ctxt *
svc_rdma_next_recv_ctxt(struct list_head *list)
{
	return list_first_entry_or_null(list, struct svc_rdma_recv_ctxt,
					rc_list);
}

<<<<<<< HEAD
=======
static void svc_rdma_recv_cid_init(struct svcxprt_rdma *rdma,
				   struct rpc_rdma_cid *cid)
{
	cid->ci_queue_id = rdma->sc_rq_cq->res.id;
	cid->ci_completion_id = atomic_inc_return(&rdma->sc_completion_ids);
}

>>>>>>> upstream/android-13
static struct svc_rdma_recv_ctxt *
svc_rdma_recv_ctxt_alloc(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;
	dma_addr_t addr;
	void *buffer;

	ctxt = kmalloc(sizeof(*ctxt), GFP_KERNEL);
	if (!ctxt)
		goto fail0;
	buffer = kmalloc(rdma->sc_max_req_size, GFP_KERNEL);
	if (!buffer)
		goto fail1;
	addr = ib_dma_map_single(rdma->sc_pd->device, buffer,
				 rdma->sc_max_req_size, DMA_FROM_DEVICE);
	if (ib_dma_mapping_error(rdma->sc_pd->device, addr))
		goto fail2;

<<<<<<< HEAD
=======
	svc_rdma_recv_cid_init(rdma, &ctxt->rc_cid);
	pcl_init(&ctxt->rc_call_pcl);
	pcl_init(&ctxt->rc_read_pcl);
	pcl_init(&ctxt->rc_write_pcl);
	pcl_init(&ctxt->rc_reply_pcl);

>>>>>>> upstream/android-13
	ctxt->rc_recv_wr.next = NULL;
	ctxt->rc_recv_wr.wr_cqe = &ctxt->rc_cqe;
	ctxt->rc_recv_wr.sg_list = &ctxt->rc_recv_sge;
	ctxt->rc_recv_wr.num_sge = 1;
	ctxt->rc_cqe.done = svc_rdma_wc_receive;
	ctxt->rc_recv_sge.addr = addr;
	ctxt->rc_recv_sge.length = rdma->sc_max_req_size;
	ctxt->rc_recv_sge.lkey = rdma->sc_pd->local_dma_lkey;
	ctxt->rc_recv_buf = buffer;
	ctxt->rc_temp = false;
	return ctxt;

fail2:
	kfree(buffer);
fail1:
	kfree(ctxt);
fail0:
	return NULL;
}

static void svc_rdma_recv_ctxt_destroy(struct svcxprt_rdma *rdma,
				       struct svc_rdma_recv_ctxt *ctxt)
{
	ib_dma_unmap_single(rdma->sc_pd->device, ctxt->rc_recv_sge.addr,
			    ctxt->rc_recv_sge.length, DMA_FROM_DEVICE);
	kfree(ctxt->rc_recv_buf);
	kfree(ctxt);
}

/**
 * svc_rdma_recv_ctxts_destroy - Release all recv_ctxt's for an xprt
 * @rdma: svcxprt_rdma being torn down
 *
 */
void svc_rdma_recv_ctxts_destroy(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;
<<<<<<< HEAD

	while ((ctxt = svc_rdma_next_recv_ctxt(&rdma->sc_recv_ctxts))) {
		list_del(&ctxt->rc_list);
=======
	struct llist_node *node;

	while ((node = llist_del_first(&rdma->sc_recv_ctxts))) {
		ctxt = llist_entry(node, struct svc_rdma_recv_ctxt, rc_node);
>>>>>>> upstream/android-13
		svc_rdma_recv_ctxt_destroy(rdma, ctxt);
	}
}

<<<<<<< HEAD
static struct svc_rdma_recv_ctxt *
svc_rdma_recv_ctxt_get(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;

	spin_lock(&rdma->sc_recv_lock);
	ctxt = svc_rdma_next_recv_ctxt(&rdma->sc_recv_ctxts);
	if (!ctxt)
		goto out_empty;
	list_del(&ctxt->rc_list);
	spin_unlock(&rdma->sc_recv_lock);
=======
/**
 * svc_rdma_recv_ctxt_get - Allocate a recv_ctxt
 * @rdma: controlling svcxprt_rdma
 *
 * Returns a recv_ctxt or (rarely) NULL if none are available.
 */
struct svc_rdma_recv_ctxt *svc_rdma_recv_ctxt_get(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;
	struct llist_node *node;

	node = llist_del_first(&rdma->sc_recv_ctxts);
	if (!node)
		goto out_empty;
	ctxt = llist_entry(node, struct svc_rdma_recv_ctxt, rc_node);
>>>>>>> upstream/android-13

out:
	ctxt->rc_page_count = 0;
	return ctxt;

out_empty:
<<<<<<< HEAD
	spin_unlock(&rdma->sc_recv_lock);

=======
>>>>>>> upstream/android-13
	ctxt = svc_rdma_recv_ctxt_alloc(rdma);
	if (!ctxt)
		return NULL;
	goto out;
}

/**
 * svc_rdma_recv_ctxt_put - Return recv_ctxt to free list
 * @rdma: controlling svcxprt_rdma
 * @ctxt: object to return to the free list
 *
 */
void svc_rdma_recv_ctxt_put(struct svcxprt_rdma *rdma,
			    struct svc_rdma_recv_ctxt *ctxt)
{
<<<<<<< HEAD
	unsigned int i;

	for (i = 0; i < ctxt->rc_page_count; i++)
		put_page(ctxt->rc_pages[i]);

	if (!ctxt->rc_temp) {
		spin_lock(&rdma->sc_recv_lock);
		list_add(&ctxt->rc_list, &rdma->sc_recv_ctxts);
		spin_unlock(&rdma->sc_recv_lock);
	} else
=======
	pcl_free(&ctxt->rc_call_pcl);
	pcl_free(&ctxt->rc_read_pcl);
	pcl_free(&ctxt->rc_write_pcl);
	pcl_free(&ctxt->rc_reply_pcl);

	if (!ctxt->rc_temp)
		llist_add(&ctxt->rc_node, &rdma->sc_recv_ctxts);
	else
>>>>>>> upstream/android-13
		svc_rdma_recv_ctxt_destroy(rdma, ctxt);
}

/**
 * svc_rdma_release_rqst - Release transport-specific per-rqst resources
 * @rqstp: svc_rqst being released
 *
 * Ensure that the recv_ctxt is released whether or not a Reply
 * was sent. For example, the client could close the connection,
 * or svc_process could drop an RPC, before the Reply is sent.
 */
void svc_rdma_release_rqst(struct svc_rqst *rqstp)
{
	struct svc_rdma_recv_ctxt *ctxt = rqstp->rq_xprt_ctxt;
	struct svc_xprt *xprt = rqstp->rq_xprt;
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);

	rqstp->rq_xprt_ctxt = NULL;
	if (ctxt)
		svc_rdma_recv_ctxt_put(rdma, ctxt);
}

<<<<<<< HEAD
static int __svc_rdma_post_recv(struct svcxprt_rdma *rdma,
				struct svc_rdma_recv_ctxt *ctxt)
{
	int ret;

	svc_xprt_get(&rdma->sc_xprt);
	ret = ib_post_recv(rdma->sc_qp, &ctxt->rc_recv_wr, NULL);
	trace_svcrdma_post_recv(&ctxt->rc_recv_wr, ret);
	if (ret)
		goto err_post;
	return 0;

err_post:
	svc_rdma_recv_ctxt_put(rdma, ctxt);
	svc_xprt_put(&rdma->sc_xprt);
	return ret;
}

static int svc_rdma_post_recv(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;

	if (test_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags))
		return 0;
	ctxt = svc_rdma_recv_ctxt_get(rdma);
	if (!ctxt)
		return -ENOMEM;
	return __svc_rdma_post_recv(rdma, ctxt);
=======
static bool svc_rdma_refresh_recvs(struct svcxprt_rdma *rdma,
				   unsigned int wanted, bool temp)
{
	const struct ib_recv_wr *bad_wr = NULL;
	struct svc_rdma_recv_ctxt *ctxt;
	struct ib_recv_wr *recv_chain;
	int ret;

	if (test_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags))
		return false;

	recv_chain = NULL;
	while (wanted--) {
		ctxt = svc_rdma_recv_ctxt_get(rdma);
		if (!ctxt)
			break;

		trace_svcrdma_post_recv(ctxt);
		ctxt->rc_temp = temp;
		ctxt->rc_recv_wr.next = recv_chain;
		recv_chain = &ctxt->rc_recv_wr;
		rdma->sc_pending_recvs++;
	}
	if (!recv_chain)
		return false;

	ret = ib_post_recv(rdma->sc_qp, recv_chain, &bad_wr);
	if (ret)
		goto err_free;
	return true;

err_free:
	trace_svcrdma_rq_post_err(rdma, ret);
	while (bad_wr) {
		ctxt = container_of(bad_wr, struct svc_rdma_recv_ctxt,
				    rc_recv_wr);
		bad_wr = bad_wr->next;
		svc_rdma_recv_ctxt_put(rdma, ctxt);
	}
	/* Since we're destroying the xprt, no need to reset
	 * sc_pending_recvs. */
	return false;
>>>>>>> upstream/android-13
}

/**
 * svc_rdma_post_recvs - Post initial set of Recv WRs
 * @rdma: fresh svcxprt_rdma
 *
 * Returns true if successful, otherwise false.
 */
bool svc_rdma_post_recvs(struct svcxprt_rdma *rdma)
{
<<<<<<< HEAD
	struct svc_rdma_recv_ctxt *ctxt;
	unsigned int i;
	int ret;

	for (i = 0; i < rdma->sc_max_requests; i++) {
		ctxt = svc_rdma_recv_ctxt_get(rdma);
		if (!ctxt)
			return false;
		ctxt->rc_temp = true;
		ret = __svc_rdma_post_recv(rdma, ctxt);
		if (ret) {
			pr_err("svcrdma: failure posting recv buffers: %d\n",
			       ret);
			return false;
		}
	}
	return true;
=======
	return svc_rdma_refresh_recvs(rdma, rdma->sc_max_requests, true);
>>>>>>> upstream/android-13
}

/**
 * svc_rdma_wc_receive - Invoked by RDMA provider for each polled Receive WC
 * @cq: Completion Queue context
 * @wc: Work Completion object
 *
<<<<<<< HEAD
 * NB: The svc_xprt/svcxprt_rdma is pinned whenever it's possible that
 * the Receive completion handler could be running.
=======
>>>>>>> upstream/android-13
 */
static void svc_rdma_wc_receive(struct ib_cq *cq, struct ib_wc *wc)
{
	struct svcxprt_rdma *rdma = cq->cq_context;
	struct ib_cqe *cqe = wc->wr_cqe;
	struct svc_rdma_recv_ctxt *ctxt;

<<<<<<< HEAD
	trace_svcrdma_wc_receive(wc);
=======
	rdma->sc_pending_recvs--;
>>>>>>> upstream/android-13

	/* WARNING: Only wc->wr_cqe and wc->status are reliable */
	ctxt = container_of(cqe, struct svc_rdma_recv_ctxt, rc_cqe);

<<<<<<< HEAD
	if (wc->status != IB_WC_SUCCESS)
		goto flushed;

	if (svc_rdma_post_recv(rdma))
		goto post_err;

	/* All wc fields are now known to be valid */
	ctxt->rc_byte_len = wc->byte_len;
	ib_dma_sync_single_for_cpu(rdma->sc_pd->device,
				   ctxt->rc_recv_sge.addr,
				   wc->byte_len, DMA_FROM_DEVICE);

	spin_lock(&rdma->sc_rq_dto_lock);
	list_add_tail(&ctxt->rc_list, &rdma->sc_rq_dto_q);
	spin_unlock(&rdma->sc_rq_dto_lock);
	set_bit(XPT_DATA, &rdma->sc_xprt.xpt_flags);
	if (!test_bit(RDMAXPRT_CONN_PENDING, &rdma->sc_flags))
		svc_xprt_enqueue(&rdma->sc_xprt);
	goto out;

flushed:
	if (wc->status != IB_WC_WR_FLUSH_ERR)
		pr_err("svcrdma: Recv: %s (%u/0x%x)\n",
		       ib_wc_status_msg(wc->status),
		       wc->status, wc->vendor_err);
post_err:
	svc_rdma_recv_ctxt_put(rdma, ctxt);
	set_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags);
	svc_xprt_enqueue(&rdma->sc_xprt);
out:
	svc_xprt_put(&rdma->sc_xprt);
=======
	trace_svcrdma_wc_receive(wc, &ctxt->rc_cid);
	if (wc->status != IB_WC_SUCCESS)
		goto flushed;

	/* If receive posting fails, the connection is about to be
	 * lost anyway. The server will not be able to send a reply
	 * for this RPC, and the client will retransmit this RPC
	 * anyway when it reconnects.
	 *
	 * Therefore we drop the Receive, even if status was SUCCESS
	 * to reduce the likelihood of replayed requests once the
	 * client reconnects.
	 */
	if (rdma->sc_pending_recvs < rdma->sc_max_requests)
		if (!svc_rdma_refresh_recvs(rdma, rdma->sc_recv_batch, false))
			goto flushed;

	/* All wc fields are now known to be valid */
	ctxt->rc_byte_len = wc->byte_len;

	spin_lock(&rdma->sc_rq_dto_lock);
	list_add_tail(&ctxt->rc_list, &rdma->sc_rq_dto_q);
	/* Note the unlock pairs with the smp_rmb in svc_xprt_ready: */
	set_bit(XPT_DATA, &rdma->sc_xprt.xpt_flags);
	spin_unlock(&rdma->sc_rq_dto_lock);
	if (!test_bit(RDMAXPRT_CONN_PENDING, &rdma->sc_flags))
		svc_xprt_enqueue(&rdma->sc_xprt);
	return;

flushed:
	svc_rdma_recv_ctxt_put(rdma, ctxt);
	svc_xprt_deferred_close(&rdma->sc_xprt);
>>>>>>> upstream/android-13
}

/**
 * svc_rdma_flush_recv_queues - Drain pending Receive work
 * @rdma: svcxprt_rdma being shut down
 *
 */
void svc_rdma_flush_recv_queues(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;

<<<<<<< HEAD
	while ((ctxt = svc_rdma_next_recv_ctxt(&rdma->sc_read_complete_q))) {
		list_del(&ctxt->rc_list);
		svc_rdma_recv_ctxt_put(rdma, ctxt);
	}
=======
>>>>>>> upstream/android-13
	while ((ctxt = svc_rdma_next_recv_ctxt(&rdma->sc_rq_dto_q))) {
		list_del(&ctxt->rc_list);
		svc_rdma_recv_ctxt_put(rdma, ctxt);
	}
}

static void svc_rdma_build_arg_xdr(struct svc_rqst *rqstp,
				   struct svc_rdma_recv_ctxt *ctxt)
{
	struct xdr_buf *arg = &rqstp->rq_arg;

	arg->head[0].iov_base = ctxt->rc_recv_buf;
	arg->head[0].iov_len = ctxt->rc_byte_len;
	arg->tail[0].iov_base = NULL;
	arg->tail[0].iov_len = 0;
	arg->page_len = 0;
	arg->page_base = 0;
	arg->buflen = ctxt->rc_byte_len;
	arg->len = ctxt->rc_byte_len;
}

<<<<<<< HEAD
/* This accommodates the largest possible Write chunk,
 * in one segment.
 */
#define MAX_BYTES_WRITE_SEG	((u32)(RPCSVC_MAXPAGES << PAGE_SHIFT))

/* This accommodates the largest possible Position-Zero
 * Read chunk or Reply chunk, in one segment.
 */
#define MAX_BYTES_SPECIAL_SEG	((u32)((RPCSVC_MAXPAGES + 2) << PAGE_SHIFT))

/* Sanity check the Read list.
 *
 * Implementation limits:
 * - This implementation supports only one Read chunk.
 *
 * Sanity checks:
 * - Read list does not overflow buffer.
 * - Segment size limited by largest NFS data payload.
 *
 * The segment count is limited to how many segments can
 * fit in the transport header without overflowing the
 * buffer. That's about 40 Read segments for a 1KB inline
 * threshold.
 *
 * Returns pointer to the following Write list.
 */
static __be32 *xdr_check_read_list(__be32 *p, const __be32 *end)
{
	u32 position;
	bool first;

	first = true;
	while (*p++ != xdr_zero) {
		if (first) {
			position = be32_to_cpup(p++);
			first = false;
		} else if (be32_to_cpup(p++) != position) {
			return NULL;
		}
		p++;	/* handle */
		if (be32_to_cpup(p++) > MAX_BYTES_SPECIAL_SEG)
			return NULL;
		p += 2;	/* offset */

		if (p > end)
			return NULL;
	}
	return p;
}

/* The segment count is limited to how many segments can
 * fit in the transport header without overflowing the
 * buffer. That's about 60 Write segments for a 1KB inline
 * threshold.
 */
static __be32 *xdr_check_write_chunk(__be32 *p, const __be32 *end,
				     u32 maxlen)
{
	u32 i, segcount;

	segcount = be32_to_cpup(p++);
	for (i = 0; i < segcount; i++) {
		p++;	/* handle */
		if (be32_to_cpup(p++) > maxlen)
			return NULL;
		p += 2;	/* offset */

		if (p > end)
			return NULL;
	}

	return p;
=======
/**
 * xdr_count_read_segments - Count number of Read segments in Read list
 * @rctxt: Ingress receive context
 * @p: Start of an un-decoded Read list
 *
 * Before allocating anything, ensure the ingress Read list is safe
 * to use.
 *
 * The segment count is limited to how many segments can fit in the
 * transport header without overflowing the buffer. That's about 40
 * Read segments for a 1KB inline threshold.
 *
 * Return values:
 *   %true: Read list is valid. @rctxt's xdr_stream is updated to point
 *	    to the first byte past the Read list. rc_read_pcl and
 *	    rc_call_pcl cl_count fields are set to the number of
 *	    Read segments in the list.
 *  %false: Read list is corrupt. @rctxt's xdr_stream is left in an
 *	    unknown state.
 */
static bool xdr_count_read_segments(struct svc_rdma_recv_ctxt *rctxt, __be32 *p)
{
	rctxt->rc_call_pcl.cl_count = 0;
	rctxt->rc_read_pcl.cl_count = 0;
	while (xdr_item_is_present(p)) {
		u32 position, handle, length;
		u64 offset;

		p = xdr_inline_decode(&rctxt->rc_stream,
				      rpcrdma_readseg_maxsz * sizeof(*p));
		if (!p)
			return false;

		xdr_decode_read_segment(p, &position, &handle,
					    &length, &offset);
		if (position) {
			if (position & 3)
				return false;
			++rctxt->rc_read_pcl.cl_count;
		} else {
			++rctxt->rc_call_pcl.cl_count;
		}

		p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
		if (!p)
			return false;
	}
	return true;
}

/* Sanity check the Read list.
 *
 * Sanity checks:
 * - Read list does not overflow Receive buffer.
 * - Chunk size limited by largest NFS data payload.
 *
 * Return values:
 *   %true: Read list is valid. @rctxt's xdr_stream is updated
 *	    to point to the first byte past the Read list.
 *  %false: Read list is corrupt. @rctxt's xdr_stream is left
 *	    in an unknown state.
 */
static bool xdr_check_read_list(struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p;

	p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
	if (!p)
		return false;
	if (!xdr_count_read_segments(rctxt, p))
		return false;
	if (!pcl_alloc_call(rctxt, p))
		return false;
	return pcl_alloc_read(rctxt, p);
}

static bool xdr_check_write_chunk(struct svc_rdma_recv_ctxt *rctxt)
{
	u32 segcount;
	__be32 *p;

	if (xdr_stream_decode_u32(&rctxt->rc_stream, &segcount))
		return false;

	/* A bogus segcount causes this buffer overflow check to fail. */
	p = xdr_inline_decode(&rctxt->rc_stream,
			      segcount * rpcrdma_segment_maxsz * sizeof(*p));
	return p != NULL;
}

/**
 * xdr_count_write_chunks - Count number of Write chunks in Write list
 * @rctxt: Received header and decoding state
 * @p: start of an un-decoded Write list
 *
 * Before allocating anything, ensure the ingress Write list is
 * safe to use.
 *
 * Return values:
 *       %true: Write list is valid. @rctxt's xdr_stream is updated
 *		to point to the first byte past the Write list, and
 *		the number of Write chunks is in rc_write_pcl.cl_count.
 *      %false: Write list is corrupt. @rctxt's xdr_stream is left
 *		in an indeterminate state.
 */
static bool xdr_count_write_chunks(struct svc_rdma_recv_ctxt *rctxt, __be32 *p)
{
	rctxt->rc_write_pcl.cl_count = 0;
	while (xdr_item_is_present(p)) {
		if (!xdr_check_write_chunk(rctxt))
			return false;
		++rctxt->rc_write_pcl.cl_count;
		p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
		if (!p)
			return false;
	}
	return true;
>>>>>>> upstream/android-13
}

/* Sanity check the Write list.
 *
 * Implementation limits:
<<<<<<< HEAD
 * - This implementation supports only one Write chunk.
 *
 * Sanity checks:
 * - Write list does not overflow buffer.
 * - Segment size limited by largest NFS data payload.
 *
 * Returns pointer to the following Reply chunk.
 */
static __be32 *xdr_check_write_list(__be32 *p, const __be32 *end)
{
	u32 chcount;

	chcount = 0;
	while (*p++ != xdr_zero) {
		p = xdr_check_write_chunk(p, end, MAX_BYTES_WRITE_SEG);
		if (!p)
			return NULL;
		if (chcount++ > 1)
			return NULL;
	}
	return p;
=======
 * - This implementation currently supports only one Write chunk.
 *
 * Sanity checks:
 * - Write list does not overflow Receive buffer.
 * - Chunk size limited by largest NFS data payload.
 *
 * Return values:
 *       %true: Write list is valid. @rctxt's xdr_stream is updated
 *		to point to the first byte past the Write list.
 *      %false: Write list is corrupt. @rctxt's xdr_stream is left
 *		in an unknown state.
 */
static bool xdr_check_write_list(struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p;

	p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
	if (!p)
		return false;
	if (!xdr_count_write_chunks(rctxt, p))
		return false;
	if (!pcl_alloc_write(rctxt, &rctxt->rc_write_pcl, p))
		return false;

	rctxt->rc_cur_result_payload = pcl_first_chunk(&rctxt->rc_write_pcl);
	return true;
>>>>>>> upstream/android-13
}

/* Sanity check the Reply chunk.
 *
 * Sanity checks:
<<<<<<< HEAD
 * - Reply chunk does not overflow buffer.
 * - Segment size limited by largest NFS data payload.
 *
 * Returns pointer to the following RPC header.
 */
static __be32 *xdr_check_reply_chunk(__be32 *p, const __be32 *end)
{
	if (*p++ != xdr_zero) {
		p = xdr_check_write_chunk(p, end, MAX_BYTES_SPECIAL_SEG);
		if (!p)
			return NULL;
	}
	return p;
}

/* On entry, xdr->head[0].iov_base points to first byte in the
 * RPC-over-RDMA header.
 *
 * On successful exit, head[0] points to first byte past the
 * RPC-over-RDMA header. For RDMA_MSG, this is the RPC message.
=======
 * - Reply chunk does not overflow Receive buffer.
 * - Chunk size limited by largest NFS data payload.
 *
 * Return values:
 *       %true: Reply chunk is valid. @rctxt's xdr_stream is updated
 *		to point to the first byte past the Reply chunk.
 *      %false: Reply chunk is corrupt. @rctxt's xdr_stream is left
 *		in an unknown state.
 */
static bool xdr_check_reply_chunk(struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p;

	p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
	if (!p)
		return false;

	if (!xdr_item_is_present(p))
		return true;
	if (!xdr_check_write_chunk(rctxt))
		return false;

	rctxt->rc_reply_pcl.cl_count = 1;
	return pcl_alloc_write(rctxt, &rctxt->rc_reply_pcl, p);
}

/* RPC-over-RDMA Version One private extension: Remote Invalidation.
 * Responder's choice: requester signals it can handle Send With
 * Invalidate, and responder chooses one R_key to invalidate.
 *
 * If there is exactly one distinct R_key in the received transport
 * header, set rc_inv_rkey to that R_key. Otherwise, set it to zero.
 */
static void svc_rdma_get_inv_rkey(struct svcxprt_rdma *rdma,
				  struct svc_rdma_recv_ctxt *ctxt)
{
	struct svc_rdma_segment *segment;
	struct svc_rdma_chunk *chunk;
	u32 inv_rkey;

	ctxt->rc_inv_rkey = 0;

	if (!rdma->sc_snd_w_inv)
		return;

	inv_rkey = 0;
	pcl_for_each_chunk(chunk, &ctxt->rc_call_pcl) {
		pcl_for_each_segment(segment, chunk) {
			if (inv_rkey == 0)
				inv_rkey = segment->rs_handle;
			else if (inv_rkey != segment->rs_handle)
				return;
		}
	}
	pcl_for_each_chunk(chunk, &ctxt->rc_read_pcl) {
		pcl_for_each_segment(segment, chunk) {
			if (inv_rkey == 0)
				inv_rkey = segment->rs_handle;
			else if (inv_rkey != segment->rs_handle)
				return;
		}
	}
	pcl_for_each_chunk(chunk, &ctxt->rc_write_pcl) {
		pcl_for_each_segment(segment, chunk) {
			if (inv_rkey == 0)
				inv_rkey = segment->rs_handle;
			else if (inv_rkey != segment->rs_handle)
				return;
		}
	}
	pcl_for_each_chunk(chunk, &ctxt->rc_reply_pcl) {
		pcl_for_each_segment(segment, chunk) {
			if (inv_rkey == 0)
				inv_rkey = segment->rs_handle;
			else if (inv_rkey != segment->rs_handle)
				return;
		}
	}
	ctxt->rc_inv_rkey = inv_rkey;
}

/**
 * svc_rdma_xdr_decode_req - Decode the transport header
 * @rq_arg: xdr_buf containing ingress RPC/RDMA message
 * @rctxt: state of decoding
 *
 * On entry, xdr->head[0].iov_base points to first byte of the
 * RPC-over-RDMA transport header.
 *
 * On successful exit, head[0] points to first byte past the
 * RPC-over-RDMA header. For RDMA_MSG, this is the RPC message.
 *
>>>>>>> upstream/android-13
 * The length of the RPC-over-RDMA header is returned.
 *
 * Assumptions:
 * - The transport header is entirely contained in the head iovec.
 */
<<<<<<< HEAD
static int svc_rdma_xdr_decode_req(struct xdr_buf *rq_arg)
{
	__be32 *p, *end, *rdma_argp;
	unsigned int hdr_len;

	/* Verify that there's enough bytes for header + something */
	if (rq_arg->len <= RPCRDMA_HDRLEN_ERR)
		goto out_short;

	rdma_argp = rq_arg->head[0].iov_base;
	if (*(rdma_argp + 1) != rpcrdma_version)
		goto out_version;

	switch (*(rdma_argp + 3)) {
=======
static int svc_rdma_xdr_decode_req(struct xdr_buf *rq_arg,
				   struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p, *rdma_argp;
	unsigned int hdr_len;

	rdma_argp = rq_arg->head[0].iov_base;
	xdr_init_decode(&rctxt->rc_stream, rq_arg, rdma_argp, NULL);

	p = xdr_inline_decode(&rctxt->rc_stream,
			      rpcrdma_fixed_maxsz * sizeof(*p));
	if (unlikely(!p))
		goto out_short;
	p++;
	if (*p != rpcrdma_version)
		goto out_version;
	p += 2;
	rctxt->rc_msgtype = *p;
	switch (rctxt->rc_msgtype) {
>>>>>>> upstream/android-13
	case rdma_msg:
		break;
	case rdma_nomsg:
		break;
<<<<<<< HEAD

	case rdma_done:
		goto out_drop;

	case rdma_error:
		goto out_drop;

=======
	case rdma_done:
		goto out_drop;
	case rdma_error:
		goto out_drop;
>>>>>>> upstream/android-13
	default:
		goto out_proc;
	}

<<<<<<< HEAD
	end = (__be32 *)((unsigned long)rdma_argp + rq_arg->len);
	p = xdr_check_read_list(rdma_argp + 4, end);
	if (!p)
		goto out_inval;
	p = xdr_check_write_list(p, end);
	if (!p)
		goto out_inval;
	p = xdr_check_reply_chunk(p, end);
	if (!p)
		goto out_inval;
	if (p > end)
		goto out_inval;

	rq_arg->head[0].iov_base = p;
	hdr_len = (unsigned long)p - (unsigned long)rdma_argp;
	rq_arg->head[0].iov_len -= hdr_len;
	rq_arg->len -= hdr_len;
	trace_svcrdma_decode_rqst(rdma_argp, hdr_len);
	return hdr_len;

out_short:
	trace_svcrdma_decode_short(rq_arg->len);
	return -EINVAL;

out_version:
	trace_svcrdma_decode_badvers(rdma_argp);
	return -EPROTONOSUPPORT;

out_drop:
	trace_svcrdma_decode_drop(rdma_argp);
	return 0;

out_proc:
	trace_svcrdma_decode_badproc(rdma_argp);
	return -EINVAL;

out_inval:
	trace_svcrdma_decode_parse(rdma_argp);
	return -EINVAL;
}

static void rdma_read_complete(struct svc_rqst *rqstp,
			       struct svc_rdma_recv_ctxt *head)
{
	int page_no;

	/* Move Read chunk pages to rqstp so that they will be released
	 * when svc_process is done with them.
	 */
	for (page_no = 0; page_no < head->rc_page_count; page_no++) {
		put_page(rqstp->rq_pages[page_no]);
		rqstp->rq_pages[page_no] = head->rc_pages[page_no];
	}
	head->rc_page_count = 0;

	/* Point rq_arg.pages past header */
	rqstp->rq_arg.pages = &rqstp->rq_pages[head->rc_hdr_count];
	rqstp->rq_arg.page_len = head->rc_arg.page_len;

	/* rq_respages starts after the last arg page */
	rqstp->rq_respages = &rqstp->rq_pages[page_no];
	rqstp->rq_next_page = rqstp->rq_respages + 1;

	/* Rebuild rq_arg head and tail. */
	rqstp->rq_arg.head[0] = head->rc_arg.head[0];
	rqstp->rq_arg.tail[0] = head->rc_arg.tail[0];
	rqstp->rq_arg.len = head->rc_arg.len;
	rqstp->rq_arg.buflen = head->rc_arg.buflen;
}

static void svc_rdma_send_error(struct svcxprt_rdma *xprt,
				__be32 *rdma_argp, int status)
{
	struct svc_rdma_send_ctxt *ctxt;
	unsigned int length;
	__be32 *p;
	int ret;

	ctxt = svc_rdma_send_ctxt_get(xprt);
	if (!ctxt)
		return;

	p = ctxt->sc_xprt_buf;
	*p++ = *rdma_argp;
	*p++ = *(rdma_argp + 1);
	*p++ = xprt->sc_fc_credits;
	*p++ = rdma_error;
	switch (status) {
	case -EPROTONOSUPPORT:
		*p++ = err_vers;
		*p++ = rpcrdma_version;
		*p++ = rpcrdma_version;
		trace_svcrdma_err_vers(*rdma_argp);
		break;
	default:
		*p++ = err_chunk;
		trace_svcrdma_err_chunk(*rdma_argp);
	}
	length = (unsigned long)p - (unsigned long)ctxt->sc_xprt_buf;
	svc_rdma_sync_reply_hdr(xprt, ctxt, length);

	ctxt->sc_send_wr.opcode = IB_WR_SEND;
	ret = svc_rdma_send(xprt, &ctxt->sc_send_wr);
	if (ret)
		svc_rdma_send_ctxt_put(xprt, ctxt);
=======
	if (!xdr_check_read_list(rctxt))
		goto out_inval;
	if (!xdr_check_write_list(rctxt))
		goto out_inval;
	if (!xdr_check_reply_chunk(rctxt))
		goto out_inval;

	rq_arg->head[0].iov_base = rctxt->rc_stream.p;
	hdr_len = xdr_stream_pos(&rctxt->rc_stream);
	rq_arg->head[0].iov_len -= hdr_len;
	rq_arg->len -= hdr_len;
	trace_svcrdma_decode_rqst(rctxt, rdma_argp, hdr_len);
	return hdr_len;

out_short:
	trace_svcrdma_decode_short_err(rctxt, rq_arg->len);
	return -EINVAL;

out_version:
	trace_svcrdma_decode_badvers_err(rctxt, rdma_argp);
	return -EPROTONOSUPPORT;

out_drop:
	trace_svcrdma_decode_drop_err(rctxt, rdma_argp);
	return 0;

out_proc:
	trace_svcrdma_decode_badproc_err(rctxt, rdma_argp);
	return -EINVAL;

out_inval:
	trace_svcrdma_decode_parse_err(rctxt, rdma_argp);
	return -EINVAL;
}

static void svc_rdma_send_error(struct svcxprt_rdma *rdma,
				struct svc_rdma_recv_ctxt *rctxt,
				int status)
{
	struct svc_rdma_send_ctxt *sctxt;

	sctxt = svc_rdma_send_ctxt_get(rdma);
	if (!sctxt)
		return;
	svc_rdma_send_error_msg(rdma, sctxt, rctxt, status);
>>>>>>> upstream/android-13
}

/* By convention, backchannel calls arrive via rdma_msg type
 * messages, and never populate the chunk lists. This makes
 * the RPC/RDMA header small and fixed in size, so it is
 * straightforward to check the RPC header's direction field.
 */
<<<<<<< HEAD
static bool svc_rdma_is_backchannel_reply(struct svc_xprt *xprt,
					  __be32 *rdma_resp)
{
	__be32 *p;
=======
static bool svc_rdma_is_reverse_direction_reply(struct svc_xprt *xprt,
						struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p = rctxt->rc_recv_buf;
>>>>>>> upstream/android-13

	if (!xprt->xpt_bc_xprt)
		return false;

<<<<<<< HEAD
	p = rdma_resp + 3;
	if (*p++ != rdma_msg)
		return false;

	if (*p++ != xdr_zero)
		return false;
	if (*p++ != xdr_zero)
		return false;
	if (*p++ != xdr_zero)
		return false;

	/* XID sanity */
	if (*p++ != *rdma_resp)
		return false;
	/* call direction */
	if (*p == cpu_to_be32(RPC_CALL))
=======
	if (rctxt->rc_msgtype != rdma_msg)
		return false;

	if (!pcl_is_empty(&rctxt->rc_call_pcl))
		return false;
	if (!pcl_is_empty(&rctxt->rc_read_pcl))
		return false;
	if (!pcl_is_empty(&rctxt->rc_write_pcl))
		return false;
	if (!pcl_is_empty(&rctxt->rc_reply_pcl))
		return false;

	/* RPC call direction */
	if (*(p + 8) == cpu_to_be32(RPC_CALL))
>>>>>>> upstream/android-13
		return false;

	return true;
}

/**
 * svc_rdma_recvfrom - Receive an RPC call
 * @rqstp: request structure into which to receive an RPC Call
 *
 * Returns:
 *	The positive number of bytes in the RPC Call message,
 *	%0 if there were no Calls ready to return,
 *	%-EINVAL if the Read chunk data is too large,
 *	%-ENOMEM if rdma_rw context pool was exhausted,
 *	%-ENOTCONN if posting failed (connection is lost),
 *	%-EIO if rdma_rw initialization failed (DMA mapping, etc).
 *
 * Called in a loop when XPT_DATA is set. XPT_DATA is cleared only
 * when there are no remaining ctxt's to process.
 *
 * The next ctxt is removed from the "receive" lists.
 *
 * - If the ctxt completes a Read, then finish assembling the Call
 *   message and return the number of bytes in the message.
 *
 * - If the ctxt completes a Receive, then construct the Call
 *   message from the contents of the Receive buffer.
 *
 *   - If there are no Read chunks in this message, then finish
 *     assembling the Call message and return the number of bytes
 *     in the message.
 *
 *   - If there are Read chunks in this message, post Read WRs to
 *     pull that payload and return 0.
 */
int svc_rdma_recvfrom(struct svc_rqst *rqstp)
{
	struct svc_xprt *xprt = rqstp->rq_xprt;
	struct svcxprt_rdma *rdma_xprt =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);
	struct svc_rdma_recv_ctxt *ctxt;
<<<<<<< HEAD
	__be32 *p;
=======
>>>>>>> upstream/android-13
	int ret;

	rqstp->rq_xprt_ctxt = NULL;

<<<<<<< HEAD
	spin_lock(&rdma_xprt->sc_rq_dto_lock);
	ctxt = svc_rdma_next_recv_ctxt(&rdma_xprt->sc_read_complete_q);
	if (ctxt) {
		list_del(&ctxt->rc_list);
		spin_unlock(&rdma_xprt->sc_rq_dto_lock);
		rdma_read_complete(rqstp, ctxt);
		goto complete;
	}
	ctxt = svc_rdma_next_recv_ctxt(&rdma_xprt->sc_rq_dto_q);
	if (!ctxt) {
		/* No new incoming requests, terminate the loop */
		clear_bit(XPT_DATA, &xprt->xpt_flags);
		spin_unlock(&rdma_xprt->sc_rq_dto_lock);
		return 0;
	}
	list_del(&ctxt->rc_list);
	spin_unlock(&rdma_xprt->sc_rq_dto_lock);

	atomic_inc(&rdma_stat_recv);

=======
	ctxt = NULL;
	spin_lock(&rdma_xprt->sc_rq_dto_lock);
	ctxt = svc_rdma_next_recv_ctxt(&rdma_xprt->sc_rq_dto_q);
	if (ctxt)
		list_del(&ctxt->rc_list);
	else
		/* No new incoming requests, terminate the loop */
		clear_bit(XPT_DATA, &xprt->xpt_flags);
	spin_unlock(&rdma_xprt->sc_rq_dto_lock);

	/* Unblock the transport for the next receive */
	svc_xprt_received(xprt);
	if (!ctxt)
		return 0;

	percpu_counter_inc(&svcrdma_stat_recv);
	ib_dma_sync_single_for_cpu(rdma_xprt->sc_pd->device,
				   ctxt->rc_recv_sge.addr, ctxt->rc_byte_len,
				   DMA_FROM_DEVICE);
>>>>>>> upstream/android-13
	svc_rdma_build_arg_xdr(rqstp, ctxt);

	/* Prevent svc_xprt_release from releasing pages in rq_pages
	 * if we return 0 or an error.
	 */
	rqstp->rq_respages = rqstp->rq_pages;
	rqstp->rq_next_page = rqstp->rq_respages;

<<<<<<< HEAD
	p = (__be32 *)rqstp->rq_arg.head[0].iov_base;
	ret = svc_rdma_xdr_decode_req(&rqstp->rq_arg);
=======
	ret = svc_rdma_xdr_decode_req(&rqstp->rq_arg, ctxt);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto out_err;
	if (ret == 0)
		goto out_drop;
<<<<<<< HEAD
	rqstp->rq_xprt_hlen = ret;

	if (svc_rdma_is_backchannel_reply(xprt, p)) {
		ret = svc_rdma_handle_bc_reply(xprt->xpt_bc_xprt, p,
					       &rqstp->rq_arg);
		svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
		return ret;
	}

	p += rpcrdma_fixed_maxsz;
	if (*p != xdr_zero)
		goto out_readchunk;

complete:
=======
	rqstp->rq_xprt_hlen = 0;

	if (svc_rdma_is_reverse_direction_reply(xprt, ctxt))
		goto out_backchannel;

	svc_rdma_get_inv_rkey(rdma_xprt, ctxt);

	if (!pcl_is_empty(&ctxt->rc_read_pcl) ||
	    !pcl_is_empty(&ctxt->rc_call_pcl)) {
		ret = svc_rdma_process_read_list(rdma_xprt, rqstp, ctxt);
		if (ret < 0)
			goto out_readfail;
	}

>>>>>>> upstream/android-13
	rqstp->rq_xprt_ctxt = ctxt;
	rqstp->rq_prot = IPPROTO_MAX;
	svc_xprt_copy_addrs(rqstp, xprt);
	return rqstp->rq_arg.len;

<<<<<<< HEAD
out_readchunk:
	ret = svc_rdma_recv_read_chunk(rdma_xprt, rqstp, ctxt, p);
	if (ret < 0)
		goto out_postfail;
	return 0;

out_err:
	svc_rdma_send_error(rdma_xprt, p, ret);
	svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
	return 0;

out_postfail:
	if (ret == -EINVAL)
		svc_rdma_send_error(rdma_xprt, p, ret);
	svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
	return ret;

=======
out_err:
	svc_rdma_send_error(rdma_xprt, ctxt, ret);
	svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
	return 0;

out_readfail:
	if (ret == -EINVAL)
		svc_rdma_send_error(rdma_xprt, ctxt, ret);
	svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
	return ret;

out_backchannel:
	svc_rdma_handle_bc_reply(rqstp, ctxt);
>>>>>>> upstream/android-13
out_drop:
	svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
	return 0;
}
