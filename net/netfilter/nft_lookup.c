<<<<<<< HEAD
/*
 * Copyright (c) 2009 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2009 Patrick McHardy <kaber@trash.net>
 *
>>>>>>> upstream/android-13
 * Development of this code funded by Astaro AG (http://www.astaro.com/)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nf_tables.h>
#include <net/netfilter/nf_tables.h>
#include <net/netfilter/nf_tables_core.h>

struct nft_lookup {
	struct nft_set			*set;
<<<<<<< HEAD
	enum nft_registers		sreg:8;
	enum nft_registers		dreg:8;
=======
	u8				sreg;
	u8				dreg;
>>>>>>> upstream/android-13
	bool				invert;
	struct nft_set_binding		binding;
};

<<<<<<< HEAD
=======
#ifdef CONFIG_RETPOLINE
bool nft_set_do_lookup(const struct net *net, const struct nft_set *set,
		       const u32 *key, const struct nft_set_ext **ext)
{
	if (set->ops == &nft_set_hash_fast_type.ops)
		return nft_hash_lookup_fast(net, set, key, ext);
	if (set->ops == &nft_set_hash_type.ops)
		return nft_hash_lookup(net, set, key, ext);

	if (set->ops == &nft_set_rhash_type.ops)
		return nft_rhash_lookup(net, set, key, ext);

	if (set->ops == &nft_set_bitmap_type.ops)
		return nft_bitmap_lookup(net, set, key, ext);

	if (set->ops == &nft_set_pipapo_type.ops)
		return nft_pipapo_lookup(net, set, key, ext);
#if defined(CONFIG_X86_64) && !defined(CONFIG_UML)
	if (set->ops == &nft_set_pipapo_avx2_type.ops)
		return nft_pipapo_avx2_lookup(net, set, key, ext);
#endif

	if (set->ops == &nft_set_rbtree_type.ops)
		return nft_rbtree_lookup(net, set, key, ext);

	WARN_ON_ONCE(1);
	return set->ops->lookup(net, set, key, ext);
}
EXPORT_SYMBOL_GPL(nft_set_do_lookup);
#endif

>>>>>>> upstream/android-13
void nft_lookup_eval(const struct nft_expr *expr,
		     struct nft_regs *regs,
		     const struct nft_pktinfo *pkt)
{
	const struct nft_lookup *priv = nft_expr_priv(expr);
	const struct nft_set *set = priv->set;
<<<<<<< HEAD
	const struct nft_set_ext *ext;
	bool found;

	found = set->ops->lookup(nft_net(pkt), set, &regs->data[priv->sreg],
				 &ext) ^ priv->invert;
	if (!found) {
		regs->verdict.code = NFT_BREAK;
		return;
	}

	if (set->flags & NFT_SET_MAP)
		nft_data_copy(&regs->data[priv->dreg],
			      nft_set_ext_data(ext), set->dlen);

=======
	const struct nft_set_ext *ext = NULL;
	const struct net *net = nft_net(pkt);
	bool found;

	found =	nft_set_do_lookup(net, set, &regs->data[priv->sreg], &ext) ^
				  priv->invert;
	if (!found) {
		ext = nft_set_catchall_lookup(net, set);
		if (!ext) {
			regs->verdict.code = NFT_BREAK;
			return;
		}
	}

	if (ext) {
		if (set->flags & NFT_SET_MAP)
			nft_data_copy(&regs->data[priv->dreg],
				      nft_set_ext_data(ext), set->dlen);

		nft_set_elem_update_expr(ext, regs, pkt);
	}
>>>>>>> upstream/android-13
}

static const struct nla_policy nft_lookup_policy[NFTA_LOOKUP_MAX + 1] = {
	[NFTA_LOOKUP_SET]	= { .type = NLA_STRING,
				    .len = NFT_SET_MAXNAMELEN - 1 },
	[NFTA_LOOKUP_SET_ID]	= { .type = NLA_U32 },
	[NFTA_LOOKUP_SREG]	= { .type = NLA_U32 },
	[NFTA_LOOKUP_DREG]	= { .type = NLA_U32 },
	[NFTA_LOOKUP_FLAGS]	= { .type = NLA_U32 },
};

static int nft_lookup_init(const struct nft_ctx *ctx,
			   const struct nft_expr *expr,
			   const struct nlattr * const tb[])
{
	struct nft_lookup *priv = nft_expr_priv(expr);
	u8 genmask = nft_genmask_next(ctx->net);
	struct nft_set *set;
	u32 flags;
	int err;

	if (tb[NFTA_LOOKUP_SET] == NULL ||
	    tb[NFTA_LOOKUP_SREG] == NULL)
		return -EINVAL;

	set = nft_set_lookup_global(ctx->net, ctx->table, tb[NFTA_LOOKUP_SET],
				    tb[NFTA_LOOKUP_SET_ID], genmask);
	if (IS_ERR(set))
		return PTR_ERR(set);

<<<<<<< HEAD
	priv->sreg = nft_parse_register(tb[NFTA_LOOKUP_SREG]);
	err = nft_validate_register_load(priv->sreg, set->klen);
=======
	err = nft_parse_register_load(tb[NFTA_LOOKUP_SREG], &priv->sreg,
				      set->klen);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	if (tb[NFTA_LOOKUP_FLAGS]) {
		flags = ntohl(nla_get_be32(tb[NFTA_LOOKUP_FLAGS]));

		if (flags & ~NFT_LOOKUP_F_INV)
			return -EINVAL;

		if (flags & NFT_LOOKUP_F_INV) {
			if (set->flags & NFT_SET_MAP)
				return -EINVAL;
			priv->invert = true;
		}
	}

	if (tb[NFTA_LOOKUP_DREG] != NULL) {
		if (priv->invert)
			return -EINVAL;
		if (!(set->flags & NFT_SET_MAP))
			return -EINVAL;

<<<<<<< HEAD
		priv->dreg = nft_parse_register(tb[NFTA_LOOKUP_DREG]);
		err = nft_validate_register_store(ctx, priv->dreg, NULL,
						  set->dtype, set->dlen);
=======
		err = nft_parse_register_store(ctx, tb[NFTA_LOOKUP_DREG],
					       &priv->dreg, NULL, set->dtype,
					       set->dlen);
>>>>>>> upstream/android-13
		if (err < 0)
			return err;
	} else if (set->flags & NFT_SET_MAP)
		return -EINVAL;

	priv->binding.flags = set->flags & NFT_SET_MAP;

	err = nf_tables_bind_set(ctx, set, &priv->binding);
	if (err < 0)
		return err;

	priv->set = set;
	return 0;
}

static void nft_lookup_deactivate(const struct nft_ctx *ctx,
				  const struct nft_expr *expr,
				  enum nft_trans_phase phase)
{
	struct nft_lookup *priv = nft_expr_priv(expr);

	nf_tables_deactivate_set(ctx, priv->set, &priv->binding, phase);
}

static void nft_lookup_activate(const struct nft_ctx *ctx,
				const struct nft_expr *expr)
{
	struct nft_lookup *priv = nft_expr_priv(expr);

	priv->set->use++;
}

static void nft_lookup_destroy(const struct nft_ctx *ctx,
			       const struct nft_expr *expr)
{
	struct nft_lookup *priv = nft_expr_priv(expr);

	nf_tables_destroy_set(ctx, priv->set);
}

static int nft_lookup_dump(struct sk_buff *skb, const struct nft_expr *expr)
{
	const struct nft_lookup *priv = nft_expr_priv(expr);
	u32 flags = priv->invert ? NFT_LOOKUP_F_INV : 0;

	if (nla_put_string(skb, NFTA_LOOKUP_SET, priv->set->name))
		goto nla_put_failure;
	if (nft_dump_register(skb, NFTA_LOOKUP_SREG, priv->sreg))
		goto nla_put_failure;
	if (priv->set->flags & NFT_SET_MAP)
		if (nft_dump_register(skb, NFTA_LOOKUP_DREG, priv->dreg))
			goto nla_put_failure;
	if (nla_put_be32(skb, NFTA_LOOKUP_FLAGS, htonl(flags)))
		goto nla_put_failure;
	return 0;

nla_put_failure:
	return -1;
}

static int nft_lookup_validate_setelem(const struct nft_ctx *ctx,
				       struct nft_set *set,
				       const struct nft_set_iter *iter,
				       struct nft_set_elem *elem)
{
	const struct nft_set_ext *ext = nft_set_elem_ext(set, elem->priv);
	struct nft_ctx *pctx = (struct nft_ctx *)ctx;
	const struct nft_data *data;
	int err;

	if (nft_set_ext_exists(ext, NFT_SET_EXT_FLAGS) &&
	    *nft_set_ext_flags(ext) & NFT_SET_ELEM_INTERVAL_END)
		return 0;

	data = nft_set_ext_data(ext);
	switch (data->verdict.code) {
	case NFT_JUMP:
	case NFT_GOTO:
		pctx->level++;
		err = nft_chain_validate(ctx, data->verdict.chain);
		if (err < 0)
			return err;
		pctx->level--;
		break;
	default:
		break;
	}

	return 0;
}

static int nft_lookup_validate(const struct nft_ctx *ctx,
			       const struct nft_expr *expr,
			       const struct nft_data **d)
{
	const struct nft_lookup *priv = nft_expr_priv(expr);
	struct nft_set_iter iter;

	if (!(priv->set->flags & NFT_SET_MAP) ||
	    priv->set->dtype != NFT_DATA_VERDICT)
		return 0;

	iter.genmask	= nft_genmask_next(ctx->net);
	iter.skip	= 0;
	iter.count	= 0;
	iter.err	= 0;
	iter.fn		= nft_lookup_validate_setelem;

	priv->set->ops->walk(ctx, priv->set, &iter);
	if (iter.err < 0)
		return iter.err;

	return 0;
}

static const struct nft_expr_ops nft_lookup_ops = {
	.type		= &nft_lookup_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_lookup)),
	.eval		= nft_lookup_eval,
	.init		= nft_lookup_init,
	.activate	= nft_lookup_activate,
	.deactivate	= nft_lookup_deactivate,
	.destroy	= nft_lookup_destroy,
	.dump		= nft_lookup_dump,
	.validate	= nft_lookup_validate,
};

struct nft_expr_type nft_lookup_type __read_mostly = {
	.name		= "lookup",
	.ops		= &nft_lookup_ops,
	.policy		= nft_lookup_policy,
	.maxattr	= NFTA_LOOKUP_MAX,
	.owner		= THIS_MODULE,
};
