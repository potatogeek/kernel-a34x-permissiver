<<<<<<< HEAD
/*
 * Copyright (c) 2011 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2011 Patrick McHardy <kaber@trash.net>
 *
>>>>>>> upstream/android-13
 * Based on Rusty Russell's IPv4 NAT code. Development of IPv6 NAT
 * funded by Astaro.
 */

#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/ipv6.h>
#include <net/ipv6.h>

#include <net/netfilter/nf_nat.h>
<<<<<<< HEAD
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_l3proto.h>

static int __net_init ip6table_nat_table_init(struct net *net);
=======

struct ip6table_nat_pernet {
	struct nf_hook_ops *nf_nat_ops;
};

static unsigned int ip6table_nat_net_id __read_mostly;
>>>>>>> upstream/android-13

static const struct xt_table nf_nat_ipv6_table = {
	.name		= "nat",
	.valid_hooks	= (1 << NF_INET_PRE_ROUTING) |
			  (1 << NF_INET_POST_ROUTING) |
			  (1 << NF_INET_LOCAL_OUT) |
			  (1 << NF_INET_LOCAL_IN),
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV6,
<<<<<<< HEAD
	.table_init	= ip6table_nat_table_init,
=======
>>>>>>> upstream/android-13
};

static unsigned int ip6table_nat_do_chain(void *priv,
					  struct sk_buff *skb,
					  const struct nf_hook_state *state)
{
<<<<<<< HEAD
	return ip6t_do_table(skb, state, state->net->ipv6.ip6table_nat);
=======
	return ip6t_do_table(skb, state, priv);
>>>>>>> upstream/android-13
}

static const struct nf_hook_ops nf_nat_ipv6_ops[] = {
	{
		.hook		= ip6table_nat_do_chain,
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP6_PRI_NAT_DST,
	},
	{
		.hook		= ip6table_nat_do_chain,
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_POST_ROUTING,
		.priority	= NF_IP6_PRI_NAT_SRC,
	},
	{
		.hook		= ip6table_nat_do_chain,
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_LOCAL_OUT,
		.priority	= NF_IP6_PRI_NAT_DST,
	},
	{
		.hook		= ip6table_nat_do_chain,
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_LOCAL_IN,
		.priority	= NF_IP6_PRI_NAT_SRC,
	},
};

static int ip6t_nat_register_lookups(struct net *net)
{
<<<<<<< HEAD
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv6_ops); i++) {
		ret = nf_nat_l3proto_ipv6_register_fn(net, &nf_nat_ipv6_ops[i]);
		if (ret) {
			while (i)
				nf_nat_l3proto_ipv6_unregister_fn(net, &nf_nat_ipv6_ops[--i]);

=======
	struct ip6table_nat_pernet *xt_nat_net;
	struct nf_hook_ops *ops;
	struct xt_table *table;
	int i, ret;

	table = xt_find_table(net, NFPROTO_IPV6, "nat");
	if (WARN_ON_ONCE(!table))
		return -ENOENT;

	xt_nat_net = net_generic(net, ip6table_nat_net_id);
	ops = kmemdup(nf_nat_ipv6_ops, sizeof(nf_nat_ipv6_ops), GFP_KERNEL);
	if (!ops)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv6_ops); i++) {
		ops[i].priv = table;
		ret = nf_nat_ipv6_register_fn(net, &ops[i]);
		if (ret) {
			while (i)
				nf_nat_ipv6_unregister_fn(net, &ops[--i]);

			kfree(ops);
>>>>>>> upstream/android-13
			return ret;
		}
	}

<<<<<<< HEAD
=======
	xt_nat_net->nf_nat_ops = ops;
>>>>>>> upstream/android-13
	return 0;
}

static void ip6t_nat_unregister_lookups(struct net *net)
{
<<<<<<< HEAD
	int i;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv6_ops); i++)
		nf_nat_l3proto_ipv6_unregister_fn(net, &nf_nat_ipv6_ops[i]);
}

static int __net_init ip6table_nat_table_init(struct net *net)
=======
	struct ip6table_nat_pernet *xt_nat_net = net_generic(net, ip6table_nat_net_id);
	struct nf_hook_ops *ops = xt_nat_net->nf_nat_ops;
	int i;

	if (!ops)
		return;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv6_ops); i++)
		nf_nat_ipv6_unregister_fn(net, &ops[i]);

	kfree(ops);
}

static int ip6table_nat_table_init(struct net *net)
>>>>>>> upstream/android-13
{
	struct ip6t_replace *repl;
	int ret;

<<<<<<< HEAD
	if (net->ipv6.ip6table_nat)
		return 0;

=======
>>>>>>> upstream/android-13
	repl = ip6t_alloc_initial_table(&nf_nat_ipv6_table);
	if (repl == NULL)
		return -ENOMEM;
	ret = ip6t_register_table(net, &nf_nat_ipv6_table, repl,
<<<<<<< HEAD
				  NULL, &net->ipv6.ip6table_nat);
=======
				  NULL);
>>>>>>> upstream/android-13
	if (ret < 0) {
		kfree(repl);
		return ret;
	}

	ret = ip6t_nat_register_lookups(net);
<<<<<<< HEAD
	if (ret < 0) {
		ip6t_unregister_table(net, net->ipv6.ip6table_nat, NULL);
		net->ipv6.ip6table_nat = NULL;
	}
=======
	if (ret < 0)
		ip6t_unregister_table_exit(net, "nat");

>>>>>>> upstream/android-13
	kfree(repl);
	return ret;
}

<<<<<<< HEAD
static void __net_exit ip6table_nat_net_exit(struct net *net)
{
	if (!net->ipv6.ip6table_nat)
		return;
	ip6t_nat_unregister_lookups(net);
	ip6t_unregister_table(net, net->ipv6.ip6table_nat, NULL);
	net->ipv6.ip6table_nat = NULL;
}

static struct pernet_operations ip6table_nat_net_ops = {
	.exit	= ip6table_nat_net_exit,
=======
static void __net_exit ip6table_nat_net_pre_exit(struct net *net)
{
	ip6t_nat_unregister_lookups(net);
}

static void __net_exit ip6table_nat_net_exit(struct net *net)
{
	ip6t_unregister_table_exit(net, "nat");
}

static struct pernet_operations ip6table_nat_net_ops = {
	.pre_exit = ip6table_nat_net_pre_exit,
	.exit	= ip6table_nat_net_exit,
	.id	= &ip6table_nat_net_id,
	.size	= sizeof(struct ip6table_nat_pernet),
>>>>>>> upstream/android-13
};

static int __init ip6table_nat_init(void)
{
<<<<<<< HEAD
	int ret = register_pernet_subsys(&ip6table_nat_net_ops);

	if (ret)
		return ret;

	ret = ip6table_nat_table_init(&init_net);
	if (ret)
		unregister_pernet_subsys(&ip6table_nat_net_ops);
=======
	int ret = xt_register_template(&nf_nat_ipv6_table,
				       ip6table_nat_table_init);

	if (ret < 0)
		return ret;

	ret = register_pernet_subsys(&ip6table_nat_net_ops);
	if (ret)
		xt_unregister_template(&nf_nat_ipv6_table);

>>>>>>> upstream/android-13
	return ret;
}

static void __exit ip6table_nat_exit(void)
{
	unregister_pernet_subsys(&ip6table_nat_net_ops);
<<<<<<< HEAD
=======
	xt_unregister_template(&nf_nat_ipv6_table);
>>>>>>> upstream/android-13
}

module_init(ip6table_nat_init);
module_exit(ip6table_nat_exit);

MODULE_LICENSE("GPL");
