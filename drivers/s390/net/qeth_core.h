/* SPDX-License-Identifier: GPL-2.0 */
/*
 *    Copyright IBM Corp. 2007
 *    Author(s): Utz Bacher <utz.bacher@de.ibm.com>,
 *		 Frank Pavlic <fpavlic@de.ibm.com>,
 *		 Thomas Spatzier <tspat@de.ibm.com>,
 *		 Frank Blaschka <frank.blaschka@de.ibm.com>
 */

#ifndef __QETH_CORE_H__
#define __QETH_CORE_H__

<<<<<<< HEAD
=======
#include <linux/completion.h>
#include <linux/debugfs.h>
>>>>>>> upstream/android-13
#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/etherdevice.h>
#include <linux/if_vlan.h>
#include <linux/ctype.h>
#include <linux/in6.h>
#include <linux/bitops.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
#include <linux/ethtool.h>
#include <linux/hashtable.h>
#include <linux/ip.h>
#include <linux/refcount.h>
#include <linux/workqueue.h>

#include <net/ipv6.h>
#include <net/if_inet6.h>
#include <net/addrconf.h>
=======
#include <linux/hashtable.h>
#include <linux/ip.h>
#include <linux/rcupdate.h>
#include <linux/refcount.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/workqueue.h>

#include <net/dst.h>
#include <net/ip6_fib.h>
#include <net/ipv6.h>
#include <net/if_inet6.h>
#include <net/addrconf.h>
#include <net/route.h>
#include <net/sch_generic.h>
#include <net/tcp.h>
>>>>>>> upstream/android-13

#include <asm/debug.h>
#include <asm/qdio.h>
#include <asm/ccwdev.h>
#include <asm/ccwgroup.h>
#include <asm/sysinfo.h>

<<<<<<< HEAD
=======
#include <uapi/linux/if_link.h>

>>>>>>> upstream/android-13
#include "qeth_core_mpc.h"

/**
 * Debug Facility stuff
 */
enum qeth_dbf_names {
	QETH_DBF_SETUP,
	QETH_DBF_MSG,
	QETH_DBF_CTRL,
	QETH_DBF_INFOS	/* must be last element */
};

struct qeth_dbf_info {
	char name[DEBUG_MAX_NAME_LEN];
	int pages;
	int areas;
	int len;
	int level;
	struct debug_view *view;
	debug_info_t *id;
};

<<<<<<< HEAD
#define QETH_DBF_CTRL_LEN 256
=======
#define QETH_DBF_CTRL_LEN 256U
>>>>>>> upstream/android-13

#define QETH_DBF_TEXT(name, level, text) \
	debug_text_event(qeth_dbf[QETH_DBF_##name].id, level, text)

#define QETH_DBF_HEX(name, level, addr, len) \
	debug_event(qeth_dbf[QETH_DBF_##name].id, level, (void *)(addr), len)

#define QETH_DBF_MESSAGE(level, text...) \
	debug_sprintf_event(qeth_dbf[QETH_DBF_MSG].id, level, text)

#define QETH_DBF_TEXT_(name, level, text...) \
	qeth_dbf_longtext(qeth_dbf[QETH_DBF_##name].id, level, text)

#define QETH_CARD_TEXT(card, level, text) \
	debug_text_event(card->debug, level, text)

#define QETH_CARD_HEX(card, level, addr, len) \
	debug_event(card->debug, level, (void *)(addr), len)

#define QETH_CARD_MESSAGE(card, text...) \
	debug_sprintf_event(card->debug, level, text)

#define QETH_CARD_TEXT_(card, level, text...) \
	qeth_dbf_longtext(card->debug, level, text)

#define SENSE_COMMAND_REJECT_BYTE 0
#define SENSE_COMMAND_REJECT_FLAG 0x80
#define SENSE_RESETTING_EVENT_BYTE 1
#define SENSE_RESETTING_EVENT_FLAG 0x80

<<<<<<< HEAD
=======
static inline u32 qeth_get_device_id(struct ccw_device *cdev)
{
	struct ccw_dev_id dev_id;
	u32 id;

	ccw_device_get_id(cdev, &dev_id);
	id = dev_id.devno;
	id |= (u32) (dev_id.ssid << 16);

	return id;
}

>>>>>>> upstream/android-13
/*
 * Common IO related definitions
 */
#define CARD_RDEV(card) card->read.ccwdev
#define CARD_WDEV(card) card->write.ccwdev
#define CARD_DDEV(card) card->data.ccwdev
#define CARD_BUS_ID(card) dev_name(&card->gdev->dev)
#define CARD_RDEV_ID(card) dev_name(&card->read.ccwdev->dev)
#define CARD_WDEV_ID(card) dev_name(&card->write.ccwdev->dev)
#define CARD_DDEV_ID(card) dev_name(&card->data.ccwdev->dev)
<<<<<<< HEAD
#define CHANNEL_ID(channel) dev_name(&channel->ccwdev->dev)

/**
 * card stuff
 */
struct qeth_perf_stats {
	unsigned int bufs_rec;
	unsigned int bufs_sent;
	unsigned int buf_elements_sent;

	unsigned int skbs_sent_pack;
	unsigned int bufs_sent_pack;

	unsigned int sc_dp_p;
	unsigned int sc_p_dp;
	/* qdio_cq_handler: number of times called, time spent in */
	__u64 cq_start_time;
	unsigned int cq_cnt;
	unsigned int cq_time;
	/* qdio_input_handler: number of times called, time spent in */
	__u64 inbound_start_time;
	unsigned int inbound_cnt;
	unsigned int inbound_time;
	/* qeth_send_packet: number of times called, time spent in */
	__u64 outbound_start_time;
	unsigned int outbound_cnt;
	unsigned int outbound_time;
	/* qdio_output_handler: number of times called, time spent in */
	__u64 outbound_handler_start_time;
	unsigned int outbound_handler_cnt;
	unsigned int outbound_handler_time;
	/* number of calls to and time spent in do_QDIO for inbound queue */
	__u64 inbound_do_qdio_start_time;
	unsigned int inbound_do_qdio_cnt;
	unsigned int inbound_do_qdio_time;
	/* number of calls to and time spent in do_QDIO for outbound queues */
	__u64 outbound_do_qdio_start_time;
	unsigned int outbound_do_qdio_cnt;
	unsigned int outbound_do_qdio_time;
	unsigned int large_send_bytes;
	unsigned int large_send_cnt;
	unsigned int sg_skbs_sent;
	/* initial values when measuring starts */
	unsigned long initial_rx_packets;
	unsigned long initial_tx_packets;
	/* inbound scatter gather data */
	unsigned int sg_skbs_rx;
	unsigned int sg_frags_rx;
	unsigned int sg_alloc_page_rx;
	unsigned int tx_csum;
	unsigned int tx_lin;
	unsigned int tx_linfail;
	unsigned int rx_csum;
};
=======
#define CCW_DEVID(cdev)		(qeth_get_device_id(cdev))
#define CARD_DEVID(card)	(CCW_DEVID(CARD_RDEV(card)))
>>>>>>> upstream/android-13

/* Routing stuff */
struct qeth_routing_info {
	enum qeth_routing_types type;
};

<<<<<<< HEAD
/* IPA stuff */
struct qeth_ipa_info {
	__u32 supported_funcs;
	__u32 enabled_funcs;
};

=======
>>>>>>> upstream/android-13
/* SETBRIDGEPORT stuff */
enum qeth_sbp_roles {
	QETH_SBP_ROLE_NONE	= 0,
	QETH_SBP_ROLE_PRIMARY	= 1,
	QETH_SBP_ROLE_SECONDARY	= 2,
};

enum qeth_sbp_states {
	QETH_SBP_STATE_INACTIVE	= 0,
	QETH_SBP_STATE_STANDBY	= 1,
	QETH_SBP_STATE_ACTIVE	= 2,
};

#define QETH_SBP_HOST_NOTIFICATION 1

struct qeth_sbp_info {
	__u32 supported_funcs;
	enum qeth_sbp_roles role;
	__u32 hostnotification:1;
	__u32 reflect_promisc:1;
	__u32 reflect_promisc_primary:1;
};

struct qeth_vnicc_info {
	/* supported/currently configured VNICCs; updated in IPA exchanges */
	u32 sup_chars;
	u32 cur_chars;
	/* supported commands: bitmasks which VNICCs support respective cmd */
	u32 set_char_sup;
	u32 getset_timeout_sup;
	/* timeout value for the learning characteristic */
	u32 learning_timeout;
	/* characteristics wanted/configured by user */
	u32 wanted_chars;
	/* has user explicitly enabled rx_bcast while online? */
	bool rx_bcast_enabled;
};

<<<<<<< HEAD
static inline int qeth_is_adp_supported(struct qeth_ipa_info *ipa,
		enum qeth_ipa_setadp_cmd func)
{
	return (ipa->supported_funcs & func);
}

static inline int qeth_is_ipa_supported(struct qeth_ipa_info *ipa,
		enum qeth_ipa_funcs func)
{
	return (ipa->supported_funcs & func);
}

static inline int qeth_is_ipa_enabled(struct qeth_ipa_info *ipa,
		enum qeth_ipa_funcs func)
{
	return (ipa->supported_funcs & ipa->enabled_funcs & func);
}

#define qeth_adp_supported(c, f) \
	qeth_is_adp_supported(&c->options.adp, f)
#define qeth_is_supported(c, f) \
	qeth_is_ipa_supported(&c->options.ipa4, f)
#define qeth_is_enabled(c, f) \
	qeth_is_ipa_enabled(&c->options.ipa4, f)
#define qeth_is_supported6(c, f) \
	qeth_is_ipa_supported(&c->options.ipa6, f)
#define qeth_is_enabled6(c, f) \
	qeth_is_ipa_enabled(&c->options.ipa6, f)
#define qeth_is_ipafunc_supported(c, prot, f) \
	 ((prot == QETH_PROT_IPV6) ? \
		qeth_is_supported6(c, f) : qeth_is_supported(c, f))
#define qeth_is_ipafunc_enabled(c, prot, f) \
	 ((prot == QETH_PROT_IPV6) ? \
		qeth_is_enabled6(c, f) : qeth_is_enabled(c, f))

=======
>>>>>>> upstream/android-13
#define QETH_IDX_FUNC_LEVEL_OSD		 0x0101
#define QETH_IDX_FUNC_LEVEL_IQD		 0x4108

#define QETH_BUFSIZE		4096
#define CCW_CMD_WRITE		0x01
#define CCW_CMD_READ		0x02

/**
 * some more defs
 */
<<<<<<< HEAD
#define QETH_TX_TIMEOUT		100 * HZ
#define QETH_RCD_TIMEOUT	60 * HZ
#define QETH_RECLAIM_WORK_TIME	HZ
#define QETH_MAX_PORTNO		15

/*IPv6 address autoconfiguration stuff*/
#define UNIQUE_ID_IF_CREATE_ADDR_FAILED 0xfffe
#define UNIQUE_ID_NOT_BY_CARD		0x10000

/*****************************************************************************/
/* QDIO queue and buffer handling                                            */
/*****************************************************************************/
#define QETH_MAX_QUEUES 4
#define QETH_IN_BUF_SIZE_DEFAULT 65536
#define QETH_IN_BUF_COUNT_DEFAULT 64
#define QETH_IN_BUF_COUNT_HSDEFAULT 128
#define QETH_IN_BUF_COUNT_MIN 8
#define QETH_IN_BUF_COUNT_MAX 128
=======
#define QETH_TX_TIMEOUT		(100 * HZ)
#define QETH_RCD_TIMEOUT	(60 * HZ)
#define QETH_RECLAIM_WORK_TIME	HZ
#define QETH_MAX_PORTNO		15

/*****************************************************************************/
/* QDIO queue and buffer handling                                            */
/*****************************************************************************/
#define QETH_MAX_OUT_QUEUES	4
#define QETH_IQD_MIN_TXQ	2	/* One for ucast, one for mcast. */
#define QETH_IQD_MCAST_TXQ	0
#define QETH_IQD_MIN_UCAST_TXQ	1

#define QETH_MAX_IN_QUEUES	2
#define QETH_RX_COPYBREAK      (PAGE_SIZE >> 1)
#define QETH_IN_BUF_SIZE_DEFAULT 65536
#define QETH_IN_BUF_COUNT_DEFAULT 64
#define QETH_IN_BUF_COUNT_HSDEFAULT 128
#define QETH_IN_BUF_COUNT_MIN	8U
#define QETH_IN_BUF_COUNT_MAX	128U
>>>>>>> upstream/android-13
#define QETH_MAX_BUFFER_ELEMENTS(card) ((card)->qdio.in_buf_size >> 12)
#define QETH_IN_BUF_REQUEUE_THRESHOLD(card) \
		 ((card)->qdio.in_buf_pool.buf_count / 2)

/* buffers we have to be behind before we get a PCI */
#define QETH_PCI_THRESHOLD_A(card) ((card)->qdio.in_buf_pool.buf_count+1)
/*enqueued free buffers left before we get a PCI*/
#define QETH_PCI_THRESHOLD_B(card) 0
/*not used unless the microcode gets patched*/
#define QETH_PCI_TIMER_VALUE(card) 3

/* priority queing */
#define QETH_PRIOQ_DEFAULT QETH_NO_PRIO_QUEUEING
#define QETH_DEFAULT_QUEUE    2
#define QETH_NO_PRIO_QUEUEING 0
#define QETH_PRIO_Q_ING_PREC  1
#define QETH_PRIO_Q_ING_TOS   2
#define QETH_PRIO_Q_ING_SKB   3
#define QETH_PRIO_Q_ING_VLAN  4
<<<<<<< HEAD
=======
#define QETH_PRIO_Q_ING_FIXED 5
>>>>>>> upstream/android-13

/* Packing */
#define QETH_LOW_WATERMARK_PACK  2
#define QETH_HIGH_WATERMARK_PACK 5
#define QETH_WATERMARK_PACK_FUZZ 1

<<<<<<< HEAD
/* large receive scatter gather copy break */
#define QETH_RX_SG_CB (PAGE_SIZE >> 1)
#define QETH_RX_PULL_LEN 256

=======
>>>>>>> upstream/android-13
struct qeth_hdr_layer3 {
	__u8  id;
	__u8  flags;
	__u16 inbound_checksum; /*TSO:__u16 seqno */
	__u32 token;		/*TSO: __u32 reserved */
	__u16 length;
	__u8  vlan_prio;
	__u8  ext_flags;
	__u16 vlan_id;
	__u16 frame_offset;
	union {
		/* TX: */
<<<<<<< HEAD
		u8 ipv6_addr[16];
		struct ipv4 {
			u8 res[12];
			u32 addr;
		} ipv4;
=======
		struct in6_addr addr;
>>>>>>> upstream/android-13
		/* RX: */
		struct rx {
			u8 res1[2];
			u8 src_mac[6];
			u8 res2[4];
			u16 vlan_id;
			u8 res3[2];
		} rx;
	} next_hop;
};

struct qeth_hdr_layer2 {
	__u8 id;
	__u8 flags[3];
	__u8 port_no;
	__u8 hdr_length;
	__u16 pkt_length;
	__u16 seq_no;
	__u16 vlan_id;
	__u32 reserved;
	__u8 reserved2[16];
} __attribute__ ((packed));

<<<<<<< HEAD
struct qeth_hdr_osn {
	__u8 id;
	__u8 reserved;
	__u16 seq_no;
	__u16 reserved2;
	__u16 control_flags;
	__u16 pdu_length;
	__u8 reserved3[18];
	__u32 ccid;
} __attribute__ ((packed));

=======
>>>>>>> upstream/android-13
struct qeth_hdr {
	union {
		struct qeth_hdr_layer2 l2;
		struct qeth_hdr_layer3 l3;
<<<<<<< HEAD
		struct qeth_hdr_osn    osn;
	} hdr;
} __attribute__ ((packed));

=======
	} hdr;
} __attribute__ ((packed));

#define QETH_QIB_PQUE_ORDER_RR		0
#define QETH_QIB_PQUE_UNITS_SBAL	2
#define QETH_QIB_PQUE_PRIO_DEFAULT	4

struct qeth_qib_parms {
	char pcit_magic[4];
	u32 pcit_a;
	u32 pcit_b;
	u32 pcit_c;
	char blkt_magic[4];
	u32 blkt_total;
	u32 blkt_inter_packet;
	u32 blkt_inter_packet_jumbo;
	char pque_magic[4];
	u8 pque_order;
	u8 pque_units;
	u16 reserved;
	u32 pque_priority[4];
};

>>>>>>> upstream/android-13
/*TCP Segmentation Offload header*/
struct qeth_hdr_ext_tso {
	__u16 hdr_tot_len;
	__u8  imb_hdr_no;
	__u8  reserved;
	__u8  hdr_type;
	__u8  hdr_version;
	__u16 hdr_len;
	__u32 payload_len;
	__u16 mss;
	__u16 dg_hdr_len;
	__u8  padding[16];
} __attribute__ ((packed));

struct qeth_hdr_tso {
	struct qeth_hdr hdr;	/*hdr->hdr.l3.xxx*/
	struct qeth_hdr_ext_tso ext;
} __attribute__ ((packed));


/* flags for qeth_hdr.flags */
#define QETH_HDR_PASSTHRU 0x10
#define QETH_HDR_IPV6     0x80
#define QETH_HDR_CAST_MASK 0x07
enum qeth_cast_flags {
	QETH_CAST_UNICAST   = 0x06,
	QETH_CAST_MULTICAST = 0x04,
	QETH_CAST_BROADCAST = 0x05,
	QETH_CAST_ANYCAST   = 0x07,
	QETH_CAST_NOCAST    = 0x00,
};

enum qeth_layer2_frame_flags {
	QETH_LAYER2_FLAG_MULTICAST = 0x01,
	QETH_LAYER2_FLAG_BROADCAST = 0x02,
	QETH_LAYER2_FLAG_UNICAST   = 0x04,
	QETH_LAYER2_FLAG_VLAN      = 0x10,
};

enum qeth_header_ids {
	QETH_HEADER_TYPE_LAYER3 = 0x01,
	QETH_HEADER_TYPE_LAYER2 = 0x02,
<<<<<<< HEAD
	QETH_HEADER_TYPE_TSO	= 0x03,
	QETH_HEADER_TYPE_OSN    = 0x04,
=======
	QETH_HEADER_TYPE_L3_TSO	= 0x03,
	QETH_HEADER_TYPE_L2_TSO	= 0x06,
	QETH_HEADER_MASK_INVAL	= 0x80,
>>>>>>> upstream/android-13
};
/* flags for qeth_hdr.ext_flags */
#define QETH_HDR_EXT_VLAN_FRAME       0x01
#define QETH_HDR_EXT_TOKEN_ID         0x02
#define QETH_HDR_EXT_INCLUDE_VLAN_TAG 0x04
#define QETH_HDR_EXT_SRC_MAC_ADDR     0x08
#define QETH_HDR_EXT_CSUM_HDR_REQ     0x10
#define QETH_HDR_EXT_CSUM_TRANSP_REQ  0x20
#define QETH_HDR_EXT_UDP	      0x40 /*bit off for TCP*/

<<<<<<< HEAD
enum qeth_qdio_buffer_states {
	/*
	 * inbound: read out by driver; owned by hardware in order to be filled
	 * outbound: owned by driver in order to be filled
	 */
	QETH_QDIO_BUF_EMPTY,
	/*
	 * inbound: filled by hardware; owned by driver in order to be read out
	 * outbound: filled by driver; owned by hardware in order to be sent
	 */
	QETH_QDIO_BUF_PRIMED,
	/*
	 * inbound: not applicable
	 * outbound: identified to be pending in TPQ
	 */
	QETH_QDIO_BUF_PENDING,
	/*
	 * inbound: not applicable
	 * outbound: found in completion queue
	 */
	QETH_QDIO_BUF_IN_CQ,
	/*
	 * inbound: not applicable
	 * outbound: handled via transfer pending / completion queue
	 */
	QETH_QDIO_BUF_HANDLED_DELAYED,
=======
static inline bool qeth_l2_same_vlan(struct qeth_hdr_layer2 *h1,
				     struct qeth_hdr_layer2 *h2)
{
	return !((h1->flags[2] ^ h2->flags[2]) & QETH_LAYER2_FLAG_VLAN) &&
	       h1->vlan_id == h2->vlan_id;
}

static inline bool qeth_l3_iqd_same_vlan(struct qeth_hdr_layer3 *h1,
					 struct qeth_hdr_layer3 *h2)
{
	return !((h1->ext_flags ^ h2->ext_flags) & QETH_HDR_EXT_VLAN_FRAME) &&
	       h1->vlan_id == h2->vlan_id;
}

static inline bool qeth_l3_same_next_hop(struct qeth_hdr_layer3 *h1,
					 struct qeth_hdr_layer3 *h2)
{
	return !((h1->flags ^ h2->flags) & QETH_HDR_IPV6) &&
	       ipv6_addr_equal(&h1->next_hop.addr, &h2->next_hop.addr);
}

struct qeth_local_addr {
	struct hlist_node hnode;
	struct rcu_head rcu;
	struct in6_addr addr;
>>>>>>> upstream/android-13
};

enum qeth_qdio_info_states {
	QETH_QDIO_UNINITIALIZED,
	QETH_QDIO_ALLOCATED,
	QETH_QDIO_ESTABLISHED,
	QETH_QDIO_CLEANING
};

struct qeth_buffer_pool_entry {
	struct list_head list;
	struct list_head init_list;
<<<<<<< HEAD
	void *elements[QDIO_MAX_ELEMENTS_PER_BUFFER];
=======
	struct page *elements[QDIO_MAX_ELEMENTS_PER_BUFFER];
>>>>>>> upstream/android-13
};

struct qeth_qdio_buffer_pool {
	struct list_head entry_list;
	int buf_count;
};

struct qeth_qdio_buffer {
	struct qdio_buffer *buffer;
	/* the buffer pool entry currently associated to this buffer */
	struct qeth_buffer_pool_entry *pool_entry;
	struct sk_buff *rx_skb;
};

struct qeth_qdio_q {
	struct qdio_buffer *qdio_bufs[QDIO_MAX_BUFFERS_PER_Q];
	struct qeth_qdio_buffer bufs[QDIO_MAX_BUFFERS_PER_Q];
	int next_buf_to_init;
};

<<<<<<< HEAD
=======
enum qeth_qdio_out_buffer_state {
	/* Owned by driver, in order to be filled. */
	QETH_QDIO_BUF_EMPTY,
	/* Filled by driver; owned by hardware in order to be sent. */
	QETH_QDIO_BUF_PRIMED,
};

enum qeth_qaob_state {
	QETH_QAOB_ISSUED,
	QETH_QAOB_PENDING,
	QETH_QAOB_DONE,
};

struct qeth_qaob_priv1 {
	unsigned int state;
	u8 queue_no;
};

>>>>>>> upstream/android-13
struct qeth_qdio_out_buffer {
	struct qdio_buffer *buffer;
	atomic_t state;
	int next_element_to_fill;
<<<<<<< HEAD
	struct sk_buff_head skb_list;
	int is_header[QDIO_MAX_ELEMENTS_PER_BUFFER];

	struct qeth_qdio_out_q *q;
	struct qeth_qdio_out_buffer *next_pending;
=======
	unsigned int frames;
	unsigned int bytes;
	struct sk_buff_head skb_list;
	DECLARE_BITMAP(from_kmem_cache, QDIO_MAX_ELEMENTS_PER_BUFFER);

	struct list_head list_entry;
	struct qaob *aob;
>>>>>>> upstream/android-13
};

struct qeth_card;

<<<<<<< HEAD
enum qeth_out_q_states {
       QETH_OUT_Q_UNLOCKED,
       QETH_OUT_Q_LOCKED,
       QETH_OUT_Q_LOCKED_FLUSH,
};

struct qeth_qdio_out_q {
	struct qdio_buffer *qdio_bufs[QDIO_MAX_BUFFERS_PER_Q];
	struct qeth_qdio_out_buffer *bufs[QDIO_MAX_BUFFERS_PER_Q];
	struct qdio_outbuf_state *bufstates; /* convenience pointer */
	int queue_no;
	struct qeth_card *card;
	atomic_t state;
	int do_pack;
	/*
	 * index of buffer to be filled by driver; state EMPTY or PACKING
	 */
	int next_buf_to_fill;
=======
#define QETH_CARD_STAT_ADD(_c, _stat, _val)	((_c)->stats._stat += (_val))
#define QETH_CARD_STAT_INC(_c, _stat)		QETH_CARD_STAT_ADD(_c, _stat, 1)

#define QETH_TXQ_STAT_ADD(_q, _stat, _val)	((_q)->stats._stat += (_val))
#define QETH_TXQ_STAT_INC(_q, _stat)		QETH_TXQ_STAT_ADD(_q, _stat, 1)

struct qeth_card_stats {
	u64 rx_bufs;
	u64 rx_skb_csum;
	u64 rx_sg_skbs;
	u64 rx_sg_frags;
	u64 rx_sg_alloc_page;

	u64 rx_dropped_nomem;
	u64 rx_dropped_notsupp;
	u64 rx_dropped_runt;

	/* rtnl_link_stats64 */
	u64 rx_packets;
	u64 rx_bytes;
	u64 rx_multicast;
	u64 rx_length_errors;
	u64 rx_frame_errors;
	u64 rx_fifo_errors;
};

struct qeth_out_q_stats {
	u64 bufs;
	u64 bufs_pack;
	u64 buf_elements;
	u64 skbs_pack;
	u64 skbs_sg;
	u64 skbs_csum;
	u64 skbs_tso;
	u64 skbs_linearized;
	u64 skbs_linearized_fail;
	u64 tso_bytes;
	u64 packing_mode_switch;
	u64 stopped;
	u64 doorbell;
	u64 coal_frames;
	u64 completion_irq;
	u64 completion_yield;
	u64 completion_timer;

	/* rtnl_link_stats64 */
	u64 tx_packets;
	u64 tx_bytes;
	u64 tx_errors;
	u64 tx_dropped;
};

#define QETH_TX_MAX_COALESCED_FRAMES	1
#define QETH_TX_COALESCE_USECS		25
#define QETH_TX_TIMER_USECS		500

struct qeth_qdio_out_q {
	struct qdio_buffer *qdio_bufs[QDIO_MAX_BUFFERS_PER_Q];
	struct qeth_qdio_out_buffer *bufs[QDIO_MAX_BUFFERS_PER_Q];
	struct list_head pending_bufs;
	struct qeth_out_q_stats stats;
	spinlock_t lock;
	unsigned int priority;
	u8 next_buf_to_fill;
	u8 max_elements;
	u8 queue_no;
	u8 do_pack;
	struct qeth_card *card;
>>>>>>> upstream/android-13
	/*
	 * number of buffers that are currently filled (PRIMED)
	 * -> these buffers are hardware-owned
	 */
	atomic_t used_buffers;
	/* indicates whether PCI flag must be set (or if one is outstanding) */
	atomic_t set_pci_flags_count;
<<<<<<< HEAD
};

=======
	struct napi_struct napi;
	struct timer_list timer;
	struct qeth_hdr *prev_hdr;
	unsigned int coalesced_frames;
	u8 bulk_start;
	u8 bulk_count;
	u8 bulk_max;

	unsigned int coalesce_usecs;
	unsigned int max_coalesced_frames;
	unsigned int rescan_usecs;
};

#define qeth_for_each_output_queue(card, q, i)		\
	for (i = 0; i < card->qdio.no_out_queues &&	\
		    (q = card->qdio.out_qs[i]); i++)

#define	qeth_napi_to_out_queue(n) container_of(n, struct qeth_qdio_out_q, napi)

static inline void qeth_tx_arm_timer(struct qeth_qdio_out_q *queue,
				     unsigned long usecs)
{
	timer_reduce(&queue->timer, usecs_to_jiffies(usecs) + jiffies);
}

static inline bool qeth_out_queue_is_full(struct qeth_qdio_out_q *queue)
{
	return atomic_read(&queue->used_buffers) >= QDIO_MAX_BUFFERS_PER_Q;
}

static inline bool qeth_out_queue_is_empty(struct qeth_qdio_out_q *queue)
{
	return atomic_read(&queue->used_buffers) == 0;
}

>>>>>>> upstream/android-13
struct qeth_qdio_info {
	atomic_t state;
	/* input */
	int no_in_queues;
	struct qeth_qdio_q *in_q;
	struct qeth_qdio_q *c_q;
	struct qeth_qdio_buffer_pool in_buf_pool;
	struct qeth_qdio_buffer_pool init_pool;
	int in_buf_size;

	/* output */
<<<<<<< HEAD
	int no_out_queues;
	struct qeth_qdio_out_q **out_qs;
	struct qdio_outbuf_state *out_bufstates;
=======
	unsigned int no_out_queues;
	struct qeth_qdio_out_q *out_qs[QETH_MAX_OUT_QUEUES];
>>>>>>> upstream/android-13

	/* priority queueing */
	int do_prio_queueing;
	int default_out_queue;
};

/**
<<<<<<< HEAD
 * buffer stuff for read channel
 */
#define QETH_CMD_BUFFER_NO	8

/**
=======
>>>>>>> upstream/android-13
 *  channel state machine
 */
enum qeth_channel_states {
	CH_STATE_UP,
	CH_STATE_DOWN,
<<<<<<< HEAD
	CH_STATE_ACTIVATING,
	CH_STATE_HALTED,
	CH_STATE_STOPPED,
	CH_STATE_RCD,
	CH_STATE_RCD_DONE,
=======
	CH_STATE_HALTED,
	CH_STATE_STOPPED,
>>>>>>> upstream/android-13
};
/**
 * card state machine
 */
enum qeth_card_states {
	CARD_STATE_DOWN,
<<<<<<< HEAD
	CARD_STATE_HARDSETUP,
	CARD_STATE_SOFTSETUP,
	CARD_STATE_UP,
	CARD_STATE_RECOVER,
=======
	CARD_STATE_SOFTSETUP,
>>>>>>> upstream/android-13
};

/**
 * Protocol versions
 */
enum qeth_prot_versions {
<<<<<<< HEAD
=======
	QETH_PROT_NONE = 0x0000,
>>>>>>> upstream/android-13
	QETH_PROT_IPV4 = 0x0004,
	QETH_PROT_IPV6 = 0x0006,
};

<<<<<<< HEAD
enum qeth_cmd_buffer_state {
	BUF_STATE_FREE,
	BUF_STATE_LOCKED,
};

=======
>>>>>>> upstream/android-13
enum qeth_cq {
	QETH_CQ_DISABLED = 0,
	QETH_CQ_ENABLED = 1,
	QETH_CQ_NOTAVAILABLE = 2,
};

struct qeth_ipato {
	bool enabled;
	bool invert4;
	bool invert6;
	struct list_head entries;
};

<<<<<<< HEAD
struct qeth_channel;

struct qeth_cmd_buffer {
	enum qeth_cmd_buffer_state state;
	struct qeth_channel *channel;
	unsigned char *data;
	int rc;
	void (*callback) (struct qeth_channel *, struct qeth_cmd_buffer *);
};

=======
struct qeth_channel {
	struct ccw_device *ccwdev;
	struct qeth_cmd_buffer *active_cmd;
	enum qeth_channel_states state;
};

struct qeth_reply {
	int (*callback)(struct qeth_card *card, struct qeth_reply *reply,
			unsigned long data);
	void *param;
};

struct qeth_cmd_buffer {
	struct list_head list_entry;
	struct completion done;
	spinlock_t lock;
	unsigned int length;
	refcount_t ref_count;
	struct qeth_channel *channel;
	struct qeth_reply reply;
	long timeout;
	unsigned char *data;
	void (*finalize)(struct qeth_card *card, struct qeth_cmd_buffer *iob);
	bool (*match)(struct qeth_cmd_buffer *iob,
		      struct qeth_cmd_buffer *reply);
	void (*callback)(struct qeth_card *card, struct qeth_cmd_buffer *iob,
			 unsigned int data_length);
	int rc;
};

static inline void qeth_get_cmd(struct qeth_cmd_buffer *iob)
{
	refcount_inc(&iob->ref_count);
}

static inline struct qeth_ipa_cmd *__ipa_reply(struct qeth_cmd_buffer *iob)
{
	if (!IS_IPA(iob->data))
		return NULL;

	return (struct qeth_ipa_cmd *) PDU_ENCAPSULATION(iob->data);
}

>>>>>>> upstream/android-13
static inline struct qeth_ipa_cmd *__ipa_cmd(struct qeth_cmd_buffer *iob)
{
	return (struct qeth_ipa_cmd *)(iob->data + IPA_PDU_HEADER_SIZE);
}

<<<<<<< HEAD
/**
 * definition of a qeth channel, used for read and write
 */
struct qeth_channel {
	enum qeth_channel_states state;
	struct ccw1 *ccw;
	spinlock_t iob_lock;
	wait_queue_head_t wait_q;
	struct ccw_device *ccwdev;
/*command buffer for control data*/
	struct qeth_cmd_buffer iob[QETH_CMD_BUFFER_NO];
	atomic_t irq_pending;
	int io_buf_no;
};
=======
static inline struct ccw1 *__ccw_from_cmd(struct qeth_cmd_buffer *iob)
{
	return (struct ccw1 *)(iob->data + ALIGN(iob->length, 8));
}
>>>>>>> upstream/android-13

/**
 *  OSA card related definitions
 */
struct qeth_token {
	__u32 issuer_rm_w;
	__u32 issuer_rm_r;
	__u32 cm_filter_w;
	__u32 cm_filter_r;
	__u32 cm_connection_w;
	__u32 cm_connection_r;
	__u32 ulp_filter_w;
	__u32 ulp_filter_r;
	__u32 ulp_connection_w;
	__u32 ulp_connection_r;
};

struct qeth_seqno {
	__u32 trans_hdr;
	__u32 pdu_hdr;
	__u32 pdu_hdr_ack;
	__u16 ipa;
<<<<<<< HEAD
	__u32 pkt_seqno;
};

struct qeth_reply {
	struct list_head list;
	wait_queue_head_t wait_q;
	int (*callback)(struct qeth_card *, struct qeth_reply *,
		unsigned long);
	u32 seqno;
	unsigned long offset;
	atomic_t received;
	int rc;
	void *param;
	struct qeth_card *card;
	refcount_t refcnt;
=======
>>>>>>> upstream/android-13
};

struct qeth_card_blkt {
	int time_total;
	int inter_packet;
	int inter_packet_jumbo;
};

<<<<<<< HEAD
#define QETH_BROADCAST_WITH_ECHO    0x01
#define QETH_BROADCAST_WITHOUT_ECHO 0x02
#define QETH_LAYER2_MAC_READ	    0x01
#define QETH_LAYER2_MAC_REGISTERED  0x02
struct qeth_card_info {
	unsigned short unit_addr2;
	unsigned short cula;
	unsigned short chpid;
	__u16 func_level;
	char mcl_level[QETH_MCL_LENGTH + 1];
	int guestlan;
	int mac_bits;
	enum qeth_card_types type;
	enum qeth_link_types link_type;
	int broadcast_capable;
	int unique_id;
	bool layer_enforced;
	struct qeth_card_blkt blkt;
	enum qeth_ipa_promisc_modes promisc_mode;
	__u32 diagass_support;
	__u32 hwtrap;
};

struct qeth_card_options {
	struct qeth_routing_info route4;
	struct qeth_ipa_info ipa4;
	struct qeth_ipa_info adp; /*Adapter parameters*/
	struct qeth_routing_info route6;
	struct qeth_ipa_info ipa6;
	struct qeth_sbp_info sbp; /* SETBRIDGEPORT options */
	struct qeth_vnicc_info vnicc; /* VNICC options */
	int fake_broadcast;
	int layer2;
	int performance_stats;
	int rx_sg_cb;
	enum qeth_ipa_isolation_modes isolation;
	enum qeth_ipa_isolation_modes prev_isolation;
	int sniffer;
	enum qeth_cq cq;
	char hsuid[9];
};

/*
 * thread bits for qeth_card thread masks
 */
enum qeth_threads {
	QETH_RECOVER_THREAD = 1,
};

struct qeth_osn_info {
	int (*assist_cb)(struct net_device *dev, void *data);
	int (*data_cb)(struct sk_buff *skb);
=======
enum qeth_pnso_mode {
	QETH_PNSO_NONE,
	QETH_PNSO_BRIDGEPORT,
	QETH_PNSO_ADDR_INFO,
};

enum qeth_link_mode {
	QETH_LINK_MODE_UNKNOWN,
	QETH_LINK_MODE_FIBRE_SHORT,
	QETH_LINK_MODE_FIBRE_LONG,
};

struct qeth_link_info {
	u32 speed;
	u8 duplex;
	u8 port;
	enum qeth_link_mode link_mode;
};

#define QETH_BROADCAST_WITH_ECHO    0x01
#define QETH_BROADCAST_WITHOUT_ECHO 0x02
struct qeth_card_info {
	unsigned short unit_addr2;
	unsigned short cula;
	__u16 func_level;
	char mcl_level[QETH_MCL_LENGTH + 1];
	/* doubleword below corresponds to net_if_token */
	u16 ddev_devno;
	u8 cssid;
	u8 iid;
	u8 ssid;
	u8 chpid;
	u16 chid;
	u8 ids_valid:1; /* cssid,iid,chid */
	u8 dev_addr_is_registered:1;
	u8 open_when_online:1;
	u8 promisc_mode:1;
	u8 use_v1_blkt:1;
	u8 is_vm_nic:1;
	/* no bitfield, we take a pointer on these two: */
	u8 has_lp2lp_cso_v6;
	u8 has_lp2lp_cso_v4;
	enum qeth_pnso_mode pnso_mode;
	enum qeth_card_types type;
	enum qeth_link_types link_type;
	int broadcast_capable;
	bool layer_enforced;
	struct qeth_card_blkt blkt;
	__u32 diagass_support;
	__u32 hwtrap;
	struct qeth_link_info link_info;
>>>>>>> upstream/android-13
};

enum qeth_discipline_id {
	QETH_DISCIPLINE_UNDETERMINED = -1,
	QETH_DISCIPLINE_LAYER3 = 0,
	QETH_DISCIPLINE_LAYER2 = 1,
};

<<<<<<< HEAD
struct qeth_discipline {
	const struct device_type *devtype;
	int (*process_rx_buffer)(struct qeth_card *card, int budget, int *done);
	int (*recover)(void *ptr);
	int (*setup) (struct ccwgroup_device *);
	void (*remove) (struct ccwgroup_device *);
	int (*set_online) (struct ccwgroup_device *);
	int (*set_offline) (struct ccwgroup_device *);
	int (*freeze)(struct ccwgroup_device *);
	int (*thaw) (struct ccwgroup_device *);
	int (*restore)(struct ccwgroup_device *);
	int (*do_ioctl)(struct net_device *dev, struct ifreq *rq, int cmd);
	int (*control_event_handler)(struct qeth_card *card,
					struct qeth_ipa_cmd *cmd);
};

struct qeth_vlan_vid {
	struct list_head list;
	unsigned short vid;
=======
struct qeth_card_options {
	struct qeth_ipa_caps ipa4;
	struct qeth_ipa_caps ipa6;
	struct qeth_routing_info route4;
	struct qeth_routing_info route6;
	struct qeth_ipa_caps adp; /* Adapter parameters */
	struct qeth_sbp_info sbp; /* SETBRIDGEPORT options */
	struct qeth_vnicc_info vnicc; /* VNICC options */
	enum qeth_discipline_id layer;
	enum qeth_ipa_isolation_modes isolation;
	int sniffer;
	enum qeth_cq cq;
	char hsuid[9];
};

#define	IS_LAYER2(card)	((card)->options.layer == QETH_DISCIPLINE_LAYER2)
#define	IS_LAYER3(card)	((card)->options.layer == QETH_DISCIPLINE_LAYER3)

/*
 * thread bits for qeth_card thread masks
 */
enum qeth_threads {
	QETH_RECOVER_THREAD = 1,
};

struct qeth_discipline {
	int (*setup) (struct ccwgroup_device *);
	void (*remove) (struct ccwgroup_device *);
	int (*set_online)(struct qeth_card *card, bool carrier_ok);
	void (*set_offline)(struct qeth_card *card);
	int (*do_ioctl)(struct net_device *dev, struct ifreq *rq,
			void __user *data, int cmd);
	int (*control_event_handler)(struct qeth_card *card,
					struct qeth_ipa_cmd *cmd);
>>>>>>> upstream/android-13
};

enum qeth_addr_disposition {
	QETH_DISP_ADDR_DELETE = 0,
	QETH_DISP_ADDR_DO_NOTHING = 1,
	QETH_DISP_ADDR_ADD = 2,
};

struct qeth_rx {
	int b_count;
	int b_index;
<<<<<<< HEAD
	struct qdio_buffer_element *b_element;
	int e_offset;
	int qdio_err;
};

struct carrier_info {
	__u8  card_type;
	__u16 port_mode;
	__u32 port_speed;
=======
	u8 buf_element;
	int e_offset;
	int qdio_err;
	u8 bufs_refill;
>>>>>>> upstream/android-13
};

struct qeth_switch_info {
	__u32 capabilities;
	__u32 settings;
};

<<<<<<< HEAD
#define QETH_NAPI_WEIGHT NAPI_POLL_WEIGHT

struct qeth_card {
	struct list_head list;
	enum qeth_card_states state;
	int lan_online;
	spinlock_t lock;
	struct ccwgroup_device *gdev;
=======
struct qeth_priv {
	unsigned int rx_copybreak;
	unsigned int tx_wanted_queues;
	u32 brport_hw_features;
	u32 brport_features;
};

#define QETH_NAPI_WEIGHT NAPI_POLL_WEIGHT

struct qeth_card {
	enum qeth_card_states state;
	spinlock_t lock;
	struct ccwgroup_device *gdev;
	struct qeth_cmd_buffer *read_cmd;
>>>>>>> upstream/android-13
	struct qeth_channel read;
	struct qeth_channel write;
	struct qeth_channel data;

	struct net_device *dev;
<<<<<<< HEAD
	struct net_device_stats stats;

=======
	struct dentry *debugfs;
	struct qeth_card_stats stats;
>>>>>>> upstream/android-13
	struct qeth_card_info info;
	struct qeth_token token;
	struct qeth_seqno seqno;
	struct qeth_card_options options;

<<<<<<< HEAD
	wait_queue_head_t wait_q;
	spinlock_t mclock;
	unsigned long active_vlans[BITS_TO_LONGS(VLAN_N_VID)];
	struct mutex vid_list_mutex;		/* vid_list */
	struct list_head vid_list;
	DECLARE_HASHTABLE(mac_htable, 4);
	DECLARE_HASHTABLE(ip_htable, 4);
	DECLARE_HASHTABLE(ip_mc_htable, 4);
=======
	struct workqueue_struct *event_wq;
	struct workqueue_struct *cmd_wq;
	wait_queue_head_t wait_q;

	struct mutex ip_lock;
	/* protected by ip_lock: */
	DECLARE_HASHTABLE(ip_htable, 4);
	struct qeth_ipato ipato;

	DECLARE_HASHTABLE(local_addrs4, 4);
	DECLARE_HASHTABLE(local_addrs6, 4);
	spinlock_t local_addrs4_lock;
	spinlock_t local_addrs6_lock;
	DECLARE_HASHTABLE(rx_mode_addrs, 4);
	struct work_struct rx_mode_work;
>>>>>>> upstream/android-13
	struct work_struct kernel_thread_starter;
	spinlock_t thread_mask_lock;
	unsigned long thread_start_mask;
	unsigned long thread_allowed_mask;
	unsigned long thread_running_mask;
<<<<<<< HEAD
	struct task_struct *recovery_task;
	spinlock_t ip_lock;
	struct qeth_ipato ipato;
	struct list_head cmd_waiter_list;
	/* QDIO buffer handling */
	struct qeth_qdio_info qdio;
	struct qeth_perf_stats perf_stats;
	int read_or_write_problem;
	struct qeth_osn_info osn_info;
	struct qeth_discipline *discipline;
=======
	struct list_head cmd_waiter_list;
	/* QDIO buffer handling */
	struct qeth_qdio_info qdio;
	int read_or_write_problem;
	const struct qeth_discipline *discipline;
>>>>>>> upstream/android-13
	atomic_t force_alloc_skb;
	struct service_level qeth_service_level;
	struct qdio_ssqd_desc ssqd;
	debug_info_t *debug;
<<<<<<< HEAD
=======
	struct mutex sbp_lock;
>>>>>>> upstream/android-13
	struct mutex conf_mutex;
	struct mutex discipline_mutex;
	struct napi_struct napi;
	struct qeth_rx rx;
	struct delayed_work buffer_reclaim_work;
<<<<<<< HEAD
	int reclaim_index;
	struct work_struct close_dev_work;
};

struct qeth_card_list_struct {
	struct list_head list;
	rwlock_t rwlock;
};
=======
};

static inline bool qeth_card_hw_is_reachable(struct qeth_card *card)
{
	return card->state == CARD_STATE_SOFTSETUP;
}

static inline bool qeth_use_tx_irqs(struct qeth_card *card)
{
	return !IS_IQD(card);
}

static inline void qeth_unlock_channel(struct qeth_card *card,
				       struct qeth_channel *channel)
{
	xchg(&channel->active_cmd, NULL);
	wake_up(&card->wait_q);
}

static inline bool qeth_trylock_channel(struct qeth_channel *channel,
					struct qeth_cmd_buffer *cmd)
{
	return cmpxchg(&channel->active_cmd, NULL, cmd) == NULL;
}
>>>>>>> upstream/android-13

struct qeth_trap_id {
	__u16 lparnr;
	char vmname[8];
	__u8 chpid;
	__u8 ssid;
	__u16 devno;
} __packed;

<<<<<<< HEAD
/*some helper functions*/
#define QETH_CARD_IFNAME(card) (((card)->dev)? (card)->dev->name : "")

static inline bool qeth_netdev_is_registered(struct net_device *dev)
{
	return dev->netdev_ops != NULL;
=======
static inline bool qeth_uses_tx_prio_queueing(struct qeth_card *card)
{
	return card->qdio.do_prio_queueing != QETH_NO_PRIO_QUEUEING;
}

static inline unsigned int qeth_tx_actual_queues(struct qeth_card *card)
{
	struct qeth_priv *priv = netdev_priv(card->dev);

	if (qeth_uses_tx_prio_queueing(card))
		return min(card->dev->num_tx_queues, card->qdio.no_out_queues);

	return min(priv->tx_wanted_queues, card->qdio.no_out_queues);
}

static inline u16 qeth_iqd_translate_txq(struct net_device *dev, u16 txq)
{
	if (txq == QETH_IQD_MCAST_TXQ)
		return dev->num_tx_queues - 1;
	if (txq == dev->num_tx_queues - 1)
		return QETH_IQD_MCAST_TXQ;
	return txq;
}

static inline bool qeth_iqd_is_mcast_queue(struct qeth_card *card,
					   struct qeth_qdio_out_q *queue)
{
	return qeth_iqd_translate_txq(card->dev, queue->queue_no) ==
	       QETH_IQD_MCAST_TXQ;
>>>>>>> upstream/android-13
}

static inline void qeth_scrub_qdio_buffer(struct qdio_buffer *buf,
					  unsigned int elements)
{
	unsigned int i;

	for (i = 0; i < elements; i++)
		memset(&buf->element[i], 0, sizeof(struct qdio_buffer_element));
	buf->element[14].sflags = 0;
	buf->element[15].sflags = 0;
}

/**
 * qeth_get_elements_for_range() -	find number of SBALEs to cover range.
 * @start:				Start of the address range.
 * @end:				Address after the end of the range.
 *
 * Returns the number of pages, and thus QDIO buffer elements, needed to cover
 * the specified address range.
 */
static inline int qeth_get_elements_for_range(addr_t start, addr_t end)
{
	return PFN_UP(end) - PFN_DOWN(start);
}

<<<<<<< HEAD
static inline int qeth_get_micros(void)
{
	return (int) (get_tod_clock() >> 12);
}

static inline int qeth_get_ip_version(struct sk_buff *skb)
{
	struct vlan_ethhdr *veth = vlan_eth_hdr(skb);
	__be16 prot = veth->h_vlan_proto;

	if (prot == htons(ETH_P_8021Q))
		prot = veth->h_vlan_encapsulated_proto;

	switch (prot) {
	case htons(ETH_P_IPV6):
		return 6;
	case htons(ETH_P_IP):
		return 4;
	default:
		return 0;
	}
}

static inline void qeth_rx_csum(struct qeth_card *card, struct sk_buff *skb,
				u8 flags)
{
	if ((card->dev->features & NETIF_F_RXCSUM) &&
	    (flags & QETH_HDR_EXT_CSUM_TRANSP_REQ)) {
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		if (card->options.performance_stats)
			card->perf_stats.rx_csum++;
	} else {
		skb->ip_summed = CHECKSUM_NONE;
	}
}

static inline void qeth_tx_csum(struct sk_buff *skb, u8 *flags, int ipv)
{
	*flags |= QETH_HDR_EXT_CSUM_TRANSP_REQ;
	if ((ipv == 4 && ip_hdr(skb)->protocol == IPPROTO_UDP) ||
	    (ipv == 6 && ipv6_hdr(skb)->nexthdr == IPPROTO_UDP))
		*flags |= QETH_HDR_EXT_UDP;
	if (ipv == 4) {
		/* some HW requires combined L3+L4 csum offload: */
		*flags |= QETH_HDR_EXT_CSUM_HDR_REQ;
		ip_hdr(skb)->check = 0;
	}
=======
static inline int qeth_get_ether_cast_type(struct sk_buff *skb)
{
	u8 *addr = eth_hdr(skb)->h_dest;

	if (is_multicast_ether_addr(addr))
		return is_broadcast_ether_addr(addr) ? RTN_BROADCAST :
						       RTN_MULTICAST;
	return RTN_UNICAST;
}

static inline struct dst_entry *qeth_dst_check_rcu(struct sk_buff *skb,
						   __be16 proto)
{
	struct dst_entry *dst = skb_dst(skb);
	struct rt6_info *rt;

	rt = (struct rt6_info *) dst;
	if (dst) {
		if (proto == htons(ETH_P_IPV6))
			dst = dst_check(dst, rt6_get_cookie(rt));
		else
			dst = dst_check(dst, 0);
	}

	return dst;
}

static inline __be32 qeth_next_hop_v4_rcu(struct sk_buff *skb,
					  struct dst_entry *dst)
{
	struct rtable *rt = (struct rtable *) dst;

	return (rt) ? rt_nexthop(rt, ip_hdr(skb)->daddr) : ip_hdr(skb)->daddr;
}

static inline struct in6_addr *qeth_next_hop_v6_rcu(struct sk_buff *skb,
						    struct dst_entry *dst)
{
	struct rt6_info *rt = (struct rt6_info *) dst;

	if (rt && !ipv6_addr_any(&rt->rt6i_gateway))
		return &rt->rt6i_gateway;
	else
		return &ipv6_hdr(skb)->daddr;
}

static inline void qeth_tx_csum(struct sk_buff *skb, u8 *flags, __be16 proto)
{
	*flags |= QETH_HDR_EXT_CSUM_TRANSP_REQ;
	if ((proto == htons(ETH_P_IP) && ip_hdr(skb)->protocol == IPPROTO_UDP) ||
	    (proto == htons(ETH_P_IPV6) && ipv6_hdr(skb)->nexthdr == IPPROTO_UDP))
		*flags |= QETH_HDR_EXT_UDP;
>>>>>>> upstream/android-13
}

static inline void qeth_put_buffer_pool_entry(struct qeth_card *card,
		struct qeth_buffer_pool_entry *entry)
{
	list_add_tail(&entry->list, &card->qdio.in_buf_pool.entry_list);
}

static inline int qeth_is_diagass_supported(struct qeth_card *card,
		enum qeth_diags_cmds cmd)
{
	return card->info.diagass_support & (__u32)cmd;
}

int qeth_send_simple_setassparms_prot(struct qeth_card *card,
				      enum qeth_ipa_funcs ipa_func,
<<<<<<< HEAD
				      u16 cmd_code, long data,
=======
				      u16 cmd_code, u32 *data,
>>>>>>> upstream/android-13
				      enum qeth_prot_versions prot);
/* IPv4 variant */
static inline int qeth_send_simple_setassparms(struct qeth_card *card,
					       enum qeth_ipa_funcs ipa_func,
<<<<<<< HEAD
					       u16 cmd_code, long data)
=======
					       u16 cmd_code, u32 *data)
>>>>>>> upstream/android-13
{
	return qeth_send_simple_setassparms_prot(card, ipa_func, cmd_code,
						 data, QETH_PROT_IPV4);
}

static inline int qeth_send_simple_setassparms_v6(struct qeth_card *card,
						  enum qeth_ipa_funcs ipa_func,
<<<<<<< HEAD
						  u16 cmd_code, long data)
=======
						  u16 cmd_code, u32 *data)
>>>>>>> upstream/android-13
{
	return qeth_send_simple_setassparms_prot(card, ipa_func, cmd_code,
						 data, QETH_PROT_IPV6);
}

<<<<<<< HEAD
int qeth_get_priority_queue(struct qeth_card *card, struct sk_buff *skb,
			    int ipv);
static inline struct qeth_qdio_out_q *qeth_get_tx_queue(struct qeth_card *card,
							struct sk_buff *skb,
							int ipv, int cast_type)
{
	if (IS_IQD(card) && cast_type != RTN_UNICAST)
		return card->qdio.out_qs[card->qdio.no_out_queues - 1];
	if (!card->qdio.do_prio_queueing)
		return card->qdio.out_qs[card->qdio.default_out_queue];
	return card->qdio.out_qs[qeth_get_priority_queue(card, skb, ipv)];
}

extern struct qeth_discipline qeth_l2_discipline;
extern struct qeth_discipline qeth_l3_discipline;
extern const struct attribute_group *qeth_generic_attr_groups[];
extern const struct attribute_group *qeth_osn_attr_groups[];
extern const struct attribute_group qeth_device_attr_group;
extern const struct attribute_group qeth_device_blkt_group;
extern const struct device_type qeth_generic_devtype;
extern struct workqueue_struct *qeth_wq;

int qeth_card_hw_is_reachable(struct qeth_card *);
const char *qeth_get_cardname_short(struct qeth_card *);
int qeth_realloc_buffer_pool(struct qeth_card *, int);
int qeth_core_load_discipline(struct qeth_card *, enum qeth_discipline_id);
void qeth_core_free_discipline(struct qeth_card *);

/* exports for qeth discipline device drivers */
extern struct qeth_card_list_struct qeth_core_card_list;
extern struct kmem_cache *qeth_core_header_cache;
extern struct qeth_dbf_info qeth_dbf[QETH_DBF_INFOS];

struct net_device *qeth_clone_netdev(struct net_device *orig);
void qeth_set_recovery_task(struct qeth_card *);
void qeth_clear_recovery_task(struct qeth_card *);
void qeth_set_allowed_threads(struct qeth_card *, unsigned long , int);
int qeth_threads_running(struct qeth_card *, unsigned long);
int qeth_wait_for_threads(struct qeth_card *, unsigned long);
int qeth_do_run_thread(struct qeth_card *, unsigned long);
void qeth_clear_thread_start_bit(struct qeth_card *, unsigned long);
void qeth_clear_thread_running_bit(struct qeth_card *, unsigned long);
int qeth_core_hardsetup_card(struct qeth_card *);
void qeth_print_status_message(struct qeth_card *);
int qeth_init_qdio_queues(struct qeth_card *);
=======
int qeth_get_priority_queue(struct qeth_card *card, struct sk_buff *skb);

extern const struct qeth_discipline qeth_l2_discipline;
extern const struct qeth_discipline qeth_l3_discipline;
extern const struct ethtool_ops qeth_ethtool_ops;
extern const struct attribute_group *qeth_dev_groups[];

const char *qeth_get_cardname_short(struct qeth_card *);
int qeth_resize_buffer_pool(struct qeth_card *card, unsigned int count);
int qeth_setup_discipline(struct qeth_card *card, enum qeth_discipline_id disc);
void qeth_remove_discipline(struct qeth_card *card);

/* exports for qeth discipline device drivers */
extern struct qeth_dbf_info qeth_dbf[QETH_DBF_INFOS];

struct net_device *qeth_clone_netdev(struct net_device *orig);
void qeth_set_allowed_threads(struct qeth_card *card, unsigned long threads,
			      int clear_start_mask);
int qeth_threads_running(struct qeth_card *, unsigned long);
int qeth_set_offline(struct qeth_card *card, const struct qeth_discipline *disc,
		     bool resetting);

>>>>>>> upstream/android-13
int qeth_send_ipa_cmd(struct qeth_card *, struct qeth_cmd_buffer *,
		  int (*reply_cb)
		  (struct qeth_card *, struct qeth_reply *, unsigned long),
		  void *);
<<<<<<< HEAD
struct qeth_cmd_buffer *qeth_get_ipacmd_buffer(struct qeth_card *,
			enum qeth_ipa_cmds, enum qeth_prot_versions);
struct sk_buff *qeth_core_get_next_skb(struct qeth_card *,
		struct qeth_qdio_buffer *, struct qdio_buffer_element **, int *,
		struct qeth_hdr **);
void qeth_schedule_recovery(struct qeth_card *);
int qeth_poll(struct napi_struct *napi, int budget);
void qeth_clear_ipacmd_list(struct qeth_card *);
int qeth_qdio_clear_card(struct qeth_card *, int);
void qeth_clear_working_pool_list(struct qeth_card *);
void qeth_clear_cmd_buffers(struct qeth_channel *);
void qeth_clear_qdio_buffers(struct qeth_card *);
void qeth_setadp_promisc_mode(struct qeth_card *);
struct net_device_stats *qeth_get_stats(struct net_device *);
int qeth_setadpparms_change_macaddr(struct qeth_card *);
void qeth_tx_timeout(struct net_device *);
void qeth_prepare_control_data(struct qeth_card *, int,
				struct qeth_cmd_buffer *);
void qeth_release_buffer(struct qeth_channel *, struct qeth_cmd_buffer *);
void qeth_prepare_ipa_cmd(struct qeth_card *card, struct qeth_cmd_buffer *iob);
struct qeth_cmd_buffer *qeth_wait_for_buffer(struct qeth_channel *);
int qeth_query_switch_attributes(struct qeth_card *card,
				  struct qeth_switch_info *sw_info);
int qeth_send_control_data(struct qeth_card *, int, struct qeth_cmd_buffer *,
	int (*reply_cb)(struct qeth_card *, struct qeth_reply*, unsigned long),
	void *reply_param);
int qeth_get_elements_no(struct qeth_card *card, struct sk_buff *skb,
			 int extra_elems, int data_offset);
int qeth_get_elements_for_frags(struct sk_buff *);
int qeth_do_send_packet_fast(struct qeth_qdio_out_q *queue, struct sk_buff *skb,
			     struct qeth_hdr *hdr, unsigned int offset,
			     unsigned int hd_len);
int qeth_do_send_packet(struct qeth_card *card, struct qeth_qdio_out_q *queue,
			struct sk_buff *skb, struct qeth_hdr *hdr,
			unsigned int offset, unsigned int hd_len,
			int elements_needed);
int qeth_do_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);
int qeth_core_get_sset_count(struct net_device *, int);
void qeth_core_get_ethtool_stats(struct net_device *,
				struct ethtool_stats *, u64 *);
void qeth_core_get_strings(struct net_device *, u32, u8 *);
void qeth_core_get_drvinfo(struct net_device *, struct ethtool_drvinfo *);
void qeth_dbf_longtext(debug_info_t *id, int level, char *text, ...);
int qeth_core_ethtool_get_link_ksettings(struct net_device *netdev,
					 struct ethtool_link_ksettings *cmd);
int qeth_set_access_ctrl_online(struct qeth_card *card, int fallback);
int qeth_hdr_chk_and_bounce(struct sk_buff *, struct qeth_hdr **, int);
int qeth_configure_cq(struct qeth_card *, enum qeth_cq);
int qeth_hw_trap(struct qeth_card *, enum qeth_diags_trap_action);
void qeth_trace_features(struct qeth_card *);
void qeth_close_dev(struct qeth_card *);
int qeth_send_setassparms(struct qeth_card *, struct qeth_cmd_buffer *, __u16,
			  long,
			  int (*reply_cb)(struct qeth_card *,
					  struct qeth_reply *, unsigned long),
			  void *);
int qeth_setassparms_cb(struct qeth_card *, struct qeth_reply *, unsigned long);
struct qeth_cmd_buffer *qeth_get_setassparms_cmd(struct qeth_card *,
						 enum qeth_ipa_funcs,
						 __u16, __u16,
						 enum qeth_prot_versions);
=======
struct qeth_cmd_buffer *qeth_ipa_alloc_cmd(struct qeth_card *card,
					   enum qeth_ipa_cmds cmd_code,
					   enum qeth_prot_versions prot,
					   unsigned int data_length);
struct qeth_cmd_buffer *qeth_get_setassparms_cmd(struct qeth_card *card,
						 enum qeth_ipa_funcs ipa_func,
						 u16 cmd_code,
						 unsigned int data_length,
						 enum qeth_prot_versions prot);
struct qeth_cmd_buffer *qeth_get_diag_cmd(struct qeth_card *card,
					  enum qeth_diags_cmds sub_cmd,
					  unsigned int data_length);

int qeth_schedule_recovery(struct qeth_card *card);
int qeth_poll(struct napi_struct *napi, int budget);
void qeth_setadp_promisc_mode(struct qeth_card *card, bool enable);
int qeth_setadpparms_change_macaddr(struct qeth_card *);
void qeth_tx_timeout(struct net_device *, unsigned int txqueue);
int qeth_query_switch_attributes(struct qeth_card *card,
				  struct qeth_switch_info *sw_info);
int qeth_query_card_info(struct qeth_card *card,
			 struct qeth_link_info *link_info);
int qeth_setadpparms_set_access_ctrl(struct qeth_card *card,
				     enum qeth_ipa_isolation_modes mode);

int qeth_do_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);
int qeth_siocdevprivate(struct net_device *dev, struct ifreq *rq,
			void __user *data, int cmd);
void qeth_dbf_longtext(debug_info_t *id, int level, char *text, ...);
int qeth_configure_cq(struct qeth_card *, enum qeth_cq);
int qeth_hw_trap(struct qeth_card *, enum qeth_diags_trap_action);
int qeth_setassparms_cb(struct qeth_card *, struct qeth_reply *, unsigned long);
>>>>>>> upstream/android-13
int qeth_set_features(struct net_device *, netdev_features_t);
void qeth_enable_hw_features(struct net_device *dev);
netdev_features_t qeth_fix_features(struct net_device *, netdev_features_t);
netdev_features_t qeth_features_check(struct sk_buff *skb,
				      struct net_device *dev,
				      netdev_features_t features);
<<<<<<< HEAD
int qeth_vm_request_mac(struct qeth_card *card);
int qeth_add_hw_header(struct qeth_card *card, struct sk_buff *skb,
		       struct qeth_hdr **hdr, unsigned int hdr_len,
		       unsigned int proto_len, unsigned int *elements);

/* exports for OSN */
int qeth_osn_assist(struct net_device *, void *, int);
int qeth_osn_register(unsigned char *read_dev_no, struct net_device **,
		int (*assist_cb)(struct net_device *, void *),
		int (*data_cb)(struct sk_buff *));
void qeth_osn_deregister(struct net_device *);
=======
void qeth_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *stats);
int qeth_set_real_num_tx_queues(struct qeth_card *card, unsigned int count);
u16 qeth_iqd_select_queue(struct net_device *dev, struct sk_buff *skb,
			  u8 cast_type, struct net_device *sb_dev);
int qeth_open(struct net_device *dev);
int qeth_stop(struct net_device *dev);

int qeth_vm_request_mac(struct qeth_card *card);
int qeth_xmit(struct qeth_card *card, struct sk_buff *skb,
	      struct qeth_qdio_out_q *queue, __be16 proto,
	      void (*fill_header)(struct qeth_qdio_out_q *queue,
				  struct qeth_hdr *hdr, struct sk_buff *skb,
				  __be16 proto, unsigned int data_len));
>>>>>>> upstream/android-13

#endif /* __QETH_CORE_H__ */
