<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* RxRPC security handling
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

#include <linux/module.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/crypto.h>
#include <net/sock.h>
#include <net/af_rxrpc.h>
#include <keys/rxrpc-type.h>
#include "ar-internal.h"

static const struct rxrpc_security *rxrpc_security_types[] = {
	[RXRPC_SECURITY_NONE]	= &rxrpc_no_security,
#ifdef CONFIG_RXKAD
	[RXRPC_SECURITY_RXKAD]	= &rxkad,
#endif
};

int __init rxrpc_init_security(void)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(rxrpc_security_types); i++) {
		if (rxrpc_security_types[i]) {
			ret = rxrpc_security_types[i]->init();
			if (ret < 0)
				goto failed;
		}
	}

	return 0;

failed:
	for (i--; i >= 0; i--)
		if (rxrpc_security_types[i])
			rxrpc_security_types[i]->exit();
	return ret;
}

void rxrpc_exit_security(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(rxrpc_security_types); i++)
		if (rxrpc_security_types[i])
			rxrpc_security_types[i]->exit();
}

/*
 * look up an rxrpc security module
 */
<<<<<<< HEAD
static const struct rxrpc_security *rxrpc_security_lookup(u8 security_index)
=======
const struct rxrpc_security *rxrpc_security_lookup(u8 security_index)
>>>>>>> upstream/android-13
{
	if (security_index >= ARRAY_SIZE(rxrpc_security_types))
		return NULL;
	return rxrpc_security_types[security_index];
}

/*
 * initialise the security on a client connection
 */
int rxrpc_init_client_conn_security(struct rxrpc_connection *conn)
{
	const struct rxrpc_security *sec;
	struct rxrpc_key_token *token;
	struct key *key = conn->params.key;
	int ret;

	_enter("{%d},{%x}", conn->debug_id, key_serial(key));

	if (!key)
		return 0;

	ret = key_validate(key);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	token = key->payload.data[0];
	if (!token)
		return -EKEYREJECTED;

	sec = rxrpc_security_lookup(token->security_index);
	if (!sec)
		return -EKEYREJECTED;
	conn->security = sec;

	ret = conn->security->init_connection_security(conn);
=======
	for (token = key->payload.data[0]; token; token = token->next) {
		sec = rxrpc_security_lookup(token->security_index);
		if (sec)
			goto found;
	}
	return -EKEYREJECTED;

found:
	conn->security = sec;

	ret = conn->security->init_connection_security(conn, token);
>>>>>>> upstream/android-13
	if (ret < 0) {
		conn->security = &rxrpc_no_security;
		return ret;
	}

	_leave(" = 0");
	return 0;
}

/*
<<<<<<< HEAD
 * initialise the security on a server connection
 */
int rxrpc_init_server_conn_security(struct rxrpc_connection *conn)
{
	const struct rxrpc_security *sec;
	struct rxrpc_local *local = conn->params.local;
	struct rxrpc_sock *rx;
	struct key *key;
	key_ref_t kref;
	char kdesc[5 + 1 + 3 + 1];

	_enter("");

	sprintf(kdesc, "%u:%u", conn->service_id, conn->security_ix);

	sec = rxrpc_security_lookup(conn->security_ix);
	if (!sec) {
		_leave(" = -ENOKEY [lookup]");
		return -ENOKEY;
	}

	/* find the service */
	read_lock(&local->services_lock);
	rx = rcu_dereference_protected(local->service,
				       lockdep_is_held(&local->services_lock));
	if (rx && (rx->srx.srx_service == conn->service_id ||
		   rx->second_service == conn->service_id))
		goto found_service;

	/* the service appears to have died */
	read_unlock(&local->services_lock);
	_leave(" = -ENOENT");
	return -ENOENT;

found_service:
	if (!rx->securities) {
		read_unlock(&local->services_lock);
		_leave(" = -ENOKEY");
		return -ENOKEY;
	}

	/* look through the service's keyring */
	kref = keyring_search(make_key_ref(rx->securities, 1UL),
			      &key_type_rxrpc_s, kdesc);
	if (IS_ERR(kref)) {
		read_unlock(&local->services_lock);
		_leave(" = %ld [search]", PTR_ERR(kref));
		return PTR_ERR(kref);
	}

	key = key_ref_to_ptr(kref);
	read_unlock(&local->services_lock);

	conn->server_key = key;
	conn->security = sec;

	_leave(" = 0");
	return 0;
=======
 * Set the ops a server connection.
 */
const struct rxrpc_security *rxrpc_get_incoming_security(struct rxrpc_sock *rx,
							 struct sk_buff *skb)
{
	const struct rxrpc_security *sec;
	struct rxrpc_skb_priv *sp = rxrpc_skb(skb);

	_enter("");

	sec = rxrpc_security_lookup(sp->hdr.securityIndex);
	if (!sec) {
		trace_rxrpc_abort(0, "SVS",
				  sp->hdr.cid, sp->hdr.callNumber, sp->hdr.seq,
				  RX_INVALID_OPERATION, EKEYREJECTED);
		skb->mark = RXRPC_SKB_MARK_REJECT_ABORT;
		skb->priority = RX_INVALID_OPERATION;
		return NULL;
	}

	if (sp->hdr.securityIndex != RXRPC_SECURITY_NONE &&
	    !rx->securities) {
		trace_rxrpc_abort(0, "SVR",
				  sp->hdr.cid, sp->hdr.callNumber, sp->hdr.seq,
				  RX_INVALID_OPERATION, EKEYREJECTED);
		skb->mark = RXRPC_SKB_MARK_REJECT_ABORT;
		skb->priority = sec->no_key_abort;
		return NULL;
	}

	return sec;
}

/*
 * Find the security key for a server connection.
 */
struct key *rxrpc_look_up_server_security(struct rxrpc_connection *conn,
					  struct sk_buff *skb,
					  u32 kvno, u32 enctype)
{
	struct rxrpc_skb_priv *sp = rxrpc_skb(skb);
	struct rxrpc_sock *rx;
	struct key *key = ERR_PTR(-EKEYREJECTED);
	key_ref_t kref = NULL;
	char kdesc[5 + 1 + 3 + 1 + 12 + 1 + 12 + 1];
	int ret;

	_enter("");

	if (enctype)
		sprintf(kdesc, "%u:%u:%u:%u",
			sp->hdr.serviceId, sp->hdr.securityIndex, kvno, enctype);
	else if (kvno)
		sprintf(kdesc, "%u:%u:%u",
			sp->hdr.serviceId, sp->hdr.securityIndex, kvno);
	else
		sprintf(kdesc, "%u:%u",
			sp->hdr.serviceId, sp->hdr.securityIndex);

	rcu_read_lock();

	rx = rcu_dereference(conn->params.local->service);
	if (!rx)
		goto out;

	/* look through the service's keyring */
	kref = keyring_search(make_key_ref(rx->securities, 1UL),
			      &key_type_rxrpc_s, kdesc, true);
	if (IS_ERR(kref)) {
		key = ERR_CAST(kref);
		goto out;
	}

	key = key_ref_to_ptr(kref);

	ret = key_validate(key);
	if (ret < 0) {
		key_put(key);
		key = ERR_PTR(ret);
		goto out;
	}

out:
	rcu_read_unlock();
	return key;
>>>>>>> upstream/android-13
}
