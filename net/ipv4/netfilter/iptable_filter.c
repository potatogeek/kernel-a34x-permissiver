<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * This is the 1999 rewrite of IP Firewalling, aiming for kernel 2.3.x.
 *
 * Copyright (C) 1999 Paul `Rusty' Russell & Michael J. Neuling
 * Copyright (C) 2000-2004 Netfilter Core Team <coreteam@netfilter.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/slab.h>
#include <net/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("iptables filter table");

#define FILTER_VALID_HOOKS ((1 << NF_INET_LOCAL_IN) | \
			    (1 << NF_INET_FORWARD) | \
			    (1 << NF_INET_LOCAL_OUT))
<<<<<<< HEAD
static int __net_init iptable_filter_table_init(struct net *net);
=======
>>>>>>> upstream/android-13

static const struct xt_table packet_filter = {
	.name		= "filter",
	.valid_hooks	= FILTER_VALID_HOOKS,
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV4,
	.priority	= NF_IP_PRI_FILTER,
<<<<<<< HEAD
	.table_init	= iptable_filter_table_init,
=======
>>>>>>> upstream/android-13
};

static unsigned int
iptable_filter_hook(void *priv, struct sk_buff *skb,
		    const struct nf_hook_state *state)
{
<<<<<<< HEAD
	return ipt_do_table(skb, state, state->net->ipv4.iptable_filter);
=======
	return ipt_do_table(skb, state, priv);
>>>>>>> upstream/android-13
}

static struct nf_hook_ops *filter_ops __read_mostly;

/* Default to forward because I got too much mail already. */
static bool forward __read_mostly = true;
module_param(forward, bool, 0000);

<<<<<<< HEAD
static int __net_init iptable_filter_table_init(struct net *net)
=======
static int iptable_filter_table_init(struct net *net)
>>>>>>> upstream/android-13
{
	struct ipt_replace *repl;
	int err;

<<<<<<< HEAD
	if (net->ipv4.iptable_filter)
		return 0;

=======
>>>>>>> upstream/android-13
	repl = ipt_alloc_initial_table(&packet_filter);
	if (repl == NULL)
		return -ENOMEM;
	/* Entry 1 is the FORWARD hook */
	((struct ipt_standard *)repl->entries)[1].target.verdict =
		forward ? -NF_ACCEPT - 1 : -NF_DROP - 1;

<<<<<<< HEAD
	err = ipt_register_table(net, &packet_filter, repl, filter_ops,
				 &net->ipv4.iptable_filter);
=======
	err = ipt_register_table(net, &packet_filter, repl, filter_ops);
>>>>>>> upstream/android-13
	kfree(repl);
	return err;
}

static int __net_init iptable_filter_net_init(struct net *net)
{
<<<<<<< HEAD
	if (net == &init_net || !forward)
=======
	if (!forward)
>>>>>>> upstream/android-13
		return iptable_filter_table_init(net);

	return 0;
}

<<<<<<< HEAD
static void __net_exit iptable_filter_net_exit(struct net *net)
{
	if (!net->ipv4.iptable_filter)
		return;
	ipt_unregister_table(net, net->ipv4.iptable_filter, filter_ops);
	net->ipv4.iptable_filter = NULL;
=======
static void __net_exit iptable_filter_net_pre_exit(struct net *net)
{
	ipt_unregister_table_pre_exit(net, "filter");
}

static void __net_exit iptable_filter_net_exit(struct net *net)
{
	ipt_unregister_table_exit(net, "filter");
>>>>>>> upstream/android-13
}

static struct pernet_operations iptable_filter_net_ops = {
	.init = iptable_filter_net_init,
<<<<<<< HEAD
=======
	.pre_exit = iptable_filter_net_pre_exit,
>>>>>>> upstream/android-13
	.exit = iptable_filter_net_exit,
};

static int __init iptable_filter_init(void)
{
<<<<<<< HEAD
	int ret;

	filter_ops = xt_hook_ops_alloc(&packet_filter, iptable_filter_hook);
	if (IS_ERR(filter_ops))
		return PTR_ERR(filter_ops);

	ret = register_pernet_subsys(&iptable_filter_net_ops);
	if (ret < 0)
		kfree(filter_ops);

	return ret;
=======
	int ret = xt_register_template(&packet_filter,
				       iptable_filter_table_init);

	if (ret < 0)
		return ret;

	filter_ops = xt_hook_ops_alloc(&packet_filter, iptable_filter_hook);
	if (IS_ERR(filter_ops)) {
		xt_unregister_template(&packet_filter);
		return PTR_ERR(filter_ops);
	}

	ret = register_pernet_subsys(&iptable_filter_net_ops);
	if (ret < 0) {
		xt_unregister_template(&packet_filter);
		kfree(filter_ops);
		return ret;
	}

	return 0;
>>>>>>> upstream/android-13
}

static void __exit iptable_filter_fini(void)
{
	unregister_pernet_subsys(&iptable_filter_net_ops);
<<<<<<< HEAD
=======
	xt_unregister_template(&packet_filter);
>>>>>>> upstream/android-13
	kfree(filter_ops);
}

module_init(iptable_filter_init);
module_exit(iptable_filter_fini);
