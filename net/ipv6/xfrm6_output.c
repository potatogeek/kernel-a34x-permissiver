<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * xfrm6_output.c - Common IPsec encapsulation code for IPv6.
 * Copyright (C) 2002 USAGI/WIDE Project
 * Copyright (c) 2004 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/if_ether.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/icmpv6.h>
#include <linux/netfilter_ipv6.h>
#include <net/dst.h>
#include <net/ipv6.h>
#include <net/ip6_route.h>
#include <net/xfrm.h>

<<<<<<< HEAD
int xfrm6_find_1stfragopt(struct xfrm_state *x, struct sk_buff *skb,
			  u8 **prevhdr)
{
	return ip6_find_1stfragopt(skb, prevhdr);
}
EXPORT_SYMBOL(xfrm6_find_1stfragopt);

static int xfrm6_local_dontfrag(struct sk_buff *skb)
{
	int proto;
	struct sock *sk = skb->sk;

	if (sk) {
		if (sk->sk_family != AF_INET6)
			return 0;

		proto = sk->sk_protocol;
		if (proto == IPPROTO_UDP || proto == IPPROTO_RAW)
			return inet6_sk(sk)->dontfrag;
	}

	return 0;
}

static void xfrm6_local_rxpmtu(struct sk_buff *skb, u32 mtu)
=======
void xfrm6_local_rxpmtu(struct sk_buff *skb, u32 mtu)
>>>>>>> upstream/android-13
{
	struct flowi6 fl6;
	struct sock *sk = skb->sk;

	fl6.flowi6_oif = sk->sk_bound_dev_if;
	fl6.daddr = ipv6_hdr(skb)->daddr;

	ipv6_local_rxpmtu(sk, &fl6, mtu);
}

void xfrm6_local_error(struct sk_buff *skb, u32 mtu)
{
	struct flowi6 fl6;
	const struct ipv6hdr *hdr;
	struct sock *sk = skb->sk;

	hdr = skb->encapsulation ? inner_ipv6_hdr(skb) : ipv6_hdr(skb);
	fl6.fl6_dport = inet_sk(sk)->inet_dport;
	fl6.daddr = hdr->daddr;

	ipv6_local_error(sk, EMSGSIZE, &fl6, mtu);
}

<<<<<<< HEAD
static int xfrm6_tunnel_check_size(struct sk_buff *skb)
{
	int mtu, ret = 0;
	struct dst_entry *dst = skb_dst(skb);

	if (skb->ignore_df)
		goto out;

	mtu = dst_mtu(dst);
	if (mtu < IPV6_MIN_MTU)
		mtu = IPV6_MIN_MTU;

	if ((!skb_is_gso(skb) && skb->len > mtu) ||
	    (skb_is_gso(skb) &&
	     !skb_gso_validate_network_len(skb, ip6_skb_dst_mtu(skb)))) {
		skb->dev = dst->dev;
		skb->protocol = htons(ETH_P_IPV6);

		if (xfrm6_local_dontfrag(skb))
			xfrm6_local_rxpmtu(skb, mtu);
		else if (skb->sk)
			xfrm_local_error(skb, mtu);
		else
			icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
		ret = -EMSGSIZE;
	}
out:
	return ret;
}

int xfrm6_extract_output(struct xfrm_state *x, struct sk_buff *skb)
{
	int err;

	err = xfrm6_tunnel_check_size(skb);
	if (err)
		return err;

	XFRM_MODE_SKB_CB(skb)->protocol = ipv6_hdr(skb)->nexthdr;

	return xfrm6_extract_header(skb);
}

int xfrm6_prepare_output(struct xfrm_state *x, struct sk_buff *skb)
{
	int err;

	err = xfrm_inner_extract_output(x, skb);
	if (err)
		return err;

	skb->ignore_df = 1;
	skb->protocol = htons(ETH_P_IPV6);

	return x->outer_mode->output2(x, skb);
}
EXPORT_SYMBOL(xfrm6_prepare_output);

int xfrm6_output_finish(struct sock *sk, struct sk_buff *skb)
{
	memset(IP6CB(skb), 0, sizeof(*IP6CB(skb)));

	IP6CB(skb)->flags |= IP6SKB_XFRM_TRANSFORMED;

	return xfrm_output(sk, skb);
}

static int __xfrm6_output_finish(struct net *net, struct sock *sk, struct sk_buff *skb)
{
	struct xfrm_state *x = skb_dst(skb)->xfrm;

	return x->outer_mode->afinfo->output_finish(sk, skb);
=======
static int __xfrm6_output_finish(struct net *net, struct sock *sk, struct sk_buff *skb)
{
	return xfrm_output(sk, skb);
}

static int xfrm6_noneed_fragment(struct sk_buff *skb)
{
	struct frag_hdr *fh;
	u8 prevhdr = ipv6_hdr(skb)->nexthdr;

	if (prevhdr != NEXTHDR_FRAGMENT)
		return 0;
	fh = (struct frag_hdr *)(skb->data + sizeof(struct ipv6hdr));
	if (fh->nexthdr == NEXTHDR_ESP || fh->nexthdr == NEXTHDR_AUTH)
		return 1;
	return 0;
>>>>>>> upstream/android-13
}

static int __xfrm6_output(struct net *net, struct sock *sk, struct sk_buff *skb)
{
	struct dst_entry *dst = skb_dst(skb);
	struct xfrm_state *x = dst->xfrm;
<<<<<<< HEAD
	int mtu;
=======
	unsigned int mtu;
>>>>>>> upstream/android-13
	bool toobig;

#ifdef CONFIG_NETFILTER
	if (!x) {
		IP6CB(skb)->flags |= IP6SKB_REROUTED;
		return dst_output(net, sk, skb);
	}
#endif

	if (x->props.mode != XFRM_MODE_TUNNEL)
		goto skip_frag;

	if (skb->protocol == htons(ETH_P_IPV6))
		mtu = ip6_skb_dst_mtu(skb);
	else
		mtu = dst_mtu(skb_dst(skb));

	toobig = skb->len > mtu && !skb_is_gso(skb);

<<<<<<< HEAD
	if (toobig && xfrm6_local_dontfrag(skb)) {
		xfrm6_local_rxpmtu(skb, mtu);
		kfree_skb(skb);
		return -EMSGSIZE;
=======
	if (toobig && xfrm6_local_dontfrag(skb->sk)) {
		xfrm6_local_rxpmtu(skb, mtu);
		kfree_skb(skb);
		return -EMSGSIZE;
	} else if (toobig && xfrm6_noneed_fragment(skb)) {
		skb->ignore_df = 1;
		goto skip_frag;
>>>>>>> upstream/android-13
	} else if (!skb->ignore_df && toobig && skb->sk) {
		xfrm_local_error(skb, mtu);
		kfree_skb(skb);
		return -EMSGSIZE;
	}

	if (toobig || dst_allfrag(skb_dst(skb)))
		return ip6_fragment(net, sk, skb,
				    __xfrm6_output_finish);

skip_frag:
<<<<<<< HEAD
	return x->outer_mode->afinfo->output_finish(sk, skb);
=======
	return xfrm_output(sk, skb);
>>>>>>> upstream/android-13
}

int xfrm6_output(struct net *net, struct sock *sk, struct sk_buff *skb)
{
	return NF_HOOK_COND(NFPROTO_IPV6, NF_INET_POST_ROUTING,
<<<<<<< HEAD
			    net, sk, skb,  NULL, skb_dst(skb)->dev,
=======
			    net, sk, skb,  skb->dev, skb_dst(skb)->dev,
>>>>>>> upstream/android-13
			    __xfrm6_output,
			    !(IP6CB(skb)->flags & IP6SKB_REROUTED));
}
