// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2015-2018 Oracle.  All rights reserved.
 *
<<<<<<< HEAD
 * Support for backward direction RPCs on RPC/RDMA (server-side).
 */

#include <linux/module.h>

=======
 * Support for reverse-direction RPCs on RPC/RDMA (server-side).
 */

>>>>>>> upstream/android-13
#include <linux/sunrpc/svc_rdma.h>

#include "xprt_rdma.h"
#include <trace/events/rpcrdma.h>

<<<<<<< HEAD
#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

#undef SVCRDMA_BACKCHANNEL_DEBUG

/**
 * svc_rdma_handle_bc_reply - Process incoming backchannel reply
 * @xprt: controlling backchannel transport
 * @rdma_resp: pointer to incoming transport header
 * @rcvbuf: XDR buffer into which to decode the reply
 *
 * Returns:
 *	%0 if @rcvbuf is filled in, xprt_complete_rqst called,
 *	%-EAGAIN if server should call ->recvfrom again.
 */
int svc_rdma_handle_bc_reply(struct rpc_xprt *xprt, __be32 *rdma_resp,
			     struct xdr_buf *rcvbuf)
{
	struct rpcrdma_xprt *r_xprt = rpcx_to_rdmax(xprt);
	struct kvec *dst, *src = &rcvbuf->head[0];
	struct rpc_rqst *req;
	unsigned long cwnd;
	u32 credits;
	size_t len;
	__be32 xid;
	__be32 *p;
	int ret;

	p = (__be32 *)src->iov_base;
	len = src->iov_len;
	xid = *rdma_resp;

#ifdef SVCRDMA_BACKCHANNEL_DEBUG
	pr_info("%s: xid=%08x, length=%zu\n",
		__func__, be32_to_cpu(xid), len);
	pr_info("%s: RPC/RDMA: %*ph\n",
		__func__, (int)RPCRDMA_HDRLEN_MIN, rdma_resp);
	pr_info("%s:      RPC: %*ph\n",
		__func__, (int)len, p);
#endif

	ret = -EAGAIN;
	if (src->iov_len < 24)
		goto out_shortreply;

	spin_lock(&xprt->recv_lock);
	req = xprt_lookup_rqst(xprt, xid);
	if (!req)
		goto out_notfound;

	dst = &req->rq_private_buf.head[0];
	memcpy(&req->rq_private_buf, &req->rq_rcv_buf, sizeof(struct xdr_buf));
	if (dst->iov_len < len)
		goto out_unlock;
	memcpy(dst->iov_base, p, len);
=======
/**
 * svc_rdma_handle_bc_reply - Process incoming backchannel Reply
 * @rqstp: resources for handling the Reply
 * @rctxt: Received message
 *
 */
void svc_rdma_handle_bc_reply(struct svc_rqst *rqstp,
			      struct svc_rdma_recv_ctxt *rctxt)
{
	struct svc_xprt *sxprt = rqstp->rq_xprt;
	struct rpc_xprt *xprt = sxprt->xpt_bc_xprt;
	struct rpcrdma_xprt *r_xprt = rpcx_to_rdmax(xprt);
	struct xdr_buf *rcvbuf = &rqstp->rq_arg;
	struct kvec *dst, *src = &rcvbuf->head[0];
	__be32 *rdma_resp = rctxt->rc_recv_buf;
	struct rpc_rqst *req;
	u32 credits;

	spin_lock(&xprt->queue_lock);
	req = xprt_lookup_rqst(xprt, *rdma_resp);
	if (!req)
		goto out_unlock;

	dst = &req->rq_private_buf.head[0];
	memcpy(&req->rq_private_buf, &req->rq_rcv_buf, sizeof(struct xdr_buf));
	if (dst->iov_len < src->iov_len)
		goto out_unlock;
	memcpy(dst->iov_base, src->iov_base, src->iov_len);
	xprt_pin_rqst(req);
	spin_unlock(&xprt->queue_lock);
>>>>>>> upstream/android-13

	credits = be32_to_cpup(rdma_resp + 2);
	if (credits == 0)
		credits = 1;	/* don't deadlock */
	else if (credits > r_xprt->rx_buf.rb_bc_max_requests)
		credits = r_xprt->rx_buf.rb_bc_max_requests;
<<<<<<< HEAD

	spin_lock_bh(&xprt->transport_lock);
	cwnd = xprt->cwnd;
	xprt->cwnd = credits << RPC_CWNDSHIFT;
	if (xprt->cwnd > cwnd)
		xprt_release_rqst_cong(req->rq_task);
	spin_unlock_bh(&xprt->transport_lock);


	ret = 0;
	xprt_complete_rqst(req->rq_task, rcvbuf->len);
	rcvbuf->len = 0;

out_unlock:
	spin_unlock(&xprt->recv_lock);
out:
	return ret;

out_shortreply:
	dprintk("svcrdma: short bc reply: xprt=%p, len=%zu\n",
		xprt, src->iov_len);
	goto out;

out_notfound:
	dprintk("svcrdma: unrecognized bc reply: xprt=%p, xid=%08x\n",
		xprt, be32_to_cpu(xid));
	goto out_unlock;
}

/* Send a backwards direction RPC call.
=======
	spin_lock(&xprt->transport_lock);
	xprt->cwnd = credits << RPC_CWNDSHIFT;
	spin_unlock(&xprt->transport_lock);

	spin_lock(&xprt->queue_lock);
	xprt_complete_rqst(req->rq_task, rcvbuf->len);
	xprt_unpin_rqst(req);
	rcvbuf->len = 0;

out_unlock:
	spin_unlock(&xprt->queue_lock);
}

/* Send a reverse-direction RPC Call.
>>>>>>> upstream/android-13
 *
 * Caller holds the connection's mutex and has already marshaled
 * the RPC/RDMA request.
 *
 * This is similar to svc_rdma_send_reply_msg, but takes a struct
 * rpc_rqst instead, does not support chunks, and avoids blocking
 * memory allocation.
 *
 * XXX: There is still an opportunity to block in svc_rdma_send()
 * if there are no SQ entries to post the Send. This may occur if
 * the adapter has a small maximum SQ depth.
 */
static int svc_rdma_bc_sendto(struct svcxprt_rdma *rdma,
			      struct rpc_rqst *rqst,
<<<<<<< HEAD
			      struct svc_rdma_send_ctxt *ctxt)
{
	int ret;

	ret = svc_rdma_map_reply_msg(rdma, ctxt, &rqst->rq_snd_buf, NULL);
=======
			      struct svc_rdma_send_ctxt *sctxt)
{
	struct svc_rdma_recv_ctxt *rctxt;
	int ret;

	rctxt = svc_rdma_recv_ctxt_get(rdma);
	if (!rctxt)
		return -EIO;

	ret = svc_rdma_map_reply_msg(rdma, sctxt, rctxt, &rqst->rq_snd_buf);
	svc_rdma_recv_ctxt_put(rdma, rctxt);
>>>>>>> upstream/android-13
	if (ret < 0)
		return -EIO;

	/* Bump page refcnt so Send completion doesn't release
	 * the rq_buffer before all retransmits are complete.
	 */
	get_page(virt_to_page(rqst->rq_buffer));
<<<<<<< HEAD
	ctxt->sc_send_wr.opcode = IB_WR_SEND;
	return svc_rdma_send(rdma, &ctxt->sc_send_wr);
=======
	sctxt->sc_send_wr.opcode = IB_WR_SEND;
	ret = svc_rdma_send(rdma, sctxt);
	if (ret < 0)
		return ret;

	ret = wait_for_completion_killable(&sctxt->sc_done);
	svc_rdma_send_ctxt_put(rdma, sctxt);
	return ret;
>>>>>>> upstream/android-13
}

/* Server-side transport endpoint wants a whole page for its send
 * buffer. The client RPC code constructs the RPC header in this
 * buffer before it invokes ->send_request.
 */
static int
xprt_rdma_bc_allocate(struct rpc_task *task)
{
	struct rpc_rqst *rqst = task->tk_rqstp;
	size_t size = rqst->rq_callsize;
	struct page *page;

	if (size > PAGE_SIZE) {
		WARN_ONCE(1, "svcrdma: large bc buffer request (size %zu)\n",
			  size);
		return -EINVAL;
	}

	page = alloc_page(RPCRDMA_DEF_GFP);
	if (!page)
		return -ENOMEM;
	rqst->rq_buffer = page_address(page);

	rqst->rq_rbuffer = kmalloc(rqst->rq_rcvsize, RPCRDMA_DEF_GFP);
	if (!rqst->rq_rbuffer) {
		put_page(page);
		return -ENOMEM;
	}
	return 0;
}

static void
xprt_rdma_bc_free(struct rpc_task *task)
{
	struct rpc_rqst *rqst = task->tk_rqstp;

	put_page(virt_to_page(rqst->rq_buffer));
	kfree(rqst->rq_rbuffer);
}

static int
rpcrdma_bc_send_request(struct svcxprt_rdma *rdma, struct rpc_rqst *rqst)
{
	struct rpc_xprt *xprt = rqst->rq_xprt;
	struct rpcrdma_xprt *r_xprt = rpcx_to_rdmax(xprt);
	struct svc_rdma_send_ctxt *ctxt;
	__be32 *p;
	int rc;

	ctxt = svc_rdma_send_ctxt_get(rdma);
	if (!ctxt)
		goto drop_connection;

<<<<<<< HEAD
	p = ctxt->sc_xprt_buf;
=======
	p = xdr_reserve_space(&ctxt->sc_stream, RPCRDMA_HDRLEN_MIN);
	if (!p)
		goto put_ctxt;
>>>>>>> upstream/android-13
	*p++ = rqst->rq_xid;
	*p++ = rpcrdma_version;
	*p++ = cpu_to_be32(r_xprt->rx_buf.rb_bc_max_requests);
	*p++ = rdma_msg;
	*p++ = xdr_zero;
	*p++ = xdr_zero;
	*p   = xdr_zero;
<<<<<<< HEAD
	svc_rdma_sync_reply_hdr(rdma, ctxt, RPCRDMA_HDRLEN_MIN);

#ifdef SVCRDMA_BACKCHANNEL_DEBUG
	pr_info("%s: %*ph\n", __func__, 64, rqst->rq_buffer);
#endif

	rc = svc_rdma_bc_sendto(rdma, rqst, ctxt);
	if (rc) {
		svc_rdma_send_ctxt_put(rdma, ctxt);
		goto drop_connection;
	}
	return rc;

drop_connection:
	dprintk("svcrdma: failed to send bc call\n");
	xprt_disconnect_done(xprt);
	return -ENOTCONN;
}

/* Send an RPC call on the passive end of a transport
 * connection.
 */
static int
xprt_rdma_bc_send_request(struct rpc_task *task)
{
	struct rpc_rqst *rqst = task->tk_rqstp;
	struct svc_xprt *sxprt = rqst->rq_xprt->bc_xprt;
	struct svcxprt_rdma *rdma;
	int ret;

	dprintk("svcrdma: sending bc call with xid: %08x\n",
		be32_to_cpu(rqst->rq_xid));

	if (!mutex_trylock(&sxprt->xpt_mutex)) {
		rpc_sleep_on(&sxprt->xpt_bc_pending, task, NULL);
		if (!mutex_trylock(&sxprt->xpt_mutex))
			return -EAGAIN;
		rpc_wake_up_queued_task(&sxprt->xpt_bc_pending, task);
	}

	ret = -ENOTCONN;
	rdma = container_of(sxprt, struct svcxprt_rdma, sc_xprt);
	if (!test_bit(XPT_DEAD, &sxprt->xpt_flags))
		ret = rpcrdma_bc_send_request(rdma, rqst);

	mutex_unlock(&sxprt->xpt_mutex);

	if (ret < 0)
		return ret;
	return 0;
=======

	rqst->rq_xtime = ktime_get();
	rc = svc_rdma_bc_sendto(rdma, rqst, ctxt);
	if (rc)
		goto put_ctxt;
	return 0;

put_ctxt:
	svc_rdma_send_ctxt_put(rdma, ctxt);

drop_connection:
	return -ENOTCONN;
}

/**
 * xprt_rdma_bc_send_request - Send a reverse-direction Call
 * @rqst: rpc_rqst containing Call message to be sent
 *
 * Return values:
 *   %0 if the message was sent successfully
 *   %ENOTCONN if the message was not sent
 */
static int xprt_rdma_bc_send_request(struct rpc_rqst *rqst)
{
	struct svc_xprt *sxprt = rqst->rq_xprt->bc_xprt;
	struct svcxprt_rdma *rdma =
		container_of(sxprt, struct svcxprt_rdma, sc_xprt);
	int ret;

	if (test_bit(XPT_DEAD, &sxprt->xpt_flags))
		return -ENOTCONN;

	ret = rpcrdma_bc_send_request(rdma, rqst);
	if (ret == -ENOTCONN)
		svc_close_xprt(sxprt);
	return ret;
>>>>>>> upstream/android-13
}

static void
xprt_rdma_bc_close(struct rpc_xprt *xprt)
{
<<<<<<< HEAD
	dprintk("svcrdma: %s: xprt %p\n", __func__, xprt);
=======
	xprt_disconnect_done(xprt);
>>>>>>> upstream/android-13
	xprt->cwnd = RPC_CWNDSHIFT;
}

static void
xprt_rdma_bc_put(struct rpc_xprt *xprt)
{
<<<<<<< HEAD
	dprintk("svcrdma: %s: xprt %p\n", __func__, xprt);

	xprt_rdma_free_addresses(xprt);
	xprt_free(xprt);
	module_put(THIS_MODULE);
=======
	xprt_rdma_free_addresses(xprt);
	xprt_free(xprt);
>>>>>>> upstream/android-13
}

static const struct rpc_xprt_ops xprt_rdma_bc_procs = {
	.reserve_xprt		= xprt_reserve_xprt_cong,
	.release_xprt		= xprt_release_xprt_cong,
	.alloc_slot		= xprt_alloc_slot,
	.free_slot		= xprt_free_slot,
	.release_request	= xprt_release_rqst_cong,
	.buf_alloc		= xprt_rdma_bc_allocate,
	.buf_free		= xprt_rdma_bc_free,
	.send_request		= xprt_rdma_bc_send_request,
<<<<<<< HEAD
	.set_retrans_timeout	= xprt_set_retrans_timeout_def,
=======
	.wait_for_reply_request	= xprt_wait_for_reply_request_def,
>>>>>>> upstream/android-13
	.close			= xprt_rdma_bc_close,
	.destroy		= xprt_rdma_bc_put,
	.print_stats		= xprt_rdma_print_stats
};

static const struct rpc_timeout xprt_rdma_bc_timeout = {
	.to_initval = 60 * HZ,
	.to_maxval = 60 * HZ,
};

/* It shouldn't matter if the number of backchannel session slots
 * doesn't match the number of RPC/RDMA credits. That just means
 * one or the other will have extra slots that aren't used.
 */
static struct rpc_xprt *
xprt_setup_rdma_bc(struct xprt_create *args)
{
	struct rpc_xprt *xprt;
	struct rpcrdma_xprt *new_xprt;

<<<<<<< HEAD
	if (args->addrlen > sizeof(xprt->addr)) {
		dprintk("RPC:       %s: address too large\n", __func__);
		return ERR_PTR(-EBADF);
	}
=======
	if (args->addrlen > sizeof(xprt->addr))
		return ERR_PTR(-EBADF);
>>>>>>> upstream/android-13

	xprt = xprt_alloc(args->net, sizeof(*new_xprt),
			  RPCRDMA_MAX_BC_REQUESTS,
			  RPCRDMA_MAX_BC_REQUESTS);
<<<<<<< HEAD
	if (!xprt) {
		dprintk("RPC:       %s: couldn't allocate rpc_xprt\n",
			__func__);
		return ERR_PTR(-ENOMEM);
	}
=======
	if (!xprt)
		return ERR_PTR(-ENOMEM);
>>>>>>> upstream/android-13

	xprt->timeout = &xprt_rdma_bc_timeout;
	xprt_set_bound(xprt);
	xprt_set_connected(xprt);
	xprt->bind_timeout = 0;
	xprt->reestablish_timeout = 0;
	xprt->idle_timeout = 0;

	xprt->prot = XPRT_TRANSPORT_BC_RDMA;
<<<<<<< HEAD
	xprt->tsh_size = 0;
=======
>>>>>>> upstream/android-13
	xprt->ops = &xprt_rdma_bc_procs;

	memcpy(&xprt->addr, args->dstaddr, args->addrlen);
	xprt->addrlen = args->addrlen;
	xprt_rdma_format_addresses(xprt, (struct sockaddr *)&xprt->addr);
	xprt->resvport = 0;

	xprt->max_payload = xprt_rdma_max_inline_read;

	new_xprt = rpcx_to_rdmax(xprt);
	new_xprt->rx_buf.rb_bc_max_requests = xprt->max_reqs;

	xprt_get(xprt);
	args->bc_xprt->xpt_bc_xprt = xprt;
	xprt->bc_xprt = args->bc_xprt;

<<<<<<< HEAD
	if (!try_module_get(THIS_MODULE))
		goto out_fail;

	/* Final put for backchannel xprt is in __svc_rdma_free */
	xprt_get(xprt);
	return xprt;

out_fail:
	xprt_rdma_free_addresses(xprt);
	args->bc_xprt->xpt_bc_xprt = NULL;
	args->bc_xprt->xpt_bc_xps = NULL;
	xprt_put(xprt);
	xprt_free(xprt);
	return ERR_PTR(-EINVAL);
=======
	/* Final put for backchannel xprt is in __svc_rdma_free */
	xprt_get(xprt);
	return xprt;
>>>>>>> upstream/android-13
}

struct xprt_class xprt_rdma_bc = {
	.list			= LIST_HEAD_INIT(xprt_rdma_bc.list),
	.name			= "rdma backchannel",
	.owner			= THIS_MODULE,
	.ident			= XPRT_TRANSPORT_BC_RDMA,
	.setup			= xprt_setup_rdma_bc,
};
