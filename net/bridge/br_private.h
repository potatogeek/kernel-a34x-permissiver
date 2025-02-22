<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _BR_PRIVATE_H
#define _BR_PRIVATE_H

#include <linux/netdevice.h>
#include <linux/if_bridge.h>
#include <linux/netpoll.h>
#include <linux/u64_stats_sync.h>
#include <net/route.h>
#include <net/ip6_fib.h>
#include <linux/if_vlan.h>
#include <linux/rhashtable.h>
#include <linux/refcount.h>

#define BR_HASH_BITS 8
#define BR_HASH_SIZE (1 << BR_HASH_BITS)

#define BR_HOLD_TIME (1*HZ)

#define BR_PORT_BITS	10
#define BR_MAX_PORTS	(1<<BR_PORT_BITS)

<<<<<<< HEAD
=======
#define BR_MULTICAST_DEFAULT_HASH_MAX 4096
#define BR_MULTICAST_QUERY_INTVL_MIN msecs_to_jiffies(1000)
#define BR_MULTICAST_STARTUP_QUERY_INTVL_MIN BR_MULTICAST_QUERY_INTVL_MIN

#define BR_HWDOM_MAX BITS_PER_LONG

>>>>>>> upstream/android-13
#define BR_VERSION	"2.3"

/* Control of forwarding link local multicast */
#define BR_GROUPFWD_DEFAULT	0
/* Don't allow forwarding of control protocols like STP, MAC PAUSE and LACP */
enum {
	BR_GROUPFWD_STP		= BIT(0),
	BR_GROUPFWD_MACPAUSE	= BIT(1),
	BR_GROUPFWD_LACP	= BIT(2),
};

#define BR_GROUPFWD_RESTRICTED (BR_GROUPFWD_STP | BR_GROUPFWD_MACPAUSE | \
				BR_GROUPFWD_LACP)
/* The Nearest Customer Bridge Group Address, 01-80-C2-00-00-[00,0B,0C,0D,0F] */
#define BR_GROUPFWD_8021AD	0xB801u

/* Path to usermode spanning tree program */
#define BR_STP_PROG	"/sbin/bridge-stp"

<<<<<<< HEAD
=======
#define BR_FDB_NOTIFY_SETTABLE_BITS (FDB_NOTIFY_BIT | FDB_NOTIFY_INACTIVE_BIT)

>>>>>>> upstream/android-13
typedef struct bridge_id bridge_id;
typedef struct mac_addr mac_addr;
typedef __u16 port_id;

<<<<<<< HEAD
struct bridge_id
{
=======
struct bridge_id {
>>>>>>> upstream/android-13
	unsigned char	prio[2];
	unsigned char	addr[ETH_ALEN];
};

<<<<<<< HEAD
struct mac_addr
{
=======
struct mac_addr {
>>>>>>> upstream/android-13
	unsigned char	addr[ETH_ALEN];
};

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
/* our own querier */
struct bridge_mcast_own_query {
	struct timer_list	timer;
	u32			startup_sent;
};

/* other querier */
struct bridge_mcast_other_query {
	struct timer_list		timer;
	unsigned long			delay_time;
};

/* selected querier */
struct bridge_mcast_querier {
	struct br_ip addr;
<<<<<<< HEAD
	struct net_bridge_port __rcu	*port;
=======
	int port_ifidx;
	seqcount_spinlock_t seq;
>>>>>>> upstream/android-13
};

/* IGMP/MLD statistics */
struct bridge_mcast_stats {
	struct br_mcast_stats mstats;
	struct u64_stats_sync syncp;
};
#endif

<<<<<<< HEAD
struct br_vlan_stats {
	u64 rx_bytes;
	u64 rx_packets;
	u64 tx_bytes;
	u64 tx_packets;
	struct u64_stats_sync syncp;
};

struct br_tunnel_info {
	__be64			tunnel_id;
	struct metadata_dst	*tunnel_dst;
=======
/* net_bridge_mcast_port must be always defined due to forwarding stubs */
struct net_bridge_mcast_port {
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	struct net_bridge_port		*port;
	struct net_bridge_vlan		*vlan;

	struct bridge_mcast_own_query	ip4_own_query;
	struct timer_list		ip4_mc_router_timer;
	struct hlist_node		ip4_rlist;
#if IS_ENABLED(CONFIG_IPV6)
	struct bridge_mcast_own_query	ip6_own_query;
	struct timer_list		ip6_mc_router_timer;
	struct hlist_node		ip6_rlist;
#endif /* IS_ENABLED(CONFIG_IPV6) */
	unsigned char			multicast_router;
#endif /* CONFIG_BRIDGE_IGMP_SNOOPING */
};

/* net_bridge_mcast must be always defined due to forwarding stubs */
struct net_bridge_mcast {
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	struct net_bridge		*br;
	struct net_bridge_vlan		*vlan;

	u32				multicast_last_member_count;
	u32				multicast_startup_query_count;

	u8				multicast_querier;
	u8				multicast_igmp_version;
	u8				multicast_router;
#if IS_ENABLED(CONFIG_IPV6)
	u8				multicast_mld_version;
#endif
	unsigned long			multicast_last_member_interval;
	unsigned long			multicast_membership_interval;
	unsigned long			multicast_querier_interval;
	unsigned long			multicast_query_interval;
	unsigned long			multicast_query_response_interval;
	unsigned long			multicast_startup_query_interval;
	struct hlist_head		ip4_mc_router_list;
	struct timer_list		ip4_mc_router_timer;
	struct bridge_mcast_other_query	ip4_other_query;
	struct bridge_mcast_own_query	ip4_own_query;
	struct bridge_mcast_querier	ip4_querier;
#if IS_ENABLED(CONFIG_IPV6)
	struct hlist_head		ip6_mc_router_list;
	struct timer_list		ip6_mc_router_timer;
	struct bridge_mcast_other_query	ip6_other_query;
	struct bridge_mcast_own_query	ip6_own_query;
	struct bridge_mcast_querier	ip6_querier;
#endif /* IS_ENABLED(CONFIG_IPV6) */
#endif /* CONFIG_BRIDGE_IGMP_SNOOPING */
};

struct br_tunnel_info {
	__be64				tunnel_id;
	struct metadata_dst __rcu	*tunnel_dst;
};

/* private vlan flags */
enum {
	BR_VLFLAG_PER_PORT_STATS = BIT(0),
	BR_VLFLAG_ADDED_BY_SWITCHDEV = BIT(1),
	BR_VLFLAG_MCAST_ENABLED = BIT(2),
	BR_VLFLAG_GLOBAL_MCAST_ENABLED = BIT(3),
>>>>>>> upstream/android-13
};

/**
 * struct net_bridge_vlan - per-vlan entry
 *
 * @vnode: rhashtable member
 * @vid: VLAN id
 * @flags: bridge vlan flags
<<<<<<< HEAD
=======
 * @priv_flags: private (in-kernel) bridge vlan flags
 * @state: STP state (e.g. blocking, learning, forwarding)
>>>>>>> upstream/android-13
 * @stats: per-cpu VLAN statistics
 * @br: if MASTER flag set, this points to a bridge struct
 * @port: if MASTER flag unset, this points to a port struct
 * @refcnt: if MASTER flag set, this is bumped for each port referencing it
 * @brvlan: if MASTER flag unset, this points to the global per-VLAN context
 *          for this VLAN entry
<<<<<<< HEAD
=======
 * @br_mcast_ctx: if MASTER flag set, this is the global vlan multicast context
 * @port_mcast_ctx: if MASTER flag unset, this is the per-port/vlan multicast
 *                  context
>>>>>>> upstream/android-13
 * @vlist: sorted list of VLAN entries
 * @rcu: used for entry destruction
 *
 * This structure is shared between the global per-VLAN entries contained in
 * the bridge rhashtable and the local per-port per-VLAN entries contained in
 * the port's rhashtable. The union entries should be interpreted depending on
 * the entry flags that are set.
 */
struct net_bridge_vlan {
	struct rhash_head		vnode;
	struct rhash_head		tnode;
	u16				vid;
	u16				flags;
<<<<<<< HEAD
	struct br_vlan_stats __percpu	*stats;
=======
	u16				priv_flags;
	u8				state;
	struct pcpu_sw_netstats __percpu *stats;
>>>>>>> upstream/android-13
	union {
		struct net_bridge	*br;
		struct net_bridge_port	*port;
	};
	union {
		refcount_t		refcnt;
		struct net_bridge_vlan	*brvlan;
	};

	struct br_tunnel_info		tinfo;

<<<<<<< HEAD
=======
	union {
		struct net_bridge_mcast		br_mcast_ctx;
		struct net_bridge_mcast_port	port_mcast_ctx;
	};

>>>>>>> upstream/android-13
	struct list_head		vlist;

	struct rcu_head			rcu;
};

/**
 * struct net_bridge_vlan_group
 *
 * @vlan_hash: VLAN entry rhashtable
 * @vlan_list: sorted VLAN entry list
 * @num_vlans: number of total VLAN entries
 * @pvid: PVID VLAN id
<<<<<<< HEAD
=======
 * @pvid_state: PVID's STP state (e.g. forwarding, learning, blocking)
>>>>>>> upstream/android-13
 *
 * IMPORTANT: Be careful when checking if there're VLAN entries using list
 *            primitives because the bridge can have entries in its list which
 *            are just for global context but not for filtering, i.e. they have
 *            the master flag set but not the brentry flag. If you have to check
 *            if there're "real" entries in the bridge please test @num_vlans
 */
struct net_bridge_vlan_group {
	struct rhashtable		vlan_hash;
	struct rhashtable		tunnel_hash;
	struct list_head		vlan_list;
	u16				num_vlans;
	u16				pvid;
<<<<<<< HEAD
=======
	u8				pvid_state;
};

/* bridge fdb flags */
enum {
	BR_FDB_LOCAL,
	BR_FDB_STATIC,
	BR_FDB_STICKY,
	BR_FDB_ADDED_BY_USER,
	BR_FDB_ADDED_BY_EXT_LEARN,
	BR_FDB_OFFLOADED,
	BR_FDB_NOTIFY,
	BR_FDB_NOTIFY_INACTIVE
>>>>>>> upstream/android-13
};

struct net_bridge_fdb_key {
	mac_addr addr;
	u16 vlan_id;
};

struct net_bridge_fdb_entry {
	struct rhash_head		rhnode;
	struct net_bridge_port		*dst;

	struct net_bridge_fdb_key	key;
	struct hlist_node		fdb_node;
<<<<<<< HEAD
	unsigned char			is_local:1,
					is_static:1,
					added_by_user:1,
					added_by_external_learn:1,
					offloaded:1;
=======
	unsigned long			flags;
>>>>>>> upstream/android-13

	/* write-heavy members should not affect lookups */
	unsigned long			updated ____cacheline_aligned_in_smp;
	unsigned long			used;

	struct rcu_head			rcu;
};

#define MDB_PG_FLAGS_PERMANENT	BIT(0)
#define MDB_PG_FLAGS_OFFLOAD	BIT(1)
<<<<<<< HEAD

struct net_bridge_port_group {
	struct net_bridge_port		*port;
	struct net_bridge_port_group __rcu *next;
	struct hlist_node		mglist;
	struct rcu_head			rcu;
	struct timer_list		timer;
	struct br_ip			addr;
	unsigned char			eth_addr[ETH_ALEN] __aligned(2);
	unsigned char			flags;
};

struct net_bridge_mdb_entry
{
	struct hlist_node		hlist[2];
	struct net_bridge		*br;
	struct net_bridge_port_group __rcu *ports;
	struct rcu_head			rcu;
	struct timer_list		timer;
	struct br_ip			addr;
	bool				host_joined;
};

struct net_bridge_mdb_htable
{
	struct hlist_head		*mhash;
	struct rcu_head			rcu;
	struct net_bridge_mdb_htable	*old;
	u32				size;
	u32				max;
	u32				secret;
	u32				ver;
=======
#define MDB_PG_FLAGS_FAST_LEAVE	BIT(2)
#define MDB_PG_FLAGS_STAR_EXCL	BIT(3)
#define MDB_PG_FLAGS_BLOCKED	BIT(4)

#define PG_SRC_ENT_LIMIT	32

#define BR_SGRP_F_DELETE	BIT(0)
#define BR_SGRP_F_SEND		BIT(1)
#define BR_SGRP_F_INSTALLED	BIT(2)

struct net_bridge_mcast_gc {
	struct hlist_node		gc_node;
	void				(*destroy)(struct net_bridge_mcast_gc *gc);
};

struct net_bridge_group_src {
	struct hlist_node		node;

	struct br_ip			addr;
	struct net_bridge_port_group	*pg;
	u8				flags;
	u8				src_query_rexmit_cnt;
	struct timer_list		timer;

	struct net_bridge		*br;
	struct net_bridge_mcast_gc	mcast_gc;
	struct rcu_head			rcu;
};

struct net_bridge_port_group_sg_key {
	struct net_bridge_port		*port;
	struct br_ip			addr;
};

struct net_bridge_port_group {
	struct net_bridge_port_group __rcu *next;
	struct net_bridge_port_group_sg_key key;
	unsigned char			eth_addr[ETH_ALEN] __aligned(2);
	unsigned char			flags;
	unsigned char			filter_mode;
	unsigned char			grp_query_rexmit_cnt;
	unsigned char			rt_protocol;

	struct hlist_head		src_list;
	unsigned int			src_ents;
	struct timer_list		timer;
	struct timer_list		rexmit_timer;
	struct hlist_node		mglist;
	struct rb_root			eht_set_tree;
	struct rb_root			eht_host_tree;

	struct rhash_head		rhnode;
	struct net_bridge_mcast_gc	mcast_gc;
	struct rcu_head			rcu;
};

struct net_bridge_mdb_entry {
	struct rhash_head		rhnode;
	struct net_bridge		*br;
	struct net_bridge_port_group __rcu *ports;
	struct br_ip			addr;
	bool				host_joined;

	struct timer_list		timer;
	struct hlist_node		mdb_node;

	struct net_bridge_mcast_gc	mcast_gc;
	struct rcu_head			rcu;
>>>>>>> upstream/android-13
};

struct net_bridge_port {
	struct net_bridge		*br;
	struct net_device		*dev;
	struct list_head		list;

	unsigned long			flags;
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
	struct net_bridge_vlan_group	__rcu *vlgrp;
#endif
	struct net_bridge_port		__rcu *backup_port;

	/* STP */
	u8				priority;
	u8				state;
	u16				port_no;
	unsigned char			topology_change_ack;
	unsigned char			config_pending;
	port_id				port_id;
	port_id				designated_port;
	bridge_id			designated_root;
	bridge_id			designated_bridge;
	u32				path_cost;
	u32				designated_cost;
	unsigned long			designated_age;

	struct timer_list		forward_delay_timer;
	struct timer_list		hold_timer;
	struct timer_list		message_age_timer;
	struct kobject			kobj;
	struct rcu_head			rcu;

<<<<<<< HEAD
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	struct bridge_mcast_own_query	ip4_own_query;
#if IS_ENABLED(CONFIG_IPV6)
	struct bridge_mcast_own_query	ip6_own_query;
#endif /* IS_ENABLED(CONFIG_IPV6) */
	unsigned char			multicast_router;
	struct bridge_mcast_stats	__percpu *mcast_stats;
	struct timer_list		multicast_router_timer;
	struct hlist_head		mglist;
	struct hlist_node		rlist;
=======
	struct net_bridge_mcast_port	multicast_ctx;

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	struct bridge_mcast_stats	__percpu *mcast_stats;

	u32				multicast_eht_hosts_limit;
	u32				multicast_eht_hosts_cnt;
	struct hlist_head		mglist;
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_SYSFS
	char				sysfs_name[IFNAMSIZ];
#endif

#ifdef CONFIG_NET_POLL_CONTROLLER
	struct netpoll			*np;
#endif
#ifdef CONFIG_NET_SWITCHDEV
<<<<<<< HEAD
	int				offload_fwd_mark;
#endif
	u16				group_fwd_mask;
	u16				backup_redirected_cnt;
=======
	/* Identifier used to group ports that share the same switchdev
	 * hardware domain.
	 */
	int				hwdom;
	int				offload_count;
	struct netdev_phys_item_id	ppid;
#endif
	u16				group_fwd_mask;
	u16				backup_redirected_cnt;

	struct bridge_stp_xstats	stp_xstats;
>>>>>>> upstream/android-13
};

#define kobj_to_brport(obj)	container_of(obj, struct net_bridge_port, kobj)

#define br_auto_port(p) ((p)->flags & BR_AUTO_MASK)
#define br_promisc_port(p) ((p)->flags & BR_PROMISC)

<<<<<<< HEAD
#define br_port_exists(dev) (dev->priv_flags & IFF_BRIDGE_PORT)

=======
>>>>>>> upstream/android-13
static inline struct net_bridge_port *br_port_get_rcu(const struct net_device *dev)
{
	return rcu_dereference(dev->rx_handler_data);
}

static inline struct net_bridge_port *br_port_get_rtnl(const struct net_device *dev)
{
<<<<<<< HEAD
	return br_port_exists(dev) ?
=======
	return netif_is_bridge_port(dev) ?
>>>>>>> upstream/android-13
		rtnl_dereference(dev->rx_handler_data) : NULL;
}

static inline struct net_bridge_port *br_port_get_rtnl_rcu(const struct net_device *dev)
{
<<<<<<< HEAD
	return br_port_exists(dev) ?
		rcu_dereference_rtnl(dev->rx_handler_data) : NULL;
}

struct net_bridge {
	spinlock_t			lock;
	spinlock_t			hash_lock;
	struct list_head		port_list;
	struct net_device		*dev;
	struct pcpu_sw_netstats		__percpu *stats;
	/* These fields are accessed on each packet */
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
	u8				vlan_enabled;
	u8				vlan_stats_enabled;
=======
	return netif_is_bridge_port(dev) ?
		rcu_dereference_rtnl(dev->rx_handler_data) : NULL;
}

enum net_bridge_opts {
	BROPT_VLAN_ENABLED,
	BROPT_VLAN_STATS_ENABLED,
	BROPT_NF_CALL_IPTABLES,
	BROPT_NF_CALL_IP6TABLES,
	BROPT_NF_CALL_ARPTABLES,
	BROPT_GROUP_ADDR_SET,
	BROPT_MULTICAST_ENABLED,
	BROPT_MULTICAST_QUERY_USE_IFADDR,
	BROPT_MULTICAST_STATS_ENABLED,
	BROPT_HAS_IPV6_ADDR,
	BROPT_NEIGH_SUPPRESS_ENABLED,
	BROPT_MTU_SET_BY_USER,
	BROPT_VLAN_STATS_PER_PORT,
	BROPT_NO_LL_LEARN,
	BROPT_VLAN_BRIDGE_BINDING,
	BROPT_MCAST_VLAN_SNOOPING_ENABLED,
};

struct net_bridge {
	spinlock_t			lock;
	spinlock_t			hash_lock;
	struct hlist_head		frame_type_list;
	struct net_device		*dev;
	unsigned long			options;
	/* These fields are accessed on each packet */
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
>>>>>>> upstream/android-13
	__be16				vlan_proto;
	u16				default_pvid;
	struct net_bridge_vlan_group	__rcu *vlgrp;
#endif

	struct rhashtable		fdb_hash_tbl;
<<<<<<< HEAD
=======
	struct list_head		port_list;
>>>>>>> upstream/android-13
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	union {
		struct rtable		fake_rtable;
		struct rt6_info		fake_rt6_info;
	};
<<<<<<< HEAD
	bool				nf_call_iptables;
	bool				nf_call_ip6tables;
	bool				nf_call_arptables;
=======
>>>>>>> upstream/android-13
#endif
	u16				group_fwd_mask;
	u16				group_fwd_mask_required;

	/* STP */
	bridge_id			designated_root;
	bridge_id			bridge_id;
<<<<<<< HEAD
	u32				root_path_cost;
=======
>>>>>>> upstream/android-13
	unsigned char			topology_change;
	unsigned char			topology_change_detected;
	u16				root_port;
	unsigned long			max_age;
	unsigned long			hello_time;
	unsigned long			forward_delay;
	unsigned long			ageing_time;
	unsigned long			bridge_max_age;
	unsigned long			bridge_hello_time;
	unsigned long			bridge_forward_delay;
	unsigned long			bridge_ageing_time;
<<<<<<< HEAD

	u8				group_addr[ETH_ALEN];
	bool				group_addr_set;
=======
	u32				root_path_cost;

	u8				group_addr[ETH_ALEN];
>>>>>>> upstream/android-13

	enum {
		BR_NO_STP, 		/* no spanning tree */
		BR_KERNEL_STP,		/* old STP in kernel */
		BR_USER_STP,		/* new RSTP in userspace */
	} stp_enabled;

<<<<<<< HEAD
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	unsigned char			multicast_router;

	u8				multicast_disabled:1;
	u8				multicast_querier:1;
	u8				multicast_query_use_ifaddr:1;
	u8				has_ipv6_addr:1;
	u8				multicast_stats_enabled:1;

	u32				hash_elasticity;
	u32				hash_max;

	u32				multicast_last_member_count;
	u32				multicast_startup_query_count;

	u8				multicast_igmp_version;

	unsigned long			multicast_last_member_interval;
	unsigned long			multicast_membership_interval;
	unsigned long			multicast_querier_interval;
	unsigned long			multicast_query_interval;
	unsigned long			multicast_query_response_interval;
	unsigned long			multicast_startup_query_interval;

	spinlock_t			multicast_lock;
	struct net_bridge_mdb_htable __rcu *mdb;
	struct hlist_head		router_list;

	struct timer_list		multicast_router_timer;
	struct bridge_mcast_other_query	ip4_other_query;
	struct bridge_mcast_own_query	ip4_own_query;
	struct bridge_mcast_querier	ip4_querier;
	struct bridge_mcast_stats	__percpu *mcast_stats;
#if IS_ENABLED(CONFIG_IPV6)
	struct bridge_mcast_other_query	ip6_other_query;
	struct bridge_mcast_own_query	ip6_own_query;
	struct bridge_mcast_querier	ip6_querier;
	u8				multicast_mld_version;
#endif /* IS_ENABLED(CONFIG_IPV6) */
=======
	struct net_bridge_mcast		multicast_ctx;

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	struct bridge_mcast_stats	__percpu *mcast_stats;

	u32				hash_max;

	spinlock_t			multicast_lock;

	struct rhashtable		mdb_hash_tbl;
	struct rhashtable		sg_port_tbl;

	struct hlist_head		mcast_gc_list;
	struct hlist_head		mdb_list;

	struct work_struct		mcast_gc_work;
>>>>>>> upstream/android-13
#endif

	struct timer_list		hello_timer;
	struct timer_list		tcn_timer;
	struct timer_list		topology_change_timer;
	struct delayed_work		gc_work;
	struct kobject			*ifobj;
	u32				auto_cnt;

#ifdef CONFIG_NET_SWITCHDEV
<<<<<<< HEAD
	int offload_fwd_mark;
#endif
	bool				neigh_suppress_enabled;
	bool				mtu_set_by_user;
	struct hlist_head		fdb_list;
=======
	/* Counter used to make sure that hardware domains get unique
	 * identifiers in case a bridge spans multiple switchdev instances.
	 */
	int				last_hwdom;
	/* Bit mask of hardware domain numbers in use */
	unsigned long			busy_hwdoms;
#endif
	struct hlist_head		fdb_list;

#if IS_ENABLED(CONFIG_BRIDGE_MRP)
	struct hlist_head		mrp_list;
#endif
#if IS_ENABLED(CONFIG_BRIDGE_CFM)
	struct hlist_head		mep_list;
#endif
>>>>>>> upstream/android-13
};

struct br_input_skb_cb {
	struct net_device *brdev;

<<<<<<< HEAD
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	int igmp;
	int mrouters_only;
#endif

	bool proxyarp_replied;
	bool src_port_isolated;

#ifdef CONFIG_BRIDGE_VLAN_FILTERING
	bool vlan_filtered;
#endif

#ifdef CONFIG_NET_SWITCHDEV
	int offload_fwd_mark;
=======
	u16 frag_max_size;
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	u8 igmp;
	u8 mrouters_only:1;
#endif
	u8 proxyarp_replied:1;
	u8 src_port_isolated:1;
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
	u8 vlan_filtered:1;
#endif
#ifdef CONFIG_NETFILTER_FAMILY_BRIDGE
	u8 br_netfilter_broute:1;
#endif

#ifdef CONFIG_NET_SWITCHDEV
	/* Set if TX data plane offloading is used towards at least one
	 * hardware domain.
	 */
	u8 tx_fwd_offload:1;
	/* The switchdev hardware domain from which this packet was received.
	 * If skb->offload_fwd_mark was set, then this packet was already
	 * forwarded by hardware to the other ports in the source hardware
	 * domain, otherwise it wasn't.
	 */
	int src_hwdom;
	/* Bit mask of hardware domains towards this packet has already been
	 * transmitted using the TX data plane offload.
	 */
	unsigned long fwd_hwdoms;
>>>>>>> upstream/android-13
#endif
};

#define BR_INPUT_SKB_CB(__skb)	((struct br_input_skb_cb *)(__skb)->cb)

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
# define BR_INPUT_SKB_CB_MROUTERS_ONLY(__skb)	(BR_INPUT_SKB_CB(__skb)->mrouters_only)
#else
# define BR_INPUT_SKB_CB_MROUTERS_ONLY(__skb)	(0)
#endif

#define br_printk(level, br, format, args...)	\
	printk(level "%s: " format, (br)->dev->name, ##args)

#define br_err(__br, format, args...)			\
	br_printk(KERN_ERR, __br, format, ##args)
#define br_warn(__br, format, args...)			\
	br_printk(KERN_WARNING, __br, format, ##args)
#define br_notice(__br, format, args...)		\
	br_printk(KERN_NOTICE, __br, format, ##args)
#define br_info(__br, format, args...)			\
	br_printk(KERN_INFO, __br, format, ##args)

#define br_debug(br, format, args...)			\
	pr_debug("%s: " format,  (br)->dev->name, ##args)

/* called under bridge lock */
static inline int br_is_root_bridge(const struct net_bridge *br)
{
	return !memcmp(&br->bridge_id, &br->designated_root, 8);
}

/* check if a VLAN entry is global */
static inline bool br_vlan_is_master(const struct net_bridge_vlan *v)
{
	return v->flags & BRIDGE_VLAN_INFO_MASTER;
}

/* check if a VLAN entry is used by the bridge */
static inline bool br_vlan_is_brentry(const struct net_bridge_vlan *v)
{
	return v->flags & BRIDGE_VLAN_INFO_BRENTRY;
}

/* check if we should use the vlan entry, returns false if it's only context */
static inline bool br_vlan_should_use(const struct net_bridge_vlan *v)
{
	if (br_vlan_is_master(v)) {
		if (br_vlan_is_brentry(v))
			return true;
		else
			return false;
	}

	return true;
}

<<<<<<< HEAD
=======
static inline bool nbp_state_should_learn(const struct net_bridge_port *p)
{
	return p->state == BR_STATE_LEARNING || p->state == BR_STATE_FORWARDING;
}

static inline bool br_vlan_valid_id(u16 vid, struct netlink_ext_ack *extack)
{
	bool ret = vid > 0 && vid < VLAN_VID_MASK;

	if (!ret)
		NL_SET_ERR_MSG_MOD(extack, "Vlan id is invalid");

	return ret;
}

static inline bool br_vlan_valid_range(const struct bridge_vlan_info *cur,
				       const struct bridge_vlan_info *last,
				       struct netlink_ext_ack *extack)
{
	/* pvid flag is not allowed in ranges */
	if (cur->flags & BRIDGE_VLAN_INFO_PVID) {
		NL_SET_ERR_MSG_MOD(extack, "Pvid isn't allowed in a range");
		return false;
	}

	/* when cur is the range end, check if:
	 *  - it has range start flag
	 *  - range ids are invalid (end is equal to or before start)
	 */
	if (last) {
		if (cur->flags & BRIDGE_VLAN_INFO_RANGE_BEGIN) {
			NL_SET_ERR_MSG_MOD(extack, "Found a new vlan range start while processing one");
			return false;
		} else if (!(cur->flags & BRIDGE_VLAN_INFO_RANGE_END)) {
			NL_SET_ERR_MSG_MOD(extack, "Vlan range end flag is missing");
			return false;
		} else if (cur->vid <= last->vid) {
			NL_SET_ERR_MSG_MOD(extack, "End vlan id is less than or equal to start vlan id");
			return false;
		}
	}

	/* check for required range flags */
	if (!(cur->flags & (BRIDGE_VLAN_INFO_RANGE_BEGIN |
			    BRIDGE_VLAN_INFO_RANGE_END))) {
		NL_SET_ERR_MSG_MOD(extack, "Both vlan range flags are missing");
		return false;
	}

	return true;
}

static inline u8 br_vlan_multicast_router(const struct net_bridge_vlan *v)
{
	u8 mcast_router = MDB_RTR_TYPE_DISABLED;

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	if (!br_vlan_is_master(v))
		mcast_router = v->port_mcast_ctx.multicast_router;
	else
		mcast_router = v->br_mcast_ctx.multicast_router;
#endif

	return mcast_router;
}

static inline int br_afspec_cmd_to_rtm(int cmd)
{
	switch (cmd) {
	case RTM_SETLINK:
		return RTM_NEWVLAN;
	case RTM_DELLINK:
		return RTM_DELVLAN;
	}

	return 0;
}

static inline int br_opt_get(const struct net_bridge *br,
			     enum net_bridge_opts opt)
{
	return test_bit(opt, &br->options);
}

int br_boolopt_toggle(struct net_bridge *br, enum br_boolopt_id opt, bool on,
		      struct netlink_ext_ack *extack);
int br_boolopt_get(const struct net_bridge *br, enum br_boolopt_id opt);
int br_boolopt_multi_toggle(struct net_bridge *br,
			    struct br_boolopt_multi *bm,
			    struct netlink_ext_ack *extack);
void br_boolopt_multi_get(const struct net_bridge *br,
			  struct br_boolopt_multi *bm);
void br_opt_toggle(struct net_bridge *br, enum net_bridge_opts opt, bool on);

>>>>>>> upstream/android-13
/* br_device.c */
void br_dev_setup(struct net_device *dev);
void br_dev_delete(struct net_device *dev, struct list_head *list);
netdev_tx_t br_dev_xmit(struct sk_buff *skb, struct net_device *dev);
#ifdef CONFIG_NET_POLL_CONTROLLER
static inline void br_netpoll_send_skb(const struct net_bridge_port *p,
				       struct sk_buff *skb)
{
<<<<<<< HEAD
	struct netpoll *np = p->np;

	if (np)
		netpoll_send_skb(np, skb);
=======
	netpoll_send_skb(p->np, skb);
>>>>>>> upstream/android-13
}

int br_netpoll_enable(struct net_bridge_port *p);
void br_netpoll_disable(struct net_bridge_port *p);
#else
static inline void br_netpoll_send_skb(const struct net_bridge_port *p,
				       struct sk_buff *skb)
{
}

static inline int br_netpoll_enable(struct net_bridge_port *p)
{
	return 0;
}

static inline void br_netpoll_disable(struct net_bridge_port *p)
{
}
#endif

/* br_fdb.c */
int br_fdb_init(void);
void br_fdb_fini(void);
int br_fdb_hash_init(struct net_bridge *br);
void br_fdb_hash_fini(struct net_bridge *br);
void br_fdb_flush(struct net_bridge *br);
void br_fdb_find_delete_local(struct net_bridge *br,
			      const struct net_bridge_port *p,
			      const unsigned char *addr, u16 vid);
void br_fdb_changeaddr(struct net_bridge_port *p, const unsigned char *newaddr);
void br_fdb_change_mac_address(struct net_bridge *br, const u8 *newaddr);
void br_fdb_cleanup(struct work_struct *work);
void br_fdb_delete_by_port(struct net_bridge *br,
			   const struct net_bridge_port *p, u16 vid, int do_all);
struct net_bridge_fdb_entry *br_fdb_find_rcu(struct net_bridge *br,
					     const unsigned char *addr,
					     __u16 vid);
int br_fdb_test_addr(struct net_device *dev, unsigned char *addr);
int br_fdb_fillbuf(struct net_bridge *br, void *buf, unsigned long count,
		   unsigned long off);
int br_fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		  const unsigned char *addr, u16 vid);
void br_fdb_update(struct net_bridge *br, struct net_bridge_port *source,
<<<<<<< HEAD
		   const unsigned char *addr, u16 vid, bool added_by_user);
=======
		   const unsigned char *addr, u16 vid, unsigned long flags);
>>>>>>> upstream/android-13

int br_fdb_delete(struct ndmsg *ndm, struct nlattr *tb[],
		  struct net_device *dev, const unsigned char *addr, u16 vid);
int br_fdb_add(struct ndmsg *nlh, struct nlattr *tb[], struct net_device *dev,
<<<<<<< HEAD
	       const unsigned char *addr, u16 vid, u16 nlh_flags);
int br_fdb_dump(struct sk_buff *skb, struct netlink_callback *cb,
		struct net_device *dev, struct net_device *fdev, int *idx);
=======
	       const unsigned char *addr, u16 vid, u16 nlh_flags,
	       struct netlink_ext_ack *extack);
int br_fdb_dump(struct sk_buff *skb, struct netlink_callback *cb,
		struct net_device *dev, struct net_device *fdev, int *idx);
int br_fdb_get(struct sk_buff *skb, struct nlattr *tb[], struct net_device *dev,
	       const unsigned char *addr, u16 vid, u32 portid, u32 seq,
	       struct netlink_ext_ack *extack);
>>>>>>> upstream/android-13
int br_fdb_sync_static(struct net_bridge *br, struct net_bridge_port *p);
void br_fdb_unsync_static(struct net_bridge *br, struct net_bridge_port *p);
int br_fdb_external_learn_add(struct net_bridge *br, struct net_bridge_port *p,
			      const unsigned char *addr, u16 vid,
			      bool swdev_notify);
int br_fdb_external_learn_del(struct net_bridge *br, struct net_bridge_port *p,
			      const unsigned char *addr, u16 vid,
			      bool swdev_notify);
void br_fdb_offloaded_set(struct net_bridge *br, struct net_bridge_port *p,
<<<<<<< HEAD
			  const unsigned char *addr, u16 vid);
=======
			  const unsigned char *addr, u16 vid, bool offloaded);
int br_fdb_replay(const struct net_device *br_dev, const void *ctx, bool adding,
		  struct notifier_block *nb);
>>>>>>> upstream/android-13

/* br_forward.c */
enum br_pkt_type {
	BR_PKT_UNICAST,
	BR_PKT_MULTICAST,
	BR_PKT_BROADCAST
};
int br_dev_queue_push_xmit(struct net *net, struct sock *sk, struct sk_buff *skb);
void br_forward(const struct net_bridge_port *to, struct sk_buff *skb,
		bool local_rcv, bool local_orig);
int br_forward_finish(struct net *net, struct sock *sk, struct sk_buff *skb);
void br_flood(struct net_bridge *br, struct sk_buff *skb,
	      enum br_pkt_type pkt_type, bool local_rcv, bool local_orig);

/* return true if both source port and dest port are isolated */
static inline bool br_skb_isolated(const struct net_bridge_port *to,
				   const struct sk_buff *skb)
{
	return BR_INPUT_SKB_CB(skb)->src_port_isolated &&
	       (to->flags & BR_ISOLATED);
}

/* br_if.c */
void br_port_carrier_check(struct net_bridge_port *p, bool *notified);
int br_add_bridge(struct net *net, const char *name);
int br_del_bridge(struct net *net, const char *name);
int br_add_if(struct net_bridge *br, struct net_device *dev,
	      struct netlink_ext_ack *extack);
int br_del_if(struct net_bridge *br, struct net_device *dev);
void br_mtu_auto_adjust(struct net_bridge *br);
netdev_features_t br_features_recompute(struct net_bridge *br,
					netdev_features_t features);
void br_port_flags_change(struct net_bridge_port *port, unsigned long mask);
void br_manage_promisc(struct net_bridge *br);
int nbp_backup_change(struct net_bridge_port *p, struct net_device *backup_dev);

/* br_input.c */
int br_handle_frame_finish(struct net *net, struct sock *sk, struct sk_buff *skb);
<<<<<<< HEAD
rx_handler_result_t br_handle_frame(struct sk_buff **pskb);

static inline bool br_rx_handler_check_rcu(const struct net_device *dev)
{
	return rcu_dereference(dev->rx_handler) == br_handle_frame;
=======
rx_handler_func_t *br_get_rx_handler(const struct net_device *dev);

struct br_frame_type {
	__be16			type;
	int			(*frame_handler)(struct net_bridge_port *port,
						 struct sk_buff *skb);
	struct hlist_node	list;
};

void br_add_frame(struct net_bridge *br, struct br_frame_type *ft);
void br_del_frame(struct net_bridge *br, struct br_frame_type *ft);

static inline bool br_rx_handler_check_rcu(const struct net_device *dev)
{
	return rcu_dereference(dev->rx_handler) == br_get_rx_handler(dev);
>>>>>>> upstream/android-13
}

static inline bool br_rx_handler_check_rtnl(const struct net_device *dev)
{
<<<<<<< HEAD
	return rcu_dereference_rtnl(dev->rx_handler) == br_handle_frame;
=======
	return rcu_dereference_rtnl(dev->rx_handler) == br_get_rx_handler(dev);
>>>>>>> upstream/android-13
}

static inline struct net_bridge_port *br_port_get_check_rcu(const struct net_device *dev)
{
	return br_rx_handler_check_rcu(dev) ? br_port_get_rcu(dev) : NULL;
}

static inline struct net_bridge_port *
br_port_get_check_rtnl(const struct net_device *dev)
{
	return br_rx_handler_check_rtnl(dev) ? br_port_get_rtnl_rcu(dev) : NULL;
}

/* br_ioctl.c */
<<<<<<< HEAD
int br_dev_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);
int br_ioctl_deviceless_stub(struct net *net, unsigned int cmd,
			     void __user *arg);

/* br_multicast.c */
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
extern unsigned int br_mdb_rehash_seq;
int br_multicast_rcv(struct net_bridge *br, struct net_bridge_port *port,
		     struct sk_buff *skb, u16 vid);
struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge *br,
=======
int br_dev_siocdevprivate(struct net_device *dev, struct ifreq *rq,
			  void __user *data, int cmd);
int br_ioctl_stub(struct net *net, struct net_bridge *br, unsigned int cmd,
		  struct ifreq *ifr, void __user *uarg);

/* br_multicast.c */
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
int br_multicast_rcv(struct net_bridge_mcast **brmctx,
		     struct net_bridge_mcast_port **pmctx,
		     struct net_bridge_vlan *vlan,
		     struct sk_buff *skb, u16 vid);
struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge_mcast *brmctx,
>>>>>>> upstream/android-13
					struct sk_buff *skb, u16 vid);
int br_multicast_add_port(struct net_bridge_port *port);
void br_multicast_del_port(struct net_bridge_port *port);
void br_multicast_enable_port(struct net_bridge_port *port);
void br_multicast_disable_port(struct net_bridge_port *port);
void br_multicast_init(struct net_bridge *br);
<<<<<<< HEAD
void br_multicast_open(struct net_bridge *br);
void br_multicast_stop(struct net_bridge *br);
void br_multicast_dev_del(struct net_bridge *br);
void br_multicast_flood(struct net_bridge_mdb_entry *mdst,
			struct sk_buff *skb, bool local_rcv, bool local_orig);
int br_multicast_set_router(struct net_bridge *br, unsigned long val);
int br_multicast_set_port_router(struct net_bridge_port *p, unsigned long val);
int br_multicast_toggle(struct net_bridge *br, unsigned long val);
int br_multicast_set_querier(struct net_bridge *br, unsigned long val);
int br_multicast_set_hash_max(struct net_bridge *br, unsigned long val);
int br_multicast_set_igmp_version(struct net_bridge *br, unsigned long val);
#if IS_ENABLED(CONFIG_IPV6)
int br_multicast_set_mld_version(struct net_bridge *br, unsigned long val);
#endif
struct net_bridge_mdb_entry *
br_mdb_ip_get(struct net_bridge_mdb_htable *mdb, struct br_ip *dst);
struct net_bridge_mdb_entry *
br_multicast_new_group(struct net_bridge *br, struct net_bridge_port *port,
		       struct br_ip *group);
void br_multicast_free_pg(struct rcu_head *head);
struct net_bridge_port_group *
br_multicast_new_port_group(struct net_bridge_port *port, struct br_ip *group,
			    struct net_bridge_port_group __rcu *next,
			    unsigned char flags, const unsigned char *src);
void br_mdb_init(void);
void br_mdb_uninit(void);
void br_mdb_notify(struct net_device *dev, struct net_bridge_port *port,
		   struct br_ip *group, int type, u8 flags);
void br_rtr_notify(struct net_device *dev, struct net_bridge_port *port,
		   int type);
void br_multicast_count(struct net_bridge *br, const struct net_bridge_port *p,
=======
void br_multicast_join_snoopers(struct net_bridge *br);
void br_multicast_leave_snoopers(struct net_bridge *br);
void br_multicast_open(struct net_bridge *br);
void br_multicast_stop(struct net_bridge *br);
void br_multicast_dev_del(struct net_bridge *br);
void br_multicast_flood(struct net_bridge_mdb_entry *mdst, struct sk_buff *skb,
			struct net_bridge_mcast *brmctx,
			bool local_rcv, bool local_orig);
int br_multicast_set_router(struct net_bridge_mcast *brmctx, unsigned long val);
int br_multicast_set_port_router(struct net_bridge_mcast_port *pmctx,
				 unsigned long val);
int br_multicast_set_vlan_router(struct net_bridge_vlan *v, u8 mcast_router);
int br_multicast_toggle(struct net_bridge *br, unsigned long val,
			struct netlink_ext_ack *extack);
int br_multicast_set_querier(struct net_bridge_mcast *brmctx, unsigned long val);
int br_multicast_set_hash_max(struct net_bridge *br, unsigned long val);
int br_multicast_set_igmp_version(struct net_bridge_mcast *brmctx,
				  unsigned long val);
#if IS_ENABLED(CONFIG_IPV6)
int br_multicast_set_mld_version(struct net_bridge_mcast *brmctx,
				 unsigned long val);
#endif
struct net_bridge_mdb_entry *
br_mdb_ip_get(struct net_bridge *br, struct br_ip *dst);
struct net_bridge_mdb_entry *
br_multicast_new_group(struct net_bridge *br, struct br_ip *group);
struct net_bridge_port_group *
br_multicast_new_port_group(struct net_bridge_port *port, struct br_ip *group,
			    struct net_bridge_port_group __rcu *next,
			    unsigned char flags, const unsigned char *src,
			    u8 filter_mode, u8 rt_protocol);
int br_mdb_hash_init(struct net_bridge *br);
void br_mdb_hash_fini(struct net_bridge *br);
void br_mdb_notify(struct net_device *dev, struct net_bridge_mdb_entry *mp,
		   struct net_bridge_port_group *pg, int type);
void br_rtr_notify(struct net_device *dev, struct net_bridge_mcast_port *pmctx,
		   int type);
void br_multicast_del_pg(struct net_bridge_mdb_entry *mp,
			 struct net_bridge_port_group *pg,
			 struct net_bridge_port_group __rcu **pp);
void br_multicast_count(struct net_bridge *br,
			const struct net_bridge_port *p,
>>>>>>> upstream/android-13
			const struct sk_buff *skb, u8 type, u8 dir);
int br_multicast_init_stats(struct net_bridge *br);
void br_multicast_uninit_stats(struct net_bridge *br);
void br_multicast_get_stats(const struct net_bridge *br,
			    const struct net_bridge_port *p,
			    struct br_mcast_stats *dest);
<<<<<<< HEAD
=======
void br_mdb_init(void);
void br_mdb_uninit(void);
void br_multicast_host_join(const struct net_bridge_mcast *brmctx,
			    struct net_bridge_mdb_entry *mp, bool notify);
void br_multicast_host_leave(struct net_bridge_mdb_entry *mp, bool notify);
void br_multicast_star_g_handle_mode(struct net_bridge_port_group *pg,
				     u8 filter_mode);
void br_multicast_sg_add_exclude_ports(struct net_bridge_mdb_entry *star_mp,
				       struct net_bridge_port_group *sg);
struct net_bridge_group_src *
br_multicast_find_group_src(struct net_bridge_port_group *pg, struct br_ip *ip);
void br_multicast_del_group_src(struct net_bridge_group_src *src,
				bool fastleave);
void br_multicast_ctx_init(struct net_bridge *br,
			   struct net_bridge_vlan *vlan,
			   struct net_bridge_mcast *brmctx);
void br_multicast_ctx_deinit(struct net_bridge_mcast *brmctx);
void br_multicast_port_ctx_init(struct net_bridge_port *port,
				struct net_bridge_vlan *vlan,
				struct net_bridge_mcast_port *pmctx);
void br_multicast_port_ctx_deinit(struct net_bridge_mcast_port *pmctx);
void br_multicast_toggle_one_vlan(struct net_bridge_vlan *vlan, bool on);
int br_multicast_toggle_vlan_snooping(struct net_bridge *br, bool on,
				      struct netlink_ext_ack *extack);
bool br_multicast_toggle_global_vlan(struct net_bridge_vlan *vlan, bool on);

int br_mdb_replay(struct net_device *br_dev, struct net_device *dev,
		  const void *ctx, bool adding, struct notifier_block *nb,
		  struct netlink_ext_ack *extack);
int br_rports_fill_info(struct sk_buff *skb,
			const struct net_bridge_mcast *brmctx);
int br_multicast_dump_querier_state(struct sk_buff *skb,
				    const struct net_bridge_mcast *brmctx,
				    int nest_attr);
size_t br_multicast_querier_state_size(void);
size_t br_rports_size(const struct net_bridge_mcast *brmctx);
void br_multicast_set_query_intvl(struct net_bridge_mcast *brmctx,
				  unsigned long val);
void br_multicast_set_startup_query_intvl(struct net_bridge_mcast *brmctx,
					  unsigned long val);

static inline bool br_group_is_l2(const struct br_ip *group)
{
	return group->proto == 0;
}
>>>>>>> upstream/android-13

#define mlock_dereference(X, br) \
	rcu_dereference_protected(X, lockdep_is_held(&br->multicast_lock))

<<<<<<< HEAD
static inline bool br_multicast_is_router(struct net_bridge *br)
{
	return br->multicast_router == 2 ||
	       (br->multicast_router == 1 &&
		timer_pending(&br->multicast_router_timer));
}

static inline bool
__br_multicast_querier_exists(struct net_bridge *br,
				struct bridge_mcast_other_query *querier,
				const bool is_ipv6)
{
	bool own_querier_enabled;

	if (br->multicast_querier) {
		if (is_ipv6 && !br->has_ipv6_addr)
=======
static inline struct hlist_node *
br_multicast_get_first_rport_node(struct net_bridge_mcast *brmctx,
				  struct sk_buff *skb)
{
#if IS_ENABLED(CONFIG_IPV6)
	if (skb->protocol == htons(ETH_P_IPV6))
		return rcu_dereference(hlist_first_rcu(&brmctx->ip6_mc_router_list));
#endif
	return rcu_dereference(hlist_first_rcu(&brmctx->ip4_mc_router_list));
}

static inline struct net_bridge_port *
br_multicast_rport_from_node_skb(struct hlist_node *rp, struct sk_buff *skb)
{
	struct net_bridge_mcast_port *mctx;

#if IS_ENABLED(CONFIG_IPV6)
	if (skb->protocol == htons(ETH_P_IPV6))
		mctx = hlist_entry_safe(rp, struct net_bridge_mcast_port,
					ip6_rlist);
	else
#endif
		mctx = hlist_entry_safe(rp, struct net_bridge_mcast_port,
					ip4_rlist);

	if (mctx)
		return mctx->port;
	else
		return NULL;
}

static inline bool br_ip4_multicast_is_router(struct net_bridge_mcast *brmctx)
{
	return timer_pending(&brmctx->ip4_mc_router_timer);
}

static inline bool br_ip6_multicast_is_router(struct net_bridge_mcast *brmctx)
{
#if IS_ENABLED(CONFIG_IPV6)
	return timer_pending(&brmctx->ip6_mc_router_timer);
#else
	return false;
#endif
}

static inline bool
br_multicast_is_router(struct net_bridge_mcast *brmctx, struct sk_buff *skb)
{
	switch (brmctx->multicast_router) {
	case MDB_RTR_TYPE_PERM:
		return true;
	case MDB_RTR_TYPE_TEMP_QUERY:
		if (skb) {
			if (skb->protocol == htons(ETH_P_IP))
				return br_ip4_multicast_is_router(brmctx);
			else if (skb->protocol == htons(ETH_P_IPV6))
				return br_ip6_multicast_is_router(brmctx);
		} else {
			return br_ip4_multicast_is_router(brmctx) ||
			       br_ip6_multicast_is_router(brmctx);
		}
		fallthrough;
	default:
		return false;
	}
}

static inline bool
__br_multicast_querier_exists(struct net_bridge_mcast *brmctx,
			      struct bridge_mcast_other_query *querier,
			      const bool is_ipv6)
{
	bool own_querier_enabled;

	if (brmctx->multicast_querier) {
		if (is_ipv6 && !br_opt_get(brmctx->br, BROPT_HAS_IPV6_ADDR))
>>>>>>> upstream/android-13
			own_querier_enabled = false;
		else
			own_querier_enabled = true;
	} else {
		own_querier_enabled = false;
	}

	return time_is_before_jiffies(querier->delay_time) &&
	       (own_querier_enabled || timer_pending(&querier->timer));
}

<<<<<<< HEAD
static inline bool br_multicast_querier_exists(struct net_bridge *br,
					       struct ethhdr *eth)
{
	switch (eth->h_proto) {
	case (htons(ETH_P_IP)):
		return __br_multicast_querier_exists(br,
			&br->ip4_other_query, false);
#if IS_ENABLED(CONFIG_IPV6)
	case (htons(ETH_P_IPV6)):
		return __br_multicast_querier_exists(br,
			&br->ip6_other_query, true);
=======
static inline bool br_multicast_querier_exists(struct net_bridge_mcast *brmctx,
					       struct ethhdr *eth,
					       const struct net_bridge_mdb_entry *mdb)
{
	switch (eth->h_proto) {
	case (htons(ETH_P_IP)):
		return __br_multicast_querier_exists(brmctx,
			&brmctx->ip4_other_query, false);
#if IS_ENABLED(CONFIG_IPV6)
	case (htons(ETH_P_IPV6)):
		return __br_multicast_querier_exists(brmctx,
			&brmctx->ip6_other_query, true);
#endif
	default:
		return !!mdb && br_group_is_l2(&mdb->addr);
	}
}

static inline bool br_multicast_is_star_g(const struct br_ip *ip)
{
	switch (ip->proto) {
	case htons(ETH_P_IP):
		return ipv4_is_zeronet(ip->src.ip4);
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		return ipv6_addr_any(&ip->src.ip6);
#endif
	default:
		return false;
	}
}

static inline bool
br_multicast_should_handle_mode(const struct net_bridge_mcast *brmctx,
				__be16 proto)
{
	switch (proto) {
	case htons(ETH_P_IP):
		return !!(brmctx->multicast_igmp_version == 3);
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		return !!(brmctx->multicast_mld_version == 2);
>>>>>>> upstream/android-13
#endif
	default:
		return false;
	}
}

static inline int br_multicast_igmp_type(const struct sk_buff *skb)
{
	return BR_INPUT_SKB_CB(skb)->igmp;
}
<<<<<<< HEAD
#else
static inline int br_multicast_rcv(struct net_bridge *br,
				   struct net_bridge_port *port,
=======

static inline unsigned long br_multicast_lmqt(const struct net_bridge_mcast *brmctx)
{
	return brmctx->multicast_last_member_interval *
	       brmctx->multicast_last_member_count;
}

static inline unsigned long br_multicast_gmi(const struct net_bridge_mcast *brmctx)
{
	return brmctx->multicast_membership_interval;
}

static inline bool
br_multicast_ctx_is_vlan(const struct net_bridge_mcast *brmctx)
{
	return !!brmctx->vlan;
}

static inline bool
br_multicast_port_ctx_is_vlan(const struct net_bridge_mcast_port *pmctx)
{
	return !!pmctx->vlan;
}

static inline struct net_bridge_mcast *
br_multicast_port_ctx_get_global(const struct net_bridge_mcast_port *pmctx)
{
	if (!br_multicast_port_ctx_is_vlan(pmctx))
		return &pmctx->port->br->multicast_ctx;
	else
		return &pmctx->vlan->brvlan->br_mcast_ctx;
}

static inline bool
br_multicast_ctx_vlan_global_disabled(const struct net_bridge_mcast *brmctx)
{
	return br_multicast_ctx_is_vlan(brmctx) &&
	       (!br_opt_get(brmctx->br, BROPT_MCAST_VLAN_SNOOPING_ENABLED) ||
		!(brmctx->vlan->priv_flags & BR_VLFLAG_GLOBAL_MCAST_ENABLED));
}

static inline bool
br_multicast_ctx_vlan_disabled(const struct net_bridge_mcast *brmctx)
{
	return br_multicast_ctx_is_vlan(brmctx) &&
	       !(brmctx->vlan->priv_flags & BR_VLFLAG_MCAST_ENABLED);
}

static inline bool
br_multicast_port_ctx_vlan_disabled(const struct net_bridge_mcast_port *pmctx)
{
	return br_multicast_port_ctx_is_vlan(pmctx) &&
	       !(pmctx->vlan->priv_flags & BR_VLFLAG_MCAST_ENABLED);
}

static inline bool
br_multicast_port_ctx_state_disabled(const struct net_bridge_mcast_port *pmctx)
{
	return pmctx->port->state == BR_STATE_DISABLED ||
	       (br_multicast_port_ctx_is_vlan(pmctx) &&
		(br_multicast_port_ctx_vlan_disabled(pmctx) ||
		 pmctx->vlan->state == BR_STATE_DISABLED));
}

static inline bool
br_multicast_port_ctx_state_stopped(const struct net_bridge_mcast_port *pmctx)
{
	return br_multicast_port_ctx_state_disabled(pmctx) ||
	       pmctx->port->state == BR_STATE_BLOCKING ||
	       (br_multicast_port_ctx_is_vlan(pmctx) &&
		pmctx->vlan->state == BR_STATE_BLOCKING);
}

static inline bool
br_rports_have_mc_router(const struct net_bridge_mcast *brmctx)
{
#if IS_ENABLED(CONFIG_IPV6)
	return !hlist_empty(&brmctx->ip4_mc_router_list) ||
	       !hlist_empty(&brmctx->ip6_mc_router_list);
#else
	return !hlist_empty(&brmctx->ip4_mc_router_list);
#endif
}

static inline bool
br_multicast_ctx_options_equal(const struct net_bridge_mcast *brmctx1,
			       const struct net_bridge_mcast *brmctx2)
{
	return brmctx1->multicast_igmp_version ==
	       brmctx2->multicast_igmp_version &&
	       brmctx1->multicast_last_member_count ==
	       brmctx2->multicast_last_member_count &&
	       brmctx1->multicast_startup_query_count ==
	       brmctx2->multicast_startup_query_count &&
	       brmctx1->multicast_last_member_interval ==
	       brmctx2->multicast_last_member_interval &&
	       brmctx1->multicast_membership_interval ==
	       brmctx2->multicast_membership_interval &&
	       brmctx1->multicast_querier_interval ==
	       brmctx2->multicast_querier_interval &&
	       brmctx1->multicast_query_interval ==
	       brmctx2->multicast_query_interval &&
	       brmctx1->multicast_query_response_interval ==
	       brmctx2->multicast_query_response_interval &&
	       brmctx1->multicast_startup_query_interval ==
	       brmctx2->multicast_startup_query_interval &&
	       brmctx1->multicast_querier == brmctx2->multicast_querier &&
	       brmctx1->multicast_router == brmctx2->multicast_router &&
	       !br_rports_have_mc_router(brmctx1) &&
	       !br_rports_have_mc_router(brmctx2) &&
#if IS_ENABLED(CONFIG_IPV6)
	       brmctx1->multicast_mld_version ==
	       brmctx2->multicast_mld_version &&
#endif
	       true;
}

static inline bool
br_multicast_ctx_matches_vlan_snooping(const struct net_bridge_mcast *brmctx)
{
	bool vlan_snooping_enabled;

	vlan_snooping_enabled = !!br_opt_get(brmctx->br,
					     BROPT_MCAST_VLAN_SNOOPING_ENABLED);

	return !!(vlan_snooping_enabled == br_multicast_ctx_is_vlan(brmctx));
}
#else
static inline int br_multicast_rcv(struct net_bridge_mcast **brmctx,
				   struct net_bridge_mcast_port **pmctx,
				   struct net_bridge_vlan *vlan,
>>>>>>> upstream/android-13
				   struct sk_buff *skb,
				   u16 vid)
{
	return 0;
}

<<<<<<< HEAD
static inline struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge *br,
=======
static inline struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge_mcast *brmctx,
>>>>>>> upstream/android-13
						      struct sk_buff *skb, u16 vid)
{
	return NULL;
}

static inline int br_multicast_add_port(struct net_bridge_port *port)
{
	return 0;
}

static inline void br_multicast_del_port(struct net_bridge_port *port)
{
}

static inline void br_multicast_enable_port(struct net_bridge_port *port)
{
}

static inline void br_multicast_disable_port(struct net_bridge_port *port)
{
}

static inline void br_multicast_init(struct net_bridge *br)
{
}

<<<<<<< HEAD
=======
static inline void br_multicast_join_snoopers(struct net_bridge *br)
{
}

static inline void br_multicast_leave_snoopers(struct net_bridge *br)
{
}

>>>>>>> upstream/android-13
static inline void br_multicast_open(struct net_bridge *br)
{
}

static inline void br_multicast_stop(struct net_bridge *br)
{
}

static inline void br_multicast_dev_del(struct net_bridge *br)
{
}

static inline void br_multicast_flood(struct net_bridge_mdb_entry *mdst,
				      struct sk_buff *skb,
<<<<<<< HEAD
=======
				      struct net_bridge_mcast *brmctx,
>>>>>>> upstream/android-13
				      bool local_rcv, bool local_orig)
{
}

<<<<<<< HEAD
static inline bool br_multicast_is_router(struct net_bridge *br)
=======
static inline bool br_multicast_is_router(struct net_bridge_mcast *brmctx,
					  struct sk_buff *skb)
>>>>>>> upstream/android-13
{
	return false;
}

<<<<<<< HEAD
static inline bool br_multicast_querier_exists(struct net_bridge *br,
					       struct ethhdr *eth)
=======
static inline bool br_multicast_querier_exists(struct net_bridge_mcast *brmctx,
					       struct ethhdr *eth,
					       const struct net_bridge_mdb_entry *mdb)
>>>>>>> upstream/android-13
{
	return false;
}

static inline void br_mdb_init(void)
{
}

static inline void br_mdb_uninit(void)
{
}

<<<<<<< HEAD
=======
static inline int br_mdb_hash_init(struct net_bridge *br)
{
	return 0;
}

static inline void br_mdb_hash_fini(struct net_bridge *br)
{
}

>>>>>>> upstream/android-13
static inline void br_multicast_count(struct net_bridge *br,
				      const struct net_bridge_port *p,
				      const struct sk_buff *skb,
				      u8 type, u8 dir)
{
}

static inline int br_multicast_init_stats(struct net_bridge *br)
{
	return 0;
}

static inline void br_multicast_uninit_stats(struct net_bridge *br)
{
}

static inline int br_multicast_igmp_type(const struct sk_buff *skb)
{
	return 0;
}
<<<<<<< HEAD
=======

static inline void br_multicast_ctx_init(struct net_bridge *br,
					 struct net_bridge_vlan *vlan,
					 struct net_bridge_mcast *brmctx)
{
}

static inline void br_multicast_ctx_deinit(struct net_bridge_mcast *brmctx)
{
}

static inline void br_multicast_port_ctx_init(struct net_bridge_port *port,
					      struct net_bridge_vlan *vlan,
					      struct net_bridge_mcast_port *pmctx)
{
}

static inline void br_multicast_port_ctx_deinit(struct net_bridge_mcast_port *pmctx)
{
}

static inline void br_multicast_toggle_one_vlan(struct net_bridge_vlan *vlan,
						bool on)
{
}

static inline int br_multicast_toggle_vlan_snooping(struct net_bridge *br,
						    bool on,
						    struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}

static inline bool br_multicast_toggle_global_vlan(struct net_bridge_vlan *vlan,
						   bool on)
{
	return false;
}

static inline int br_mdb_replay(struct net_device *br_dev,
				struct net_device *dev, const void *ctx,
				bool adding, struct notifier_block *nb,
				struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}

static inline bool
br_multicast_ctx_options_equal(const struct net_bridge_mcast *brmctx1,
			       const struct net_bridge_mcast *brmctx2)
{
	return true;
}
>>>>>>> upstream/android-13
#endif

/* br_vlan.c */
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
bool br_allowed_ingress(const struct net_bridge *br,
			struct net_bridge_vlan_group *vg, struct sk_buff *skb,
<<<<<<< HEAD
			u16 *vid);
=======
			u16 *vid, u8 *state,
			struct net_bridge_vlan **vlan);
>>>>>>> upstream/android-13
bool br_allowed_egress(struct net_bridge_vlan_group *vg,
		       const struct sk_buff *skb);
bool br_should_learn(struct net_bridge_port *p, struct sk_buff *skb, u16 *vid);
struct sk_buff *br_handle_vlan(struct net_bridge *br,
			       const struct net_bridge_port *port,
			       struct net_bridge_vlan_group *vg,
			       struct sk_buff *skb);
int br_vlan_add(struct net_bridge *br, u16 vid, u16 flags,
<<<<<<< HEAD
		bool *changed);
=======
		bool *changed, struct netlink_ext_ack *extack);
>>>>>>> upstream/android-13
int br_vlan_delete(struct net_bridge *br, u16 vid);
void br_vlan_flush(struct net_bridge *br);
struct net_bridge_vlan *br_vlan_find(struct net_bridge_vlan_group *vg, u16 vid);
void br_recalculate_fwd_mask(struct net_bridge *br);
<<<<<<< HEAD
int __br_vlan_filter_toggle(struct net_bridge *br, unsigned long val);
int br_vlan_filter_toggle(struct net_bridge *br, unsigned long val);
int __br_vlan_set_proto(struct net_bridge *br, __be16 proto);
int br_vlan_set_proto(struct net_bridge *br, unsigned long val);
int br_vlan_set_stats(struct net_bridge *br, unsigned long val);
int br_vlan_init(struct net_bridge *br);
int br_vlan_set_default_pvid(struct net_bridge *br, unsigned long val);
int __br_vlan_set_default_pvid(struct net_bridge *br, u16 pvid);
int nbp_vlan_add(struct net_bridge_port *port, u16 vid, u16 flags,
		 bool *changed);
int nbp_vlan_delete(struct net_bridge_port *port, u16 vid);
void nbp_vlan_flush(struct net_bridge_port *port);
int nbp_vlan_init(struct net_bridge_port *port);
int nbp_get_num_vlan_infos(struct net_bridge_port *p, u32 filter_mask);
void br_vlan_get_stats(const struct net_bridge_vlan *v,
		       struct br_vlan_stats *stats);
=======
int br_vlan_filter_toggle(struct net_bridge *br, unsigned long val,
			  struct netlink_ext_ack *extack);
int __br_vlan_set_proto(struct net_bridge *br, __be16 proto,
			struct netlink_ext_ack *extack);
int br_vlan_set_proto(struct net_bridge *br, unsigned long val,
		      struct netlink_ext_ack *extack);
int br_vlan_set_stats(struct net_bridge *br, unsigned long val);
int br_vlan_set_stats_per_port(struct net_bridge *br, unsigned long val);
int br_vlan_init(struct net_bridge *br);
int br_vlan_set_default_pvid(struct net_bridge *br, unsigned long val,
			     struct netlink_ext_ack *extack);
int __br_vlan_set_default_pvid(struct net_bridge *br, u16 pvid,
			       struct netlink_ext_ack *extack);
int nbp_vlan_add(struct net_bridge_port *port, u16 vid, u16 flags,
		 bool *changed, struct netlink_ext_ack *extack);
int nbp_vlan_delete(struct net_bridge_port *port, u16 vid);
void nbp_vlan_flush(struct net_bridge_port *port);
int nbp_vlan_init(struct net_bridge_port *port, struct netlink_ext_ack *extack);
int nbp_get_num_vlan_infos(struct net_bridge_port *p, u32 filter_mask);
void br_vlan_get_stats(const struct net_bridge_vlan *v,
		       struct pcpu_sw_netstats *stats);
void br_vlan_port_event(struct net_bridge_port *p, unsigned long event);
int br_vlan_bridge_event(struct net_device *dev, unsigned long event,
			 void *ptr);
void br_vlan_rtnl_init(void);
void br_vlan_rtnl_uninit(void);
void br_vlan_notify(const struct net_bridge *br,
		    const struct net_bridge_port *p,
		    u16 vid, u16 vid_range,
		    int cmd);
int br_vlan_replay(struct net_device *br_dev, struct net_device *dev,
		   const void *ctx, bool adding, struct notifier_block *nb,
		   struct netlink_ext_ack *extack);
bool br_vlan_can_enter_range(const struct net_bridge_vlan *v_curr,
			     const struct net_bridge_vlan *range_end);

void br_vlan_fill_forward_path_pvid(struct net_bridge *br,
				    struct net_device_path_ctx *ctx,
				    struct net_device_path *path);
int br_vlan_fill_forward_path_mode(struct net_bridge *br,
				   struct net_bridge_port *dst,
				   struct net_device_path *path);
>>>>>>> upstream/android-13

static inline struct net_bridge_vlan_group *br_vlan_group(
					const struct net_bridge *br)
{
	return rtnl_dereference(br->vlgrp);
}

static inline struct net_bridge_vlan_group *nbp_vlan_group(
					const struct net_bridge_port *p)
{
	return rtnl_dereference(p->vlgrp);
}

static inline struct net_bridge_vlan_group *br_vlan_group_rcu(
					const struct net_bridge *br)
{
	return rcu_dereference(br->vlgrp);
}

static inline struct net_bridge_vlan_group *nbp_vlan_group_rcu(
					const struct net_bridge_port *p)
{
	return rcu_dereference(p->vlgrp);
}

/* Since bridge now depends on 8021Q module, but the time bridge sees the
 * skb, the vlan tag will always be present if the frame was tagged.
 */
static inline int br_vlan_get_tag(const struct sk_buff *skb, u16 *vid)
{
	int err = 0;

	if (skb_vlan_tag_present(skb)) {
<<<<<<< HEAD
		*vid = skb_vlan_tag_get(skb) & VLAN_VID_MASK;
=======
		*vid = skb_vlan_tag_get_id(skb);
>>>>>>> upstream/android-13
	} else {
		*vid = 0;
		err = -EINVAL;
	}

	return err;
}

static inline u16 br_get_pvid(const struct net_bridge_vlan_group *vg)
{
	if (!vg)
		return 0;

	smp_rmb();
	return vg->pvid;
}

<<<<<<< HEAD
=======
static inline u16 br_vlan_flags(const struct net_bridge_vlan *v, u16 pvid)
{
	return v->vid == pvid ? v->flags | BRIDGE_VLAN_INFO_PVID : v->flags;
}
>>>>>>> upstream/android-13
#else
static inline bool br_allowed_ingress(const struct net_bridge *br,
				      struct net_bridge_vlan_group *vg,
				      struct sk_buff *skb,
<<<<<<< HEAD
				      u16 *vid)
{
=======
				      u16 *vid, u8 *state,
				      struct net_bridge_vlan **vlan)

{
	*vlan = NULL;
>>>>>>> upstream/android-13
	return true;
}

static inline bool br_allowed_egress(struct net_bridge_vlan_group *vg,
				     const struct sk_buff *skb)
{
	return true;
}

static inline bool br_should_learn(struct net_bridge_port *p,
				   struct sk_buff *skb, u16 *vid)
{
	return true;
}

static inline struct sk_buff *br_handle_vlan(struct net_bridge *br,
					     const struct net_bridge_port *port,
					     struct net_bridge_vlan_group *vg,
					     struct sk_buff *skb)
{
	return skb;
}

static inline int br_vlan_add(struct net_bridge *br, u16 vid, u16 flags,
<<<<<<< HEAD
			      bool *changed)
=======
			      bool *changed, struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	*changed = false;
	return -EOPNOTSUPP;
}

static inline int br_vlan_delete(struct net_bridge *br, u16 vid)
{
	return -EOPNOTSUPP;
}

static inline void br_vlan_flush(struct net_bridge *br)
{
}

static inline void br_recalculate_fwd_mask(struct net_bridge *br)
{
}

static inline int br_vlan_init(struct net_bridge *br)
{
	return 0;
}

static inline int nbp_vlan_add(struct net_bridge_port *port, u16 vid, u16 flags,
<<<<<<< HEAD
			       bool *changed)
=======
			       bool *changed, struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	*changed = false;
	return -EOPNOTSUPP;
}

static inline int nbp_vlan_delete(struct net_bridge_port *port, u16 vid)
{
	return -EOPNOTSUPP;
}

static inline void nbp_vlan_flush(struct net_bridge_port *port)
{
}

static inline struct net_bridge_vlan *br_vlan_find(struct net_bridge_vlan_group *vg,
						   u16 vid)
{
	return NULL;
}

<<<<<<< HEAD
static inline int nbp_vlan_init(struct net_bridge_port *port)
=======
static inline int nbp_vlan_init(struct net_bridge_port *port,
				struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	return 0;
}

static inline u16 br_vlan_get_tag(const struct sk_buff *skb, u16 *tag)
{
	return 0;
}

static inline u16 br_get_pvid(const struct net_bridge_vlan_group *vg)
{
	return 0;
}

<<<<<<< HEAD
static inline int __br_vlan_filter_toggle(struct net_bridge *br,
					  unsigned long val)
=======
static inline int br_vlan_filter_toggle(struct net_bridge *br,
					unsigned long val,
					struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	return -EOPNOTSUPP;
}

static inline int nbp_get_num_vlan_infos(struct net_bridge_port *p,
					 u32 filter_mask)
{
	return 0;
}

<<<<<<< HEAD
=======
static inline void br_vlan_fill_forward_path_pvid(struct net_bridge *br,
						  struct net_device_path_ctx *ctx,
						  struct net_device_path *path)
{
}

static inline int br_vlan_fill_forward_path_mode(struct net_bridge *br,
						 struct net_bridge_port *dst,
						 struct net_device_path *path)
{
	return 0;
}

>>>>>>> upstream/android-13
static inline struct net_bridge_vlan_group *br_vlan_group(
					const struct net_bridge *br)
{
	return NULL;
}

static inline struct net_bridge_vlan_group *nbp_vlan_group(
					const struct net_bridge_port *p)
{
	return NULL;
}

static inline struct net_bridge_vlan_group *br_vlan_group_rcu(
					const struct net_bridge *br)
{
	return NULL;
}

static inline struct net_bridge_vlan_group *nbp_vlan_group_rcu(
					const struct net_bridge_port *p)
{
	return NULL;
}

static inline void br_vlan_get_stats(const struct net_bridge_vlan *v,
<<<<<<< HEAD
				     struct br_vlan_stats *stats)
{
}
=======
				     struct pcpu_sw_netstats *stats)
{
}

static inline void br_vlan_port_event(struct net_bridge_port *p,
				      unsigned long event)
{
}

static inline int br_vlan_bridge_event(struct net_device *dev,
				       unsigned long event, void *ptr)
{
	return 0;
}

static inline void br_vlan_rtnl_init(void)
{
}

static inline void br_vlan_rtnl_uninit(void)
{
}

static inline void br_vlan_notify(const struct net_bridge *br,
				  const struct net_bridge_port *p,
				  u16 vid, u16 vid_range,
				  int cmd)
{
}

static inline bool br_vlan_can_enter_range(const struct net_bridge_vlan *v_curr,
					   const struct net_bridge_vlan *range_end)
{
	return true;
}

static inline int br_vlan_replay(struct net_device *br_dev,
				 struct net_device *dev, const void *ctx,
				 bool adding, struct notifier_block *nb,
				 struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}
#endif

/* br_vlan_options.c */
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
bool br_vlan_opts_eq_range(const struct net_bridge_vlan *v_curr,
			   const struct net_bridge_vlan *range_end);
bool br_vlan_opts_fill(struct sk_buff *skb, const struct net_bridge_vlan *v);
size_t br_vlan_opts_nl_size(void);
int br_vlan_process_options(const struct net_bridge *br,
			    const struct net_bridge_port *p,
			    struct net_bridge_vlan *range_start,
			    struct net_bridge_vlan *range_end,
			    struct nlattr **tb,
			    struct netlink_ext_ack *extack);
int br_vlan_rtm_process_global_options(struct net_device *dev,
				       const struct nlattr *attr,
				       int cmd,
				       struct netlink_ext_ack *extack);
bool br_vlan_global_opts_can_enter_range(const struct net_bridge_vlan *v_curr,
					 const struct net_bridge_vlan *r_end);
bool br_vlan_global_opts_fill(struct sk_buff *skb, u16 vid, u16 vid_range,
			      const struct net_bridge_vlan *v_opts);

/* vlan state manipulation helpers using *_ONCE to annotate lock-free access */
static inline u8 br_vlan_get_state(const struct net_bridge_vlan *v)
{
	return READ_ONCE(v->state);
}

static inline void br_vlan_set_state(struct net_bridge_vlan *v, u8 state)
{
	WRITE_ONCE(v->state, state);
}

static inline u8 br_vlan_get_pvid_state(const struct net_bridge_vlan_group *vg)
{
	return READ_ONCE(vg->pvid_state);
}

static inline void br_vlan_set_pvid_state(struct net_bridge_vlan_group *vg,
					  u8 state)
{
	WRITE_ONCE(vg->pvid_state, state);
}

/* learn_allow is true at ingress and false at egress */
static inline bool br_vlan_state_allowed(u8 state, bool learn_allow)
{
	switch (state) {
	case BR_STATE_LEARNING:
		return learn_allow;
	case BR_STATE_FORWARDING:
		return true;
	default:
		return false;
	}
}
>>>>>>> upstream/android-13
#endif

struct nf_br_ops {
	int (*br_dev_xmit_hook)(struct sk_buff *skb);
};
extern const struct nf_br_ops __rcu *nf_br_ops;

/* br_netfilter.c */
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
int br_nf_core_init(void);
void br_nf_core_fini(void);
void br_netfilter_rtable_init(struct net_bridge *);
#else
static inline int br_nf_core_init(void) { return 0; }
static inline void br_nf_core_fini(void) {}
#define br_netfilter_rtable_init(x)
#endif

/* br_stp.c */
void br_set_state(struct net_bridge_port *p, unsigned int state);
struct net_bridge_port *br_get_port(struct net_bridge *br, u16 port_no);
void br_init_port(struct net_bridge_port *p);
void br_become_designated_port(struct net_bridge_port *p);

void __br_set_forward_delay(struct net_bridge *br, unsigned long t);
int br_set_forward_delay(struct net_bridge *br, unsigned long x);
int br_set_hello_time(struct net_bridge *br, unsigned long x);
int br_set_max_age(struct net_bridge *br, unsigned long x);
int __set_ageing_time(struct net_device *dev, unsigned long t);
int br_set_ageing_time(struct net_bridge *br, clock_t ageing_time);


/* br_stp_if.c */
void br_stp_enable_bridge(struct net_bridge *br);
void br_stp_disable_bridge(struct net_bridge *br);
<<<<<<< HEAD
void br_stp_set_enabled(struct net_bridge *br, unsigned long val);
=======
int br_stp_set_enabled(struct net_bridge *br, unsigned long val,
		       struct netlink_ext_ack *extack);
>>>>>>> upstream/android-13
void br_stp_enable_port(struct net_bridge_port *p);
void br_stp_disable_port(struct net_bridge_port *p);
bool br_stp_recalculate_bridge_id(struct net_bridge *br);
void br_stp_change_bridge_id(struct net_bridge *br, const unsigned char *a);
void br_stp_set_bridge_priority(struct net_bridge *br, u16 newprio);
int br_stp_set_port_priority(struct net_bridge_port *p, unsigned long newprio);
int br_stp_set_path_cost(struct net_bridge_port *p, unsigned long path_cost);
ssize_t br_show_bridge_id(char *buf, const struct bridge_id *id);

/* br_stp_bpdu.c */
struct stp_proto;
void br_stp_rcv(const struct stp_proto *proto, struct sk_buff *skb,
		struct net_device *dev);

/* br_stp_timer.c */
void br_stp_timer_init(struct net_bridge *br);
void br_stp_port_timer_init(struct net_bridge_port *p);
unsigned long br_timer_value(const struct timer_list *timer);

/* br.c */
#if IS_ENABLED(CONFIG_ATM_LANE)
extern int (*br_fdb_test_addr_hook)(struct net_device *dev, unsigned char *addr);
#endif

<<<<<<< HEAD
=======
/* br_mrp.c */
#if IS_ENABLED(CONFIG_BRIDGE_MRP)
int br_mrp_parse(struct net_bridge *br, struct net_bridge_port *p,
		 struct nlattr *attr, int cmd, struct netlink_ext_ack *extack);
bool br_mrp_enabled(struct net_bridge *br);
void br_mrp_port_del(struct net_bridge *br, struct net_bridge_port *p);
int br_mrp_fill_info(struct sk_buff *skb, struct net_bridge *br);
#else
static inline int br_mrp_parse(struct net_bridge *br, struct net_bridge_port *p,
			       struct nlattr *attr, int cmd,
			       struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}

static inline bool br_mrp_enabled(struct net_bridge *br)
{
	return false;
}

static inline void br_mrp_port_del(struct net_bridge *br,
				   struct net_bridge_port *p)
{
}

static inline int br_mrp_fill_info(struct sk_buff *skb, struct net_bridge *br)
{
	return 0;
}

#endif

/* br_cfm.c */
#if IS_ENABLED(CONFIG_BRIDGE_CFM)
int br_cfm_parse(struct net_bridge *br, struct net_bridge_port *p,
		 struct nlattr *attr, int cmd, struct netlink_ext_ack *extack);
bool br_cfm_created(struct net_bridge *br);
void br_cfm_port_del(struct net_bridge *br, struct net_bridge_port *p);
int br_cfm_config_fill_info(struct sk_buff *skb, struct net_bridge *br);
int br_cfm_status_fill_info(struct sk_buff *skb,
			    struct net_bridge *br,
			    bool getlink);
int br_cfm_mep_count(struct net_bridge *br, u32 *count);
int br_cfm_peer_mep_count(struct net_bridge *br, u32 *count);
#else
static inline int br_cfm_parse(struct net_bridge *br, struct net_bridge_port *p,
			       struct nlattr *attr, int cmd,
			       struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}

static inline bool br_cfm_created(struct net_bridge *br)
{
	return false;
}

static inline void br_cfm_port_del(struct net_bridge *br,
				   struct net_bridge_port *p)
{
}

static inline int br_cfm_config_fill_info(struct sk_buff *skb, struct net_bridge *br)
{
	return -EOPNOTSUPP;
}

static inline int br_cfm_status_fill_info(struct sk_buff *skb,
					  struct net_bridge *br,
					  bool getlink)
{
	return -EOPNOTSUPP;
}

static inline int br_cfm_mep_count(struct net_bridge *br, u32 *count)
{
	*count = 0;
	return -EOPNOTSUPP;
}

static inline int br_cfm_peer_mep_count(struct net_bridge *br, u32 *count)
{
	*count = 0;
	return -EOPNOTSUPP;
}
#endif

>>>>>>> upstream/android-13
/* br_netlink.c */
extern struct rtnl_link_ops br_link_ops;
int br_netlink_init(void);
void br_netlink_fini(void);
void br_ifinfo_notify(int event, const struct net_bridge *br,
		      const struct net_bridge_port *port);
<<<<<<< HEAD
int br_setlink(struct net_device *dev, struct nlmsghdr *nlmsg, u16 flags);
int br_dellink(struct net_device *dev, struct nlmsghdr *nlmsg, u16 flags);
int br_getlink(struct sk_buff *skb, u32 pid, u32 seq, struct net_device *dev,
	       u32 filter_mask, int nlflags);
=======
void br_info_notify(int event, const struct net_bridge *br,
		    const struct net_bridge_port *port, u32 filter);
int br_setlink(struct net_device *dev, struct nlmsghdr *nlmsg, u16 flags,
	       struct netlink_ext_ack *extack);
int br_dellink(struct net_device *dev, struct nlmsghdr *nlmsg, u16 flags);
int br_getlink(struct sk_buff *skb, u32 pid, u32 seq, struct net_device *dev,
	       u32 filter_mask, int nlflags);
int br_process_vlan_info(struct net_bridge *br,
			 struct net_bridge_port *p, int cmd,
			 struct bridge_vlan_info *vinfo_curr,
			 struct bridge_vlan_info **vinfo_last,
			 bool *changed,
			 struct netlink_ext_ack *extack);
>>>>>>> upstream/android-13

#ifdef CONFIG_SYSFS
/* br_sysfs_if.c */
extern const struct sysfs_ops brport_sysfs_ops;
int br_sysfs_addif(struct net_bridge_port *p);
int br_sysfs_renameif(struct net_bridge_port *p);

/* br_sysfs_br.c */
int br_sysfs_addbr(struct net_device *dev);
void br_sysfs_delbr(struct net_device *dev);

#else

static inline int br_sysfs_addif(struct net_bridge_port *p) { return 0; }
static inline int br_sysfs_renameif(struct net_bridge_port *p) { return 0; }
static inline int br_sysfs_addbr(struct net_device *dev) { return 0; }
static inline void br_sysfs_delbr(struct net_device *dev) { return; }
#endif /* CONFIG_SYSFS */

/* br_switchdev.c */
#ifdef CONFIG_NET_SWITCHDEV
<<<<<<< HEAD
int nbp_switchdev_mark_set(struct net_bridge_port *p);
=======
int br_switchdev_port_offload(struct net_bridge_port *p,
			      struct net_device *dev, const void *ctx,
			      struct notifier_block *atomic_nb,
			      struct notifier_block *blocking_nb,
			      bool tx_fwd_offload,
			      struct netlink_ext_ack *extack);

void br_switchdev_port_unoffload(struct net_bridge_port *p, const void *ctx,
				 struct notifier_block *atomic_nb,
				 struct notifier_block *blocking_nb);

bool br_switchdev_frame_uses_tx_fwd_offload(struct sk_buff *skb);

void br_switchdev_frame_set_offload_fwd_mark(struct sk_buff *skb);

void nbp_switchdev_frame_mark_tx_fwd_offload(const struct net_bridge_port *p,
					     struct sk_buff *skb);
void nbp_switchdev_frame_mark_tx_fwd_to_hwdom(const struct net_bridge_port *p,
					      struct sk_buff *skb);
>>>>>>> upstream/android-13
void nbp_switchdev_frame_mark(const struct net_bridge_port *p,
			      struct sk_buff *skb);
bool nbp_switchdev_allowed_egress(const struct net_bridge_port *p,
				  const struct sk_buff *skb);
int br_switchdev_set_port_flag(struct net_bridge_port *p,
			       unsigned long flags,
<<<<<<< HEAD
			       unsigned long mask);
void br_switchdev_fdb_notify(const struct net_bridge_fdb_entry *fdb,
			     int type);
int br_switchdev_port_vlan_add(struct net_device *dev, u16 vid, u16 flags);
int br_switchdev_port_vlan_del(struct net_device *dev, u16 vid);
=======
			       unsigned long mask,
			       struct netlink_ext_ack *extack);
void br_switchdev_fdb_notify(struct net_bridge *br,
			     const struct net_bridge_fdb_entry *fdb, int type);
int br_switchdev_port_vlan_add(struct net_device *dev, u16 vid, u16 flags,
			       struct netlink_ext_ack *extack);
int br_switchdev_port_vlan_del(struct net_device *dev, u16 vid);
void br_switchdev_init(struct net_bridge *br);
>>>>>>> upstream/android-13

static inline void br_switchdev_frame_unmark(struct sk_buff *skb)
{
	skb->offload_fwd_mark = 0;
}
#else
<<<<<<< HEAD
static inline int nbp_switchdev_mark_set(struct net_bridge_port *p)
{
	return 0;
=======
static inline int
br_switchdev_port_offload(struct net_bridge_port *p,
			  struct net_device *dev, const void *ctx,
			  struct notifier_block *atomic_nb,
			  struct notifier_block *blocking_nb,
			  bool tx_fwd_offload,
			  struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}

static inline void
br_switchdev_port_unoffload(struct net_bridge_port *p, const void *ctx,
			    struct notifier_block *atomic_nb,
			    struct notifier_block *blocking_nb)
{
}

static inline bool br_switchdev_frame_uses_tx_fwd_offload(struct sk_buff *skb)
{
	return false;
}

static inline void br_switchdev_frame_set_offload_fwd_mark(struct sk_buff *skb)
{
}

static inline void
nbp_switchdev_frame_mark_tx_fwd_offload(const struct net_bridge_port *p,
					struct sk_buff *skb)
{
}

static inline void
nbp_switchdev_frame_mark_tx_fwd_to_hwdom(const struct net_bridge_port *p,
					 struct sk_buff *skb)
{
>>>>>>> upstream/android-13
}

static inline void nbp_switchdev_frame_mark(const struct net_bridge_port *p,
					    struct sk_buff *skb)
{
}

static inline bool nbp_switchdev_allowed_egress(const struct net_bridge_port *p,
						const struct sk_buff *skb)
{
	return true;
}

static inline int br_switchdev_set_port_flag(struct net_bridge_port *p,
					     unsigned long flags,
<<<<<<< HEAD
					     unsigned long mask)
=======
					     unsigned long mask,
					     struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	return 0;
}

static inline int br_switchdev_port_vlan_add(struct net_device *dev,
<<<<<<< HEAD
					     u16 vid, u16 flags)
=======
					     u16 vid, u16 flags,
					     struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	return -EOPNOTSUPP;
}

static inline int br_switchdev_port_vlan_del(struct net_device *dev, u16 vid)
{
	return -EOPNOTSUPP;
}

static inline void
<<<<<<< HEAD
br_switchdev_fdb_notify(const struct net_bridge_fdb_entry *fdb, int type)
=======
br_switchdev_fdb_notify(struct net_bridge *br,
			const struct net_bridge_fdb_entry *fdb, int type)
>>>>>>> upstream/android-13
{
}

static inline void br_switchdev_frame_unmark(struct sk_buff *skb)
{
}
<<<<<<< HEAD
=======

static inline void br_switchdev_init(struct net_bridge *br)
{
}

>>>>>>> upstream/android-13
#endif /* CONFIG_NET_SWITCHDEV */

/* br_arp_nd_proxy.c */
void br_recalculate_neigh_suppress_enabled(struct net_bridge *br);
void br_do_proxy_suppress_arp(struct sk_buff *skb, struct net_bridge *br,
			      u16 vid, struct net_bridge_port *p);
void br_do_suppress_nd(struct sk_buff *skb, struct net_bridge *br,
		       u16 vid, struct net_bridge_port *p, struct nd_msg *msg);
struct nd_msg *br_is_nd_neigh_msg(struct sk_buff *skb, struct nd_msg *m);
#endif
