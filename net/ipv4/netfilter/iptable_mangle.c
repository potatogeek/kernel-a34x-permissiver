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
=======
>>>>>>> upstream/android-13
 */
#include <linux/module.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <net/route.h>
#include <linux/ip.h>
#include <net/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("iptables mangle table");

#define MANGLE_VALID_HOOKS ((1 << NF_INET_PRE_ROUTING) | \
			    (1 << NF_INET_LOCAL_IN) | \
			    (1 << NF_INET_FORWARD) | \
			    (1 << NF_INET_LOCAL_OUT) | \
			    (1 << NF_INET_POST_ROUTING))

<<<<<<< HEAD
static int __net_init iptable_mangle_table_init(struct net *net);

=======
>>>>>>> upstream/android-13
static const struct xt_table packet_mangler = {
	.name		= "mangle",
	.valid_hooks	= MANGLE_VALID_HOOKS,
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV4,
	.priority	= NF_IP_PRI_MANGLE,
<<<<<<< HEAD
	.table_init	= iptable_mangle_table_init,
};

static unsigned int
ipt_mangle_out(struct sk_buff *skb, const struct nf_hook_state *state)
=======
};

static unsigned int
ipt_mangle_out(struct sk_buff *skb, const struct nf_hook_state *state, void *priv)
>>>>>>> upstream/android-13
{
	unsigned int ret;
	const struct iphdr *iph;
	u_int8_t tos;
	__be32 saddr, daddr;
	u_int32_t mark;
	int err;

	/* Save things which could affect route */
	mark = skb->mark;
	iph = ip_hdr(skb);
	saddr = iph->saddr;
	daddr = iph->daddr;
	tos = iph->tos;

<<<<<<< HEAD
	ret = ipt_do_table(skb, state, state->net->ipv4.iptable_mangle);
=======
	ret = ipt_do_table(skb, state, priv);
>>>>>>> upstream/android-13
	/* Reroute for ANY change. */
	if (ret != NF_DROP && ret != NF_STOLEN) {
		iph = ip_hdr(skb);

		if (iph->saddr != saddr ||
		    iph->daddr != daddr ||
		    skb->mark != mark ||
		    iph->tos != tos) {
			err = ip_route_me_harder(state->net, state->sk, skb, RTN_UNSPEC);
			if (err < 0)
				ret = NF_DROP_ERR(err);
		}
	}

	return ret;
}

/* The work comes in here from netfilter.c. */
static unsigned int
iptable_mangle_hook(void *priv,
		     struct sk_buff *skb,
		     const struct nf_hook_state *state)
{
	if (state->hook == NF_INET_LOCAL_OUT)
<<<<<<< HEAD
		return ipt_mangle_out(skb, state);
	return ipt_do_table(skb, state, state->net->ipv4.iptable_mangle);
}

static struct nf_hook_ops *mangle_ops __read_mostly;
static int __net_init iptable_mangle_table_init(struct net *net)
=======
		return ipt_mangle_out(skb, state, priv);
	return ipt_do_table(skb, state, priv);
}

static struct nf_hook_ops *mangle_ops __read_mostly;
static int iptable_mangle_table_init(struct net *net)
>>>>>>> upstream/android-13
{
	struct ipt_replace *repl;
	int ret;

<<<<<<< HEAD
	if (net->ipv4.iptable_mangle)
		return 0;

	repl = ipt_alloc_initial_table(&packet_mangler);
	if (repl == NULL)
		return -ENOMEM;
	ret = ipt_register_table(net, &packet_mangler, repl, mangle_ops,
				 &net->ipv4.iptable_mangle);
=======
	repl = ipt_alloc_initial_table(&packet_mangler);
	if (repl == NULL)
		return -ENOMEM;
	ret = ipt_register_table(net, &packet_mangler, repl, mangle_ops);
>>>>>>> upstream/android-13
	kfree(repl);
	return ret;
}

<<<<<<< HEAD
static void __net_exit iptable_mangle_net_exit(struct net *net)
{
	if (!net->ipv4.iptable_mangle)
		return;
	ipt_unregister_table(net, net->ipv4.iptable_mangle, mangle_ops);
	net->ipv4.iptable_mangle = NULL;
}

static struct pernet_operations iptable_mangle_net_ops = {
=======
static void __net_exit iptable_mangle_net_pre_exit(struct net *net)
{
	ipt_unregister_table_pre_exit(net, "mangle");
}

static void __net_exit iptable_mangle_net_exit(struct net *net)
{
	ipt_unregister_table_exit(net, "mangle");
}

static struct pernet_operations iptable_mangle_net_ops = {
	.pre_exit = iptable_mangle_net_pre_exit,
>>>>>>> upstream/android-13
	.exit = iptable_mangle_net_exit,
};

static int __init iptable_mangle_init(void)
{
<<<<<<< HEAD
	int ret;

	mangle_ops = xt_hook_ops_alloc(&packet_mangler, iptable_mangle_hook);
	if (IS_ERR(mangle_ops)) {
=======
	int ret = xt_register_template(&packet_mangler,
				       iptable_mangle_table_init);
	if (ret < 0)
		return ret;

	mangle_ops = xt_hook_ops_alloc(&packet_mangler, iptable_mangle_hook);
	if (IS_ERR(mangle_ops)) {
		xt_unregister_template(&packet_mangler);
>>>>>>> upstream/android-13
		ret = PTR_ERR(mangle_ops);
		return ret;
	}

	ret = register_pernet_subsys(&iptable_mangle_net_ops);
	if (ret < 0) {
<<<<<<< HEAD
=======
		xt_unregister_template(&packet_mangler);
>>>>>>> upstream/android-13
		kfree(mangle_ops);
		return ret;
	}

<<<<<<< HEAD
	ret = iptable_mangle_table_init(&init_net);
	if (ret) {
		unregister_pernet_subsys(&iptable_mangle_net_ops);
		kfree(mangle_ops);
	}

=======
>>>>>>> upstream/android-13
	return ret;
}

static void __exit iptable_mangle_fini(void)
{
	unregister_pernet_subsys(&iptable_mangle_net_ops);
<<<<<<< HEAD
=======
	xt_unregister_template(&packet_mangler);
>>>>>>> upstream/android-13
	kfree(mangle_ops);
}

module_init(iptable_mangle_init);
module_exit(iptable_mangle_fini);
