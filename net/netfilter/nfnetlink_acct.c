<<<<<<< HEAD
/*
 * (C) 2011 Pablo Neira Ayuso <pablo@netfilter.org>
 * (C) 2011 Intra2net AG <http://www.intra2net.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation (or any later at your option).
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) 2011 Pablo Neira Ayuso <pablo@netfilter.org>
 * (C) 2011 Intra2net AG <https://www.intra2net.com>
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/atomic.h>
#include <linux/refcount.h>
#include <linux/netlink.h>
#include <linux/rculist.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <net/netlink.h>
#include <net/sock.h>
<<<<<<< HEAD
=======
#include <net/netns/generic.h>
>>>>>>> upstream/android-13

#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_acct.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo Neira Ayuso <pablo@netfilter.org>");
MODULE_DESCRIPTION("nfacct: Extended Netfilter accounting infrastructure");

struct nf_acct {
	atomic64_t		pkts;
	atomic64_t		bytes;
	unsigned long		flags;
	struct list_head	head;
	refcount_t		refcnt;
	char			name[NFACCT_NAME_MAX];
	struct rcu_head		rcu_head;
<<<<<<< HEAD
	char			data[0];
=======
	char			data[];
>>>>>>> upstream/android-13
};

struct nfacct_filter {
	u32 value;
	u32 mask;
};

<<<<<<< HEAD
#define NFACCT_F_QUOTA (NFACCT_F_QUOTA_PKTS | NFACCT_F_QUOTA_BYTES)
#define NFACCT_OVERQUOTA_BIT	2	/* NFACCT_F_OVERQUOTA */

static int nfnl_acct_new(struct net *net, struct sock *nfnl,
			 struct sk_buff *skb, const struct nlmsghdr *nlh,
			 const struct nlattr * const tb[],
			 struct netlink_ext_ack *extack)
{
	struct nf_acct *nfacct, *matching = NULL;
	char *acct_name;
	unsigned int size = 0;
=======
struct nfnl_acct_net {
	struct list_head        nfnl_acct_list;
};

static unsigned int nfnl_acct_net_id __read_mostly;

static inline struct nfnl_acct_net *nfnl_acct_pernet(struct net *net)
{
	return net_generic(net, nfnl_acct_net_id);
}

#define NFACCT_F_QUOTA (NFACCT_F_QUOTA_PKTS | NFACCT_F_QUOTA_BYTES)
#define NFACCT_OVERQUOTA_BIT	2	/* NFACCT_F_OVERQUOTA */

static int nfnl_acct_new(struct sk_buff *skb, const struct nfnl_info *info,
			 const struct nlattr * const tb[])
{
	struct nfnl_acct_net *nfnl_acct_net = nfnl_acct_pernet(info->net);
	struct nf_acct *nfacct, *matching = NULL;
	unsigned int size = 0;
	char *acct_name;
>>>>>>> upstream/android-13
	u32 flags = 0;

	if (!tb[NFACCT_NAME])
		return -EINVAL;

	acct_name = nla_data(tb[NFACCT_NAME]);
	if (strlen(acct_name) == 0)
		return -EINVAL;

<<<<<<< HEAD
	list_for_each_entry(nfacct, &net->nfnl_acct_list, head) {
		if (strncmp(nfacct->name, acct_name, NFACCT_NAME_MAX) != 0)
			continue;

                if (nlh->nlmsg_flags & NLM_F_EXCL)
=======
	list_for_each_entry(nfacct, &nfnl_acct_net->nfnl_acct_list, head) {
		if (strncmp(nfacct->name, acct_name, NFACCT_NAME_MAX) != 0)
			continue;

                if (info->nlh->nlmsg_flags & NLM_F_EXCL)
>>>>>>> upstream/android-13
			return -EEXIST;

		matching = nfacct;
		break;
        }

	if (matching) {
<<<<<<< HEAD
		if (nlh->nlmsg_flags & NLM_F_REPLACE) {
=======
		if (info->nlh->nlmsg_flags & NLM_F_REPLACE) {
>>>>>>> upstream/android-13
			/* reset counters if you request a replacement. */
			atomic64_set(&matching->pkts, 0);
			atomic64_set(&matching->bytes, 0);
			smp_mb__before_atomic();
			/* reset overquota flag if quota is enabled. */
			if ((matching->flags & NFACCT_F_QUOTA))
				clear_bit(NFACCT_OVERQUOTA_BIT,
					  &matching->flags);
			return 0;
		}
		return -EBUSY;
	}

	if (tb[NFACCT_FLAGS]) {
		flags = ntohl(nla_get_be32(tb[NFACCT_FLAGS]));
		if (flags & ~NFACCT_F_QUOTA)
			return -EOPNOTSUPP;
		if ((flags & NFACCT_F_QUOTA) == NFACCT_F_QUOTA)
			return -EINVAL;
		if (flags & NFACCT_F_OVERQUOTA)
			return -EINVAL;
		if ((flags & NFACCT_F_QUOTA) && !tb[NFACCT_QUOTA])
			return -EINVAL;

		size += sizeof(u64);
	}

	nfacct = kzalloc(sizeof(struct nf_acct) + size, GFP_KERNEL);
	if (nfacct == NULL)
		return -ENOMEM;

	if (flags & NFACCT_F_QUOTA) {
		u64 *quota = (u64 *)nfacct->data;

		*quota = be64_to_cpu(nla_get_be64(tb[NFACCT_QUOTA]));
		nfacct->flags = flags;
	}

<<<<<<< HEAD
	nla_strlcpy(nfacct->name, tb[NFACCT_NAME], NFACCT_NAME_MAX);
=======
	nla_strscpy(nfacct->name, tb[NFACCT_NAME], NFACCT_NAME_MAX);
>>>>>>> upstream/android-13

	if (tb[NFACCT_BYTES]) {
		atomic64_set(&nfacct->bytes,
			     be64_to_cpu(nla_get_be64(tb[NFACCT_BYTES])));
	}
	if (tb[NFACCT_PKTS]) {
		atomic64_set(&nfacct->pkts,
			     be64_to_cpu(nla_get_be64(tb[NFACCT_PKTS])));
	}
	refcount_set(&nfacct->refcnt, 1);
<<<<<<< HEAD
	list_add_tail_rcu(&nfacct->head, &net->nfnl_acct_list);
=======
	list_add_tail_rcu(&nfacct->head, &nfnl_acct_net->nfnl_acct_list);
>>>>>>> upstream/android-13
	return 0;
}

static int
nfnl_acct_fill_info(struct sk_buff *skb, u32 portid, u32 seq, u32 type,
		   int event, struct nf_acct *acct)
{
	struct nlmsghdr *nlh;
<<<<<<< HEAD
	struct nfgenmsg *nfmsg;
=======
>>>>>>> upstream/android-13
	unsigned int flags = portid ? NLM_F_MULTI : 0;
	u64 pkts, bytes;
	u32 old_flags;

	event = nfnl_msg_type(NFNL_SUBSYS_ACCT, event);
<<<<<<< HEAD
	nlh = nlmsg_put(skb, portid, seq, event, sizeof(*nfmsg), flags);
	if (nlh == NULL)
		goto nlmsg_failure;

	nfmsg = nlmsg_data(nlh);
	nfmsg->nfgen_family = AF_UNSPEC;
	nfmsg->version = NFNETLINK_V0;
	nfmsg->res_id = 0;

=======
	nlh = nfnl_msg_put(skb, portid, seq, event, flags, AF_UNSPEC,
			   NFNETLINK_V0, 0);
	if (!nlh)
		goto nlmsg_failure;

>>>>>>> upstream/android-13
	if (nla_put_string(skb, NFACCT_NAME, acct->name))
		goto nla_put_failure;

	old_flags = acct->flags;
	if (type == NFNL_MSG_ACCT_GET_CTRZERO) {
		pkts = atomic64_xchg(&acct->pkts, 0);
		bytes = atomic64_xchg(&acct->bytes, 0);
		smp_mb__before_atomic();
		if (acct->flags & NFACCT_F_QUOTA)
			clear_bit(NFACCT_OVERQUOTA_BIT, &acct->flags);
	} else {
		pkts = atomic64_read(&acct->pkts);
		bytes = atomic64_read(&acct->bytes);
	}
	if (nla_put_be64(skb, NFACCT_PKTS, cpu_to_be64(pkts),
			 NFACCT_PAD) ||
	    nla_put_be64(skb, NFACCT_BYTES, cpu_to_be64(bytes),
			 NFACCT_PAD) ||
	    nla_put_be32(skb, NFACCT_USE, htonl(refcount_read(&acct->refcnt))))
		goto nla_put_failure;
	if (acct->flags & NFACCT_F_QUOTA) {
		u64 *quota = (u64 *)acct->data;

		if (nla_put_be32(skb, NFACCT_FLAGS, htonl(old_flags)) ||
		    nla_put_be64(skb, NFACCT_QUOTA, cpu_to_be64(*quota),
				 NFACCT_PAD))
			goto nla_put_failure;
	}
	nlmsg_end(skb, nlh);
	return skb->len;

nlmsg_failure:
nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -1;
}

static int
nfnl_acct_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
<<<<<<< HEAD
=======
	struct nfnl_acct_net *nfnl_acct_net = nfnl_acct_pernet(net);
>>>>>>> upstream/android-13
	struct nf_acct *cur, *last;
	const struct nfacct_filter *filter = cb->data;

	if (cb->args[2])
		return 0;

	last = (struct nf_acct *)cb->args[1];
	if (cb->args[1])
		cb->args[1] = 0;

	rcu_read_lock();
<<<<<<< HEAD
	list_for_each_entry_rcu(cur, &net->nfnl_acct_list, head) {
=======
	list_for_each_entry_rcu(cur, &nfnl_acct_net->nfnl_acct_list, head) {
>>>>>>> upstream/android-13
		if (last) {
			if (cur != last)
				continue;

			last = NULL;
		}

		if (filter && (cur->flags & filter->mask) != filter->value)
			continue;

		if (nfnl_acct_fill_info(skb, NETLINK_CB(cb->skb).portid,
				       cb->nlh->nlmsg_seq,
				       NFNL_MSG_TYPE(cb->nlh->nlmsg_type),
				       NFNL_MSG_ACCT_NEW, cur) < 0) {
			cb->args[1] = (unsigned long)cur;
			break;
		}
	}
	if (!cb->args[1])
		cb->args[2] = 1;
	rcu_read_unlock();
	return skb->len;
}

static int nfnl_acct_done(struct netlink_callback *cb)
{
	kfree(cb->data);
	return 0;
}

static const struct nla_policy filter_policy[NFACCT_FILTER_MAX + 1] = {
	[NFACCT_FILTER_MASK]	= { .type = NLA_U32 },
	[NFACCT_FILTER_VALUE]	= { .type = NLA_U32 },
};

static int nfnl_acct_start(struct netlink_callback *cb)
{
	const struct nlattr *const attr = cb->data;
	struct nlattr *tb[NFACCT_FILTER_MAX + 1];
	struct nfacct_filter *filter;
	int err;

	if (!attr)
		return 0;

<<<<<<< HEAD
	err = nla_parse_nested(tb, NFACCT_FILTER_MAX, attr, filter_policy,
			       NULL);
=======
	err = nla_parse_nested_deprecated(tb, NFACCT_FILTER_MAX, attr,
					  filter_policy, NULL);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	if (!tb[NFACCT_FILTER_MASK] || !tb[NFACCT_FILTER_VALUE])
		return -EINVAL;

	filter = kzalloc(sizeof(struct nfacct_filter), GFP_KERNEL);
	if (!filter)
		return -ENOMEM;

	filter->mask = ntohl(nla_get_be32(tb[NFACCT_FILTER_MASK]));
	filter->value = ntohl(nla_get_be32(tb[NFACCT_FILTER_VALUE]));
	cb->data = filter;

	return 0;
}

<<<<<<< HEAD
static int nfnl_acct_get(struct net *net, struct sock *nfnl,
			 struct sk_buff *skb, const struct nlmsghdr *nlh,
			 const struct nlattr * const tb[],
			 struct netlink_ext_ack *extack)
{
=======
static int nfnl_acct_get(struct sk_buff *skb, const struct nfnl_info *info,
			 const struct nlattr * const tb[])
{
	struct nfnl_acct_net *nfnl_acct_net = nfnl_acct_pernet(info->net);
>>>>>>> upstream/android-13
	int ret = -ENOENT;
	struct nf_acct *cur;
	char *acct_name;

<<<<<<< HEAD
	if (nlh->nlmsg_flags & NLM_F_DUMP) {
=======
	if (info->nlh->nlmsg_flags & NLM_F_DUMP) {
>>>>>>> upstream/android-13
		struct netlink_dump_control c = {
			.dump = nfnl_acct_dump,
			.start = nfnl_acct_start,
			.done = nfnl_acct_done,
			.data = (void *)tb[NFACCT_FILTER],
		};

<<<<<<< HEAD
		return netlink_dump_start(nfnl, skb, nlh, &c);
=======
		return netlink_dump_start(info->sk, skb, info->nlh, &c);
>>>>>>> upstream/android-13
	}

	if (!tb[NFACCT_NAME])
		return -EINVAL;
	acct_name = nla_data(tb[NFACCT_NAME]);

<<<<<<< HEAD
	list_for_each_entry(cur, &net->nfnl_acct_list, head) {
=======
	list_for_each_entry(cur, &nfnl_acct_net->nfnl_acct_list, head) {
>>>>>>> upstream/android-13
		struct sk_buff *skb2;

		if (strncmp(cur->name, acct_name, NFACCT_NAME_MAX)!= 0)
			continue;

		skb2 = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
		if (skb2 == NULL) {
			ret = -ENOMEM;
			break;
		}

		ret = nfnl_acct_fill_info(skb2, NETLINK_CB(skb).portid,
<<<<<<< HEAD
					 nlh->nlmsg_seq,
					 NFNL_MSG_TYPE(nlh->nlmsg_type),
					 NFNL_MSG_ACCT_NEW, cur);
=======
					  info->nlh->nlmsg_seq,
					  NFNL_MSG_TYPE(info->nlh->nlmsg_type),
					  NFNL_MSG_ACCT_NEW, cur);
>>>>>>> upstream/android-13
		if (ret <= 0) {
			kfree_skb(skb2);
			break;
		}
<<<<<<< HEAD
		ret = netlink_unicast(nfnl, skb2, NETLINK_CB(skb).portid,
					MSG_DONTWAIT);
		if (ret > 0)
			ret = 0;

		/* this avoids a loop in nfnetlink. */
		return ret == -EAGAIN ? -ENOBUFS : ret;
	}
=======

		ret = nfnetlink_unicast(skb2, info->net, NETLINK_CB(skb).portid);
		break;
	}

>>>>>>> upstream/android-13
	return ret;
}

/* try to delete object, fail if it is still in use. */
static int nfnl_acct_try_del(struct nf_acct *cur)
{
	int ret = 0;

	/* We want to avoid races with nfnl_acct_put. So only when the current
	 * refcnt is 1, we decrease it to 0.
	 */
	if (refcount_dec_if_one(&cur->refcnt)) {
		/* We are protected by nfnl mutex. */
		list_del_rcu(&cur->head);
		kfree_rcu(cur, rcu_head);
	} else {
		ret = -EBUSY;
	}
	return ret;
}

<<<<<<< HEAD
static int nfnl_acct_del(struct net *net, struct sock *nfnl,
			 struct sk_buff *skb, const struct nlmsghdr *nlh,
			 const struct nlattr * const tb[],
			 struct netlink_ext_ack *extack)
{
=======
static int nfnl_acct_del(struct sk_buff *skb, const struct nfnl_info *info,
			 const struct nlattr * const tb[])
{
	struct nfnl_acct_net *nfnl_acct_net = nfnl_acct_pernet(info->net);
>>>>>>> upstream/android-13
	struct nf_acct *cur, *tmp;
	int ret = -ENOENT;
	char *acct_name;

	if (!tb[NFACCT_NAME]) {
<<<<<<< HEAD
		list_for_each_entry_safe(cur, tmp, &net->nfnl_acct_list, head)
=======
		list_for_each_entry_safe(cur, tmp, &nfnl_acct_net->nfnl_acct_list, head)
>>>>>>> upstream/android-13
			nfnl_acct_try_del(cur);

		return 0;
	}
	acct_name = nla_data(tb[NFACCT_NAME]);

<<<<<<< HEAD
	list_for_each_entry(cur, &net->nfnl_acct_list, head) {
=======
	list_for_each_entry(cur, &nfnl_acct_net->nfnl_acct_list, head) {
>>>>>>> upstream/android-13
		if (strncmp(cur->name, acct_name, NFACCT_NAME_MAX) != 0)
			continue;

		ret = nfnl_acct_try_del(cur);
		if (ret < 0)
			return ret;

		break;
	}
	return ret;
}

static const struct nla_policy nfnl_acct_policy[NFACCT_MAX+1] = {
	[NFACCT_NAME] = { .type = NLA_NUL_STRING, .len = NFACCT_NAME_MAX-1 },
	[NFACCT_BYTES] = { .type = NLA_U64 },
	[NFACCT_PKTS] = { .type = NLA_U64 },
	[NFACCT_FLAGS] = { .type = NLA_U32 },
	[NFACCT_QUOTA] = { .type = NLA_U64 },
	[NFACCT_FILTER] = {.type = NLA_NESTED },
};

static const struct nfnl_callback nfnl_acct_cb[NFNL_MSG_ACCT_MAX] = {
<<<<<<< HEAD
	[NFNL_MSG_ACCT_NEW]		= { .call = nfnl_acct_new,
					    .attr_count = NFACCT_MAX,
					    .policy = nfnl_acct_policy },
	[NFNL_MSG_ACCT_GET] 		= { .call = nfnl_acct_get,
					    .attr_count = NFACCT_MAX,
					    .policy = nfnl_acct_policy },
	[NFNL_MSG_ACCT_GET_CTRZERO] 	= { .call = nfnl_acct_get,
					    .attr_count = NFACCT_MAX,
					    .policy = nfnl_acct_policy },
	[NFNL_MSG_ACCT_DEL]		= { .call = nfnl_acct_del,
					    .attr_count = NFACCT_MAX,
					    .policy = nfnl_acct_policy },
=======
	[NFNL_MSG_ACCT_NEW] = {
		.call		= nfnl_acct_new,
		.type		= NFNL_CB_MUTEX,
		.attr_count	= NFACCT_MAX,
		.policy		= nfnl_acct_policy
	},
	[NFNL_MSG_ACCT_GET] = {
		.call		= nfnl_acct_get,
		.type		= NFNL_CB_MUTEX,
		.attr_count	= NFACCT_MAX,
		.policy		= nfnl_acct_policy
	},
	[NFNL_MSG_ACCT_GET_CTRZERO] = {
		.call		= nfnl_acct_get,
		.type		= NFNL_CB_MUTEX,
		.attr_count	= NFACCT_MAX,
		.policy		= nfnl_acct_policy
	},
	[NFNL_MSG_ACCT_DEL] = {
		.call		= nfnl_acct_del,
		.type		= NFNL_CB_MUTEX,
		.attr_count	= NFACCT_MAX,
		.policy		= nfnl_acct_policy
	},
>>>>>>> upstream/android-13
};

static const struct nfnetlink_subsystem nfnl_acct_subsys = {
	.name				= "acct",
	.subsys_id			= NFNL_SUBSYS_ACCT,
	.cb_count			= NFNL_MSG_ACCT_MAX,
	.cb				= nfnl_acct_cb,
};

MODULE_ALIAS_NFNL_SUBSYS(NFNL_SUBSYS_ACCT);

struct nf_acct *nfnl_acct_find_get(struct net *net, const char *acct_name)
{
<<<<<<< HEAD
	struct nf_acct *cur, *acct = NULL;

	rcu_read_lock();
	list_for_each_entry_rcu(cur, &net->nfnl_acct_list, head) {
=======
	struct nfnl_acct_net *nfnl_acct_net = nfnl_acct_pernet(net);
	struct nf_acct *cur, *acct = NULL;

	rcu_read_lock();
	list_for_each_entry_rcu(cur, &nfnl_acct_net->nfnl_acct_list, head) {
>>>>>>> upstream/android-13
		if (strncmp(cur->name, acct_name, NFACCT_NAME_MAX)!= 0)
			continue;

		if (!try_module_get(THIS_MODULE))
			goto err;

		if (!refcount_inc_not_zero(&cur->refcnt)) {
			module_put(THIS_MODULE);
			goto err;
		}

		acct = cur;
		break;
	}
err:
	rcu_read_unlock();
	return acct;
}
EXPORT_SYMBOL_GPL(nfnl_acct_find_get);

void nfnl_acct_put(struct nf_acct *acct)
{
	if (refcount_dec_and_test(&acct->refcnt))
		kfree_rcu(acct, rcu_head);

	module_put(THIS_MODULE);
}
EXPORT_SYMBOL_GPL(nfnl_acct_put);

void nfnl_acct_update(const struct sk_buff *skb, struct nf_acct *nfacct)
{
	atomic64_inc(&nfacct->pkts);
	atomic64_add(skb->len, &nfacct->bytes);
}
EXPORT_SYMBOL_GPL(nfnl_acct_update);

static void nfnl_overquota_report(struct net *net, struct nf_acct *nfacct)
{
	int ret;
	struct sk_buff *skb;

	skb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_ATOMIC);
	if (skb == NULL)
		return;

	ret = nfnl_acct_fill_info(skb, 0, 0, NFNL_MSG_ACCT_OVERQUOTA, 0,
				  nfacct);
	if (ret <= 0) {
		kfree_skb(skb);
		return;
	}
<<<<<<< HEAD
	netlink_broadcast(net->nfnl, skb, 0, NFNLGRP_ACCT_QUOTA,
			  GFP_ATOMIC);
=======
	nfnetlink_broadcast(net, skb, 0, NFNLGRP_ACCT_QUOTA, GFP_ATOMIC);
>>>>>>> upstream/android-13
}

int nfnl_acct_overquota(struct net *net, struct nf_acct *nfacct)
{
	u64 now;
	u64 *quota;
	int ret = NFACCT_UNDERQUOTA;

	/* no place here if we don't have a quota */
	if (!(nfacct->flags & NFACCT_F_QUOTA))
		return NFACCT_NO_QUOTA;

	quota = (u64 *)nfacct->data;
	now = (nfacct->flags & NFACCT_F_QUOTA_PKTS) ?
	       atomic64_read(&nfacct->pkts) : atomic64_read(&nfacct->bytes);

	ret = now > *quota;

	if (now >= *quota &&
	    !test_and_set_bit(NFACCT_OVERQUOTA_BIT, &nfacct->flags)) {
		nfnl_overquota_report(net, nfacct);
	}

	return ret;
}
EXPORT_SYMBOL_GPL(nfnl_acct_overquota);

static int __net_init nfnl_acct_net_init(struct net *net)
{
<<<<<<< HEAD
	INIT_LIST_HEAD(&net->nfnl_acct_list);
=======
	INIT_LIST_HEAD(&nfnl_acct_pernet(net)->nfnl_acct_list);
>>>>>>> upstream/android-13

	return 0;
}

static void __net_exit nfnl_acct_net_exit(struct net *net)
{
<<<<<<< HEAD
	struct nf_acct *cur, *tmp;

	list_for_each_entry_safe(cur, tmp, &net->nfnl_acct_list, head) {
=======
	struct nfnl_acct_net *nfnl_acct_net = nfnl_acct_pernet(net);
	struct nf_acct *cur, *tmp;

	list_for_each_entry_safe(cur, tmp, &nfnl_acct_net->nfnl_acct_list, head) {
>>>>>>> upstream/android-13
		list_del_rcu(&cur->head);

		if (refcount_dec_and_test(&cur->refcnt))
			kfree_rcu(cur, rcu_head);
	}
}

static struct pernet_operations nfnl_acct_ops = {
        .init   = nfnl_acct_net_init,
        .exit   = nfnl_acct_net_exit,
<<<<<<< HEAD
=======
        .id     = &nfnl_acct_net_id,
        .size   = sizeof(struct nfnl_acct_net),
>>>>>>> upstream/android-13
};

static int __init nfnl_acct_init(void)
{
	int ret;

	ret = register_pernet_subsys(&nfnl_acct_ops);
	if (ret < 0) {
		pr_err("nfnl_acct_init: failed to register pernet ops\n");
		goto err_out;
	}

	ret = nfnetlink_subsys_register(&nfnl_acct_subsys);
	if (ret < 0) {
		pr_err("nfnl_acct_init: cannot register with nfnetlink.\n");
		goto cleanup_pernet;
	}
	return 0;

cleanup_pernet:
	unregister_pernet_subsys(&nfnl_acct_ops);
err_out:
	return ret;
}

static void __exit nfnl_acct_exit(void)
{
	nfnetlink_subsys_unregister(&nfnl_acct_subsys);
	unregister_pernet_subsys(&nfnl_acct_ops);
}

module_init(nfnl_acct_init);
module_exit(nfnl_acct_exit);
