<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * net/sched/act_skbmod.c  skb data modifier
 *
 * Copyright (c) 2016 Jamal Hadi Salim <jhs@mojatatu.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
*/

#include <linux/module.h>
=======
*/

#include <linux/module.h>
#include <linux/if_arp.h>
>>>>>>> upstream/android-13
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
<<<<<<< HEAD
#include <net/netlink.h>
#include <net/pkt_sched.h>
=======
#include <net/inet_ecn.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>
#include <net/pkt_cls.h>
>>>>>>> upstream/android-13

#include <linux/tc_act/tc_skbmod.h>
#include <net/tc_act/tc_skbmod.h>

static unsigned int skbmod_net_id;
static struct tc_action_ops act_skbmod_ops;

<<<<<<< HEAD
#define MAX_EDIT_LEN ETH_HLEN
=======
>>>>>>> upstream/android-13
static int tcf_skbmod_act(struct sk_buff *skb, const struct tc_action *a,
			  struct tcf_result *res)
{
	struct tcf_skbmod *d = to_skbmod(a);
<<<<<<< HEAD
	int action;
	struct tcf_skbmod_params *p;
	u64 flags;
	int err;
=======
	int action, max_edit_len, err;
	struct tcf_skbmod_params *p;
	u64 flags;
>>>>>>> upstream/android-13

	tcf_lastuse_update(&d->tcf_tm);
	bstats_cpu_update(this_cpu_ptr(d->common.cpu_bstats), skb);

<<<<<<< HEAD
	/* XXX: if you are going to edit more fields beyond ethernet header
	 * (example when you add IP header replacement or vlan swap)
	 * then MAX_EDIT_LEN needs to change appropriately
	*/
	err = skb_ensure_writable(skb, MAX_EDIT_LEN);
	if (unlikely(err)) /* best policy is to drop on the floor */
		goto drop;

=======
>>>>>>> upstream/android-13
	action = READ_ONCE(d->tcf_action);
	if (unlikely(action == TC_ACT_SHOT))
		goto drop;

<<<<<<< HEAD
	p = rcu_dereference_bh(d->skbmod_p);
	flags = p->flags;
=======
	max_edit_len = skb_mac_header_len(skb);
	p = rcu_dereference_bh(d->skbmod_p);
	flags = p->flags;

	/* tcf_skbmod_init() guarantees "flags" to be one of the following:
	 *	1. a combination of SKBMOD_F_{DMAC,SMAC,ETYPE}
	 *	2. SKBMOD_F_SWAPMAC
	 *	3. SKBMOD_F_ECN
	 * SKBMOD_F_ECN only works with IP packets; all other flags only work with Ethernet
	 * packets.
	 */
	if (flags == SKBMOD_F_ECN) {
		switch (skb_protocol(skb, true)) {
		case cpu_to_be16(ETH_P_IP):
		case cpu_to_be16(ETH_P_IPV6):
			max_edit_len += skb_network_header_len(skb);
			break;
		default:
			goto out;
		}
	} else if (!skb->dev || skb->dev->type != ARPHRD_ETHER) {
		goto out;
	}

	err = skb_ensure_writable(skb, max_edit_len);
	if (unlikely(err)) /* best policy is to drop on the floor */
		goto drop;

>>>>>>> upstream/android-13
	if (flags & SKBMOD_F_DMAC)
		ether_addr_copy(eth_hdr(skb)->h_dest, p->eth_dst);
	if (flags & SKBMOD_F_SMAC)
		ether_addr_copy(eth_hdr(skb)->h_source, p->eth_src);
	if (flags & SKBMOD_F_ETYPE)
		eth_hdr(skb)->h_proto = p->eth_type;

	if (flags & SKBMOD_F_SWAPMAC) {
		u16 tmpaddr[ETH_ALEN / 2]; /* ether_addr_copy() requirement */
		/*XXX: I am sure we can come up with more efficient swapping*/
		ether_addr_copy((u8 *)tmpaddr, eth_hdr(skb)->h_dest);
		ether_addr_copy(eth_hdr(skb)->h_dest, eth_hdr(skb)->h_source);
		ether_addr_copy(eth_hdr(skb)->h_source, (u8 *)tmpaddr);
	}

<<<<<<< HEAD
=======
	if (flags & SKBMOD_F_ECN)
		INET_ECN_set_ce(skb);

out:
>>>>>>> upstream/android-13
	return action;

drop:
	qstats_overlimit_inc(this_cpu_ptr(d->common.cpu_qstats));
	return TC_ACT_SHOT;
}

static const struct nla_policy skbmod_policy[TCA_SKBMOD_MAX + 1] = {
	[TCA_SKBMOD_PARMS]		= { .len = sizeof(struct tc_skbmod) },
	[TCA_SKBMOD_DMAC]		= { .len = ETH_ALEN },
	[TCA_SKBMOD_SMAC]		= { .len = ETH_ALEN },
	[TCA_SKBMOD_ETYPE]		= { .type = NLA_U16 },
};

static int tcf_skbmod_init(struct net *net, struct nlattr *nla,
			   struct nlattr *est, struct tc_action **a,
<<<<<<< HEAD
			   int ovr, int bind, bool rtnl_held,
			   struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, skbmod_net_id);
	struct nlattr *tb[TCA_SKBMOD_MAX + 1];
	struct tcf_skbmod_params *p, *p_old;
=======
			   struct tcf_proto *tp, u32 flags,
			   struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, skbmod_net_id);
	bool ovr = flags & TCA_ACT_FLAGS_REPLACE;
	bool bind = flags & TCA_ACT_FLAGS_BIND;
	struct nlattr *tb[TCA_SKBMOD_MAX + 1];
	struct tcf_skbmod_params *p, *p_old;
	struct tcf_chain *goto_ch = NULL;
>>>>>>> upstream/android-13
	struct tc_skbmod *parm;
	u32 lflags = 0, index;
	struct tcf_skbmod *d;
	bool exists = false;
	u8 *daddr = NULL;
	u8 *saddr = NULL;
	u16 eth_type = 0;
	int ret = 0, err;

	if (!nla)
		return -EINVAL;

<<<<<<< HEAD
	err = nla_parse_nested(tb, TCA_SKBMOD_MAX, nla, skbmod_policy, NULL);
=======
	err = nla_parse_nested_deprecated(tb, TCA_SKBMOD_MAX, nla,
					  skbmod_policy, NULL);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	if (!tb[TCA_SKBMOD_PARMS])
		return -EINVAL;

	if (tb[TCA_SKBMOD_DMAC]) {
		daddr = nla_data(tb[TCA_SKBMOD_DMAC]);
		lflags |= SKBMOD_F_DMAC;
	}

	if (tb[TCA_SKBMOD_SMAC]) {
		saddr = nla_data(tb[TCA_SKBMOD_SMAC]);
		lflags |= SKBMOD_F_SMAC;
	}

	if (tb[TCA_SKBMOD_ETYPE]) {
		eth_type = nla_get_u16(tb[TCA_SKBMOD_ETYPE]);
		lflags |= SKBMOD_F_ETYPE;
	}

	parm = nla_data(tb[TCA_SKBMOD_PARMS]);
	index = parm->index;
	if (parm->flags & SKBMOD_F_SWAPMAC)
		lflags = SKBMOD_F_SWAPMAC;
<<<<<<< HEAD
=======
	if (parm->flags & SKBMOD_F_ECN)
		lflags = SKBMOD_F_ECN;
>>>>>>> upstream/android-13

	err = tcf_idr_check_alloc(tn, &index, a, bind);
	if (err < 0)
		return err;
	exists = err;
	if (exists && bind)
		return 0;

	if (!lflags) {
		if (exists)
			tcf_idr_release(*a, bind);
		else
			tcf_idr_cleanup(tn, index);
		return -EINVAL;
	}

	if (!exists) {
		ret = tcf_idr_create(tn, index, est, a,
<<<<<<< HEAD
				     &act_skbmod_ops, bind, true);
=======
				     &act_skbmod_ops, bind, true, 0);
>>>>>>> upstream/android-13
		if (ret) {
			tcf_idr_cleanup(tn, index);
			return ret;
		}

		ret = ACT_P_CREATED;
	} else if (!ovr) {
		tcf_idr_release(*a, bind);
		return -EEXIST;
	}
<<<<<<< HEAD
=======
	err = tcf_action_check_ctrlact(parm->action, tp, &goto_ch, extack);
	if (err < 0)
		goto release_idr;
>>>>>>> upstream/android-13

	d = to_skbmod(*a);

	p = kzalloc(sizeof(struct tcf_skbmod_params), GFP_KERNEL);
	if (unlikely(!p)) {
<<<<<<< HEAD
		tcf_idr_release(*a, bind);
		return -ENOMEM;
	}

	p->flags = lflags;
	d->tcf_action = parm->action;
=======
		err = -ENOMEM;
		goto put_chain;
	}

	p->flags = lflags;
>>>>>>> upstream/android-13

	if (ovr)
		spin_lock_bh(&d->tcf_lock);
	/* Protected by tcf_lock if overwriting existing action. */
<<<<<<< HEAD
=======
	goto_ch = tcf_action_set_ctrlact(*a, parm->action, goto_ch);
>>>>>>> upstream/android-13
	p_old = rcu_dereference_protected(d->skbmod_p, 1);

	if (lflags & SKBMOD_F_DMAC)
		ether_addr_copy(p->eth_dst, daddr);
	if (lflags & SKBMOD_F_SMAC)
		ether_addr_copy(p->eth_src, saddr);
	if (lflags & SKBMOD_F_ETYPE)
		p->eth_type = htons(eth_type);

	rcu_assign_pointer(d->skbmod_p, p);
	if (ovr)
		spin_unlock_bh(&d->tcf_lock);

	if (p_old)
		kfree_rcu(p_old, rcu);
<<<<<<< HEAD

	if (ret == ACT_P_CREATED)
		tcf_idr_insert(tn, *a);
	return ret;
=======
	if (goto_ch)
		tcf_chain_put_by_act(goto_ch);

	return ret;
put_chain:
	if (goto_ch)
		tcf_chain_put_by_act(goto_ch);
release_idr:
	tcf_idr_release(*a, bind);
	return err;
>>>>>>> upstream/android-13
}

static void tcf_skbmod_cleanup(struct tc_action *a)
{
	struct tcf_skbmod *d = to_skbmod(a);
	struct tcf_skbmod_params  *p;

	p = rcu_dereference_protected(d->skbmod_p, 1);
	if (p)
		kfree_rcu(p, rcu);
}

static int tcf_skbmod_dump(struct sk_buff *skb, struct tc_action *a,
			   int bind, int ref)
{
	struct tcf_skbmod *d = to_skbmod(a);
	unsigned char *b = skb_tail_pointer(skb);
	struct tcf_skbmod_params  *p;
	struct tc_skbmod opt = {
		.index   = d->tcf_index,
		.refcnt  = refcount_read(&d->tcf_refcnt) - ref,
		.bindcnt = atomic_read(&d->tcf_bindcnt) - bind,
	};
	struct tcf_t t;

	spin_lock_bh(&d->tcf_lock);
	opt.action = d->tcf_action;
	p = rcu_dereference_protected(d->skbmod_p,
				      lockdep_is_held(&d->tcf_lock));
	opt.flags  = p->flags;
	if (nla_put(skb, TCA_SKBMOD_PARMS, sizeof(opt), &opt))
		goto nla_put_failure;
	if ((p->flags & SKBMOD_F_DMAC) &&
	    nla_put(skb, TCA_SKBMOD_DMAC, ETH_ALEN, p->eth_dst))
		goto nla_put_failure;
	if ((p->flags & SKBMOD_F_SMAC) &&
	    nla_put(skb, TCA_SKBMOD_SMAC, ETH_ALEN, p->eth_src))
		goto nla_put_failure;
	if ((p->flags & SKBMOD_F_ETYPE) &&
	    nla_put_u16(skb, TCA_SKBMOD_ETYPE, ntohs(p->eth_type)))
		goto nla_put_failure;

	tcf_tm_dump(&t, &d->tcf_tm);
	if (nla_put_64bit(skb, TCA_SKBMOD_TM, sizeof(t), &t, TCA_SKBMOD_PAD))
		goto nla_put_failure;

	spin_unlock_bh(&d->tcf_lock);
	return skb->len;
nla_put_failure:
	spin_unlock_bh(&d->tcf_lock);
	nlmsg_trim(skb, b);
	return -1;
}

static int tcf_skbmod_walker(struct net *net, struct sk_buff *skb,
			     struct netlink_callback *cb, int type,
			     const struct tc_action_ops *ops,
			     struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, skbmod_net_id);

	return tcf_generic_walker(tn, skb, cb, type, ops, extack);
}

<<<<<<< HEAD
static int tcf_skbmod_search(struct net *net, struct tc_action **a, u32 index,
			     struct netlink_ext_ack *extack)
=======
static int tcf_skbmod_search(struct net *net, struct tc_action **a, u32 index)
>>>>>>> upstream/android-13
{
	struct tc_action_net *tn = net_generic(net, skbmod_net_id);

	return tcf_idr_search(tn, a, index);
}

static struct tc_action_ops act_skbmod_ops = {
	.kind		=	"skbmod",
<<<<<<< HEAD
	.type		=	TCA_ACT_SKBMOD,
=======
	.id		=	TCA_ACT_SKBMOD,
>>>>>>> upstream/android-13
	.owner		=	THIS_MODULE,
	.act		=	tcf_skbmod_act,
	.dump		=	tcf_skbmod_dump,
	.init		=	tcf_skbmod_init,
	.cleanup	=	tcf_skbmod_cleanup,
	.walk		=	tcf_skbmod_walker,
	.lookup		=	tcf_skbmod_search,
	.size		=	sizeof(struct tcf_skbmod),
};

static __net_init int skbmod_init_net(struct net *net)
{
	struct tc_action_net *tn = net_generic(net, skbmod_net_id);

	return tc_action_net_init(net, tn, &act_skbmod_ops);
}

static void __net_exit skbmod_exit_net(struct list_head *net_list)
{
	tc_action_net_exit(net_list, skbmod_net_id);
}

static struct pernet_operations skbmod_net_ops = {
	.init = skbmod_init_net,
	.exit_batch = skbmod_exit_net,
	.id   = &skbmod_net_id,
	.size = sizeof(struct tc_action_net),
};

MODULE_AUTHOR("Jamal Hadi Salim, <jhs@mojatatu.com>");
MODULE_DESCRIPTION("SKB data mod-ing");
MODULE_LICENSE("GPL");

static int __init skbmod_init_module(void)
{
	return tcf_register_action(&act_skbmod_ops, &skbmod_net_ops);
}

static void __exit skbmod_cleanup_module(void)
{
	tcf_unregister_action(&act_skbmod_ops, &skbmod_net_ops);
}

module_init(skbmod_init_module);
module_exit(skbmod_cleanup_module);
