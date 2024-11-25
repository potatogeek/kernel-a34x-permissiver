<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2004-2009 Red Hat, Inc.  All rights reserved.
**
<<<<<<< HEAD
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
=======
>>>>>>> upstream/android-13
**
*******************************************************************************
******************************************************************************/

/*
 * lowcomms.c
 *
 * This is the "low-level" comms layer.
 *
 * It is responsible for sending/receiving messages
 * from other nodes in the cluster.
 *
 * Cluster nodes are referred to by their nodeids. nodeids are
 * simply 32 bit numbers to the locking module - if they need to
 * be expanded for the cluster infrastructure then that is its
 * responsibility. It is this layer's
 * responsibility to resolve these into IP address or
 * whatever it needs for inter-node communication.
 *
 * The comms level is two kernel threads that deal mainly with
 * the receiving of messages from other nodes and passing them
 * up to the mid-level comms layer (which understands the
 * message format) for execution by the locking core, and
 * a send thread which does all the setting up of connections
 * to remote nodes and the sending of data. Threads are not allowed
 * to send their own data because it may cause them to wait in times
 * of high load. Also, this way, the sending thread can collect together
 * messages bound for one node and send them in one block.
 *
 * lowcomms will choose to use either TCP or SCTP as its transport layer
 * depending on the configuration variable 'protocol'. This should be set
 * to 0 (default) for TCP or 1 for SCTP. It should be configured using a
 * cluster-wide mechanism as it must be the same on all nodes of the cluster
 * for the DLM to function.
 *
 */

#include <asm/ioctls.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <linux/pagemap.h>
#include <linux/file.h>
#include <linux/mutex.h>
#include <linux/sctp.h>
#include <linux/slab.h>
#include <net/sctp/sctp.h>
#include <net/ipv6.h>

#include "dlm_internal.h"
#include "lowcomms.h"
#include "midcomms.h"
#include "config.h"

#define NEEDED_RMEM (4*1024*1024)
<<<<<<< HEAD
#define CONN_HASH_SIZE 32

/* Number of messages to send before rescheduling */
#define MAX_SEND_MSG_COUNT 25

struct cbuf {
	unsigned int base;
	unsigned int len;
	unsigned int mask;
};

static void cbuf_add(struct cbuf *cb, int n)
{
	cb->len += n;
}

static int cbuf_data(struct cbuf *cb)
{
	return ((cb->base + cb->len) & cb->mask);
}

static void cbuf_init(struct cbuf *cb, int size)
{
	cb->base = cb->len = 0;
	cb->mask = size-1;
}

static void cbuf_eat(struct cbuf *cb, int n)
{
	cb->len  -= n;
	cb->base += n;
	cb->base &= cb->mask;
}

static bool cbuf_empty(struct cbuf *cb)
{
	return cb->len == 0;
}
=======

/* Number of messages to send before rescheduling */
#define MAX_SEND_MSG_COUNT 25
#define DLM_SHUTDOWN_WAIT_TIMEOUT msecs_to_jiffies(10000)
>>>>>>> upstream/android-13

struct connection {
	struct socket *sock;	/* NULL if not connected */
	uint32_t nodeid;	/* So we know who we are in the list */
	struct mutex sock_mutex;
	unsigned long flags;
#define CF_READ_PENDING 1
#define CF_WRITE_PENDING 2
#define CF_INIT_PENDING 4
#define CF_IS_OTHERCON 5
#define CF_CLOSE 6
#define CF_APP_LIMITED 7
#define CF_CLOSING 8
<<<<<<< HEAD
	struct list_head writequeue;  /* List of outgoing writequeue_entries */
	spinlock_t writequeue_lock;
	int (*rx_action) (struct connection *);	/* What to do when active */
	void (*connect_action) (struct connection *);	/* What to do to connect */
	struct page *rx_page;
	struct cbuf cb;
=======
#define CF_SHUTDOWN 9
#define CF_CONNECTED 10
#define CF_RECONNECT 11
#define CF_DELAY_CONNECT 12
#define CF_EOF 13
	struct list_head writequeue;  /* List of outgoing writequeue_entries */
	spinlock_t writequeue_lock;
	atomic_t writequeue_cnt;
	struct mutex wq_alloc;
>>>>>>> upstream/android-13
	int retries;
#define MAX_CONNECT_RETRIES 3
	struct hlist_node list;
	struct connection *othercon;
<<<<<<< HEAD
	struct work_struct rwork; /* Receive workqueue */
	struct work_struct swork; /* Send workqueue */
};
#define sock2con(x) ((struct connection *)(x)->sk_user_data)

=======
	struct connection *sendcon;
	struct work_struct rwork; /* Receive workqueue */
	struct work_struct swork; /* Send workqueue */
	wait_queue_head_t shutdown_wait; /* wait for graceful shutdown */
	unsigned char *rx_buf;
	int rx_buflen;
	int rx_leftover;
	struct rcu_head rcu;
};
#define sock2con(x) ((struct connection *)(x)->sk_user_data)

struct listen_connection {
	struct socket *sock;
	struct work_struct rwork;
};

#define DLM_WQ_REMAIN_BYTES(e) (PAGE_SIZE - e->end)
#define DLM_WQ_LENGTH_BYTES(e) (e->end - e->offset)

>>>>>>> upstream/android-13
/* An entry waiting to be sent */
struct writequeue_entry {
	struct list_head list;
	struct page *page;
	int offset;
	int len;
	int end;
	int users;
<<<<<<< HEAD
	struct connection *con;
=======
	bool dirty;
	struct connection *con;
	struct list_head msgs;
	struct kref ref;
};

struct dlm_msg {
	struct writequeue_entry *entry;
	struct dlm_msg *orig_msg;
	bool retransmit;
	void *ppc;
	int len;
	int idx; /* new()/commit() idx exchange */

	struct list_head list;
	struct kref ref;
>>>>>>> upstream/android-13
};

struct dlm_node_addr {
	struct list_head list;
	int nodeid;
<<<<<<< HEAD
=======
	int mark;
>>>>>>> upstream/android-13
	int addr_count;
	int curr_addr_index;
	struct sockaddr_storage *addr[DLM_MAX_ADDR_COUNT];
};

<<<<<<< HEAD
=======
struct dlm_proto_ops {
	bool try_new_addr;
	const char *name;
	int proto;

	int (*connect)(struct connection *con, struct socket *sock,
		       struct sockaddr *addr, int addr_len);
	void (*sockopts)(struct socket *sock);
	int (*bind)(struct socket *sock);
	int (*listen_validate)(void);
	void (*listen_sockopts)(struct socket *sock);
	int (*listen_bind)(struct socket *sock);
	/* What to do to shutdown */
	void (*shutdown_action)(struct connection *con);
	/* What to do to eof check */
	bool (*eof_condition)(struct connection *con);
};

>>>>>>> upstream/android-13
static struct listen_sock_callbacks {
	void (*sk_error_report)(struct sock *);
	void (*sk_data_ready)(struct sock *);
	void (*sk_state_change)(struct sock *);
	void (*sk_write_space)(struct sock *);
} listen_sock;

static LIST_HEAD(dlm_node_addrs);
static DEFINE_SPINLOCK(dlm_node_addrs_spin);

<<<<<<< HEAD
static struct sockaddr_storage *dlm_local_addr[DLM_MAX_ADDR_COUNT];
static int dlm_local_count;
static int dlm_allow_conn;
=======
static struct listen_connection listen_con;
static struct sockaddr_storage *dlm_local_addr[DLM_MAX_ADDR_COUNT];
static int dlm_local_count;
int dlm_allow_conn;
>>>>>>> upstream/android-13

/* Work queues */
static struct workqueue_struct *recv_workqueue;
static struct workqueue_struct *send_workqueue;

static struct hlist_head connection_hash[CONN_HASH_SIZE];
<<<<<<< HEAD
static DEFINE_MUTEX(connections_lock);
static struct kmem_cache *con_cache;
=======
static DEFINE_SPINLOCK(connections_lock);
DEFINE_STATIC_SRCU(connections_srcu);

static const struct dlm_proto_ops *dlm_proto_ops;
>>>>>>> upstream/android-13

static void process_recv_sockets(struct work_struct *work);
static void process_send_sockets(struct work_struct *work);

<<<<<<< HEAD

/* This is deliberately very simple because most clusters have simple
   sequential nodeids, so we should be able to go straight to a connection
   struct in the array */
static inline int nodeid_hash(int nodeid)
{
	return nodeid & (CONN_HASH_SIZE-1);
}

static struct connection *__find_con(int nodeid)
{
	int r;
	struct connection *con;

	r = nodeid_hash(nodeid);

	hlist_for_each_entry(con, &connection_hash[r], list) {
		if (con->nodeid == nodeid)
			return con;
	}
	return NULL;
}

=======
/* need to held writequeue_lock */
static struct writequeue_entry *con_next_wq(struct connection *con)
{
	struct writequeue_entry *e;

	if (list_empty(&con->writequeue))
		return NULL;

	e = list_first_entry(&con->writequeue, struct writequeue_entry,
			     list);
	if (e->len == 0)
		return NULL;

	return e;
}

static struct connection *__find_con(int nodeid, int r)
{
	struct connection *con;

	hlist_for_each_entry_rcu(con, &connection_hash[r], list) {
		if (con->nodeid == nodeid)
			return con;
	}

	return NULL;
}

static bool tcp_eof_condition(struct connection *con)
{
	return atomic_read(&con->writequeue_cnt);
}

static int dlm_con_init(struct connection *con, int nodeid)
{
	con->rx_buflen = dlm_config.ci_buffer_size;
	con->rx_buf = kmalloc(con->rx_buflen, GFP_NOFS);
	if (!con->rx_buf)
		return -ENOMEM;

	con->nodeid = nodeid;
	mutex_init(&con->sock_mutex);
	INIT_LIST_HEAD(&con->writequeue);
	spin_lock_init(&con->writequeue_lock);
	atomic_set(&con->writequeue_cnt, 0);
	INIT_WORK(&con->swork, process_send_sockets);
	INIT_WORK(&con->rwork, process_recv_sockets);
	init_waitqueue_head(&con->shutdown_wait);

	return 0;
}

>>>>>>> upstream/android-13
/*
 * If 'allocation' is zero then we don't attempt to create a new
 * connection structure for this node.
 */
<<<<<<< HEAD
static struct connection *__nodeid2con(int nodeid, gfp_t alloc)
{
	struct connection *con = NULL;
	int r;

	con = __find_con(nodeid);
	if (con || !alloc)
		return con;

	con = kmem_cache_zalloc(con_cache, alloc);
	if (!con)
		return NULL;

	r = nodeid_hash(nodeid);
	hlist_add_head(&con->list, &connection_hash[r]);

	con->nodeid = nodeid;
	mutex_init(&con->sock_mutex);
	INIT_LIST_HEAD(&con->writequeue);
	spin_lock_init(&con->writequeue_lock);
	INIT_WORK(&con->swork, process_send_sockets);
	INIT_WORK(&con->rwork, process_recv_sockets);

	/* Setup action pointers for child sockets */
	if (con->nodeid) {
		struct connection *zerocon = __find_con(0);

		con->connect_action = zerocon->connect_action;
		if (!con->rx_action)
			con->rx_action = zerocon->rx_action;
	}

=======
static struct connection *nodeid2con(int nodeid, gfp_t alloc)
{
	struct connection *con, *tmp;
	int r, ret;

	r = nodeid_hash(nodeid);
	con = __find_con(nodeid, r);
	if (con || !alloc)
		return con;

	con = kzalloc(sizeof(*con), alloc);
	if (!con)
		return NULL;

	ret = dlm_con_init(con, nodeid);
	if (ret) {
		kfree(con);
		return NULL;
	}

	mutex_init(&con->wq_alloc);

	spin_lock(&connections_lock);
	/* Because multiple workqueues/threads calls this function it can
	 * race on multiple cpu's. Instead of locking hot path __find_con()
	 * we just check in rare cases of recently added nodes again
	 * under protection of connections_lock. If this is the case we
	 * abort our connection creation and return the existing connection.
	 */
	tmp = __find_con(nodeid, r);
	if (tmp) {
		spin_unlock(&connections_lock);
		kfree(con->rx_buf);
		kfree(con);
		return tmp;
	}

	hlist_add_head_rcu(&con->list, &connection_hash[r]);
	spin_unlock(&connections_lock);

>>>>>>> upstream/android-13
	return con;
}

/* Loop round all connections */
static void foreach_conn(void (*conn_func)(struct connection *c))
{
	int i;
<<<<<<< HEAD
	struct hlist_node *n;
	struct connection *con;

	for (i = 0; i < CONN_HASH_SIZE; i++) {
		hlist_for_each_entry_safe(con, n, &connection_hash[i], list)
=======
	struct connection *con;

	for (i = 0; i < CONN_HASH_SIZE; i++) {
		hlist_for_each_entry_rcu(con, &connection_hash[i], list)
>>>>>>> upstream/android-13
			conn_func(con);
	}
}

<<<<<<< HEAD
static struct connection *nodeid2con(int nodeid, gfp_t allocation)
{
	struct connection *con;

	mutex_lock(&connections_lock);
	con = __nodeid2con(nodeid, allocation);
	mutex_unlock(&connections_lock);

	return con;
}

=======
>>>>>>> upstream/android-13
static struct dlm_node_addr *find_node_addr(int nodeid)
{
	struct dlm_node_addr *na;

	list_for_each_entry(na, &dlm_node_addrs, list) {
		if (na->nodeid == nodeid)
			return na;
	}
	return NULL;
}

<<<<<<< HEAD
static int addr_compare(struct sockaddr_storage *x, struct sockaddr_storage *y)
=======
static int addr_compare(const struct sockaddr_storage *x,
			const struct sockaddr_storage *y)
>>>>>>> upstream/android-13
{
	switch (x->ss_family) {
	case AF_INET: {
		struct sockaddr_in *sinx = (struct sockaddr_in *)x;
		struct sockaddr_in *siny = (struct sockaddr_in *)y;
		if (sinx->sin_addr.s_addr != siny->sin_addr.s_addr)
			return 0;
		if (sinx->sin_port != siny->sin_port)
			return 0;
		break;
	}
	case AF_INET6: {
		struct sockaddr_in6 *sinx = (struct sockaddr_in6 *)x;
		struct sockaddr_in6 *siny = (struct sockaddr_in6 *)y;
		if (!ipv6_addr_equal(&sinx->sin6_addr, &siny->sin6_addr))
			return 0;
		if (sinx->sin6_port != siny->sin6_port)
			return 0;
		break;
	}
	default:
		return 0;
	}
	return 1;
}

static int nodeid_to_addr(int nodeid, struct sockaddr_storage *sas_out,
<<<<<<< HEAD
			  struct sockaddr *sa_out, bool try_new_addr)
=======
			  struct sockaddr *sa_out, bool try_new_addr,
			  unsigned int *mark)
>>>>>>> upstream/android-13
{
	struct sockaddr_storage sas;
	struct dlm_node_addr *na;

	if (!dlm_local_count)
		return -1;

	spin_lock(&dlm_node_addrs_spin);
	na = find_node_addr(nodeid);
	if (na && na->addr_count) {
		memcpy(&sas, na->addr[na->curr_addr_index],
		       sizeof(struct sockaddr_storage));

		if (try_new_addr) {
			na->curr_addr_index++;
			if (na->curr_addr_index == na->addr_count)
				na->curr_addr_index = 0;
		}
	}
	spin_unlock(&dlm_node_addrs_spin);

	if (!na)
		return -EEXIST;

	if (!na->addr_count)
		return -ENOENT;

<<<<<<< HEAD
=======
	*mark = na->mark;

>>>>>>> upstream/android-13
	if (sas_out)
		memcpy(sas_out, &sas, sizeof(struct sockaddr_storage));

	if (!sa_out)
		return 0;

	if (dlm_local_addr[0]->ss_family == AF_INET) {
		struct sockaddr_in *in4  = (struct sockaddr_in *) &sas;
		struct sockaddr_in *ret4 = (struct sockaddr_in *) sa_out;
		ret4->sin_addr.s_addr = in4->sin_addr.s_addr;
	} else {
		struct sockaddr_in6 *in6  = (struct sockaddr_in6 *) &sas;
		struct sockaddr_in6 *ret6 = (struct sockaddr_in6 *) sa_out;
		ret6->sin6_addr = in6->sin6_addr;
	}

	return 0;
}

<<<<<<< HEAD
static int addr_to_nodeid(struct sockaddr_storage *addr, int *nodeid)
=======
static int addr_to_nodeid(struct sockaddr_storage *addr, int *nodeid,
			  unsigned int *mark)
>>>>>>> upstream/android-13
{
	struct dlm_node_addr *na;
	int rv = -EEXIST;
	int addr_i;

	spin_lock(&dlm_node_addrs_spin);
	list_for_each_entry(na, &dlm_node_addrs, list) {
		if (!na->addr_count)
			continue;

		for (addr_i = 0; addr_i < na->addr_count; addr_i++) {
			if (addr_compare(na->addr[addr_i], addr)) {
				*nodeid = na->nodeid;
<<<<<<< HEAD
=======
				*mark = na->mark;
>>>>>>> upstream/android-13
				rv = 0;
				goto unlock;
			}
		}
	}
unlock:
	spin_unlock(&dlm_node_addrs_spin);
	return rv;
}

<<<<<<< HEAD
=======
/* caller need to held dlm_node_addrs_spin lock */
static bool dlm_lowcomms_na_has_addr(const struct dlm_node_addr *na,
				     const struct sockaddr_storage *addr)
{
	int i;

	for (i = 0; i < na->addr_count; i++) {
		if (addr_compare(na->addr[i], addr))
			return true;
	}

	return false;
}

>>>>>>> upstream/android-13
int dlm_lowcomms_addr(int nodeid, struct sockaddr_storage *addr, int len)
{
	struct sockaddr_storage *new_addr;
	struct dlm_node_addr *new_node, *na;
<<<<<<< HEAD
=======
	bool ret;
>>>>>>> upstream/android-13

	new_node = kzalloc(sizeof(struct dlm_node_addr), GFP_NOFS);
	if (!new_node)
		return -ENOMEM;

	new_addr = kzalloc(sizeof(struct sockaddr_storage), GFP_NOFS);
	if (!new_addr) {
		kfree(new_node);
		return -ENOMEM;
	}

	memcpy(new_addr, addr, len);

	spin_lock(&dlm_node_addrs_spin);
	na = find_node_addr(nodeid);
	if (!na) {
		new_node->nodeid = nodeid;
		new_node->addr[0] = new_addr;
		new_node->addr_count = 1;
<<<<<<< HEAD
=======
		new_node->mark = dlm_config.ci_mark;
>>>>>>> upstream/android-13
		list_add(&new_node->list, &dlm_node_addrs);
		spin_unlock(&dlm_node_addrs_spin);
		return 0;
	}

<<<<<<< HEAD
=======
	ret = dlm_lowcomms_na_has_addr(na, addr);
	if (ret) {
		spin_unlock(&dlm_node_addrs_spin);
		kfree(new_addr);
		kfree(new_node);
		return -EEXIST;
	}

>>>>>>> upstream/android-13
	if (na->addr_count >= DLM_MAX_ADDR_COUNT) {
		spin_unlock(&dlm_node_addrs_spin);
		kfree(new_addr);
		kfree(new_node);
		return -ENOSPC;
	}

	na->addr[na->addr_count++] = new_addr;
	spin_unlock(&dlm_node_addrs_spin);
	kfree(new_node);
	return 0;
}

/* Data available on socket or listen socket received a connect */
static void lowcomms_data_ready(struct sock *sk)
{
	struct connection *con;

	read_lock_bh(&sk->sk_callback_lock);
	con = sock2con(sk);
	if (con && !test_and_set_bit(CF_READ_PENDING, &con->flags))
		queue_work(recv_workqueue, &con->rwork);
	read_unlock_bh(&sk->sk_callback_lock);
}

<<<<<<< HEAD
=======
static void lowcomms_listen_data_ready(struct sock *sk)
{
	if (!dlm_allow_conn)
		return;

	queue_work(recv_workqueue, &listen_con.rwork);
}

>>>>>>> upstream/android-13
static void lowcomms_write_space(struct sock *sk)
{
	struct connection *con;

	read_lock_bh(&sk->sk_callback_lock);
	con = sock2con(sk);
	if (!con)
		goto out;

<<<<<<< HEAD
=======
	if (!test_and_set_bit(CF_CONNECTED, &con->flags)) {
		log_print("successful connected to node %d", con->nodeid);
		queue_work(send_workqueue, &con->swork);
		goto out;
	}

>>>>>>> upstream/android-13
	clear_bit(SOCK_NOSPACE, &con->sock->flags);

	if (test_and_clear_bit(CF_APP_LIMITED, &con->flags)) {
		con->sock->sk->sk_write_pending--;
		clear_bit(SOCKWQ_ASYNC_NOSPACE, &con->sock->flags);
	}

	queue_work(send_workqueue, &con->swork);
out:
	read_unlock_bh(&sk->sk_callback_lock);
}

static inline void lowcomms_connect_sock(struct connection *con)
{
	if (test_bit(CF_CLOSE, &con->flags))
		return;
	queue_work(send_workqueue, &con->swork);
	cond_resched();
}

static void lowcomms_state_change(struct sock *sk)
{
	/* SCTP layer is not calling sk_data_ready when the connection
	 * is done, so we catch the signal through here. Also, it
	 * doesn't switch socket state when entering shutdown, so we
	 * skip the write in that case.
	 */
	if (sk->sk_shutdown) {
		if (sk->sk_shutdown == RCV_SHUTDOWN)
			lowcomms_data_ready(sk);
	} else if (sk->sk_state == TCP_ESTABLISHED) {
		lowcomms_write_space(sk);
	}
}

int dlm_lowcomms_connect_node(int nodeid)
{
	struct connection *con;
<<<<<<< HEAD
=======
	int idx;
>>>>>>> upstream/android-13

	if (nodeid == dlm_our_nodeid())
		return 0;

<<<<<<< HEAD
	con = nodeid2con(nodeid, GFP_NOFS);
	if (!con)
		return -ENOMEM;
	lowcomms_connect_sock(con);
=======
	idx = srcu_read_lock(&connections_srcu);
	con = nodeid2con(nodeid, GFP_NOFS);
	if (!con) {
		srcu_read_unlock(&connections_srcu, idx);
		return -ENOMEM;
	}

	lowcomms_connect_sock(con);
	srcu_read_unlock(&connections_srcu, idx);

	return 0;
}

int dlm_lowcomms_nodes_set_mark(int nodeid, unsigned int mark)
{
	struct dlm_node_addr *na;

	spin_lock(&dlm_node_addrs_spin);
	na = find_node_addr(nodeid);
	if (!na) {
		spin_unlock(&dlm_node_addrs_spin);
		return -ENOENT;
	}

	na->mark = mark;
	spin_unlock(&dlm_node_addrs_spin);

>>>>>>> upstream/android-13
	return 0;
}

static void lowcomms_error_report(struct sock *sk)
{
	struct connection *con;
<<<<<<< HEAD
	struct sockaddr_storage saddr;
	void (*orig_report)(struct sock *) = NULL;
=======
	void (*orig_report)(struct sock *) = NULL;
	struct inet_sock *inet;
>>>>>>> upstream/android-13

	read_lock_bh(&sk->sk_callback_lock);
	con = sock2con(sk);
	if (con == NULL)
		goto out;

	orig_report = listen_sock.sk_error_report;
<<<<<<< HEAD
	if (con->sock == NULL ||
	    kernel_getpeername(con->sock, (struct sockaddr *)&saddr) < 0) {
		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "sending to node %d, port %d, "
				   "sk_err=%d/%d\n", dlm_our_nodeid(),
				   con->nodeid, dlm_config.ci_tcp_port,
				   sk->sk_err, sk->sk_err_soft);
	} else if (saddr.ss_family == AF_INET) {
		struct sockaddr_in *sin4 = (struct sockaddr_in *)&saddr;

		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "sending to node %d at %pI4, port %d, "
				   "sk_err=%d/%d\n", dlm_our_nodeid(),
				   con->nodeid, &sin4->sin_addr.s_addr,
				   dlm_config.ci_tcp_port, sk->sk_err,
				   sk->sk_err_soft);
	} else {
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)&saddr;

		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "sending to node %d at %u.%u.%u.%u, "
				   "port %d, sk_err=%d/%d\n", dlm_our_nodeid(),
				   con->nodeid, sin6->sin6_addr.s6_addr32[0],
				   sin6->sin6_addr.s6_addr32[1],
				   sin6->sin6_addr.s6_addr32[2],
				   sin6->sin6_addr.s6_addr32[3],
				   dlm_config.ci_tcp_port, sk->sk_err,
				   sk->sk_err_soft);
	}
=======

	inet = inet_sk(sk);
	switch (sk->sk_family) {
	case AF_INET:
		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "sending to node %d at %pI4, dport %d, "
				   "sk_err=%d/%d\n", dlm_our_nodeid(),
				   con->nodeid, &inet->inet_daddr,
				   ntohs(inet->inet_dport), sk->sk_err,
				   sk->sk_err_soft);
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case AF_INET6:
		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "sending to node %d at %pI6c, "
				   "dport %d, sk_err=%d/%d\n", dlm_our_nodeid(),
				   con->nodeid, &sk->sk_v6_daddr,
				   ntohs(inet->inet_dport), sk->sk_err,
				   sk->sk_err_soft);
		break;
#endif
	default:
		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "invalid socket family %d set, "
				   "sk_err=%d/%d\n", dlm_our_nodeid(),
				   sk->sk_family, sk->sk_err, sk->sk_err_soft);
		goto out;
	}

	/* below sendcon only handling */
	if (test_bit(CF_IS_OTHERCON, &con->flags))
		con = con->sendcon;

	switch (sk->sk_err) {
	case ECONNREFUSED:
		set_bit(CF_DELAY_CONNECT, &con->flags);
		break;
	default:
		break;
	}

	if (!test_and_set_bit(CF_RECONNECT, &con->flags))
		queue_work(send_workqueue, &con->swork);

>>>>>>> upstream/android-13
out:
	read_unlock_bh(&sk->sk_callback_lock);
	if (orig_report)
		orig_report(sk);
}

/* Note: sk_callback_lock must be locked before calling this function. */
static void save_listen_callbacks(struct socket *sock)
{
	struct sock *sk = sock->sk;

	listen_sock.sk_data_ready = sk->sk_data_ready;
	listen_sock.sk_state_change = sk->sk_state_change;
	listen_sock.sk_write_space = sk->sk_write_space;
	listen_sock.sk_error_report = sk->sk_error_report;
}

static void restore_callbacks(struct socket *sock)
{
	struct sock *sk = sock->sk;

	write_lock_bh(&sk->sk_callback_lock);
	sk->sk_user_data = NULL;
	sk->sk_data_ready = listen_sock.sk_data_ready;
	sk->sk_state_change = listen_sock.sk_state_change;
	sk->sk_write_space = listen_sock.sk_write_space;
	sk->sk_error_report = listen_sock.sk_error_report;
	write_unlock_bh(&sk->sk_callback_lock);
}

<<<<<<< HEAD
=======
static void add_listen_sock(struct socket *sock, struct listen_connection *con)
{
	struct sock *sk = sock->sk;

	write_lock_bh(&sk->sk_callback_lock);
	save_listen_callbacks(sock);
	con->sock = sock;

	sk->sk_user_data = con;
	sk->sk_allocation = GFP_NOFS;
	/* Install a data_ready callback */
	sk->sk_data_ready = lowcomms_listen_data_ready;
	write_unlock_bh(&sk->sk_callback_lock);
}

>>>>>>> upstream/android-13
/* Make a socket active */
static void add_sock(struct socket *sock, struct connection *con)
{
	struct sock *sk = sock->sk;

	write_lock_bh(&sk->sk_callback_lock);
	con->sock = sock;

	sk->sk_user_data = con;
	/* Install a data_ready callback */
	sk->sk_data_ready = lowcomms_data_ready;
	sk->sk_write_space = lowcomms_write_space;
	sk->sk_state_change = lowcomms_state_change;
	sk->sk_allocation = GFP_NOFS;
	sk->sk_error_report = lowcomms_error_report;
	write_unlock_bh(&sk->sk_callback_lock);
}

/* Add the port number to an IPv6 or 4 sockaddr and return the address
   length */
static void make_sockaddr(struct sockaddr_storage *saddr, uint16_t port,
			  int *addr_len)
{
	saddr->ss_family =  dlm_local_addr[0]->ss_family;
	if (saddr->ss_family == AF_INET) {
		struct sockaddr_in *in4_addr = (struct sockaddr_in *)saddr;
		in4_addr->sin_port = cpu_to_be16(port);
		*addr_len = sizeof(struct sockaddr_in);
		memset(&in4_addr->sin_zero, 0, sizeof(in4_addr->sin_zero));
	} else {
		struct sockaddr_in6 *in6_addr = (struct sockaddr_in6 *)saddr;
		in6_addr->sin6_port = cpu_to_be16(port);
		*addr_len = sizeof(struct sockaddr_in6);
	}
	memset((char *)saddr + *addr_len, 0, sizeof(struct sockaddr_storage) - *addr_len);
}

<<<<<<< HEAD
=======
static void dlm_page_release(struct kref *kref)
{
	struct writequeue_entry *e = container_of(kref, struct writequeue_entry,
						  ref);

	__free_page(e->page);
	kfree(e);
}

static void dlm_msg_release(struct kref *kref)
{
	struct dlm_msg *msg = container_of(kref, struct dlm_msg, ref);

	kref_put(&msg->entry->ref, dlm_page_release);
	kfree(msg);
}

static void free_entry(struct writequeue_entry *e)
{
	struct dlm_msg *msg, *tmp;

	list_for_each_entry_safe(msg, tmp, &e->msgs, list) {
		if (msg->orig_msg) {
			msg->orig_msg->retransmit = false;
			kref_put(&msg->orig_msg->ref, dlm_msg_release);
		}

		list_del(&msg->list);
		kref_put(&msg->ref, dlm_msg_release);
	}

	list_del(&e->list);
	atomic_dec(&e->con->writequeue_cnt);
	kref_put(&e->ref, dlm_page_release);
}

static void dlm_close_sock(struct socket **sock)
{
	if (*sock) {
		restore_callbacks(*sock);
		sock_release(*sock);
		*sock = NULL;
	}
}

>>>>>>> upstream/android-13
/* Close a remote connection and tidy up */
static void close_connection(struct connection *con, bool and_other,
			     bool tx, bool rx)
{
	bool closing = test_and_set_bit(CF_CLOSING, &con->flags);
<<<<<<< HEAD
=======
	struct writequeue_entry *e;
>>>>>>> upstream/android-13

	if (tx && !closing && cancel_work_sync(&con->swork)) {
		log_print("canceled swork for node %d", con->nodeid);
		clear_bit(CF_WRITE_PENDING, &con->flags);
	}
	if (rx && !closing && cancel_work_sync(&con->rwork)) {
		log_print("canceled rwork for node %d", con->nodeid);
		clear_bit(CF_READ_PENDING, &con->flags);
	}

	mutex_lock(&con->sock_mutex);
<<<<<<< HEAD
	if (con->sock) {
		restore_callbacks(con->sock);
		sock_release(con->sock);
		con->sock = NULL;
	}
	if (con->othercon && and_other) {
		/* Will only re-enter once. */
		close_connection(con->othercon, false, true, true);
	}
	if (con->rx_page) {
		__free_page(con->rx_page);
		con->rx_page = NULL;
	}

	con->retries = 0;
=======
	dlm_close_sock(&con->sock);

	if (con->othercon && and_other) {
		/* Will only re-enter once. */
		close_connection(con->othercon, false, tx, rx);
	}

	/* if we send a writequeue entry only a half way, we drop the
	 * whole entry because reconnection and that we not start of the
	 * middle of a msg which will confuse the other end.
	 *
	 * we can always drop messages because retransmits, but what we
	 * cannot allow is to transmit half messages which may be processed
	 * at the other side.
	 *
	 * our policy is to start on a clean state when disconnects, we don't
	 * know what's send/received on transport layer in this case.
	 */
	spin_lock(&con->writequeue_lock);
	if (!list_empty(&con->writequeue)) {
		e = list_first_entry(&con->writequeue, struct writequeue_entry,
				     list);
		if (e->dirty)
			free_entry(e);
	}
	spin_unlock(&con->writequeue_lock);

	con->rx_leftover = 0;
	con->retries = 0;
	clear_bit(CF_APP_LIMITED, &con->flags);
	clear_bit(CF_CONNECTED, &con->flags);
	clear_bit(CF_DELAY_CONNECT, &con->flags);
	clear_bit(CF_RECONNECT, &con->flags);
	clear_bit(CF_EOF, &con->flags);
>>>>>>> upstream/android-13
	mutex_unlock(&con->sock_mutex);
	clear_bit(CF_CLOSING, &con->flags);
}

<<<<<<< HEAD
/* Data received from remote end */
static int receive_from_sock(struct connection *con)
{
	int ret = 0;
	struct msghdr msg = {};
	struct kvec iov[2];
	unsigned len;
	int r;
	int call_again_soon = 0;
	int nvec;
=======
static void shutdown_connection(struct connection *con)
{
	int ret;

	flush_work(&con->swork);

	mutex_lock(&con->sock_mutex);
	/* nothing to shutdown */
	if (!con->sock) {
		mutex_unlock(&con->sock_mutex);
		return;
	}

	set_bit(CF_SHUTDOWN, &con->flags);
	ret = kernel_sock_shutdown(con->sock, SHUT_WR);
	mutex_unlock(&con->sock_mutex);
	if (ret) {
		log_print("Connection %p failed to shutdown: %d will force close",
			  con, ret);
		goto force_close;
	} else {
		ret = wait_event_timeout(con->shutdown_wait,
					 !test_bit(CF_SHUTDOWN, &con->flags),
					 DLM_SHUTDOWN_WAIT_TIMEOUT);
		if (ret == 0) {
			log_print("Connection %p shutdown timed out, will force close",
				  con);
			goto force_close;
		}
	}

	return;

force_close:
	clear_bit(CF_SHUTDOWN, &con->flags);
	close_connection(con, false, true, true);
}

static void dlm_tcp_shutdown(struct connection *con)
{
	if (con->othercon)
		shutdown_connection(con->othercon);
	shutdown_connection(con);
}

static int con_realloc_receive_buf(struct connection *con, int newlen)
{
	unsigned char *newbuf;

	newbuf = kmalloc(newlen, GFP_NOFS);
	if (!newbuf)
		return -ENOMEM;

	/* copy any leftover from last receive */
	if (con->rx_leftover)
		memmove(newbuf, con->rx_buf, con->rx_leftover);

	/* swap to new buffer space */
	kfree(con->rx_buf);
	con->rx_buflen = newlen;
	con->rx_buf = newbuf;

	return 0;
}

/* Data received from remote end */
static int receive_from_sock(struct connection *con)
{
	struct msghdr msg;
	struct kvec iov;
	int ret, buflen;
>>>>>>> upstream/android-13

	mutex_lock(&con->sock_mutex);

	if (con->sock == NULL) {
		ret = -EAGAIN;
		goto out_close;
	}
<<<<<<< HEAD
	if (con->nodeid == 0) {
		ret = -EINVAL;
		goto out_close;
	}

	if (con->rx_page == NULL) {
		/*
		 * This doesn't need to be atomic, but I think it should
		 * improve performance if it is.
		 */
		con->rx_page = alloc_page(GFP_ATOMIC);
		if (con->rx_page == NULL)
			goto out_resched;
		cbuf_init(&con->cb, PAGE_SIZE);
	}

	/*
	 * iov[0] is the bit of the circular buffer between the current end
	 * point (cb.base + cb.len) and the end of the buffer.
	 */
	iov[0].iov_len = con->cb.base - cbuf_data(&con->cb);
	iov[0].iov_base = page_address(con->rx_page) + cbuf_data(&con->cb);
	iov[1].iov_len = 0;
	nvec = 1;

	/*
	 * iov[1] is the bit of the circular buffer between the start of the
	 * buffer and the start of the currently used section (cb.base)
	 */
	if (cbuf_data(&con->cb) >= con->cb.base) {
		iov[0].iov_len = PAGE_SIZE - cbuf_data(&con->cb);
		iov[1].iov_len = con->cb.base;
		iov[1].iov_base = page_address(con->rx_page);
		nvec = 2;
	}
	len = iov[0].iov_len + iov[1].iov_len;
	iov_iter_kvec(&msg.msg_iter, READ | ITER_KVEC, iov, nvec, len);

	r = ret = sock_recvmsg(con->sock, &msg, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret <= 0)
		goto out_close;
	else if (ret == len)
		call_again_soon = 1;

	cbuf_add(&con->cb, ret);
	ret = dlm_process_incoming_buffer(con->nodeid,
					  page_address(con->rx_page),
					  con->cb.base, con->cb.len,
					  PAGE_SIZE);
	if (ret == -EBADMSG) {
		log_print("lowcomms: addr=%p, base=%u, len=%u, read=%d",
			  page_address(con->rx_page), con->cb.base,
			  con->cb.len, r);
	}
	if (ret < 0)
		goto out_close;
	cbuf_eat(&con->cb, ret);

	if (cbuf_empty(&con->cb) && !call_again_soon) {
		__free_page(con->rx_page);
		con->rx_page = NULL;
	}

	if (call_again_soon)
		goto out_resched;
=======

	/* realloc if we get new buffer size to read out */
	buflen = dlm_config.ci_buffer_size;
	if (con->rx_buflen != buflen && con->rx_leftover <= buflen) {
		ret = con_realloc_receive_buf(con, buflen);
		if (ret < 0)
			goto out_resched;
	}

	for (;;) {
		/* calculate new buffer parameter regarding last receive and
		 * possible leftover bytes
		 */
		iov.iov_base = con->rx_buf + con->rx_leftover;
		iov.iov_len = con->rx_buflen - con->rx_leftover;

		memset(&msg, 0, sizeof(msg));
		msg.msg_flags = MSG_DONTWAIT | MSG_NOSIGNAL;
		ret = kernel_recvmsg(con->sock, &msg, &iov, 1, iov.iov_len,
				     msg.msg_flags);
		if (ret == -EAGAIN)
			break;
		else if (ret <= 0)
			goto out_close;

		/* new buflen according readed bytes and leftover from last receive */
		buflen = ret + con->rx_leftover;
		ret = dlm_process_incoming_buffer(con->nodeid, con->rx_buf, buflen);
		if (ret < 0)
			goto out_close;

		/* calculate leftover bytes from process and put it into begin of
		 * the receive buffer, so next receive we have the full message
		 * at the start address of the receive buffer.
		 */
		con->rx_leftover = buflen - ret;
		if (con->rx_leftover) {
			memmove(con->rx_buf, con->rx_buf + ret,
				con->rx_leftover);
		}
	}

	dlm_midcomms_receive_done(con->nodeid);
>>>>>>> upstream/android-13
	mutex_unlock(&con->sock_mutex);
	return 0;

out_resched:
	if (!test_and_set_bit(CF_READ_PENDING, &con->flags))
		queue_work(recv_workqueue, &con->rwork);
	mutex_unlock(&con->sock_mutex);
	return -EAGAIN;

out_close:
<<<<<<< HEAD
	mutex_unlock(&con->sock_mutex);
	if (ret != -EAGAIN) {
		close_connection(con, true, true, false);
		/* Reconnect when there is something to send */
	}
	/* Don't return success if we really got EOF */
	if (ret == 0)
		ret = -EAGAIN;

=======
	if (ret == 0) {
		log_print("connection %p got EOF from %d",
			  con, con->nodeid);

		if (dlm_proto_ops->eof_condition &&
		    dlm_proto_ops->eof_condition(con)) {
			set_bit(CF_EOF, &con->flags);
			mutex_unlock(&con->sock_mutex);
		} else {
			mutex_unlock(&con->sock_mutex);
			close_connection(con, false, true, false);

			/* handling for tcp shutdown */
			clear_bit(CF_SHUTDOWN, &con->flags);
			wake_up(&con->shutdown_wait);
		}

		/* signal to breaking receive worker */
		ret = -1;
	} else {
		mutex_unlock(&con->sock_mutex);
	}
>>>>>>> upstream/android-13
	return ret;
}

/* Listening socket is busy, accept a connection */
<<<<<<< HEAD
static int tcp_accept_from_sock(struct connection *con)
=======
static int accept_from_sock(struct listen_connection *con)
>>>>>>> upstream/android-13
{
	int result;
	struct sockaddr_storage peeraddr;
	struct socket *newsock;
<<<<<<< HEAD
	int len;
	int nodeid;
	struct connection *newcon;
	struct connection *addcon;

	mutex_lock(&connections_lock);
	if (!dlm_allow_conn) {
		mutex_unlock(&connections_lock);
		return -1;
	}
	mutex_unlock(&connections_lock);

	mutex_lock_nested(&con->sock_mutex, 0);

	if (!con->sock) {
		mutex_unlock(&con->sock_mutex);
		return -ENOTCONN;
	}
=======
	int len, idx;
	int nodeid;
	struct connection *newcon;
	struct connection *addcon;
	unsigned int mark;

	if (!con->sock)
		return -ENOTCONN;
>>>>>>> upstream/android-13

	result = kernel_accept(con->sock, &newsock, O_NONBLOCK);
	if (result < 0)
		goto accept_err;

	/* Get the connected socket's peer */
	memset(&peeraddr, 0, sizeof(peeraddr));
	len = newsock->ops->getname(newsock, (struct sockaddr *)&peeraddr, 2);
	if (len < 0) {
		result = -ECONNABORTED;
		goto accept_err;
	}

	/* Get the new node's NODEID */
	make_sockaddr(&peeraddr, 0, &len);
<<<<<<< HEAD
	if (addr_to_nodeid(&peeraddr, &nodeid)) {
=======
	if (addr_to_nodeid(&peeraddr, &nodeid, &mark)) {
>>>>>>> upstream/android-13
		unsigned char *b=(unsigned char *)&peeraddr;
		log_print("connect from non cluster node");
		print_hex_dump_bytes("ss: ", DUMP_PREFIX_NONE, 
				     b, sizeof(struct sockaddr_storage));
		sock_release(newsock);
<<<<<<< HEAD
		mutex_unlock(&con->sock_mutex);
=======
>>>>>>> upstream/android-13
		return -1;
	}

	log_print("got connection from %d", nodeid);

	/*  Check to see if we already have a connection to this node. This
	 *  could happen if the two nodes initiate a connection at roughly
	 *  the same time and the connections cross on the wire.
	 *  In this case we store the incoming one in "othercon"
	 */
<<<<<<< HEAD
	newcon = nodeid2con(nodeid, GFP_NOFS);
	if (!newcon) {
		result = -ENOMEM;
		goto accept_err;
	}
	mutex_lock_nested(&newcon->sock_mutex, 1);
=======
	idx = srcu_read_lock(&connections_srcu);
	newcon = nodeid2con(nodeid, GFP_NOFS);
	if (!newcon) {
		srcu_read_unlock(&connections_srcu, idx);
		result = -ENOMEM;
		goto accept_err;
	}

	sock_set_mark(newsock->sk, mark);

	mutex_lock(&newcon->sock_mutex);
>>>>>>> upstream/android-13
	if (newcon->sock) {
		struct connection *othercon = newcon->othercon;

		if (!othercon) {
<<<<<<< HEAD
			othercon = kmem_cache_zalloc(con_cache, GFP_NOFS);
			if (!othercon) {
				log_print("failed to allocate incoming socket");
				mutex_unlock(&newcon->sock_mutex);
				result = -ENOMEM;
				goto accept_err;
			}
			othercon->nodeid = nodeid;
			othercon->rx_action = receive_from_sock;
			mutex_init(&othercon->sock_mutex);
			INIT_LIST_HEAD(&othercon->writequeue);
			spin_lock_init(&othercon->writequeue_lock);
			INIT_WORK(&othercon->swork, process_send_sockets);
			INIT_WORK(&othercon->rwork, process_recv_sockets);
			set_bit(CF_IS_OTHERCON, &othercon->flags);
		}
		mutex_lock_nested(&othercon->sock_mutex, 2);
		if (!othercon->sock) {
			newcon->othercon = othercon;
			add_sock(newsock, othercon);
			addcon = othercon;
			mutex_unlock(&othercon->sock_mutex);
		}
		else {
			printk("Extra connection from node %d attempted\n", nodeid);
			result = -EAGAIN;
			mutex_unlock(&othercon->sock_mutex);
			mutex_unlock(&newcon->sock_mutex);
			goto accept_err;
		}
	}
	else {
		newcon->rx_action = receive_from_sock;
=======
			othercon = kzalloc(sizeof(*othercon), GFP_NOFS);
			if (!othercon) {
				log_print("failed to allocate incoming socket");
				mutex_unlock(&newcon->sock_mutex);
				srcu_read_unlock(&connections_srcu, idx);
				result = -ENOMEM;
				goto accept_err;
			}

			result = dlm_con_init(othercon, nodeid);
			if (result < 0) {
				kfree(othercon);
				mutex_unlock(&newcon->sock_mutex);
				srcu_read_unlock(&connections_srcu, idx);
				goto accept_err;
			}

			lockdep_set_subclass(&othercon->sock_mutex, 1);
			set_bit(CF_IS_OTHERCON, &othercon->flags);
			newcon->othercon = othercon;
			othercon->sendcon = newcon;
		} else {
			/* close other sock con if we have something new */
			close_connection(othercon, false, true, false);
		}

		mutex_lock(&othercon->sock_mutex);
		add_sock(newsock, othercon);
		addcon = othercon;
		mutex_unlock(&othercon->sock_mutex);
	}
	else {
>>>>>>> upstream/android-13
		/* accept copies the sk after we've saved the callbacks, so we
		   don't want to save them a second time or comm errors will
		   result in calling sk_error_report recursively. */
		add_sock(newsock, newcon);
		addcon = newcon;
	}

<<<<<<< HEAD
=======
	set_bit(CF_CONNECTED, &addcon->flags);
>>>>>>> upstream/android-13
	mutex_unlock(&newcon->sock_mutex);

	/*
	 * Add it to the active queue in case we got data
	 * between processing the accept adding the socket
	 * to the read_sockets list
	 */
	if (!test_and_set_bit(CF_READ_PENDING, &addcon->flags))
		queue_work(recv_workqueue, &addcon->rwork);
<<<<<<< HEAD
	mutex_unlock(&con->sock_mutex);
=======

	srcu_read_unlock(&connections_srcu, idx);
>>>>>>> upstream/android-13

	return 0;

accept_err:
<<<<<<< HEAD
	mutex_unlock(&con->sock_mutex);
=======
>>>>>>> upstream/android-13
	if (newsock)
		sock_release(newsock);

	if (result != -EAGAIN)
		log_print("error accepting connection from node: %d", result);
	return result;
}

<<<<<<< HEAD
static int sctp_accept_from_sock(struct connection *con)
{
	/* Check that the new node is in the lockspace */
	struct sctp_prim prim;
	int nodeid;
	int prim_len, ret;
	int addr_len;
	struct connection *newcon;
	struct connection *addcon;
	struct socket *newsock;

	mutex_lock(&connections_lock);
	if (!dlm_allow_conn) {
		mutex_unlock(&connections_lock);
		return -1;
	}
	mutex_unlock(&connections_lock);

	mutex_lock_nested(&con->sock_mutex, 0);

	ret = kernel_accept(con->sock, &newsock, O_NONBLOCK);
	if (ret < 0)
		goto accept_err;

	memset(&prim, 0, sizeof(struct sctp_prim));
	prim_len = sizeof(struct sctp_prim);

	ret = kernel_getsockopt(newsock, IPPROTO_SCTP, SCTP_PRIMARY_ADDR,
				(char *)&prim, &prim_len);
	if (ret < 0) {
		log_print("getsockopt/sctp_primary_addr failed: %d", ret);
		goto accept_err;
	}

	make_sockaddr(&prim.ssp_addr, 0, &addr_len);
	ret = addr_to_nodeid(&prim.ssp_addr, &nodeid);
	if (ret) {
		unsigned char *b = (unsigned char *)&prim.ssp_addr;

		log_print("reject connect from unknown addr");
		print_hex_dump_bytes("ss: ", DUMP_PREFIX_NONE,
				     b, sizeof(struct sockaddr_storage));
		goto accept_err;
	}

	newcon = nodeid2con(nodeid, GFP_NOFS);
	if (!newcon) {
		ret = -ENOMEM;
		goto accept_err;
	}

	mutex_lock_nested(&newcon->sock_mutex, 1);

	if (newcon->sock) {
		struct connection *othercon = newcon->othercon;

		if (!othercon) {
			othercon = kmem_cache_zalloc(con_cache, GFP_NOFS);
			if (!othercon) {
				log_print("failed to allocate incoming socket");
				mutex_unlock(&newcon->sock_mutex);
				ret = -ENOMEM;
				goto accept_err;
			}
			othercon->nodeid = nodeid;
			othercon->rx_action = receive_from_sock;
			mutex_init(&othercon->sock_mutex);
			INIT_LIST_HEAD(&othercon->writequeue);
			spin_lock_init(&othercon->writequeue_lock);
			INIT_WORK(&othercon->swork, process_send_sockets);
			INIT_WORK(&othercon->rwork, process_recv_sockets);
			set_bit(CF_IS_OTHERCON, &othercon->flags);
		}
		mutex_lock_nested(&othercon->sock_mutex, 2);
		if (!othercon->sock) {
			newcon->othercon = othercon;
			add_sock(newsock, othercon);
			addcon = othercon;
			mutex_unlock(&othercon->sock_mutex);
		} else {
			printk("Extra connection from node %d attempted\n", nodeid);
			ret = -EAGAIN;
			mutex_unlock(&othercon->sock_mutex);
			mutex_unlock(&newcon->sock_mutex);
			goto accept_err;
		}
	} else {
		newcon->rx_action = receive_from_sock;
		add_sock(newsock, newcon);
		addcon = newcon;
	}

	log_print("connected to %d", nodeid);

	mutex_unlock(&newcon->sock_mutex);

	/*
	 * Add it to the active queue in case we got data
	 * between processing the accept adding the socket
	 * to the read_sockets list
	 */
	if (!test_and_set_bit(CF_READ_PENDING, &addcon->flags))
		queue_work(recv_workqueue, &addcon->rwork);
	mutex_unlock(&con->sock_mutex);

	return 0;

accept_err:
	mutex_unlock(&con->sock_mutex);
	if (newsock)
		sock_release(newsock);
	if (ret != -EAGAIN)
		log_print("error accepting connection from node: %d", ret);

	return ret;
}

static void free_entry(struct writequeue_entry *e)
{
	__free_page(e->page);
	kfree(e);
}

=======
>>>>>>> upstream/android-13
/*
 * writequeue_entry_complete - try to delete and free write queue entry
 * @e: write queue entry to try to delete
 * @completed: bytes completed
 *
 * writequeue_lock must be held.
 */
static void writequeue_entry_complete(struct writequeue_entry *e, int completed)
{
	e->offset += completed;
	e->len -= completed;
<<<<<<< HEAD

	if (e->len == 0 && e->users == 0) {
		list_del(&e->list);
		free_entry(e);
	}
=======
	/* signal that page was half way transmitted */
	e->dirty = true;

	if (e->len == 0 && e->users == 0)
		free_entry(e);
>>>>>>> upstream/android-13
}

/*
 * sctp_bind_addrs - bind a SCTP socket to all our addresses
 */
<<<<<<< HEAD
static int sctp_bind_addrs(struct connection *con, uint16_t port)
{
	struct sockaddr_storage localaddr;
=======
static int sctp_bind_addrs(struct socket *sock, uint16_t port)
{
	struct sockaddr_storage localaddr;
	struct sockaddr *addr = (struct sockaddr *)&localaddr;
>>>>>>> upstream/android-13
	int i, addr_len, result = 0;

	for (i = 0; i < dlm_local_count; i++) {
		memcpy(&localaddr, dlm_local_addr[i], sizeof(localaddr));
		make_sockaddr(&localaddr, port, &addr_len);

		if (!i)
<<<<<<< HEAD
			result = kernel_bind(con->sock,
					     (struct sockaddr *)&localaddr,
					     addr_len);
		else
			result = kernel_setsockopt(con->sock, SOL_SCTP,
						   SCTP_SOCKOPT_BINDX_ADD,
						   (char *)&localaddr, addr_len);
=======
			result = kernel_bind(sock, addr, addr_len);
		else
			result = sock_bind_add(sock->sk, addr, addr_len);
>>>>>>> upstream/android-13

		if (result < 0) {
			log_print("Can't bind to %d addr number %d, %d.\n",
				  port, i + 1, result);
			break;
		}
	}
	return result;
}

<<<<<<< HEAD
/* Initiate an SCTP association.
   This is a special case of send_to_sock() in that we don't yet have a
   peeled-off socket for this association, so we use the listening socket
   and add the primary IP address of the remote node.
 */
static void sctp_connect_to_sock(struct connection *con)
{
	struct sockaddr_storage daddr;
	int one = 1;
	int result;
	int addr_len;
	struct socket *sock;
	struct timeval tv = { .tv_sec = 5, .tv_usec = 0 };

	if (con->nodeid == 0) {
		log_print("attempt to connect sock 0 foiled");
		return;
	}

	mutex_lock(&con->sock_mutex);

	/* Some odd races can cause double-connects, ignore them */
	if (con->retries++ > MAX_CONNECT_RETRIES)
		goto out;

	if (con->sock) {
		log_print("node %d already connected.", con->nodeid);
		goto out;
	}

	memset(&daddr, 0, sizeof(daddr));
	result = nodeid_to_addr(con->nodeid, &daddr, NULL, true);
	if (result < 0) {
		log_print("no address for nodeid %d", con->nodeid);
		goto out;
	}

	/* Create a socket to communicate with */
	result = sock_create_kern(&init_net, dlm_local_addr[0]->ss_family,
				  SOCK_STREAM, IPPROTO_SCTP, &sock);
	if (result < 0)
		goto socket_err;

	con->rx_action = receive_from_sock;
	con->connect_action = sctp_connect_to_sock;
	add_sock(sock, con);

	/* Bind to all addresses. */
	if (sctp_bind_addrs(con, 0))
		goto bind_err;

	make_sockaddr(&daddr, dlm_config.ci_tcp_port, &addr_len);

	log_print("connecting to %d", con->nodeid);

	/* Turn off Nagle's algorithm */
	kernel_setsockopt(sock, SOL_SCTP, SCTP_NODELAY, (char *)&one,
			  sizeof(one));

	/*
	 * Make sock->ops->connect() function return in specified time,
	 * since O_NONBLOCK argument in connect() function does not work here,
	 * then, we should restore the default value of this attribute.
	 */
	kernel_setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv,
			  sizeof(tv));
	result = sock->ops->connect(sock, (struct sockaddr *)&daddr, addr_len,
				   0);
	memset(&tv, 0, sizeof(tv));
	kernel_setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv,
			  sizeof(tv));

	if (result == -EINPROGRESS)
		result = 0;
	if (result == 0)
		goto out;

bind_err:
	con->sock = NULL;
	sock_release(sock);

socket_err:
	/*
	 * Some errors are fatal and this list might need adjusting. For other
	 * errors we try again until the max number of retries is reached.
	 */
	if (result != -EHOSTUNREACH &&
	    result != -ENETUNREACH &&
	    result != -ENETDOWN &&
	    result != -EINVAL &&
	    result != -EPROTONOSUPPORT) {
		log_print("connect %d try %d error %d", con->nodeid,
			  con->retries, result);
		mutex_unlock(&con->sock_mutex);
		msleep(1000);
		lowcomms_connect_sock(con);
		return;
	}

out:
	mutex_unlock(&con->sock_mutex);
}

/* Connect a new socket to its peer */
static void tcp_connect_to_sock(struct connection *con)
{
	struct sockaddr_storage saddr, src_addr;
	int addr_len;
	struct socket *sock = NULL;
	int one = 1;
	int result;

	if (con->nodeid == 0) {
		log_print("attempt to connect sock 0 foiled");
		return;
	}

	mutex_lock(&con->sock_mutex);
	if (con->retries++ > MAX_CONNECT_RETRIES)
		goto out;

	/* Some odd races can cause double-connects, ignore them */
	if (con->sock)
		goto out;

	/* Create a socket to communicate with */
	result = sock_create_kern(&init_net, dlm_local_addr[0]->ss_family,
				  SOCK_STREAM, IPPROTO_TCP, &sock);
	if (result < 0)
		goto out_err;

	memset(&saddr, 0, sizeof(saddr));
	result = nodeid_to_addr(con->nodeid, &saddr, NULL, false);
	if (result < 0) {
		log_print("no address for nodeid %d", con->nodeid);
		goto out_err;
	}

	con->rx_action = receive_from_sock;
	con->connect_action = tcp_connect_to_sock;
	add_sock(sock, con);

	/* Bind to our cluster-known address connecting to avoid
	   routing problems */
	memcpy(&src_addr, dlm_local_addr[0], sizeof(src_addr));
	make_sockaddr(&src_addr, 0, &addr_len);
	result = sock->ops->bind(sock, (struct sockaddr *) &src_addr,
				 addr_len);
	if (result < 0) {
		log_print("could not bind for connect: %d", result);
		/* This *may* not indicate a critical error */
	}

	make_sockaddr(&saddr, dlm_config.ci_tcp_port, &addr_len);

	log_print("connecting to %d", con->nodeid);

	/* Turn off Nagle's algorithm */
	kernel_setsockopt(sock, SOL_TCP, TCP_NODELAY, (char *)&one,
			  sizeof(one));

	result = sock->ops->connect(sock, (struct sockaddr *)&saddr, addr_len,
				   O_NONBLOCK);
	if (result == -EINPROGRESS)
		result = 0;
	if (result == 0)
		goto out;

out_err:
	if (con->sock) {
		sock_release(con->sock);
		con->sock = NULL;
	} else if (sock) {
		sock_release(sock);
	}
	/*
	 * Some errors are fatal and this list might need adjusting. For other
	 * errors we try again until the max number of retries is reached.
	 */
	if (result != -EHOSTUNREACH &&
	    result != -ENETUNREACH &&
	    result != -ENETDOWN && 
	    result != -EINVAL &&
	    result != -EPROTONOSUPPORT) {
		log_print("connect %d try %d error %d", con->nodeid,
			  con->retries, result);
		mutex_unlock(&con->sock_mutex);
		msleep(1000);
		lowcomms_connect_sock(con);
		return;
	}
out:
	mutex_unlock(&con->sock_mutex);
	return;
}

static struct socket *tcp_create_listen_sock(struct connection *con,
					     struct sockaddr_storage *saddr)
{
	struct socket *sock = NULL;
	int result = 0;
	int one = 1;
	int addr_len;

	if (dlm_local_addr[0]->ss_family == AF_INET)
		addr_len = sizeof(struct sockaddr_in);
	else
		addr_len = sizeof(struct sockaddr_in6);

	/* Create a socket to communicate with */
	result = sock_create_kern(&init_net, dlm_local_addr[0]->ss_family,
				  SOCK_STREAM, IPPROTO_TCP, &sock);
	if (result < 0) {
		log_print("Can't create listening comms socket");
		goto create_out;
	}

	/* Turn off Nagle's algorithm */
	kernel_setsockopt(sock, SOL_TCP, TCP_NODELAY, (char *)&one,
			  sizeof(one));

	result = kernel_setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
				   (char *)&one, sizeof(one));

	if (result < 0) {
		log_print("Failed to set SO_REUSEADDR on socket: %d", result);
	}
	write_lock_bh(&sock->sk->sk_callback_lock);
	sock->sk->sk_user_data = con;
	save_listen_callbacks(sock);
	con->rx_action = tcp_accept_from_sock;
	con->connect_action = tcp_connect_to_sock;
	write_unlock_bh(&sock->sk->sk_callback_lock);

	/* Bind to our port */
	make_sockaddr(saddr, dlm_config.ci_tcp_port, &addr_len);
	result = sock->ops->bind(sock, (struct sockaddr *) saddr, addr_len);
	if (result < 0) {
		log_print("Can't bind to port %d", dlm_config.ci_tcp_port);
		sock_release(sock);
		sock = NULL;
		con->sock = NULL;
		goto create_out;
	}
	result = kernel_setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,
				 (char *)&one, sizeof(one));
	if (result < 0) {
		log_print("Set keepalive failed: %d", result);
	}

	result = sock->ops->listen(sock, 5);
	if (result < 0) {
		log_print("Can't listen on port %d", dlm_config.ci_tcp_port);
		sock_release(sock);
		sock = NULL;
		goto create_out;
	}

create_out:
	return sock;
}

=======
>>>>>>> upstream/android-13
/* Get local addresses */
static void init_local(void)
{
	struct sockaddr_storage sas, *addr;
	int i;

	dlm_local_count = 0;
	for (i = 0; i < DLM_MAX_ADDR_COUNT; i++) {
		if (dlm_our_addr(&sas, i))
			break;

		addr = kmemdup(&sas, sizeof(*addr), GFP_NOFS);
		if (!addr)
			break;
		dlm_local_addr[dlm_local_count++] = addr;
	}
}

<<<<<<< HEAD
/* Initialise SCTP socket and bind to all interfaces */
static int sctp_listen_for_all(void)
{
	struct socket *sock = NULL;
	int result = -EINVAL;
	struct connection *con = nodeid2con(0, GFP_NOFS);
	int bufsize = NEEDED_RMEM;
	int one = 1;

	if (!con)
		return -ENOMEM;

	log_print("Using SCTP for communications");

	result = sock_create_kern(&init_net, dlm_local_addr[0]->ss_family,
				  SOCK_STREAM, IPPROTO_SCTP, &sock);
	if (result < 0) {
		log_print("Can't create comms socket, check SCTP is loaded");
		goto out;
	}

	result = kernel_setsockopt(sock, SOL_SOCKET, SO_RCVBUFFORCE,
				 (char *)&bufsize, sizeof(bufsize));
	if (result)
		log_print("Error increasing buffer space on socket %d", result);

	result = kernel_setsockopt(sock, SOL_SCTP, SCTP_NODELAY, (char *)&one,
				   sizeof(one));
	if (result < 0)
		log_print("Could not set SCTP NODELAY error %d\n", result);

	write_lock_bh(&sock->sk->sk_callback_lock);
	/* Init con struct */
	sock->sk->sk_user_data = con;
	save_listen_callbacks(sock);
	con->sock = sock;
	con->sock->sk->sk_data_ready = lowcomms_data_ready;
	con->rx_action = sctp_accept_from_sock;
	con->connect_action = sctp_connect_to_sock;

	write_unlock_bh(&sock->sk->sk_callback_lock);

	/* Bind to all addresses. */
	if (sctp_bind_addrs(con, dlm_config.ci_tcp_port))
		goto create_delsock;

	result = sock->ops->listen(sock, 5);
	if (result < 0) {
		log_print("Can't set socket listening");
		goto create_delsock;
	}

	return 0;

create_delsock:
	sock_release(sock);
	con->sock = NULL;
out:
	return result;
}

static int tcp_listen_for_all(void)
{
	struct socket *sock = NULL;
	struct connection *con = nodeid2con(0, GFP_NOFS);
	int result = -EINVAL;

	if (!con)
		return -ENOMEM;

	/* We don't support multi-homed hosts */
	if (dlm_local_addr[1] != NULL) {
		log_print("TCP protocol can't handle multi-homed hosts, "
			  "try SCTP");
		return -EINVAL;
	}

	log_print("Using TCP for communications");

	sock = tcp_create_listen_sock(con, dlm_local_addr[0]);
	if (sock) {
		add_sock(sock, con);
		result = 0;
	}
	else {
		result = -EADDRINUSE;
	}

	return result;
}



=======
static void deinit_local(void)
{
	int i;

	for (i = 0; i < dlm_local_count; i++)
		kfree(dlm_local_addr[i]);
}

>>>>>>> upstream/android-13
static struct writequeue_entry *new_writequeue_entry(struct connection *con,
						     gfp_t allocation)
{
	struct writequeue_entry *entry;

<<<<<<< HEAD
	entry = kmalloc(sizeof(struct writequeue_entry), allocation);
	if (!entry)
		return NULL;

	entry->page = alloc_page(allocation);
=======
	entry = kzalloc(sizeof(*entry), allocation);
	if (!entry)
		return NULL;

	entry->page = alloc_page(allocation | __GFP_ZERO);
>>>>>>> upstream/android-13
	if (!entry->page) {
		kfree(entry);
		return NULL;
	}

<<<<<<< HEAD
	entry->offset = 0;
	entry->len = 0;
	entry->end = 0;
	entry->users = 0;
	entry->con = con;
=======
	entry->con = con;
	entry->users = 1;
	kref_init(&entry->ref);
	INIT_LIST_HEAD(&entry->msgs);
>>>>>>> upstream/android-13

	return entry;
}

<<<<<<< HEAD
void *dlm_lowcomms_get_buffer(int nodeid, int len, gfp_t allocation, char **ppc)
{
	struct connection *con;
	struct writequeue_entry *e;
	int offset = 0;

	con = nodeid2con(nodeid, allocation);
	if (!con)
		return NULL;

	spin_lock(&con->writequeue_lock);
	e = list_entry(con->writequeue.prev, struct writequeue_entry, list);
	if ((&e->list == &con->writequeue) ||
	    (PAGE_SIZE - e->end < len)) {
		e = NULL;
	} else {
		offset = e->end;
		e->end += len;
		e->users++;
	}
	spin_unlock(&con->writequeue_lock);

	if (e) {
	got_one:
		*ppc = page_address(e->page) + offset;
		return e;
	}

	e = new_writequeue_entry(con, allocation);
	if (e) {
		spin_lock(&con->writequeue_lock);
		offset = e->end;
		e->end += len;
		e->users++;
		list_add_tail(&e->list, &con->writequeue);
		spin_unlock(&con->writequeue_lock);
		goto got_one;
	}
	return NULL;
}

void dlm_lowcomms_commit_buffer(void *mh)
{
	struct writequeue_entry *e = (struct writequeue_entry *)mh;
=======
static struct writequeue_entry *new_wq_entry(struct connection *con, int len,
					     gfp_t allocation, char **ppc,
					     void (*cb)(struct dlm_mhandle *mh),
					     struct dlm_mhandle *mh)
{
	struct writequeue_entry *e;

	spin_lock(&con->writequeue_lock);
	if (!list_empty(&con->writequeue)) {
		e = list_last_entry(&con->writequeue, struct writequeue_entry, list);
		if (DLM_WQ_REMAIN_BYTES(e) >= len) {
			kref_get(&e->ref);

			*ppc = page_address(e->page) + e->end;
			if (cb)
				cb(mh);

			e->end += len;
			e->users++;
			spin_unlock(&con->writequeue_lock);

			return e;
		}
	}
	spin_unlock(&con->writequeue_lock);

	e = new_writequeue_entry(con, allocation);
	if (!e)
		return NULL;

	kref_get(&e->ref);
	*ppc = page_address(e->page);
	e->end += len;
	atomic_inc(&con->writequeue_cnt);

	spin_lock(&con->writequeue_lock);
	if (cb)
		cb(mh);

	list_add_tail(&e->list, &con->writequeue);
	spin_unlock(&con->writequeue_lock);

	return e;
};

static struct dlm_msg *dlm_lowcomms_new_msg_con(struct connection *con, int len,
						gfp_t allocation, char **ppc,
						void (*cb)(struct dlm_mhandle *mh),
						struct dlm_mhandle *mh)
{
	struct writequeue_entry *e;
	struct dlm_msg *msg;
	bool sleepable;

	msg = kzalloc(sizeof(*msg), allocation);
	if (!msg)
		return NULL;

	/* this mutex is being used as a wait to avoid multiple "fast"
	 * new writequeue page list entry allocs in new_wq_entry in
	 * normal operation which is sleepable context. Without it
	 * we could end in multiple writequeue entries with one
	 * dlm message because multiple callers were waiting at
	 * the writequeue_lock in new_wq_entry().
	 */
	sleepable = gfpflags_normal_context(allocation);
	if (sleepable)
		mutex_lock(&con->wq_alloc);

	kref_init(&msg->ref);

	e = new_wq_entry(con, len, allocation, ppc, cb, mh);
	if (!e) {
		if (sleepable)
			mutex_unlock(&con->wq_alloc);

		kfree(msg);
		return NULL;
	}

	if (sleepable)
		mutex_unlock(&con->wq_alloc);

	msg->ppc = *ppc;
	msg->len = len;
	msg->entry = e;

	return msg;
}

struct dlm_msg *dlm_lowcomms_new_msg(int nodeid, int len, gfp_t allocation,
				     char **ppc, void (*cb)(struct dlm_mhandle *mh),
				     struct dlm_mhandle *mh)
{
	struct connection *con;
	struct dlm_msg *msg;
	int idx;

	if (len > DLM_MAX_SOCKET_BUFSIZE ||
	    len < sizeof(struct dlm_header)) {
		BUILD_BUG_ON(PAGE_SIZE < DLM_MAX_SOCKET_BUFSIZE);
		log_print("failed to allocate a buffer of size %d", len);
		WARN_ON(1);
		return NULL;
	}

	idx = srcu_read_lock(&connections_srcu);
	con = nodeid2con(nodeid, allocation);
	if (!con) {
		srcu_read_unlock(&connections_srcu, idx);
		return NULL;
	}

	msg = dlm_lowcomms_new_msg_con(con, len, allocation, ppc, cb, mh);
	if (!msg) {
		srcu_read_unlock(&connections_srcu, idx);
		return NULL;
	}

	/* we assume if successful commit must called */
	msg->idx = idx;
	return msg;
}

static void _dlm_lowcomms_commit_msg(struct dlm_msg *msg)
{
	struct writequeue_entry *e = msg->entry;
>>>>>>> upstream/android-13
	struct connection *con = e->con;
	int users;

	spin_lock(&con->writequeue_lock);
<<<<<<< HEAD
	users = --e->users;
	if (users)
		goto out;
	e->len = e->end - e->offset;
=======
	kref_get(&msg->ref);
	list_add(&msg->list, &e->msgs);

	users = --e->users;
	if (users)
		goto out;

	e->len = DLM_WQ_LENGTH_BYTES(e);
>>>>>>> upstream/android-13
	spin_unlock(&con->writequeue_lock);

	queue_work(send_workqueue, &con->swork);
	return;

out:
	spin_unlock(&con->writequeue_lock);
	return;
}

<<<<<<< HEAD
/* Send a message */
static void send_to_sock(struct connection *con)
{
	int ret = 0;
	const int msg_flags = MSG_DONTWAIT | MSG_NOSIGNAL;
	struct writequeue_entry *e;
	int len, offset;
=======
void dlm_lowcomms_commit_msg(struct dlm_msg *msg)
{
	_dlm_lowcomms_commit_msg(msg);
	srcu_read_unlock(&connections_srcu, msg->idx);
}

void dlm_lowcomms_put_msg(struct dlm_msg *msg)
{
	kref_put(&msg->ref, dlm_msg_release);
}

/* does not held connections_srcu, usage workqueue only */
int dlm_lowcomms_resend_msg(struct dlm_msg *msg)
{
	struct dlm_msg *msg_resend;
	char *ppc;

	if (msg->retransmit)
		return 1;

	msg_resend = dlm_lowcomms_new_msg_con(msg->entry->con, msg->len,
					      GFP_ATOMIC, &ppc, NULL, NULL);
	if (!msg_resend)
		return -ENOMEM;

	msg->retransmit = true;
	kref_get(&msg->ref);
	msg_resend->orig_msg = msg;

	memcpy(ppc, msg->ppc, msg->len);
	_dlm_lowcomms_commit_msg(msg_resend);
	dlm_lowcomms_put_msg(msg_resend);

	return 0;
}

/* Send a message */
static void send_to_sock(struct connection *con)
{
	const int msg_flags = MSG_DONTWAIT | MSG_NOSIGNAL;
	struct writequeue_entry *e;
	int len, offset, ret;
>>>>>>> upstream/android-13
	int count = 0;

	mutex_lock(&con->sock_mutex);
	if (con->sock == NULL)
		goto out_connect;

	spin_lock(&con->writequeue_lock);
	for (;;) {
<<<<<<< HEAD
		e = list_entry(con->writequeue.next, struct writequeue_entry,
			       list);
		if ((struct list_head *) e == &con->writequeue)
			break;

=======
		e = con_next_wq(con);
		if (!e)
			break;

		e = list_first_entry(&con->writequeue, struct writequeue_entry, list);
>>>>>>> upstream/android-13
		len = e->len;
		offset = e->offset;
		BUG_ON(len == 0 && e->users == 0);
		spin_unlock(&con->writequeue_lock);

<<<<<<< HEAD
		ret = 0;
		if (len) {
			ret = kernel_sendpage(con->sock, e->page, offset, len,
					      msg_flags);
			if (ret == -EAGAIN || ret == 0) {
				if (ret == -EAGAIN &&
				    test_bit(SOCKWQ_ASYNC_NOSPACE, &con->sock->flags) &&
				    !test_and_set_bit(CF_APP_LIMITED, &con->flags)) {
					/* Notify TCP that we're limited by the
					 * application window size.
					 */
					set_bit(SOCK_NOSPACE, &con->sock->flags);
					con->sock->sk->sk_write_pending++;
				}
				cond_resched();
				goto out;
			} else if (ret < 0)
				goto send_error;
		}
=======
		ret = kernel_sendpage(con->sock, e->page, offset, len,
				      msg_flags);
		if (ret == -EAGAIN || ret == 0) {
			if (ret == -EAGAIN &&
			    test_bit(SOCKWQ_ASYNC_NOSPACE, &con->sock->flags) &&
			    !test_and_set_bit(CF_APP_LIMITED, &con->flags)) {
				/* Notify TCP that we're limited by the
				 * application window size.
				 */
				set_bit(SOCK_NOSPACE, &con->sock->flags);
				con->sock->sk->sk_write_pending++;
			}
			cond_resched();
			goto out;
		} else if (ret < 0)
			goto out;
>>>>>>> upstream/android-13

		/* Don't starve people filling buffers */
		if (++count >= MAX_SEND_MSG_COUNT) {
			cond_resched();
			count = 0;
		}

		spin_lock(&con->writequeue_lock);
		writequeue_entry_complete(e, ret);
	}
	spin_unlock(&con->writequeue_lock);
<<<<<<< HEAD
out:
	mutex_unlock(&con->sock_mutex);
	return;

send_error:
	mutex_unlock(&con->sock_mutex);
	close_connection(con, true, false, true);
	/* Requeue the send work. When the work daemon runs again, it will try
	   a new connection, then call this function again. */
	queue_work(send_workqueue, &con->swork);
=======

	/* close if we got EOF */
	if (test_and_clear_bit(CF_EOF, &con->flags)) {
		mutex_unlock(&con->sock_mutex);
		close_connection(con, false, false, true);

		/* handling for tcp shutdown */
		clear_bit(CF_SHUTDOWN, &con->flags);
		wake_up(&con->shutdown_wait);
	} else {
		mutex_unlock(&con->sock_mutex);
	}

	return;

out:
	mutex_unlock(&con->sock_mutex);
>>>>>>> upstream/android-13
	return;

out_connect:
	mutex_unlock(&con->sock_mutex);
	queue_work(send_workqueue, &con->swork);
	cond_resched();
}

static void clean_one_writequeue(struct connection *con)
{
	struct writequeue_entry *e, *safe;

	spin_lock(&con->writequeue_lock);
	list_for_each_entry_safe(e, safe, &con->writequeue, list) {
<<<<<<< HEAD
		list_del(&e->list);
=======
>>>>>>> upstream/android-13
		free_entry(e);
	}
	spin_unlock(&con->writequeue_lock);
}

/* Called from recovery when it knows that a node has
   left the cluster */
int dlm_lowcomms_close(int nodeid)
{
	struct connection *con;
	struct dlm_node_addr *na;
<<<<<<< HEAD

	log_print("closing connection to node %d", nodeid);
=======
	int idx;

	log_print("closing connection to node %d", nodeid);
	idx = srcu_read_lock(&connections_srcu);
>>>>>>> upstream/android-13
	con = nodeid2con(nodeid, 0);
	if (con) {
		set_bit(CF_CLOSE, &con->flags);
		close_connection(con, true, true, true);
		clean_one_writequeue(con);
<<<<<<< HEAD
	}
=======
		if (con->othercon)
			clean_one_writequeue(con->othercon);
	}
	srcu_read_unlock(&connections_srcu, idx);
>>>>>>> upstream/android-13

	spin_lock(&dlm_node_addrs_spin);
	na = find_node_addr(nodeid);
	if (na) {
		list_del(&na->list);
		while (na->addr_count--)
			kfree(na->addr[na->addr_count]);
		kfree(na);
	}
	spin_unlock(&dlm_node_addrs_spin);

	return 0;
}

/* Receive workqueue function */
static void process_recv_sockets(struct work_struct *work)
{
	struct connection *con = container_of(work, struct connection, rwork);
<<<<<<< HEAD
	int err;

	clear_bit(CF_READ_PENDING, &con->flags);
	do {
		err = con->rx_action(con);
	} while (!err);
=======

	clear_bit(CF_READ_PENDING, &con->flags);
	receive_from_sock(con);
}

static void process_listen_recv_socket(struct work_struct *work)
{
	accept_from_sock(&listen_con);
}

static void dlm_connect(struct connection *con)
{
	struct sockaddr_storage addr;
	int result, addr_len;
	struct socket *sock;
	unsigned int mark;

	/* Some odd races can cause double-connects, ignore them */
	if (con->retries++ > MAX_CONNECT_RETRIES)
		return;

	if (con->sock) {
		log_print("node %d already connected.", con->nodeid);
		return;
	}

	memset(&addr, 0, sizeof(addr));
	result = nodeid_to_addr(con->nodeid, &addr, NULL,
				dlm_proto_ops->try_new_addr, &mark);
	if (result < 0) {
		log_print("no address for nodeid %d", con->nodeid);
		return;
	}

	/* Create a socket to communicate with */
	result = sock_create_kern(&init_net, dlm_local_addr[0]->ss_family,
				  SOCK_STREAM, dlm_proto_ops->proto, &sock);
	if (result < 0)
		goto socket_err;

	sock_set_mark(sock->sk, mark);
	dlm_proto_ops->sockopts(sock);

	add_sock(sock, con);

	result = dlm_proto_ops->bind(sock);
	if (result < 0)
		goto add_sock_err;

	log_print_ratelimited("connecting to %d", con->nodeid);
	make_sockaddr(&addr, dlm_config.ci_tcp_port, &addr_len);
	result = dlm_proto_ops->connect(con, sock, (struct sockaddr *)&addr,
					addr_len);
	if (result < 0)
		goto add_sock_err;

	return;

add_sock_err:
	dlm_close_sock(&con->sock);

socket_err:
	/*
	 * Some errors are fatal and this list might need adjusting. For other
	 * errors we try again until the max number of retries is reached.
	 */
	if (result != -EHOSTUNREACH &&
	    result != -ENETUNREACH &&
	    result != -ENETDOWN &&
	    result != -EINVAL &&
	    result != -EPROTONOSUPPORT) {
		log_print("connect %d try %d error %d", con->nodeid,
			  con->retries, result);
		msleep(1000);
		lowcomms_connect_sock(con);
	}
>>>>>>> upstream/android-13
}

/* Send workqueue function */
static void process_send_sockets(struct work_struct *work)
{
	struct connection *con = container_of(work, struct connection, swork);

<<<<<<< HEAD
	clear_bit(CF_WRITE_PENDING, &con->flags);
	if (con->sock == NULL) /* not mutex protected so check it inside too */
		con->connect_action(con);
=======
	WARN_ON(test_bit(CF_IS_OTHERCON, &con->flags));

	clear_bit(CF_WRITE_PENDING, &con->flags);

	if (test_and_clear_bit(CF_RECONNECT, &con->flags)) {
		close_connection(con, false, false, true);
		dlm_midcomms_unack_msg_resend(con->nodeid);
	}

	if (con->sock == NULL) {
		if (test_and_clear_bit(CF_DELAY_CONNECT, &con->flags))
			msleep(1000);

		mutex_lock(&con->sock_mutex);
		dlm_connect(con);
		mutex_unlock(&con->sock_mutex);
	}

>>>>>>> upstream/android-13
	if (!list_empty(&con->writequeue))
		send_to_sock(con);
}

<<<<<<< HEAD

/* Discard all entries on the write queues */
static void clean_writequeues(void)
{
	foreach_conn(clean_one_writequeue);
}

static void work_stop(void)
{
	if (recv_workqueue)
		destroy_workqueue(recv_workqueue);
	if (send_workqueue)
		destroy_workqueue(send_workqueue);
=======
static void work_stop(void)
{
	if (recv_workqueue) {
		destroy_workqueue(recv_workqueue);
		recv_workqueue = NULL;
	}

	if (send_workqueue) {
		destroy_workqueue(send_workqueue);
		send_workqueue = NULL;
	}
>>>>>>> upstream/android-13
}

static int work_start(void)
{
<<<<<<< HEAD
	recv_workqueue = alloc_workqueue("dlm_recv",
					 WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
=======
	recv_workqueue = alloc_ordered_workqueue("dlm_recv", WQ_MEM_RECLAIM);
>>>>>>> upstream/android-13
	if (!recv_workqueue) {
		log_print("can't start dlm_recv");
		return -ENOMEM;
	}

<<<<<<< HEAD
	send_workqueue = alloc_workqueue("dlm_send",
					 WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
	if (!send_workqueue) {
		log_print("can't start dlm_send");
		destroy_workqueue(recv_workqueue);
=======
	send_workqueue = alloc_ordered_workqueue("dlm_send", WQ_MEM_RECLAIM);
	if (!send_workqueue) {
		log_print("can't start dlm_send");
		destroy_workqueue(recv_workqueue);
		recv_workqueue = NULL;
>>>>>>> upstream/android-13
		return -ENOMEM;
	}

	return 0;
}

<<<<<<< HEAD
=======
static void shutdown_conn(struct connection *con)
{
	if (dlm_proto_ops->shutdown_action)
		dlm_proto_ops->shutdown_action(con);
}

void dlm_lowcomms_shutdown(void)
{
	int idx;

	/* Set all the flags to prevent any
	 * socket activity.
	 */
	dlm_allow_conn = 0;

	if (recv_workqueue)
		flush_workqueue(recv_workqueue);
	if (send_workqueue)
		flush_workqueue(send_workqueue);

	dlm_close_sock(&listen_con.sock);

	idx = srcu_read_lock(&connections_srcu);
	foreach_conn(shutdown_conn);
	srcu_read_unlock(&connections_srcu, idx);
}

>>>>>>> upstream/android-13
static void _stop_conn(struct connection *con, bool and_other)
{
	mutex_lock(&con->sock_mutex);
	set_bit(CF_CLOSE, &con->flags);
	set_bit(CF_READ_PENDING, &con->flags);
	set_bit(CF_WRITE_PENDING, &con->flags);
	if (con->sock && con->sock->sk) {
		write_lock_bh(&con->sock->sk->sk_callback_lock);
		con->sock->sk->sk_user_data = NULL;
		write_unlock_bh(&con->sock->sk->sk_callback_lock);
	}
	if (con->othercon && and_other)
		_stop_conn(con->othercon, false);
	mutex_unlock(&con->sock_mutex);
}

static void stop_conn(struct connection *con)
{
	_stop_conn(con, true);
}

<<<<<<< HEAD
static void free_conn(struct connection *con)
{
	close_connection(con, true, true, true);
	if (con->othercon)
		kmem_cache_free(con_cache, con->othercon);
	hlist_del(&con->list);
	kmem_cache_free(con_cache, con);
=======
static void connection_release(struct rcu_head *rcu)
{
	struct connection *con = container_of(rcu, struct connection, rcu);

	kfree(con->rx_buf);
	kfree(con);
}

static void free_conn(struct connection *con)
{
	close_connection(con, true, true, true);
	spin_lock(&connections_lock);
	hlist_del_rcu(&con->list);
	spin_unlock(&connections_lock);
	if (con->othercon) {
		clean_one_writequeue(con->othercon);
		call_srcu(&connections_srcu, &con->othercon->rcu,
			  connection_release);
	}
	clean_one_writequeue(con);
	call_srcu(&connections_srcu, &con->rcu, connection_release);
>>>>>>> upstream/android-13
}

static void work_flush(void)
{
	int ok;
	int i;
<<<<<<< HEAD
	struct hlist_node *n;
	struct connection *con;

	if (recv_workqueue)
		flush_workqueue(recv_workqueue);
	if (send_workqueue)
		flush_workqueue(send_workqueue);
=======
	struct connection *con;

>>>>>>> upstream/android-13
	do {
		ok = 1;
		foreach_conn(stop_conn);
		if (recv_workqueue)
			flush_workqueue(recv_workqueue);
		if (send_workqueue)
			flush_workqueue(send_workqueue);
		for (i = 0; i < CONN_HASH_SIZE && ok; i++) {
<<<<<<< HEAD
			hlist_for_each_entry_safe(con, n,
						  &connection_hash[i], list) {
=======
			hlist_for_each_entry_rcu(con, &connection_hash[i],
						 list) {
>>>>>>> upstream/android-13
				ok &= test_bit(CF_READ_PENDING, &con->flags);
				ok &= test_bit(CF_WRITE_PENDING, &con->flags);
				if (con->othercon) {
					ok &= test_bit(CF_READ_PENDING,
						       &con->othercon->flags);
					ok &= test_bit(CF_WRITE_PENDING,
						       &con->othercon->flags);
				}
			}
		}
	} while (!ok);
}

void dlm_lowcomms_stop(void)
{
<<<<<<< HEAD
	/* Set all the flags to prevent any
	   socket activity.
	*/
	mutex_lock(&connections_lock);
	dlm_allow_conn = 0;
	mutex_unlock(&connections_lock);
	work_flush();
	clean_writequeues();
	foreach_conn(free_conn);
	work_stop();

	kmem_cache_destroy(con_cache);
}

int dlm_lowcomms_start(void)
{
	int error = -EINVAL;
	struct connection *con;
=======
	int idx;

	idx = srcu_read_lock(&connections_srcu);
	work_flush();
	foreach_conn(free_conn);
	srcu_read_unlock(&connections_srcu, idx);
	work_stop();
	deinit_local();

	dlm_proto_ops = NULL;
}

static int dlm_listen_for_all(void)
{
	struct socket *sock;
	int result;

	log_print("Using %s for communications",
		  dlm_proto_ops->name);

	result = dlm_proto_ops->listen_validate();
	if (result < 0)
		return result;

	result = sock_create_kern(&init_net, dlm_local_addr[0]->ss_family,
				  SOCK_STREAM, dlm_proto_ops->proto, &sock);
	if (result < 0) {
		log_print("Can't create comms socket, check SCTP is loaded");
		goto out;
	}

	sock_set_mark(sock->sk, dlm_config.ci_mark);
	dlm_proto_ops->listen_sockopts(sock);

	result = dlm_proto_ops->listen_bind(sock);
	if (result < 0)
		goto out;

	save_listen_callbacks(sock);
	add_listen_sock(sock, &listen_con);

	INIT_WORK(&listen_con.rwork, process_listen_recv_socket);
	result = sock->ops->listen(sock, 5);
	if (result < 0) {
		dlm_close_sock(&listen_con.sock);
		goto out;
	}

	return 0;

out:
	sock_release(sock);
	return result;
}

static int dlm_tcp_bind(struct socket *sock)
{
	struct sockaddr_storage src_addr;
	int result, addr_len;

	/* Bind to our cluster-known address connecting to avoid
	 * routing problems.
	 */
	memcpy(&src_addr, dlm_local_addr[0], sizeof(src_addr));
	make_sockaddr(&src_addr, 0, &addr_len);

	result = sock->ops->bind(sock, (struct sockaddr *)&src_addr,
				 addr_len);
	if (result < 0) {
		/* This *may* not indicate a critical error */
		log_print("could not bind for connect: %d", result);
	}

	return 0;
}

static int dlm_tcp_connect(struct connection *con, struct socket *sock,
			   struct sockaddr *addr, int addr_len)
{
	int ret;

	ret = sock->ops->connect(sock, addr, addr_len, O_NONBLOCK);
	switch (ret) {
	case -EINPROGRESS:
		fallthrough;
	case 0:
		return 0;
	}

	return ret;
}

static int dlm_tcp_listen_validate(void)
{
	/* We don't support multi-homed hosts */
	if (dlm_local_count > 1) {
		log_print("TCP protocol can't handle multi-homed hosts, try SCTP");
		return -EINVAL;
	}

	return 0;
}

static void dlm_tcp_sockopts(struct socket *sock)
{
	/* Turn off Nagle's algorithm */
	tcp_sock_set_nodelay(sock->sk);
}

static void dlm_tcp_listen_sockopts(struct socket *sock)
{
	dlm_tcp_sockopts(sock);
	sock_set_reuseaddr(sock->sk);
}

static int dlm_tcp_listen_bind(struct socket *sock)
{
	int addr_len;

	/* Bind to our port */
	make_sockaddr(dlm_local_addr[0], dlm_config.ci_tcp_port, &addr_len);
	return sock->ops->bind(sock, (struct sockaddr *)dlm_local_addr[0],
			       addr_len);
}

static const struct dlm_proto_ops dlm_tcp_ops = {
	.name = "TCP",
	.proto = IPPROTO_TCP,
	.connect = dlm_tcp_connect,
	.sockopts = dlm_tcp_sockopts,
	.bind = dlm_tcp_bind,
	.listen_validate = dlm_tcp_listen_validate,
	.listen_sockopts = dlm_tcp_listen_sockopts,
	.listen_bind = dlm_tcp_listen_bind,
	.shutdown_action = dlm_tcp_shutdown,
	.eof_condition = tcp_eof_condition,
};

static int dlm_sctp_bind(struct socket *sock)
{
	return sctp_bind_addrs(sock, 0);
}

static int dlm_sctp_connect(struct connection *con, struct socket *sock,
			    struct sockaddr *addr, int addr_len)
{
	int ret;

	/*
	 * Make sock->ops->connect() function return in specified time,
	 * since O_NONBLOCK argument in connect() function does not work here,
	 * then, we should restore the default value of this attribute.
	 */
	sock_set_sndtimeo(sock->sk, 5);
	ret = sock->ops->connect(sock, addr, addr_len, 0);
	sock_set_sndtimeo(sock->sk, 0);
	if (ret < 0)
		return ret;

	if (!test_and_set_bit(CF_CONNECTED, &con->flags))
		log_print("successful connected to node %d", con->nodeid);

	return 0;
}

static int dlm_sctp_listen_validate(void)
{
	if (!IS_ENABLED(CONFIG_IP_SCTP)) {
		log_print("SCTP is not enabled by this kernel");
		return -EOPNOTSUPP;
	}

	request_module("sctp");
	return 0;
}

static int dlm_sctp_bind_listen(struct socket *sock)
{
	return sctp_bind_addrs(sock, dlm_config.ci_tcp_port);
}

static void dlm_sctp_sockopts(struct socket *sock)
{
	/* Turn off Nagle's algorithm */
	sctp_sock_set_nodelay(sock->sk);
	sock_set_rcvbuf(sock->sk, NEEDED_RMEM);
}

static const struct dlm_proto_ops dlm_sctp_ops = {
	.name = "SCTP",
	.proto = IPPROTO_SCTP,
	.try_new_addr = true,
	.connect = dlm_sctp_connect,
	.sockopts = dlm_sctp_sockopts,
	.bind = dlm_sctp_bind,
	.listen_validate = dlm_sctp_listen_validate,
	.listen_sockopts = dlm_sctp_sockopts,
	.listen_bind = dlm_sctp_bind_listen,
};

int dlm_lowcomms_start(void)
{
	int error = -EINVAL;
>>>>>>> upstream/android-13
	int i;

	for (i = 0; i < CONN_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&connection_hash[i]);

	init_local();
	if (!dlm_local_count) {
		error = -ENOTCONN;
		log_print("no local IP address has been set");
		goto fail;
	}

<<<<<<< HEAD
	error = -ENOMEM;
	con_cache = kmem_cache_create("dlm_conn", sizeof(struct connection),
				      __alignof__(struct connection), 0,
				      NULL);
	if (!con_cache)
		goto fail;

	error = work_start();
	if (error)
		goto fail_destroy;
=======
	INIT_WORK(&listen_con.rwork, process_listen_recv_socket);

	error = work_start();
	if (error)
		goto fail_local;
>>>>>>> upstream/android-13

	dlm_allow_conn = 1;

	/* Start listening */
<<<<<<< HEAD
	if (dlm_config.ci_protocol == 0)
		error = tcp_listen_for_all();
	else
		error = sctp_listen_for_all();
	if (error)
		goto fail_unlisten;

	return 0;

fail_unlisten:
	dlm_allow_conn = 0;
	con = nodeid2con(0,0);
	if (con) {
		close_connection(con, false, true, true);
		kmem_cache_free(con_cache, con);
	}
fail_destroy:
	kmem_cache_destroy(con_cache);
=======
	switch (dlm_config.ci_protocol) {
	case DLM_PROTO_TCP:
		dlm_proto_ops = &dlm_tcp_ops;
		break;
	case DLM_PROTO_SCTP:
		dlm_proto_ops = &dlm_sctp_ops;
		break;
	default:
		log_print("Invalid protocol identifier %d set",
			  dlm_config.ci_protocol);
		error = -EINVAL;
		goto fail_proto_ops;
	}

	error = dlm_listen_for_all();
	if (error)
		goto fail_listen;

	return 0;

fail_listen:
	dlm_proto_ops = NULL;
fail_proto_ops:
	dlm_allow_conn = 0;
	dlm_close_sock(&listen_con.sock);
	work_stop();
fail_local:
	deinit_local();
>>>>>>> upstream/android-13
fail:
	return error;
}

void dlm_lowcomms_exit(void)
{
	struct dlm_node_addr *na, *safe;

	spin_lock(&dlm_node_addrs_spin);
	list_for_each_entry_safe(na, safe, &dlm_node_addrs, list) {
		list_del(&na->list);
		while (na->addr_count--)
			kfree(na->addr[na->addr_count]);
		kfree(na);
	}
	spin_unlock(&dlm_node_addrs_spin);
}
