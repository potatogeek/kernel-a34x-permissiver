<<<<<<< HEAD
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
>>>>>>> upstream/android-13
 */

#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/netfilter.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_timeout.h>

static const unsigned int nf_ct_generic_timeout = 600*HZ;

<<<<<<< HEAD
static bool nf_generic_should_process(u8 proto)
{
	switch (proto) {
#ifdef CONFIG_NF_CT_PROTO_GRE_MODULE
	case IPPROTO_GRE:
		return false;
#endif
	default:
		return true;
	}
}

static inline struct nf_generic_net *generic_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.generic;
}

static bool generic_pkt_to_tuple(const struct sk_buff *skb,
				 unsigned int dataoff,
				 struct net *net, struct nf_conntrack_tuple *tuple)
{
	tuple->src.u.all = 0;
	tuple->dst.u.all = 0;

	return true;
}

/* Returns verdict for packet, or -1 for invalid. */
static int generic_packet(struct nf_conn *ct,
			  const struct sk_buff *skb,
			  unsigned int dataoff,
			  enum ip_conntrack_info ctinfo)
{
	const unsigned int *timeout = nf_ct_timeout_lookup(ct);

	if (!timeout)
		timeout = &generic_pernet(nf_ct_net(ct))->timeout;

	nf_ct_refresh_acct(ct, ctinfo, skb, *timeout);
	return NF_ACCEPT;
}

/* Called when a new connection for this protocol found. */
static bool generic_new(struct nf_conn *ct, const struct sk_buff *skb,
			unsigned int dataoff)
{
	bool ret;

	ret = nf_generic_should_process(nf_ct_protonum(ct));
	if (!ret)
		pr_warn_once("conntrack: generic helper won't handle protocol %d. Please consider loading the specific helper module.\n",
			     nf_ct_protonum(ct));
	return ret;
}

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT

#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_cttimeout.h>

static int generic_timeout_nlattr_to_obj(struct nlattr *tb[],
					 struct net *net, void *data)
{
<<<<<<< HEAD
	struct nf_generic_net *gn = generic_pernet(net);
=======
	struct nf_generic_net *gn = nf_generic_pernet(net);
>>>>>>> upstream/android-13
	unsigned int *timeout = data;

	if (!timeout)
		timeout = &gn->timeout;

	if (tb[CTA_TIMEOUT_GENERIC_TIMEOUT])
		*timeout =
		    ntohl(nla_get_be32(tb[CTA_TIMEOUT_GENERIC_TIMEOUT])) * HZ;
	else {
		/* Set default generic timeout. */
		*timeout = gn->timeout;
	}

	return 0;
}

static int
generic_timeout_obj_to_nlattr(struct sk_buff *skb, const void *data)
{
	const unsigned int *timeout = data;

	if (nla_put_be32(skb, CTA_TIMEOUT_GENERIC_TIMEOUT, htonl(*timeout / HZ)))
		goto nla_put_failure;

	return 0;

nla_put_failure:
        return -ENOSPC;
}

static const struct nla_policy
generic_timeout_nla_policy[CTA_TIMEOUT_GENERIC_MAX+1] = {
	[CTA_TIMEOUT_GENERIC_TIMEOUT]	= { .type = NLA_U32 },
};
#endif /* CONFIG_NF_CONNTRACK_TIMEOUT */

<<<<<<< HEAD
#ifdef CONFIG_SYSCTL
static struct ctl_table generic_sysctl_table[] = {
	{
		.procname	= "nf_conntrack_generic_timeout",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{ }
};
#endif /* CONFIG_SYSCTL */

static int generic_kmemdup_sysctl_table(struct nf_proto_net *pn,
					struct nf_generic_net *gn)
{
#ifdef CONFIG_SYSCTL
	pn->ctl_table = kmemdup(generic_sysctl_table,
				sizeof(generic_sysctl_table),
				GFP_KERNEL);
	if (!pn->ctl_table)
		return -ENOMEM;

	pn->ctl_table[0].data = &gn->timeout;
#endif
	return 0;
}

static int generic_init_net(struct net *net, u_int16_t proto)
{
	struct nf_generic_net *gn = generic_pernet(net);
	struct nf_proto_net *pn = &gn->pn;

	gn->timeout = nf_ct_generic_timeout;

	return generic_kmemdup_sysctl_table(pn, gn);
}

static struct nf_proto_net *generic_get_net_proto(struct net *net)
{
	return &net->ct.nf_ct_proto.generic.pn;
=======
void nf_conntrack_generic_init_net(struct net *net)
{
	struct nf_generic_net *gn = nf_generic_pernet(net);

	gn->timeout = nf_ct_generic_timeout;
>>>>>>> upstream/android-13
}

const struct nf_conntrack_l4proto nf_conntrack_l4proto_generic =
{
<<<<<<< HEAD
	.l3proto		= PF_UNSPEC,
	.l4proto		= 255,
	.pkt_to_tuple		= generic_pkt_to_tuple,
	.packet			= generic_packet,
	.new			= generic_new,
=======
	.l4proto		= 255,
>>>>>>> upstream/android-13
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
	.ctnl_timeout		= {
		.nlattr_to_obj	= generic_timeout_nlattr_to_obj,
		.obj_to_nlattr	= generic_timeout_obj_to_nlattr,
		.nlattr_max	= CTA_TIMEOUT_GENERIC_MAX,
		.obj_size	= sizeof(unsigned int),
		.nla_policy	= generic_timeout_nla_policy,
	},
#endif /* CONFIG_NF_CONNTRACK_TIMEOUT */
<<<<<<< HEAD
	.init_net		= generic_init_net,
	.get_net_proto		= generic_get_net_proto,
=======
>>>>>>> upstream/android-13
};
