// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * Copyright (c) 2015-2018 Oracle. All rights reserved.
 * Copyright (c) 2014 Open Grid Computing, Inc. All rights reserved.
 * Copyright (c) 2005-2007 Network Appliance, Inc. All rights reserved.
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

#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/export.h>

#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
#include <rdma/rw.h>

#include <linux/sunrpc/addr.h>
#include <linux/sunrpc/debug.h>
<<<<<<< HEAD
#include <linux/sunrpc/rpc_rdma.h>
=======
>>>>>>> upstream/android-13
#include <linux/sunrpc/svc_xprt.h>
#include <linux/sunrpc/svc_rdma.h>

#include "xprt_rdma.h"
#include <trace/events/rpcrdma.h>

#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

static struct svcxprt_rdma *svc_rdma_create_xprt(struct svc_serv *serv,
						 struct net *net);
static struct svc_xprt *svc_rdma_create(struct svc_serv *serv,
					struct net *net,
					struct sockaddr *sa, int salen,
					int flags);
static struct svc_xprt *svc_rdma_accept(struct svc_xprt *xprt);
static void svc_rdma_detach(struct svc_xprt *xprt);
static void svc_rdma_free(struct svc_xprt *xprt);
static int svc_rdma_has_wspace(struct svc_xprt *xprt);
static void svc_rdma_secure_port(struct svc_rqst *);
static void svc_rdma_kill_temp_xprt(struct svc_xprt *);

static const struct svc_xprt_ops svc_rdma_ops = {
	.xpo_create = svc_rdma_create,
	.xpo_recvfrom = svc_rdma_recvfrom,
	.xpo_sendto = svc_rdma_sendto,
<<<<<<< HEAD
	.xpo_release_rqst = svc_rdma_release_rqst,
	.xpo_detach = svc_rdma_detach,
	.xpo_free = svc_rdma_free,
	.xpo_prep_reply_hdr = svc_rdma_prep_reply_hdr,
=======
	.xpo_result_payload = svc_rdma_result_payload,
	.xpo_release_rqst = svc_rdma_release_rqst,
	.xpo_detach = svc_rdma_detach,
	.xpo_free = svc_rdma_free,
>>>>>>> upstream/android-13
	.xpo_has_wspace = svc_rdma_has_wspace,
	.xpo_accept = svc_rdma_accept,
	.xpo_secure_port = svc_rdma_secure_port,
	.xpo_kill_temp_xprt = svc_rdma_kill_temp_xprt,
};

struct svc_xprt_class svc_rdma_class = {
	.xcl_name = "rdma",
	.xcl_owner = THIS_MODULE,
	.xcl_ops = &svc_rdma_ops,
	.xcl_max_payload = RPCSVC_MAXPAYLOAD_RDMA,
	.xcl_ident = XPRT_TRANSPORT_RDMA,
};

<<<<<<< HEAD
#if defined(CONFIG_SUNRPC_BACKCHANNEL)
static struct svc_xprt *svc_rdma_bc_create(struct svc_serv *, struct net *,
					   struct sockaddr *, int, int);
static void svc_rdma_bc_detach(struct svc_xprt *);
static void svc_rdma_bc_free(struct svc_xprt *);

static const struct svc_xprt_ops svc_rdma_bc_ops = {
	.xpo_create = svc_rdma_bc_create,
	.xpo_detach = svc_rdma_bc_detach,
	.xpo_free = svc_rdma_bc_free,
	.xpo_prep_reply_hdr = svc_rdma_prep_reply_hdr,
	.xpo_secure_port = svc_rdma_secure_port,
};

struct svc_xprt_class svc_rdma_bc_class = {
	.xcl_name = "rdma-bc",
	.xcl_owner = THIS_MODULE,
	.xcl_ops = &svc_rdma_bc_ops,
	.xcl_max_payload = (1024 - RPCRDMA_HDRLEN_MIN)
};

static struct svc_xprt *svc_rdma_bc_create(struct svc_serv *serv,
					   struct net *net,
					   struct sockaddr *sa, int salen,
					   int flags)
{
	struct svcxprt_rdma *cma_xprt;
	struct svc_xprt *xprt;

	cma_xprt = svc_rdma_create_xprt(serv, net);
	if (!cma_xprt)
		return ERR_PTR(-ENOMEM);
	xprt = &cma_xprt->sc_xprt;

	svc_xprt_init(net, &svc_rdma_bc_class, xprt, serv);
	set_bit(XPT_CONG_CTRL, &xprt->xpt_flags);
	serv->sv_bc_xprt = xprt;

	dprintk("svcrdma: %s(%p)\n", __func__, xprt);
	return xprt;
}

static void svc_rdma_bc_detach(struct svc_xprt *xprt)
{
	dprintk("svcrdma: %s(%p)\n", __func__, xprt);
}

static void svc_rdma_bc_free(struct svc_xprt *xprt)
{
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);

	dprintk("svcrdma: %s(%p)\n", __func__, xprt);
	if (xprt)
		kfree(rdma);
}
#endif	/* CONFIG_SUNRPC_BACKCHANNEL */

=======
>>>>>>> upstream/android-13
/* QP event handler */
static void qp_event_handler(struct ib_event *event, void *context)
{
	struct svc_xprt *xprt = context;

	trace_svcrdma_qp_error(event, (struct sockaddr *)&xprt->xpt_remote);
	switch (event->event) {
	/* These are considered benign events */
	case IB_EVENT_PATH_MIG:
	case IB_EVENT_COMM_EST:
	case IB_EVENT_SQ_DRAINED:
	case IB_EVENT_QP_LAST_WQE_REACHED:
		break;

	/* These are considered fatal events */
	case IB_EVENT_PATH_MIG_ERR:
	case IB_EVENT_QP_FATAL:
	case IB_EVENT_QP_REQ_ERR:
	case IB_EVENT_QP_ACCESS_ERR:
	case IB_EVENT_DEVICE_FATAL:
	default:
<<<<<<< HEAD
		set_bit(XPT_CLOSE, &xprt->xpt_flags);
		svc_xprt_enqueue(xprt);
=======
		svc_xprt_deferred_close(xprt);
>>>>>>> upstream/android-13
		break;
	}
}

static struct svcxprt_rdma *svc_rdma_create_xprt(struct svc_serv *serv,
						 struct net *net)
{
	struct svcxprt_rdma *cma_xprt = kzalloc(sizeof *cma_xprt, GFP_KERNEL);

	if (!cma_xprt) {
		dprintk("svcrdma: failed to create new transport\n");
		return NULL;
	}
	svc_xprt_init(net, &svc_rdma_class, &cma_xprt->sc_xprt, serv);
	INIT_LIST_HEAD(&cma_xprt->sc_accept_q);
	INIT_LIST_HEAD(&cma_xprt->sc_rq_dto_q);
<<<<<<< HEAD
	INIT_LIST_HEAD(&cma_xprt->sc_read_complete_q);
	INIT_LIST_HEAD(&cma_xprt->sc_send_ctxts);
	INIT_LIST_HEAD(&cma_xprt->sc_recv_ctxts);
	INIT_LIST_HEAD(&cma_xprt->sc_rw_ctxts);
=======
	init_llist_head(&cma_xprt->sc_send_ctxts);
	init_llist_head(&cma_xprt->sc_recv_ctxts);
	init_llist_head(&cma_xprt->sc_rw_ctxts);
>>>>>>> upstream/android-13
	init_waitqueue_head(&cma_xprt->sc_send_wait);

	spin_lock_init(&cma_xprt->sc_lock);
	spin_lock_init(&cma_xprt->sc_rq_dto_lock);
	spin_lock_init(&cma_xprt->sc_send_lock);
<<<<<<< HEAD
	spin_lock_init(&cma_xprt->sc_recv_lock);
=======
>>>>>>> upstream/android-13
	spin_lock_init(&cma_xprt->sc_rw_ctxt_lock);

	/*
	 * Note that this implies that the underlying transport support
	 * has some form of congestion control (see RFC 7530 section 3.1
	 * paragraph 2). For now, we assume that all supported RDMA
	 * transports are suitable here.
	 */
	set_bit(XPT_CONG_CTRL, &cma_xprt->sc_xprt.xpt_flags);

	return cma_xprt;
}

static void
svc_rdma_parse_connect_private(struct svcxprt_rdma *newxprt,
			       struct rdma_conn_param *param)
{
	const struct rpcrdma_connect_private *pmsg = param->private_data;

	if (pmsg &&
	    pmsg->cp_magic == rpcrdma_cmp_magic &&
	    pmsg->cp_version == RPCRDMA_CMP_VERSION) {
		newxprt->sc_snd_w_inv = pmsg->cp_flags &
					RPCRDMA_CMP_F_SND_W_INV_OK;

		dprintk("svcrdma: client send_size %u, recv_size %u "
			"remote inv %ssupported\n",
			rpcrdma_decode_buffer_size(pmsg->cp_send_size),
			rpcrdma_decode_buffer_size(pmsg->cp_recv_size),
			newxprt->sc_snd_w_inv ? "" : "un");
	}
}

/*
 * This function handles the CONNECT_REQUEST event on a listening
 * endpoint. It is passed the cma_id for the _new_ connection. The context in
 * this cma_id is inherited from the listening cma_id and is the svc_xprt
 * structure for the listening endpoint.
 *
 * This function creates a new xprt for the new connection and enqueues it on
 * the accept queue for the listent xprt. When the listen thread is kicked, it
 * will call the recvfrom method on the listen xprt which will accept the new
 * connection.
 */
static void handle_connect_req(struct rdma_cm_id *new_cma_id,
			       struct rdma_conn_param *param)
{
	struct svcxprt_rdma *listen_xprt = new_cma_id->context;
	struct svcxprt_rdma *newxprt;
	struct sockaddr *sa;

	/* Create a new transport */
	newxprt = svc_rdma_create_xprt(listen_xprt->sc_xprt.xpt_server,
				       listen_xprt->sc_xprt.xpt_net);
	if (!newxprt)
		return;
	newxprt->sc_cm_id = new_cma_id;
	new_cma_id->context = newxprt;
	svc_rdma_parse_connect_private(newxprt, param);

	/* Save client advertised inbound read limit for use later in accept. */
	newxprt->sc_ord = param->initiator_depth;

	sa = (struct sockaddr *)&newxprt->sc_cm_id->route.addr.dst_addr;
<<<<<<< HEAD
	svc_xprt_set_remote(&newxprt->sc_xprt, sa, svc_addr_len(sa));
=======
	newxprt->sc_xprt.xpt_remotelen = svc_addr_len(sa);
	memcpy(&newxprt->sc_xprt.xpt_remote, sa,
	       newxprt->sc_xprt.xpt_remotelen);
	snprintf(newxprt->sc_xprt.xpt_remotebuf,
		 sizeof(newxprt->sc_xprt.xpt_remotebuf) - 1, "%pISc", sa);

>>>>>>> upstream/android-13
	/* The remote port is arbitrary and not under the control of the
	 * client ULP. Set it to a fixed value so that the DRC continues
	 * to be effective after a reconnect.
	 */
	rpc_set_port((struct sockaddr *)&newxprt->sc_xprt.xpt_remote, 0);

	sa = (struct sockaddr *)&newxprt->sc_cm_id->route.addr.src_addr;
	svc_xprt_set_local(&newxprt->sc_xprt, sa, svc_addr_len(sa));

	/*
	 * Enqueue the new transport on the accept queue of the listening
	 * transport
	 */
<<<<<<< HEAD
	spin_lock_bh(&listen_xprt->sc_lock);
	list_add_tail(&newxprt->sc_accept_q, &listen_xprt->sc_accept_q);
	spin_unlock_bh(&listen_xprt->sc_lock);
=======
	spin_lock(&listen_xprt->sc_lock);
	list_add_tail(&newxprt->sc_accept_q, &listen_xprt->sc_accept_q);
	spin_unlock(&listen_xprt->sc_lock);
>>>>>>> upstream/android-13

	set_bit(XPT_CONN, &listen_xprt->sc_xprt.xpt_flags);
	svc_xprt_enqueue(&listen_xprt->sc_xprt);
}

<<<<<<< HEAD
/*
 * Handles events generated on the listening endpoint. These events will be
 * either be incoming connect requests or adapter removal  events.
 */
static int rdma_listen_handler(struct rdma_cm_id *cma_id,
			       struct rdma_cm_event *event)
{
	struct sockaddr *sap = (struct sockaddr *)&cma_id->route.addr.src_addr;

	trace_svcrdma_cm_event(event, sap);

	switch (event->event) {
	case RDMA_CM_EVENT_CONNECT_REQUEST:
		dprintk("svcrdma: Connect request on cma_id=%p, xprt = %p, "
			"event = %s (%d)\n", cma_id, cma_id->context,
			rdma_event_msg(event->event), event->event);
		handle_connect_req(cma_id, &event->param.conn);
		break;
	default:
		/* NB: No device removal upcall for INADDR_ANY listeners */
		dprintk("svcrdma: Unexpected event on listening endpoint %p, "
			"event = %s (%d)\n", cma_id,
			rdma_event_msg(event->event), event->event);
		break;
	}

	return 0;
}

static int rdma_cma_handler(struct rdma_cm_id *cma_id,
			    struct rdma_cm_event *event)
{
	struct sockaddr *sap = (struct sockaddr *)&cma_id->route.addr.dst_addr;
	struct svcxprt_rdma *rdma = cma_id->context;
	struct svc_xprt *xprt = &rdma->sc_xprt;

	trace_svcrdma_cm_event(event, sap);

	switch (event->event) {
	case RDMA_CM_EVENT_ESTABLISHED:
		/* Accept complete */
		svc_xprt_get(xprt);
		dprintk("svcrdma: Connection completed on DTO xprt=%p, "
			"cm_id=%p\n", xprt, cma_id);
		clear_bit(RDMAXPRT_CONN_PENDING, &rdma->sc_flags);
		svc_xprt_enqueue(xprt);
		break;
	case RDMA_CM_EVENT_DISCONNECTED:
		dprintk("svcrdma: Disconnect on DTO xprt=%p, cm_id=%p\n",
			xprt, cma_id);
		set_bit(XPT_CLOSE, &xprt->xpt_flags);
		svc_xprt_enqueue(xprt);
		svc_xprt_put(xprt);
		break;
	case RDMA_CM_EVENT_DEVICE_REMOVAL:
		dprintk("svcrdma: Device removal cma_id=%p, xprt = %p, "
			"event = %s (%d)\n", cma_id, xprt,
			rdma_event_msg(event->event), event->event);
		set_bit(XPT_CLOSE, &xprt->xpt_flags);
		svc_xprt_enqueue(xprt);
		svc_xprt_put(xprt);
		break;
	default:
		dprintk("svcrdma: Unexpected event on DTO endpoint %p, "
			"event = %s (%d)\n", cma_id,
			rdma_event_msg(event->event), event->event);
=======
/**
 * svc_rdma_listen_handler - Handle CM events generated on a listening endpoint
 * @cma_id: the server's listener rdma_cm_id
 * @event: details of the event
 *
 * Return values:
 *     %0: Do not destroy @cma_id
 *     %1: Destroy @cma_id (never returned here)
 *
 * NB: There is never a DEVICE_REMOVAL event for INADDR_ANY listeners.
 */
static int svc_rdma_listen_handler(struct rdma_cm_id *cma_id,
				   struct rdma_cm_event *event)
{
	switch (event->event) {
	case RDMA_CM_EVENT_CONNECT_REQUEST:
		handle_connect_req(cma_id, &event->param.conn);
		break;
	default:
		break;
	}
	return 0;
}

/**
 * svc_rdma_cma_handler - Handle CM events on client connections
 * @cma_id: the server's listener rdma_cm_id
 * @event: details of the event
 *
 * Return values:
 *     %0: Do not destroy @cma_id
 *     %1: Destroy @cma_id (never returned here)
 */
static int svc_rdma_cma_handler(struct rdma_cm_id *cma_id,
				struct rdma_cm_event *event)
{
	struct svcxprt_rdma *rdma = cma_id->context;
	struct svc_xprt *xprt = &rdma->sc_xprt;

	switch (event->event) {
	case RDMA_CM_EVENT_ESTABLISHED:
		clear_bit(RDMAXPRT_CONN_PENDING, &rdma->sc_flags);

		/* Handle any requests that were received while
		 * CONN_PENDING was set. */
		svc_xprt_enqueue(xprt);
		break;
	case RDMA_CM_EVENT_DISCONNECTED:
	case RDMA_CM_EVENT_DEVICE_REMOVAL:
		svc_xprt_deferred_close(xprt);
		break;
	default:
>>>>>>> upstream/android-13
		break;
	}
	return 0;
}

/*
 * Create a listening RDMA service endpoint.
 */
static struct svc_xprt *svc_rdma_create(struct svc_serv *serv,
					struct net *net,
					struct sockaddr *sa, int salen,
					int flags)
{
	struct rdma_cm_id *listen_id;
	struct svcxprt_rdma *cma_xprt;
	int ret;

<<<<<<< HEAD
	dprintk("svcrdma: Creating RDMA listener\n");
	if ((sa->sa_family != AF_INET) && (sa->sa_family != AF_INET6)) {
		dprintk("svcrdma: Address family %d is not supported.\n", sa->sa_family);
		return ERR_PTR(-EAFNOSUPPORT);
	}
=======
	if (sa->sa_family != AF_INET && sa->sa_family != AF_INET6)
		return ERR_PTR(-EAFNOSUPPORT);
>>>>>>> upstream/android-13
	cma_xprt = svc_rdma_create_xprt(serv, net);
	if (!cma_xprt)
		return ERR_PTR(-ENOMEM);
	set_bit(XPT_LISTENER, &cma_xprt->sc_xprt.xpt_flags);
	strcpy(cma_xprt->sc_xprt.xpt_remotebuf, "listener");

<<<<<<< HEAD
	listen_id = rdma_create_id(net, rdma_listen_handler, cma_xprt,
				   RDMA_PS_TCP, IB_QPT_RC);
	if (IS_ERR(listen_id)) {
		ret = PTR_ERR(listen_id);
		dprintk("svcrdma: rdma_create_id failed = %d\n", ret);
=======
	listen_id = rdma_create_id(net, svc_rdma_listen_handler, cma_xprt,
				   RDMA_PS_TCP, IB_QPT_RC);
	if (IS_ERR(listen_id)) {
		ret = PTR_ERR(listen_id);
>>>>>>> upstream/android-13
		goto err0;
	}

	/* Allow both IPv4 and IPv6 sockets to bind a single port
	 * at the same time.
	 */
#if IS_ENABLED(CONFIG_IPV6)
	ret = rdma_set_afonly(listen_id, 1);
<<<<<<< HEAD
	if (ret) {
		dprintk("svcrdma: rdma_set_afonly failed = %d\n", ret);
		goto err1;
	}
#endif
	ret = rdma_bind_addr(listen_id, sa);
	if (ret) {
		dprintk("svcrdma: rdma_bind_addr failed = %d\n", ret);
		goto err1;
	}
	cma_xprt->sc_cm_id = listen_id;

	ret = rdma_listen(listen_id, RPCRDMA_LISTEN_BACKLOG);
	if (ret) {
		dprintk("svcrdma: rdma_listen failed = %d\n", ret);
		goto err1;
	}
=======
	if (ret)
		goto err1;
#endif
	ret = rdma_bind_addr(listen_id, sa);
	if (ret)
		goto err1;
	cma_xprt->sc_cm_id = listen_id;

	ret = rdma_listen(listen_id, RPCRDMA_LISTEN_BACKLOG);
	if (ret)
		goto err1;
>>>>>>> upstream/android-13

	/*
	 * We need to use the address from the cm_id in case the
	 * caller specified 0 for the port number.
	 */
	sa = (struct sockaddr *)&cma_xprt->sc_cm_id->route.addr.src_addr;
	svc_xprt_set_local(&cma_xprt->sc_xprt, sa, salen);

	return &cma_xprt->sc_xprt;

 err1:
	rdma_destroy_id(listen_id);
 err0:
	kfree(cma_xprt);
	return ERR_PTR(ret);
}

/*
 * This is the xpo_recvfrom function for listening endpoints. Its
 * purpose is to accept incoming connections. The CMA callback handler
 * has already created a new transport and attached it to the new CMA
 * ID.
 *
 * There is a queue of pending connections hung on the listening
 * transport. This queue contains the new svc_xprt structure. This
 * function takes svc_xprt structures off the accept_q and completes
 * the connection.
 */
static struct svc_xprt *svc_rdma_accept(struct svc_xprt *xprt)
{
	struct svcxprt_rdma *listen_rdma;
	struct svcxprt_rdma *newxprt = NULL;
	struct rdma_conn_param conn_param;
	struct rpcrdma_connect_private pmsg;
	struct ib_qp_init_attr qp_attr;
	unsigned int ctxts, rq_depth;
	struct ib_device *dev;
<<<<<<< HEAD
	struct sockaddr *sap;
	int ret = 0;
=======
	int ret = 0;
	RPC_IFDEBUG(struct sockaddr *sap);
>>>>>>> upstream/android-13

	listen_rdma = container_of(xprt, struct svcxprt_rdma, sc_xprt);
	clear_bit(XPT_CONN, &xprt->xpt_flags);
	/* Get the next entry off the accept list */
<<<<<<< HEAD
	spin_lock_bh(&listen_rdma->sc_lock);
=======
	spin_lock(&listen_rdma->sc_lock);
>>>>>>> upstream/android-13
	if (!list_empty(&listen_rdma->sc_accept_q)) {
		newxprt = list_entry(listen_rdma->sc_accept_q.next,
				     struct svcxprt_rdma, sc_accept_q);
		list_del_init(&newxprt->sc_accept_q);
	}
	if (!list_empty(&listen_rdma->sc_accept_q))
		set_bit(XPT_CONN, &listen_rdma->sc_xprt.xpt_flags);
<<<<<<< HEAD
	spin_unlock_bh(&listen_rdma->sc_lock);
	if (!newxprt)
		return NULL;

	dprintk("svcrdma: newxprt from accept queue = %p, cm_id=%p\n",
		newxprt, newxprt->sc_cm_id);

=======
	spin_unlock(&listen_rdma->sc_lock);
	if (!newxprt)
		return NULL;

>>>>>>> upstream/android-13
	dev = newxprt->sc_cm_id->device;
	newxprt->sc_port_num = newxprt->sc_cm_id->port_num;

	/* Qualify the transport resource defaults with the
	 * capabilities of this particular device */
	/* Transport header, head iovec, tail iovec */
	newxprt->sc_max_send_sges = 3;
	/* Add one SGE per page list entry */
	newxprt->sc_max_send_sges += (svcrdma_max_req_size / PAGE_SIZE) + 1;
	if (newxprt->sc_max_send_sges > dev->attrs.max_send_sge)
		newxprt->sc_max_send_sges = dev->attrs.max_send_sge;
	newxprt->sc_max_req_size = svcrdma_max_req_size;
	newxprt->sc_max_requests = svcrdma_max_requests;
	newxprt->sc_max_bc_requests = svcrdma_max_bc_requests;
<<<<<<< HEAD
	rq_depth = newxprt->sc_max_requests + newxprt->sc_max_bc_requests;
=======
	newxprt->sc_recv_batch = RPCRDMA_MAX_RECV_BATCH;
	rq_depth = newxprt->sc_max_requests + newxprt->sc_max_bc_requests +
		   newxprt->sc_recv_batch;
>>>>>>> upstream/android-13
	if (rq_depth > dev->attrs.max_qp_wr) {
		pr_warn("svcrdma: reducing receive depth to %d\n",
			dev->attrs.max_qp_wr);
		rq_depth = dev->attrs.max_qp_wr;
<<<<<<< HEAD
=======
		newxprt->sc_recv_batch = 1;
>>>>>>> upstream/android-13
		newxprt->sc_max_requests = rq_depth - 2;
		newxprt->sc_max_bc_requests = 2;
	}
	newxprt->sc_fc_credits = cpu_to_be32(newxprt->sc_max_requests);
	ctxts = rdma_rw_mr_factor(dev, newxprt->sc_port_num, RPCSVC_MAXPAGES);
	ctxts *= newxprt->sc_max_requests;
	newxprt->sc_sq_depth = rq_depth + ctxts;
	if (newxprt->sc_sq_depth > dev->attrs.max_qp_wr) {
		pr_warn("svcrdma: reducing send depth to %d\n",
			dev->attrs.max_qp_wr);
		newxprt->sc_sq_depth = dev->attrs.max_qp_wr;
	}
	atomic_set(&newxprt->sc_sq_avail, newxprt->sc_sq_depth);

	newxprt->sc_pd = ib_alloc_pd(dev, 0);
	if (IS_ERR(newxprt->sc_pd)) {
<<<<<<< HEAD
		dprintk("svcrdma: error creating PD for connect request\n");
		goto errout;
	}
	newxprt->sc_sq_cq = ib_alloc_cq(dev, newxprt, newxprt->sc_sq_depth,
					0, IB_POLL_WORKQUEUE);
	if (IS_ERR(newxprt->sc_sq_cq)) {
		dprintk("svcrdma: error creating SQ CQ for connect request\n");
		goto errout;
	}
	newxprt->sc_rq_cq = ib_alloc_cq(dev, newxprt, rq_depth,
					0, IB_POLL_WORKQUEUE);
	if (IS_ERR(newxprt->sc_rq_cq)) {
		dprintk("svcrdma: error creating RQ CQ for connect request\n");
		goto errout;
	}
=======
		trace_svcrdma_pd_err(newxprt, PTR_ERR(newxprt->sc_pd));
		goto errout;
	}
	newxprt->sc_sq_cq = ib_alloc_cq_any(dev, newxprt, newxprt->sc_sq_depth,
					    IB_POLL_WORKQUEUE);
	if (IS_ERR(newxprt->sc_sq_cq))
		goto errout;
	newxprt->sc_rq_cq =
		ib_alloc_cq_any(dev, newxprt, rq_depth, IB_POLL_WORKQUEUE);
	if (IS_ERR(newxprt->sc_rq_cq))
		goto errout;
>>>>>>> upstream/android-13

	memset(&qp_attr, 0, sizeof qp_attr);
	qp_attr.event_handler = qp_event_handler;
	qp_attr.qp_context = &newxprt->sc_xprt;
	qp_attr.port_num = newxprt->sc_port_num;
	qp_attr.cap.max_rdma_ctxs = ctxts;
	qp_attr.cap.max_send_wr = newxprt->sc_sq_depth - ctxts;
	qp_attr.cap.max_recv_wr = rq_depth;
	qp_attr.cap.max_send_sge = newxprt->sc_max_send_sges;
	qp_attr.cap.max_recv_sge = 1;
	qp_attr.sq_sig_type = IB_SIGNAL_REQ_WR;
	qp_attr.qp_type = IB_QPT_RC;
	qp_attr.send_cq = newxprt->sc_sq_cq;
	qp_attr.recv_cq = newxprt->sc_rq_cq;
	dprintk("svcrdma: newxprt->sc_cm_id=%p, newxprt->sc_pd=%p\n",
		newxprt->sc_cm_id, newxprt->sc_pd);
	dprintk("    cap.max_send_wr = %d, cap.max_recv_wr = %d\n",
		qp_attr.cap.max_send_wr, qp_attr.cap.max_recv_wr);
	dprintk("    cap.max_send_sge = %d, cap.max_recv_sge = %d\n",
		qp_attr.cap.max_send_sge, qp_attr.cap.max_recv_sge);

	ret = rdma_create_qp(newxprt->sc_cm_id, newxprt->sc_pd, &qp_attr);
	if (ret) {
<<<<<<< HEAD
		dprintk("svcrdma: failed to create QP, ret=%d\n", ret);
=======
		trace_svcrdma_qp_err(newxprt, ret);
>>>>>>> upstream/android-13
		goto errout;
	}
	newxprt->sc_qp = newxprt->sc_cm_id->qp;

	if (!(dev->attrs.device_cap_flags & IB_DEVICE_MEM_MGT_EXTENSIONS))
		newxprt->sc_snd_w_inv = false;
	if (!rdma_protocol_iwarp(dev, newxprt->sc_port_num) &&
<<<<<<< HEAD
	    !rdma_ib_or_roce(dev, newxprt->sc_port_num))
		goto errout;
=======
	    !rdma_ib_or_roce(dev, newxprt->sc_port_num)) {
		trace_svcrdma_fabric_err(newxprt, -EINVAL);
		goto errout;
	}
>>>>>>> upstream/android-13

	if (!svc_rdma_post_recvs(newxprt))
		goto errout;

<<<<<<< HEAD
	/* Swap out the handler */
	newxprt->sc_cm_id->event_handler = rdma_cma_handler;

=======
>>>>>>> upstream/android-13
	/* Construct RDMA-CM private message */
	pmsg.cp_magic = rpcrdma_cmp_magic;
	pmsg.cp_version = RPCRDMA_CMP_VERSION;
	pmsg.cp_flags = 0;
	pmsg.cp_send_size = pmsg.cp_recv_size =
		rpcrdma_encode_buffer_size(newxprt->sc_max_req_size);

	/* Accept Connection */
	set_bit(RDMAXPRT_CONN_PENDING, &newxprt->sc_flags);
	memset(&conn_param, 0, sizeof conn_param);
	conn_param.responder_resources = 0;
	conn_param.initiator_depth = min_t(int, newxprt->sc_ord,
					   dev->attrs.max_qp_init_rd_atom);
	if (!conn_param.initiator_depth) {
<<<<<<< HEAD
		dprintk("svcrdma: invalid ORD setting\n");
		ret = -EINVAL;
=======
		ret = -EINVAL;
		trace_svcrdma_initdepth_err(newxprt, ret);
>>>>>>> upstream/android-13
		goto errout;
	}
	conn_param.private_data = &pmsg;
	conn_param.private_data_len = sizeof(pmsg);
<<<<<<< HEAD
	ret = rdma_accept(newxprt->sc_cm_id, &conn_param);
	if (ret)
		goto errout;

=======
	rdma_lock_handler(newxprt->sc_cm_id);
	newxprt->sc_cm_id->event_handler = svc_rdma_cma_handler;
	ret = rdma_accept(newxprt->sc_cm_id, &conn_param);
	rdma_unlock_handler(newxprt->sc_cm_id);
	if (ret) {
		trace_svcrdma_accept_err(newxprt, ret);
		goto errout;
	}

#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
>>>>>>> upstream/android-13
	dprintk("svcrdma: new connection %p accepted:\n", newxprt);
	sap = (struct sockaddr *)&newxprt->sc_cm_id->route.addr.src_addr;
	dprintk("    local address   : %pIS:%u\n", sap, rpc_get_port(sap));
	sap = (struct sockaddr *)&newxprt->sc_cm_id->route.addr.dst_addr;
	dprintk("    remote address  : %pIS:%u\n", sap, rpc_get_port(sap));
	dprintk("    max_sge         : %d\n", newxprt->sc_max_send_sges);
	dprintk("    sq_depth        : %d\n", newxprt->sc_sq_depth);
	dprintk("    rdma_rw_ctxs    : %d\n", ctxts);
	dprintk("    max_requests    : %d\n", newxprt->sc_max_requests);
	dprintk("    ord             : %d\n", conn_param.initiator_depth);
<<<<<<< HEAD

	trace_svcrdma_xprt_accept(&newxprt->sc_xprt);
	return &newxprt->sc_xprt;

 errout:
	dprintk("svcrdma: failure accepting new connection rc=%d.\n", ret);
	trace_svcrdma_xprt_fail(&newxprt->sc_xprt);
=======
#endif

	return &newxprt->sc_xprt;

 errout:
>>>>>>> upstream/android-13
	/* Take a reference in case the DTO handler runs */
	svc_xprt_get(&newxprt->sc_xprt);
	if (newxprt->sc_qp && !IS_ERR(newxprt->sc_qp))
		ib_destroy_qp(newxprt->sc_qp);
	rdma_destroy_id(newxprt->sc_cm_id);
	/* This call to put will destroy the transport */
	svc_xprt_put(&newxprt->sc_xprt);
	return NULL;
}

<<<<<<< HEAD
/*
 * When connected, an svc_xprt has at least two references:
 *
 * - A reference held by the cm_id between the ESTABLISHED and
 *   DISCONNECTED events. If the remote peer disconnected first, this
 *   reference could be gone.
 *
 * - A reference held by the svc_recv code that called this function
 *   as part of close processing.
 *
 * At a minimum one references should still be held.
 */
=======
>>>>>>> upstream/android-13
static void svc_rdma_detach(struct svc_xprt *xprt)
{
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);

<<<<<<< HEAD
	/* Disconnect and flush posted WQE */
=======
>>>>>>> upstream/android-13
	rdma_disconnect(rdma->sc_cm_id);
}

static void __svc_rdma_free(struct work_struct *work)
{
	struct svcxprt_rdma *rdma =
		container_of(work, struct svcxprt_rdma, sc_work);
<<<<<<< HEAD
	struct svc_xprt *xprt = &rdma->sc_xprt;

	trace_svcrdma_xprt_free(xprt);

	if (rdma->sc_qp && !IS_ERR(rdma->sc_qp))
		ib_drain_qp(rdma->sc_qp);

	/* We should only be called from kref_put */
	if (kref_read(&xprt->xpt_ref) != 0)
		pr_err("svcrdma: sc_xprt still in use? (%d)\n",
		       kref_read(&xprt->xpt_ref));

	svc_rdma_flush_recv_queues(rdma);

	/* Final put of backchannel client transport */
	if (xprt->xpt_bc_xprt) {
		xprt_put(xprt->xpt_bc_xprt);
		xprt->xpt_bc_xprt = NULL;
	}

=======

	/* This blocks until the Completion Queues are empty */
	if (rdma->sc_qp && !IS_ERR(rdma->sc_qp))
		ib_drain_qp(rdma->sc_qp);

	svc_rdma_flush_recv_queues(rdma);

>>>>>>> upstream/android-13
	svc_rdma_destroy_rw_ctxts(rdma);
	svc_rdma_send_ctxts_destroy(rdma);
	svc_rdma_recv_ctxts_destroy(rdma);

	/* Destroy the QP if present (not a listener) */
	if (rdma->sc_qp && !IS_ERR(rdma->sc_qp))
		ib_destroy_qp(rdma->sc_qp);

	if (rdma->sc_sq_cq && !IS_ERR(rdma->sc_sq_cq))
		ib_free_cq(rdma->sc_sq_cq);

	if (rdma->sc_rq_cq && !IS_ERR(rdma->sc_rq_cq))
		ib_free_cq(rdma->sc_rq_cq);

	if (rdma->sc_pd && !IS_ERR(rdma->sc_pd))
		ib_dealloc_pd(rdma->sc_pd);

	/* Destroy the CM ID */
	rdma_destroy_id(rdma->sc_cm_id);

	kfree(rdma);
}

static void svc_rdma_free(struct svc_xprt *xprt)
{
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);
<<<<<<< HEAD
	INIT_WORK(&rdma->sc_work, __svc_rdma_free);
	queue_work(svc_rdma_wq, &rdma->sc_work);
=======

	INIT_WORK(&rdma->sc_work, __svc_rdma_free);
	schedule_work(&rdma->sc_work);
>>>>>>> upstream/android-13
}

static int svc_rdma_has_wspace(struct svc_xprt *xprt)
{
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);

	/*
	 * If there are already waiters on the SQ,
	 * return false.
	 */
	if (waitqueue_active(&rdma->sc_send_wait))
		return 0;

	/* Otherwise return true. */
	return 1;
}

static void svc_rdma_secure_port(struct svc_rqst *rqstp)
{
	set_bit(RQ_SECURE, &rqstp->rq_flags);
}

static void svc_rdma_kill_temp_xprt(struct svc_xprt *xprt)
{
}
