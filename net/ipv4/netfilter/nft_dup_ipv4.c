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
#include <net/netfilter/ipv4/nf_dup_ipv4.h>

struct nft_dup_ipv4 {
<<<<<<< HEAD
	enum nft_registers	sreg_addr:8;
	enum nft_registers	sreg_dev:8;
=======
	u8	sreg_addr;
	u8	sreg_dev;
>>>>>>> upstream/android-13
};

static void nft_dup_ipv4_eval(const struct nft_expr *expr,
			      struct nft_regs *regs,
			      const struct nft_pktinfo *pkt)
{
	struct nft_dup_ipv4 *priv = nft_expr_priv(expr);
	struct in_addr gw = {
		.s_addr = (__force __be32)regs->data[priv->sreg_addr],
	};
	int oif = priv->sreg_dev ? regs->data[priv->sreg_dev] : -1;

	nf_dup_ipv4(nft_net(pkt), pkt->skb, nft_hook(pkt), &gw, oif);
}

static int nft_dup_ipv4_init(const struct nft_ctx *ctx,
			     const struct nft_expr *expr,
			     const struct nlattr * const tb[])
{
	struct nft_dup_ipv4 *priv = nft_expr_priv(expr);
	int err;

	if (tb[NFTA_DUP_SREG_ADDR] == NULL)
		return -EINVAL;

<<<<<<< HEAD
	priv->sreg_addr = nft_parse_register(tb[NFTA_DUP_SREG_ADDR]);
	err = nft_validate_register_load(priv->sreg_addr, sizeof(struct in_addr));
	if (err < 0)
		return err;

	if (tb[NFTA_DUP_SREG_DEV] != NULL) {
		priv->sreg_dev = nft_parse_register(tb[NFTA_DUP_SREG_DEV]);
		return nft_validate_register_load(priv->sreg_dev, sizeof(int));
	}
	return 0;
=======
	err = nft_parse_register_load(tb[NFTA_DUP_SREG_ADDR], &priv->sreg_addr,
				      sizeof(struct in_addr));
	if (err < 0)
		return err;

	if (tb[NFTA_DUP_SREG_DEV])
		err = nft_parse_register_load(tb[NFTA_DUP_SREG_DEV],
					      &priv->sreg_dev, sizeof(int));

	return err;
>>>>>>> upstream/android-13
}

static int nft_dup_ipv4_dump(struct sk_buff *skb, const struct nft_expr *expr)
{
	struct nft_dup_ipv4 *priv = nft_expr_priv(expr);

	if (nft_dump_register(skb, NFTA_DUP_SREG_ADDR, priv->sreg_addr))
		goto nla_put_failure;
	if (priv->sreg_dev &&
	    nft_dump_register(skb, NFTA_DUP_SREG_DEV, priv->sreg_dev))
		goto nla_put_failure;

	return 0;

nla_put_failure:
	return -1;
}

static struct nft_expr_type nft_dup_ipv4_type;
static const struct nft_expr_ops nft_dup_ipv4_ops = {
	.type		= &nft_dup_ipv4_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_dup_ipv4)),
	.eval		= nft_dup_ipv4_eval,
	.init		= nft_dup_ipv4_init,
	.dump		= nft_dup_ipv4_dump,
};

static const struct nla_policy nft_dup_ipv4_policy[NFTA_DUP_MAX + 1] = {
	[NFTA_DUP_SREG_ADDR]	= { .type = NLA_U32 },
	[NFTA_DUP_SREG_DEV]	= { .type = NLA_U32 },
};

static struct nft_expr_type nft_dup_ipv4_type __read_mostly = {
	.family		= NFPROTO_IPV4,
	.name		= "dup",
	.ops		= &nft_dup_ipv4_ops,
	.policy		= nft_dup_ipv4_policy,
	.maxattr	= NFTA_DUP_MAX,
	.owner		= THIS_MODULE,
};

static int __init nft_dup_ipv4_module_init(void)
{
	return nft_register_expr(&nft_dup_ipv4_type);
}

static void __exit nft_dup_ipv4_module_exit(void)
{
	nft_unregister_expr(&nft_dup_ipv4_type);
}

module_init(nft_dup_ipv4_module_init);
module_exit(nft_dup_ipv4_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo Neira Ayuso <pablo@netfilter.org>");
MODULE_ALIAS_NFT_AF_EXPR(AF_INET, "dup");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("IPv4 nftables packet duplication support");
>>>>>>> upstream/android-13
