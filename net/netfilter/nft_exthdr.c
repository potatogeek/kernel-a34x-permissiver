<<<<<<< HEAD
/*
 * Copyright (c) 2008 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2008 Patrick McHardy <kaber@trash.net>
 *
>>>>>>> upstream/android-13
 * Development of this code funded by Astaro AG (http://www.astaro.com/)
 */

#include <asm/unaligned.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nf_tables.h>
<<<<<<< HEAD
#include <net/netfilter/nf_tables_core.h>
#include <net/netfilter/nf_tables.h>
=======
#include <linux/sctp.h>
#include <net/netfilter/nf_tables_core.h>
#include <net/netfilter/nf_tables.h>
#include <net/sctp/sctp.h>
>>>>>>> upstream/android-13
#include <net/tcp.h>

struct nft_exthdr {
	u8			type;
	u8			offset;
	u8			len;
	u8			op;
<<<<<<< HEAD
	enum nft_registers	dreg:8;
	enum nft_registers	sreg:8;
=======
	u8			dreg;
	u8			sreg;
>>>>>>> upstream/android-13
	u8			flags;
};

static unsigned int optlen(const u8 *opt, unsigned int offset)
{
	/* Beware zero-length options: make finite progress */
	if (opt[offset] <= TCPOPT_NOP || opt[offset + 1] == 0)
		return 1;
	else
		return opt[offset + 1];
}

static void nft_exthdr_ipv6_eval(const struct nft_expr *expr,
				 struct nft_regs *regs,
				 const struct nft_pktinfo *pkt)
{
	struct nft_exthdr *priv = nft_expr_priv(expr);
	u32 *dest = &regs->data[priv->dreg];
	unsigned int offset = 0;
	int err;

<<<<<<< HEAD
	err = ipv6_find_hdr(pkt->skb, &offset, priv->type, NULL, NULL);
	if (priv->flags & NFT_EXTHDR_F_PRESENT) {
		*dest = (err >= 0);
=======
	if (pkt->skb->protocol != htons(ETH_P_IPV6))
		goto err;

	err = ipv6_find_hdr(pkt->skb, &offset, priv->type, NULL, NULL);
	if (priv->flags & NFT_EXTHDR_F_PRESENT) {
		nft_reg_store8(dest, err >= 0);
		return;
	} else if (err < 0) {
		goto err;
	}
	offset += priv->offset;

	dest[priv->len / NFT_REG32_SIZE] = 0;
	if (skb_copy_bits(pkt->skb, offset, dest, priv->len) < 0)
		goto err;
	return;
err:
	regs->verdict.code = NFT_BREAK;
}

/* find the offset to specified option.
 *
 * If target header is found, its offset is set in *offset and return option
 * number. Otherwise, return negative error.
 *
 * If the first fragment doesn't contain the End of Options it is considered
 * invalid.
 */
static int ipv4_find_option(struct net *net, struct sk_buff *skb,
			    unsigned int *offset, int target)
{
	unsigned char optbuf[sizeof(struct ip_options) + 40];
	struct ip_options *opt = (struct ip_options *)optbuf;
	struct iphdr *iph, _iph;
	unsigned int start;
	bool found = false;
	__be32 info;
	int optlen;

	iph = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
	if (!iph)
		return -EBADMSG;
	start = sizeof(struct iphdr);

	optlen = iph->ihl * 4 - (int)sizeof(struct iphdr);
	if (optlen <= 0)
		return -ENOENT;

	memset(opt, 0, sizeof(struct ip_options));
	/* Copy the options since __ip_options_compile() modifies
	 * the options.
	 */
	if (skb_copy_bits(skb, start, opt->__data, optlen))
		return -EBADMSG;
	opt->optlen = optlen;

	if (__ip_options_compile(net, opt, NULL, &info))
		return -EBADMSG;

	switch (target) {
	case IPOPT_SSRR:
	case IPOPT_LSRR:
		if (!opt->srr)
			break;
		found = target == IPOPT_SSRR ? opt->is_strictroute :
					       !opt->is_strictroute;
		if (found)
			*offset = opt->srr + start;
		break;
	case IPOPT_RR:
		if (!opt->rr)
			break;
		*offset = opt->rr + start;
		found = true;
		break;
	case IPOPT_RA:
		if (!opt->router_alert)
			break;
		*offset = opt->router_alert + start;
		found = true;
		break;
	default:
		return -EOPNOTSUPP;
	}
	return found ? target : -ENOENT;
}

static void nft_exthdr_ipv4_eval(const struct nft_expr *expr,
				 struct nft_regs *regs,
				 const struct nft_pktinfo *pkt)
{
	struct nft_exthdr *priv = nft_expr_priv(expr);
	u32 *dest = &regs->data[priv->dreg];
	struct sk_buff *skb = pkt->skb;
	unsigned int offset;
	int err;

	if (skb->protocol != htons(ETH_P_IP))
		goto err;

	err = ipv4_find_option(nft_net(pkt), skb, &offset, priv->type);
	if (priv->flags & NFT_EXTHDR_F_PRESENT) {
		nft_reg_store8(dest, err >= 0);
>>>>>>> upstream/android-13
		return;
	} else if (err < 0) {
		goto err;
	}
	offset += priv->offset;

	dest[priv->len / NFT_REG32_SIZE] = 0;
	if (skb_copy_bits(pkt->skb, offset, dest, priv->len) < 0)
		goto err;
	return;
err:
	regs->verdict.code = NFT_BREAK;
}

static void *
nft_tcp_header_pointer(const struct nft_pktinfo *pkt,
		       unsigned int len, void *buffer, unsigned int *tcphdr_len)
{
	struct tcphdr *tcph;

<<<<<<< HEAD
	if (!pkt->tprot_set || pkt->tprot != IPPROTO_TCP)
		return NULL;

	tcph = skb_header_pointer(pkt->skb, pkt->xt.thoff, sizeof(*tcph), buffer);
=======
	if (pkt->tprot != IPPROTO_TCP)
		return NULL;

	tcph = skb_header_pointer(pkt->skb, nft_thoff(pkt), sizeof(*tcph), buffer);
>>>>>>> upstream/android-13
	if (!tcph)
		return NULL;

	*tcphdr_len = __tcp_hdrlen(tcph);
	if (*tcphdr_len < sizeof(*tcph) || *tcphdr_len > len)
		return NULL;

<<<<<<< HEAD
	return skb_header_pointer(pkt->skb, pkt->xt.thoff, *tcphdr_len, buffer);
=======
	return skb_header_pointer(pkt->skb, nft_thoff(pkt), *tcphdr_len, buffer);
>>>>>>> upstream/android-13
}

static void nft_exthdr_tcp_eval(const struct nft_expr *expr,
				struct nft_regs *regs,
				const struct nft_pktinfo *pkt)
{
	u8 buff[sizeof(struct tcphdr) + MAX_TCP_OPTION_SPACE];
	struct nft_exthdr *priv = nft_expr_priv(expr);
	unsigned int i, optl, tcphdr_len, offset;
	u32 *dest = &regs->data[priv->dreg];
	struct tcphdr *tcph;
	u8 *opt;

	tcph = nft_tcp_header_pointer(pkt, sizeof(buff), buff, &tcphdr_len);
	if (!tcph)
		goto err;

	opt = (u8 *)tcph;
	for (i = sizeof(*tcph); i < tcphdr_len - 1; i += optl) {
		optl = optlen(opt, i);

		if (priv->type != opt[i])
			continue;

		if (i + optl > tcphdr_len || priv->len + priv->offset > optl)
			goto err;

		offset = i + priv->offset;
		if (priv->flags & NFT_EXTHDR_F_PRESENT) {
			*dest = 1;
		} else {
			dest[priv->len / NFT_REG32_SIZE] = 0;
			memcpy(dest, opt + offset, priv->len);
		}

		return;
	}

err:
	if (priv->flags & NFT_EXTHDR_F_PRESENT)
		*dest = 0;
	else
		regs->verdict.code = NFT_BREAK;
}

static void nft_exthdr_tcp_set_eval(const struct nft_expr *expr,
				    struct nft_regs *regs,
				    const struct nft_pktinfo *pkt)
{
	u8 buff[sizeof(struct tcphdr) + MAX_TCP_OPTION_SPACE];
	struct nft_exthdr *priv = nft_expr_priv(expr);
	unsigned int i, optl, tcphdr_len, offset;
	struct tcphdr *tcph;
	u8 *opt;
<<<<<<< HEAD
	u32 src;

	tcph = nft_tcp_header_pointer(pkt, sizeof(buff), buff, &tcphdr_len);
	if (!tcph)
		return;
=======

	tcph = nft_tcp_header_pointer(pkt, sizeof(buff), buff, &tcphdr_len);
	if (!tcph)
		goto err;
>>>>>>> upstream/android-13

	opt = (u8 *)tcph;
	for (i = sizeof(*tcph); i < tcphdr_len - 1; i += optl) {
		union {
<<<<<<< HEAD
			u8 octet;
=======
>>>>>>> upstream/android-13
			__be16 v16;
			__be32 v32;
		} old, new;

		optl = optlen(opt, i);

		if (priv->type != opt[i])
			continue;

		if (i + optl > tcphdr_len || priv->len + priv->offset > optl)
<<<<<<< HEAD
			return;

		if (!skb_make_writable(pkt->skb, pkt->xt.thoff + i + priv->len))
			return;
=======
			goto err;

		if (skb_ensure_writable(pkt->skb,
					nft_thoff(pkt) + i + priv->len))
			goto err;
>>>>>>> upstream/android-13

		tcph = nft_tcp_header_pointer(pkt, sizeof(buff), buff,
					      &tcphdr_len);
		if (!tcph)
<<<<<<< HEAD
			return;

		src = regs->data[priv->sreg];
=======
			goto err;

>>>>>>> upstream/android-13
		offset = i + priv->offset;

		switch (priv->len) {
		case 2:
			old.v16 = get_unaligned((u16 *)(opt + offset));
<<<<<<< HEAD
			new.v16 = src;
=======
			new.v16 = (__force __be16)nft_reg_load16(
				&regs->data[priv->sreg]);
>>>>>>> upstream/android-13

			switch (priv->type) {
			case TCPOPT_MSS:
				/* increase can cause connection to stall */
				if (ntohs(old.v16) <= ntohs(new.v16))
					return;
			break;
			}

			if (old.v16 == new.v16)
				return;

			put_unaligned(new.v16, (u16*)(opt + offset));
			inet_proto_csum_replace2(&tcph->check, pkt->skb,
						 old.v16, new.v16, false);
			break;
		case 4:
<<<<<<< HEAD
			new.v32 = src;
=======
			new.v32 = regs->data[priv->sreg];
>>>>>>> upstream/android-13
			old.v32 = get_unaligned((u32 *)(opt + offset));

			if (old.v32 == new.v32)
				return;

			put_unaligned(new.v32, (u32*)(opt + offset));
			inet_proto_csum_replace4(&tcph->check, pkt->skb,
						 old.v32, new.v32, false);
			break;
		default:
			WARN_ON_ONCE(1);
			break;
		}

		return;
	}
<<<<<<< HEAD
=======
	return;
err:
	regs->verdict.code = NFT_BREAK;
}

static void nft_exthdr_sctp_eval(const struct nft_expr *expr,
				 struct nft_regs *regs,
				 const struct nft_pktinfo *pkt)
{
	unsigned int offset = nft_thoff(pkt) + sizeof(struct sctphdr);
	struct nft_exthdr *priv = nft_expr_priv(expr);
	u32 *dest = &regs->data[priv->dreg];
	const struct sctp_chunkhdr *sch;
	struct sctp_chunkhdr _sch;

	if (pkt->tprot != IPPROTO_SCTP)
		goto err;

	do {
		sch = skb_header_pointer(pkt->skb, offset, sizeof(_sch), &_sch);
		if (!sch || !sch->length)
			break;

		if (sch->type == priv->type) {
			if (priv->flags & NFT_EXTHDR_F_PRESENT) {
				nft_reg_store8(dest, true);
				return;
			}
			if (priv->offset + priv->len > ntohs(sch->length) ||
			    offset + ntohs(sch->length) > pkt->skb->len)
				break;

			dest[priv->len / NFT_REG32_SIZE] = 0;
			if (skb_copy_bits(pkt->skb, offset + priv->offset,
					  dest, priv->len) < 0)
				break;
			return;
		}
		offset += SCTP_PAD4(ntohs(sch->length));
	} while (offset < pkt->skb->len);
err:
	if (priv->flags & NFT_EXTHDR_F_PRESENT)
		nft_reg_store8(dest, false);
	else
		regs->verdict.code = NFT_BREAK;
>>>>>>> upstream/android-13
}

static const struct nla_policy nft_exthdr_policy[NFTA_EXTHDR_MAX + 1] = {
	[NFTA_EXTHDR_DREG]		= { .type = NLA_U32 },
	[NFTA_EXTHDR_TYPE]		= { .type = NLA_U8 },
	[NFTA_EXTHDR_OFFSET]		= { .type = NLA_U32 },
	[NFTA_EXTHDR_LEN]		= { .type = NLA_U32 },
	[NFTA_EXTHDR_FLAGS]		= { .type = NLA_U32 },
	[NFTA_EXTHDR_OP]		= { .type = NLA_U32 },
	[NFTA_EXTHDR_SREG]		= { .type = NLA_U32 },
};

static int nft_exthdr_init(const struct nft_ctx *ctx,
			   const struct nft_expr *expr,
			   const struct nlattr * const tb[])
{
	struct nft_exthdr *priv = nft_expr_priv(expr);
	u32 offset, len, flags = 0, op = NFT_EXTHDR_OP_IPV6;
	int err;

	if (!tb[NFTA_EXTHDR_DREG] ||
	    !tb[NFTA_EXTHDR_TYPE] ||
	    !tb[NFTA_EXTHDR_OFFSET] ||
	    !tb[NFTA_EXTHDR_LEN])
		return -EINVAL;

	err = nft_parse_u32_check(tb[NFTA_EXTHDR_OFFSET], U8_MAX, &offset);
	if (err < 0)
		return err;

	err = nft_parse_u32_check(tb[NFTA_EXTHDR_LEN], U8_MAX, &len);
	if (err < 0)
		return err;

	if (tb[NFTA_EXTHDR_FLAGS]) {
		err = nft_parse_u32_check(tb[NFTA_EXTHDR_FLAGS], U8_MAX, &flags);
		if (err < 0)
			return err;

		if (flags & ~NFT_EXTHDR_F_PRESENT)
			return -EINVAL;
	}

	if (tb[NFTA_EXTHDR_OP]) {
		err = nft_parse_u32_check(tb[NFTA_EXTHDR_OP], U8_MAX, &op);
		if (err < 0)
			return err;
	}

	priv->type   = nla_get_u8(tb[NFTA_EXTHDR_TYPE]);
	priv->offset = offset;
	priv->len    = len;
<<<<<<< HEAD
	priv->dreg   = nft_parse_register(tb[NFTA_EXTHDR_DREG]);
	priv->flags  = flags;
	priv->op     = op;

	return nft_validate_register_store(ctx, priv->dreg, NULL,
					   NFT_DATA_VALUE, priv->len);
=======
	priv->flags  = flags;
	priv->op     = op;

	return nft_parse_register_store(ctx, tb[NFTA_EXTHDR_DREG],
					&priv->dreg, NULL, NFT_DATA_VALUE,
					priv->len);
>>>>>>> upstream/android-13
}

static int nft_exthdr_tcp_set_init(const struct nft_ctx *ctx,
				   const struct nft_expr *expr,
				   const struct nlattr * const tb[])
{
	struct nft_exthdr *priv = nft_expr_priv(expr);
	u32 offset, len, flags = 0, op = NFT_EXTHDR_OP_IPV6;
	int err;

	if (!tb[NFTA_EXTHDR_SREG] ||
	    !tb[NFTA_EXTHDR_TYPE] ||
	    !tb[NFTA_EXTHDR_OFFSET] ||
	    !tb[NFTA_EXTHDR_LEN])
		return -EINVAL;

	if (tb[NFTA_EXTHDR_DREG] || tb[NFTA_EXTHDR_FLAGS])
		return -EINVAL;

	err = nft_parse_u32_check(tb[NFTA_EXTHDR_OFFSET], U8_MAX, &offset);
	if (err < 0)
		return err;

	err = nft_parse_u32_check(tb[NFTA_EXTHDR_LEN], U8_MAX, &len);
	if (err < 0)
		return err;

	if (offset < 2)
		return -EOPNOTSUPP;

	switch (len) {
	case 2: break;
	case 4: break;
	default:
		return -EOPNOTSUPP;
	}

	err = nft_parse_u32_check(tb[NFTA_EXTHDR_OP], U8_MAX, &op);
	if (err < 0)
		return err;

	priv->type   = nla_get_u8(tb[NFTA_EXTHDR_TYPE]);
	priv->offset = offset;
	priv->len    = len;
<<<<<<< HEAD
	priv->sreg   = nft_parse_register(tb[NFTA_EXTHDR_SREG]);
	priv->flags  = flags;
	priv->op     = op;

	return nft_validate_register_load(priv->sreg, priv->len);
=======
	priv->flags  = flags;
	priv->op     = op;

	return nft_parse_register_load(tb[NFTA_EXTHDR_SREG], &priv->sreg,
				       priv->len);
}

static int nft_exthdr_ipv4_init(const struct nft_ctx *ctx,
				const struct nft_expr *expr,
				const struct nlattr * const tb[])
{
	struct nft_exthdr *priv = nft_expr_priv(expr);
	int err = nft_exthdr_init(ctx, expr, tb);

	if (err < 0)
		return err;

	switch (priv->type) {
	case IPOPT_SSRR:
	case IPOPT_LSRR:
	case IPOPT_RR:
	case IPOPT_RA:
		break;
	default:
		return -EOPNOTSUPP;
	}
	return 0;
>>>>>>> upstream/android-13
}

static int nft_exthdr_dump_common(struct sk_buff *skb, const struct nft_exthdr *priv)
{
	if (nla_put_u8(skb, NFTA_EXTHDR_TYPE, priv->type))
		goto nla_put_failure;
	if (nla_put_be32(skb, NFTA_EXTHDR_OFFSET, htonl(priv->offset)))
		goto nla_put_failure;
	if (nla_put_be32(skb, NFTA_EXTHDR_LEN, htonl(priv->len)))
		goto nla_put_failure;
	if (nla_put_be32(skb, NFTA_EXTHDR_FLAGS, htonl(priv->flags)))
		goto nla_put_failure;
	if (nla_put_be32(skb, NFTA_EXTHDR_OP, htonl(priv->op)))
		goto nla_put_failure;
	return 0;

nla_put_failure:
	return -1;
}

static int nft_exthdr_dump(struct sk_buff *skb, const struct nft_expr *expr)
{
	const struct nft_exthdr *priv = nft_expr_priv(expr);

	if (nft_dump_register(skb, NFTA_EXTHDR_DREG, priv->dreg))
		return -1;

	return nft_exthdr_dump_common(skb, priv);
}

static int nft_exthdr_dump_set(struct sk_buff *skb, const struct nft_expr *expr)
{
	const struct nft_exthdr *priv = nft_expr_priv(expr);

	if (nft_dump_register(skb, NFTA_EXTHDR_SREG, priv->sreg))
		return -1;

	return nft_exthdr_dump_common(skb, priv);
}

static const struct nft_expr_ops nft_exthdr_ipv6_ops = {
	.type		= &nft_exthdr_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_exthdr)),
	.eval		= nft_exthdr_ipv6_eval,
	.init		= nft_exthdr_init,
	.dump		= nft_exthdr_dump,
};

<<<<<<< HEAD
=======
static const struct nft_expr_ops nft_exthdr_ipv4_ops = {
	.type		= &nft_exthdr_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_exthdr)),
	.eval		= nft_exthdr_ipv4_eval,
	.init		= nft_exthdr_ipv4_init,
	.dump		= nft_exthdr_dump,
};

>>>>>>> upstream/android-13
static const struct nft_expr_ops nft_exthdr_tcp_ops = {
	.type		= &nft_exthdr_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_exthdr)),
	.eval		= nft_exthdr_tcp_eval,
	.init		= nft_exthdr_init,
	.dump		= nft_exthdr_dump,
};

static const struct nft_expr_ops nft_exthdr_tcp_set_ops = {
	.type		= &nft_exthdr_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_exthdr)),
	.eval		= nft_exthdr_tcp_set_eval,
	.init		= nft_exthdr_tcp_set_init,
	.dump		= nft_exthdr_dump_set,
};

<<<<<<< HEAD
=======
static const struct nft_expr_ops nft_exthdr_sctp_ops = {
	.type		= &nft_exthdr_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_exthdr)),
	.eval		= nft_exthdr_sctp_eval,
	.init		= nft_exthdr_init,
	.dump		= nft_exthdr_dump,
};

>>>>>>> upstream/android-13
static const struct nft_expr_ops *
nft_exthdr_select_ops(const struct nft_ctx *ctx,
		      const struct nlattr * const tb[])
{
	u32 op;

	if (!tb[NFTA_EXTHDR_OP])
		return &nft_exthdr_ipv6_ops;

	if (tb[NFTA_EXTHDR_SREG] && tb[NFTA_EXTHDR_DREG])
		return ERR_PTR(-EOPNOTSUPP);

	op = ntohl(nla_get_be32(tb[NFTA_EXTHDR_OP]));
	switch (op) {
	case NFT_EXTHDR_OP_TCPOPT:
		if (tb[NFTA_EXTHDR_SREG])
			return &nft_exthdr_tcp_set_ops;
		if (tb[NFTA_EXTHDR_DREG])
			return &nft_exthdr_tcp_ops;
		break;
	case NFT_EXTHDR_OP_IPV6:
		if (tb[NFTA_EXTHDR_DREG])
			return &nft_exthdr_ipv6_ops;
		break;
<<<<<<< HEAD
=======
	case NFT_EXTHDR_OP_IPV4:
		if (ctx->family != NFPROTO_IPV6) {
			if (tb[NFTA_EXTHDR_DREG])
				return &nft_exthdr_ipv4_ops;
		}
		break;
	case NFT_EXTHDR_OP_SCTP:
		if (tb[NFTA_EXTHDR_DREG])
			return &nft_exthdr_sctp_ops;
		break;
>>>>>>> upstream/android-13
	}

	return ERR_PTR(-EOPNOTSUPP);
}

struct nft_expr_type nft_exthdr_type __read_mostly = {
	.name		= "exthdr",
	.select_ops	= nft_exthdr_select_ops,
	.policy		= nft_exthdr_policy,
	.maxattr	= NFTA_EXTHDR_MAX,
	.owner		= THIS_MODULE,
};
