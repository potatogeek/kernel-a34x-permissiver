<<<<<<< HEAD
/*
 * Copyright (c) 2010 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2010 Patrick McHardy <kaber@trash.net>
>>>>>>> upstream/android-13
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_CT.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_timeout.h>
#include <net/netfilter/nf_conntrack_zones.h>

static inline int xt_ct_target(struct sk_buff *skb, struct nf_conn *ct)
{
	/* Previously seen (loopback)? Ignore. */
	if (skb->_nfct != 0)
		return XT_CONTINUE;

	if (ct) {
<<<<<<< HEAD
		atomic_inc(&ct->ct_general.use);
=======
		refcount_inc(&ct->ct_general.use);
>>>>>>> upstream/android-13
		nf_ct_set(skb, ct, IP_CT_NEW);
	} else {
		nf_ct_set(skb, ct, IP_CT_UNTRACKED);
	}

	return XT_CONTINUE;
}

static unsigned int xt_ct_target_v0(struct sk_buff *skb,
				    const struct xt_action_param *par)
{
	const struct xt_ct_target_info *info = par->targinfo;
	struct nf_conn *ct = info->ct;

	return xt_ct_target(skb, ct);
}

static unsigned int xt_ct_target_v1(struct sk_buff *skb,
				    const struct xt_action_param *par)
{
	const struct xt_ct_target_info_v1 *info = par->targinfo;
	struct nf_conn *ct = info->ct;

	return xt_ct_target(skb, ct);
}

static u8 xt_ct_find_proto(const struct xt_tgchk_param *par)
{
	if (par->family == NFPROTO_IPV4) {
		const struct ipt_entry *e = par->entryinfo;

		if (e->ip.invflags & IPT_INV_PROTO)
			return 0;
		return e->ip.proto;
	} else if (par->family == NFPROTO_IPV6) {
		const struct ip6t_entry *e = par->entryinfo;

		if (e->ipv6.invflags & IP6T_INV_PROTO)
			return 0;
		return e->ipv6.proto;
	} else
		return 0;
}

static int
xt_ct_set_helper(struct nf_conn *ct, const char *helper_name,
		 const struct xt_tgchk_param *par)
{
	struct nf_conntrack_helper *helper;
	struct nf_conn_help *help;
	u8 proto;

	proto = xt_ct_find_proto(par);
	if (!proto) {
		pr_info_ratelimited("You must specify a L4 protocol and not use inversions on it\n");
		return -ENOENT;
	}

	helper = nf_conntrack_helper_try_module_get(helper_name, par->family,
						    proto);
	if (helper == NULL) {
		pr_info_ratelimited("No such helper \"%s\"\n", helper_name);
		return -ENOENT;
	}

	help = nf_ct_helper_ext_add(ct, GFP_KERNEL);
	if (help == NULL) {
		nf_conntrack_helper_put(helper);
		return -ENOMEM;
	}

	help->helper = helper;
	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
static void __xt_ct_tg_timeout_put(struct nf_ct_timeout *timeout)
{
	typeof(nf_ct_timeout_put_hook) timeout_put;

	timeout_put = rcu_dereference(nf_ct_timeout_put_hook);
	if (timeout_put)
		timeout_put(timeout);
}
#endif

=======
>>>>>>> upstream/android-13
static int
xt_ct_set_timeout(struct nf_conn *ct, const struct xt_tgchk_param *par,
		  const char *timeout_name)
{
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
<<<<<<< HEAD
	typeof(nf_ct_timeout_find_get_hook) timeout_find_get;
	const struct nf_conntrack_l4proto *l4proto;
	struct nf_ct_timeout *timeout;
	struct nf_conn_timeout *timeout_ext;
	const char *errmsg = NULL;
	int ret = 0;
	u8 proto;

	rcu_read_lock();
	timeout_find_get = rcu_dereference(nf_ct_timeout_find_get_hook);
	if (timeout_find_get == NULL) {
		ret = -ENOENT;
		errmsg = "Timeout policy base is empty";
		goto out;
	}

	proto = xt_ct_find_proto(par);
	if (!proto) {
		ret = -EINVAL;
		errmsg = "You must specify a L4 protocol and not use inversions on it";
		goto out;
	}

	timeout = timeout_find_get(par->net, timeout_name);
	if (timeout == NULL) {
		ret = -ENOENT;
		pr_info_ratelimited("No such timeout policy \"%s\"\n",
				    timeout_name);
		goto out;
	}

	if (timeout->l3num != par->family) {
		ret = -EINVAL;
		pr_info_ratelimited("Timeout policy `%s' can only be used by L%d protocol number %d\n",
				    timeout_name, 3, timeout->l3num);
		goto err_put_timeout;
	}
	/* Make sure the timeout policy matches any existing protocol tracker,
	 * otherwise default to generic.
	 */
	l4proto = __nf_ct_l4proto_find(par->family, proto);
	if (timeout->l4proto->l4proto != l4proto->l4proto) {
		ret = -EINVAL;
		pr_info_ratelimited("Timeout policy `%s' can only be used by L%d protocol number %d\n",
				    timeout_name, 4, timeout->l4proto->l4proto);
		goto err_put_timeout;
	}
	timeout_ext = nf_ct_timeout_ext_add(ct, timeout, GFP_ATOMIC);
	if (!timeout_ext) {
		ret = -ENOMEM;
		goto err_put_timeout;
	}

	rcu_read_unlock();
	return ret;

err_put_timeout:
	__xt_ct_tg_timeout_put(timeout);
out:
	rcu_read_unlock();
	if (errmsg)
		pr_info_ratelimited("%s\n", errmsg);
	return ret;
=======
	const struct nf_conntrack_l4proto *l4proto;
	u8 proto;

	proto = xt_ct_find_proto(par);
	if (!proto) {
		pr_info_ratelimited("You must specify a L4 protocol and not "
				    "use inversions on it");
		return -EINVAL;
	}
	l4proto = nf_ct_l4proto_find(proto);
	return nf_ct_set_timeout(par->net, ct, par->family, l4proto->l4proto,
				 timeout_name);

>>>>>>> upstream/android-13
#else
	return -EOPNOTSUPP;
#endif
}

static u16 xt_ct_flags_to_dir(const struct xt_ct_target_info_v1 *info)
{
	switch (info->flags & (XT_CT_ZONE_DIR_ORIG |
			       XT_CT_ZONE_DIR_REPL)) {
	case XT_CT_ZONE_DIR_ORIG:
		return NF_CT_ZONE_DIR_ORIG;
	case XT_CT_ZONE_DIR_REPL:
		return NF_CT_ZONE_DIR_REPL;
	default:
		return NF_CT_DEFAULT_ZONE_DIR;
	}
}

static int xt_ct_tg_check(const struct xt_tgchk_param *par,
			  struct xt_ct_target_info_v1 *info)
{
	struct nf_conntrack_zone zone;
	struct nf_conn_help *help;
	struct nf_conn *ct;
	int ret = -EOPNOTSUPP;

	if (info->flags & XT_CT_NOTRACK) {
		ct = NULL;
		goto out;
	}

#ifndef CONFIG_NF_CONNTRACK_ZONES
	if (info->zone || info->flags & (XT_CT_ZONE_DIR_ORIG |
					 XT_CT_ZONE_DIR_REPL |
					 XT_CT_ZONE_MARK))
		goto err1;
#endif

	ret = nf_ct_netns_get(par->net, par->family);
	if (ret < 0)
		goto err1;

	memset(&zone, 0, sizeof(zone));
	zone.id = info->zone;
	zone.dir = xt_ct_flags_to_dir(info);
	if (info->flags & XT_CT_ZONE_MARK)
		zone.flags |= NF_CT_FLAG_MARK;

	ct = nf_ct_tmpl_alloc(par->net, &zone, GFP_KERNEL);
	if (!ct) {
		ret = -ENOMEM;
		goto err2;
	}

<<<<<<< HEAD
	ret = 0;
=======
>>>>>>> upstream/android-13
	if ((info->ct_events || info->exp_events) &&
	    !nf_ct_ecache_ext_add(ct, info->ct_events, info->exp_events,
				  GFP_KERNEL)) {
		ret = -EINVAL;
		goto err3;
	}

	if (info->helper[0]) {
		if (strnlen(info->helper, sizeof(info->helper)) == sizeof(info->helper)) {
			ret = -ENAMETOOLONG;
			goto err3;
		}

		ret = xt_ct_set_helper(ct, info->helper, par);
		if (ret < 0)
			goto err3;
	}

	if (info->timeout[0]) {
		if (strnlen(info->timeout, sizeof(info->timeout)) == sizeof(info->timeout)) {
			ret = -ENAMETOOLONG;
			goto err4;
		}

		ret = xt_ct_set_timeout(ct, par, info->timeout);
		if (ret < 0)
			goto err4;
	}
	__set_bit(IPS_CONFIRMED_BIT, &ct->status);
<<<<<<< HEAD
	nf_conntrack_get(&ct->ct_general);
=======
>>>>>>> upstream/android-13
out:
	info->ct = ct;
	return 0;

err4:
	help = nfct_help(ct);
	if (help)
		nf_conntrack_helper_put(help->helper);
err3:
	nf_ct_tmpl_free(ct);
err2:
	nf_ct_netns_put(par->net, par->family);
err1:
	return ret;
}

static int xt_ct_tg_check_v0(const struct xt_tgchk_param *par)
{
	struct xt_ct_target_info *info = par->targinfo;
	struct xt_ct_target_info_v1 info_v1 = {
		.flags 		= info->flags,
		.zone		= info->zone,
		.ct_events	= info->ct_events,
		.exp_events	= info->exp_events,
	};
	int ret;

	if (info->flags & ~XT_CT_NOTRACK)
		return -EINVAL;

	memcpy(info_v1.helper, info->helper, sizeof(info->helper));

	ret = xt_ct_tg_check(par, &info_v1);
	if (ret < 0)
		return ret;

	info->ct = info_v1.ct;

	return ret;
}

static int xt_ct_tg_check_v1(const struct xt_tgchk_param *par)
{
	struct xt_ct_target_info_v1 *info = par->targinfo;

	if (info->flags & ~XT_CT_NOTRACK)
		return -EINVAL;

	return xt_ct_tg_check(par, par->targinfo);
}

static int xt_ct_tg_check_v2(const struct xt_tgchk_param *par)
{
	struct xt_ct_target_info_v1 *info = par->targinfo;

	if (info->flags & ~XT_CT_MASK)
		return -EINVAL;

	return xt_ct_tg_check(par, par->targinfo);
}

<<<<<<< HEAD
static void xt_ct_destroy_timeout(struct nf_conn *ct)
{
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
	struct nf_conn_timeout *timeout_ext;
	typeof(nf_ct_timeout_put_hook) timeout_put;

	rcu_read_lock();
	timeout_put = rcu_dereference(nf_ct_timeout_put_hook);

	if (timeout_put) {
		timeout_ext = nf_ct_timeout_find(ct);
		if (timeout_ext) {
			timeout_put(timeout_ext->timeout);
			RCU_INIT_POINTER(timeout_ext->timeout, NULL);
		}
	}
	rcu_read_unlock();
#endif
}

=======
>>>>>>> upstream/android-13
static void xt_ct_tg_destroy(const struct xt_tgdtor_param *par,
			     struct xt_ct_target_info_v1 *info)
{
	struct nf_conn *ct = info->ct;
	struct nf_conn_help *help;

	if (ct) {
		help = nfct_help(ct);
		if (help)
			nf_conntrack_helper_put(help->helper);

		nf_ct_netns_put(par->net, par->family);

<<<<<<< HEAD
		xt_ct_destroy_timeout(ct);
=======
		nf_ct_destroy_timeout(ct);
>>>>>>> upstream/android-13
		nf_ct_put(info->ct);
	}
}

static void xt_ct_tg_destroy_v0(const struct xt_tgdtor_param *par)
{
	struct xt_ct_target_info *info = par->targinfo;
	struct xt_ct_target_info_v1 info_v1 = {
		.flags 		= info->flags,
		.zone		= info->zone,
		.ct_events	= info->ct_events,
		.exp_events	= info->exp_events,
		.ct		= info->ct,
	};
	memcpy(info_v1.helper, info->helper, sizeof(info->helper));

	xt_ct_tg_destroy(par, &info_v1);
}

static void xt_ct_tg_destroy_v1(const struct xt_tgdtor_param *par)
{
	xt_ct_tg_destroy(par, par->targinfo);
}

static struct xt_target xt_ct_tg_reg[] __read_mostly = {
	{
		.name		= "CT",
		.family		= NFPROTO_UNSPEC,
		.targetsize	= sizeof(struct xt_ct_target_info),
		.usersize	= offsetof(struct xt_ct_target_info, ct),
		.checkentry	= xt_ct_tg_check_v0,
		.destroy	= xt_ct_tg_destroy_v0,
		.target		= xt_ct_target_v0,
		.table		= "raw",
		.me		= THIS_MODULE,
	},
	{
		.name		= "CT",
		.family		= NFPROTO_UNSPEC,
		.revision	= 1,
		.targetsize	= sizeof(struct xt_ct_target_info_v1),
		.usersize	= offsetof(struct xt_ct_target_info, ct),
		.checkentry	= xt_ct_tg_check_v1,
		.destroy	= xt_ct_tg_destroy_v1,
		.target		= xt_ct_target_v1,
		.table		= "raw",
		.me		= THIS_MODULE,
	},
	{
		.name		= "CT",
		.family		= NFPROTO_UNSPEC,
		.revision	= 2,
		.targetsize	= sizeof(struct xt_ct_target_info_v1),
		.usersize	= offsetof(struct xt_ct_target_info, ct),
		.checkentry	= xt_ct_tg_check_v2,
		.destroy	= xt_ct_tg_destroy_v1,
		.target		= xt_ct_target_v1,
		.table		= "raw",
		.me		= THIS_MODULE,
	},
};

static unsigned int
notrack_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	/* Previously seen (loopback)? Ignore. */
	if (skb->_nfct != 0)
		return XT_CONTINUE;

	nf_ct_set(skb, NULL, IP_CT_UNTRACKED);

	return XT_CONTINUE;
}

<<<<<<< HEAD
static int notrack_chk(const struct xt_tgchk_param *par)
{
	if (!par->net->xt.notrack_deprecated_warning) {
		pr_info("netfilter: NOTRACK target is deprecated, "
			"use CT instead or upgrade iptables\n");
		par->net->xt.notrack_deprecated_warning = true;
	}
	return 0;
}

=======
>>>>>>> upstream/android-13
static struct xt_target notrack_tg_reg __read_mostly = {
	.name		= "NOTRACK",
	.revision	= 0,
	.family		= NFPROTO_UNSPEC,
<<<<<<< HEAD
	.checkentry	= notrack_chk,
=======
>>>>>>> upstream/android-13
	.target		= notrack_tg,
	.table		= "raw",
	.me		= THIS_MODULE,
};

static int __init xt_ct_tg_init(void)
{
	int ret;

	ret = xt_register_target(&notrack_tg_reg);
	if (ret < 0)
		return ret;

	ret = xt_register_targets(xt_ct_tg_reg, ARRAY_SIZE(xt_ct_tg_reg));
	if (ret < 0) {
		xt_unregister_target(&notrack_tg_reg);
		return ret;
	}
	return 0;
}

static void __exit xt_ct_tg_exit(void)
{
	xt_unregister_targets(xt_ct_tg_reg, ARRAY_SIZE(xt_ct_tg_reg));
	xt_unregister_target(&notrack_tg_reg);
}

module_init(xt_ct_tg_init);
module_exit(xt_ct_tg_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Xtables: connection tracking target");
MODULE_ALIAS("ipt_CT");
MODULE_ALIAS("ip6t_CT");
MODULE_ALIAS("ipt_NOTRACK");
MODULE_ALIAS("ip6t_NOTRACK");
