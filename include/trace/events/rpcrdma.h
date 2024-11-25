/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2017, 2018 Oracle.  All rights reserved.
 *
 * Trace point definitions for the "rpcrdma" subsystem.
 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM rpcrdma

#if !defined(_TRACE_RPCRDMA_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_RPCRDMA_H

<<<<<<< HEAD
#include <linux/tracepoint.h>
=======
#include <linux/scatterlist.h>
#include <linux/sunrpc/rpc_rdma_cid.h>
#include <linux/tracepoint.h>
#include <rdma/ib_cm.h>
>>>>>>> upstream/android-13
#include <trace/events/rdma.h>

/**
 ** Event classes
 **/

<<<<<<< HEAD
DECLARE_EVENT_CLASS(xprtrdma_reply_event,
=======
DECLARE_EVENT_CLASS(rpcrdma_completion_class,
	TP_PROTO(
		const struct ib_wc *wc,
		const struct rpc_rdma_cid *cid
	),

	TP_ARGS(wc, cid),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
		__field(unsigned long, status)
		__field(unsigned int, vendor_err)
	),

	TP_fast_assign(
		__entry->cq_id = cid->ci_queue_id;
		__entry->completion_id = cid->ci_completion_id;
		__entry->status = wc->status;
		if (wc->status)
			__entry->vendor_err = wc->vendor_err;
		else
			__entry->vendor_err = 0;
	),

	TP_printk("cq.id=%u cid=%d status=%s (%lu/0x%x)",
		__entry->cq_id, __entry->completion_id,
		rdma_show_wc_status(__entry->status),
		__entry->status, __entry->vendor_err
	)
);

#define DEFINE_COMPLETION_EVENT(name)					\
		DEFINE_EVENT(rpcrdma_completion_class, name,		\
				TP_PROTO(				\
					const struct ib_wc *wc,		\
					const struct rpc_rdma_cid *cid	\
				),					\
				TP_ARGS(wc, cid))

DECLARE_EVENT_CLASS(rpcrdma_mr_completion_class,
	TP_PROTO(
		const struct ib_wc *wc,
		const struct rpc_rdma_cid *cid
	),

	TP_ARGS(wc, cid),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
		__field(unsigned long, status)
		__field(unsigned int, vendor_err)
	),

	TP_fast_assign(
		__entry->cq_id = cid->ci_queue_id;
		__entry->completion_id = cid->ci_completion_id;
		__entry->status = wc->status;
		if (wc->status)
			__entry->vendor_err = wc->vendor_err;
		else
			__entry->vendor_err = 0;
	),

	TP_printk("cq.id=%u mr.id=%d status=%s (%lu/0x%x)",
		__entry->cq_id, __entry->completion_id,
		rdma_show_wc_status(__entry->status),
		__entry->status, __entry->vendor_err
	)
);

#define DEFINE_MR_COMPLETION_EVENT(name)				\
		DEFINE_EVENT(rpcrdma_mr_completion_class, name,		\
				TP_PROTO(				\
					const struct ib_wc *wc,		\
					const struct rpc_rdma_cid *cid	\
				),					\
				TP_ARGS(wc, cid))

DECLARE_EVENT_CLASS(rpcrdma_receive_completion_class,
	TP_PROTO(
		const struct ib_wc *wc,
		const struct rpc_rdma_cid *cid
	),

	TP_ARGS(wc, cid),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
		__field(u32, received)
		__field(unsigned long, status)
		__field(unsigned int, vendor_err)
	),

	TP_fast_assign(
		__entry->cq_id = cid->ci_queue_id;
		__entry->completion_id = cid->ci_completion_id;
		__entry->status = wc->status;
		if (wc->status) {
			__entry->received = 0;
			__entry->vendor_err = wc->vendor_err;
		} else {
			__entry->received = wc->byte_len;
			__entry->vendor_err = 0;
		}
	),

	TP_printk("cq.id=%u cid=%d status=%s (%lu/0x%x) received=%u",
		__entry->cq_id, __entry->completion_id,
		rdma_show_wc_status(__entry->status),
		__entry->status, __entry->vendor_err,
		__entry->received
	)
);

#define DEFINE_RECEIVE_COMPLETION_EVENT(name)				\
		DEFINE_EVENT(rpcrdma_receive_completion_class, name,	\
				TP_PROTO(				\
					const struct ib_wc *wc,		\
					const struct rpc_rdma_cid *cid	\
				),					\
				TP_ARGS(wc, cid))

DECLARE_EVENT_CLASS(xprtrdma_reply_class,
>>>>>>> upstream/android-13
	TP_PROTO(
		const struct rpcrdma_rep *rep
	),

	TP_ARGS(rep),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(const void *, rep)
		__field(const void *, r_xprt)
		__field(u32, xid)
		__field(u32, version)
		__field(u32, proc)
	),

	TP_fast_assign(
		__entry->rep = rep;
		__entry->r_xprt = rep->rr_rxprt;
		__entry->xid = be32_to_cpu(rep->rr_xid);
		__entry->version = be32_to_cpu(rep->rr_vers);
		__entry->proc = be32_to_cpu(rep->rr_proc);
	),

	TP_printk("rxprt %p xid=0x%08x rep=%p: version %u proc %u",
		__entry->r_xprt, __entry->xid, __entry->rep,
		__entry->version, __entry->proc
=======
		__field(u32, xid)
		__field(u32, version)
		__field(u32, proc)
		__string(addr, rpcrdma_addrstr(rep->rr_rxprt))
		__string(port, rpcrdma_portstr(rep->rr_rxprt))
	),

	TP_fast_assign(
		__entry->xid = be32_to_cpu(rep->rr_xid);
		__entry->version = be32_to_cpu(rep->rr_vers);
		__entry->proc = be32_to_cpu(rep->rr_proc);
		__assign_str(addr, rpcrdma_addrstr(rep->rr_rxprt));
		__assign_str(port, rpcrdma_portstr(rep->rr_rxprt));
	),

	TP_printk("peer=[%s]:%s xid=0x%08x version=%u proc=%u",
		__get_str(addr), __get_str(port),
		__entry->xid, __entry->version, __entry->proc
>>>>>>> upstream/android-13
	)
);

#define DEFINE_REPLY_EVENT(name)					\
<<<<<<< HEAD
		DEFINE_EVENT(xprtrdma_reply_event, name,		\
=======
		DEFINE_EVENT(xprtrdma_reply_class,			\
				xprtrdma_reply_##name##_err,		\
>>>>>>> upstream/android-13
				TP_PROTO(				\
					const struct rpcrdma_rep *rep	\
				),					\
				TP_ARGS(rep))

DECLARE_EVENT_CLASS(xprtrdma_rxprt,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt
	),

	TP_ARGS(r_xprt),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(const void *, r_xprt)
=======
>>>>>>> upstream/android-13
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->r_xprt = r_xprt;
=======
>>>>>>> upstream/android-13
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

<<<<<<< HEAD
	TP_printk("peer=[%s]:%s r_xprt=%p",
		__get_str(addr), __get_str(port), __entry->r_xprt
=======
	TP_printk("peer=[%s]:%s",
		__get_str(addr), __get_str(port)
>>>>>>> upstream/android-13
	)
);

#define DEFINE_RXPRT_EVENT(name)					\
		DEFINE_EVENT(xprtrdma_rxprt, name,			\
				TP_PROTO(				\
					const struct rpcrdma_xprt *r_xprt \
				),					\
				TP_ARGS(r_xprt))

<<<<<<< HEAD
=======
DECLARE_EVENT_CLASS(xprtrdma_connect_class,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		int rc
	),

	TP_ARGS(r_xprt, rc),

	TP_STRUCT__entry(
		__field(int, rc)
		__field(int, connect_status)
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
		__entry->rc = rc;
		__entry->connect_status = r_xprt->rx_ep->re_connect_status;
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

	TP_printk("peer=[%s]:%s rc=%d connection status=%d",
		__get_str(addr), __get_str(port),
		__entry->rc, __entry->connect_status
	)
);

#define DEFINE_CONN_EVENT(name)						\
		DEFINE_EVENT(xprtrdma_connect_class, xprtrdma_##name,	\
				TP_PROTO(				\
					const struct rpcrdma_xprt *r_xprt, \
					int rc				\
				),					\
				TP_ARGS(r_xprt, rc))

>>>>>>> upstream/android-13
DECLARE_EVENT_CLASS(xprtrdma_rdch_event,
	TP_PROTO(
		const struct rpc_task *task,
		unsigned int pos,
		struct rpcrdma_mr *mr,
		int nsegs
	),

	TP_ARGS(task, pos, mr, nsegs),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
<<<<<<< HEAD
		__field(const void *, mr)
=======
>>>>>>> upstream/android-13
		__field(unsigned int, pos)
		__field(int, nents)
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
		__field(int, nsegs)
	),

	TP_fast_assign(
		__entry->task_id = task->tk_pid;
		__entry->client_id = task->tk_client->cl_clid;
<<<<<<< HEAD
		__entry->mr = mr;
=======
>>>>>>> upstream/android-13
		__entry->pos = pos;
		__entry->nents = mr->mr_nents;
		__entry->handle = mr->mr_handle;
		__entry->length = mr->mr_length;
		__entry->offset = mr->mr_offset;
		__entry->nsegs = nsegs;
	),

<<<<<<< HEAD
	TP_printk("task:%u@%u mr=%p pos=%u %u@0x%016llx:0x%08x (%s)",
		__entry->task_id, __entry->client_id, __entry->mr,
=======
	TP_printk("task:%u@%u pos=%u %u@0x%016llx:0x%08x (%s)",
		__entry->task_id, __entry->client_id,
>>>>>>> upstream/android-13
		__entry->pos, __entry->length,
		(unsigned long long)__entry->offset, __entry->handle,
		__entry->nents < __entry->nsegs ? "more" : "last"
	)
);

#define DEFINE_RDCH_EVENT(name)						\
<<<<<<< HEAD
		DEFINE_EVENT(xprtrdma_rdch_event, name,			\
=======
		DEFINE_EVENT(xprtrdma_rdch_event, xprtrdma_chunk_##name,\
>>>>>>> upstream/android-13
				TP_PROTO(				\
					const struct rpc_task *task,	\
					unsigned int pos,		\
					struct rpcrdma_mr *mr,		\
					int nsegs			\
				),					\
				TP_ARGS(task, pos, mr, nsegs))

DECLARE_EVENT_CLASS(xprtrdma_wrch_event,
	TP_PROTO(
		const struct rpc_task *task,
		struct rpcrdma_mr *mr,
		int nsegs
	),

	TP_ARGS(task, mr, nsegs),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
<<<<<<< HEAD
		__field(const void *, mr)
=======
>>>>>>> upstream/android-13
		__field(int, nents)
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
		__field(int, nsegs)
	),

	TP_fast_assign(
		__entry->task_id = task->tk_pid;
		__entry->client_id = task->tk_client->cl_clid;
<<<<<<< HEAD
		__entry->mr = mr;
=======
>>>>>>> upstream/android-13
		__entry->nents = mr->mr_nents;
		__entry->handle = mr->mr_handle;
		__entry->length = mr->mr_length;
		__entry->offset = mr->mr_offset;
		__entry->nsegs = nsegs;
	),

<<<<<<< HEAD
	TP_printk("task:%u@%u mr=%p %u@0x%016llx:0x%08x (%s)",
		__entry->task_id, __entry->client_id, __entry->mr,
=======
	TP_printk("task:%u@%u %u@0x%016llx:0x%08x (%s)",
		__entry->task_id, __entry->client_id,
>>>>>>> upstream/android-13
		__entry->length, (unsigned long long)__entry->offset,
		__entry->handle,
		__entry->nents < __entry->nsegs ? "more" : "last"
	)
);

#define DEFINE_WRCH_EVENT(name)						\
<<<<<<< HEAD
		DEFINE_EVENT(xprtrdma_wrch_event, name,			\
=======
		DEFINE_EVENT(xprtrdma_wrch_event, xprtrdma_chunk_##name,\
>>>>>>> upstream/android-13
				TP_PROTO(				\
					const struct rpc_task *task,	\
					struct rpcrdma_mr *mr,		\
					int nsegs			\
				),					\
				TP_ARGS(task, mr, nsegs))

<<<<<<< HEAD
TRACE_DEFINE_ENUM(FRWR_IS_INVALID);
TRACE_DEFINE_ENUM(FRWR_IS_VALID);
TRACE_DEFINE_ENUM(FRWR_FLUSHED_FR);
TRACE_DEFINE_ENUM(FRWR_FLUSHED_LI);

#define xprtrdma_show_frwr_state(x)					\
		__print_symbolic(x,					\
				{ FRWR_IS_INVALID, "INVALID" },		\
				{ FRWR_IS_VALID, "VALID" },		\
				{ FRWR_FLUSHED_FR, "FLUSHED_FR" },	\
				{ FRWR_FLUSHED_LI, "FLUSHED_LI" })

DECLARE_EVENT_CLASS(xprtrdma_frwr_done,
	TP_PROTO(
		const struct ib_wc *wc,
		const struct rpcrdma_frwr *frwr
	),

	TP_ARGS(wc, frwr),

	TP_STRUCT__entry(
		__field(const void *, mr)
		__field(unsigned int, state)
		__field(unsigned int, status)
		__field(unsigned int, vendor_err)
	),

	TP_fast_assign(
		__entry->mr = container_of(frwr, struct rpcrdma_mr, frwr);
		__entry->state = frwr->fr_state;
		__entry->status = wc->status;
		__entry->vendor_err = __entry->status ? wc->vendor_err : 0;
	),

	TP_printk(
		"mr=%p state=%s: %s (%u/0x%x)",
		__entry->mr, xprtrdma_show_frwr_state(__entry->state),
		rdma_show_wc_status(__entry->status),
		__entry->status, __entry->vendor_err
	)
);

#define DEFINE_FRWR_DONE_EVENT(name)					\
		DEFINE_EVENT(xprtrdma_frwr_done, name,			\
				TP_PROTO(				\
					const struct ib_wc *wc,		\
					const struct rpcrdma_frwr *frwr	\
				),					\
				TP_ARGS(wc, frwr))

DECLARE_EVENT_CLASS(xprtrdma_mr,
=======
TRACE_DEFINE_ENUM(DMA_BIDIRECTIONAL);
TRACE_DEFINE_ENUM(DMA_TO_DEVICE);
TRACE_DEFINE_ENUM(DMA_FROM_DEVICE);
TRACE_DEFINE_ENUM(DMA_NONE);

#define xprtrdma_show_direction(x)					\
		__print_symbolic(x,					\
				{ DMA_BIDIRECTIONAL, "BIDIR" },		\
				{ DMA_TO_DEVICE, "TO_DEVICE" },		\
				{ DMA_FROM_DEVICE, "FROM_DEVICE" },	\
				{ DMA_NONE, "NONE" })

DECLARE_EVENT_CLASS(xprtrdma_mr_class,
>>>>>>> upstream/android-13
	TP_PROTO(
		const struct rpcrdma_mr *mr
	),

	TP_ARGS(mr),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(const void *, mr)
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
	),

	TP_fast_assign(
		__entry->mr = mr;
		__entry->handle = mr->mr_handle;
		__entry->length = mr->mr_length;
		__entry->offset = mr->mr_offset;
	),

	TP_printk("mr=%p %u@0x%016llx:0x%08x",
		__entry->mr, __entry->length,
		(unsigned long long)__entry->offset,
		__entry->handle
	)
);

#define DEFINE_MR_EVENT(name) \
		DEFINE_EVENT(xprtrdma_mr, name, \
				TP_PROTO( \
					const struct rpcrdma_mr *mr \
				), \
				TP_ARGS(mr))

DECLARE_EVENT_CLASS(xprtrdma_cb_event,
	TP_PROTO(
		const struct rpc_rqst *rqst
	),

	TP_ARGS(rqst),

	TP_STRUCT__entry(
		__field(const void *, rqst)
		__field(const void *, rep)
		__field(const void *, req)
		__field(u32, xid)
	),

	TP_fast_assign(
		__entry->rqst = rqst;
		__entry->req = rpcr_to_rdmar(rqst);
		__entry->rep = rpcr_to_rdmar(rqst)->rl_reply;
		__entry->xid = be32_to_cpu(rqst->rq_xid);
	),

	TP_printk("xid=0x%08x, rqst=%p req=%p rep=%p",
		__entry->xid, __entry->rqst, __entry->req, __entry->rep
	)
);

#define DEFINE_CB_EVENT(name)						\
		DEFINE_EVENT(xprtrdma_cb_event, name,			\
				TP_PROTO(				\
					const struct rpc_rqst *rqst	\
				),					\
				TP_ARGS(rqst))
=======
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(u32, mr_id)
		__field(int, nents)
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
		__field(u32, dir)
	),

	TP_fast_assign(
		const struct rpcrdma_req *req = mr->mr_req;
		const struct rpc_task *task = req->rl_slot.rq_task;

		__entry->task_id = task->tk_pid;
		__entry->client_id = task->tk_client->cl_clid;
		__entry->mr_id  = mr->mr_ibmr->res.id;
		__entry->nents  = mr->mr_nents;
		__entry->handle = mr->mr_handle;
		__entry->length = mr->mr_length;
		__entry->offset = mr->mr_offset;
		__entry->dir    = mr->mr_dir;
	),

	TP_printk("task:%u@%u mr.id=%u nents=%d %u@0x%016llx:0x%08x (%s)",
		__entry->task_id, __entry->client_id,
		__entry->mr_id, __entry->nents, __entry->length,
		(unsigned long long)__entry->offset, __entry->handle,
		xprtrdma_show_direction(__entry->dir)
	)
);

#define DEFINE_MR_EVENT(name)						\
		DEFINE_EVENT(xprtrdma_mr_class,				\
				xprtrdma_mr_##name,			\
				TP_PROTO(				\
					const struct rpcrdma_mr *mr	\
				),					\
				TP_ARGS(mr))

DECLARE_EVENT_CLASS(xprtrdma_anonymous_mr_class,
	TP_PROTO(
		const struct rpcrdma_mr *mr
	),

	TP_ARGS(mr),

	TP_STRUCT__entry(
		__field(u32, mr_id)
		__field(int, nents)
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
		__field(u32, dir)
	),

	TP_fast_assign(
		__entry->mr_id  = mr->mr_ibmr->res.id;
		__entry->nents  = mr->mr_nents;
		__entry->handle = mr->mr_handle;
		__entry->length = mr->mr_length;
		__entry->offset = mr->mr_offset;
		__entry->dir    = mr->mr_dir;
	),

	TP_printk("mr.id=%u nents=%d %u@0x%016llx:0x%08x (%s)",
		__entry->mr_id, __entry->nents, __entry->length,
		(unsigned long long)__entry->offset, __entry->handle,
		xprtrdma_show_direction(__entry->dir)
	)
);

#define DEFINE_ANON_MR_EVENT(name)					\
		DEFINE_EVENT(xprtrdma_anonymous_mr_class,		\
				xprtrdma_mr_##name,			\
				TP_PROTO(				\
					const struct rpcrdma_mr *mr	\
				),					\
				TP_ARGS(mr))

DECLARE_EVENT_CLASS(xprtrdma_callback_class,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		const struct rpc_rqst *rqst
	),

	TP_ARGS(r_xprt, rqst),

	TP_STRUCT__entry(
		__field(u32, xid)
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
		__entry->xid = be32_to_cpu(rqst->rq_xid);
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

	TP_printk("peer=[%s]:%s xid=0x%08x",
		__get_str(addr), __get_str(port), __entry->xid
	)
);

#define DEFINE_CALLBACK_EVENT(name)					\
		DEFINE_EVENT(xprtrdma_callback_class,			\
				xprtrdma_cb_##name,			\
				TP_PROTO(				\
					const struct rpcrdma_xprt *r_xprt, \
					const struct rpc_rqst *rqst	\
				),					\
				TP_ARGS(r_xprt, rqst))
>>>>>>> upstream/android-13

/**
 ** Connection events
 **/

<<<<<<< HEAD
TRACE_EVENT(xprtrdma_conn_upcall,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		struct rdma_cm_event *event
	),

	TP_ARGS(r_xprt, event),

	TP_STRUCT__entry(
		__field(const void *, r_xprt)
		__field(unsigned int, event)
		__field(int, status)
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
		__entry->r_xprt = r_xprt;
		__entry->event = event->event;
		__entry->status = event->status;
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

	TP_printk("peer=[%s]:%s r_xprt=%p: %s (%u/%d)",
		__get_str(addr), __get_str(port),
		__entry->r_xprt, rdma_show_cm_event(__entry->event),
		__entry->event, __entry->status
	)
);

TRACE_EVENT(xprtrdma_disconnect,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		int status
	),

	TP_ARGS(r_xprt, status),

	TP_STRUCT__entry(
		__field(const void *, r_xprt)
		__field(int, status)
		__field(int, connected)
=======
TRACE_EVENT(xprtrdma_inline_thresh,
	TP_PROTO(
		const struct rpcrdma_ep *ep
	),

	TP_ARGS(ep),

	TP_STRUCT__entry(
		__field(unsigned int, inline_send)
		__field(unsigned int, inline_recv)
		__field(unsigned int, max_send)
		__field(unsigned int, max_recv)
		__array(unsigned char, srcaddr, sizeof(struct sockaddr_in6))
		__array(unsigned char, dstaddr, sizeof(struct sockaddr_in6))
	),

	TP_fast_assign(
		const struct rdma_cm_id *id = ep->re_id;

		__entry->inline_send = ep->re_inline_send;
		__entry->inline_recv = ep->re_inline_recv;
		__entry->max_send = ep->re_max_inline_send;
		__entry->max_recv = ep->re_max_inline_recv;
		memcpy(__entry->srcaddr, &id->route.addr.src_addr,
		       sizeof(struct sockaddr_in6));
		memcpy(__entry->dstaddr, &id->route.addr.dst_addr,
		       sizeof(struct sockaddr_in6));
	),

	TP_printk("%pISpc -> %pISpc neg send/recv=%u/%u, calc send/recv=%u/%u",
		__entry->srcaddr, __entry->dstaddr,
		__entry->inline_send, __entry->inline_recv,
		__entry->max_send, __entry->max_recv
	)
);

DEFINE_CONN_EVENT(connect);
DEFINE_CONN_EVENT(disconnect);

DEFINE_RXPRT_EVENT(xprtrdma_op_inject_dsc);

TRACE_EVENT(xprtrdma_op_connect,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		unsigned long delay
	),

	TP_ARGS(r_xprt, delay),

	TP_STRUCT__entry(
		__field(unsigned long, delay)
>>>>>>> upstream/android-13
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->r_xprt = r_xprt;
		__entry->status = status;
		__entry->connected = r_xprt->rx_ep.rep_connected;
=======
		__entry->delay = delay;
>>>>>>> upstream/android-13
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

<<<<<<< HEAD
	TP_printk("peer=[%s]:%s r_xprt=%p: status=%d %sconnected",
		__get_str(addr), __get_str(port),
		__entry->r_xprt, __entry->status,
		__entry->connected == 1 ? "still " : "dis"
	)
);

DEFINE_RXPRT_EVENT(xprtrdma_conn_start);
DEFINE_RXPRT_EVENT(xprtrdma_conn_tout);
DEFINE_RXPRT_EVENT(xprtrdma_create);
DEFINE_RXPRT_EVENT(xprtrdma_destroy);
DEFINE_RXPRT_EVENT(xprtrdma_remove);
DEFINE_RXPRT_EVENT(xprtrdma_reinsert);
DEFINE_RXPRT_EVENT(xprtrdma_reconnect);
DEFINE_RXPRT_EVENT(xprtrdma_inject_dsc);

TRACE_EVENT(xprtrdma_qp_error,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		const struct ib_event *event
	),

	TP_ARGS(r_xprt, event),

	TP_STRUCT__entry(
		__field(const void *, r_xprt)
		__field(unsigned int, event)
		__string(name, event->device->name)
=======
	TP_printk("peer=[%s]:%s delay=%lu",
		__get_str(addr), __get_str(port), __entry->delay
	)
);


TRACE_EVENT(xprtrdma_op_set_cto,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		unsigned long connect,
		unsigned long reconnect
	),

	TP_ARGS(r_xprt, connect, reconnect),

	TP_STRUCT__entry(
		__field(unsigned long, connect)
		__field(unsigned long, reconnect)
>>>>>>> upstream/android-13
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->r_xprt = r_xprt;
		__entry->event = event->event;
		__assign_str(name, event->device->name);
=======
		__entry->connect = connect;
		__entry->reconnect = reconnect;
>>>>>>> upstream/android-13
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

<<<<<<< HEAD
	TP_printk("peer=[%s]:%s r_xprt=%p: dev %s: %s (%u)",
		__get_str(addr), __get_str(port), __entry->r_xprt,
		__get_str(name), rdma_show_ib_event(__entry->event),
		__entry->event
=======
	TP_printk("peer=[%s]:%s connect=%lu reconnect=%lu",
		__get_str(addr), __get_str(port),
		__entry->connect / HZ, __entry->reconnect / HZ
>>>>>>> upstream/android-13
	)
);

/**
 ** Call events
 **/

TRACE_EVENT(xprtrdma_createmrs,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		unsigned int count
	),

	TP_ARGS(r_xprt, count),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(const void *, r_xprt)
=======
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
>>>>>>> upstream/android-13
		__field(unsigned int, count)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->r_xprt = r_xprt;
		__entry->count = count;
	),

	TP_printk("r_xprt=%p: created %u MRs",
		__entry->r_xprt, __entry->count
	)
);

DEFINE_RXPRT_EVENT(xprtrdma_nomrs);

DEFINE_RDCH_EVENT(xprtrdma_read_chunk);
DEFINE_WRCH_EVENT(xprtrdma_write_chunk);
DEFINE_WRCH_EVENT(xprtrdma_reply_chunk);

TRACE_DEFINE_ENUM(rpcrdma_noch);
=======
		__entry->count = count;
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

	TP_printk("peer=[%s]:%s created %u MRs",
		__get_str(addr), __get_str(port), __entry->count
	)
);

TRACE_EVENT(xprtrdma_nomrs_err,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		const struct rpcrdma_req *req
	),

	TP_ARGS(r_xprt, req),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
		const struct rpc_rqst *rqst = &req->rl_slot;

		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

	TP_printk("peer=[%s]:%s task:%u@%u",
		__get_str(addr), __get_str(port),
		__entry->task_id, __entry->client_id
	)
);

DEFINE_RDCH_EVENT(read);
DEFINE_WRCH_EVENT(write);
DEFINE_WRCH_EVENT(reply);

TRACE_DEFINE_ENUM(rpcrdma_noch);
TRACE_DEFINE_ENUM(rpcrdma_noch_pullup);
TRACE_DEFINE_ENUM(rpcrdma_noch_mapped);
>>>>>>> upstream/android-13
TRACE_DEFINE_ENUM(rpcrdma_readch);
TRACE_DEFINE_ENUM(rpcrdma_areadch);
TRACE_DEFINE_ENUM(rpcrdma_writech);
TRACE_DEFINE_ENUM(rpcrdma_replych);

#define xprtrdma_show_chunktype(x)					\
		__print_symbolic(x,					\
				{ rpcrdma_noch, "inline" },		\
<<<<<<< HEAD
=======
				{ rpcrdma_noch_pullup, "pullup" },	\
				{ rpcrdma_noch_mapped, "mapped" },	\
>>>>>>> upstream/android-13
				{ rpcrdma_readch, "read list" },	\
				{ rpcrdma_areadch, "*read list" },	\
				{ rpcrdma_writech, "write list" },	\
				{ rpcrdma_replych, "reply chunk" })

TRACE_EVENT(xprtrdma_marshal,
	TP_PROTO(
<<<<<<< HEAD
		const struct rpc_rqst *rqst,
		unsigned int hdrlen,
=======
		const struct rpcrdma_req *req,
>>>>>>> upstream/android-13
		unsigned int rtype,
		unsigned int wtype
	),

<<<<<<< HEAD
	TP_ARGS(rqst, hdrlen, rtype, wtype),
=======
	TP_ARGS(req, rtype, wtype),
>>>>>>> upstream/android-13

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(u32, xid)
		__field(unsigned int, hdrlen)
		__field(unsigned int, headlen)
		__field(unsigned int, pagelen)
		__field(unsigned int, taillen)
		__field(unsigned int, rtype)
		__field(unsigned int, wtype)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->xid = be32_to_cpu(rqst->rq_xid);
		__entry->hdrlen = hdrlen;
=======
		const struct rpc_rqst *rqst = &req->rl_slot;

		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->xid = be32_to_cpu(rqst->rq_xid);
		__entry->hdrlen = req->rl_hdrbuf.len;
>>>>>>> upstream/android-13
		__entry->headlen = rqst->rq_snd_buf.head[0].iov_len;
		__entry->pagelen = rqst->rq_snd_buf.page_len;
		__entry->taillen = rqst->rq_snd_buf.tail[0].iov_len;
		__entry->rtype = rtype;
		__entry->wtype = wtype;
	),

	TP_printk("task:%u@%u xid=0x%08x: hdr=%u xdr=%u/%u/%u %s/%s",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->hdrlen,
		__entry->headlen, __entry->pagelen, __entry->taillen,
		xprtrdma_show_chunktype(__entry->rtype),
		xprtrdma_show_chunktype(__entry->wtype)
	)
);

<<<<<<< HEAD
TRACE_EVENT(xprtrdma_post_send,
	TP_PROTO(
=======
TRACE_EVENT(xprtrdma_marshal_failed,
	TP_PROTO(const struct rpc_rqst *rqst,
		 int ret
	),

	TP_ARGS(rqst, ret),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(u32, xid)
		__field(int, ret)
	),

	TP_fast_assign(
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->xid = be32_to_cpu(rqst->rq_xid);
		__entry->ret = ret;
	),

	TP_printk("task:%u@%u xid=0x%08x: ret=%d",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->ret
	)
);

TRACE_EVENT(xprtrdma_prepsend_failed,
	TP_PROTO(const struct rpc_rqst *rqst,
		 int ret
	),

	TP_ARGS(rqst, ret),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(u32, xid)
		__field(int, ret)
	),

	TP_fast_assign(
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->xid = be32_to_cpu(rqst->rq_xid);
		__entry->ret = ret;
	),

	TP_printk("task:%u@%u xid=0x%08x: ret=%d",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->ret
	)
);

TRACE_EVENT(xprtrdma_post_send,
	TP_PROTO(
		const struct rpcrdma_req *req
	),

	TP_ARGS(req),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(int, num_sge)
		__field(int, signaled)
	),

	TP_fast_assign(
		const struct rpc_rqst *rqst = &req->rl_slot;
		const struct rpcrdma_sendctx *sc = req->rl_sendctx;

		__entry->cq_id = sc->sc_cid.ci_queue_id;
		__entry->completion_id = sc->sc_cid.ci_completion_id;
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client ?
				     rqst->rq_task->tk_client->cl_clid : -1;
		__entry->num_sge = req->rl_wr.num_sge;
		__entry->signaled = req->rl_wr.send_flags & IB_SEND_SIGNALED;
	),

	TP_printk("task:%u@%u cq.id=%u cid=%d (%d SGE%s) %s",
		__entry->task_id, __entry->client_id,
		__entry->cq_id, __entry->completion_id,
		__entry->num_sge, (__entry->num_sge == 1 ? "" : "s"),
		(__entry->signaled ? "signaled" : "")
	)
);

TRACE_EVENT(xprtrdma_post_send_err,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		const struct rpcrdma_req *req,
		int rc
	),

	TP_ARGS(r_xprt, req, rc),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(int, rc)
	),

	TP_fast_assign(
		const struct rpc_rqst *rqst = &req->rl_slot;
		const struct rpcrdma_ep *ep = r_xprt->rx_ep;

		__entry->cq_id = ep ? ep->re_attr.recv_cq->res.id : 0;
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client ?
				     rqst->rq_task->tk_client->cl_clid : -1;
		__entry->rc = rc;
	),

	TP_printk("task:%u@%u cq.id=%u rc=%d",
		__entry->task_id, __entry->client_id,
		__entry->cq_id, __entry->rc
	)
);

TRACE_EVENT(xprtrdma_post_recv,
	TP_PROTO(
		const struct rpcrdma_rep *rep
	),

	TP_ARGS(rep),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
	),

	TP_fast_assign(
		__entry->cq_id = rep->rr_cid.ci_queue_id;
		__entry->completion_id = rep->rr_cid.ci_completion_id;
	),

	TP_printk("cq.id=%d cid=%d",
		__entry->cq_id, __entry->completion_id
	)
);

TRACE_EVENT(xprtrdma_post_recvs,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		unsigned int count
	),

	TP_ARGS(r_xprt, count),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(unsigned int, count)
		__field(int, posted)
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
		const struct rpcrdma_ep *ep = r_xprt->rx_ep;

		__entry->cq_id = ep->re_attr.recv_cq->res.id;
		__entry->count = count;
		__entry->posted = ep->re_receive_count;
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

	TP_printk("peer=[%s]:%s cq.id=%d %u new recvs, %d active",
		__get_str(addr), __get_str(port), __entry->cq_id,
		__entry->count, __entry->posted
	)
);

TRACE_EVENT(xprtrdma_post_recvs_err,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		int status
	),

	TP_ARGS(r_xprt, status),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, status)
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
		const struct rpcrdma_ep *ep = r_xprt->rx_ep;

		__entry->cq_id = ep->re_attr.recv_cq->res.id;
		__entry->status = status;
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

	TP_printk("peer=[%s]:%s cq.id=%d rc=%d",
		__get_str(addr), __get_str(port), __entry->cq_id,
		__entry->status
	)
);

TRACE_EVENT(xprtrdma_post_linv_err,
	TP_PROTO(
>>>>>>> upstream/android-13
		const struct rpcrdma_req *req,
		int status
	),

	TP_ARGS(req, status),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(const void *, req)
		__field(int, num_sge)
		__field(bool, signaled)
=======
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
>>>>>>> upstream/android-13
		__field(int, status)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->req = req;
		__entry->num_sge = req->rl_sendctx->sc_wr.num_sge;
		__entry->signaled = req->rl_sendctx->sc_wr.send_flags &
				    IB_SEND_SIGNALED;
		__entry->status = status;
	),

	TP_printk("req=%p, %d SGEs%s, status=%d",
		__entry->req, __entry->num_sge,
		(__entry->signaled ? ", signaled" : ""),
		__entry->status
	)
);

TRACE_EVENT(xprtrdma_post_recv,
	TP_PROTO(
		const struct ib_cqe *cqe
	),

	TP_ARGS(cqe),

	TP_STRUCT__entry(
		__field(const void *, cqe)
	),

	TP_fast_assign(
		__entry->cqe = cqe;
	),

	TP_printk("cqe=%p",
		__entry->cqe
	)
);

TRACE_EVENT(xprtrdma_post_recvs,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		unsigned int count,
		int status
	),

	TP_ARGS(r_xprt, count, status),

	TP_STRUCT__entry(
		__field(const void *, r_xprt)
		__field(unsigned int, count)
		__field(int, status)
		__field(int, posted)
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
		__entry->r_xprt = r_xprt;
		__entry->count = count;
		__entry->status = status;
		__entry->posted = r_xprt->rx_buf.rb_posted_receives;
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

	TP_printk("peer=[%s]:%s r_xprt=%p: %u new recvs, %d active (rc %d)",
		__get_str(addr), __get_str(port), __entry->r_xprt,
		__entry->count, __entry->posted, __entry->status
=======
		const struct rpc_task *task = req->rl_slot.rq_task;

		__entry->task_id = task->tk_pid;
		__entry->client_id = task->tk_client->cl_clid;
		__entry->status = status;
	),

	TP_printk("task:%u@%u status=%d",
		__entry->task_id, __entry->client_id, __entry->status
>>>>>>> upstream/android-13
	)
);

/**
 ** Completion events
 **/

<<<<<<< HEAD
TRACE_EVENT(xprtrdma_wc_send,
	TP_PROTO(
		const struct rpcrdma_sendctx *sc,
		const struct ib_wc *wc
	),

	TP_ARGS(sc, wc),

	TP_STRUCT__entry(
		__field(const void *, req)
		__field(unsigned int, unmap_count)
		__field(unsigned int, status)
		__field(unsigned int, vendor_err)
	),

	TP_fast_assign(
		__entry->req = sc->sc_req;
		__entry->unmap_count = sc->sc_unmap_count;
		__entry->status = wc->status;
		__entry->vendor_err = __entry->status ? wc->vendor_err : 0;
	),

	TP_printk("req=%p, unmapped %u pages: %s (%u/0x%x)",
		__entry->req, __entry->unmap_count,
		rdma_show_wc_status(__entry->status),
		__entry->status, __entry->vendor_err
	)
);

TRACE_EVENT(xprtrdma_wc_receive,
	TP_PROTO(
		const struct ib_wc *wc
	),

	TP_ARGS(wc),

	TP_STRUCT__entry(
		__field(const void *, cqe)
		__field(u32, byte_len)
		__field(unsigned int, status)
		__field(u32, vendor_err)
	),

	TP_fast_assign(
		__entry->cqe = wc->wr_cqe;
		__entry->status = wc->status;
		if (wc->status) {
			__entry->byte_len = 0;
			__entry->vendor_err = wc->vendor_err;
		} else {
			__entry->byte_len = wc->byte_len;
			__entry->vendor_err = 0;
		}
	),

	TP_printk("cqe=%p %u bytes: %s (%u/0x%x)",
		__entry->cqe, __entry->byte_len,
		rdma_show_wc_status(__entry->status),
		__entry->status, __entry->vendor_err
	)
);

DEFINE_FRWR_DONE_EVENT(xprtrdma_wc_fastreg);
DEFINE_FRWR_DONE_EVENT(xprtrdma_wc_li);
DEFINE_FRWR_DONE_EVENT(xprtrdma_wc_li_wake);

DEFINE_MR_EVENT(xprtrdma_localinv);
DEFINE_MR_EVENT(xprtrdma_dma_map);
DEFINE_MR_EVENT(xprtrdma_dma_unmap);
DEFINE_MR_EVENT(xprtrdma_remoteinv);
DEFINE_MR_EVENT(xprtrdma_recover_mr);
=======
DEFINE_RECEIVE_COMPLETION_EVENT(xprtrdma_wc_receive);

DEFINE_COMPLETION_EVENT(xprtrdma_wc_send);
DEFINE_MR_COMPLETION_EVENT(xprtrdma_wc_fastreg);
DEFINE_MR_COMPLETION_EVENT(xprtrdma_wc_li);
DEFINE_MR_COMPLETION_EVENT(xprtrdma_wc_li_wake);
DEFINE_MR_COMPLETION_EVENT(xprtrdma_wc_li_done);

TRACE_EVENT(xprtrdma_frwr_alloc,
	TP_PROTO(
		const struct rpcrdma_mr *mr,
		int rc
	),

	TP_ARGS(mr, rc),

	TP_STRUCT__entry(
		__field(u32, mr_id)
		__field(int, rc)
	),

	TP_fast_assign(
		__entry->mr_id = mr->mr_ibmr->res.id;
		__entry->rc = rc;
	),

	TP_printk("mr.id=%u: rc=%d",
		__entry->mr_id, __entry->rc
	)
);

TRACE_EVENT(xprtrdma_frwr_dereg,
	TP_PROTO(
		const struct rpcrdma_mr *mr,
		int rc
	),

	TP_ARGS(mr, rc),

	TP_STRUCT__entry(
		__field(u32, mr_id)
		__field(int, nents)
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
		__field(u32, dir)
		__field(int, rc)
	),

	TP_fast_assign(
		__entry->mr_id  = mr->mr_ibmr->res.id;
		__entry->nents  = mr->mr_nents;
		__entry->handle = mr->mr_handle;
		__entry->length = mr->mr_length;
		__entry->offset = mr->mr_offset;
		__entry->dir    = mr->mr_dir;
		__entry->rc	= rc;
	),

	TP_printk("mr.id=%u nents=%d %u@0x%016llx:0x%08x (%s): rc=%d",
		__entry->mr_id, __entry->nents, __entry->length,
		(unsigned long long)__entry->offset, __entry->handle,
		xprtrdma_show_direction(__entry->dir),
		__entry->rc
	)
);

TRACE_EVENT(xprtrdma_frwr_sgerr,
	TP_PROTO(
		const struct rpcrdma_mr *mr,
		int sg_nents
	),

	TP_ARGS(mr, sg_nents),

	TP_STRUCT__entry(
		__field(u32, mr_id)
		__field(u64, addr)
		__field(u32, dir)
		__field(int, nents)
	),

	TP_fast_assign(
		__entry->mr_id = mr->mr_ibmr->res.id;
		__entry->addr = mr->mr_sg->dma_address;
		__entry->dir = mr->mr_dir;
		__entry->nents = sg_nents;
	),

	TP_printk("mr.id=%u DMA addr=0x%llx (%s) sg_nents=%d",
		__entry->mr_id, __entry->addr,
		xprtrdma_show_direction(__entry->dir),
		__entry->nents
	)
);

TRACE_EVENT(xprtrdma_frwr_maperr,
	TP_PROTO(
		const struct rpcrdma_mr *mr,
		int num_mapped
	),

	TP_ARGS(mr, num_mapped),

	TP_STRUCT__entry(
		__field(u32, mr_id)
		__field(u64, addr)
		__field(u32, dir)
		__field(int, num_mapped)
		__field(int, nents)
	),

	TP_fast_assign(
		__entry->mr_id = mr->mr_ibmr->res.id;
		__entry->addr = mr->mr_sg->dma_address;
		__entry->dir = mr->mr_dir;
		__entry->num_mapped = num_mapped;
		__entry->nents = mr->mr_nents;
	),

	TP_printk("mr.id=%u DMA addr=0x%llx (%s) nents=%d of %d",
		__entry->mr_id, __entry->addr,
		xprtrdma_show_direction(__entry->dir),
		__entry->num_mapped, __entry->nents
	)
);

DEFINE_MR_EVENT(fastreg);
DEFINE_MR_EVENT(localinv);
DEFINE_MR_EVENT(reminv);
DEFINE_MR_EVENT(map);

DEFINE_ANON_MR_EVENT(unmap);

TRACE_EVENT(xprtrdma_dma_maperr,
	TP_PROTO(
		u64 addr
	),

	TP_ARGS(addr),

	TP_STRUCT__entry(
		__field(u64, addr)
	),

	TP_fast_assign(
		__entry->addr = addr;
	),

	TP_printk("dma addr=0x%llx\n", __entry->addr)
);
>>>>>>> upstream/android-13

/**
 ** Reply events
 **/

TRACE_EVENT(xprtrdma_reply,
	TP_PROTO(
		const struct rpc_task *task,
		const struct rpcrdma_rep *rep,
<<<<<<< HEAD
		const struct rpcrdma_req *req,
		unsigned int credits
	),

	TP_ARGS(task, rep, req, credits),
=======
		unsigned int credits
	),

	TP_ARGS(task, rep, credits),
>>>>>>> upstream/android-13

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
<<<<<<< HEAD
		__field(const void *, rep)
		__field(const void *, req)
=======
>>>>>>> upstream/android-13
		__field(u32, xid)
		__field(unsigned int, credits)
	),

	TP_fast_assign(
		__entry->task_id = task->tk_pid;
		__entry->client_id = task->tk_client->cl_clid;
<<<<<<< HEAD
		__entry->rep = rep;
		__entry->req = req;
=======
>>>>>>> upstream/android-13
		__entry->xid = be32_to_cpu(rep->rr_xid);
		__entry->credits = credits;
	),

<<<<<<< HEAD
	TP_printk("task:%u@%u xid=0x%08x, %u credits, rep=%p -> req=%p",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->credits, __entry->rep, __entry->req
	)
);

TRACE_EVENT(xprtrdma_defer_cmp,
	TP_PROTO(
		const struct rpcrdma_rep *rep
	),

	TP_ARGS(rep),
=======
	TP_printk("task:%u@%u xid=0x%08x credits=%u",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->credits
	)
);

DEFINE_REPLY_EVENT(vers);
DEFINE_REPLY_EVENT(rqst);
DEFINE_REPLY_EVENT(short);
DEFINE_REPLY_EVENT(hdr);

TRACE_EVENT(xprtrdma_err_vers,
	TP_PROTO(
		const struct rpc_rqst *rqst,
		__be32 *min,
		__be32 *max
	),

	TP_ARGS(rqst, min, max),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(u32, xid)
		__field(u32, min)
		__field(u32, max)
	),

	TP_fast_assign(
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->xid = be32_to_cpu(rqst->rq_xid);
		__entry->min = be32_to_cpup(min);
		__entry->max = be32_to_cpup(max);
	),

	TP_printk("task:%u@%u xid=0x%08x versions=[%u, %u]",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->min, __entry->max
	)
);

TRACE_EVENT(xprtrdma_err_chunk,
	TP_PROTO(
		const struct rpc_rqst *rqst
	),

	TP_ARGS(rqst),
>>>>>>> upstream/android-13

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
<<<<<<< HEAD
		__field(const void *, rep)
=======
>>>>>>> upstream/android-13
		__field(u32, xid)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->task_id = rep->rr_rqst->rq_task->tk_pid;
		__entry->client_id = rep->rr_rqst->rq_task->tk_client->cl_clid;
		__entry->rep = rep;
		__entry->xid = be32_to_cpu(rep->rr_xid);
	),

	TP_printk("task:%u@%u xid=0x%08x rep=%p",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->rep
	)
);

DEFINE_REPLY_EVENT(xprtrdma_reply_vers);
DEFINE_REPLY_EVENT(xprtrdma_reply_rqst);
DEFINE_REPLY_EVENT(xprtrdma_reply_short);
DEFINE_REPLY_EVENT(xprtrdma_reply_hdr);
=======
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->xid = be32_to_cpu(rqst->rq_xid);
	),

	TP_printk("task:%u@%u xid=0x%08x",
		__entry->task_id, __entry->client_id, __entry->xid
	)
);

TRACE_EVENT(xprtrdma_err_unrecognized,
	TP_PROTO(
		const struct rpc_rqst *rqst,
		__be32 *procedure
	),

	TP_ARGS(rqst, procedure),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(u32, xid)
		__field(u32, procedure)
	),

	TP_fast_assign(
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->procedure = be32_to_cpup(procedure);
	),

	TP_printk("task:%u@%u xid=0x%08x procedure=%u",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->procedure
	)
);
>>>>>>> upstream/android-13

TRACE_EVENT(xprtrdma_fixup,
	TP_PROTO(
		const struct rpc_rqst *rqst,
<<<<<<< HEAD
		int len,
		int hdrlen
	),

	TP_ARGS(rqst, len, hdrlen),
=======
		unsigned long fixup
	),

	TP_ARGS(rqst, fixup),
>>>>>>> upstream/android-13

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
<<<<<<< HEAD
		__field(const void *, base)
		__field(int, len)
		__field(int, hdrlen)
=======
		__field(unsigned long, fixup)
		__field(size_t, headlen)
		__field(unsigned int, pagelen)
		__field(size_t, taillen)
>>>>>>> upstream/android-13
	),

	TP_fast_assign(
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
<<<<<<< HEAD
		__entry->base = rqst->rq_rcv_buf.head[0].iov_base;
		__entry->len = len;
		__entry->hdrlen = hdrlen;
	),

	TP_printk("task:%u@%u base=%p len=%d hdrlen=%d",
		__entry->task_id, __entry->client_id,
		__entry->base, __entry->len, __entry->hdrlen
	)
);

TRACE_EVENT(xprtrdma_fixup_pg,
	TP_PROTO(
		const struct rpc_rqst *rqst,
		int pageno,
		const void *pos,
		int len,
		int curlen
	),

	TP_ARGS(rqst, pageno, pos, len, curlen),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(const void *, pos)
		__field(int, pageno)
		__field(int, len)
		__field(int, curlen)
	),

	TP_fast_assign(
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->pos = pos;
		__entry->pageno = pageno;
		__entry->len = len;
		__entry->curlen = curlen;
	),

	TP_printk("task:%u@%u pageno=%d pos=%p len=%d curlen=%d",
		__entry->task_id, __entry->client_id,
		__entry->pageno, __entry->pos, __entry->len, __entry->curlen
=======
		__entry->fixup = fixup;
		__entry->headlen = rqst->rq_rcv_buf.head[0].iov_len;
		__entry->pagelen = rqst->rq_rcv_buf.page_len;
		__entry->taillen = rqst->rq_rcv_buf.tail[0].iov_len;
	),

	TP_printk("task:%u@%u fixup=%lu xdr=%zu/%u/%zu",
		__entry->task_id, __entry->client_id, __entry->fixup,
		__entry->headlen, __entry->pagelen, __entry->taillen
>>>>>>> upstream/android-13
	)
);

TRACE_EVENT(xprtrdma_decode_seg,
	TP_PROTO(
		u32 handle,
		u32 length,
		u64 offset
	),

	TP_ARGS(handle, length, offset),

	TP_STRUCT__entry(
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
	),

	TP_fast_assign(
		__entry->handle = handle;
		__entry->length = length;
		__entry->offset = offset;
	),

	TP_printk("%u@0x%016llx:0x%08x",
		__entry->length, (unsigned long long)__entry->offset,
		__entry->handle
	)
);

<<<<<<< HEAD
/**
 ** Allocation/release of rpcrdma_reqs and rpcrdma_reps
 **/

TRACE_EVENT(xprtrdma_allocate,
	TP_PROTO(
		const struct rpc_task *task,
		const struct rpcrdma_req *req
	),

	TP_ARGS(task, req),
=======
TRACE_EVENT(xprtrdma_mrs_zap,
	TP_PROTO(
		const struct rpc_task *task
	),

	TP_ARGS(task),
>>>>>>> upstream/android-13

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
<<<<<<< HEAD
		__field(const void *, req)
		__field(size_t, callsize)
		__field(size_t, rcvsize)
=======
>>>>>>> upstream/android-13
	),

	TP_fast_assign(
		__entry->task_id = task->tk_pid;
		__entry->client_id = task->tk_client->cl_clid;
<<<<<<< HEAD
		__entry->req = req;
		__entry->callsize = task->tk_rqstp->rq_callsize;
		__entry->rcvsize = task->tk_rqstp->rq_rcvsize;
	),

	TP_printk("task:%u@%u req=%p (%zu, %zu)",
		__entry->task_id, __entry->client_id,
		__entry->req, __entry->callsize, __entry->rcvsize
	)
);

TRACE_EVENT(xprtrdma_rpc_done,
	TP_PROTO(
		const struct rpc_task *task,
		const struct rpcrdma_req *req
	),

	TP_ARGS(task, req),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(const void *, req)
		__field(const void *, rep)
	),

	TP_fast_assign(
		__entry->task_id = task->tk_pid;
		__entry->client_id = task->tk_client->cl_clid;
		__entry->req = req;
		__entry->rep = req->rl_reply;
	),

	TP_printk("task:%u@%u req=%p rep=%p",
		__entry->task_id, __entry->client_id,
		__entry->req, __entry->rep
=======
	),

	TP_printk("task:%u@%u",
		__entry->task_id, __entry->client_id
>>>>>>> upstream/android-13
	)
);

/**
 ** Callback events
 **/

TRACE_EVENT(xprtrdma_cb_setup,
	TP_PROTO(
		const struct rpcrdma_xprt *r_xprt,
		unsigned int reqs
	),

	TP_ARGS(r_xprt, reqs),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(const void *, r_xprt)
=======
>>>>>>> upstream/android-13
		__field(unsigned int, reqs)
		__string(addr, rpcrdma_addrstr(r_xprt))
		__string(port, rpcrdma_portstr(r_xprt))
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->r_xprt = r_xprt;
=======
>>>>>>> upstream/android-13
		__entry->reqs = reqs;
		__assign_str(addr, rpcrdma_addrstr(r_xprt));
		__assign_str(port, rpcrdma_portstr(r_xprt));
	),

<<<<<<< HEAD
	TP_printk("peer=[%s]:%s r_xprt=%p: %u reqs",
		__get_str(addr), __get_str(port),
		__entry->r_xprt, __entry->reqs
	)
);

DEFINE_CB_EVENT(xprtrdma_cb_call);
DEFINE_CB_EVENT(xprtrdma_cb_reply);

TRACE_EVENT(xprtrdma_leaked_rep,
	TP_PROTO(
		const struct rpc_rqst *rqst,
		const struct rpcrdma_rep *rep
	),

	TP_ARGS(rqst, rep),

	TP_STRUCT__entry(
		__field(unsigned int, task_id)
		__field(unsigned int, client_id)
		__field(u32, xid)
		__field(const void *, rep)
	),

	TP_fast_assign(
		__entry->task_id = rqst->rq_task->tk_pid;
		__entry->client_id = rqst->rq_task->tk_client->cl_clid;
		__entry->xid = be32_to_cpu(rqst->rq_xid);
		__entry->rep = rep;
	),

	TP_printk("task:%u@%u xid=0x%08x rep=%p",
		__entry->task_id, __entry->client_id, __entry->xid,
		__entry->rep
	)
);
=======
	TP_printk("peer=[%s]:%s %u reqs",
		__get_str(addr), __get_str(port), __entry->reqs
	)
);

DEFINE_CALLBACK_EVENT(call);
DEFINE_CALLBACK_EVENT(reply);
>>>>>>> upstream/android-13

/**
 ** Server-side RPC/RDMA events
 **/

<<<<<<< HEAD
DECLARE_EVENT_CLASS(svcrdma_xprt_event,
	TP_PROTO(
		const struct svc_xprt *xprt
	),

	TP_ARGS(xprt),

	TP_STRUCT__entry(
		__field(const void *, xprt)
		__string(addr, xprt->xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->xprt = xprt;
		__assign_str(addr, xprt->xpt_remotebuf);
	),

	TP_printk("xprt=%p addr=%s",
		__entry->xprt, __get_str(addr)
	)
);

#define DEFINE_XPRT_EVENT(name)						\
		DEFINE_EVENT(svcrdma_xprt_event, svcrdma_xprt_##name,	\
				TP_PROTO(				\
					const struct svc_xprt *xprt	\
				),					\
				TP_ARGS(xprt))

DEFINE_XPRT_EVENT(accept);
DEFINE_XPRT_EVENT(fail);
DEFINE_XPRT_EVENT(free);
=======
DECLARE_EVENT_CLASS(svcrdma_accept_class,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		long status
	),

	TP_ARGS(rdma, status),

	TP_STRUCT__entry(
		__field(long, status)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->status = status;
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

	TP_printk("addr=%s status=%ld",
		__get_str(addr), __entry->status
	)
);

#define DEFINE_ACCEPT_EVENT(name) \
		DEFINE_EVENT(svcrdma_accept_class, svcrdma_##name##_err, \
				TP_PROTO( \
					const struct svcxprt_rdma *rdma, \
					long status \
				), \
				TP_ARGS(rdma, status))

DEFINE_ACCEPT_EVENT(pd);
DEFINE_ACCEPT_EVENT(qp);
DEFINE_ACCEPT_EVENT(fabric);
DEFINE_ACCEPT_EVENT(initdepth);
DEFINE_ACCEPT_EVENT(accept);
>>>>>>> upstream/android-13

TRACE_DEFINE_ENUM(RDMA_MSG);
TRACE_DEFINE_ENUM(RDMA_NOMSG);
TRACE_DEFINE_ENUM(RDMA_MSGP);
TRACE_DEFINE_ENUM(RDMA_DONE);
TRACE_DEFINE_ENUM(RDMA_ERROR);

#define show_rpcrdma_proc(x)						\
		__print_symbolic(x,					\
				{ RDMA_MSG, "RDMA_MSG" },		\
				{ RDMA_NOMSG, "RDMA_NOMSG" },		\
				{ RDMA_MSGP, "RDMA_MSGP" },		\
				{ RDMA_DONE, "RDMA_DONE" },		\
				{ RDMA_ERROR, "RDMA_ERROR" })

TRACE_EVENT(svcrdma_decode_rqst,
	TP_PROTO(
<<<<<<< HEAD
=======
		const struct svc_rdma_recv_ctxt *ctxt,
>>>>>>> upstream/android-13
		__be32 *p,
		unsigned int hdrlen
	),

<<<<<<< HEAD
	TP_ARGS(p, hdrlen),

	TP_STRUCT__entry(
=======
	TP_ARGS(ctxt, p, hdrlen),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
>>>>>>> upstream/android-13
		__field(u32, xid)
		__field(u32, vers)
		__field(u32, proc)
		__field(u32, credits)
		__field(unsigned int, hdrlen)
	),

	TP_fast_assign(
<<<<<<< HEAD
=======
		__entry->cq_id = ctxt->rc_cid.ci_queue_id;
		__entry->completion_id = ctxt->rc_cid.ci_completion_id;
>>>>>>> upstream/android-13
		__entry->xid = be32_to_cpup(p++);
		__entry->vers = be32_to_cpup(p++);
		__entry->credits = be32_to_cpup(p++);
		__entry->proc = be32_to_cpup(p);
		__entry->hdrlen = hdrlen;
	),

<<<<<<< HEAD
	TP_printk("xid=0x%08x vers=%u credits=%u proc=%s hdrlen=%u",
=======
	TP_printk("cq.id=%u cid=%d xid=0x%08x vers=%u credits=%u proc=%s hdrlen=%u",
		__entry->cq_id, __entry->completion_id,
>>>>>>> upstream/android-13
		__entry->xid, __entry->vers, __entry->credits,
		show_rpcrdma_proc(__entry->proc), __entry->hdrlen)
);

<<<<<<< HEAD
TRACE_EVENT(svcrdma_decode_short,
	TP_PROTO(
		unsigned int hdrlen
	),

	TP_ARGS(hdrlen),

	TP_STRUCT__entry(
=======
TRACE_EVENT(svcrdma_decode_short_err,
	TP_PROTO(
		const struct svc_rdma_recv_ctxt *ctxt,
		unsigned int hdrlen
	),

	TP_ARGS(ctxt, hdrlen),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
>>>>>>> upstream/android-13
		__field(unsigned int, hdrlen)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->hdrlen = hdrlen;
	),

	TP_printk("hdrlen=%u", __entry->hdrlen)
=======
		__entry->cq_id = ctxt->rc_cid.ci_queue_id;
		__entry->completion_id = ctxt->rc_cid.ci_completion_id;
		__entry->hdrlen = hdrlen;
	),

	TP_printk("cq.id=%u cid=%d hdrlen=%u",
		__entry->cq_id, __entry->completion_id,
		__entry->hdrlen)
>>>>>>> upstream/android-13
);

DECLARE_EVENT_CLASS(svcrdma_badreq_event,
	TP_PROTO(
<<<<<<< HEAD
		__be32 *p
	),

	TP_ARGS(p),

	TP_STRUCT__entry(
=======
		const struct svc_rdma_recv_ctxt *ctxt,
		__be32 *p
	),

	TP_ARGS(ctxt, p),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
>>>>>>> upstream/android-13
		__field(u32, xid)
		__field(u32, vers)
		__field(u32, proc)
		__field(u32, credits)
	),

	TP_fast_assign(
<<<<<<< HEAD
=======
		__entry->cq_id = ctxt->rc_cid.ci_queue_id;
		__entry->completion_id = ctxt->rc_cid.ci_completion_id;
>>>>>>> upstream/android-13
		__entry->xid = be32_to_cpup(p++);
		__entry->vers = be32_to_cpup(p++);
		__entry->credits = be32_to_cpup(p++);
		__entry->proc = be32_to_cpup(p);
	),

<<<<<<< HEAD
	TP_printk("xid=0x%08x vers=%u credits=%u proc=%u",
=======
	TP_printk("cq.id=%u cid=%d xid=0x%08x vers=%u credits=%u proc=%u",
		__entry->cq_id, __entry->completion_id,
>>>>>>> upstream/android-13
		__entry->xid, __entry->vers, __entry->credits, __entry->proc)
);

#define DEFINE_BADREQ_EVENT(name)					\
<<<<<<< HEAD
		DEFINE_EVENT(svcrdma_badreq_event, svcrdma_decode_##name,\
				TP_PROTO(				\
					__be32 *p			\
				),					\
				TP_ARGS(p))
=======
		DEFINE_EVENT(svcrdma_badreq_event,			\
			     svcrdma_decode_##name##_err,		\
				TP_PROTO(				\
					const struct svc_rdma_recv_ctxt *ctxt,	\
					__be32 *p			\
				),					\
				TP_ARGS(ctxt, p))
>>>>>>> upstream/android-13

DEFINE_BADREQ_EVENT(badvers);
DEFINE_BADREQ_EVENT(drop);
DEFINE_BADREQ_EVENT(badproc);
DEFINE_BADREQ_EVENT(parse);

<<<<<<< HEAD
DECLARE_EVENT_CLASS(svcrdma_segment_event,
	TP_PROTO(
=======
TRACE_EVENT(svcrdma_encode_wseg,
	TP_PROTO(
		const struct svc_rdma_send_ctxt *ctxt,
		u32 segno,
>>>>>>> upstream/android-13
		u32 handle,
		u32 length,
		u64 offset
	),

<<<<<<< HEAD
	TP_ARGS(handle, length, offset),

	TP_STRUCT__entry(
=======
	TP_ARGS(ctxt, segno, handle, length, offset),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
		__field(u32, segno)
>>>>>>> upstream/android-13
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
	),

	TP_fast_assign(
<<<<<<< HEAD
=======
		__entry->cq_id = ctxt->sc_cid.ci_queue_id;
		__entry->completion_id = ctxt->sc_cid.ci_completion_id;
		__entry->segno = segno;
>>>>>>> upstream/android-13
		__entry->handle = handle;
		__entry->length = length;
		__entry->offset = offset;
	),

<<<<<<< HEAD
	TP_printk("%u@0x%016llx:0x%08x",
		__entry->length, (unsigned long long)__entry->offset,
		__entry->handle
	)
);

#define DEFINE_SEGMENT_EVENT(name)					\
		DEFINE_EVENT(svcrdma_segment_event, svcrdma_encode_##name,\
				TP_PROTO(				\
					u32 handle,			\
					u32 length,			\
					u64 offset			\
				),					\
				TP_ARGS(handle, length, offset))

DEFINE_SEGMENT_EVENT(rseg);
DEFINE_SEGMENT_EVENT(wseg);

DECLARE_EVENT_CLASS(svcrdma_chunk_event,
	TP_PROTO(
		u32 length
	),

	TP_ARGS(length),

	TP_STRUCT__entry(
		__field(u32, length)
	),

	TP_fast_assign(
		__entry->length = length;
	),

	TP_printk("length=%u",
		__entry->length
	)
);

#define DEFINE_CHUNK_EVENT(name)					\
		DEFINE_EVENT(svcrdma_chunk_event, svcrdma_encode_##name,\
				TP_PROTO(				\
					u32 length			\
				),					\
				TP_ARGS(length))

DEFINE_CHUNK_EVENT(pzr);
DEFINE_CHUNK_EVENT(write);
DEFINE_CHUNK_EVENT(reply);

TRACE_EVENT(svcrdma_encode_read,
	TP_PROTO(
		u32 length,
		u32 position
	),

	TP_ARGS(length, position),

	TP_STRUCT__entry(
		__field(u32, length)
		__field(u32, position)
	),

	TP_fast_assign(
		__entry->length = length;
		__entry->position = position;
	),

	TP_printk("length=%u position=%u",
		__entry->length, __entry->position
=======
	TP_printk("cq_id=%u cid=%d segno=%u %u@0x%016llx:0x%08x",
		__entry->cq_id, __entry->completion_id,
		__entry->segno, __entry->length,
		(unsigned long long)__entry->offset, __entry->handle
	)
);

TRACE_EVENT(svcrdma_decode_rseg,
	TP_PROTO(
		const struct rpc_rdma_cid *cid,
		const struct svc_rdma_chunk *chunk,
		const struct svc_rdma_segment *segment
	),

	TP_ARGS(cid, chunk, segment),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
		__field(u32, segno)
		__field(u32, position)
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
	),

	TP_fast_assign(
		__entry->cq_id = cid->ci_queue_id;
		__entry->completion_id = cid->ci_completion_id;
		__entry->segno = chunk->ch_segcount;
		__entry->position = chunk->ch_position;
		__entry->handle = segment->rs_handle;
		__entry->length = segment->rs_length;
		__entry->offset = segment->rs_offset;
	),

	TP_printk("cq_id=%u cid=%d segno=%u position=%u %u@0x%016llx:0x%08x",
		__entry->cq_id, __entry->completion_id,
		__entry->segno, __entry->position, __entry->length,
		(unsigned long long)__entry->offset, __entry->handle
	)
);

TRACE_EVENT(svcrdma_decode_wseg,
	TP_PROTO(
		const struct rpc_rdma_cid *cid,
		const struct svc_rdma_chunk *chunk,
		u32 segno
	),

	TP_ARGS(cid, chunk, segno),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
		__field(u32, segno)
		__field(u32, handle)
		__field(u32, length)
		__field(u64, offset)
	),

	TP_fast_assign(
		const struct svc_rdma_segment *segment =
			&chunk->ch_segments[segno];

		__entry->cq_id = cid->ci_queue_id;
		__entry->completion_id = cid->ci_completion_id;
		__entry->segno = segno;
		__entry->handle = segment->rs_handle;
		__entry->length = segment->rs_length;
		__entry->offset = segment->rs_offset;
	),

	TP_printk("cq_id=%u cid=%d segno=%u %u@0x%016llx:0x%08x",
		__entry->cq_id, __entry->completion_id,
		__entry->segno, __entry->length,
		(unsigned long long)__entry->offset, __entry->handle
>>>>>>> upstream/android-13
	)
);

DECLARE_EVENT_CLASS(svcrdma_error_event,
	TP_PROTO(
		__be32 xid
	),

	TP_ARGS(xid),

	TP_STRUCT__entry(
		__field(u32, xid)
	),

	TP_fast_assign(
		__entry->xid = be32_to_cpu(xid);
	),

	TP_printk("xid=0x%08x",
		__entry->xid
	)
);

#define DEFINE_ERROR_EVENT(name)					\
		DEFINE_EVENT(svcrdma_error_event, svcrdma_err_##name,	\
				TP_PROTO(				\
					__be32 xid			\
				),					\
				TP_ARGS(xid))

DEFINE_ERROR_EVENT(vers);
DEFINE_ERROR_EVENT(chunk);

/**
 ** Server-side RDMA API events
 **/

<<<<<<< HEAD
TRACE_EVENT(svcrdma_dma_map_page,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		const void *page
	),

	TP_ARGS(rdma, page),

	TP_STRUCT__entry(
		__field(const void *, page);
=======
DECLARE_EVENT_CLASS(svcrdma_dma_map_class,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		u64 dma_addr,
		u32 length
	),

	TP_ARGS(rdma, dma_addr, length),

	TP_STRUCT__entry(
		__field(u64, dma_addr)
		__field(u32, length)
>>>>>>> upstream/android-13
		__string(device, rdma->sc_cm_id->device->name)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->page = page;
=======
		__entry->dma_addr = dma_addr;
		__entry->length = length;
>>>>>>> upstream/android-13
		__assign_str(device, rdma->sc_cm_id->device->name);
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

<<<<<<< HEAD
	TP_printk("addr=%s device=%s page=%p",
		__get_str(addr), __get_str(device), __entry->page
	)
);

TRACE_EVENT(svcrdma_dma_map_rwctx,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		int status
	),

	TP_ARGS(rdma, status),

	TP_STRUCT__entry(
		__field(int, status)
=======
	TP_printk("addr=%s device=%s dma_addr=%llu length=%u",
		__get_str(addr), __get_str(device),
		__entry->dma_addr, __entry->length
	)
);

#define DEFINE_SVC_DMA_EVENT(name)					\
		DEFINE_EVENT(svcrdma_dma_map_class, svcrdma_##name,	\
				TP_PROTO(				\
					const struct svcxprt_rdma *rdma,\
					u64 dma_addr,			\
					u32 length			\
				),					\
				TP_ARGS(rdma, dma_addr, length))

DEFINE_SVC_DMA_EVENT(dma_map_page);
DEFINE_SVC_DMA_EVENT(dma_map_err);
DEFINE_SVC_DMA_EVENT(dma_unmap_page);

TRACE_EVENT(svcrdma_dma_map_rw_err,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		unsigned int nents,
		int status
	),

	TP_ARGS(rdma, nents, status),

	TP_STRUCT__entry(
		__field(int, status)
		__field(unsigned int, nents)
>>>>>>> upstream/android-13
		__string(device, rdma->sc_cm_id->device->name)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->status = status;
<<<<<<< HEAD
=======
		__entry->nents = nents;
>>>>>>> upstream/android-13
		__assign_str(device, rdma->sc_cm_id->device->name);
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

<<<<<<< HEAD
	TP_printk("addr=%s device=%s status=%d",
		__get_str(addr), __get_str(device), __entry->status
	)
);

TRACE_EVENT(svcrdma_send_failed,
=======
	TP_printk("addr=%s device=%s nents=%u status=%d",
		__get_str(addr), __get_str(device), __entry->nents,
		__entry->status
	)
);

TRACE_EVENT(svcrdma_no_rwctx_err,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		unsigned int num_sges
	),

	TP_ARGS(rdma, num_sges),

	TP_STRUCT__entry(
		__field(unsigned int, num_sges)
		__string(device, rdma->sc_cm_id->device->name)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->num_sges = num_sges;
		__assign_str(device, rdma->sc_cm_id->device->name);
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

	TP_printk("addr=%s device=%s num_sges=%d",
		__get_str(addr), __get_str(device), __entry->num_sges
	)
);

TRACE_EVENT(svcrdma_page_overrun_err,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		const struct svc_rqst *rqst,
		unsigned int pageno
	),

	TP_ARGS(rdma, rqst, pageno),

	TP_STRUCT__entry(
		__field(unsigned int, pageno)
		__field(u32, xid)
		__string(device, rdma->sc_cm_id->device->name)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->pageno = pageno;
		__entry->xid = __be32_to_cpu(rqst->rq_xid);
		__assign_str(device, rdma->sc_cm_id->device->name);
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

	TP_printk("addr=%s device=%s xid=0x%08x pageno=%u", __get_str(addr),
		__get_str(device), __entry->xid, __entry->pageno
	)
);

TRACE_EVENT(svcrdma_small_wrch_err,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		unsigned int remaining,
		unsigned int seg_no,
		unsigned int num_segs
	),

	TP_ARGS(rdma, remaining, seg_no, num_segs),

	TP_STRUCT__entry(
		__field(unsigned int, remaining)
		__field(unsigned int, seg_no)
		__field(unsigned int, num_segs)
		__string(device, rdma->sc_cm_id->device->name)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->remaining = remaining;
		__entry->seg_no = seg_no;
		__entry->num_segs = num_segs;
		__assign_str(device, rdma->sc_cm_id->device->name);
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

	TP_printk("addr=%s device=%s remaining=%u seg_no=%u num_segs=%u",
		__get_str(addr), __get_str(device), __entry->remaining,
		__entry->seg_no, __entry->num_segs
	)
);

TRACE_EVENT(svcrdma_send_pullup,
	TP_PROTO(
		const struct svc_rdma_send_ctxt *ctxt,
		unsigned int msglen
	),

	TP_ARGS(ctxt, msglen),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
		__field(unsigned int, hdrlen)
		__field(unsigned int, msglen)
	),

	TP_fast_assign(
		__entry->cq_id = ctxt->sc_cid.ci_queue_id;
		__entry->completion_id = ctxt->sc_cid.ci_completion_id;
		__entry->hdrlen = ctxt->sc_hdrbuf.len,
		__entry->msglen = msglen;
	),

	TP_printk("cq_id=%u cid=%d hdr=%u msg=%u (total %u)",
		__entry->cq_id, __entry->completion_id,
		__entry->hdrlen, __entry->msglen,
		__entry->hdrlen + __entry->msglen)
);

TRACE_EVENT(svcrdma_send_err,
>>>>>>> upstream/android-13
	TP_PROTO(
		const struct svc_rqst *rqst,
		int status
	),

	TP_ARGS(rqst, status),

	TP_STRUCT__entry(
		__field(int, status)
		__field(u32, xid)
<<<<<<< HEAD
		__field(const void *, xprt)
=======
>>>>>>> upstream/android-13
		__string(addr, rqst->rq_xprt->xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->status = status;
		__entry->xid = __be32_to_cpu(rqst->rq_xid);
<<<<<<< HEAD
		__entry->xprt = rqst->rq_xprt;
		__assign_str(addr, rqst->rq_xprt->xpt_remotebuf);
	),

	TP_printk("xprt=%p addr=%s xid=0x%08x status=%d",
		__entry->xprt, __get_str(addr),
=======
		__assign_str(addr, rqst->rq_xprt->xpt_remotebuf);
	),

	TP_printk("addr=%s xid=0x%08x status=%d", __get_str(addr),
>>>>>>> upstream/android-13
		__entry->xid, __entry->status
	)
);

<<<<<<< HEAD
DECLARE_EVENT_CLASS(svcrdma_sendcomp_event,
	TP_PROTO(
		const struct ib_wc *wc
	),

	TP_ARGS(wc),

	TP_STRUCT__entry(
		__field(const void *, cqe)
		__field(unsigned int, status)
		__field(unsigned int, vendor_err)
	),

	TP_fast_assign(
		__entry->cqe = wc->wr_cqe;
		__entry->status = wc->status;
		if (wc->status)
			__entry->vendor_err = wc->vendor_err;
		else
			__entry->vendor_err = 0;
	),

	TP_printk("cqe=%p status=%s (%u/0x%x)",
		__entry->cqe, rdma_show_wc_status(__entry->status),
		__entry->status, __entry->vendor_err
	)
);

#define DEFINE_SENDCOMP_EVENT(name)					\
		DEFINE_EVENT(svcrdma_sendcomp_event, svcrdma_wc_##name,	\
				TP_PROTO(				\
					const struct ib_wc *wc		\
				),					\
				TP_ARGS(wc))

TRACE_EVENT(svcrdma_post_send,
	TP_PROTO(
		const struct ib_send_wr *wr
	),

	TP_ARGS(wr),

	TP_STRUCT__entry(
		__field(const void *, cqe)
=======
TRACE_EVENT(svcrdma_post_send,
	TP_PROTO(
		const struct svc_rdma_send_ctxt *ctxt
	),

	TP_ARGS(ctxt),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
>>>>>>> upstream/android-13
		__field(unsigned int, num_sge)
		__field(u32, inv_rkey)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->cqe = wr->wr_cqe;
=======
		const struct ib_send_wr *wr = &ctxt->sc_send_wr;

		__entry->cq_id = ctxt->sc_cid.ci_queue_id;
		__entry->completion_id = ctxt->sc_cid.ci_completion_id;
>>>>>>> upstream/android-13
		__entry->num_sge = wr->num_sge;
		__entry->inv_rkey = (wr->opcode == IB_WR_SEND_WITH_INV) ?
					wr->ex.invalidate_rkey : 0;
	),

<<<<<<< HEAD
	TP_printk("cqe=%p num_sge=%u inv_rkey=0x%08x",
		__entry->cqe, __entry->num_sge,
		__entry->inv_rkey
	)
);

DEFINE_SENDCOMP_EVENT(send);

TRACE_EVENT(svcrdma_post_recv,
	TP_PROTO(
		const struct ib_recv_wr *wr,
		int status
	),

	TP_ARGS(wr, status),

	TP_STRUCT__entry(
		__field(const void *, cqe)
		__field(int, status)
	),

	TP_fast_assign(
		__entry->cqe = wr->wr_cqe;
		__entry->status = status;
	),

	TP_printk("cqe=%p status=%d",
		__entry->cqe, __entry->status
	)
);

TRACE_EVENT(svcrdma_wc_receive,
	TP_PROTO(
		const struct ib_wc *wc
	),

	TP_ARGS(wc),

	TP_STRUCT__entry(
		__field(const void *, cqe)
		__field(u32, byte_len)
		__field(unsigned int, status)
		__field(u32, vendor_err)
	),

	TP_fast_assign(
		__entry->cqe = wc->wr_cqe;
		__entry->status = wc->status;
		if (wc->status) {
			__entry->byte_len = 0;
			__entry->vendor_err = wc->vendor_err;
		} else {
			__entry->byte_len = wc->byte_len;
			__entry->vendor_err = 0;
		}
	),

	TP_printk("cqe=%p byte_len=%u status=%s (%u/0x%x)",
		__entry->cqe, __entry->byte_len,
		rdma_show_wc_status(__entry->status),
		__entry->status, __entry->vendor_err
	)
);

TRACE_EVENT(svcrdma_post_rw,
	TP_PROTO(
		const void *cqe,
		int sqecount
	),

	TP_ARGS(cqe, sqecount),

	TP_STRUCT__entry(
		__field(const void *, cqe)
=======
	TP_printk("cq_id=%u cid=%d num_sge=%u inv_rkey=0x%08x",
		__entry->cq_id, __entry->completion_id,
		__entry->num_sge, __entry->inv_rkey
	)
);

DEFINE_COMPLETION_EVENT(svcrdma_wc_send);

TRACE_EVENT(svcrdma_post_recv,
	TP_PROTO(
		const struct svc_rdma_recv_ctxt *ctxt
	),

	TP_ARGS(ctxt),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
	),

	TP_fast_assign(
		__entry->cq_id = ctxt->rc_cid.ci_queue_id;
		__entry->completion_id = ctxt->rc_cid.ci_completion_id;
	),

	TP_printk("cq.id=%d cid=%d",
		__entry->cq_id, __entry->completion_id
	)
);

DEFINE_RECEIVE_COMPLETION_EVENT(svcrdma_wc_receive);

TRACE_EVENT(svcrdma_rq_post_err,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		int status
	),

	TP_ARGS(rdma, status),

	TP_STRUCT__entry(
		__field(int, status)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->status = status;
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

	TP_printk("addr=%s status=%d",
		__get_str(addr), __entry->status
	)
);

DECLARE_EVENT_CLASS(svcrdma_post_chunk_class,
	TP_PROTO(
		const struct rpc_rdma_cid *cid,
		int sqecount
	),

	TP_ARGS(cid, sqecount),

	TP_STRUCT__entry(
		__field(u32, cq_id)
		__field(int, completion_id)
>>>>>>> upstream/android-13
		__field(int, sqecount)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->cqe = cqe;
		__entry->sqecount = sqecount;
	),

	TP_printk("cqe=%p sqecount=%d",
		__entry->cqe, __entry->sqecount
	)
);

DEFINE_SENDCOMP_EVENT(read);
DEFINE_SENDCOMP_EVENT(write);

TRACE_EVENT(svcrdma_cm_event,
	TP_PROTO(
		const struct rdma_cm_event *event,
		const struct sockaddr *sap
	),

	TP_ARGS(event, sap),

	TP_STRUCT__entry(
		__field(unsigned int, event)
		__field(int, status)
		__array(__u8, addr, INET6_ADDRSTRLEN + 10)
	),

	TP_fast_assign(
		__entry->event = event->event;
		__entry->status = event->status;
		snprintf(__entry->addr, sizeof(__entry->addr) - 1,
			 "%pISpc", sap);
	),

	TP_printk("addr=%s event=%s (%u/%d)",
		__entry->addr,
		rdma_show_cm_event(__entry->event),
		__entry->event, __entry->status
	)
);
=======
		__entry->cq_id = cid->ci_queue_id;
		__entry->completion_id = cid->ci_completion_id;
		__entry->sqecount = sqecount;
	),

	TP_printk("cq.id=%u cid=%d sqecount=%d",
		__entry->cq_id, __entry->completion_id,
		__entry->sqecount
	)
);

#define DEFINE_POST_CHUNK_EVENT(name)					\
		DEFINE_EVENT(svcrdma_post_chunk_class,			\
				svcrdma_post_##name##_chunk,		\
				TP_PROTO(				\
					const struct rpc_rdma_cid *cid,	\
					int sqecount			\
				),					\
				TP_ARGS(cid, sqecount))

DEFINE_POST_CHUNK_EVENT(read);
DEFINE_POST_CHUNK_EVENT(write);
DEFINE_POST_CHUNK_EVENT(reply);

DEFINE_COMPLETION_EVENT(svcrdma_wc_read);
DEFINE_COMPLETION_EVENT(svcrdma_wc_write);
>>>>>>> upstream/android-13

TRACE_EVENT(svcrdma_qp_error,
	TP_PROTO(
		const struct ib_event *event,
		const struct sockaddr *sap
	),

	TP_ARGS(event, sap),

	TP_STRUCT__entry(
		__field(unsigned int, event)
		__string(device, event->device->name)
		__array(__u8, addr, INET6_ADDRSTRLEN + 10)
	),

	TP_fast_assign(
		__entry->event = event->event;
		__assign_str(device, event->device->name);
		snprintf(__entry->addr, sizeof(__entry->addr) - 1,
			 "%pISpc", sap);
	),

	TP_printk("addr=%s dev=%s event=%s (%u)",
		__entry->addr, __get_str(device),
		rdma_show_ib_event(__entry->event), __entry->event
	)
);

DECLARE_EVENT_CLASS(svcrdma_sendqueue_event,
	TP_PROTO(
		const struct svcxprt_rdma *rdma
	),

	TP_ARGS(rdma),

	TP_STRUCT__entry(
		__field(int, avail)
		__field(int, depth)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->avail = atomic_read(&rdma->sc_sq_avail);
		__entry->depth = rdma->sc_sq_depth;
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

	TP_printk("addr=%s sc_sq_avail=%d/%d",
		__get_str(addr), __entry->avail, __entry->depth
	)
);

#define DEFINE_SQ_EVENT(name)						\
		DEFINE_EVENT(svcrdma_sendqueue_event, svcrdma_sq_##name,\
				TP_PROTO(				\
					const struct svcxprt_rdma *rdma \
				),					\
				TP_ARGS(rdma))

DEFINE_SQ_EVENT(full);
DEFINE_SQ_EVENT(retry);

TRACE_EVENT(svcrdma_sq_post_err,
	TP_PROTO(
		const struct svcxprt_rdma *rdma,
		int status
	),

	TP_ARGS(rdma, status),

	TP_STRUCT__entry(
		__field(int, avail)
		__field(int, depth)
		__field(int, status)
		__string(addr, rdma->sc_xprt.xpt_remotebuf)
	),

	TP_fast_assign(
		__entry->avail = atomic_read(&rdma->sc_sq_avail);
		__entry->depth = rdma->sc_sq_depth;
		__entry->status = status;
		__assign_str(addr, rdma->sc_xprt.xpt_remotebuf);
	),

	TP_printk("addr=%s sc_sq_avail=%d/%d status=%d",
		__get_str(addr), __entry->avail, __entry->depth,
		__entry->status
	)
);

#endif /* _TRACE_RPCRDMA_H */

#include <trace/define_trace.h>
