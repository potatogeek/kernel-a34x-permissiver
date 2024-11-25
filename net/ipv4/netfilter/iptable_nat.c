<<<<<<< HEAD
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2011 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2011 Patrick McHardy <kaber@trash.net>
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/ip.h>
#include <net/ip.h>

#include <net/netfilter/nf_nat.h>
<<<<<<< HEAD
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_l3proto.h>

static int __net_init iptable_nat_table_init(struct net *net);
=======

struct iptable_nat_pernet {
	struct nf_hook_ops *nf_nat_ops;
};

static unsigned int iptable_nat_net_id __read_mostly;
>>>>>>> upstream/android-13

static const struct xt_table nf_nat_ipv4_table = {
	.name		= "nat",
	.valid_hooks	= (1 << NF_INET_PRE_ROUTING) |
			  (1 << NF_INET_POST_ROUTING) |
			  (1 << NF_INET_LOCAL_OUT) |
			  (1 << NF_INET_LOCAL_IN),
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV4,
<<<<<<< HEAD
	.table_init	= iptable_nat_table_init,
=======
>>>>>>> upstream/android-13
};

static unsigned int iptable_nat_do_chain(void *priv,
					 struct sk_buff *skb,
					 const struct nf_hook_state *state)
{
<<<<<<< HEAD
	return ipt_do_table(skb, state, state->net->ipv4.nat_table);
=======
	return ipt_do_table(skb, state, priv);
>>>>>>> upstream/android-13
}

static const struct nf_hook_ops nf_nat_ipv4_ops[] = {
	{
		.hook		= iptable_nat_do_chain,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP_PRI_NAT_DST,
	},
	{
		.hook		= iptable_nat_do_chain,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_POST_ROUTING,
		.priority	= NF_IP_PRI_NAT_SRC,
	},
	{
		.hook		= iptable_nat_do_chain,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_LOCAL_OUT,
		.priority	= NF_IP_PRI_NAT_DST,
	},
	{
		.hook		= iptable_nat_do_chain,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_LOCAL_IN,
		.priority	= NF_IP_PRI_NAT_SRC,
	},
};

static int ipt_nat_register_lookups(struct net *net)
{
<<<<<<< HEAD
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv4_ops); i++) {
		ret = nf_nat_l3proto_ipv4_register_fn(net, &nf_nat_ipv4_ops[i]);
		if (ret) {
			while (i)
				nf_nat_l3proto_ipv4_unregister_fn(net, &nf_nat_ipv4_ops[--i]);

=======
	struct iptable_nat_pernet *xt_nat_net;
	struct nf_hook_ops *ops;
	struct xt_table *table;
	int i, ret;

	xt_nat_net = net_generic(net, iptable_nat_net_id);
	table = xt_find_table(net, NFPROTO_IPV4, "nat");
	if (WARN_ON_ONCE(!table))
		return -ENOENT;

	ops = kmemdup(nf_nat_ipv4_ops, sizeof(nf_nat_ipv4_ops), GFP_KERNEL);
	if (!ops)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv4_ops); i++) {
		ops[i].priv = table;
		ret = nf_nat_ipv4_register_fn(net, &ops[i]);
		if (ret) {
			while (i)
				nf_nat_ipv4_unregister_fn(net, &ops[--i]);

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

static void ipt_nat_unregister_lookups(struct net *net)
{
<<<<<<< HEAD
	int i;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv4_ops); i++)
		nf_nat_l3proto_ipv4_unregister_fn(net, &nf_nat_ipv4_ops[i]);
}

static int __net_init iptable_nat_table_init(struct net *net)
=======
	struct iptable_nat_pernet *xt_nat_net = net_generic(net, iptable_nat_net_id);
	struct nf_hook_ops *ops = xt_nat_net->nf_nat_ops;
	int i;

	if (!ops)
		return;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv4_ops); i++)
		nf_nat_ipv4_unregister_fn(net, &ops[i]);

	kfree(ops);
}

static int iptable_nat_table_init(struct net *net)
>>>>>>> upstream/android-13
{
	struct ipt_replace *repl;
	int ret;

<<<<<<< HEAD
	if (net->ipv4.nat_table)
		return 0;

	repl = ipt_alloc_initial_table(&nf_nat_ipv4_table);
	if (repl == NULL)
		return -ENOMEM;
	ret = ipt_register_table(net, &nf_nat_ipv4_table, repl,
				 NULL, &net->ipv4.nat_table);
=======
	repl = ipt_alloc_initial_table(&nf_nat_ipv4_table);
	if (repl == NULL)
		return -ENOMEM;

	ret = ipt_register_table(net, &nf_nat_ipv4_table, repl, NULL);
>>>>>>> upstream/android-13
	if (ret < 0) {
		kfree(repl);
		return ret;
	}

	ret = ipt_nat_register_lookups(net);
<<<<<<< HEAD
	if (ret < 0) {
		ipt_unregister_table(net, net->ipv4.nat_table, NULL);
		net->ipv4.nat_table = NULL;
	}
=======
	if (ret < 0)
		ipt_unregister_table_exit(net, "nat");
>>>>>>> upstream/android-13

	kfree(repl);
	return ret;
}

<<<<<<< HEAD
static void __net_exit iptable_nat_net_exit(struct net *net)
{
	if (!net->ipv4.nat_table)
		return;
	ipt_nat_unregister_lookups(net);
	ipt_unregister_table(net, net->ipv4.nat_table, NULL);
	net->ipv4.nat_table = NULL;
}

static struct pernet_operations iptable_nat_net_ops = {
	.exit	= iptable_nat_net_exit,
=======
static void __net_exit iptable_nat_net_pre_exit(struct net *net)
{
	ipt_nat_unregister_lookups(net);
}

static void __net_exit iptable_nat_net_exit(struct net *net)
{
	ipt_unregister_table_exit(net, "nat");
}

static struct pernet_operations iptable_nat_net_ops = {
	.pre_exit = iptable_nat_net_pre_exit,
	.exit	= iptable_nat_net_exit,
	.id	= &iptable_nat_net_id,
	.size	= sizeof(struct iptable_nat_pernet),
>>>>>>> upstream/android-13
};

static int __init iptable_nat_init(void)
{
<<<<<<< HEAD
	int ret = register_pernet_subsys(&iptable_nat_net_ops);

	if (ret)
		return ret;

	ret = iptable_nat_table_init(&init_net);
	if (ret)
		unregister_pernet_subsys(&iptable_nat_net_ops);
=======
	int ret = xt_register_template(&nf_nat_ipv4_table,
				       iptable_nat_table_init);

	if (ret < 0)
		return ret;

	ret = register_pernet_subsys(&iptable_nat_net_ops);
	if (ret < 0) {
		xt_unregister_template(&nf_nat_ipv4_table);
		return ret;
	}

>>>>>>> upstream/android-13
	return ret;
}

static void __exit iptable_nat_exit(void)
{
	unregister_pernet_subsys(&iptable_nat_net_ops);
<<<<<<< HEAD
=======
	xt_unregister_template(&nf_nat_ipv4_table);
>>>>>>> upstream/android-13
}

module_init(iptable_nat_init);
module_exit(iptable_nat_exit);

MODULE_LICENSE("GPL");
