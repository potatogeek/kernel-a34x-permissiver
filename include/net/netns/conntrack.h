/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NETNS_CONNTRACK_H
#define __NETNS_CONNTRACK_H

#include <linux/list.h>
#include <linux/list_nulls.h>
#include <linux/atomic.h>
#include <linux/workqueue.h>
#include <linux/netfilter/nf_conntrack_tcp.h>
#ifdef CONFIG_NF_CT_PROTO_DCCP
#include <linux/netfilter/nf_conntrack_dccp.h>
#endif
#ifdef CONFIG_NF_CT_PROTO_SCTP
#include <linux/netfilter/nf_conntrack_sctp.h>
#endif
#include <linux/seqlock.h>

struct ctl_table_header;
struct nf_conntrack_ecache;

<<<<<<< HEAD
struct nf_proto_net {
#ifdef CONFIG_SYSCTL
	struct ctl_table_header *ctl_table_header;
	struct ctl_table        *ctl_table;
#endif
	unsigned int		users;
};

struct nf_generic_net {
	struct nf_proto_net pn;
=======
struct nf_generic_net {
>>>>>>> upstream/android-13
	unsigned int timeout;
};

struct nf_tcp_net {
<<<<<<< HEAD
	struct nf_proto_net pn;
	unsigned int timeouts[TCP_CONNTRACK_TIMEOUT_MAX];
	unsigned int tcp_loose;
	unsigned int tcp_be_liberal;
	unsigned int tcp_max_retrans;
=======
	unsigned int timeouts[TCP_CONNTRACK_TIMEOUT_MAX];
	u8 tcp_loose;
	u8 tcp_be_liberal;
	u8 tcp_max_retrans;
	u8 tcp_ignore_invalid_rst;
#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	unsigned int offload_timeout;
#endif
>>>>>>> upstream/android-13
};

enum udp_conntrack {
	UDP_CT_UNREPLIED,
	UDP_CT_REPLIED,
	UDP_CT_MAX
};

struct nf_udp_net {
<<<<<<< HEAD
	struct nf_proto_net pn;
	unsigned int timeouts[UDP_CT_MAX];
};

struct nf_icmp_net {
	struct nf_proto_net pn;
=======
	unsigned int timeouts[UDP_CT_MAX];
#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	unsigned int offload_timeout;
#endif
};

struct nf_icmp_net {
>>>>>>> upstream/android-13
	unsigned int timeout;
};

#ifdef CONFIG_NF_CT_PROTO_DCCP
struct nf_dccp_net {
<<<<<<< HEAD
	struct nf_proto_net pn;
	int dccp_loose;
=======
	u8 dccp_loose;
>>>>>>> upstream/android-13
	unsigned int dccp_timeout[CT_DCCP_MAX + 1];
};
#endif

#ifdef CONFIG_NF_CT_PROTO_SCTP
struct nf_sctp_net {
<<<<<<< HEAD
	struct nf_proto_net pn;
=======
>>>>>>> upstream/android-13
	unsigned int timeouts[SCTP_CONNTRACK_MAX];
};
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_NF_CT_PROTO_GRE
enum gre_conntrack {
	GRE_CT_UNREPLIED,
	GRE_CT_REPLIED,
	GRE_CT_MAX
};

struct nf_gre_net {
	struct list_head	keymap_list;
	unsigned int		timeouts[GRE_CT_MAX];
};
#endif

>>>>>>> upstream/android-13
struct nf_ip_net {
	struct nf_generic_net   generic;
	struct nf_tcp_net	tcp;
	struct nf_udp_net	udp;
	struct nf_icmp_net	icmp;
	struct nf_icmp_net	icmpv6;
#ifdef CONFIG_NF_CT_PROTO_DCCP
	struct nf_dccp_net	dccp;
#endif
#ifdef CONFIG_NF_CT_PROTO_SCTP
	struct nf_sctp_net	sctp;
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_NF_CT_PROTO_GRE
	struct nf_gre_net	gre;
#endif
>>>>>>> upstream/android-13
};

struct ct_pcpu {
	spinlock_t		lock;
	struct hlist_nulls_head unconfirmed;
	struct hlist_nulls_head dying;
};

struct netns_ct {
<<<<<<< HEAD
	atomic_t		count;
	unsigned int		expect_count;
#ifdef CONFIG_NF_CONNTRACK_EVENTS
	struct delayed_work ecache_dwork;
	bool ecache_dwork_pending;
#endif
#ifdef CONFIG_SYSCTL
	struct ctl_table_header	*sysctl_header;
	struct ctl_table_header	*acct_sysctl_header;
	struct ctl_table_header	*tstamp_sysctl_header;
	struct ctl_table_header	*event_sysctl_header;
	struct ctl_table_header	*helper_sysctl_header;
#endif
	unsigned int		sysctl_log_invalid; /* Log invalid packets */
	int			sysctl_events;
	int			sysctl_acct;
	int			sysctl_auto_assign_helper;
	bool			auto_assign_helper_warned;
	int			sysctl_tstamp;
	int			sysctl_checksum;
=======
#ifdef CONFIG_NF_CONNTRACK_EVENTS
	bool ecache_dwork_pending;
#endif
	u8			sysctl_log_invalid; /* Log invalid packets */
	u8			sysctl_events;
	u8			sysctl_acct;
	u8			sysctl_auto_assign_helper;
	u8			sysctl_tstamp;
	u8			sysctl_checksum;
>>>>>>> upstream/android-13

	struct ct_pcpu __percpu *pcpu_lists;
	struct ip_conntrack_stat __percpu *stat;
	struct nf_ct_event_notifier __rcu *nf_conntrack_event_cb;
<<<<<<< HEAD
	struct nf_exp_event_notifier __rcu *nf_expect_event_cb;
=======
>>>>>>> upstream/android-13
	struct nf_ip_net	nf_ct_proto;
#if defined(CONFIG_NF_CONNTRACK_LABELS)
	unsigned int		labels_used;
#endif
};
#endif
