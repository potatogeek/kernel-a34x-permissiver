<<<<<<< HEAD
/*
 * Copyright 2015 Amazon.com, Inc. or its affiliates.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
=======
/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */
/*
 * Copyright 2015-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef ENA_H
#define ENA_H

#include <linux/bitops.h>
<<<<<<< HEAD
#include <linux/etherdevice.h>
=======
#include <linux/dim.h>
#include <linux/etherdevice.h>
#include <linux/if_vlan.h>
>>>>>>> upstream/android-13
#include <linux/inetdevice.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>

#include "ena_com.h"
#include "ena_eth_com.h"

<<<<<<< HEAD
#define DRV_MODULE_VER_MAJOR	1
#define DRV_MODULE_VER_MINOR	5
#define DRV_MODULE_VER_SUBMINOR 0

#define DRV_MODULE_NAME		"ena"
#ifndef DRV_MODULE_VERSION
#define DRV_MODULE_VERSION \
	__stringify(DRV_MODULE_VER_MAJOR) "."	\
	__stringify(DRV_MODULE_VER_MINOR) "."	\
	__stringify(DRV_MODULE_VER_SUBMINOR) "K"
#endif
=======
#define DRV_MODULE_GEN_MAJOR	2
#define DRV_MODULE_GEN_MINOR	1
#define DRV_MODULE_GEN_SUBMINOR 0

#define DRV_MODULE_NAME		"ena"
>>>>>>> upstream/android-13

#define DEVICE_NAME	"Elastic Network Adapter (ENA)"

/* 1 for AENQ + ADMIN */
#define ENA_ADMIN_MSIX_VEC		1
#define ENA_MAX_MSIX_VEC(io_queues)	(ENA_ADMIN_MSIX_VEC + (io_queues))

<<<<<<< HEAD
=======
/* The ENA buffer length fields is 16 bit long. So when PAGE_SIZE == 64kB the
 * driver passes 0.
 * Since the max packet size the ENA handles is ~9kB limit the buffer length to
 * 16kB.
 */
#if PAGE_SIZE > SZ_16K
#define ENA_PAGE_SIZE (_AC(SZ_16K, UL))
#else
#define ENA_PAGE_SIZE PAGE_SIZE
#endif

>>>>>>> upstream/android-13
#define ENA_MIN_MSIX_VEC		2

#define ENA_REG_BAR			0
#define ENA_MEM_BAR			2
#define ENA_BAR_MASK (BIT(ENA_REG_BAR) | BIT(ENA_MEM_BAR))

#define ENA_DEFAULT_RING_SIZE	(1024)
<<<<<<< HEAD

#define ENA_TX_WAKEUP_THRESH		(MAX_SKB_FRAGS + 2)
#define ENA_DEFAULT_RX_COPYBREAK	(128 - NET_IP_ALIGN)

/* limit the buffer size to 600 bytes to handle MTU changes from very
 * small to very large, in which case the number of buffers per packet
 * could exceed ENA_PKT_MAX_BUFS
 */
#define ENA_DEFAULT_MIN_RX_BUFF_ALLOC_SIZE 600
=======
#define ENA_MIN_RING_SIZE	(256)

#define ENA_MIN_NUM_IO_QUEUES	(1)

#define ENA_TX_WAKEUP_THRESH		(MAX_SKB_FRAGS + 2)
#define ENA_DEFAULT_RX_COPYBREAK	(256 - NET_IP_ALIGN)
>>>>>>> upstream/android-13

#define ENA_MIN_MTU		128

#define ENA_NAME_MAX_LEN	20
#define ENA_IRQNAME_SIZE	40

#define ENA_PKT_MAX_BUFS	19

#define ENA_RX_RSS_TABLE_LOG_SIZE  7
#define ENA_RX_RSS_TABLE_SIZE	(1 << ENA_RX_RSS_TABLE_LOG_SIZE)

<<<<<<< HEAD
#define ENA_HASH_KEY_SIZE	40

=======
>>>>>>> upstream/android-13
/* The number of tx packet completions that will be handled each NAPI poll
 * cycle is ring_size / ENA_TX_POLL_BUDGET_DIVIDER.
 */
#define ENA_TX_POLL_BUDGET_DIVIDER	4

<<<<<<< HEAD
/* Refill Rx queue when number of available descriptors is below
 * QUEUE_SIZE / ENA_RX_REFILL_THRESH_DIVIDER
 */
#define ENA_RX_REFILL_THRESH_DIVIDER	8
=======
/* Refill Rx queue when number of required descriptors is above
 * QUEUE_SIZE / ENA_RX_REFILL_THRESH_DIVIDER or ENA_RX_REFILL_THRESH_PACKET
 */
#define ENA_RX_REFILL_THRESH_DIVIDER	8
#define ENA_RX_REFILL_THRESH_PACKET	256
>>>>>>> upstream/android-13

/* Number of queues to check for missing queues per timer service */
#define ENA_MONITORED_TX_QUEUES	4
/* Max timeout packets before device reset */
#define MAX_NUM_OF_TIMEOUTED_PACKETS 128

#define ENA_TX_RING_IDX_NEXT(idx, ring_size) (((idx) + 1) & ((ring_size) - 1))

#define ENA_RX_RING_IDX_NEXT(idx, ring_size) (((idx) + 1) & ((ring_size) - 1))
#define ENA_RX_RING_IDX_ADD(idx, n, ring_size) \
	(((idx) + (n)) & ((ring_size) - 1))

#define ENA_IO_TXQ_IDX(q)	(2 * (q))
#define ENA_IO_RXQ_IDX(q)	(2 * (q) + 1)
#define ENA_IO_TXQ_IDX_TO_COMBINED_IDX(q)	((q) / 2)
#define ENA_IO_RXQ_IDX_TO_COMBINED_IDX(q)	(((q) - 1) / 2)

#define ENA_MGMNT_IRQ_IDX		0
#define ENA_IO_IRQ_FIRST_IDX		1
#define ENA_IO_IRQ_IDX(q)		(ENA_IO_IRQ_FIRST_IDX + (q))

<<<<<<< HEAD
=======
#define ENA_ADMIN_POLL_DELAY_US 100

>>>>>>> upstream/android-13
/* ENA device should send keep alive msg every 1 sec.
 * We wait for 6 sec just to be on the safe side.
 */
#define ENA_DEVICE_KALIVE_TIMEOUT	(6 * HZ)
#define ENA_MAX_NO_INTERRUPT_ITERATIONS 3

#define ENA_MMIO_DISABLE_REG_READ	BIT(0)

<<<<<<< HEAD
=======
/* The max MTU size is configured to be the ethernet frame size without
 * the overhead of the ethernet header, which can have a VLAN header, and
 * a frame check sequence (FCS).
 * The buffer size we share with the device is defined to be ENA_PAGE_SIZE
 */

#define ENA_XDP_MAX_MTU (ENA_PAGE_SIZE - ETH_HLEN - ETH_FCS_LEN -	\
			 VLAN_HLEN - XDP_PACKET_HEADROOM -		\
			 SKB_DATA_ALIGN(sizeof(struct skb_shared_info)))

#define ENA_IS_XDP_INDEX(adapter, index) (((index) >= (adapter)->xdp_first_ring) && \
	((index) < (adapter)->xdp_first_ring + (adapter)->xdp_num_queues))

>>>>>>> upstream/android-13
struct ena_irq {
	irq_handler_t handler;
	void *data;
	int cpu;
	u32 vector;
	cpumask_t affinity_hint_mask;
	char name[ENA_IRQNAME_SIZE];
};

struct ena_napi {
<<<<<<< HEAD
	struct napi_struct napi ____cacheline_aligned;
	struct ena_ring *tx_ring;
	struct ena_ring *rx_ring;
	u32 qid;
=======
	u8 first_interrupt ____cacheline_aligned;
	u8 interrupts_masked;
	struct napi_struct napi;
	struct ena_ring *tx_ring;
	struct ena_ring *rx_ring;
	struct ena_ring *xdp_ring;
	u32 qid;
	struct dim dim;
};

struct ena_calc_queue_size_ctx {
	struct ena_com_dev_get_features_ctx *get_feat_ctx;
	struct ena_com_dev *ena_dev;
	struct pci_dev *pdev;
	u32 tx_queue_size;
	u32 rx_queue_size;
	u32 max_tx_queue_size;
	u32 max_rx_queue_size;
	u16 max_tx_sgl_size;
	u16 max_rx_sgl_size;
>>>>>>> upstream/android-13
};

struct ena_tx_buffer {
	struct sk_buff *skb;
	/* num of ena desc for this specific skb
	 * (includes data desc and metadata desc)
	 */
	u32 tx_descs;
	/* num of buffers used by this skb */
	u32 num_of_bufs;

<<<<<<< HEAD
=======
	/* XDP buffer structure which is used for sending packets in
	 * the xdp queues
	 */
	struct xdp_frame *xdpf;

	/* Indicate if bufs[0] map the linear data of the skb. */
	u8 map_linear_data;

>>>>>>> upstream/android-13
	/* Used for detect missing tx packets to limit the number of prints */
	u32 print_once;
	/* Save the last jiffies to detect missing tx packets
	 *
	 * sets to non zero value on ena_start_xmit and set to zero on
	 * napi and timer_Service_routine.
	 *
	 * while this value is not protected by lock,
	 * a given packet is not expected to be handled by ena_start_xmit
	 * and by napi/timer_service at the same time.
	 */
	unsigned long last_jiffies;
	struct ena_com_buf bufs[ENA_PKT_MAX_BUFS];
} ____cacheline_aligned;

struct ena_rx_buffer {
	struct sk_buff *skb;
	struct page *page;
	u32 page_offset;
	struct ena_com_buf ena_buf;
} ____cacheline_aligned;

struct ena_stats_tx {
	u64 cnt;
	u64 bytes;
	u64 queue_stop;
	u64 prepare_ctx_err;
	u64 queue_wakeup;
	u64 dma_mapping_err;
	u64 linearize;
	u64 linearize_failed;
	u64 napi_comp;
	u64 tx_poll;
	u64 doorbells;
	u64 bad_req_id;
<<<<<<< HEAD
	u64 missed_tx;
=======
	u64 llq_buffer_copy;
	u64 missed_tx;
	u64 unmask_interrupt;
	u64 last_napi_jiffies;
>>>>>>> upstream/android-13
};

struct ena_stats_rx {
	u64 cnt;
	u64 bytes;
<<<<<<< HEAD
=======
	u64 rx_copybreak_pkt;
	u64 csum_good;
>>>>>>> upstream/android-13
	u64 refil_partial;
	u64 bad_csum;
	u64 page_alloc_fail;
	u64 skb_alloc_fail;
	u64 dma_mapping_err;
	u64 bad_desc_num;
<<<<<<< HEAD
	u64 rx_copybreak_pkt;
	u64 bad_req_id;
	u64 empty_rx_ring;
};

struct ena_ring {
	union {
		/* Holds the empty requests for TX/RX
		 * out of order completions
		 */
		u16 *free_tx_ids;
		u16 *free_rx_ids;
	};
=======
	u64 bad_req_id;
	u64 empty_rx_ring;
	u64 csum_unchecked;
	u64 xdp_aborted;
	u64 xdp_drop;
	u64 xdp_pass;
	u64 xdp_tx;
	u64 xdp_invalid;
	u64 xdp_redirect;
};

struct ena_ring {
	/* Holds the empty requests for TX/RX
	 * out of order completions
	 */
	u16 *free_ids;
>>>>>>> upstream/android-13

	union {
		struct ena_tx_buffer *tx_buffer_info;
		struct ena_rx_buffer *rx_buffer_info;
	};

	/* cache ptr to avoid using the adapter */
	struct device *dev;
	struct pci_dev *pdev;
	struct napi_struct *napi;
	struct net_device *netdev;
	struct ena_com_dev *ena_dev;
	struct ena_adapter *adapter;
	struct ena_com_io_cq *ena_com_io_cq;
	struct ena_com_io_sq *ena_com_io_sq;
<<<<<<< HEAD
=======
	struct bpf_prog *xdp_bpf_prog;
	struct xdp_rxq_info xdp_rxq;
	spinlock_t xdp_tx_lock;	/* synchronize XDP TX/Redirect traffic */
	/* Used for rx queues only to point to the xdp tx ring, to
	 * which traffic should be redirected from this rx ring.
	 */
	struct ena_ring *xdp_ring;
>>>>>>> upstream/android-13

	u16 next_to_use;
	u16 next_to_clean;
	u16 rx_copybreak;
<<<<<<< HEAD
=======
	u16 rx_headroom;
>>>>>>> upstream/android-13
	u16 qid;
	u16 mtu;
	u16 sgl_size;

	/* The maximum header length the device can handle */
	u8 tx_max_header_size;

<<<<<<< HEAD
	bool first_interrupt;
=======
	bool disable_meta_caching;
>>>>>>> upstream/android-13
	u16 no_interrupt_event_cnt;

	/* cpu for TPH */
	int cpu;
	 /* number of tx/rx_buffer_info's entries */
	int ring_size;

	enum ena_admin_placement_policy_type tx_mem_queue_type;

	struct ena_com_rx_buf_info ena_bufs[ENA_PKT_MAX_BUFS];
	u32  smoothed_interval;
	u32  per_napi_packets;
<<<<<<< HEAD
	u32  per_napi_bytes;
	enum ena_intr_moder_level moder_tbl_idx;
=======
	u16 non_empty_napi_events;
>>>>>>> upstream/android-13
	struct u64_stats_sync syncp;
	union {
		struct ena_stats_tx tx_stats;
		struct ena_stats_rx rx_stats;
	};
<<<<<<< HEAD
=======

	u8 *push_buf_intermediate_buf;
>>>>>>> upstream/android-13
	int empty_rx_queue;
} ____cacheline_aligned;

struct ena_stats_dev {
	u64 tx_timeout;
	u64 suspend;
	u64 resume;
	u64 wd_expired;
	u64 interface_up;
	u64 interface_down;
	u64 admin_q_pause;
	u64 rx_drops;
<<<<<<< HEAD
=======
	u64 tx_drops;
>>>>>>> upstream/android-13
};

enum ena_flags_t {
	ENA_FLAG_DEVICE_RUNNING,
	ENA_FLAG_DEV_UP,
	ENA_FLAG_LINK_UP,
	ENA_FLAG_MSIX_ENABLED,
	ENA_FLAG_TRIGGER_RESET,
	ENA_FLAG_ONGOING_RESET
};

/* adapter specific private data structure */
struct ena_adapter {
	struct ena_com_dev *ena_dev;
	/* OS defined structs */
	struct net_device *netdev;
	struct pci_dev *pdev;

	/* rx packets that shorter that this len will be copied to the skb
	 * header
	 */
	u32 rx_copybreak;
	u32 max_mtu;

<<<<<<< HEAD
	int num_queues;
=======
	u32 num_io_queues;
	u32 max_num_io_queues;
>>>>>>> upstream/android-13

	int msix_vecs;

	u32 missing_tx_completion_threshold;

<<<<<<< HEAD
	u32 tx_usecs, rx_usecs; /* interrupt moderation */
	u32 tx_frames, rx_frames; /* interrupt moderation */

	u32 tx_ring_size;
	u32 rx_ring_size;
=======
	u32 requested_tx_ring_size;
	u32 requested_rx_ring_size;

	u32 max_tx_ring_size;
	u32 max_rx_ring_size;
>>>>>>> upstream/android-13

	u32 msg_enable;

	u16 max_tx_sgl_size;
	u16 max_rx_sgl_size;

	u8 mac_addr[ETH_ALEN];

	unsigned long keep_alive_timeout;
	unsigned long missing_tx_completion_to;

	char name[ENA_NAME_MAX_LEN];

	unsigned long flags;
	/* TX */
	struct ena_ring tx_ring[ENA_MAX_NUM_IO_QUEUES]
		____cacheline_aligned_in_smp;

	/* RX */
	struct ena_ring rx_ring[ENA_MAX_NUM_IO_QUEUES]
		____cacheline_aligned_in_smp;

	struct ena_napi ena_napi[ENA_MAX_NUM_IO_QUEUES];

	struct ena_irq irq_tbl[ENA_MAX_MSIX_VEC(ENA_MAX_NUM_IO_QUEUES)];

	/* timer service */
	struct work_struct reset_task;
	struct timer_list timer_service;

	bool wd_state;
	bool dev_up_before_reset;
<<<<<<< HEAD
=======
	bool disable_meta_caching;
>>>>>>> upstream/android-13
	unsigned long last_keep_alive_jiffies;

	struct u64_stats_sync syncp;
	struct ena_stats_dev dev_stats;
<<<<<<< HEAD
=======
	struct ena_admin_eni_stats eni_stats;
	bool eni_stats_supported;
>>>>>>> upstream/android-13

	/* last queue index that was checked for uncompleted tx packets */
	u32 last_monitored_tx_qid;

	enum ena_regs_reset_reason_types reset_reason;
<<<<<<< HEAD
=======

	struct bpf_prog *xdp_bpf_prog;
	u32 xdp_first_ring;
	u32 xdp_num_queues;
>>>>>>> upstream/android-13
};

void ena_set_ethtool_ops(struct net_device *netdev);

void ena_dump_stats_to_dmesg(struct ena_adapter *adapter);

void ena_dump_stats_to_buf(struct ena_adapter *adapter, u8 *buf);

<<<<<<< HEAD
int ena_get_sset_count(struct net_device *netdev, int sset);

/* The ENA buffer length fields is 16 bit long. So when PAGE_SIZE == 64kB the
 * driver passas 0.
 * Since the max packet size the ENA handles is ~9kB limit the buffer length to
 * 16kB.
 */
#if PAGE_SIZE > SZ_16K
#define ENA_PAGE_SIZE SZ_16K
#else
#define ENA_PAGE_SIZE PAGE_SIZE
#endif
=======
int ena_update_hw_stats(struct ena_adapter *adapter);

int ena_update_queue_sizes(struct ena_adapter *adapter,
			   u32 new_tx_size,
			   u32 new_rx_size);

int ena_update_queue_count(struct ena_adapter *adapter, u32 new_channel_count);

int ena_get_sset_count(struct net_device *netdev, int sset);

enum ena_xdp_errors_t {
	ENA_XDP_ALLOWED = 0,
	ENA_XDP_CURRENT_MTU_TOO_LARGE,
	ENA_XDP_NO_ENOUGH_QUEUES,
};

static inline bool ena_xdp_present(struct ena_adapter *adapter)
{
	return !!adapter->xdp_bpf_prog;
}

static inline bool ena_xdp_present_ring(struct ena_ring *ring)
{
	return !!ring->xdp_bpf_prog;
}

static inline bool ena_xdp_legal_queue_count(struct ena_adapter *adapter,
					     u32 queues)
{
	return 2 * queues <= adapter->max_num_io_queues;
}

static inline enum ena_xdp_errors_t ena_xdp_allowed(struct ena_adapter *adapter)
{
	enum ena_xdp_errors_t rc = ENA_XDP_ALLOWED;

	if (adapter->netdev->mtu > ENA_XDP_MAX_MTU)
		rc = ENA_XDP_CURRENT_MTU_TOO_LARGE;
	else if (!ena_xdp_legal_queue_count(adapter, adapter->num_io_queues))
		rc = ENA_XDP_NO_ENOUGH_QUEUES;

	return rc;
}
>>>>>>> upstream/android-13

#endif /* !(ENA_H) */
