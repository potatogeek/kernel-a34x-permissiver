<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Stateless NAT actions
 *
 * Copyright (c) 2007 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/tc_act/tc_nat.h>
#include <net/act_api.h>
<<<<<<< HEAD
=======
#include <net/pkt_cls.h>
>>>>>>> upstream/android-13
#include <net/icmp.h>
#include <net/ip.h>
#include <net/netlink.h>
#include <net/tc_act/tc_nat.h>
#include <net/tcp.h>
#include <net/udp.h>


static unsigned int nat_net_id;
static struct tc_action_ops act_nat_ops;

static const struct nla_policy nat_policy[TCA_NAT_MAX + 1] = {
	[TCA_NAT_PARMS]	= { .len = sizeof(struct tc_nat) },
};

static int tcf_nat_init(struct net *net, struct nlattr *nla, struct nlattr *est,
<<<<<<< HEAD
			struct tc_action **a, int ovr, int bind,
			bool rtnl_held, struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, nat_net_id);
	struct nlattr *tb[TCA_NAT_MAX + 1];
=======
			struct tc_action **a, struct tcf_proto *tp,
			u32 flags, struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, nat_net_id);
	bool bind = flags & TCA_ACT_FLAGS_BIND;
	struct nlattr *tb[TCA_NAT_MAX + 1];
	struct tcf_chain *goto_ch = NULL;
>>>>>>> upstream/android-13
	struct tc_nat *parm;
	int ret = 0, err;
	struct tcf_nat *p;
	u32 index;

	if (nla == NULL)
		return -EINVAL;

<<<<<<< HEAD
	err = nla_parse_nested(tb, TCA_NAT_MAX, nla, nat_policy, NULL);
=======
	err = nla_parse_nested_deprecated(tb, TCA_NAT_MAX, nla, nat_policy,
					  NULL);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	if (tb[TCA_NAT_PARMS] == NULL)
		return -EINVAL;
	parm = nla_data(tb[TCA_NAT_PARMS]);
	index = parm->index;
	err = tcf_idr_check_alloc(tn, &index, a, bind);
	if (!err) {
		ret = tcf_idr_create(tn, index, est, a,
<<<<<<< HEAD
				     &act_nat_ops, bind, false);
=======
				     &act_nat_ops, bind, false, 0);
>>>>>>> upstream/android-13
		if (ret) {
			tcf_idr_cleanup(tn, index);
			return ret;
		}
		ret = ACT_P_CREATED;
	} else if (err > 0) {
		if (bind)
			return 0;
<<<<<<< HEAD
		if (!ovr) {
=======
		if (!(flags & TCA_ACT_FLAGS_REPLACE)) {
>>>>>>> upstream/android-13
			tcf_idr_release(*a, bind);
			return -EEXIST;
		}
	} else {
		return err;
	}
<<<<<<< HEAD
=======
	err = tcf_action_check_ctrlact(parm->action, tp, &goto_ch, extack);
	if (err < 0)
		goto release_idr;
>>>>>>> upstream/android-13
	p = to_tcf_nat(*a);

	spin_lock_bh(&p->tcf_lock);
	p->old_addr = parm->old_addr;
	p->new_addr = parm->new_addr;
	p->mask = parm->mask;
	p->flags = parm->flags;

<<<<<<< HEAD
	p->tcf_action = parm->action;
	spin_unlock_bh(&p->tcf_lock);

	if (ret == ACT_P_CREATED)
		tcf_idr_insert(tn, *a);

	return ret;
=======
	goto_ch = tcf_action_set_ctrlact(*a, parm->action, goto_ch);
	spin_unlock_bh(&p->tcf_lock);
	if (goto_ch)
		tcf_chain_put_by_act(goto_ch);

	return ret;
release_idr:
	tcf_idr_release(*a, bind);
	return err;
>>>>>>> upstream/android-13
}

static int tcf_nat_act(struct sk_buff *skb, const struct tc_action *a,
		       struct tcf_result *res)
{
	struct tcf_nat *p = to_tcf_nat(a);
	struct iphdr *iph;
	__be32 old_addr;
	__be32 new_addr;
	__be32 mask;
	__be32 addr;
	int egress;
	int action;
	int ihl;
	int noff;

	spin_lock(&p->tcf_lock);

	tcf_lastuse_update(&p->tcf_tm);
	old_addr = p->old_addr;
	new_addr = p->new_addr;
	mask = p->mask;
	egress = p->flags & TCA_NAT_FLAG_EGRESS;
	action = p->tcf_action;

	bstats_update(&p->tcf_bstats, skb);

	spin_unlock(&p->tcf_lock);

	if (unlikely(action == TC_ACT_SHOT))
		goto drop;

	noff = skb_network_offset(skb);
	if (!pskb_may_pull(skb, sizeof(*iph) + noff))
		goto drop;

	iph = ip_hdr(skb);

	if (egress)
		addr = iph->saddr;
	else
		addr = iph->daddr;

	if (!((old_addr ^ addr) & mask)) {
		if (skb_try_make_writable(skb, sizeof(*iph) + noff))
			goto drop;

		new_addr &= mask;
		new_addr |= addr & ~mask;

		/* Rewrite IP header */
		iph = ip_hdr(skb);
		if (egress)
			iph->saddr = new_addr;
		else
			iph->daddr = new_addr;

		csum_replace4(&iph->check, addr, new_addr);
	} else if ((iph->frag_off & htons(IP_OFFSET)) ||
		   iph->protocol != IPPROTO_ICMP) {
		goto out;
	}

	ihl = iph->ihl * 4;

	/* It would be nice to share code with stateful NAT. */
	switch (iph->frag_off & htons(IP_OFFSET) ? 0 : iph->protocol) {
	case IPPROTO_TCP:
	{
		struct tcphdr *tcph;

		if (!pskb_may_pull(skb, ihl + sizeof(*tcph) + noff) ||
		    skb_try_make_writable(skb, ihl + sizeof(*tcph) + noff))
			goto drop;

		tcph = (void *)(skb_network_header(skb) + ihl);
		inet_proto_csum_replace4(&tcph->check, skb, addr, new_addr,
					 true);
		break;
	}
	case IPPROTO_UDP:
	{
		struct udphdr *udph;

		if (!pskb_may_pull(skb, ihl + sizeof(*udph) + noff) ||
		    skb_try_make_writable(skb, ihl + sizeof(*udph) + noff))
			goto drop;

		udph = (void *)(skb_network_header(skb) + ihl);
		if (udph->check || skb->ip_summed == CHECKSUM_PARTIAL) {
			inet_proto_csum_replace4(&udph->check, skb, addr,
						 new_addr, true);
			if (!udph->check)
				udph->check = CSUM_MANGLED_0;
		}
		break;
	}
	case IPPROTO_ICMP:
	{
		struct icmphdr *icmph;

		if (!pskb_may_pull(skb, ihl + sizeof(*icmph) + noff))
			goto drop;

		icmph = (void *)(skb_network_header(skb) + ihl);

<<<<<<< HEAD
		if ((icmph->type != ICMP_DEST_UNREACH) &&
		    (icmph->type != ICMP_TIME_EXCEEDED) &&
		    (icmph->type != ICMP_PARAMETERPROB))
=======
		if (!icmp_is_err(icmph->type))
>>>>>>> upstream/android-13
			break;

		if (!pskb_may_pull(skb, ihl + sizeof(*icmph) + sizeof(*iph) +
					noff))
			goto drop;

		icmph = (void *)(skb_network_header(skb) + ihl);
		iph = (void *)(icmph + 1);
		if (egress)
			addr = iph->daddr;
		else
			addr = iph->saddr;

		if ((old_addr ^ addr) & mask)
			break;

		if (skb_try_make_writable(skb, ihl + sizeof(*icmph) +
					  sizeof(*iph) + noff))
			goto drop;

		icmph = (void *)(skb_network_header(skb) + ihl);
		iph = (void *)(icmph + 1);

		new_addr &= mask;
		new_addr |= addr & ~mask;

		/* XXX Fix up the inner checksums. */
		if (egress)
			iph->daddr = new_addr;
		else
			iph->saddr = new_addr;

		inet_proto_csum_replace4(&icmph->checksum, skb, addr, new_addr,
					 false);
		break;
	}
	default:
		break;
	}

out:
	return action;

drop:
	spin_lock(&p->tcf_lock);
	p->tcf_qstats.drops++;
	spin_unlock(&p->tcf_lock);
	return TC_ACT_SHOT;
}

static int tcf_nat_dump(struct sk_buff *skb, struct tc_action *a,
			int bind, int ref)
{
	unsigned char *b = skb_tail_pointer(skb);
	struct tcf_nat *p = to_tcf_nat(a);
	struct tc_nat opt = {
<<<<<<< HEAD
		.old_addr = p->old_addr,
		.new_addr = p->new_addr,
		.mask     = p->mask,
		.flags    = p->flags,

		.index    = p->tcf_index,
		.action   = p->tcf_action,
=======
		.index    = p->tcf_index,
>>>>>>> upstream/android-13
		.refcnt   = refcount_read(&p->tcf_refcnt) - ref,
		.bindcnt  = atomic_read(&p->tcf_bindcnt) - bind,
	};
	struct tcf_t t;

<<<<<<< HEAD
=======
	spin_lock_bh(&p->tcf_lock);
	opt.old_addr = p->old_addr;
	opt.new_addr = p->new_addr;
	opt.mask = p->mask;
	opt.flags = p->flags;
	opt.action = p->tcf_action;

>>>>>>> upstream/android-13
	if (nla_put(skb, TCA_NAT_PARMS, sizeof(opt), &opt))
		goto nla_put_failure;

	tcf_tm_dump(&t, &p->tcf_tm);
	if (nla_put_64bit(skb, TCA_NAT_TM, sizeof(t), &t, TCA_NAT_PAD))
		goto nla_put_failure;
<<<<<<< HEAD
=======
	spin_unlock_bh(&p->tcf_lock);
>>>>>>> upstream/android-13

	return skb->len;

nla_put_failure:
<<<<<<< HEAD
=======
	spin_unlock_bh(&p->tcf_lock);
>>>>>>> upstream/android-13
	nlmsg_trim(skb, b);
	return -1;
}

static int tcf_nat_walker(struct net *net, struct sk_buff *skb,
			  struct netlink_callback *cb, int type,
			  const struct tc_action_ops *ops,
			  struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, nat_net_id);

	return tcf_generic_walker(tn, skb, cb, type, ops, extack);
}

<<<<<<< HEAD
static int tcf_nat_search(struct net *net, struct tc_action **a, u32 index,
			  struct netlink_ext_ack *extack)
=======
static int tcf_nat_search(struct net *net, struct tc_action **a, u32 index)
>>>>>>> upstream/android-13
{
	struct tc_action_net *tn = net_generic(net, nat_net_id);

	return tcf_idr_search(tn, a, index);
}

static struct tc_action_ops act_nat_ops = {
	.kind		=	"nat",
<<<<<<< HEAD
	.type		=	TCA_ACT_NAT,
=======
	.id		=	TCA_ID_NAT,
>>>>>>> upstream/android-13
	.owner		=	THIS_MODULE,
	.act		=	tcf_nat_act,
	.dump		=	tcf_nat_dump,
	.init		=	tcf_nat_init,
	.walk		=	tcf_nat_walker,
	.lookup		=	tcf_nat_search,
	.size		=	sizeof(struct tcf_nat),
};

static __net_init int nat_init_net(struct net *net)
{
	struct tc_action_net *tn = net_generic(net, nat_net_id);

	return tc_action_net_init(net, tn, &act_nat_ops);
}

static void __net_exit nat_exit_net(struct list_head *net_list)
{
	tc_action_net_exit(net_list, nat_net_id);
}

static struct pernet_operations nat_net_ops = {
	.init = nat_init_net,
	.exit_batch = nat_exit_net,
	.id   = &nat_net_id,
	.size = sizeof(struct tc_action_net),
};

MODULE_DESCRIPTION("Stateless NAT actions");
MODULE_LICENSE("GPL");

static int __init nat_init_module(void)
{
	return tcf_register_action(&act_nat_ops, &nat_net_ops);
}

static void __exit nat_cleanup_module(void)
{
	tcf_unregister_action(&act_nat_ops, &nat_net_ops);
}

module_init(nat_init_module);
module_exit(nat_cleanup_module);
