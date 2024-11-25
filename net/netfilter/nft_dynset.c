<<<<<<< HEAD
/*
 * Copyright (c) 2015 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015 Patrick McHardy <kaber@trash.net>
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nf_tables.h>
#include <net/netfilter/nf_tables.h>
#include <net/netfilter/nf_tables_core.h>

struct nft_dynset {
	struct nft_set			*set;
	struct nft_set_ext_tmpl		tmpl;
	enum nft_dynset_ops		op:8;
<<<<<<< HEAD
	enum nft_registers		sreg_key:8;
	enum nft_registers		sreg_data:8;
	bool				invert;
	u64				timeout;
	struct nft_expr			*expr;
	struct nft_set_binding		binding;
};

=======
	u8				sreg_key;
	u8				sreg_data;
	bool				invert;
	bool				expr;
	u8				num_exprs;
	u64				timeout;
	struct nft_expr			*expr_array[NFT_SET_EXPR_MAX];
	struct nft_set_binding		binding;
};

static int nft_dynset_expr_setup(const struct nft_dynset *priv,
				 const struct nft_set_ext *ext)
{
	struct nft_set_elem_expr *elem_expr = nft_set_ext_expr(ext);
	struct nft_expr *expr;
	int i;

	for (i = 0; i < priv->num_exprs; i++) {
		expr = nft_setelem_expr_at(elem_expr, elem_expr->size);
		if (nft_expr_clone(expr, priv->expr_array[i]) < 0)
			return -1;

		elem_expr->size += priv->expr_array[i]->ops->size;
	}

	return 0;
}

>>>>>>> upstream/android-13
static void *nft_dynset_new(struct nft_set *set, const struct nft_expr *expr,
			    struct nft_regs *regs)
{
	const struct nft_dynset *priv = nft_expr_priv(expr);
	struct nft_set_ext *ext;
	u64 timeout;
	void *elem;

	if (!atomic_add_unless(&set->nelems, 1, set->size))
		return NULL;

	timeout = priv->timeout ? : set->timeout;
	elem = nft_set_elem_init(set, &priv->tmpl,
<<<<<<< HEAD
				 &regs->data[priv->sreg_key],
				 &regs->data[priv->sreg_data],
				 timeout, GFP_ATOMIC);
=======
				 &regs->data[priv->sreg_key], NULL,
				 &regs->data[priv->sreg_data],
				 timeout, 0, GFP_ATOMIC);
>>>>>>> upstream/android-13
	if (elem == NULL)
		goto err1;

	ext = nft_set_elem_ext(set, elem);
<<<<<<< HEAD
	if (priv->expr != NULL &&
	    nft_expr_clone(nft_set_ext_expr(ext), priv->expr) < 0)
=======
	if (priv->num_exprs && nft_dynset_expr_setup(priv, ext) < 0)
>>>>>>> upstream/android-13
		goto err2;

	return elem;

err2:
	nft_set_elem_destroy(set, elem, false);
err1:
	if (set->size)
		atomic_dec(&set->nelems);
	return NULL;
}

<<<<<<< HEAD
static void nft_dynset_eval(const struct nft_expr *expr,
			    struct nft_regs *regs,
			    const struct nft_pktinfo *pkt)
=======
void nft_dynset_eval(const struct nft_expr *expr,
		     struct nft_regs *regs, const struct nft_pktinfo *pkt)
>>>>>>> upstream/android-13
{
	const struct nft_dynset *priv = nft_expr_priv(expr);
	struct nft_set *set = priv->set;
	const struct nft_set_ext *ext;
<<<<<<< HEAD
	const struct nft_expr *sexpr;
	u64 timeout;

	if (set->ops->update(set, &regs->data[priv->sreg_key], nft_dynset_new,
			     expr, regs, &ext)) {
		sexpr = NULL;
		if (nft_set_ext_exists(ext, NFT_SET_EXT_EXPR))
			sexpr = nft_set_ext_expr(ext);

=======
	u64 timeout;

	if (priv->op == NFT_DYNSET_OP_DELETE) {
		set->ops->delete(set, &regs->data[priv->sreg_key]);
		return;
	}

	if (set->ops->update(set, &regs->data[priv->sreg_key], nft_dynset_new,
			     expr, regs, &ext)) {
>>>>>>> upstream/android-13
		if (priv->op == NFT_DYNSET_OP_UPDATE &&
		    nft_set_ext_exists(ext, NFT_SET_EXT_EXPIRATION)) {
			timeout = priv->timeout ? : set->timeout;
			*nft_set_ext_expiration(ext) = get_jiffies_64() + timeout;
		}

<<<<<<< HEAD
		if (sexpr != NULL)
			sexpr->ops->eval(sexpr, regs, pkt);
=======
		nft_set_elem_update_expr(ext, regs, pkt);
>>>>>>> upstream/android-13

		if (priv->invert)
			regs->verdict.code = NFT_BREAK;
		return;
	}

	if (!priv->invert)
		regs->verdict.code = NFT_BREAK;
}

<<<<<<< HEAD
=======
static void nft_dynset_ext_add_expr(struct nft_dynset *priv)
{
	u8 size = 0;
	int i;

	for (i = 0; i < priv->num_exprs; i++)
		size += priv->expr_array[i]->ops->size;

	nft_set_ext_add_length(&priv->tmpl, NFT_SET_EXT_EXPRESSIONS,
			       sizeof(struct nft_set_elem_expr) + size);
}

static struct nft_expr *
nft_dynset_expr_alloc(const struct nft_ctx *ctx, const struct nft_set *set,
		      const struct nlattr *attr, int pos)
{
	struct nft_expr *expr;
	int err;

	expr = nft_set_elem_expr_alloc(ctx, set, attr);
	if (IS_ERR(expr))
		return expr;

	if (set->exprs[pos] && set->exprs[pos]->ops != expr->ops) {
		err = -EOPNOTSUPP;
		goto err_dynset_expr;
	}

	return expr;

err_dynset_expr:
	nft_expr_destroy(ctx, expr);
	return ERR_PTR(err);
}

>>>>>>> upstream/android-13
static const struct nla_policy nft_dynset_policy[NFTA_DYNSET_MAX + 1] = {
	[NFTA_DYNSET_SET_NAME]	= { .type = NLA_STRING,
				    .len = NFT_SET_MAXNAMELEN - 1 },
	[NFTA_DYNSET_SET_ID]	= { .type = NLA_U32 },
	[NFTA_DYNSET_OP]	= { .type = NLA_U32 },
	[NFTA_DYNSET_SREG_KEY]	= { .type = NLA_U32 },
	[NFTA_DYNSET_SREG_DATA]	= { .type = NLA_U32 },
	[NFTA_DYNSET_TIMEOUT]	= { .type = NLA_U64 },
	[NFTA_DYNSET_EXPR]	= { .type = NLA_NESTED },
	[NFTA_DYNSET_FLAGS]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
	[NFTA_DYNSET_EXPRESSIONS] = { .type = NLA_NESTED },
>>>>>>> upstream/android-13
};

static int nft_dynset_init(const struct nft_ctx *ctx,
			   const struct nft_expr *expr,
			   const struct nlattr * const tb[])
{
<<<<<<< HEAD
=======
	struct nftables_pernet *nft_net = nft_pernet(ctx->net);
>>>>>>> upstream/android-13
	struct nft_dynset *priv = nft_expr_priv(expr);
	u8 genmask = nft_genmask_next(ctx->net);
	struct nft_set *set;
	u64 timeout;
<<<<<<< HEAD
	int err;

	lockdep_assert_held(&ctx->net->nft.commit_mutex);
=======
	int err, i;

	lockdep_assert_held(&nft_net->commit_mutex);
>>>>>>> upstream/android-13

	if (tb[NFTA_DYNSET_SET_NAME] == NULL ||
	    tb[NFTA_DYNSET_OP] == NULL ||
	    tb[NFTA_DYNSET_SREG_KEY] == NULL)
		return -EINVAL;

	if (tb[NFTA_DYNSET_FLAGS]) {
		u32 flags = ntohl(nla_get_be32(tb[NFTA_DYNSET_FLAGS]));
<<<<<<< HEAD

		if (flags & ~NFT_DYNSET_F_INV)
			return -EINVAL;
		if (flags & NFT_DYNSET_F_INV)
			priv->invert = true;
=======
		if (flags & ~(NFT_DYNSET_F_INV | NFT_DYNSET_F_EXPR))
			return -EOPNOTSUPP;
		if (flags & NFT_DYNSET_F_INV)
			priv->invert = true;
		if (flags & NFT_DYNSET_F_EXPR)
			priv->expr = true;
>>>>>>> upstream/android-13
	}

	set = nft_set_lookup_global(ctx->net, ctx->table,
				    tb[NFTA_DYNSET_SET_NAME],
				    tb[NFTA_DYNSET_SET_ID], genmask);
	if (IS_ERR(set))
		return PTR_ERR(set);

	if (set->ops->update == NULL)
		return -EOPNOTSUPP;

	if (set->flags & NFT_SET_CONSTANT)
		return -EBUSY;

	priv->op = ntohl(nla_get_be32(tb[NFTA_DYNSET_OP]));
<<<<<<< HEAD
	switch (priv->op) {
	case NFT_DYNSET_OP_ADD:
		break;
	case NFT_DYNSET_OP_UPDATE:
		if (!(set->flags & NFT_SET_TIMEOUT))
			return -EOPNOTSUPP;
		break;
	default:
		return -EOPNOTSUPP;
	}
=======
	if (priv->op > NFT_DYNSET_OP_DELETE)
		return -EOPNOTSUPP;
>>>>>>> upstream/android-13

	timeout = 0;
	if (tb[NFTA_DYNSET_TIMEOUT] != NULL) {
		if (!(set->flags & NFT_SET_TIMEOUT))
<<<<<<< HEAD
			return -EINVAL;
		timeout = msecs_to_jiffies(be64_to_cpu(nla_get_be64(
						tb[NFTA_DYNSET_TIMEOUT])));
	}

	priv->sreg_key = nft_parse_register(tb[NFTA_DYNSET_SREG_KEY]);
	err = nft_validate_register_load(priv->sreg_key, set->klen);
=======
			return -EOPNOTSUPP;

		err = nf_msecs_to_jiffies64(tb[NFTA_DYNSET_TIMEOUT], &timeout);
		if (err)
			return err;
	}

	err = nft_parse_register_load(tb[NFTA_DYNSET_SREG_KEY], &priv->sreg_key,
				      set->klen);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	if (tb[NFTA_DYNSET_SREG_DATA] != NULL) {
		if (!(set->flags & NFT_SET_MAP))
<<<<<<< HEAD
			return -EINVAL;
		if (set->dtype == NFT_DATA_VERDICT)
			return -EOPNOTSUPP;

		priv->sreg_data = nft_parse_register(tb[NFTA_DYNSET_SREG_DATA]);
		err = nft_validate_register_load(priv->sreg_data, set->dlen);
=======
			return -EOPNOTSUPP;
		if (set->dtype == NFT_DATA_VERDICT)
			return -EOPNOTSUPP;

		err = nft_parse_register_load(tb[NFTA_DYNSET_SREG_DATA],
					      &priv->sreg_data, set->dlen);
>>>>>>> upstream/android-13
		if (err < 0)
			return err;
	} else if (set->flags & NFT_SET_MAP)
		return -EINVAL;

<<<<<<< HEAD
	if (tb[NFTA_DYNSET_EXPR] != NULL) {
		if (!(set->flags & NFT_SET_EVAL))
			return -EINVAL;

		priv->expr = nft_expr_init(ctx, tb[NFTA_DYNSET_EXPR]);
		if (IS_ERR(priv->expr))
			return PTR_ERR(priv->expr);

		err = -EOPNOTSUPP;
		if (!(priv->expr->ops->type->flags & NFT_EXPR_STATEFUL))
			goto err1;

		if (priv->expr->ops->type->flags & NFT_EXPR_GC) {
			if (set->flags & NFT_SET_TIMEOUT)
				goto err1;
			if (!set->ops->gc_init)
				goto err1;
			set->ops->gc_init(set);
		}
=======
	if ((tb[NFTA_DYNSET_EXPR] || tb[NFTA_DYNSET_EXPRESSIONS]) &&
	    !(set->flags & NFT_SET_EVAL))
		return -EINVAL;

	if (tb[NFTA_DYNSET_EXPR]) {
		struct nft_expr *dynset_expr;

		dynset_expr = nft_dynset_expr_alloc(ctx, set,
						    tb[NFTA_DYNSET_EXPR], 0);
		if (IS_ERR(dynset_expr))
			return PTR_ERR(dynset_expr);

		priv->num_exprs++;
		priv->expr_array[0] = dynset_expr;

		if (set->num_exprs > 1 ||
		    (set->num_exprs == 1 &&
		     dynset_expr->ops != set->exprs[0]->ops)) {
			err = -EOPNOTSUPP;
			goto err_expr_free;
		}
	} else if (tb[NFTA_DYNSET_EXPRESSIONS]) {
		struct nft_expr *dynset_expr;
		struct nlattr *tmp;
		int left;

		if (!priv->expr)
			return -EINVAL;

		i = 0;
		nla_for_each_nested(tmp, tb[NFTA_DYNSET_EXPRESSIONS], left) {
			if (i == NFT_SET_EXPR_MAX) {
				err = -E2BIG;
				goto err_expr_free;
			}
			if (nla_type(tmp) != NFTA_LIST_ELEM) {
				err = -EINVAL;
				goto err_expr_free;
			}
			dynset_expr = nft_dynset_expr_alloc(ctx, set, tmp, i);
			if (IS_ERR(dynset_expr)) {
				err = PTR_ERR(dynset_expr);
				goto err_expr_free;
			}
			priv->expr_array[i] = dynset_expr;
			priv->num_exprs++;

			if (set->num_exprs &&
			    dynset_expr->ops != set->exprs[i]->ops) {
				err = -EOPNOTSUPP;
				goto err_expr_free;
			}
			i++;
		}
		if (set->num_exprs && set->num_exprs != i) {
			err = -EOPNOTSUPP;
			goto err_expr_free;
		}
	} else if (set->num_exprs > 0) {
		err = nft_set_elem_expr_clone(ctx, set, priv->expr_array);
		if (err < 0)
			return err;

		priv->num_exprs = set->num_exprs;
>>>>>>> upstream/android-13
	}

	nft_set_ext_prepare(&priv->tmpl);
	nft_set_ext_add_length(&priv->tmpl, NFT_SET_EXT_KEY, set->klen);
	if (set->flags & NFT_SET_MAP)
		nft_set_ext_add_length(&priv->tmpl, NFT_SET_EXT_DATA, set->dlen);
<<<<<<< HEAD
	if (priv->expr != NULL)
		nft_set_ext_add_length(&priv->tmpl, NFT_SET_EXT_EXPR,
				       priv->expr->ops->size);
=======

	if (priv->num_exprs)
		nft_dynset_ext_add_expr(priv);

>>>>>>> upstream/android-13
	if (set->flags & NFT_SET_TIMEOUT) {
		if (timeout || set->timeout) {
			nft_set_ext_add(&priv->tmpl, NFT_SET_EXT_TIMEOUT);
			nft_set_ext_add(&priv->tmpl, NFT_SET_EXT_EXPIRATION);
		}
	}

	priv->timeout = timeout;

	err = nf_tables_bind_set(ctx, set, &priv->binding);
	if (err < 0)
<<<<<<< HEAD
		goto err1;
=======
		goto err_expr_free;
>>>>>>> upstream/android-13

	if (set->size == 0)
		set->size = 0xffff;

	priv->set = set;
	return 0;

<<<<<<< HEAD
err1:
	if (priv->expr != NULL)
		nft_expr_destroy(ctx, priv->expr);
=======
err_expr_free:
	for (i = 0; i < priv->num_exprs; i++)
		nft_expr_destroy(ctx, priv->expr_array[i]);
>>>>>>> upstream/android-13
	return err;
}

static void nft_dynset_deactivate(const struct nft_ctx *ctx,
				  const struct nft_expr *expr,
				  enum nft_trans_phase phase)
{
	struct nft_dynset *priv = nft_expr_priv(expr);

	nf_tables_deactivate_set(ctx, priv->set, &priv->binding, phase);
}

static void nft_dynset_activate(const struct nft_ctx *ctx,
				const struct nft_expr *expr)
{
	struct nft_dynset *priv = nft_expr_priv(expr);

	priv->set->use++;
}

static void nft_dynset_destroy(const struct nft_ctx *ctx,
			       const struct nft_expr *expr)
{
	struct nft_dynset *priv = nft_expr_priv(expr);
<<<<<<< HEAD

	if (priv->expr != NULL)
		nft_expr_destroy(ctx, priv->expr);
=======
	int i;

	for (i = 0; i < priv->num_exprs; i++)
		nft_expr_destroy(ctx, priv->expr_array[i]);
>>>>>>> upstream/android-13

	nf_tables_destroy_set(ctx, priv->set);
}

static int nft_dynset_dump(struct sk_buff *skb, const struct nft_expr *expr)
{
	const struct nft_dynset *priv = nft_expr_priv(expr);
	u32 flags = priv->invert ? NFT_DYNSET_F_INV : 0;
<<<<<<< HEAD
=======
	int i;
>>>>>>> upstream/android-13

	if (nft_dump_register(skb, NFTA_DYNSET_SREG_KEY, priv->sreg_key))
		goto nla_put_failure;
	if (priv->set->flags & NFT_SET_MAP &&
	    nft_dump_register(skb, NFTA_DYNSET_SREG_DATA, priv->sreg_data))
		goto nla_put_failure;
	if (nla_put_be32(skb, NFTA_DYNSET_OP, htonl(priv->op)))
		goto nla_put_failure;
	if (nla_put_string(skb, NFTA_DYNSET_SET_NAME, priv->set->name))
		goto nla_put_failure;
	if (nla_put_be64(skb, NFTA_DYNSET_TIMEOUT,
<<<<<<< HEAD
			 cpu_to_be64(jiffies_to_msecs(priv->timeout)),
			 NFTA_DYNSET_PAD))
		goto nla_put_failure;
	if (priv->expr && nft_expr_dump(skb, NFTA_DYNSET_EXPR, priv->expr))
		goto nla_put_failure;
=======
			 nf_jiffies64_to_msecs(priv->timeout),
			 NFTA_DYNSET_PAD))
		goto nla_put_failure;
	if (priv->set->num_exprs == 0) {
		if (priv->num_exprs == 1) {
			if (nft_expr_dump(skb, NFTA_DYNSET_EXPR,
					  priv->expr_array[0]))
				goto nla_put_failure;
		} else if (priv->num_exprs > 1) {
			struct nlattr *nest;

			nest = nla_nest_start_noflag(skb, NFTA_DYNSET_EXPRESSIONS);
			if (!nest)
				goto nla_put_failure;

			for (i = 0; i < priv->num_exprs; i++) {
				if (nft_expr_dump(skb, NFTA_LIST_ELEM,
						  priv->expr_array[i]))
					goto nla_put_failure;
			}
			nla_nest_end(skb, nest);
		}
	}
>>>>>>> upstream/android-13
	if (nla_put_be32(skb, NFTA_DYNSET_FLAGS, htonl(flags)))
		goto nla_put_failure;
	return 0;

nla_put_failure:
	return -1;
}

static const struct nft_expr_ops nft_dynset_ops = {
	.type		= &nft_dynset_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_dynset)),
	.eval		= nft_dynset_eval,
	.init		= nft_dynset_init,
	.destroy	= nft_dynset_destroy,
	.activate	= nft_dynset_activate,
	.deactivate	= nft_dynset_deactivate,
	.dump		= nft_dynset_dump,
};

struct nft_expr_type nft_dynset_type __read_mostly = {
	.name		= "dynset",
	.ops		= &nft_dynset_ops,
	.policy		= nft_dynset_policy,
	.maxattr	= NFTA_DYNSET_MAX,
	.owner		= THIS_MODULE,
};
