<<<<<<< HEAD
/*
 * Copyright (c) 2015 Pablo Neira Ayuso <pablo@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015 Pablo Neira Ayuso <pablo@netfilter.org>
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nf_tables.h>
#include <net/netfilter/nf_tables.h>
<<<<<<< HEAD
#include <net/netfilter/nf_dup_netdev.h>

struct nft_dup_netdev {
	enum nft_registers	sreg_dev:8;
=======
#include <net/netfilter/nf_tables_offload.h>
#include <net/netfilter/nf_dup_netdev.h>

struct nft_dup_netdev {
	u8	sreg_dev;
>>>>>>> upstream/android-13
};

static void nft_dup_netdev_eval(const struct nft_expr *expr,
				struct nft_regs *regs,
				const struct nft_pktinfo *pkt)
{
	struct nft_dup_netdev *priv = nft_expr_priv(expr);
	int oif = regs->data[priv->sreg_dev];

	nf_dup_netdev_egress(pkt, oif);
}

static const struct nla_policy nft_dup_netdev_policy[NFTA_DUP_MAX + 1] = {
	[NFTA_DUP_SREG_DEV]	= { .type = NLA_U32 },
};

static int nft_dup_netdev_init(const struct nft_ctx *ctx,
			       const struct nft_expr *expr,
			       const struct nlattr * const tb[])
{
	struct nft_dup_netdev *priv = nft_expr_priv(expr);

	if (tb[NFTA_DUP_SREG_DEV] == NULL)
		return -EINVAL;

<<<<<<< HEAD
	priv->sreg_dev = nft_parse_register(tb[NFTA_DUP_SREG_DEV]);
	return nft_validate_register_load(priv->sreg_dev, sizeof(int));
}

static const struct nft_expr_ops nft_dup_netdev_ingress_ops;

=======
	return nft_parse_register_load(tb[NFTA_DUP_SREG_DEV], &priv->sreg_dev,
				       sizeof(int));
}

>>>>>>> upstream/android-13
static int nft_dup_netdev_dump(struct sk_buff *skb, const struct nft_expr *expr)
{
	struct nft_dup_netdev *priv = nft_expr_priv(expr);

	if (nft_dump_register(skb, NFTA_DUP_SREG_DEV, priv->sreg_dev))
		goto nla_put_failure;

	return 0;

nla_put_failure:
	return -1;
}

<<<<<<< HEAD
=======
static int nft_dup_netdev_offload(struct nft_offload_ctx *ctx,
				  struct nft_flow_rule *flow,
				  const struct nft_expr *expr)
{
	const struct nft_dup_netdev *priv = nft_expr_priv(expr);
	int oif = ctx->regs[priv->sreg_dev].data.data[0];

	return nft_fwd_dup_netdev_offload(ctx, flow, FLOW_ACTION_MIRRED, oif);
}

static bool nft_dup_netdev_offload_action(const struct nft_expr *expr)
{
	return true;
}

>>>>>>> upstream/android-13
static struct nft_expr_type nft_dup_netdev_type;
static const struct nft_expr_ops nft_dup_netdev_ops = {
	.type		= &nft_dup_netdev_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_dup_netdev)),
	.eval		= nft_dup_netdev_eval,
	.init		= nft_dup_netdev_init,
	.dump		= nft_dup_netdev_dump,
<<<<<<< HEAD
=======
	.offload	= nft_dup_netdev_offload,
	.offload_action	= nft_dup_netdev_offload_action,
>>>>>>> upstream/android-13
};

static struct nft_expr_type nft_dup_netdev_type __read_mostly = {
	.family		= NFPROTO_NETDEV,
	.name		= "dup",
	.ops		= &nft_dup_netdev_ops,
	.policy		= nft_dup_netdev_policy,
	.maxattr	= NFTA_DUP_MAX,
	.owner		= THIS_MODULE,
};

static int __init nft_dup_netdev_module_init(void)
{
	return nft_register_expr(&nft_dup_netdev_type);
}

static void __exit nft_dup_netdev_module_exit(void)
{
	nft_unregister_expr(&nft_dup_netdev_type);
}

module_init(nft_dup_netdev_module_init);
module_exit(nft_dup_netdev_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo Neira Ayuso <pablo@netfilter.org>");
MODULE_ALIAS_NFT_AF_EXPR(5, "dup");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("nftables netdev packet duplication support");
>>>>>>> upstream/android-13
