<<<<<<< HEAD
/*
 * net/sched/act_gact.c		Generic actions
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * copyright 	Jamal Hadi Salim (2002-4)
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/act_gact.c		Generic actions
 *
 * copyright 	Jamal Hadi Salim (2002-4)
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
#include <net/netlink.h>
#include <net/pkt_sched.h>
<<<<<<< HEAD
=======
#include <net/pkt_cls.h>
>>>>>>> upstream/android-13
#include <linux/tc_act/tc_gact.h>
#include <net/tc_act/tc_gact.h>

static unsigned int gact_net_id;
static struct tc_action_ops act_gact_ops;

#ifdef CONFIG_GACT_PROB
static int gact_net_rand(struct tcf_gact *gact)
{
	smp_rmb(); /* coupled with smp_wmb() in tcf_gact_init() */
	if (prandom_u32() % gact->tcfg_pval)
		return gact->tcf_action;
	return gact->tcfg_paction;
}

static int gact_determ(struct tcf_gact *gact)
{
	u32 pack = atomic_inc_return(&gact->packets);

	smp_rmb(); /* coupled with smp_wmb() in tcf_gact_init() */
	if (pack % gact->tcfg_pval)
		return gact->tcf_action;
	return gact->tcfg_paction;
}

typedef int (*g_rand)(struct tcf_gact *gact);
static g_rand gact_rand[MAX_RAND] = { NULL, gact_net_rand, gact_determ };
#endif /* CONFIG_GACT_PROB */

static const struct nla_policy gact_policy[TCA_GACT_MAX + 1] = {
	[TCA_GACT_PARMS]	= { .len = sizeof(struct tc_gact) },
	[TCA_GACT_PROB]		= { .len = sizeof(struct tc_gact_p) },
};

static int tcf_gact_init(struct net *net, struct nlattr *nla,
			 struct nlattr *est, struct tc_action **a,
<<<<<<< HEAD
			 int ovr, int bind, bool rtnl_held,
			 struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);
	struct nlattr *tb[TCA_GACT_MAX + 1];
=======
			 struct tcf_proto *tp, u32 flags,
			 struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);
	bool bind = flags & TCA_ACT_FLAGS_BIND;
	struct nlattr *tb[TCA_GACT_MAX + 1];
	struct tcf_chain *goto_ch = NULL;
>>>>>>> upstream/android-13
	struct tc_gact *parm;
	struct tcf_gact *gact;
	int ret = 0;
	u32 index;
	int err;
#ifdef CONFIG_GACT_PROB
	struct tc_gact_p *p_parm = NULL;
#endif

	if (nla == NULL)
		return -EINVAL;

<<<<<<< HEAD
	err = nla_parse_nested(tb, TCA_GACT_MAX, nla, gact_policy, NULL);
=======
	err = nla_parse_nested_deprecated(tb, TCA_GACT_MAX, nla, gact_policy,
					  NULL);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	if (tb[TCA_GACT_PARMS] == NULL)
		return -EINVAL;
	parm = nla_data(tb[TCA_GACT_PARMS]);
	index = parm->index;

#ifndef CONFIG_GACT_PROB
	if (tb[TCA_GACT_PROB] != NULL)
		return -EOPNOTSUPP;
#else
	if (tb[TCA_GACT_PROB]) {
		p_parm = nla_data(tb[TCA_GACT_PROB]);
		if (p_parm->ptype >= MAX_RAND)
			return -EINVAL;
<<<<<<< HEAD
=======
		if (TC_ACT_EXT_CMP(p_parm->paction, TC_ACT_GOTO_CHAIN)) {
			NL_SET_ERR_MSG(extack,
				       "goto chain not allowed on fallback");
			return -EINVAL;
		}
>>>>>>> upstream/android-13
	}
#endif

	err = tcf_idr_check_alloc(tn, &index, a, bind);
	if (!err) {
<<<<<<< HEAD
		ret = tcf_idr_create(tn, index, est, a,
				     &act_gact_ops, bind, true);
=======
		ret = tcf_idr_create_from_flags(tn, index, est, a,
						&act_gact_ops, bind, flags);
>>>>>>> upstream/android-13
		if (ret) {
			tcf_idr_cleanup(tn, index);
			return ret;
		}
		ret = ACT_P_CREATED;
	} else if (err > 0) {
		if (bind)/* dont override defaults */
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
	gact = to_gact(*a);

	spin_lock_bh(&gact->tcf_lock);
	gact->tcf_action = parm->action;
=======
	err = tcf_action_check_ctrlact(parm->action, tp, &goto_ch, extack);
	if (err < 0)
		goto release_idr;
	gact = to_gact(*a);

	spin_lock_bh(&gact->tcf_lock);
	goto_ch = tcf_action_set_ctrlact(*a, parm->action, goto_ch);
>>>>>>> upstream/android-13
#ifdef CONFIG_GACT_PROB
	if (p_parm) {
		gact->tcfg_paction = p_parm->paction;
		gact->tcfg_pval    = max_t(u16, 1, p_parm->pval);
		/* Make sure tcfg_pval is written before tcfg_ptype
		 * coupled with smp_rmb() in gact_net_rand() & gact_determ()
		 */
		smp_wmb();
		gact->tcfg_ptype   = p_parm->ptype;
	}
#endif
	spin_unlock_bh(&gact->tcf_lock);

<<<<<<< HEAD
	if (ret == ACT_P_CREATED)
		tcf_idr_insert(tn, *a);
	return ret;
=======
	if (goto_ch)
		tcf_chain_put_by_act(goto_ch);

	return ret;
release_idr:
	tcf_idr_release(*a, bind);
	return err;
>>>>>>> upstream/android-13
}

static int tcf_gact_act(struct sk_buff *skb, const struct tc_action *a,
			struct tcf_result *res)
{
	struct tcf_gact *gact = to_gact(a);
	int action = READ_ONCE(gact->tcf_action);

#ifdef CONFIG_GACT_PROB
	{
	u32 ptype = READ_ONCE(gact->tcfg_ptype);

	if (ptype)
		action = gact_rand[ptype](gact);
	}
#endif
<<<<<<< HEAD
	bstats_cpu_update(this_cpu_ptr(gact->common.cpu_bstats), skb);
	if (action == TC_ACT_SHOT)
		qstats_drop_inc(this_cpu_ptr(gact->common.cpu_qstats));
=======
	tcf_action_update_bstats(&gact->common, skb);
	if (action == TC_ACT_SHOT)
		tcf_action_inc_drop_qstats(&gact->common);
>>>>>>> upstream/android-13

	tcf_lastuse_update(&gact->tcf_tm);

	return action;
}

<<<<<<< HEAD
static void tcf_gact_stats_update(struct tc_action *a, u64 bytes, u32 packets,
				  u64 lastuse)
=======
static void tcf_gact_stats_update(struct tc_action *a, u64 bytes, u64 packets,
				  u64 drops, u64 lastuse, bool hw)
>>>>>>> upstream/android-13
{
	struct tcf_gact *gact = to_gact(a);
	int action = READ_ONCE(gact->tcf_action);
	struct tcf_t *tm = &gact->tcf_tm;

<<<<<<< HEAD
	_bstats_cpu_update(this_cpu_ptr(gact->common.cpu_bstats), bytes,
			   packets);
	if (action == TC_ACT_SHOT)
		this_cpu_ptr(gact->common.cpu_qstats)->drops += packets;

=======
	tcf_action_update_stats(a, bytes, packets,
				action == TC_ACT_SHOT ? packets : drops, hw);
>>>>>>> upstream/android-13
	tm->lastuse = max_t(u64, tm->lastuse, lastuse);
}

static int tcf_gact_dump(struct sk_buff *skb, struct tc_action *a,
			 int bind, int ref)
{
	unsigned char *b = skb_tail_pointer(skb);
	struct tcf_gact *gact = to_gact(a);
	struct tc_gact opt = {
		.index   = gact->tcf_index,
		.refcnt  = refcount_read(&gact->tcf_refcnt) - ref,
		.bindcnt = atomic_read(&gact->tcf_bindcnt) - bind,
	};
	struct tcf_t t;

	spin_lock_bh(&gact->tcf_lock);
	opt.action = gact->tcf_action;
	if (nla_put(skb, TCA_GACT_PARMS, sizeof(opt), &opt))
		goto nla_put_failure;
#ifdef CONFIG_GACT_PROB
	if (gact->tcfg_ptype) {
		struct tc_gact_p p_opt = {
			.paction = gact->tcfg_paction,
			.pval    = gact->tcfg_pval,
			.ptype   = gact->tcfg_ptype,
		};

		if (nla_put(skb, TCA_GACT_PROB, sizeof(p_opt), &p_opt))
			goto nla_put_failure;
	}
#endif
	tcf_tm_dump(&t, &gact->tcf_tm);
	if (nla_put_64bit(skb, TCA_GACT_TM, sizeof(t), &t, TCA_GACT_PAD))
		goto nla_put_failure;
	spin_unlock_bh(&gact->tcf_lock);

	return skb->len;

nla_put_failure:
	spin_unlock_bh(&gact->tcf_lock);
	nlmsg_trim(skb, b);
	return -1;
}

static int tcf_gact_walker(struct net *net, struct sk_buff *skb,
			   struct netlink_callback *cb, int type,
			   const struct tc_action_ops *ops,
			   struct netlink_ext_ack *extack)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);

	return tcf_generic_walker(tn, skb, cb, type, ops, extack);
}

<<<<<<< HEAD
static int tcf_gact_search(struct net *net, struct tc_action **a, u32 index,
			   struct netlink_ext_ack *extack)
=======
static int tcf_gact_search(struct net *net, struct tc_action **a, u32 index)
>>>>>>> upstream/android-13
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);

	return tcf_idr_search(tn, a, index);
}

static size_t tcf_gact_get_fill_size(const struct tc_action *act)
{
	size_t sz = nla_total_size(sizeof(struct tc_gact)); /* TCA_GACT_PARMS */

#ifdef CONFIG_GACT_PROB
	if (to_gact(act)->tcfg_ptype)
		/* TCA_GACT_PROB */
		sz += nla_total_size(sizeof(struct tc_gact_p));
#endif

	return sz;
}

static struct tc_action_ops act_gact_ops = {
	.kind		=	"gact",
<<<<<<< HEAD
	.type		=	TCA_ACT_GACT,
=======
	.id		=	TCA_ID_GACT,
>>>>>>> upstream/android-13
	.owner		=	THIS_MODULE,
	.act		=	tcf_gact_act,
	.stats_update	=	tcf_gact_stats_update,
	.dump		=	tcf_gact_dump,
	.init		=	tcf_gact_init,
	.walk		=	tcf_gact_walker,
	.lookup		=	tcf_gact_search,
	.get_fill_size	=	tcf_gact_get_fill_size,
	.size		=	sizeof(struct tcf_gact),
};

static __net_init int gact_init_net(struct net *net)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);

	return tc_action_net_init(net, tn, &act_gact_ops);
}

static void __net_exit gact_exit_net(struct list_head *net_list)
{
	tc_action_net_exit(net_list, gact_net_id);
}

static struct pernet_operations gact_net_ops = {
	.init = gact_init_net,
	.exit_batch = gact_exit_net,
	.id   = &gact_net_id,
	.size = sizeof(struct tc_action_net),
};

MODULE_AUTHOR("Jamal Hadi Salim(2002-4)");
MODULE_DESCRIPTION("Generic Classifier actions");
MODULE_LICENSE("GPL");

static int __init gact_init_module(void)
{
#ifdef CONFIG_GACT_PROB
	pr_info("GACT probability on\n");
#else
	pr_info("GACT probability NOT on\n");
#endif

	return tcf_register_action(&act_gact_ops, &gact_net_ops);
}

static void __exit gact_cleanup_module(void)
{
	tcf_unregister_action(&act_gact_ops, &gact_net_ops);
}

module_init(gact_init_module);
module_exit(gact_cleanup_module);
