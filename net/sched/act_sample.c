<<<<<<< HEAD
/*
 * net/sched/act_sample.c - Packet sampling tc action
 * Copyright (c) 2017 Yotam Gigi <yotamg@mellanox.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * net/sched/act_sample.c - Packet sampling tc action
 * Copyright (c) 2017 Yotam Gigi <yotamg@mellanox.com>
>>>>>>> upstream/android-13
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gfp.h>
#include <net/net_namespace.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>
#include <linux/tc_act/tc_sample.h>
#include <net/tc_act/tc_sample.h>
#include <net/psample.h>
<<<<<<< HEAD
=======
#include <net/pkt_cls.h>
>>>>>>> upstream/android-13

#include <linux/if_arp.h>

static unsigned int sample_net_id;
static struct tc_action_ops act_sample_ops;

static const struct nla_policy sample_policy[TCA_SAMPLE_MAX + 1] = {
	[TCA_SAMPLE_PARMS]		= { .len = sizeof(struct tc_sample) },
	[TCA_SAMPLE_RATE]		= { .type = NLA_U32 },
	[TCA_SAMPLE_TRUNC_SIZE]		= { .type = NLA_U32 },
	[TCA_SAMPLE_PSAMPLE_GROUP]	= { .type = NLA_U32 },
};

static int tcf_sample_init(struct net *net, struct nlattr *nla,
<<<<<<< HEAD
			   struct nlattr *est, struct tc_action **a, int ovr,
			   int bind, bool rtnl_held,
			   struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, sample_net_id);
	struct nlattr *tb[TCA_SAMPLE_MAX + 1];
	struct psample_group *psample_group;
	u32 psample_group_num, rate, index;
=======
			   struct nlattr *est, struct tc_action **a,
			   struct tcf_proto *tp,
			   u32 flags, struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, sample_net_id);
	bool bind = flags & TCA_ACT_FLAGS_BIND;
	struct nlattr *tb[TCA_SAMPLE_MAX + 1];
	struct psample_group *psample_group;
	u32 psample_group_num, rate, index;
	struct tcf_chain *goto_ch = NULL;
>>>>>>> upstream/android-13
	struct tc_sample *parm;
	struct tcf_sample *s;
	bool exists = false;
	int ret, err;

	if (!nla)
		return -EINVAL;
<<<<<<< HEAD
	ret = nla_parse_nested(tb, TCA_SAMPLE_MAX, nla, sample_policy, NULL);
=======
	ret = nla_parse_nested_deprecated(tb, TCA_SAMPLE_MAX, nla,
					  sample_policy, NULL);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;
	if (!tb[TCA_SAMPLE_PARMS] || !tb[TCA_SAMPLE_RATE] ||
	    !tb[TCA_SAMPLE_PSAMPLE_GROUP])
		return -EINVAL;

	parm = nla_data(tb[TCA_SAMPLE_PARMS]);
	index = parm->index;
	err = tcf_idr_check_alloc(tn, &index, a, bind);
	if (err < 0)
		return err;
	exists = err;
	if (exists && bind)
		return 0;

	if (!exists) {
		ret = tcf_idr_create(tn, index, est, a,
<<<<<<< HEAD
				     &act_sample_ops, bind, true);
=======
				     &act_sample_ops, bind, true, 0);
>>>>>>> upstream/android-13
		if (ret) {
			tcf_idr_cleanup(tn, index);
			return ret;
		}
		ret = ACT_P_CREATED;
<<<<<<< HEAD
	} else if (!ovr) {
		tcf_idr_release(*a, bind);
		return -EEXIST;
	}
=======
	} else if (!(flags & TCA_ACT_FLAGS_REPLACE)) {
		tcf_idr_release(*a, bind);
		return -EEXIST;
	}
	err = tcf_action_check_ctrlact(parm->action, tp, &goto_ch, extack);
	if (err < 0)
		goto release_idr;
>>>>>>> upstream/android-13

	rate = nla_get_u32(tb[TCA_SAMPLE_RATE]);
	if (!rate) {
		NL_SET_ERR_MSG(extack, "invalid sample rate");
<<<<<<< HEAD
		tcf_idr_release(*a, bind);
		return -EINVAL;
=======
		err = -EINVAL;
		goto put_chain;
>>>>>>> upstream/android-13
	}
	psample_group_num = nla_get_u32(tb[TCA_SAMPLE_PSAMPLE_GROUP]);
	psample_group = psample_group_get(net, psample_group_num);
	if (!psample_group) {
<<<<<<< HEAD
		tcf_idr_release(*a, bind);
		return -ENOMEM;
=======
		err = -ENOMEM;
		goto put_chain;
>>>>>>> upstream/android-13
	}

	s = to_sample(*a);

	spin_lock_bh(&s->tcf_lock);
<<<<<<< HEAD
	s->tcf_action = parm->action;
	s->rate = rate;
	s->psample_group_num = psample_group_num;
	rcu_swap_protected(s->psample_group, psample_group,
			   lockdep_is_held(&s->tcf_lock));
=======
	goto_ch = tcf_action_set_ctrlact(*a, parm->action, goto_ch);
	s->rate = rate;
	s->psample_group_num = psample_group_num;
	psample_group = rcu_replace_pointer(s->psample_group, psample_group,
					    lockdep_is_held(&s->tcf_lock));
>>>>>>> upstream/android-13

	if (tb[TCA_SAMPLE_TRUNC_SIZE]) {
		s->truncate = true;
		s->trunc_size = nla_get_u32(tb[TCA_SAMPLE_TRUNC_SIZE]);
	}
	spin_unlock_bh(&s->tcf_lock);

	if (psample_group)
		psample_group_put(psample_group);
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

static void tcf_sample_cleanup(struct tc_action *a)
{
	struct tcf_sample *s = to_sample(a);
	struct psample_group *psample_group;

	/* last reference to action, no need to lock */
	psample_group = rcu_dereference_protected(s->psample_group, 1);
	RCU_INIT_POINTER(s->psample_group, NULL);
	if (psample_group)
		psample_group_put(psample_group);
}

static bool tcf_sample_dev_ok_push(struct net_device *dev)
{
	switch (dev->type) {
	case ARPHRD_TUNNEL:
	case ARPHRD_TUNNEL6:
	case ARPHRD_SIT:
	case ARPHRD_IPGRE:
	case ARPHRD_IP6GRE:
	case ARPHRD_VOID:
	case ARPHRD_NONE:
		return false;
	default:
		return true;
	}
}

static int tcf_sample_act(struct sk_buff *skb, const struct tc_action *a,
			  struct tcf_result *res)
{
	struct tcf_sample *s = to_sample(a);
	struct psample_group *psample_group;
<<<<<<< HEAD
	int retval;
	int size;
	int iif;
	int oif;
=======
	struct psample_metadata md = {};
	int retval;
>>>>>>> upstream/android-13

	tcf_lastuse_update(&s->tcf_tm);
	bstats_cpu_update(this_cpu_ptr(s->common.cpu_bstats), skb);
	retval = READ_ONCE(s->tcf_action);

	psample_group = rcu_dereference_bh(s->psample_group);

	/* randomly sample packets according to rate */
	if (psample_group && (prandom_u32() % s->rate == 0)) {
		if (!skb_at_tc_ingress(skb)) {
<<<<<<< HEAD
			iif = skb->skb_iif;
			oif = skb->dev->ifindex;
		} else {
			iif = skb->dev->ifindex;
			oif = 0;
=======
			md.in_ifindex = skb->skb_iif;
			md.out_ifindex = skb->dev->ifindex;
		} else {
			md.in_ifindex = skb->dev->ifindex;
>>>>>>> upstream/android-13
		}

		/* on ingress, the mac header gets popped, so push it back */
		if (skb_at_tc_ingress(skb) && tcf_sample_dev_ok_push(skb->dev))
			skb_push(skb, skb->mac_len);

<<<<<<< HEAD
		size = s->truncate ? s->trunc_size : skb->len;
		psample_sample_packet(psample_group, skb, size, iif, oif,
				      s->rate);
=======
		md.trunc_size = s->truncate ? s->trunc_size : skb->len;
		psample_sample_packet(psample_group, skb, s->rate, &md);
>>>>>>> upstream/android-13

		if (skb_at_tc_ingress(skb) && tcf_sample_dev_ok_push(skb->dev))
			skb_pull(skb, skb->mac_len);
	}

	return retval;
}

<<<<<<< HEAD
=======
static void tcf_sample_stats_update(struct tc_action *a, u64 bytes, u64 packets,
				    u64 drops, u64 lastuse, bool hw)
{
	struct tcf_sample *s = to_sample(a);
	struct tcf_t *tm = &s->tcf_tm;

	tcf_action_update_stats(a, bytes, packets, drops, hw);
	tm->lastuse = max_t(u64, tm->lastuse, lastuse);
}

>>>>>>> upstream/android-13
static int tcf_sample_dump(struct sk_buff *skb, struct tc_action *a,
			   int bind, int ref)
{
	unsigned char *b = skb_tail_pointer(skb);
	struct tcf_sample *s = to_sample(a);
	struct tc_sample opt = {
		.index      = s->tcf_index,
		.refcnt     = refcount_read(&s->tcf_refcnt) - ref,
		.bindcnt    = atomic_read(&s->tcf_bindcnt) - bind,
	};
	struct tcf_t t;

	spin_lock_bh(&s->tcf_lock);
	opt.action = s->tcf_action;
	if (nla_put(skb, TCA_SAMPLE_PARMS, sizeof(opt), &opt))
		goto nla_put_failure;

	tcf_tm_dump(&t, &s->tcf_tm);
	if (nla_put_64bit(skb, TCA_SAMPLE_TM, sizeof(t), &t, TCA_SAMPLE_PAD))
		goto nla_put_failure;

	if (nla_put_u32(skb, TCA_SAMPLE_RATE, s->rate))
		goto nla_put_failure;

	if (s->truncate)
		if (nla_put_u32(skb, TCA_SAMPLE_TRUNC_SIZE, s->trunc_size))
			goto nla_put_failure;

	if (nla_put_u32(skb, TCA_SAMPLE_PSAMPLE_GROUP, s->psample_group_num))
		goto nla_put_failure;
	spin_unlock_bh(&s->tcf_lock);

	return skb->len;

nla_put_failure:
	spin_unlock_bh(&s->tcf_lock);
	nlmsg_trim(skb, b);
	return -1;
}

static int tcf_sample_walker(struct net *net, struct sk_buff *skb,
			     struct netlink_callback *cb, int type,
			     const struct tc_action_ops *ops,
			     struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, sample_net_id);

	return tcf_generic_walker(tn, skb, cb, type, ops, extack);
}

<<<<<<< HEAD
static int tcf_sample_search(struct net *net, struct tc_action **a, u32 index,
			     struct netlink_ext_ack *extack)
=======
static int tcf_sample_search(struct net *net, struct tc_action **a, u32 index)
>>>>>>> upstream/android-13
{
	struct tc_action_net *tn = net_generic(net, sample_net_id);

	return tcf_idr_search(tn, a, index);
}

<<<<<<< HEAD
static struct tc_action_ops act_sample_ops = {
	.kind	  = "sample",
	.type	  = TCA_ACT_SAMPLE,
	.owner	  = THIS_MODULE,
	.act	  = tcf_sample_act,
=======
static void tcf_psample_group_put(void *priv)
{
	struct psample_group *group = priv;

	psample_group_put(group);
}

static struct psample_group *
tcf_sample_get_group(const struct tc_action *a,
		     tc_action_priv_destructor *destructor)
{
	struct tcf_sample *s = to_sample(a);
	struct psample_group *group;

	group = rcu_dereference_protected(s->psample_group,
					  lockdep_is_held(&s->tcf_lock));
	if (group) {
		psample_group_take(group);
		*destructor = tcf_psample_group_put;
	}

	return group;
}

static struct tc_action_ops act_sample_ops = {
	.kind	  = "sample",
	.id	  = TCA_ID_SAMPLE,
	.owner	  = THIS_MODULE,
	.act	  = tcf_sample_act,
	.stats_update = tcf_sample_stats_update,
>>>>>>> upstream/android-13
	.dump	  = tcf_sample_dump,
	.init	  = tcf_sample_init,
	.cleanup  = tcf_sample_cleanup,
	.walk	  = tcf_sample_walker,
	.lookup	  = tcf_sample_search,
<<<<<<< HEAD
=======
	.get_psample_group = tcf_sample_get_group,
>>>>>>> upstream/android-13
	.size	  = sizeof(struct tcf_sample),
};

static __net_init int sample_init_net(struct net *net)
{
	struct tc_action_net *tn = net_generic(net, sample_net_id);

	return tc_action_net_init(net, tn, &act_sample_ops);
}

static void __net_exit sample_exit_net(struct list_head *net_list)
{
	tc_action_net_exit(net_list, sample_net_id);
}

static struct pernet_operations sample_net_ops = {
	.init = sample_init_net,
	.exit_batch = sample_exit_net,
	.id   = &sample_net_id,
	.size = sizeof(struct tc_action_net),
};

static int __init sample_init_module(void)
{
	return tcf_register_action(&act_sample_ops, &sample_net_ops);
}

static void __exit sample_cleanup_module(void)
{
	tcf_unregister_action(&act_sample_ops, &sample_net_ops);
}

module_init(sample_init_module);
module_exit(sample_cleanup_module);

MODULE_AUTHOR("Yotam Gigi <yotam.gi@gmail.com>");
MODULE_DESCRIPTION("Packet sampling action");
MODULE_LICENSE("GPL v2");
