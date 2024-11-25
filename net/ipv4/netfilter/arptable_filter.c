<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Filtering ARP tables module.
 *
 * Copyright (C) 2002 David S. Miller (davem@redhat.com)
 *
 */

#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_arp/arp_tables.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David S. Miller <davem@redhat.com>");
MODULE_DESCRIPTION("arptables filter table");

#define FILTER_VALID_HOOKS ((1 << NF_ARP_IN) | (1 << NF_ARP_OUT) | \
			   (1 << NF_ARP_FORWARD))

<<<<<<< HEAD
static int __net_init arptable_filter_table_init(struct net *net);

=======
>>>>>>> upstream/android-13
static const struct xt_table packet_filter = {
	.name		= "filter",
	.valid_hooks	= FILTER_VALID_HOOKS,
	.me		= THIS_MODULE,
	.af		= NFPROTO_ARP,
	.priority	= NF_IP_PRI_FILTER,
<<<<<<< HEAD
	.table_init	= arptable_filter_table_init,
=======
>>>>>>> upstream/android-13
};

/* The work comes in here from netfilter.c */
static unsigned int
arptable_filter_hook(void *priv, struct sk_buff *skb,
		     const struct nf_hook_state *state)
{
<<<<<<< HEAD
	return arpt_do_table(skb, state, state->net->ipv4.arptable_filter);
=======
	return arpt_do_table(skb, state, priv);
>>>>>>> upstream/android-13
}

static struct nf_hook_ops *arpfilter_ops __read_mostly;

<<<<<<< HEAD
static int __net_init arptable_filter_table_init(struct net *net)
=======
static int arptable_filter_table_init(struct net *net)
>>>>>>> upstream/android-13
{
	struct arpt_replace *repl;
	int err;

<<<<<<< HEAD
	if (net->ipv4.arptable_filter)
		return 0;

	repl = arpt_alloc_initial_table(&packet_filter);
	if (repl == NULL)
		return -ENOMEM;
	err = arpt_register_table(net, &packet_filter, repl, arpfilter_ops,
				  &net->ipv4.arptable_filter);
=======
	repl = arpt_alloc_initial_table(&packet_filter);
	if (repl == NULL)
		return -ENOMEM;
	err = arpt_register_table(net, &packet_filter, repl, arpfilter_ops);
>>>>>>> upstream/android-13
	kfree(repl);
	return err;
}

<<<<<<< HEAD
static void __net_exit arptable_filter_net_exit(struct net *net)
{
	if (!net->ipv4.arptable_filter)
		return;
	arpt_unregister_table(net, net->ipv4.arptable_filter, arpfilter_ops);
	net->ipv4.arptable_filter = NULL;
=======
static void __net_exit arptable_filter_net_pre_exit(struct net *net)
{
	arpt_unregister_table_pre_exit(net, "filter");
}

static void __net_exit arptable_filter_net_exit(struct net *net)
{
	arpt_unregister_table(net, "filter");
>>>>>>> upstream/android-13
}

static struct pernet_operations arptable_filter_net_ops = {
	.exit = arptable_filter_net_exit,
<<<<<<< HEAD
=======
	.pre_exit = arptable_filter_net_pre_exit,
>>>>>>> upstream/android-13
};

static int __init arptable_filter_init(void)
{
<<<<<<< HEAD
	int ret;

	arpfilter_ops = xt_hook_ops_alloc(&packet_filter, arptable_filter_hook);
	if (IS_ERR(arpfilter_ops))
		return PTR_ERR(arpfilter_ops);

	ret = register_pernet_subsys(&arptable_filter_net_ops);
	if (ret < 0) {
=======
	int ret = xt_register_template(&packet_filter,
				       arptable_filter_table_init);

	if (ret < 0)
		return ret;

	arpfilter_ops = xt_hook_ops_alloc(&packet_filter, arptable_filter_hook);
	if (IS_ERR(arpfilter_ops)) {
		xt_unregister_template(&packet_filter);
		return PTR_ERR(arpfilter_ops);
	}

	ret = register_pernet_subsys(&arptable_filter_net_ops);
	if (ret < 0) {
		xt_unregister_template(&packet_filter);
>>>>>>> upstream/android-13
		kfree(arpfilter_ops);
		return ret;
	}

<<<<<<< HEAD
	ret = arptable_filter_table_init(&init_net);
	if (ret) {
		unregister_pernet_subsys(&arptable_filter_net_ops);
		kfree(arpfilter_ops);
	}

=======
>>>>>>> upstream/android-13
	return ret;
}

static void __exit arptable_filter_fini(void)
{
	unregister_pernet_subsys(&arptable_filter_net_ops);
<<<<<<< HEAD
=======
	xt_unregister_template(&packet_filter);
>>>>>>> upstream/android-13
	kfree(arpfilter_ops);
}

module_init(arptable_filter_init);
module_exit(arptable_filter_fini);
