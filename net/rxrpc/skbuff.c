<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* ar-skbuff.c: socket buffer destruction handling
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

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <net/af_rxrpc.h>
#include "ar-internal.h"

<<<<<<< HEAD
#define select_skb_count(op) (op >= rxrpc_skb_tx_cleaned ? &rxrpc_n_tx_skbs : &rxrpc_n_rx_skbs)
=======
#define is_tx_skb(skb) (rxrpc_skb(skb)->rx_flags & RXRPC_SKB_TX_BUFFER)
#define select_skb_count(skb) (is_tx_skb(skb) ? &rxrpc_n_tx_skbs : &rxrpc_n_rx_skbs)
>>>>>>> upstream/android-13

/*
 * Note the allocation or reception of a socket buffer.
 */
void rxrpc_new_skb(struct sk_buff *skb, enum rxrpc_skb_trace op)
{
	const void *here = __builtin_return_address(0);
<<<<<<< HEAD
	int n = atomic_inc_return(select_skb_count(op));
	trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n, here);
=======
	int n = atomic_inc_return(select_skb_count(skb));
	trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n,
			rxrpc_skb(skb)->rx_flags, here);
>>>>>>> upstream/android-13
}

/*
 * Note the re-emergence of a socket buffer from a queue or buffer.
 */
void rxrpc_see_skb(struct sk_buff *skb, enum rxrpc_skb_trace op)
{
	const void *here = __builtin_return_address(0);
	if (skb) {
<<<<<<< HEAD
		int n = atomic_read(select_skb_count(op));
		trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n, here);
=======
		int n = atomic_read(select_skb_count(skb));
		trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n,
				rxrpc_skb(skb)->rx_flags, here);
>>>>>>> upstream/android-13
	}
}

/*
 * Note the addition of a ref on a socket buffer.
 */
void rxrpc_get_skb(struct sk_buff *skb, enum rxrpc_skb_trace op)
{
	const void *here = __builtin_return_address(0);
<<<<<<< HEAD
	int n = atomic_inc_return(select_skb_count(op));
	trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n, here);
=======
	int n = atomic_inc_return(select_skb_count(skb));
	trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n,
			rxrpc_skb(skb)->rx_flags, here);
>>>>>>> upstream/android-13
	skb_get(skb);
}

/*
<<<<<<< HEAD
=======
 * Note the dropping of a ref on a socket buffer by the core.
 */
void rxrpc_eaten_skb(struct sk_buff *skb, enum rxrpc_skb_trace op)
{
	const void *here = __builtin_return_address(0);
	int n = atomic_inc_return(&rxrpc_n_rx_skbs);
	trace_rxrpc_skb(skb, op, 0, n, 0, here);
}

/*
>>>>>>> upstream/android-13
 * Note the destruction of a socket buffer.
 */
void rxrpc_free_skb(struct sk_buff *skb, enum rxrpc_skb_trace op)
{
	const void *here = __builtin_return_address(0);
	if (skb) {
		int n;
		CHECK_SLAB_OKAY(&skb->users);
<<<<<<< HEAD
		n = atomic_dec_return(select_skb_count(op));
		trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n, here);
		kfree_skb(skb);
	}
}

/*
 * Note the injected loss of a socket buffer.
 */
void rxrpc_lose_skb(struct sk_buff *skb, enum rxrpc_skb_trace op)
{
	const void *here = __builtin_return_address(0);
	if (skb) {
		int n;
		CHECK_SLAB_OKAY(&skb->users);
		n = atomic_dec_return(select_skb_count(op));
		trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n, here);
=======
		n = atomic_dec_return(select_skb_count(skb));
		trace_rxrpc_skb(skb, op, refcount_read(&skb->users), n,
				rxrpc_skb(skb)->rx_flags, here);
>>>>>>> upstream/android-13
		kfree_skb(skb);
	}
}

/*
 * Clear a queue of socket buffers.
 */
void rxrpc_purge_queue(struct sk_buff_head *list)
{
	const void *here = __builtin_return_address(0);
	struct sk_buff *skb;
	while ((skb = skb_dequeue((list))) != NULL) {
<<<<<<< HEAD
		int n = atomic_dec_return(select_skb_count(rxrpc_skb_rx_purged));
		trace_rxrpc_skb(skb, rxrpc_skb_rx_purged,
				refcount_read(&skb->users), n, here);
=======
		int n = atomic_dec_return(select_skb_count(skb));
		trace_rxrpc_skb(skb, rxrpc_skb_purged,
				refcount_read(&skb->users), n,
				rxrpc_skb(skb)->rx_flags, here);
>>>>>>> upstream/android-13
		kfree_skb(skb);
	}
}
