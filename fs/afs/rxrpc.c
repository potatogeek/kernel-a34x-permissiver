<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* Maintain an RxRPC server socket to do AFS communications through
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/slab.h>
#include <linux/sched/signal.h>

#include <net/sock.h>
#include <net/af_rxrpc.h>
#include "internal.h"
#include "afs_cm.h"
<<<<<<< HEAD
=======
#include "protocol_yfs.h"
>>>>>>> upstream/android-13

struct workqueue_struct *afs_async_calls;

static void afs_wake_up_call_waiter(struct sock *, struct rxrpc_call *, unsigned long);
<<<<<<< HEAD
static long afs_wait_for_call_to_complete(struct afs_call *, struct afs_addr_cursor *);
=======
>>>>>>> upstream/android-13
static void afs_wake_up_async_call(struct sock *, struct rxrpc_call *, unsigned long);
static void afs_process_async_call(struct work_struct *);
static void afs_rx_new_call(struct sock *, struct rxrpc_call *, unsigned long);
static void afs_rx_discard_new_call(struct rxrpc_call *, unsigned long);
static int afs_deliver_cm_op_id(struct afs_call *);

/* asynchronous incoming call initial processing */
static const struct afs_call_type afs_RXCMxxxx = {
	.name		= "CB.xxxx",
	.deliver	= afs_deliver_cm_op_id,
};

/*
 * open an RxRPC socket and bind it to be a server for callback notifications
 * - the socket is left in blocking mode and non-blocking ops use MSG_DONTWAIT
 */
int afs_open_socket(struct afs_net *net)
{
	struct sockaddr_rxrpc srx;
	struct socket *socket;
<<<<<<< HEAD
	unsigned int min_level;
=======
>>>>>>> upstream/android-13
	int ret;

	_enter("");

	ret = sock_create_kern(net->net, AF_RXRPC, SOCK_DGRAM, PF_INET6, &socket);
	if (ret < 0)
		goto error_1;

	socket->sk->sk_allocation = GFP_NOFS;

	/* bind the callback manager's address to make this a server socket */
	memset(&srx, 0, sizeof(srx));
	srx.srx_family			= AF_RXRPC;
	srx.srx_service			= CM_SERVICE;
	srx.transport_type		= SOCK_DGRAM;
	srx.transport_len		= sizeof(srx.transport.sin6);
	srx.transport.sin6.sin6_family	= AF_INET6;
	srx.transport.sin6.sin6_port	= htons(AFS_CM_PORT);

<<<<<<< HEAD
	min_level = RXRPC_SECURITY_ENCRYPT;
	ret = kernel_setsockopt(socket, SOL_RXRPC, RXRPC_MIN_SECURITY_LEVEL,
				(void *)&min_level, sizeof(min_level));
=======
	ret = rxrpc_sock_set_min_security_level(socket->sk,
						RXRPC_SECURITY_ENCRYPT);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto error_2;

	ret = kernel_bind(socket, (struct sockaddr *) &srx, sizeof(srx));
	if (ret == -EADDRINUSE) {
		srx.transport.sin6.sin6_port = 0;
		ret = kernel_bind(socket, (struct sockaddr *) &srx, sizeof(srx));
	}
	if (ret < 0)
		goto error_2;

<<<<<<< HEAD
=======
	srx.srx_service = YFS_CM_SERVICE;
	ret = kernel_bind(socket, (struct sockaddr *) &srx, sizeof(srx));
	if (ret < 0)
		goto error_2;

	/* Ideally, we'd turn on service upgrade here, but we can't because
	 * OpenAFS is buggy and leaks the userStatus field from packet to
	 * packet and between FS packets and CB packets - so if we try to do an
	 * upgrade on an FS packet, OpenAFS will leak that into the CB packet
	 * it sends back to us.
	 */

>>>>>>> upstream/android-13
	rxrpc_kernel_new_call_notification(socket, afs_rx_new_call,
					   afs_rx_discard_new_call);

	ret = kernel_listen(socket, INT_MAX);
	if (ret < 0)
		goto error_2;

	net->socket = socket;
	afs_charge_preallocation(&net->charge_preallocation_work);
	_leave(" = 0");
	return 0;

error_2:
	sock_release(socket);
error_1:
	_leave(" = %d", ret);
	return ret;
}

/*
 * close the RxRPC socket AFS was using
 */
void afs_close_socket(struct afs_net *net)
{
	_enter("");

	kernel_listen(net->socket, 0);
	flush_workqueue(afs_async_calls);

	if (net->spare_incoming_call) {
		afs_put_call(net->spare_incoming_call);
		net->spare_incoming_call = NULL;
	}

	_debug("outstanding %u", atomic_read(&net->nr_outstanding_calls));
	wait_var_event(&net->nr_outstanding_calls,
		       !atomic_read(&net->nr_outstanding_calls));
	_debug("no outstanding calls");

	kernel_sock_shutdown(net->socket, SHUT_RDWR);
	flush_workqueue(afs_async_calls);
	sock_release(net->socket);

	_debug("dework");
	_leave("");
}

/*
 * Allocate a call.
 */
static struct afs_call *afs_alloc_call(struct afs_net *net,
				       const struct afs_call_type *type,
				       gfp_t gfp)
{
	struct afs_call *call;
	int o;

	call = kzalloc(sizeof(*call), gfp);
	if (!call)
		return NULL;

	call->type = type;
	call->net = net;
	call->debug_id = atomic_inc_return(&rxrpc_debug_id);
	atomic_set(&call->usage, 1);
	INIT_WORK(&call->async_work, afs_process_async_call);
	init_waitqueue_head(&call->waitq);
	spin_lock_init(&call->state_lock);
<<<<<<< HEAD
=======
	call->iter = &call->def_iter;
>>>>>>> upstream/android-13

	o = atomic_inc_return(&net->nr_outstanding_calls);
	trace_afs_call(call, afs_call_trace_alloc, 1, o,
		       __builtin_return_address(0));
	return call;
}

/*
 * Dispose of a reference on a call.
 */
void afs_put_call(struct afs_call *call)
{
	struct afs_net *net = call->net;
	int n = atomic_dec_return(&call->usage);
	int o = atomic_read(&net->nr_outstanding_calls);

	trace_afs_call(call, afs_call_trace_put, n, o,
		       __builtin_return_address(0));

	ASSERTCMP(n, >=, 0);
	if (n == 0) {
		ASSERT(!work_pending(&call->async_work));
		ASSERT(call->type->name != NULL);

		if (call->rxcall) {
			rxrpc_kernel_end_call(net->socket, call->rxcall);
			call->rxcall = NULL;
		}
		if (call->type->destructor)
			call->type->destructor(call);

<<<<<<< HEAD
		afs_put_server(call->net, call->cm_server);
		afs_put_cb_interest(call->net, call->cbi);
=======
		afs_unuse_server_notime(call->net, call->server, afs_server_trace_put_call);
		afs_put_addrlist(call->alist);
>>>>>>> upstream/android-13
		kfree(call->request);

		trace_afs_call(call, afs_call_trace_free, 0, o,
			       __builtin_return_address(0));
		kfree(call);

		o = atomic_dec_return(&net->nr_outstanding_calls);
		if (o == 0)
			wake_up_var(&net->nr_outstanding_calls);
	}
}

<<<<<<< HEAD
/*
 * Queue the call for actual work.  Returns 0 unconditionally for convenience.
 */
int afs_queue_call_work(struct afs_call *call)
{
	int u = atomic_inc_return(&call->usage);

	trace_afs_call(call, afs_call_trace_work, u,
		       atomic_read(&call->net->nr_outstanding_calls),
		       __builtin_return_address(0));

	INIT_WORK(&call->work, call->type->work);

	if (!queue_work(afs_wq, &call->work))
		afs_put_call(call);
	return 0;
=======
static struct afs_call *afs_get_call(struct afs_call *call,
				     enum afs_call_trace why)
{
	int u = atomic_inc_return(&call->usage);

	trace_afs_call(call, why, u,
		       atomic_read(&call->net->nr_outstanding_calls),
		       __builtin_return_address(0));
	return call;
}

/*
 * Queue the call for actual work.
 */
static void afs_queue_call_work(struct afs_call *call)
{
	if (call->type->work) {
		INIT_WORK(&call->work, call->type->work);

		afs_get_call(call, afs_call_trace_work);
		if (!queue_work(afs_wq, &call->work))
			afs_put_call(call);
	}
>>>>>>> upstream/android-13
}

/*
 * allocate a call with flat request and reply buffers
 */
struct afs_call *afs_alloc_flat_call(struct afs_net *net,
				     const struct afs_call_type *type,
				     size_t request_size, size_t reply_max)
{
	struct afs_call *call;

	call = afs_alloc_call(net, type, GFP_NOFS);
	if (!call)
		goto nomem_call;

	if (request_size) {
		call->request_size = request_size;
		call->request = kmalloc(request_size, GFP_NOFS);
		if (!call->request)
			goto nomem_free;
	}

	if (reply_max) {
		call->reply_max = reply_max;
		call->buffer = kmalloc(reply_max, GFP_NOFS);
		if (!call->buffer)
			goto nomem_free;
	}

<<<<<<< HEAD
=======
	afs_extract_to_buf(call, call->reply_max);
>>>>>>> upstream/android-13
	call->operation_ID = type->op;
	init_waitqueue_head(&call->waitq);
	return call;

nomem_free:
	afs_put_call(call);
nomem_call:
	return NULL;
}

/*
 * clean up a call with flat buffer
 */
void afs_flat_call_destructor(struct afs_call *call)
{
	_enter("");

	kfree(call->request);
	call->request = NULL;
	kfree(call->buffer);
	call->buffer = NULL;
}

<<<<<<< HEAD
#define AFS_BVEC_MAX 8

/*
 * Load the given bvec with the next few pages.
 */
static void afs_load_bvec(struct afs_call *call, struct msghdr *msg,
			  struct bio_vec *bv, pgoff_t first, pgoff_t last,
			  unsigned offset)
{
	struct page *pages[AFS_BVEC_MAX];
	unsigned int nr, n, i, to, bytes = 0;

	nr = min_t(pgoff_t, last - first + 1, AFS_BVEC_MAX);
	n = find_get_pages_contig(call->mapping, first, nr, pages);
	ASSERTCMP(n, ==, nr);

	msg->msg_flags |= MSG_MORE;
	for (i = 0; i < nr; i++) {
		to = PAGE_SIZE;
		if (first + i >= last) {
			to = call->last_to;
			msg->msg_flags &= ~MSG_MORE;
		}
		bv[i].bv_page = pages[i];
		bv[i].bv_len = to - offset;
		bv[i].bv_offset = offset;
		bytes += to - offset;
		offset = 0;
	}

	iov_iter_bvec(&msg->msg_iter, WRITE | ITER_BVEC, bv, nr, bytes);
}

=======
>>>>>>> upstream/android-13
/*
 * Advance the AFS call state when the RxRPC call ends the transmit phase.
 */
static void afs_notify_end_request_tx(struct sock *sock,
				      struct rxrpc_call *rxcall,
				      unsigned long call_user_ID)
{
	struct afs_call *call = (struct afs_call *)call_user_ID;

	afs_set_call_state(call, AFS_CALL_CL_REQUESTING, AFS_CALL_CL_AWAIT_REPLY);
}

/*
<<<<<<< HEAD
 * attach the data from a bunch of pages on an inode to a call
 */
static int afs_send_pages(struct afs_call *call, struct msghdr *msg)
{
	struct bio_vec bv[AFS_BVEC_MAX];
	unsigned int bytes, nr, loop, offset;
	pgoff_t first = call->first, last = call->last;
	int ret;

	offset = call->first_offset;
	call->first_offset = 0;

	do {
		afs_load_bvec(call, msg, bv, first, last, offset);
		trace_afs_send_pages(call, msg, first, last, offset);

		offset = 0;
		bytes = msg->msg_iter.count;
		nr = msg->msg_iter.nr_segs;

		ret = rxrpc_kernel_send_data(call->net->socket, call->rxcall, msg,
					     bytes, afs_notify_end_request_tx);
		for (loop = 0; loop < nr; loop++)
			put_page(bv[loop].bv_page);
		if (ret < 0)
			break;

		first += nr;
	} while (first <= last);

	trace_afs_sent_pages(call, call->first, last, first, ret);
	return ret;
}

/*
 * initiate a call
 */
long afs_make_call(struct afs_addr_cursor *ac, struct afs_call *call,
		   gfp_t gfp, bool async)
{
	struct sockaddr_rxrpc *srx = ac->addr;
	struct rxrpc_call *rxcall;
	struct msghdr msg;
	struct kvec iov[1];
=======
 * Initiate a call and synchronously queue up the parameters for dispatch.  Any
 * error is stored into the call struct, which the caller must check for.
 */
void afs_make_call(struct afs_addr_cursor *ac, struct afs_call *call, gfp_t gfp)
{
	struct sockaddr_rxrpc *srx = &ac->alist->addrs[ac->index];
	struct rxrpc_call *rxcall;
	struct msghdr msg;
	struct kvec iov[1];
	size_t len;
>>>>>>> upstream/android-13
	s64 tx_total_len;
	int ret;

	_enter(",{%pISp},", &srx->transport);

	ASSERT(call->type != NULL);
	ASSERT(call->type->name != NULL);

	_debug("____MAKE %p{%s,%x} [%d]____",
	       call, call->type->name, key_serial(call->key),
	       atomic_read(&call->net->nr_outstanding_calls));

<<<<<<< HEAD
	call->async = async;
=======
	call->addr_ix = ac->index;
	call->alist = afs_get_addrlist(ac->alist);
>>>>>>> upstream/android-13

	/* Work out the length we're going to transmit.  This is awkward for
	 * calls such as FS.StoreData where there's an extra injection of data
	 * after the initial fixed part.
	 */
	tx_total_len = call->request_size;
<<<<<<< HEAD
	if (call->send_pages) {
		if (call->last == call->first) {
			tx_total_len += call->last_to - call->first_offset;
		} else {
			/* It looks mathematically like you should be able to
			 * combine the following lines with the ones above, but
			 * unsigned arithmetic is fun when it wraps...
			 */
			tx_total_len += PAGE_SIZE - call->first_offset;
			tx_total_len += call->last_to;
			tx_total_len += (call->last - call->first - 1) * PAGE_SIZE;
		}
=======
	if (call->write_iter)
		tx_total_len += iov_iter_count(call->write_iter);

	/* If the call is going to be asynchronous, we need an extra ref for
	 * the call to hold itself so the caller need not hang on to its ref.
	 */
	if (call->async) {
		afs_get_call(call, afs_call_trace_get);
		call->drop_ref = true;
>>>>>>> upstream/android-13
	}

	/* create a call */
	rxcall = rxrpc_kernel_begin_call(call->net->socket, srx, call->key,
					 (unsigned long)call,
					 tx_total_len, gfp,
<<<<<<< HEAD
					 (async ?
					  afs_wake_up_async_call :
					  afs_wake_up_call_waiter),
					 call->upgrade,
					 call->debug_id);
	if (IS_ERR(rxcall)) {
		ret = PTR_ERR(rxcall);
=======
					 (call->async ?
					  afs_wake_up_async_call :
					  afs_wake_up_call_waiter),
					 call->upgrade,
					 (call->intr ? RXRPC_PREINTERRUPTIBLE :
					  RXRPC_UNINTERRUPTIBLE),
					 call->debug_id);
	if (IS_ERR(rxcall)) {
		ret = PTR_ERR(rxcall);
		call->error = ret;
>>>>>>> upstream/android-13
		goto error_kill_call;
	}

	call->rxcall = rxcall;

<<<<<<< HEAD
=======
	if (call->max_lifespan)
		rxrpc_kernel_set_max_life(call->net->socket, rxcall,
					  call->max_lifespan);

>>>>>>> upstream/android-13
	/* send the request */
	iov[0].iov_base	= call->request;
	iov[0].iov_len	= call->request_size;

	msg.msg_name		= NULL;
	msg.msg_namelen		= 0;
<<<<<<< HEAD
	iov_iter_kvec(&msg.msg_iter, WRITE | ITER_KVEC, iov, 1,
		      call->request_size);
	msg.msg_control		= NULL;
	msg.msg_controllen	= 0;
	msg.msg_flags		= MSG_WAITALL | (call->send_pages ? MSG_MORE : 0);
=======
	iov_iter_kvec(&msg.msg_iter, WRITE, iov, 1, call->request_size);
	msg.msg_control		= NULL;
	msg.msg_controllen	= 0;
	msg.msg_flags		= MSG_WAITALL | (call->write_iter ? MSG_MORE : 0);
>>>>>>> upstream/android-13

	ret = rxrpc_kernel_send_data(call->net->socket, rxcall,
				     &msg, call->request_size,
				     afs_notify_end_request_tx);
	if (ret < 0)
		goto error_do_abort;

<<<<<<< HEAD
	if (call->send_pages) {
		ret = afs_send_pages(call, &msg);
=======
	if (call->write_iter) {
		msg.msg_iter = *call->write_iter;
		msg.msg_flags &= ~MSG_MORE;
		trace_afs_send_data(call, &msg);

		ret = rxrpc_kernel_send_data(call->net->socket,
					     call->rxcall, &msg,
					     iov_iter_count(&msg.msg_iter),
					     afs_notify_end_request_tx);
		*call->write_iter = msg.msg_iter;

		trace_afs_sent_data(call, &msg, ret);
>>>>>>> upstream/android-13
		if (ret < 0)
			goto error_do_abort;
	}

<<<<<<< HEAD
	/* at this point, an async call may no longer exist as it may have
	 * already completed */
	if (call->async)
		return -EINPROGRESS;

	return afs_wait_for_call_to_complete(call, ac);

error_do_abort:
	call->state = AFS_CALL_COMPLETE;
=======
	/* Note that at this point, we may have received the reply or an abort
	 * - and an asynchronous call may already have completed.
	 *
	 * afs_wait_for_call_to_complete(call, ac)
	 * must be called to synchronously clean up.
	 */
	return;

error_do_abort:
>>>>>>> upstream/android-13
	if (ret != -ECONNABORTED) {
		rxrpc_kernel_abort_call(call->net->socket, rxcall,
					RX_USER_ABORT, ret, "KSD");
	} else {
<<<<<<< HEAD
		iov_iter_kvec(&msg.msg_iter, READ | ITER_KVEC, NULL, 0, 0);
		rxrpc_kernel_recv_data(call->net->socket, rxcall,
				       &msg.msg_iter, false,
=======
		len = 0;
		iov_iter_kvec(&msg.msg_iter, READ, NULL, 0, 0);
		rxrpc_kernel_recv_data(call->net->socket, rxcall,
				       &msg.msg_iter, &len, false,
>>>>>>> upstream/android-13
				       &call->abort_code, &call->service_id);
		ac->abort_code = call->abort_code;
		ac->responded = true;
	}
	call->error = ret;
	trace_afs_call_done(call);
error_kill_call:
<<<<<<< HEAD
	afs_put_call(call);
	ac->error = ret;
	_leave(" = %d", ret);
	return ret;
=======
	if (call->type->done)
		call->type->done(call);

	/* We need to dispose of the extra ref we grabbed for an async call.
	 * The call, however, might be queued on afs_async_calls and we need to
	 * make sure we don't get any more notifications that might requeue it.
	 */
	if (call->rxcall) {
		rxrpc_kernel_end_call(call->net->socket, call->rxcall);
		call->rxcall = NULL;
	}
	if (call->async) {
		if (cancel_work_sync(&call->async_work))
			afs_put_call(call);
		afs_put_call(call);
	}

	ac->error = ret;
	call->state = AFS_CALL_COMPLETE;
	_leave(" = %d", ret);
}

/*
 * Log remote abort codes that indicate that we have a protocol disagreement
 * with the server.
 */
static void afs_log_error(struct afs_call *call, s32 remote_abort)
{
	static int max = 0;
	const char *msg;
	int m;

	switch (remote_abort) {
	case RX_EOF:		 msg = "unexpected EOF";	break;
	case RXGEN_CC_MARSHAL:	 msg = "client marshalling";	break;
	case RXGEN_CC_UNMARSHAL: msg = "client unmarshalling";	break;
	case RXGEN_SS_MARSHAL:	 msg = "server marshalling";	break;
	case RXGEN_SS_UNMARSHAL: msg = "server unmarshalling";	break;
	case RXGEN_DECODE:	 msg = "opcode decode";		break;
	case RXGEN_SS_XDRFREE:	 msg = "server XDR cleanup";	break;
	case RXGEN_CC_XDRFREE:	 msg = "client XDR cleanup";	break;
	case -32:		 msg = "insufficient data";	break;
	default:
		return;
	}

	m = max;
	if (m < 3) {
		max = m + 1;
		pr_notice("kAFS: Peer reported %s failure on %s [%pISp]\n",
			  msg, call->type->name,
			  &call->alist->addrs[call->addr_ix].transport);
	}
>>>>>>> upstream/android-13
}

/*
 * deliver messages to a call
 */
static void afs_deliver_to_call(struct afs_call *call)
{
	enum afs_call_state state;
<<<<<<< HEAD
=======
	size_t len;
>>>>>>> upstream/android-13
	u32 abort_code, remote_abort = 0;
	int ret;

	_enter("%s", call->type->name);

	while (state = READ_ONCE(call->state),
	       state == AFS_CALL_CL_AWAIT_REPLY ||
	       state == AFS_CALL_SV_AWAIT_OP_ID ||
	       state == AFS_CALL_SV_AWAIT_REQUEST ||
	       state == AFS_CALL_SV_AWAIT_ACK
	       ) {
		if (state == AFS_CALL_SV_AWAIT_ACK) {
<<<<<<< HEAD
			struct iov_iter iter;

			iov_iter_kvec(&iter, READ | ITER_KVEC, NULL, 0, 0);
			ret = rxrpc_kernel_recv_data(call->net->socket,
						     call->rxcall, &iter, false,
						     &remote_abort,
						     &call->service_id);
			trace_afs_recv_data(call, 0, 0, false, ret);
=======
			len = 0;
			iov_iter_kvec(&call->def_iter, READ, NULL, 0, 0);
			ret = rxrpc_kernel_recv_data(call->net->socket,
						     call->rxcall, &call->def_iter,
						     &len, false, &remote_abort,
						     &call->service_id);
			trace_afs_receive_data(call, &call->def_iter, false, ret);
>>>>>>> upstream/android-13

			if (ret == -EINPROGRESS || ret == -EAGAIN)
				return;
			if (ret < 0 || ret == 1) {
				if (ret == 1)
					ret = 0;
				goto call_complete;
			}
			return;
		}

<<<<<<< HEAD
		ret = call->type->deliver(call);
		state = READ_ONCE(call->state);
		switch (ret) {
		case 0:
			if (state == AFS_CALL_CL_PROC_REPLY) {
				if (call->cbi)
					set_bit(AFS_SERVER_FL_MAY_HAVE_CB,
						&call->cbi->server->flags);
=======
		if (!call->have_reply_time &&
		    rxrpc_kernel_get_reply_time(call->net->socket,
						call->rxcall,
						&call->reply_time))
			call->have_reply_time = true;

		ret = call->type->deliver(call);
		state = READ_ONCE(call->state);
		if (ret == 0 && call->unmarshalling_error)
			ret = -EBADMSG;
		switch (ret) {
		case 0:
			afs_queue_call_work(call);
			if (state == AFS_CALL_CL_PROC_REPLY) {
				if (call->op)
					set_bit(AFS_SERVER_FL_MAY_HAVE_CB,
						&call->op->server->flags);
>>>>>>> upstream/android-13
				goto call_complete;
			}
			ASSERTCMP(state, >, AFS_CALL_CL_PROC_REPLY);
			goto done;
		case -EINPROGRESS:
		case -EAGAIN:
			goto out;
		case -ECONNABORTED:
			ASSERTCMP(state, ==, AFS_CALL_COMPLETE);
<<<<<<< HEAD
=======
			afs_log_error(call, call->abort_code);
>>>>>>> upstream/android-13
			goto done;
		case -ENOTSUPP:
			abort_code = RXGEN_OPCODE;
			rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
						abort_code, ret, "KIV");
			goto local_abort;
		case -EIO:
			pr_err("kAFS: Call %u in bad state %u\n",
			       call->debug_id, state);
<<<<<<< HEAD
			/* Fall through */
		case -ENODATA:
		case -EBADMSG:
		case -EMSGSIZE:
		default:
=======
			fallthrough;
		case -ENODATA:
		case -EBADMSG:
		case -EMSGSIZE:
>>>>>>> upstream/android-13
			abort_code = RXGEN_CC_UNMARSHAL;
			if (state != AFS_CALL_CL_AWAIT_REPLY)
				abort_code = RXGEN_SS_UNMARSHAL;
			rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
<<<<<<< HEAD
						abort_code, -EBADMSG, "KUM");
=======
						abort_code, ret, "KUM");
			goto local_abort;
		default:
			abort_code = RX_USER_ABORT;
			rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
						abort_code, ret, "KER");
>>>>>>> upstream/android-13
			goto local_abort;
		}
	}

done:
<<<<<<< HEAD
	if (state == AFS_CALL_COMPLETE && call->incoming)
		afs_put_call(call);
=======
	if (call->type->done)
		call->type->done(call);
>>>>>>> upstream/android-13
out:
	_leave("");
	return;

local_abort:
	abort_code = 0;
call_complete:
	afs_set_call_complete(call, ret, remote_abort);
	state = AFS_CALL_COMPLETE;
	goto done;
}

/*
<<<<<<< HEAD
 * wait synchronously for a call to complete
 */
static long afs_wait_for_call_to_complete(struct afs_call *call,
					  struct afs_addr_cursor *ac)
{
	signed long rtt2, timeout;
	long ret;
	u64 rtt;
	u32 life, last_life;
=======
 * Wait synchronously for a call to complete and clean up the call struct.
 */
long afs_wait_for_call_to_complete(struct afs_call *call,
				   struct afs_addr_cursor *ac)
{
	long ret;
	bool rxrpc_complete = false;
>>>>>>> upstream/android-13

	DECLARE_WAITQUEUE(myself, current);

	_enter("");

<<<<<<< HEAD
	rtt = rxrpc_kernel_get_rtt(call->net->socket, call->rxcall);
	rtt2 = nsecs_to_jiffies64(rtt) * 2;
	if (rtt2 < 2)
		rtt2 = 2;

	timeout = rtt2;
	last_life = rxrpc_kernel_check_life(call->net->socket, call->rxcall);
=======
	ret = call->error;
	if (ret < 0)
		goto out;
>>>>>>> upstream/android-13

	add_wait_queue(&call->waitq, &myself);
	for (;;) {
		set_current_state(TASK_UNINTERRUPTIBLE);

		/* deliver any messages that are in the queue */
		if (!afs_check_call_state(call, AFS_CALL_COMPLETE) &&
		    call->need_attention) {
			call->need_attention = false;
			__set_current_state(TASK_RUNNING);
			afs_deliver_to_call(call);
<<<<<<< HEAD
			timeout = rtt2;
=======
>>>>>>> upstream/android-13
			continue;
		}

		if (afs_check_call_state(call, AFS_CALL_COMPLETE))
			break;

<<<<<<< HEAD
		life = rxrpc_kernel_check_life(call->net->socket, call->rxcall);
		if (timeout == 0 &&
		    life == last_life && signal_pending(current))
				break;

		if (life != last_life) {
			timeout = rtt2;
			last_life = life;
		}

		timeout = schedule_timeout(timeout);
=======
		if (!rxrpc_kernel_check_life(call->net->socket, call->rxcall)) {
			/* rxrpc terminated the call. */
			rxrpc_complete = true;
			break;
		}

		schedule();
>>>>>>> upstream/android-13
	}

	remove_wait_queue(&call->waitq, &myself);
	__set_current_state(TASK_RUNNING);

<<<<<<< HEAD
	/* Kill off the call if it's still live. */
	if (!afs_check_call_state(call, AFS_CALL_COMPLETE)) {
		_debug("call interrupted");
		if (rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
					    RX_USER_ABORT, -EINTR, "KWI"))
			afs_set_call_complete(call, -EINTR, 0);
=======
	if (!afs_check_call_state(call, AFS_CALL_COMPLETE)) {
		if (rxrpc_complete) {
			afs_set_call_complete(call, call->error, call->abort_code);
		} else {
			/* Kill off the call if it's still live. */
			_debug("call interrupted");
			if (rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
						    RX_USER_ABORT, -EINTR, "KWI"))
				afs_set_call_complete(call, -EINTR, 0);
		}
>>>>>>> upstream/android-13
	}

	spin_lock_bh(&call->state_lock);
	ac->abort_code = call->abort_code;
	ac->error = call->error;
	spin_unlock_bh(&call->state_lock);

	ret = ac->error;
	switch (ret) {
	case 0:
<<<<<<< HEAD
		if (call->ret_reply0) {
			ret = (long)call->reply[0];
			call->reply[0] = NULL;
		}
		/* Fall through */
=======
		ret = call->ret0;
		call->ret0 = 0;

		fallthrough;
>>>>>>> upstream/android-13
	case -ECONNABORTED:
		ac->responded = true;
		break;
	}

<<<<<<< HEAD
=======
out:
>>>>>>> upstream/android-13
	_debug("call complete");
	afs_put_call(call);
	_leave(" = %p", (void *)ret);
	return ret;
}

/*
 * wake up a waiting call
 */
static void afs_wake_up_call_waiter(struct sock *sk, struct rxrpc_call *rxcall,
				    unsigned long call_user_ID)
{
	struct afs_call *call = (struct afs_call *)call_user_ID;

	call->need_attention = true;
	wake_up(&call->waitq);
}

/*
 * wake up an asynchronous call
 */
static void afs_wake_up_async_call(struct sock *sk, struct rxrpc_call *rxcall,
				   unsigned long call_user_ID)
{
	struct afs_call *call = (struct afs_call *)call_user_ID;
	int u;

	trace_afs_notify_call(rxcall, call);
	call->need_attention = true;

	u = atomic_fetch_add_unless(&call->usage, 1, 0);
	if (u != 0) {
		trace_afs_call(call, afs_call_trace_wake, u + 1,
			       atomic_read(&call->net->nr_outstanding_calls),
			       __builtin_return_address(0));

		if (!queue_work(afs_async_calls, &call->async_work))
			afs_put_call(call);
	}
}

/*
<<<<<<< HEAD
 * Delete an asynchronous call.  The work item carries a ref to the call struct
 * that we need to release.
 */
static void afs_delete_async_call(struct work_struct *work)
{
	struct afs_call *call = container_of(work, struct afs_call, async_work);

	_enter("");

	afs_put_call(call);

	_leave("");
}

/*
=======
>>>>>>> upstream/android-13
 * Perform I/O processing on an asynchronous call.  The work item carries a ref
 * to the call struct that we either need to release or to pass on.
 */
static void afs_process_async_call(struct work_struct *work)
{
	struct afs_call *call = container_of(work, struct afs_call, async_work);

	_enter("");

	if (call->state < AFS_CALL_COMPLETE && call->need_attention) {
		call->need_attention = false;
		afs_deliver_to_call(call);
	}

<<<<<<< HEAD
	if (call->state == AFS_CALL_COMPLETE) {
		/* We have two refs to release - one from the alloc and one
		 * queued with the work item - and we can't just deallocate the
		 * call because the work item may be queued again.
		 */
		call->async_work.func = afs_delete_async_call;
		if (!queue_work(afs_async_calls, &call->async_work))
			afs_put_call(call);
	}

=======
>>>>>>> upstream/android-13
	afs_put_call(call);
	_leave("");
}

static void afs_rx_attach(struct rxrpc_call *rxcall, unsigned long user_call_ID)
{
	struct afs_call *call = (struct afs_call *)user_call_ID;

	call->rxcall = rxcall;
}

/*
 * Charge the incoming call preallocation.
 */
void afs_charge_preallocation(struct work_struct *work)
{
	struct afs_net *net =
		container_of(work, struct afs_net, charge_preallocation_work);
	struct afs_call *call = net->spare_incoming_call;

	for (;;) {
		if (!call) {
			call = afs_alloc_call(net, &afs_RXCMxxxx, GFP_KERNEL);
			if (!call)
				break;

<<<<<<< HEAD
			call->async = true;
			call->state = AFS_CALL_SV_AWAIT_OP_ID;
			init_waitqueue_head(&call->waitq);
=======
			call->drop_ref = true;
			call->async = true;
			call->state = AFS_CALL_SV_AWAIT_OP_ID;
			init_waitqueue_head(&call->waitq);
			afs_extract_to_tmp(call);
>>>>>>> upstream/android-13
		}

		if (rxrpc_kernel_charge_accept(net->socket,
					       afs_wake_up_async_call,
					       afs_rx_attach,
					       (unsigned long)call,
					       GFP_KERNEL,
					       call->debug_id) < 0)
			break;
		call = NULL;
	}
	net->spare_incoming_call = call;
}

/*
 * Discard a preallocated call when a socket is shut down.
 */
static void afs_rx_discard_new_call(struct rxrpc_call *rxcall,
				    unsigned long user_call_ID)
{
	struct afs_call *call = (struct afs_call *)user_call_ID;

	call->rxcall = NULL;
	afs_put_call(call);
}

/*
 * Notification of an incoming call.
 */
static void afs_rx_new_call(struct sock *sk, struct rxrpc_call *rxcall,
			    unsigned long user_call_ID)
{
	struct afs_net *net = afs_sock2net(sk);

	queue_work(afs_wq, &net->charge_preallocation_work);
}

/*
 * Grab the operation ID from an incoming cache manager call.  The socket
 * buffer is discarded on error or if we don't yet have sufficient data.
 */
static int afs_deliver_cm_op_id(struct afs_call *call)
{
	int ret;

<<<<<<< HEAD
	_enter("{%zu}", call->offset);

	ASSERTCMP(call->offset, <, 4);

	/* the operation ID forms the first four bytes of the request data */
	ret = afs_extract_data(call, &call->tmp, 4, true);
=======
	_enter("{%zu}", iov_iter_count(call->iter));

	/* the operation ID forms the first four bytes of the request data */
	ret = afs_extract_data(call, true);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	call->operation_ID = ntohl(call->tmp);
	afs_set_call_state(call, AFS_CALL_SV_AWAIT_OP_ID, AFS_CALL_SV_AWAIT_REQUEST);
<<<<<<< HEAD
	call->offset = 0;
=======
>>>>>>> upstream/android-13

	/* ask the cache manager to route the call (it'll change the call type
	 * if successful) */
	if (!afs_cm_incoming_call(call))
		return -ENOTSUPP;

	trace_afs_cb_call(call);

	/* pass responsibility for the remainer of this message off to the
	 * cache manager op */
	return call->type->deliver(call);
}

/*
 * Advance the AFS call state when an RxRPC service call ends the transmit
 * phase.
 */
static void afs_notify_end_reply_tx(struct sock *sock,
				    struct rxrpc_call *rxcall,
				    unsigned long call_user_ID)
{
	struct afs_call *call = (struct afs_call *)call_user_ID;

	afs_set_call_state(call, AFS_CALL_SV_REPLYING, AFS_CALL_SV_AWAIT_ACK);
}

/*
 * send an empty reply
 */
void afs_send_empty_reply(struct afs_call *call)
{
	struct afs_net *net = call->net;
	struct msghdr msg;

	_enter("");

	rxrpc_kernel_set_tx_length(net->socket, call->rxcall, 0);

	msg.msg_name		= NULL;
	msg.msg_namelen		= 0;
<<<<<<< HEAD
	iov_iter_kvec(&msg.msg_iter, WRITE | ITER_KVEC, NULL, 0, 0);
=======
	iov_iter_kvec(&msg.msg_iter, WRITE, NULL, 0, 0);
>>>>>>> upstream/android-13
	msg.msg_control		= NULL;
	msg.msg_controllen	= 0;
	msg.msg_flags		= 0;

	switch (rxrpc_kernel_send_data(net->socket, call->rxcall, &msg, 0,
				       afs_notify_end_reply_tx)) {
	case 0:
		_leave(" [replied]");
		return;

	case -ENOMEM:
		_debug("oom");
		rxrpc_kernel_abort_call(net->socket, call->rxcall,
					RX_USER_ABORT, -ENOMEM, "KOO");
<<<<<<< HEAD
=======
		fallthrough;
>>>>>>> upstream/android-13
	default:
		_leave(" [error]");
		return;
	}
}

/*
 * send a simple reply
 */
void afs_send_simple_reply(struct afs_call *call, const void *buf, size_t len)
{
	struct afs_net *net = call->net;
	struct msghdr msg;
	struct kvec iov[1];
	int n;

	_enter("");

	rxrpc_kernel_set_tx_length(net->socket, call->rxcall, len);

	iov[0].iov_base		= (void *) buf;
	iov[0].iov_len		= len;
	msg.msg_name		= NULL;
	msg.msg_namelen		= 0;
<<<<<<< HEAD
	iov_iter_kvec(&msg.msg_iter, WRITE | ITER_KVEC, iov, 1, len);
=======
	iov_iter_kvec(&msg.msg_iter, WRITE, iov, 1, len);
>>>>>>> upstream/android-13
	msg.msg_control		= NULL;
	msg.msg_controllen	= 0;
	msg.msg_flags		= 0;

	n = rxrpc_kernel_send_data(net->socket, call->rxcall, &msg, len,
				   afs_notify_end_reply_tx);
	if (n >= 0) {
		/* Success */
		_leave(" [replied]");
		return;
	}

	if (n == -ENOMEM) {
		_debug("oom");
		rxrpc_kernel_abort_call(net->socket, call->rxcall,
					RX_USER_ABORT, -ENOMEM, "KOO");
	}
	_leave(" [error]");
}

/*
 * Extract a piece of data from the received data socket buffers.
 */
<<<<<<< HEAD
int afs_extract_data(struct afs_call *call, void *buf, size_t count,
		     bool want_more)
{
	struct afs_net *net = call->net;
	struct iov_iter iter;
	struct kvec iov;
=======
int afs_extract_data(struct afs_call *call, bool want_more)
{
	struct afs_net *net = call->net;
	struct iov_iter *iter = call->iter;
>>>>>>> upstream/android-13
	enum afs_call_state state;
	u32 remote_abort = 0;
	int ret;

<<<<<<< HEAD
	_enter("{%s,%zu},,%zu,%d",
	       call->type->name, call->offset, count, want_more);

	ASSERTCMP(call->offset, <=, count);

	iov.iov_base = buf + call->offset;
	iov.iov_len = count - call->offset;
	iov_iter_kvec(&iter, ITER_KVEC | READ, &iov, 1, count - call->offset);

	ret = rxrpc_kernel_recv_data(net->socket, call->rxcall, &iter,
				     want_more, &remote_abort,
				     &call->service_id);
	call->offset += (count - call->offset) - iov_iter_count(&iter);
	trace_afs_recv_data(call, count, call->offset, want_more, ret);
=======
	_enter("{%s,%zu,%zu},%d",
	       call->type->name, call->iov_len, iov_iter_count(iter), want_more);

	ret = rxrpc_kernel_recv_data(net->socket, call->rxcall, iter,
				     &call->iov_len, want_more, &remote_abort,
				     &call->service_id);
>>>>>>> upstream/android-13
	if (ret == 0 || ret == -EAGAIN)
		return ret;

	state = READ_ONCE(call->state);
	if (ret == 1) {
		switch (state) {
		case AFS_CALL_CL_AWAIT_REPLY:
			afs_set_call_state(call, state, AFS_CALL_CL_PROC_REPLY);
			break;
		case AFS_CALL_SV_AWAIT_REQUEST:
			afs_set_call_state(call, state, AFS_CALL_SV_REPLYING);
			break;
		case AFS_CALL_COMPLETE:
			kdebug("prem complete %d", call->error);
<<<<<<< HEAD
			return -EIO;
=======
			return afs_io_error(call, afs_io_error_extract);
>>>>>>> upstream/android-13
		default:
			break;
		}
		return 0;
	}

	afs_set_call_complete(call, ret, remote_abort);
	return ret;
}

/*
 * Log protocol error production.
 */
<<<<<<< HEAD
noinline int afs_protocol_error(struct afs_call *call, int error)
{
	trace_afs_protocol_error(call, error, __builtin_return_address(0));
	return error;
=======
noinline int afs_protocol_error(struct afs_call *call,
				enum afs_eproto_cause cause)
{
	trace_afs_protocol_error(call, cause);
	if (call)
		call->unmarshalling_error = true;
	return -EBADMSG;
>>>>>>> upstream/android-13
}
