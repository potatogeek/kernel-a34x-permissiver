<<<<<<< HEAD
/* Client connection-specific management code.
 *
 * Copyright (C) 2016 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 *
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* Client connection-specific management code.
 *
 * Copyright (C) 2016, 2020 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
>>>>>>> upstream/android-13
 * Client connections need to be cached for a little while after they've made a
 * call so as to handle retransmitted DATA packets in case the server didn't
 * receive the final ACK or terminating ABORT we sent it.
 *
<<<<<<< HEAD
 * Client connections can be in one of a number of cache states:
 *
 *  (1) INACTIVE - The connection is not held in any list and may not have been
 *      exposed to the world.  If it has been previously exposed, it was
 *      discarded from the idle list after expiring.
 *
 *  (2) WAITING - The connection is waiting for the number of client conns to
 *      drop below the maximum capacity.  Calls may be in progress upon it from
 *      when it was active and got culled.
 *
 *	The connection is on the rxrpc_waiting_client_conns list which is kept
 *	in to-be-granted order.  Culled conns with waiters go to the back of
 *	the queue just like new conns.
 *
 *  (3) ACTIVE - The connection has at least one call in progress upon it, it
 *      may freely grant available channels to new calls and calls may be
 *      waiting on it for channels to become available.
 *
 *	The connection is on the rxnet->active_client_conns list which is kept
 *	in activation order for culling purposes.
 *
 *	rxrpc_nr_active_client_conns is held incremented also.
 *
 *  (4) UPGRADE - As for ACTIVE, but only one call may be in progress and is
 *      being used to probe for service upgrade.
 *
 *  (5) CULLED - The connection got summarily culled to try and free up
 *      capacity.  Calls currently in progress on the connection are allowed to
 *      continue, but new calls will have to wait.  There can be no waiters in
 *      this state - the conn would have to go to the WAITING state instead.
 *
 *  (6) IDLE - The connection has no calls in progress upon it and must have
 *      been exposed to the world (ie. the EXPOSED flag must be set).  When it
 *      expires, the EXPOSED flag is cleared and the connection transitions to
 *      the INACTIVE state.
 *
 *	The connection is on the rxnet->idle_client_conns list which is kept in
 *	order of how soon they'll expire.
 *
 * There are flags of relevance to the cache:
 *
 *  (1) EXPOSED - The connection ID got exposed to the world.  If this flag is
 *      set, an extra ref is added to the connection preventing it from being
 *      reaped when it has no calls outstanding.  This flag is cleared and the
 *      ref dropped when a conn is discarded from the idle list.
 *
 *      This allows us to move terminal call state retransmission to the
 *      connection and to discard the call immediately we think it is done
 *      with.  It also give us a chance to reuse the connection.
 *
=======
 * There are flags of relevance to the cache:
 *
>>>>>>> upstream/android-13
 *  (2) DONT_REUSE - The connection should be discarded as soon as possible and
 *      should not be reused.  This is set when an exclusive connection is used
 *      or a call ID counter overflows.
 *
 * The caching state may only be changed if the cache lock is held.
 *
 * There are two idle client connection expiry durations.  If the total number
 * of connections is below the reap threshold, we use the normal duration; if
 * it's above, we use the fast duration.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/slab.h>
#include <linux/idr.h>
#include <linux/timer.h>
#include <linux/sched/signal.h>

#include "ar-internal.h"

<<<<<<< HEAD
__read_mostly unsigned int rxrpc_max_client_connections = 1000;
=======
>>>>>>> upstream/android-13
__read_mostly unsigned int rxrpc_reap_client_connections = 900;
__read_mostly unsigned long rxrpc_conn_idle_client_expiry = 2 * 60 * HZ;
__read_mostly unsigned long rxrpc_conn_idle_client_fast_expiry = 2 * HZ;

/*
 * We use machine-unique IDs for our client connections.
 */
DEFINE_IDR(rxrpc_client_conn_ids);
static DEFINE_SPINLOCK(rxrpc_conn_id_lock);

<<<<<<< HEAD
static void rxrpc_cull_active_client_conns(struct rxrpc_net *);

=======
>>>>>>> upstream/android-13
/*
 * Get a connection ID and epoch for a client connection from the global pool.
 * The connection struct pointer is then recorded in the idr radix tree.  The
 * epoch doesn't change until the client is rebooted (or, at least, unless the
 * module is unloaded).
 */
static int rxrpc_get_client_connection_id(struct rxrpc_connection *conn,
					  gfp_t gfp)
{
	struct rxrpc_net *rxnet = conn->params.local->rxnet;
	int id;

	_enter("");

	idr_preload(gfp);
	spin_lock(&rxrpc_conn_id_lock);

	id = idr_alloc_cyclic(&rxrpc_client_conn_ids, conn,
			      1, 0x40000000, GFP_NOWAIT);
	if (id < 0)
		goto error;

	spin_unlock(&rxrpc_conn_id_lock);
	idr_preload_end();

	conn->proto.epoch = rxnet->epoch;
	conn->proto.cid = id << RXRPC_CIDSHIFT;
	set_bit(RXRPC_CONN_HAS_IDR, &conn->flags);
	_leave(" [CID %x]", conn->proto.cid);
	return 0;

error:
	spin_unlock(&rxrpc_conn_id_lock);
	idr_preload_end();
	_leave(" = %d", id);
	return id;
}

/*
 * Release a connection ID for a client connection from the global pool.
 */
static void rxrpc_put_client_connection_id(struct rxrpc_connection *conn)
{
	if (test_bit(RXRPC_CONN_HAS_IDR, &conn->flags)) {
		spin_lock(&rxrpc_conn_id_lock);
		idr_remove(&rxrpc_client_conn_ids,
			   conn->proto.cid >> RXRPC_CIDSHIFT);
		spin_unlock(&rxrpc_conn_id_lock);
	}
}

/*
 * Destroy the client connection ID tree.
 */
void rxrpc_destroy_client_conn_ids(void)
{
	struct rxrpc_connection *conn;
	int id;

	if (!idr_is_empty(&rxrpc_client_conn_ids)) {
		idr_for_each_entry(&rxrpc_client_conn_ids, conn, id) {
			pr_err("AF_RXRPC: Leaked client conn %p {%d}\n",
			       conn, atomic_read(&conn->usage));
		}
		BUG();
	}

	idr_destroy(&rxrpc_client_conn_ids);
}

/*
<<<<<<< HEAD
 * Allocate a client connection.
 */
static struct rxrpc_connection *
rxrpc_alloc_client_connection(struct rxrpc_conn_parameters *cp, gfp_t gfp)
{
	struct rxrpc_connection *conn;
	struct rxrpc_net *rxnet = cp->local->rxnet;
=======
 * Allocate a connection bundle.
 */
static struct rxrpc_bundle *rxrpc_alloc_bundle(struct rxrpc_conn_parameters *cp,
					       gfp_t gfp)
{
	struct rxrpc_bundle *bundle;

	bundle = kzalloc(sizeof(*bundle), gfp);
	if (bundle) {
		bundle->params = *cp;
		rxrpc_get_peer(bundle->params.peer);
		atomic_set(&bundle->usage, 1);
		spin_lock_init(&bundle->channel_lock);
		INIT_LIST_HEAD(&bundle->waiting_calls);
	}
	return bundle;
}

struct rxrpc_bundle *rxrpc_get_bundle(struct rxrpc_bundle *bundle)
{
	atomic_inc(&bundle->usage);
	return bundle;
}

static void rxrpc_free_bundle(struct rxrpc_bundle *bundle)
{
	rxrpc_put_peer(bundle->params.peer);
	kfree(bundle);
}

void rxrpc_put_bundle(struct rxrpc_bundle *bundle)
{
	unsigned int d = bundle->debug_id;
	unsigned int u = atomic_dec_return(&bundle->usage);

	_debug("PUT B=%x %u", d, u);
	if (u == 0)
		rxrpc_free_bundle(bundle);
}

/*
 * Allocate a client connection.
 */
static struct rxrpc_connection *
rxrpc_alloc_client_connection(struct rxrpc_bundle *bundle, gfp_t gfp)
{
	struct rxrpc_connection *conn;
	struct rxrpc_net *rxnet = bundle->params.local->rxnet;
>>>>>>> upstream/android-13
	int ret;

	_enter("");

	conn = rxrpc_alloc_connection(gfp);
	if (!conn) {
		_leave(" = -ENOMEM");
		return ERR_PTR(-ENOMEM);
	}

	atomic_set(&conn->usage, 1);
<<<<<<< HEAD
	if (cp->exclusive)
		__set_bit(RXRPC_CONN_DONT_REUSE, &conn->flags);
	if (cp->upgrade)
		__set_bit(RXRPC_CONN_PROBING_FOR_UPGRADE, &conn->flags);

	conn->params		= *cp;
	conn->out_clientflag	= RXRPC_CLIENT_INITIATED;
	conn->state		= RXRPC_CONN_CLIENT;
	conn->service_id	= cp->service_id;
=======
	conn->bundle		= bundle;
	conn->params		= bundle->params;
	conn->out_clientflag	= RXRPC_CLIENT_INITIATED;
	conn->state		= RXRPC_CONN_CLIENT;
	conn->service_id	= conn->params.service_id;
>>>>>>> upstream/android-13

	ret = rxrpc_get_client_connection_id(conn, gfp);
	if (ret < 0)
		goto error_0;

	ret = rxrpc_init_client_conn_security(conn);
	if (ret < 0)
		goto error_1;

<<<<<<< HEAD
	ret = conn->security->prime_packet_security(conn);
	if (ret < 0)
		goto error_2;

=======
>>>>>>> upstream/android-13
	atomic_inc(&rxnet->nr_conns);
	write_lock(&rxnet->conn_lock);
	list_add_tail(&conn->proc_link, &rxnet->conn_proc_list);
	write_unlock(&rxnet->conn_lock);

<<<<<<< HEAD
	/* We steal the caller's peer ref. */
	cp->peer = NULL;
=======
	rxrpc_get_bundle(bundle);
	rxrpc_get_peer(conn->params.peer);
>>>>>>> upstream/android-13
	rxrpc_get_local(conn->params.local);
	key_get(conn->params.key);

	trace_rxrpc_conn(conn->debug_id, rxrpc_conn_new_client,
			 atomic_read(&conn->usage),
			 __builtin_return_address(0));
<<<<<<< HEAD
=======

	atomic_inc(&rxnet->nr_client_conns);
>>>>>>> upstream/android-13
	trace_rxrpc_client(conn, -1, rxrpc_client_alloc);
	_leave(" = %p", conn);
	return conn;

<<<<<<< HEAD
error_2:
	conn->security->clear(conn);
=======
>>>>>>> upstream/android-13
error_1:
	rxrpc_put_client_connection_id(conn);
error_0:
	kfree(conn);
	_leave(" = %d", ret);
	return ERR_PTR(ret);
}

/*
 * Determine if a connection may be reused.
 */
static bool rxrpc_may_reuse_conn(struct rxrpc_connection *conn)
{
<<<<<<< HEAD
	struct rxrpc_net *rxnet = conn->params.local->rxnet;
	int id_cursor, id, distance, limit;

	if (test_bit(RXRPC_CONN_DONT_REUSE, &conn->flags))
		goto dont_reuse;

	if (conn->proto.epoch != rxnet->epoch)
=======
	struct rxrpc_net *rxnet;
	int id_cursor, id, distance, limit;

	if (!conn)
		goto dont_reuse;

	rxnet = conn->params.local->rxnet;
	if (test_bit(RXRPC_CONN_DONT_REUSE, &conn->flags))
		goto dont_reuse;

	if (conn->state != RXRPC_CONN_CLIENT ||
	    conn->proto.epoch != rxnet->epoch)
>>>>>>> upstream/android-13
		goto mark_dont_reuse;

	/* The IDR tree gets very expensive on memory if the connection IDs are
	 * widely scattered throughout the number space, so we shall want to
	 * kill off connections that, say, have an ID more than about four
	 * times the maximum number of client conns away from the current
	 * allocation point to try and keep the IDs concentrated.
	 */
	id_cursor = idr_get_cursor(&rxrpc_client_conn_ids);
	id = conn->proto.cid >> RXRPC_CIDSHIFT;
	distance = id - id_cursor;
	if (distance < 0)
		distance = -distance;
<<<<<<< HEAD
	limit = max(rxrpc_max_client_connections * 4, 1024U);
=======
	limit = max_t(unsigned long, atomic_read(&rxnet->nr_conns) * 4, 1024);
>>>>>>> upstream/android-13
	if (distance > limit)
		goto mark_dont_reuse;

	return true;

mark_dont_reuse:
	set_bit(RXRPC_CONN_DONT_REUSE, &conn->flags);
dont_reuse:
	return false;
}

/*
<<<<<<< HEAD
 * Create or find a client connection to use for a call.
=======
 * Look up the conn bundle that matches the connection parameters, adding it if
 * it doesn't yet exist.
 */
static struct rxrpc_bundle *rxrpc_look_up_bundle(struct rxrpc_conn_parameters *cp,
						 gfp_t gfp)
{
	static atomic_t rxrpc_bundle_id;
	struct rxrpc_bundle *bundle, *candidate;
	struct rxrpc_local *local = cp->local;
	struct rb_node *p, **pp, *parent;
	long diff;

	_enter("{%px,%x,%u,%u}",
	       cp->peer, key_serial(cp->key), cp->security_level, cp->upgrade);

	if (cp->exclusive)
		return rxrpc_alloc_bundle(cp, gfp);

	/* First, see if the bundle is already there. */
	_debug("search 1");
	spin_lock(&local->client_bundles_lock);
	p = local->client_bundles.rb_node;
	while (p) {
		bundle = rb_entry(p, struct rxrpc_bundle, local_node);

#define cmp(X) ((long)bundle->params.X - (long)cp->X)
		diff = (cmp(peer) ?:
			cmp(key) ?:
			cmp(security_level) ?:
			cmp(upgrade));
#undef cmp
		if (diff < 0)
			p = p->rb_left;
		else if (diff > 0)
			p = p->rb_right;
		else
			goto found_bundle;
	}
	spin_unlock(&local->client_bundles_lock);
	_debug("not found");

	/* It wasn't.  We need to add one. */
	candidate = rxrpc_alloc_bundle(cp, gfp);
	if (!candidate)
		return NULL;

	_debug("search 2");
	spin_lock(&local->client_bundles_lock);
	pp = &local->client_bundles.rb_node;
	parent = NULL;
	while (*pp) {
		parent = *pp;
		bundle = rb_entry(parent, struct rxrpc_bundle, local_node);

#define cmp(X) ((long)bundle->params.X - (long)cp->X)
		diff = (cmp(peer) ?:
			cmp(key) ?:
			cmp(security_level) ?:
			cmp(upgrade));
#undef cmp
		if (diff < 0)
			pp = &(*pp)->rb_left;
		else if (diff > 0)
			pp = &(*pp)->rb_right;
		else
			goto found_bundle_free;
	}

	_debug("new bundle");
	candidate->debug_id = atomic_inc_return(&rxrpc_bundle_id);
	rb_link_node(&candidate->local_node, parent, pp);
	rb_insert_color(&candidate->local_node, &local->client_bundles);
	rxrpc_get_bundle(candidate);
	spin_unlock(&local->client_bundles_lock);
	_leave(" = %u [new]", candidate->debug_id);
	return candidate;

found_bundle_free:
	rxrpc_free_bundle(candidate);
found_bundle:
	rxrpc_get_bundle(bundle);
	spin_unlock(&local->client_bundles_lock);
	_leave(" = %u [found]", bundle->debug_id);
	return bundle;
}

/*
 * Create or find a client bundle to use for a call.
>>>>>>> upstream/android-13
 *
 * If we return with a connection, the call will be on its waiting list.  It's
 * left to the caller to assign a channel and wake up the call.
 */
<<<<<<< HEAD
static int rxrpc_get_client_conn(struct rxrpc_sock *rx,
				 struct rxrpc_call *call,
				 struct rxrpc_conn_parameters *cp,
				 struct sockaddr_rxrpc *srx,
				 gfp_t gfp)
{
	struct rxrpc_connection *conn, *candidate = NULL;
	struct rxrpc_local *local = cp->local;
	struct rb_node *p, **pp, *parent;
	long diff;
	int ret = -ENOMEM;
=======
static struct rxrpc_bundle *rxrpc_prep_call(struct rxrpc_sock *rx,
					    struct rxrpc_call *call,
					    struct rxrpc_conn_parameters *cp,
					    struct sockaddr_rxrpc *srx,
					    gfp_t gfp)
{
	struct rxrpc_bundle *bundle;
>>>>>>> upstream/android-13

	_enter("{%d,%lx},", call->debug_id, call->user_call_ID);

	cp->peer = rxrpc_lookup_peer(rx, cp->local, srx, gfp);
	if (!cp->peer)
		goto error;

	call->cong_cwnd = cp->peer->cong_cwnd;
	if (call->cong_cwnd >= call->cong_ssthresh)
		call->cong_mode = RXRPC_CALL_CONGEST_AVOIDANCE;
	else
		call->cong_mode = RXRPC_CALL_SLOW_START;
<<<<<<< HEAD

	/* If the connection is not meant to be exclusive, search the available
	 * connections to see if the connection we want to use already exists.
	 */
	if (!cp->exclusive) {
		_debug("search 1");
		spin_lock(&local->client_conns_lock);
		p = local->client_conns.rb_node;
		while (p) {
			conn = rb_entry(p, struct rxrpc_connection, client_node);

#define cmp(X) ((long)conn->params.X - (long)cp->X)
			diff = (cmp(peer) ?:
				cmp(key) ?:
				cmp(security_level) ?:
				cmp(upgrade));
#undef cmp
			if (diff < 0) {
				p = p->rb_left;
			} else if (diff > 0) {
				p = p->rb_right;
			} else {
				if (rxrpc_may_reuse_conn(conn) &&
				    rxrpc_get_connection_maybe(conn))
					goto found_extant_conn;
				/* The connection needs replacing.  It's better
				 * to effect that when we have something to
				 * replace it with so that we don't have to
				 * rebalance the tree twice.
				 */
				break;
			}
		}
		spin_unlock(&local->client_conns_lock);
	}

	/* There wasn't a connection yet or we need an exclusive connection.
	 * We need to create a candidate and then potentially redo the search
	 * in case we're racing with another thread also trying to connect on a
	 * shareable connection.
	 */
	_debug("new conn");
	candidate = rxrpc_alloc_client_connection(cp, gfp);
	if (IS_ERR(candidate)) {
		ret = PTR_ERR(candidate);
		goto error_peer;
	}

	/* Add the call to the new connection's waiting list in case we're
	 * going to have to wait for the connection to come live.  It's our
	 * connection, so we want first dibs on the channel slots.  We would
	 * normally have to take channel_lock but we do this before anyone else
	 * can see the connection.
	 */
	list_add(&call->chan_wait_link, &candidate->waiting_calls);

	if (cp->exclusive) {
		call->conn = candidate;
		call->security_ix = candidate->security_ix;
		call->service_id = candidate->service_id;
		_leave(" = 0 [exclusive %d]", candidate->debug_id);
		return 0;
	}

	/* Publish the new connection for userspace to find.  We need to redo
	 * the search before doing this lest we race with someone else adding a
	 * conflicting instance.
	 */
	_debug("search 2");
	spin_lock(&local->client_conns_lock);

	pp = &local->client_conns.rb_node;
	parent = NULL;
	while (*pp) {
		parent = *pp;
		conn = rb_entry(parent, struct rxrpc_connection, client_node);

#define cmp(X) ((long)conn->params.X - (long)candidate->params.X)
		diff = (cmp(peer) ?:
			cmp(key) ?:
			cmp(security_level) ?:
			cmp(upgrade));
#undef cmp
		if (diff < 0) {
			pp = &(*pp)->rb_left;
		} else if (diff > 0) {
			pp = &(*pp)->rb_right;
		} else {
			if (rxrpc_may_reuse_conn(conn) &&
			    rxrpc_get_connection_maybe(conn))
				goto found_extant_conn;
			/* The old connection is from an outdated epoch. */
			_debug("replace conn");
			clear_bit(RXRPC_CONN_IN_CLIENT_CONNS, &conn->flags);
			rb_replace_node(&conn->client_node,
					&candidate->client_node,
					&local->client_conns);
			trace_rxrpc_client(conn, -1, rxrpc_client_replace);
			goto candidate_published;
		}
	}

	_debug("new conn");
	rb_link_node(&candidate->client_node, parent, pp);
	rb_insert_color(&candidate->client_node, &local->client_conns);

candidate_published:
	set_bit(RXRPC_CONN_IN_CLIENT_CONNS, &candidate->flags);
	call->conn = candidate;
	call->security_ix = candidate->security_ix;
	call->service_id = candidate->service_id;
	spin_unlock(&local->client_conns_lock);
	_leave(" = 0 [new %d]", candidate->debug_id);
	return 0;

	/* We come here if we found a suitable connection already in existence.
	 * Discard any candidate we may have allocated, and try to get a
	 * channel on this one.
	 */
found_extant_conn:
	_debug("found conn");
	spin_unlock(&local->client_conns_lock);

	if (candidate) {
		trace_rxrpc_client(candidate, -1, rxrpc_client_duplicate);
		rxrpc_put_connection(candidate);
		candidate = NULL;
	}

	spin_lock(&conn->channel_lock);
	call->conn = conn;
	call->security_ix = conn->security_ix;
	call->service_id = conn->service_id;
	list_add_tail(&call->chan_wait_link, &conn->waiting_calls);
	spin_unlock(&conn->channel_lock);
	_leave(" = 0 [extant %d]", conn->debug_id);
	return 0;

error_peer:
	rxrpc_put_peer(cp->peer);
	cp->peer = NULL;
error:
	_leave(" = %d", ret);
	return ret;
}

/*
 * Activate a connection.
 */
static void rxrpc_activate_conn(struct rxrpc_net *rxnet,
				struct rxrpc_connection *conn)
{
	if (test_bit(RXRPC_CONN_PROBING_FOR_UPGRADE, &conn->flags)) {
		trace_rxrpc_client(conn, -1, rxrpc_client_to_upgrade);
		conn->cache_state = RXRPC_CONN_CLIENT_UPGRADE;
	} else {
		trace_rxrpc_client(conn, -1, rxrpc_client_to_active);
		conn->cache_state = RXRPC_CONN_CLIENT_ACTIVE;
	}
	rxnet->nr_active_client_conns++;
	list_move_tail(&conn->cache_link, &rxnet->active_client_conns);
}

/*
 * Attempt to animate a connection for a new call.
 *
 * If it's not exclusive, the connection is in the endpoint tree, and we're in
 * the conn's list of those waiting to grab a channel.  There is, however, a
 * limit on the number of live connections allowed at any one time, so we may
 * have to wait for capacity to become available.
 *
 * Note that a connection on the waiting queue might *also* have active
 * channels if it has been culled to make space and then re-requested by a new
 * call.
 */
static void rxrpc_animate_client_conn(struct rxrpc_net *rxnet,
				      struct rxrpc_connection *conn)
{
	unsigned int nr_conns;

	_enter("%d,%d", conn->debug_id, conn->cache_state);

	if (conn->cache_state == RXRPC_CONN_CLIENT_ACTIVE ||
	    conn->cache_state == RXRPC_CONN_CLIENT_UPGRADE)
		goto out;

	spin_lock(&rxnet->client_conn_cache_lock);

	nr_conns = rxnet->nr_client_conns;
	if (!test_and_set_bit(RXRPC_CONN_COUNTED, &conn->flags)) {
		trace_rxrpc_client(conn, -1, rxrpc_client_count);
		rxnet->nr_client_conns = nr_conns + 1;
	}

	switch (conn->cache_state) {
	case RXRPC_CONN_CLIENT_ACTIVE:
	case RXRPC_CONN_CLIENT_UPGRADE:
	case RXRPC_CONN_CLIENT_WAITING:
		break;

	case RXRPC_CONN_CLIENT_INACTIVE:
	case RXRPC_CONN_CLIENT_CULLED:
	case RXRPC_CONN_CLIENT_IDLE:
		if (nr_conns >= rxrpc_max_client_connections)
			goto wait_for_capacity;
		goto activate_conn;

	default:
		BUG();
	}

out_unlock:
	spin_unlock(&rxnet->client_conn_cache_lock);
out:
	_leave(" [%d]", conn->cache_state);
	return;

activate_conn:
	_debug("activate");
	rxrpc_activate_conn(rxnet, conn);
	goto out_unlock;

wait_for_capacity:
	_debug("wait");
	trace_rxrpc_client(conn, -1, rxrpc_client_to_waiting);
	conn->cache_state = RXRPC_CONN_CLIENT_WAITING;
	list_move_tail(&conn->cache_link, &rxnet->waiting_client_conns);
	goto out_unlock;
}

/*
 * Deactivate a channel.
 */
static void rxrpc_deactivate_one_channel(struct rxrpc_connection *conn,
					 unsigned int channel)
{
	struct rxrpc_channel *chan = &conn->channels[channel];

	rcu_assign_pointer(chan->call, NULL);
	conn->active_chans &= ~(1 << channel);
=======
	if (cp->upgrade)
		__set_bit(RXRPC_CALL_UPGRADE, &call->flags);

	/* Find the client connection bundle. */
	bundle = rxrpc_look_up_bundle(cp, gfp);
	if (!bundle)
		goto error;

	/* Get this call queued.  Someone else may activate it whilst we're
	 * lining up a new connection, but that's fine.
	 */
	spin_lock(&bundle->channel_lock);
	list_add_tail(&call->chan_wait_link, &bundle->waiting_calls);
	spin_unlock(&bundle->channel_lock);

	_leave(" = [B=%x]", bundle->debug_id);
	return bundle;

error:
	_leave(" = -ENOMEM");
	return ERR_PTR(-ENOMEM);
}

/*
 * Allocate a new connection and add it into a bundle.
 */
static void rxrpc_add_conn_to_bundle(struct rxrpc_bundle *bundle, gfp_t gfp)
	__releases(bundle->channel_lock)
{
	struct rxrpc_connection *candidate = NULL, *old = NULL;
	bool conflict;
	int i;

	_enter("");

	conflict = bundle->alloc_conn;
	if (!conflict)
		bundle->alloc_conn = true;
	spin_unlock(&bundle->channel_lock);
	if (conflict) {
		_leave(" [conf]");
		return;
	}

	candidate = rxrpc_alloc_client_connection(bundle, gfp);

	spin_lock(&bundle->channel_lock);
	bundle->alloc_conn = false;

	if (IS_ERR(candidate)) {
		bundle->alloc_error = PTR_ERR(candidate);
		spin_unlock(&bundle->channel_lock);
		_leave(" [err %ld]", PTR_ERR(candidate));
		return;
	}

	bundle->alloc_error = 0;

	for (i = 0; i < ARRAY_SIZE(bundle->conns); i++) {
		unsigned int shift = i * RXRPC_MAXCALLS;
		int j;

		old = bundle->conns[i];
		if (!rxrpc_may_reuse_conn(old)) {
			if (old)
				trace_rxrpc_client(old, -1, rxrpc_client_replace);
			candidate->bundle_shift = shift;
			bundle->conns[i] = candidate;
			for (j = 0; j < RXRPC_MAXCALLS; j++)
				set_bit(shift + j, &bundle->avail_chans);
			candidate = NULL;
			break;
		}

		old = NULL;
	}

	spin_unlock(&bundle->channel_lock);

	if (candidate) {
		_debug("discard C=%x", candidate->debug_id);
		trace_rxrpc_client(candidate, -1, rxrpc_client_duplicate);
		rxrpc_put_connection(candidate);
	}

	rxrpc_put_connection(old);
	_leave("");
}

/*
 * Add a connection to a bundle if there are no usable connections or we have
 * connections waiting for extra capacity.
 */
static void rxrpc_maybe_add_conn(struct rxrpc_bundle *bundle, gfp_t gfp)
{
	struct rxrpc_call *call;
	int i, usable;

	_enter("");

	spin_lock(&bundle->channel_lock);

	/* See if there are any usable connections. */
	usable = 0;
	for (i = 0; i < ARRAY_SIZE(bundle->conns); i++)
		if (rxrpc_may_reuse_conn(bundle->conns[i]))
			usable++;

	if (!usable && !list_empty(&bundle->waiting_calls)) {
		call = list_first_entry(&bundle->waiting_calls,
					struct rxrpc_call, chan_wait_link);
		if (test_bit(RXRPC_CALL_UPGRADE, &call->flags))
			bundle->try_upgrade = true;
	}

	if (!usable)
		goto alloc_conn;

	if (!bundle->avail_chans &&
	    !bundle->try_upgrade &&
	    !list_empty(&bundle->waiting_calls) &&
	    usable < ARRAY_SIZE(bundle->conns))
		goto alloc_conn;

	spin_unlock(&bundle->channel_lock);
	_leave("");
	return;

alloc_conn:
	return rxrpc_add_conn_to_bundle(bundle, gfp);
>>>>>>> upstream/android-13
}

/*
 * Assign a channel to the call at the front of the queue and wake the call up.
 * We don't increment the callNumber counter until this number has been exposed
 * to the world.
 */
static void rxrpc_activate_one_channel(struct rxrpc_connection *conn,
				       unsigned int channel)
{
	struct rxrpc_channel *chan = &conn->channels[channel];
<<<<<<< HEAD
	struct rxrpc_call *call = list_entry(conn->waiting_calls.next,
					     struct rxrpc_call, chan_wait_link);
	u32 call_id = chan->call_counter + 1;

=======
	struct rxrpc_bundle *bundle = conn->bundle;
	struct rxrpc_call *call = list_entry(bundle->waiting_calls.next,
					     struct rxrpc_call, chan_wait_link);
	u32 call_id = chan->call_counter + 1;

	_enter("C=%x,%u", conn->debug_id, channel);

>>>>>>> upstream/android-13
	trace_rxrpc_client(conn, channel, rxrpc_client_chan_activate);

	/* Cancel the final ACK on the previous call if it hasn't been sent yet
	 * as the DATA packet will implicitly ACK it.
	 */
	clear_bit(RXRPC_CONN_FINAL_ACK_0 + channel, &conn->flags);
<<<<<<< HEAD

	write_lock_bh(&call->state_lock);
	if (!test_bit(RXRPC_CALL_TX_LASTQ, &call->flags))
		call->state = RXRPC_CALL_CLIENT_SEND_REQUEST;
	else
		call->state = RXRPC_CALL_CLIENT_AWAIT_REPLY;
	write_unlock_bh(&call->state_lock);

	rxrpc_see_call(call);
	list_del_init(&call->chan_wait_link);
	conn->active_chans |= 1 << channel;
	call->peer	= rxrpc_get_peer(conn->params.peer);
	call->cid	= conn->proto.cid | channel;
	call->call_id	= call_id;
=======
	clear_bit(conn->bundle_shift + channel, &bundle->avail_chans);

	rxrpc_see_call(call);
	list_del_init(&call->chan_wait_link);
	call->peer	= rxrpc_get_peer(conn->params.peer);
	call->conn	= rxrpc_get_connection(conn);
	call->cid	= conn->proto.cid | channel;
	call->call_id	= call_id;
	call->security	= conn->security;
	call->security_ix = conn->security_ix;
	call->service_id = conn->service_id;
>>>>>>> upstream/android-13

	trace_rxrpc_connect_call(call);
	_net("CONNECT call %08x:%08x as call %d on conn %d",
	     call->cid, call->call_id, call->debug_id, conn->debug_id);

<<<<<<< HEAD
	/* Paired with the read barrier in rxrpc_wait_for_channel().  This
	 * orders cid and epoch in the connection wrt to call_id without the
	 * need to take the channel_lock.
=======
	write_lock_bh(&call->state_lock);
	call->state = RXRPC_CALL_CLIENT_SEND_REQUEST;
	write_unlock_bh(&call->state_lock);

	/* Paired with the read barrier in rxrpc_connect_call().  This orders
	 * cid and epoch in the connection wrt to call_id without the need to
	 * take the channel_lock.
>>>>>>> upstream/android-13
	 *
	 * We provisionally assign a callNumber at this point, but we don't
	 * confirm it until the call is about to be exposed.
	 *
	 * TODO: Pair with a barrier in the data_ready handler when that looks
	 * at the call ID through a connection channel.
	 */
	smp_wmb();
<<<<<<< HEAD
	chan->call_id	= call_id;
	chan->call_debug_id = call->debug_id;
=======

	chan->call_id		= call_id;
	chan->call_debug_id	= call->debug_id;
>>>>>>> upstream/android-13
	rcu_assign_pointer(chan->call, call);
	wake_up(&call->waitq);
}

/*
<<<<<<< HEAD
 * Assign channels and callNumbers to waiting calls with channel_lock
 * held by caller.
 */
static void rxrpc_activate_channels_locked(struct rxrpc_connection *conn)
{
	u8 avail, mask;

	switch (conn->cache_state) {
	case RXRPC_CONN_CLIENT_ACTIVE:
		mask = RXRPC_ACTIVE_CHANS_MASK;
		break;
	case RXRPC_CONN_CLIENT_UPGRADE:
		mask = 0x01;
		break;
	default:
		return;
	}

	while (!list_empty(&conn->waiting_calls) &&
	       (avail = ~conn->active_chans,
		avail &= mask,
		avail != 0))
		rxrpc_activate_one_channel(conn, __ffs(avail));
=======
 * Remove a connection from the idle list if it's on it.
 */
static void rxrpc_unidle_conn(struct rxrpc_bundle *bundle, struct rxrpc_connection *conn)
{
	struct rxrpc_net *rxnet = bundle->params.local->rxnet;
	bool drop_ref;

	if (!list_empty(&conn->cache_link)) {
		drop_ref = false;
		spin_lock(&rxnet->client_conn_cache_lock);
		if (!list_empty(&conn->cache_link)) {
			list_del_init(&conn->cache_link);
			drop_ref = true;
		}
		spin_unlock(&rxnet->client_conn_cache_lock);
		if (drop_ref)
			rxrpc_put_connection(conn);
	}
}

/*
 * Assign channels and callNumbers to waiting calls with channel_lock
 * held by caller.
 */
static void rxrpc_activate_channels_locked(struct rxrpc_bundle *bundle)
{
	struct rxrpc_connection *conn;
	unsigned long avail, mask;
	unsigned int channel, slot;

	if (bundle->try_upgrade)
		mask = 1;
	else
		mask = ULONG_MAX;

	while (!list_empty(&bundle->waiting_calls)) {
		avail = bundle->avail_chans & mask;
		if (!avail)
			break;
		channel = __ffs(avail);
		clear_bit(channel, &bundle->avail_chans);

		slot = channel / RXRPC_MAXCALLS;
		conn = bundle->conns[slot];
		if (!conn)
			break;

		if (bundle->try_upgrade)
			set_bit(RXRPC_CONN_PROBING_FOR_UPGRADE, &conn->flags);
		rxrpc_unidle_conn(bundle, conn);

		channel &= (RXRPC_MAXCALLS - 1);
		conn->act_chans	|= 1 << channel;
		rxrpc_activate_one_channel(conn, channel);
	}
>>>>>>> upstream/android-13
}

/*
 * Assign channels and callNumbers to waiting calls.
 */
<<<<<<< HEAD
static void rxrpc_activate_channels(struct rxrpc_connection *conn)
{
	_enter("%d", conn->debug_id);

	trace_rxrpc_client(conn, -1, rxrpc_client_activate_chans);

	if (conn->active_chans == RXRPC_ACTIVE_CHANS_MASK)
		return;

	spin_lock(&conn->channel_lock);
	rxrpc_activate_channels_locked(conn);
	spin_unlock(&conn->channel_lock);
=======
static void rxrpc_activate_channels(struct rxrpc_bundle *bundle)
{
	_enter("B=%x", bundle->debug_id);

	trace_rxrpc_client(NULL, -1, rxrpc_client_activate_chans);

	if (!bundle->avail_chans)
		return;

	spin_lock(&bundle->channel_lock);
	rxrpc_activate_channels_locked(bundle);
	spin_unlock(&bundle->channel_lock);
>>>>>>> upstream/android-13
	_leave("");
}

/*
 * Wait for a callNumber and a channel to be granted to a call.
 */
<<<<<<< HEAD
static int rxrpc_wait_for_channel(struct rxrpc_call *call, gfp_t gfp)
{
=======
static int rxrpc_wait_for_channel(struct rxrpc_bundle *bundle,
				  struct rxrpc_call *call, gfp_t gfp)
{
	DECLARE_WAITQUEUE(myself, current);
>>>>>>> upstream/android-13
	int ret = 0;

	_enter("%d", call->debug_id);

<<<<<<< HEAD
	if (!call->call_id) {
		DECLARE_WAITQUEUE(myself, current);

		if (!gfpflags_allow_blocking(gfp)) {
			ret = -EAGAIN;
			goto out;
		}

		add_wait_queue_exclusive(&call->waitq, &myself);
		for (;;) {
			set_current_state(TASK_INTERRUPTIBLE);
			if (call->call_id)
				break;
			if (signal_pending(current)) {
				ret = -ERESTARTSYS;
				break;
			}
			schedule();
		}
		remove_wait_queue(&call->waitq, &myself);
		__set_current_state(TASK_RUNNING);
	}

	/* Paired with the write barrier in rxrpc_activate_one_channel(). */
	smp_rmb();
=======
	if (!gfpflags_allow_blocking(gfp)) {
		rxrpc_maybe_add_conn(bundle, gfp);
		rxrpc_activate_channels(bundle);
		ret = bundle->alloc_error ?: -EAGAIN;
		goto out;
	}

	add_wait_queue_exclusive(&call->waitq, &myself);
	for (;;) {
		rxrpc_maybe_add_conn(bundle, gfp);
		rxrpc_activate_channels(bundle);
		ret = bundle->alloc_error;
		if (ret < 0)
			break;

		switch (call->interruptibility) {
		case RXRPC_INTERRUPTIBLE:
		case RXRPC_PREINTERRUPTIBLE:
			set_current_state(TASK_INTERRUPTIBLE);
			break;
		case RXRPC_UNINTERRUPTIBLE:
		default:
			set_current_state(TASK_UNINTERRUPTIBLE);
			break;
		}
		if (READ_ONCE(call->state) != RXRPC_CALL_CLIENT_AWAIT_CONN)
			break;
		if ((call->interruptibility == RXRPC_INTERRUPTIBLE ||
		     call->interruptibility == RXRPC_PREINTERRUPTIBLE) &&
		    signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
		schedule();
	}
	remove_wait_queue(&call->waitq, &myself);
	__set_current_state(TASK_RUNNING);
>>>>>>> upstream/android-13

out:
	_leave(" = %d", ret);
	return ret;
}

/*
 * find a connection for a call
 * - called in process context with IRQs enabled
 */
int rxrpc_connect_call(struct rxrpc_sock *rx,
		       struct rxrpc_call *call,
		       struct rxrpc_conn_parameters *cp,
		       struct sockaddr_rxrpc *srx,
		       gfp_t gfp)
{
<<<<<<< HEAD
	struct rxrpc_net *rxnet = cp->local->rxnet;
	int ret;
=======
	struct rxrpc_bundle *bundle;
	struct rxrpc_net *rxnet = cp->local->rxnet;
	int ret = 0;
>>>>>>> upstream/android-13

	_enter("{%d,%lx},", call->debug_id, call->user_call_ID);

	rxrpc_discard_expired_client_conns(&rxnet->client_conn_reaper);
<<<<<<< HEAD
	rxrpc_cull_active_client_conns(rxnet);

	ret = rxrpc_get_client_conn(rx, call, cp, srx, gfp);
	if (ret < 0)
		goto out;

	rxrpc_animate_client_conn(rxnet, call->conn);
	rxrpc_activate_channels(call->conn);

	ret = rxrpc_wait_for_channel(call, gfp);
	if (ret < 0) {
		trace_rxrpc_client(call->conn, ret, rxrpc_client_chan_wait_failed);
		rxrpc_disconnect_client_call(call);
		goto out;
	}

	spin_lock_bh(&call->conn->params.peer->lock);
	hlist_add_head_rcu(&call->error_link,
			   &call->conn->params.peer->error_targets);
	spin_unlock_bh(&call->conn->params.peer->lock);

out:
	_leave(" = %d", ret);
	return ret;
}

/*
 * Note that a connection is about to be exposed to the world.  Once it is
 * exposed, we maintain an extra ref on it that stops it from being summarily
 * discarded before it's (a) had a chance to deal with retransmission and (b)
 * had a chance at re-use (the per-connection security negotiation is
 * expensive).
 */
static void rxrpc_expose_client_conn(struct rxrpc_connection *conn,
				     unsigned int channel)
{
	if (!test_and_set_bit(RXRPC_CONN_EXPOSED, &conn->flags)) {
		trace_rxrpc_client(conn, channel, rxrpc_client_exposed);
		rxrpc_get_connection(conn);
	}
=======

	bundle = rxrpc_prep_call(rx, call, cp, srx, gfp);
	if (IS_ERR(bundle)) {
		ret = PTR_ERR(bundle);
		goto out;
	}

	if (call->state == RXRPC_CALL_CLIENT_AWAIT_CONN) {
		ret = rxrpc_wait_for_channel(bundle, call, gfp);
		if (ret < 0)
			goto wait_failed;
	}

granted_channel:
	/* Paired with the write barrier in rxrpc_activate_one_channel(). */
	smp_rmb();

out_put_bundle:
	rxrpc_put_bundle(bundle);
out:
	_leave(" = %d", ret);
	return ret;

wait_failed:
	spin_lock(&bundle->channel_lock);
	list_del_init(&call->chan_wait_link);
	spin_unlock(&bundle->channel_lock);

	if (call->state != RXRPC_CALL_CLIENT_AWAIT_CONN) {
		ret = 0;
		goto granted_channel;
	}

	trace_rxrpc_client(call->conn, ret, rxrpc_client_chan_wait_failed);
	rxrpc_set_call_completion(call, RXRPC_CALL_LOCAL_ERROR, 0, ret);
	rxrpc_disconnect_client_call(bundle, call);
	goto out_put_bundle;
>>>>>>> upstream/android-13
}

/*
 * Note that a call, and thus a connection, is about to be exposed to the
 * world.
 */
void rxrpc_expose_client_call(struct rxrpc_call *call)
{
	unsigned int channel = call->cid & RXRPC_CHANNELMASK;
	struct rxrpc_connection *conn = call->conn;
	struct rxrpc_channel *chan = &conn->channels[channel];

	if (!test_and_set_bit(RXRPC_CALL_EXPOSED, &call->flags)) {
		/* Mark the call ID as being used.  If the callNumber counter
		 * exceeds ~2 billion, we kill the connection after its
		 * outstanding calls have finished so that the counter doesn't
		 * wrap.
		 */
		chan->call_counter++;
		if (chan->call_counter >= INT_MAX)
			set_bit(RXRPC_CONN_DONT_REUSE, &conn->flags);
<<<<<<< HEAD
		rxrpc_expose_client_conn(conn, channel);
=======
		trace_rxrpc_client(conn, channel, rxrpc_client_exposed);
>>>>>>> upstream/android-13
	}
}

/*
 * Set the reap timer.
 */
static void rxrpc_set_client_reap_timer(struct rxrpc_net *rxnet)
{
<<<<<<< HEAD
	unsigned long now = jiffies;
	unsigned long reap_at = now + rxrpc_conn_idle_client_expiry;

	if (rxnet->live)
		timer_reduce(&rxnet->client_conn_reap_timer, reap_at);
=======
	if (!rxnet->kill_all_client_conns) {
		unsigned long now = jiffies;
		unsigned long reap_at = now + rxrpc_conn_idle_client_expiry;

		if (rxnet->live)
			timer_reduce(&rxnet->client_conn_reap_timer, reap_at);
	}
>>>>>>> upstream/android-13
}

/*
 * Disconnect a client call.
 */
<<<<<<< HEAD
void rxrpc_disconnect_client_call(struct rxrpc_call *call)
{
	struct rxrpc_connection *conn = call->conn;
	struct rxrpc_channel *chan = NULL;
	struct rxrpc_net *rxnet = conn->params.local->rxnet;
	unsigned int channel = -1;
	u32 cid;

	spin_lock(&conn->channel_lock);
	set_bit(RXRPC_CALL_DISCONNECTED, &call->flags);

	cid = call->cid;
	if (cid) {
		channel = cid & RXRPC_CHANNELMASK;
		chan = &conn->channels[channel];
	}
	trace_rxrpc_client(conn, channel, rxrpc_client_chan_disconnect);

	/* Calls that have never actually been assigned a channel can simply be
	 * discarded.  If the conn didn't get used either, it will follow
	 * immediately unless someone else grabs it in the meantime.
	 */
	if (!list_empty(&call->chan_wait_link)) {
=======
void rxrpc_disconnect_client_call(struct rxrpc_bundle *bundle, struct rxrpc_call *call)
{
	struct rxrpc_connection *conn;
	struct rxrpc_channel *chan = NULL;
	struct rxrpc_net *rxnet = bundle->params.local->rxnet;
	unsigned int channel;
	bool may_reuse;
	u32 cid;

	_enter("c=%x", call->debug_id);

	spin_lock(&bundle->channel_lock);
	set_bit(RXRPC_CALL_DISCONNECTED, &call->flags);

	/* Calls that have never actually been assigned a channel can simply be
	 * discarded.
	 */
	conn = call->conn;
	if (!conn) {
>>>>>>> upstream/android-13
		_debug("call is waiting");
		ASSERTCMP(call->call_id, ==, 0);
		ASSERT(!test_bit(RXRPC_CALL_EXPOSED, &call->flags));
		list_del_init(&call->chan_wait_link);
<<<<<<< HEAD

		trace_rxrpc_client(conn, channel, rxrpc_client_chan_unstarted);

		/* We must deactivate or idle the connection if it's now
		 * waiting for nothing.
		 */
		spin_lock(&rxnet->client_conn_cache_lock);
		if (conn->cache_state == RXRPC_CONN_CLIENT_WAITING &&
		    list_empty(&conn->waiting_calls) &&
		    !conn->active_chans)
			goto idle_connection;
		goto out;
	}

	if (rcu_access_pointer(chan->call) != call) {
		spin_unlock(&conn->channel_lock);
		BUG();
	}

=======
		goto out;
	}

	cid = call->cid;
	channel = cid & RXRPC_CHANNELMASK;
	chan = &conn->channels[channel];
	trace_rxrpc_client(conn, channel, rxrpc_client_chan_disconnect);

	if (rcu_access_pointer(chan->call) != call) {
		spin_unlock(&bundle->channel_lock);
		BUG();
	}

	may_reuse = rxrpc_may_reuse_conn(conn);

>>>>>>> upstream/android-13
	/* If a client call was exposed to the world, we save the result for
	 * retransmission.
	 *
	 * We use a barrier here so that the call number and abort code can be
	 * read without needing to take a lock.
	 *
	 * TODO: Make the incoming packet handler check this and handle
	 * terminal retransmission without requiring access to the call.
	 */
	if (test_bit(RXRPC_CALL_EXPOSED, &call->flags)) {
		_debug("exposed %u,%u", call->call_id, call->abort_code);
		__rxrpc_disconnect_call(conn, call);
<<<<<<< HEAD
	}

	/* See if we can pass the channel directly to another call. */
	if (conn->cache_state == RXRPC_CONN_CLIENT_ACTIVE &&
	    !list_empty(&conn->waiting_calls)) {
		trace_rxrpc_client(conn, channel, rxrpc_client_chan_pass);
		rxrpc_activate_one_channel(conn, channel);
		goto out_2;
=======

		if (test_and_clear_bit(RXRPC_CONN_PROBING_FOR_UPGRADE, &conn->flags)) {
			trace_rxrpc_client(conn, channel, rxrpc_client_to_active);
			bundle->try_upgrade = false;
			if (may_reuse)
				rxrpc_activate_channels_locked(bundle);
		}

	}

	/* See if we can pass the channel directly to another call. */
	if (may_reuse && !list_empty(&bundle->waiting_calls)) {
		trace_rxrpc_client(conn, channel, rxrpc_client_chan_pass);
		rxrpc_activate_one_channel(conn, channel);
		goto out;
>>>>>>> upstream/android-13
	}

	/* Schedule the final ACK to be transmitted in a short while so that it
	 * can be skipped if we find a follow-on call.  The first DATA packet
	 * of the follow on call will implicitly ACK this call.
	 */
	if (call->completion == RXRPC_CALL_SUCCEEDED &&
	    test_bit(RXRPC_CALL_EXPOSED, &call->flags)) {
		unsigned long final_ack_at = jiffies + 2;

		WRITE_ONCE(chan->final_ack_at, final_ack_at);
		smp_wmb(); /* vs rxrpc_process_delayed_final_acks() */
		set_bit(RXRPC_CONN_FINAL_ACK_0 + channel, &conn->flags);
		rxrpc_reduce_conn_timer(conn, final_ack_at);
	}

<<<<<<< HEAD
	/* Things are more complex and we need the cache lock.  We might be
	 * able to simply idle the conn or it might now be lurking on the wait
	 * list.  It might even get moved back to the active list whilst we're
	 * waiting for the lock.
	 */
	spin_lock(&rxnet->client_conn_cache_lock);

	switch (conn->cache_state) {
	case RXRPC_CONN_CLIENT_UPGRADE:
		/* Deal with termination of a service upgrade probe. */
		if (test_bit(RXRPC_CONN_EXPOSED, &conn->flags)) {
			clear_bit(RXRPC_CONN_PROBING_FOR_UPGRADE, &conn->flags);
			trace_rxrpc_client(conn, channel, rxrpc_client_to_active);
			conn->cache_state = RXRPC_CONN_CLIENT_ACTIVE;
			rxrpc_activate_channels_locked(conn);
		}
		/* fall through */
	case RXRPC_CONN_CLIENT_ACTIVE:
		if (list_empty(&conn->waiting_calls)) {
			rxrpc_deactivate_one_channel(conn, channel);
			if (!conn->active_chans) {
				rxnet->nr_active_client_conns--;
				goto idle_connection;
			}
			goto out;
		}

		trace_rxrpc_client(conn, channel, rxrpc_client_chan_pass);
		rxrpc_activate_one_channel(conn, channel);
		goto out;

	case RXRPC_CONN_CLIENT_CULLED:
		rxrpc_deactivate_one_channel(conn, channel);
		ASSERT(list_empty(&conn->waiting_calls));
		if (!conn->active_chans)
			goto idle_connection;
		goto out;

	case RXRPC_CONN_CLIENT_WAITING:
		rxrpc_deactivate_one_channel(conn, channel);
		goto out;

	default:
		BUG();
	}

out:
	spin_unlock(&rxnet->client_conn_cache_lock);
out_2:
	spin_unlock(&conn->channel_lock);
	_leave("");
	return;

idle_connection:
	/* As no channels remain active, the connection gets deactivated
	 * immediately or moved to the idle list for a short while.
	 */
	if (test_bit(RXRPC_CONN_EXPOSED, &conn->flags)) {
		trace_rxrpc_client(conn, channel, rxrpc_client_to_idle);
		conn->idle_timestamp = jiffies;
		conn->cache_state = RXRPC_CONN_CLIENT_IDLE;
		list_move_tail(&conn->cache_link, &rxnet->idle_client_conns);
		if (rxnet->idle_client_conns.next == &conn->cache_link &&
		    !rxnet->kill_all_client_conns)
			rxrpc_set_client_reap_timer(rxnet);
	} else {
		trace_rxrpc_client(conn, channel, rxrpc_client_to_inactive);
		conn->cache_state = RXRPC_CONN_CLIENT_INACTIVE;
		list_del_init(&conn->cache_link);
	}
	goto out;
=======
	/* Deactivate the channel. */
	rcu_assign_pointer(chan->call, NULL);
	set_bit(conn->bundle_shift + channel, &conn->bundle->avail_chans);
	conn->act_chans	&= ~(1 << channel);

	/* If no channels remain active, then put the connection on the idle
	 * list for a short while.  Give it a ref to stop it going away if it
	 * becomes unbundled.
	 */
	if (!conn->act_chans) {
		trace_rxrpc_client(conn, channel, rxrpc_client_to_idle);
		conn->idle_timestamp = jiffies;

		rxrpc_get_connection(conn);
		spin_lock(&rxnet->client_conn_cache_lock);
		list_move_tail(&conn->cache_link, &rxnet->idle_client_conns);
		spin_unlock(&rxnet->client_conn_cache_lock);

		rxrpc_set_client_reap_timer(rxnet);
	}

out:
	spin_unlock(&bundle->channel_lock);
	_leave("");
	return;
}

/*
 * Remove a connection from a bundle.
 */
static void rxrpc_unbundle_conn(struct rxrpc_connection *conn)
{
	struct rxrpc_bundle *bundle = conn->bundle;
	struct rxrpc_local *local = bundle->params.local;
	unsigned int bindex;
	bool need_drop = false, need_put = false;
	int i;

	_enter("C=%x", conn->debug_id);

	if (conn->flags & RXRPC_CONN_FINAL_ACK_MASK)
		rxrpc_process_delayed_final_acks(conn, true);

	spin_lock(&bundle->channel_lock);
	bindex = conn->bundle_shift / RXRPC_MAXCALLS;
	if (bundle->conns[bindex] == conn) {
		_debug("clear slot %u", bindex);
		bundle->conns[bindex] = NULL;
		for (i = 0; i < RXRPC_MAXCALLS; i++)
			clear_bit(conn->bundle_shift + i, &bundle->avail_chans);
		need_drop = true;
	}
	spin_unlock(&bundle->channel_lock);

	/* If there are no more connections, remove the bundle */
	if (!bundle->avail_chans) {
		_debug("maybe unbundle");
		spin_lock(&local->client_bundles_lock);

		for (i = 0; i < ARRAY_SIZE(bundle->conns); i++)
			if (bundle->conns[i])
				break;
		if (i == ARRAY_SIZE(bundle->conns) && !bundle->params.exclusive) {
			_debug("erase bundle");
			rb_erase(&bundle->local_node, &local->client_bundles);
			need_put = true;
		}

		spin_unlock(&local->client_bundles_lock);
		if (need_put)
			rxrpc_put_bundle(bundle);
	}

	if (need_drop)
		rxrpc_put_connection(conn);
	_leave("");
>>>>>>> upstream/android-13
}

/*
 * Clean up a dead client connection.
 */
<<<<<<< HEAD
static struct rxrpc_connection *
rxrpc_put_one_client_conn(struct rxrpc_connection *conn)
{
	struct rxrpc_connection *next = NULL;
	struct rxrpc_local *local = conn->params.local;
	struct rxrpc_net *rxnet = local->rxnet;
	unsigned int nr_conns;

	trace_rxrpc_client(conn, -1, rxrpc_client_cleanup);

	if (test_bit(RXRPC_CONN_IN_CLIENT_CONNS, &conn->flags)) {
		spin_lock(&local->client_conns_lock);
		if (test_and_clear_bit(RXRPC_CONN_IN_CLIENT_CONNS,
				       &conn->flags))
			rb_erase(&conn->client_node, &local->client_conns);
		spin_unlock(&local->client_conns_lock);
	}

	rxrpc_put_client_connection_id(conn);

	ASSERTCMP(conn->cache_state, ==, RXRPC_CONN_CLIENT_INACTIVE);

	if (test_bit(RXRPC_CONN_COUNTED, &conn->flags)) {
		trace_rxrpc_client(conn, -1, rxrpc_client_uncount);
		spin_lock(&rxnet->client_conn_cache_lock);
		nr_conns = --rxnet->nr_client_conns;

		if (nr_conns < rxrpc_max_client_connections &&
		    !list_empty(&rxnet->waiting_client_conns)) {
			next = list_entry(rxnet->waiting_client_conns.next,
					  struct rxrpc_connection, cache_link);
			rxrpc_get_connection(next);
			rxrpc_activate_conn(rxnet, next);
		}

		spin_unlock(&rxnet->client_conn_cache_lock);
	}

	rxrpc_kill_connection(conn);
	if (next)
		rxrpc_activate_channels(next);

	/* We need to get rid of the temporary ref we took upon next, but we
	 * can't call rxrpc_put_connection() recursively.
	 */
	return next;
=======
static void rxrpc_kill_client_conn(struct rxrpc_connection *conn)
{
	struct rxrpc_local *local = conn->params.local;
	struct rxrpc_net *rxnet = local->rxnet;

	_enter("C=%x", conn->debug_id);

	trace_rxrpc_client(conn, -1, rxrpc_client_cleanup);
	atomic_dec(&rxnet->nr_client_conns);

	rxrpc_put_client_connection_id(conn);
	rxrpc_kill_connection(conn);
>>>>>>> upstream/android-13
}

/*
 * Clean up a dead client connections.
 */
void rxrpc_put_client_conn(struct rxrpc_connection *conn)
{
	const void *here = __builtin_return_address(0);
	unsigned int debug_id = conn->debug_id;
	int n;

<<<<<<< HEAD
	do {
		n = atomic_dec_return(&conn->usage);
		trace_rxrpc_conn(debug_id, rxrpc_conn_put_client, n, here);
		if (n > 0)
			return;
		ASSERTCMP(n, >=, 0);

		conn = rxrpc_put_one_client_conn(conn);
	} while (conn);
}

/*
 * Kill the longest-active client connections to make room for new ones.
 */
static void rxrpc_cull_active_client_conns(struct rxrpc_net *rxnet)
{
	struct rxrpc_connection *conn;
	unsigned int nr_conns = rxnet->nr_client_conns;
	unsigned int nr_active, limit;

	_enter("");

	ASSERTCMP(nr_conns, >=, 0);
	if (nr_conns < rxrpc_max_client_connections) {
		_leave(" [ok]");
		return;
	}
	limit = rxrpc_reap_client_connections;

	spin_lock(&rxnet->client_conn_cache_lock);
	nr_active = rxnet->nr_active_client_conns;

	while (nr_active > limit) {
		ASSERT(!list_empty(&rxnet->active_client_conns));
		conn = list_entry(rxnet->active_client_conns.next,
				  struct rxrpc_connection, cache_link);
		ASSERTIFCMP(conn->cache_state != RXRPC_CONN_CLIENT_ACTIVE,
			    conn->cache_state, ==, RXRPC_CONN_CLIENT_UPGRADE);

		if (list_empty(&conn->waiting_calls)) {
			trace_rxrpc_client(conn, -1, rxrpc_client_to_culled);
			conn->cache_state = RXRPC_CONN_CLIENT_CULLED;
			list_del_init(&conn->cache_link);
		} else {
			trace_rxrpc_client(conn, -1, rxrpc_client_to_waiting);
			conn->cache_state = RXRPC_CONN_CLIENT_WAITING;
			list_move_tail(&conn->cache_link,
				       &rxnet->waiting_client_conns);
		}

		nr_active--;
	}

	rxnet->nr_active_client_conns = nr_active;
	spin_unlock(&rxnet->client_conn_cache_lock);
	ASSERTCMP(nr_active, >=, 0);
	_leave(" [culled]");
=======
	n = atomic_dec_return(&conn->usage);
	trace_rxrpc_conn(debug_id, rxrpc_conn_put_client, n, here);
	if (n <= 0) {
		ASSERTCMP(n, >=, 0);
		rxrpc_kill_client_conn(conn);
	}
>>>>>>> upstream/android-13
}

/*
 * Discard expired client connections from the idle list.  Each conn in the
 * idle list has been exposed and holds an extra ref because of that.
 *
 * This may be called from conn setup or from a work item so cannot be
 * considered non-reentrant.
 */
void rxrpc_discard_expired_client_conns(struct work_struct *work)
{
	struct rxrpc_connection *conn;
	struct rxrpc_net *rxnet =
		container_of(work, struct rxrpc_net, client_conn_reaper);
	unsigned long expiry, conn_expires_at, now;
	unsigned int nr_conns;

	_enter("");

	if (list_empty(&rxnet->idle_client_conns)) {
		_leave(" [empty]");
		return;
	}

	/* Don't double up on the discarding */
	if (!spin_trylock(&rxnet->client_conn_discard_lock)) {
		_leave(" [already]");
		return;
	}

	/* We keep an estimate of what the number of conns ought to be after
	 * we've discarded some so that we don't overdo the discarding.
	 */
<<<<<<< HEAD
	nr_conns = rxnet->nr_client_conns;
=======
	nr_conns = atomic_read(&rxnet->nr_client_conns);
>>>>>>> upstream/android-13

next:
	spin_lock(&rxnet->client_conn_cache_lock);

	if (list_empty(&rxnet->idle_client_conns))
		goto out;

	conn = list_entry(rxnet->idle_client_conns.next,
			  struct rxrpc_connection, cache_link);
<<<<<<< HEAD
	ASSERT(test_bit(RXRPC_CONN_EXPOSED, &conn->flags));
=======
>>>>>>> upstream/android-13

	if (!rxnet->kill_all_client_conns) {
		/* If the number of connections is over the reap limit, we
		 * expedite discard by reducing the expiry timeout.  We must,
		 * however, have at least a short grace period to be able to do
		 * final-ACK or ABORT retransmission.
		 */
		expiry = rxrpc_conn_idle_client_expiry;
		if (nr_conns > rxrpc_reap_client_connections)
			expiry = rxrpc_conn_idle_client_fast_expiry;
		if (conn->params.local->service_closed)
			expiry = rxrpc_closed_conn_expiry * HZ;

		conn_expires_at = conn->idle_timestamp + expiry;

		now = READ_ONCE(jiffies);
		if (time_after(conn_expires_at, now))
			goto not_yet_expired;
	}

	trace_rxrpc_client(conn, -1, rxrpc_client_discard);
<<<<<<< HEAD
	if (!test_and_clear_bit(RXRPC_CONN_EXPOSED, &conn->flags))
		BUG();
	conn->cache_state = RXRPC_CONN_CLIENT_INACTIVE;
=======
>>>>>>> upstream/android-13
	list_del_init(&conn->cache_link);

	spin_unlock(&rxnet->client_conn_cache_lock);

<<<<<<< HEAD
	/* When we cleared the EXPOSED flag, we took on responsibility for the
	 * reference that that had on the usage count.  We deal with that here.
	 * If someone re-sets the flag and re-gets the ref, that's fine.
	 */
	rxrpc_put_connection(conn);
=======
	rxrpc_unbundle_conn(conn);
	rxrpc_put_connection(conn); /* Drop the ->cache_link ref */

>>>>>>> upstream/android-13
	nr_conns--;
	goto next;

not_yet_expired:
	/* The connection at the front of the queue hasn't yet expired, so
	 * schedule the work item for that point if we discarded something.
	 *
	 * We don't worry if the work item is already scheduled - it can look
	 * after rescheduling itself at a later time.  We could cancel it, but
	 * then things get messier.
	 */
	_debug("not yet");
	if (!rxnet->kill_all_client_conns)
<<<<<<< HEAD
		timer_reduce(&rxnet->client_conn_reap_timer,
			     conn_expires_at);
=======
		timer_reduce(&rxnet->client_conn_reap_timer, conn_expires_at);
>>>>>>> upstream/android-13

out:
	spin_unlock(&rxnet->client_conn_cache_lock);
	spin_unlock(&rxnet->client_conn_discard_lock);
	_leave("");
}

/*
 * Preemptively destroy all the client connection records rather than waiting
 * for them to time out
 */
void rxrpc_destroy_all_client_connections(struct rxrpc_net *rxnet)
{
	_enter("");

	spin_lock(&rxnet->client_conn_cache_lock);
	rxnet->kill_all_client_conns = true;
	spin_unlock(&rxnet->client_conn_cache_lock);

	del_timer_sync(&rxnet->client_conn_reap_timer);

	if (!rxrpc_queue_work(&rxnet->client_conn_reaper))
		_debug("destroy: queue failed");

	_leave("");
}

/*
 * Clean up the client connections on a local endpoint.
 */
void rxrpc_clean_up_local_conns(struct rxrpc_local *local)
{
	struct rxrpc_connection *conn, *tmp;
	struct rxrpc_net *rxnet = local->rxnet;
<<<<<<< HEAD
	unsigned int nr_active;
=======
>>>>>>> upstream/android-13
	LIST_HEAD(graveyard);

	_enter("");

	spin_lock(&rxnet->client_conn_cache_lock);
<<<<<<< HEAD
	nr_active = rxnet->nr_active_client_conns;
=======
>>>>>>> upstream/android-13

	list_for_each_entry_safe(conn, tmp, &rxnet->idle_client_conns,
				 cache_link) {
		if (conn->params.local == local) {
<<<<<<< HEAD
			ASSERTCMP(conn->cache_state, ==, RXRPC_CONN_CLIENT_IDLE);

			trace_rxrpc_client(conn, -1, rxrpc_client_discard);
			if (!test_and_clear_bit(RXRPC_CONN_EXPOSED, &conn->flags))
				BUG();
			conn->cache_state = RXRPC_CONN_CLIENT_INACTIVE;
			list_move(&conn->cache_link, &graveyard);
			nr_active--;
		}
	}

	rxnet->nr_active_client_conns = nr_active;
	spin_unlock(&rxnet->client_conn_cache_lock);
	ASSERTCMP(nr_active, >=, 0);
=======
			trace_rxrpc_client(conn, -1, rxrpc_client_discard);
			list_move(&conn->cache_link, &graveyard);
		}
	}

	spin_unlock(&rxnet->client_conn_cache_lock);
>>>>>>> upstream/android-13

	while (!list_empty(&graveyard)) {
		conn = list_entry(graveyard.next,
				  struct rxrpc_connection, cache_link);
		list_del_init(&conn->cache_link);
<<<<<<< HEAD

=======
		rxrpc_unbundle_conn(conn);
>>>>>>> upstream/android-13
		rxrpc_put_connection(conn);
	}

	_leave(" [culled]");
}
