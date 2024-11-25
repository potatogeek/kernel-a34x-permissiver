<<<<<<< HEAD
/*
 * 'raw' table, which is the very first hooked in at PRE_ROUTING and LOCAL_OUT .
 *
 * Copyright (C) 2003 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * 'raw' table, which is the very first hooked in at PRE_ROUTING and LOCAL_OUT .
 *
 * Copyright (C) 2003 Jozsef Kadlecsik <kadlec@netfilter.org>
>>>>>>> upstream/android-13
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/slab.h>
#include <net/ip.h>

#define RAW_VALID_HOOKS ((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_OUT))

<<<<<<< HEAD
static int __net_init iptable_raw_table_init(struct net *net);

=======
>>>>>>> upstream/android-13
static bool raw_before_defrag __read_mostly;
MODULE_PARM_DESC(raw_before_defrag, "Enable raw table before defrag");
module_param(raw_before_defrag, bool, 0000);

static const struct xt_table packet_raw = {
	.name = "raw",
	.valid_hooks =  RAW_VALID_HOOKS,
	.me = THIS_MODULE,
	.af = NFPROTO_IPV4,
	.priority = NF_IP_PRI_RAW,
<<<<<<< HEAD
	.table_init = iptable_raw_table_init,
=======
>>>>>>> upstream/android-13
};

static const struct xt_table packet_raw_before_defrag = {
	.name = "raw",
	.valid_hooks =  RAW_VALID_HOOKS,
	.me = THIS_MODULE,
	.af = NFPROTO_IPV4,
	.priority = NF_IP_PRI_RAW_BEFORE_DEFRAG,
<<<<<<< HEAD
	.table_init = iptable_raw_table_init,
=======
>>>>>>> upstream/android-13
};

/* The work comes in here from netfilter.c. */
static unsigned int
iptable_raw_hook(void *priv, struct sk_buff *skb,
		 const struct nf_hook_state *state)
{
<<<<<<< HEAD
	return ipt_do_table(skb, state, state->net->ipv4.iptable_raw);
=======
	return ipt_do_table(skb, state, priv);
>>>>>>> upstream/android-13
}

static struct nf_hook_ops *rawtable_ops __read_mostly;

<<<<<<< HEAD
static int __net_init iptable_raw_table_init(struct net *net)
=======
static int iptable_raw_table_init(struct net *net)
>>>>>>> upstream/android-13
{
	struct ipt_replace *repl;
	const struct xt_table *table = &packet_raw;
	int ret;

	if (raw_before_defrag)
		table = &packet_raw_before_defrag;

<<<<<<< HEAD
	if (net->ipv4.iptable_raw)
		return 0;

	repl = ipt_alloc_initial_table(table);
	if (repl == NULL)
		return -ENOMEM;
	ret = ipt_register_table(net, table, repl, rawtable_ops,
				 &net->ipv4.iptable_raw);
=======
	repl = ipt_alloc_initial_table(table);
	if (repl == NULL)
		return -ENOMEM;
	ret = ipt_register_table(net, table, repl, rawtable_ops);
>>>>>>> upstream/android-13
	kfree(repl);
	return ret;
}

<<<<<<< HEAD
static void __net_exit iptable_raw_net_exit(struct net *net)
{
	if (!net->ipv4.iptable_raw)
		return;
	ipt_unregister_table(net, net->ipv4.iptable_raw, rawtable_ops);
	net->ipv4.iptable_raw = NULL;
}

static struct pernet_operations iptable_raw_net_ops = {
=======
static void __net_exit iptable_raw_net_pre_exit(struct net *net)
{
	ipt_unregister_table_pre_exit(net, "raw");
}

static void __net_exit iptable_raw_net_exit(struct net *net)
{
	ipt_unregister_table_exit(net, "raw");
}

static struct pernet_operations iptable_raw_net_ops = {
	.pre_exit = iptable_raw_net_pre_exit,
>>>>>>> upstream/android-13
	.exit = iptable_raw_net_exit,
};

static int __init iptable_raw_init(void)
{
	int ret;
	const struct xt_table *table = &packet_raw;

	if (raw_before_defrag) {
		table = &packet_raw_before_defrag;

		pr_info("Enabling raw table before defrag\n");
	}

<<<<<<< HEAD
	rawtable_ops = xt_hook_ops_alloc(table, iptable_raw_hook);
	if (IS_ERR(rawtable_ops))
		return PTR_ERR(rawtable_ops);

	ret = register_pernet_subsys(&iptable_raw_net_ops);
	if (ret < 0) {
=======
	ret = xt_register_template(table,
				   iptable_raw_table_init);
	if (ret < 0)
		return ret;

	rawtable_ops = xt_hook_ops_alloc(table, iptable_raw_hook);
	if (IS_ERR(rawtable_ops)) {
		xt_unregister_template(table);
		return PTR_ERR(rawtable_ops);
	}

	ret = register_pernet_subsys(&iptable_raw_net_ops);
	if (ret < 0) {
		xt_unregister_template(table);
>>>>>>> upstream/android-13
		kfree(rawtable_ops);
		return ret;
	}

<<<<<<< HEAD
	ret = iptable_raw_table_init(&init_net);
	if (ret) {
		unregister_pernet_subsys(&iptable_raw_net_ops);
		kfree(rawtable_ops);
	}

=======
>>>>>>> upstream/android-13
	return ret;
}

static void __exit iptable_raw_fini(void)
{
	unregister_pernet_subsys(&iptable_raw_net_ops);
	kfree(rawtable_ops);
<<<<<<< HEAD
=======
	xt_unregister_template(&packet_raw);
>>>>>>> upstream/android-13
}

module_init(iptable_raw_init);
module_exit(iptable_raw_fini);
MODULE_LICENSE("GPL");
