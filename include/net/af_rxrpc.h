<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/* RxRPC kernel service interface definitions
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

#ifndef _NET_RXRPC_H
#define _NET_RXRPC_H

#include <linux/rxrpc.h>
<<<<<<< HEAD
=======
#include <linux/ktime.h>
>>>>>>> upstream/android-13

struct key;
struct sock;
struct socket;
struct rxrpc_call;

<<<<<<< HEAD
/*
 * Call completion condition (state == RXRPC_CALL_COMPLETE).
 */
enum rxrpc_call_completion {
	RXRPC_CALL_SUCCEEDED,		/* - Normal termination */
	RXRPC_CALL_REMOTELY_ABORTED,	/* - call aborted by peer */
	RXRPC_CALL_LOCALLY_ABORTED,	/* - call aborted locally on error or close */
	RXRPC_CALL_LOCAL_ERROR,		/* - call failed due to local error */
	RXRPC_CALL_NETWORK_ERROR,	/* - call terminated by network error */
	NR__RXRPC_CALL_COMPLETIONS
=======
enum rxrpc_interruptibility {
	RXRPC_INTERRUPTIBLE,	/* Call is interruptible */
	RXRPC_PREINTERRUPTIBLE,	/* Call can be cancelled whilst waiting for a slot */
	RXRPC_UNINTERRUPTIBLE,	/* Call should not be interruptible at all */
>>>>>>> upstream/android-13
};

/*
 * Debug ID counter for tracing.
 */
extern atomic_t rxrpc_debug_id;

typedef void (*rxrpc_notify_rx_t)(struct sock *, struct rxrpc_call *,
				  unsigned long);
typedef void (*rxrpc_notify_end_tx_t)(struct sock *, struct rxrpc_call *,
				      unsigned long);
typedef void (*rxrpc_notify_new_call_t)(struct sock *, struct rxrpc_call *,
					unsigned long);
typedef void (*rxrpc_discard_new_call_t)(struct rxrpc_call *, unsigned long);
typedef void (*rxrpc_user_attach_call_t)(struct rxrpc_call *, unsigned long);

void rxrpc_kernel_new_call_notification(struct socket *,
					rxrpc_notify_new_call_t,
					rxrpc_discard_new_call_t);
struct rxrpc_call *rxrpc_kernel_begin_call(struct socket *,
					   struct sockaddr_rxrpc *,
					   struct key *,
					   unsigned long,
					   s64,
					   gfp_t,
					   rxrpc_notify_rx_t,
					   bool,
<<<<<<< HEAD
=======
					   enum rxrpc_interruptibility,
>>>>>>> upstream/android-13
					   unsigned int);
int rxrpc_kernel_send_data(struct socket *, struct rxrpc_call *,
			   struct msghdr *, size_t,
			   rxrpc_notify_end_tx_t);
int rxrpc_kernel_recv_data(struct socket *, struct rxrpc_call *,
<<<<<<< HEAD
			   struct iov_iter *, bool, u32 *, u16 *);
=======
			   struct iov_iter *, size_t *, bool, u32 *, u16 *);
>>>>>>> upstream/android-13
bool rxrpc_kernel_abort_call(struct socket *, struct rxrpc_call *,
			     u32, int, const char *);
void rxrpc_kernel_end_call(struct socket *, struct rxrpc_call *);
void rxrpc_kernel_get_peer(struct socket *, struct rxrpc_call *,
			   struct sockaddr_rxrpc *);
<<<<<<< HEAD
u64 rxrpc_kernel_get_rtt(struct socket *, struct rxrpc_call *);
=======
bool rxrpc_kernel_get_srtt(struct socket *, struct rxrpc_call *, u32 *);
>>>>>>> upstream/android-13
int rxrpc_kernel_charge_accept(struct socket *, rxrpc_notify_rx_t,
			       rxrpc_user_attach_call_t, unsigned long, gfp_t,
			       unsigned int);
void rxrpc_kernel_set_tx_length(struct socket *, struct rxrpc_call *, s64);
<<<<<<< HEAD
int rxrpc_kernel_retry_call(struct socket *, struct rxrpc_call *,
			    struct sockaddr_rxrpc *, struct key *);
int rxrpc_kernel_check_call(struct socket *, struct rxrpc_call *,
			    enum rxrpc_call_completion *, u32 *);
u32 rxrpc_kernel_check_life(struct socket *, struct rxrpc_call *);
=======
bool rxrpc_kernel_check_life(const struct socket *, const struct rxrpc_call *);
u32 rxrpc_kernel_get_epoch(struct socket *, struct rxrpc_call *);
bool rxrpc_kernel_get_reply_time(struct socket *, struct rxrpc_call *,
				 ktime_t *);
bool rxrpc_kernel_call_is_complete(struct rxrpc_call *);
void rxrpc_kernel_set_max_life(struct socket *, struct rxrpc_call *,
			       unsigned long);

int rxrpc_sock_set_min_security_level(struct sock *sk, unsigned int val);
>>>>>>> upstream/android-13

#endif /* _NET_RXRPC_H */
