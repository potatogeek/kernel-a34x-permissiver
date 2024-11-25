<<<<<<< HEAD
/*
 * aQuantia Corporation Network Driver
 * Copyright (C) 2014-2017 aQuantia Corporation. All rights reserved
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Atlantic Network Driver
 *
 * Copyright (C) 2014-2019 aQuantia Corporation
 * Copyright (C) 2019-2020 Marvell International Ltd.
>>>>>>> upstream/android-13
 */

/* File aq_ring.h: Declaration of functions for Rx/Tx rings. */

#ifndef AQ_RING_H
#define AQ_RING_H

#include "aq_common.h"

struct page;
struct aq_nic_cfg_s;

<<<<<<< HEAD
=======
struct aq_rxpage {
	struct page *page;
	dma_addr_t daddr;
	unsigned int order;
	unsigned int pg_off;
};

>>>>>>> upstream/android-13
/*           TxC       SOP        DX         EOP
 *         +----------+----------+----------+-----------
 *   8bytes|len l3,l4 | pa       | pa       | pa
 *         +----------+----------+----------+-----------
 * 4/8bytes|len pkt   |len pkt   |          | skb
 *         +----------+----------+----------+-----------
<<<<<<< HEAD
 * 4/8bytes|is_txc    |len,flags |len       |len,is_eop
=======
 * 4/8bytes|is_gso    |len,flags |len       |len,is_eop
>>>>>>> upstream/android-13
 *         +----------+----------+----------+-----------
 *
 *  This aq_ring_buff_s doesn't have endianness dependency.
 *  It is __packed for cache line optimizations.
 */
struct __packed aq_ring_buff_s {
	union {
<<<<<<< HEAD
=======
		/* RX/TX */
		dma_addr_t pa;
>>>>>>> upstream/android-13
		/* RX */
		struct {
			u32 rss_hash;
			u16 next;
			u8 is_hash_l4;
			u8 rsvd1;
<<<<<<< HEAD
			struct page *page;
=======
			struct aq_rxpage rxdata;
			u16 vlan_rx_tag;
>>>>>>> upstream/android-13
		};
		/* EOP */
		struct {
			dma_addr_t pa_eop;
			struct sk_buff *skb;
		};
<<<<<<< HEAD
		/* DX */
		struct {
			dma_addr_t pa;
		};
		/* SOP */
		struct {
			dma_addr_t pa_sop;
			u32 len_pkt_sop;
		};
=======
>>>>>>> upstream/android-13
		/* TxC */
		struct {
			u32 mss;
			u8 len_l2;
			u8 len_l3;
			u8 len_l4;
			u8 is_ipv6:1;
			u8 rsvd2:7;
			u32 len_pkt;
<<<<<<< HEAD
=======
			u16 vlan_tx_tag;
>>>>>>> upstream/android-13
		};
	};
	union {
		struct {
<<<<<<< HEAD
			u16 len;
=======
			u32 len:16;
>>>>>>> upstream/android-13
			u32 is_ip_cso:1;
			u32 is_udp_cso:1;
			u32 is_tcp_cso:1;
			u32 is_cso_err:1;
			u32 is_sop:1;
			u32 is_eop:1;
<<<<<<< HEAD
			u32 is_txc:1;
			u32 is_mapped:1;
			u32 is_cleaned:1;
			u32 is_error:1;
			u32 rsvd3:6;
=======
			u32 is_gso_tcp:1;
			u32 is_gso_udp:1;
			u32 is_mapped:1;
			u32 is_cleaned:1;
			u32 is_error:1;
			u32 is_vlan:1;
			u32 is_lro:1;
			u32 rsvd3:3;
>>>>>>> upstream/android-13
			u16 eop_index;
			u16 rsvd4;
		};
		u64 flags;
	};
};

struct aq_ring_stats_rx_s {
<<<<<<< HEAD
=======
	struct u64_stats_sync syncp;	/* must be first */
>>>>>>> upstream/android-13
	u64 errors;
	u64 packets;
	u64 bytes;
	u64 lro_packets;
	u64 jumbo_packets;
<<<<<<< HEAD
};

struct aq_ring_stats_tx_s {
=======
	u64 alloc_fails;
	u64 skb_alloc_fails;
	u64 polls;
	u64 pg_losts;
	u64 pg_flips;
	u64 pg_reuses;
};

struct aq_ring_stats_tx_s {
	struct u64_stats_sync syncp;	/* must be first */
>>>>>>> upstream/android-13
	u64 errors;
	u64 packets;
	u64 bytes;
	u64 queue_restarts;
};

union aq_ring_stats_s {
	struct aq_ring_stats_rx_s rx;
	struct aq_ring_stats_tx_s tx;
};

<<<<<<< HEAD
=======
enum atl_ring_type {
	ATL_RING_TX,
	ATL_RING_RX,
};

>>>>>>> upstream/android-13
struct aq_ring_s {
	struct aq_ring_buff_s *buff_ring;
	u8 *dx_ring;		/* descriptors ring, dma shared mem */
	struct aq_nic_s *aq_nic;
	unsigned int idx;	/* for HW layer registers operations */
	unsigned int hw_head;
	unsigned int sw_head;
	unsigned int sw_tail;
	unsigned int size;	/* descriptors number */
	unsigned int dx_size;	/* TX or RX descriptor size,  */
				/* stored here for fater math */
<<<<<<< HEAD
	union aq_ring_stats_s stats;
	dma_addr_t dx_ring_pa;
=======
	unsigned int page_order;
	union aq_ring_stats_s stats;
	dma_addr_t dx_ring_pa;
	enum atl_ring_type ring_type;
>>>>>>> upstream/android-13
};

struct aq_ring_param_s {
	unsigned int vec_idx;
	unsigned int cpu;
	cpumask_t affinity_mask;
};

<<<<<<< HEAD
=======
static inline void *aq_buf_vaddr(struct aq_rxpage *rxpage)
{
	return page_to_virt(rxpage->page) + rxpage->pg_off;
}

static inline dma_addr_t aq_buf_daddr(struct aq_rxpage *rxpage)
{
	return rxpage->daddr + rxpage->pg_off;
}

>>>>>>> upstream/android-13
static inline unsigned int aq_ring_next_dx(struct aq_ring_s *self,
					   unsigned int dx)
{
	return (++dx >= self->size) ? 0U : dx;
}

static inline unsigned int aq_ring_avail_dx(struct aq_ring_s *self)
{
	return (((self->sw_tail >= self->sw_head)) ?
		(self->size - 1) - self->sw_tail + self->sw_head :
		self->sw_head - self->sw_tail - 1);
}

struct aq_ring_s *aq_ring_tx_alloc(struct aq_ring_s *self,
				   struct aq_nic_s *aq_nic,
				   unsigned int idx,
				   struct aq_nic_cfg_s *aq_nic_cfg);
struct aq_ring_s *aq_ring_rx_alloc(struct aq_ring_s *self,
				   struct aq_nic_s *aq_nic,
				   unsigned int idx,
				   struct aq_nic_cfg_s *aq_nic_cfg);
<<<<<<< HEAD
int aq_ring_init(struct aq_ring_s *self);
=======
int aq_ring_init(struct aq_ring_s *self, const enum atl_ring_type ring_type);
>>>>>>> upstream/android-13
void aq_ring_rx_deinit(struct aq_ring_s *self);
void aq_ring_free(struct aq_ring_s *self);
void aq_ring_update_queue_state(struct aq_ring_s *ring);
void aq_ring_queue_wake(struct aq_ring_s *ring);
void aq_ring_queue_stop(struct aq_ring_s *ring);
bool aq_ring_tx_clean(struct aq_ring_s *self);
int aq_ring_rx_clean(struct aq_ring_s *self,
		     struct napi_struct *napi,
		     int *work_done,
		     int budget);
int aq_ring_rx_fill(struct aq_ring_s *self);

<<<<<<< HEAD
=======
struct aq_ring_s *aq_ring_hwts_rx_alloc(struct aq_ring_s *self,
		struct aq_nic_s *aq_nic, unsigned int idx,
		unsigned int size, unsigned int dx_size);
void aq_ring_hwts_rx_clean(struct aq_ring_s *self, struct aq_nic_s *aq_nic);

unsigned int aq_ring_fill_stats_data(struct aq_ring_s *self, u64 *data);

>>>>>>> upstream/android-13
#endif /* AQ_RING_H */
