/* SPDX-License-Identifier: GPL-2.0 */
/*
 * ipv6 in net namespaces
 */

#include <net/inet_frag.h>

#ifndef __NETNS_IPV6_H__
#define __NETNS_IPV6_H__
<<<<<<< HEAD
#include <net/dst_ops.h>
=======
#include <linux/android_kabi.h>
#include <net/dst_ops.h>
#include <uapi/linux/icmpv6.h>
>>>>>>> upstream/android-13

struct ctl_table_header;

struct netns_sysctl_ipv6 {
#ifdef CONFIG_SYSCTL
	struct ctl_table_header *hdr;
	struct ctl_table_header *route_hdr;
	struct ctl_table_header *icmp_hdr;
	struct ctl_table_header *frags_hdr;
	struct ctl_table_header *xfrm6_hdr;
#endif
<<<<<<< HEAD
	int bindv6only;
=======
>>>>>>> upstream/android-13
	int flush_delay;
	int ip6_rt_max_size;
	int ip6_rt_gc_min_interval;
	int ip6_rt_gc_timeout;
	int ip6_rt_gc_interval;
	int ip6_rt_gc_elasticity;
	int ip6_rt_mtu_expires;
	int ip6_rt_min_advmss;
<<<<<<< HEAD
	int multipath_hash_policy;
	int flowlabel_consistency;
	int auto_flowlabels;
	int icmpv6_time;
	int icmpv6_echo_ignore_all;
	int anycast_src_echo_reply;
	int ip_nonlocal_bind;
	int fwmark_reflect;
	int idgen_retries;
	int idgen_delay;
	int flowlabel_state_ranges;
=======
	u32 multipath_hash_fields;
	u8 multipath_hash_policy;
	u8 bindv6only;
	u8 flowlabel_consistency;
	u8 auto_flowlabels;
	int icmpv6_time;
	u8 icmpv6_echo_ignore_all;
	u8 icmpv6_echo_ignore_multicast;
	u8 icmpv6_echo_ignore_anycast;
	DECLARE_BITMAP(icmpv6_ratemask, ICMPV6_MSG_MAX + 1);
	unsigned long *icmpv6_ratemask_ptr;
	u8 anycast_src_echo_reply;
	u8 ip_nonlocal_bind;
	u8 fwmark_reflect;
	u8 flowlabel_state_ranges;
	int idgen_retries;
	int idgen_delay;
>>>>>>> upstream/android-13
	int flowlabel_reflect;
	int max_dst_opts_cnt;
	int max_hbh_opts_cnt;
	int max_dst_opts_len;
	int max_hbh_opts_len;
	int seg6_flowlabel;
<<<<<<< HEAD
};

struct netns_ipv6 {
=======
	u32 ioam6_id;
	u64 ioam6_id_wide;
	bool skip_notify_on_dev_down;
	u8 fib_notify_on_flag_change;
	ANDROID_KABI_RESERVE(1);
};

struct netns_ipv6 {
	/* Keep ip6_dst_ops at the beginning of netns_sysctl_ipv6 */
	struct dst_ops		ip6_dst_ops;

>>>>>>> upstream/android-13
	struct netns_sysctl_ipv6 sysctl;
	struct ipv6_devconf	*devconf_all;
	struct ipv6_devconf	*devconf_dflt;
	struct inet_peer_base	*peers;
<<<<<<< HEAD
	struct netns_frags	frags;
#ifdef CONFIG_NETFILTER
	struct xt_table		*ip6table_filter;
	struct xt_table		*ip6table_mangle;
	struct xt_table		*ip6table_raw;
#ifdef CONFIG_SECURITY
	struct xt_table		*ip6table_security;
#endif
	struct xt_table		*ip6table_nat;
#endif
=======
	struct fqdir		*fqdir;
>>>>>>> upstream/android-13
	struct fib6_info	*fib6_null_entry;
	struct rt6_info		*ip6_null_entry;
	struct rt6_statistics   *rt6_stats;
	struct timer_list       ip6_fib_timer;
	struct hlist_head       *fib_table_hash;
	struct fib6_table       *fib6_main_tbl;
	struct list_head	fib6_walkers;
<<<<<<< HEAD
	struct dst_ops		ip6_dst_ops;
	rwlock_t		fib6_walker_lock;
	spinlock_t		fib6_gc_lock;
	unsigned int		 ip6_rt_gc_expire;
	unsigned long		 ip6_rt_last_gc;
#ifdef CONFIG_IPV6_MULTIPLE_TABLES
	unsigned int		fib6_rules_require_fldissect;
	bool			fib6_has_custom_rules;
=======
	rwlock_t		fib6_walker_lock;
	spinlock_t		fib6_gc_lock;
	atomic_t		ip6_rt_gc_expire;
	unsigned long		ip6_rt_last_gc;
	unsigned char		flowlabel_has_excl;
#ifdef CONFIG_IPV6_MULTIPLE_TABLES
	bool			fib6_has_custom_rules;
	unsigned int		fib6_rules_require_fldissect;
#ifdef CONFIG_IPV6_SUBTREES
	unsigned int		fib6_routes_require_src;
#endif
>>>>>>> upstream/android-13
	struct rt6_info         *ip6_prohibit_entry;
	struct rt6_info         *ip6_blk_hole_entry;
	struct fib6_table       *fib6_local_tbl;
	struct fib_rules_ops    *fib6_rules_ops;
#endif
<<<<<<< HEAD
	struct sock		**icmp_sk;
=======
	struct sock * __percpu	*icmp_sk;
>>>>>>> upstream/android-13
	struct sock             *ndisc_sk;
	struct sock             *tcp_sk;
	struct sock             *igmp_sk;
	struct sock		*mc_autojoin_sk;
#ifdef CONFIG_IPV6_MROUTE
#ifndef CONFIG_IPV6_MROUTE_MULTIPLE_TABLES
	struct mr_table		*mrt6;
#else
	struct list_head	mr6_tables;
	struct fib_rules_ops	*mr6_rules_ops;
#endif
#endif
	atomic_t		dev_addr_genid;
	atomic_t		fib6_sernum;
	struct seg6_pernet_data *seg6_data;
	struct fib_notifier_ops	*notifier_ops;
	struct fib_notifier_ops	*ip6mr_notifier_ops;
	unsigned int ipmr_seq; /* protected by rtnl_mutex */
	struct {
		struct hlist_head head;
		spinlock_t	lock;
		u32		seq;
	} ip6addrlbl_table;
<<<<<<< HEAD
=======
	struct ioam6_pernet_data *ioam6_data;

	ANDROID_KABI_RESERVE(1);
>>>>>>> upstream/android-13
};

#if IS_ENABLED(CONFIG_NF_DEFRAG_IPV6)
struct netns_nf_frag {
<<<<<<< HEAD
	struct netns_frags	frags;
=======
	struct fqdir	*fqdir;
>>>>>>> upstream/android-13
};
#endif

#endif
