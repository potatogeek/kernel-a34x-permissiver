<<<<<<< HEAD
/*
 * IPv6 raw table, a port of the IPv4 raw table to IPv6
 *
 * Copyright (C) 2003 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * IPv6 raw table, a port of the IPv4 raw table to IPv6
 *
 * Copyright (C) 2003 Jozsef Kadlecsik <kadlec@netfilter.org>
>>>>>>> upstream/android-13
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/slab.h>

#define RAW_VALID_HOOKS ((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_OUT))

<<<<<<< HEAD
static int __net_init ip6table_raw_table_init(struct net *net);

=======
>>>>>>> upstream/android-13
static bool raw_before_defrag __read_mostly;
MODULE_PARM_DESC(raw_before_defrag, "Enable raw table before defrag");
module_param(raw_before_defrag, bool, 0000);

static const struct xt_table packet_raw = {
	.name = "raw",
	.valid_hooks = RAW_VALID_HOOKS,
	.me = THIS_MODULE,
	.af = NFPROTO_IPV6,
	.priority = NF_IP6_PRI_RAW,
<<<<<<< HEAD
	.table_init = ip6table_raw_table_init,
=======
>>>>>>> upstream/android-13
};

static const struct xt_table packet_raw_before_defrag = {
	.name = "raw",
	.valid_hooks = RAW_VALID_HOOKS,
	.me = THIS_MODULE,
	.af = NFPROTO_IPV6,
	.priority = NF_IP6_PRI_RAW_BEFORE_DEFRAG,
<<<<<<< HEAD
	.table_init = ip6table_raw_table_init,
=======
>>>>>>> upstream/android-13
};

/* The work comes in here from netfilter.c. */
static unsigned int
ip6table_raw_hook(void *priv, struct sk_buff *skb,
		  const struct nf_hook_state *state)
{
<<<<<<< HEAD
	return ip6t_do_table(skb, state, state->net->ipv6.ip6table_raw);
=======
	return ip6t_do_table(skb, state, priv);
>>>>>>> upstream/android-13
}

static struct nf_hook_ops *rawtable_ops __read_mostly;

<<<<<<< HEAD
static int __net_init ip6table_raw_table_init(struct net *net)
=======
static int ip6table_raw_table_init(struct net *net)
>>>>>>> upstream/android-13
{
	struct ip6t_replace *repl;
	const struct xt_table *table = &packet_raw;
	int ret;

	if (raw_before_defrag)
		table = &packet_raw_before_defrag;

<<<<<<< HEAD
	if (net->ipv6.ip6table_raw)
		return 0;

	repl = ip6t_alloc_initial_table(table);
	if (repl == NULL)
		return -ENOMEM;
	ret = ip6t_register_table(net, table, repl, rawtable_ops,
				  &net->ipv6.ip6table_raw);
=======
	repl = ip6t_alloc_initial_table(table);
	if (repl == NULL)
		return -ENOMEM;
	ret = ip6t_register_table(net, table, repl, rawtable_ops);
>>>>>>> upstream/android-13
	kfree(repl);
	return ret;
}

<<<<<<< HEAD
static void __net_exit ip6table_raw_net_exit(struct net *net)
{
	if (!net->ipv6.ip6table_raw)
		return;
	ip6t_unregister_table(net, net->ipv6.ip6table_raw, rawtable_ops);
	net->ipv6.ip6table_raw = NULL;
}

static struct pernet_operations ip6table_raw_net_ops = {
=======
static void __net_exit ip6table_raw_net_pre_exit(struct net *net)
{
	ip6t_unregister_table_pre_exit(net, "raw");
}

static void __net_exit ip6table_raw_net_exit(struct net *net)
{
	ip6t_unregister_table_exit(net, "raw");
}

static struct pernet_operations ip6table_raw_net_ops = {
	.pre_exit = ip6table_raw_net_pre_exit,
>>>>>>> upstream/android-13
	.exit = ip6table_raw_net_exit,
};

static int __init ip6table_raw_init(void)
{
<<<<<<< HEAD
	int ret;
	const struct xt_table *table = &packet_raw;

	if (raw_before_defrag) {
		table = &packet_raw_before_defrag;

		pr_info("Enabling raw table before defrag\n");
	}

	/* Register hooks */
	rawtable_ops = xt_hook_ops_alloc(table, ip6table_raw_hook);
	if (IS_ERR(rawtable_ops))
		return PTR_ERR(rawtable_ops);
=======
	const struct xt_table *table = &packet_raw;
	int ret;

	if (raw_before_defrag) {
		table = &packet_raw_before_defrag;
		pr_info("Enabling raw table before defrag\n");
	}

	ret = xt_register_template(table, ip6table_raw_table_init);
	if (ret < 0)
		return ret;

	/* Register hooks */
	rawtable_ops = xt_hook_ops_alloc(table, ip6table_raw_hook);
	if (IS_ERR(rawtable_ops)) {
		xt_unregister_template(table);
		return PTR_ERR(rawtable_ops);
	}
>>>>>>> upstream/android-13

	ret = register_pernet_subsys(&ip6table_raw_net_ops);
	if (ret < 0) {
		kfree(rawtable_ops);
<<<<<<< HEAD
		return ret;
	}

	ret = ip6table_raw_table_init(&init_net);
	if (ret) {
		unregister_pernet_subsys(&ip6table_raw_net_ops);
		kfree(rawtable_ops);
	}
=======
		xt_unregister_template(table);
		return ret;
	}

>>>>>>> upstream/android-13
	return ret;
}

static void __exit ip6table_raw_fini(void)
{
	unregister_pernet_subsys(&ip6table_raw_net_ops);
<<<<<<< HEAD
=======
	xt_unregister_template(&packet_raw);
>>>>>>> upstream/android-13
	kfree(rawtable_ops);
}

module_init(ip6table_raw_init);
module_exit(ip6table_raw_fini);
MODULE_LICENSE("GPL");
