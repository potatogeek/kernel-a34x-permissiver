/* SPDX-License-Identifier: GPL-2.0 */
/* AF_XDP internal functions
 * Copyright(c) 2018 Intel Corporation.
 */

#ifndef _LINUX_XDP_SOCK_H
#define _LINUX_XDP_SOCK_H

#include <linux/workqueue.h>
#include <linux/if_xdp.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <net/sock.h>

struct net_device;
struct xsk_queue;
<<<<<<< HEAD

struct xdp_umem_props {
	u64 chunk_mask;
	u64 size;
};

struct xdp_umem_page {
	void *addr;
	dma_addr_t dma;
};

struct xdp_umem {
	struct xsk_queue *fq;
	struct xsk_queue *cq;
	struct xdp_umem_page *pages;
	struct xdp_umem_props props;
	u32 headroom;
	u32 chunk_size_nohr;
	struct user_struct *user;
	unsigned long address;
	refcount_t users;
	struct work_struct work;
	struct page **pgs;
	u32 npgs;
	struct net_device *dev;
	u16 queue_id;
	bool zc;
	spinlock_t xsk_list_lock;
	struct list_head xsk_list;
=======
struct xdp_buff;

struct xdp_umem {
	void *addrs;
	u64 size;
	u32 headroom;
	u32 chunk_size;
	u32 chunks;
	u32 npgs;
	struct user_struct *user;
	refcount_t users;
	u8 flags;
	bool zc;
	struct page **pgs;
	int id;
	struct list_head xsk_dma_list;
	struct work_struct work;
};

struct xsk_map {
	struct bpf_map map;
	spinlock_t lock; /* Synchronize map updates */
	struct xdp_sock __rcu *xsk_map[];
>>>>>>> upstream/android-13
};

struct xdp_sock {
	/* struct sock must be the first member of struct xdp_sock */
	struct sock sk;
<<<<<<< HEAD
	struct xsk_queue *rx;
	struct net_device *dev;
	struct xdp_umem *umem;
	struct list_head flush_node;
	u16 queue_id;
	struct xsk_queue *tx ____cacheline_aligned_in_smp;
	struct list_head list;
	bool zc;
	/* Protects multiple processes in the control path */
	struct mutex mutex;
	/* Mutual exclusion of NAPI TX thread and sendmsg error paths
	 * in the SKB destructor callback.
	 */
	spinlock_t tx_completion_lock;
	u64 rx_dropped;
};

struct xdp_buff;
#ifdef CONFIG_XDP_SOCKETS
int xsk_generic_rcv(struct xdp_sock *xs, struct xdp_buff *xdp);
int xsk_rcv(struct xdp_sock *xs, struct xdp_buff *xdp);
void xsk_flush(struct xdp_sock *xs);
bool xsk_is_setup_for_bpf_map(struct xdp_sock *xs);
/* Used from netdev driver */
u64 *xsk_umem_peek_addr(struct xdp_umem *umem, u64 *addr);
void xsk_umem_discard_addr(struct xdp_umem *umem);
void xsk_umem_complete_tx(struct xdp_umem *umem, u32 nb_entries);
bool xsk_umem_consume_tx(struct xdp_umem *umem, dma_addr_t *dma, u32 *len);
void xsk_umem_consume_tx_done(struct xdp_umem *umem);
#else
=======
	struct xsk_queue *rx ____cacheline_aligned_in_smp;
	struct net_device *dev;
	struct xdp_umem *umem;
	struct list_head flush_node;
	struct xsk_buff_pool *pool;
	u16 queue_id;
	bool zc;
	enum {
		XSK_READY = 0,
		XSK_BOUND,
		XSK_UNBOUND,
	} state;

	struct xsk_queue *tx ____cacheline_aligned_in_smp;
	struct list_head tx_list;
	/* Protects generic receive. */
	spinlock_t rx_lock;

	/* Statistics */
	u64 rx_dropped;
	u64 rx_queue_full;

	struct list_head map_list;
	/* Protects map_list */
	spinlock_t map_list_lock;
	/* Protects multiple processes in the control path */
	struct mutex mutex;
	struct xsk_queue *fq_tmp; /* Only as tmp storage before bind */
	struct xsk_queue *cq_tmp; /* Only as tmp storage before bind */
};

#ifdef CONFIG_XDP_SOCKETS

int xsk_generic_rcv(struct xdp_sock *xs, struct xdp_buff *xdp);
int __xsk_map_redirect(struct xdp_sock *xs, struct xdp_buff *xdp);
void __xsk_map_flush(void);

#else

>>>>>>> upstream/android-13
static inline int xsk_generic_rcv(struct xdp_sock *xs, struct xdp_buff *xdp)
{
	return -ENOTSUPP;
}

<<<<<<< HEAD
static inline int xsk_rcv(struct xdp_sock *xs, struct xdp_buff *xdp)
{
	return -ENOTSUPP;
}

static inline void xsk_flush(struct xdp_sock *xs)
{
}

static inline bool xsk_is_setup_for_bpf_map(struct xdp_sock *xs)
{
	return false;
}
=======
static inline int __xsk_map_redirect(struct xdp_sock *xs, struct xdp_buff *xdp)
{
	return -EOPNOTSUPP;
}

static inline void __xsk_map_flush(void)
{
}

>>>>>>> upstream/android-13
#endif /* CONFIG_XDP_SOCKETS */

#endif /* _LINUX_XDP_SOCK_H */
