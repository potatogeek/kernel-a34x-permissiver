/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_TABLES_IPV6_H_
#define _NF_TABLES_IPV6_H_

#include <linux/netfilter_ipv6/ip6_tables.h>
#include <net/ipv6.h>
<<<<<<< HEAD

static inline void nft_set_pktinfo_ipv6(struct nft_pktinfo *pkt,
					struct sk_buff *skb)
=======
#include <net/netfilter/nf_tables.h>

static inline void nft_set_pktinfo_ipv6(struct nft_pktinfo *pkt)
>>>>>>> upstream/android-13
{
	unsigned int flags = IP6_FH_F_AUTH;
	int protohdr, thoff = 0;
	unsigned short frag_off;

	protohdr = ipv6_find_hdr(pkt->skb, &thoff, -1, &frag_off, &flags);
	if (protohdr < 0) {
<<<<<<< HEAD
		nft_set_pktinfo_unspec(pkt, skb);
=======
		nft_set_pktinfo_unspec(pkt);
>>>>>>> upstream/android-13
		return;
	}

	pkt->tprot_set = true;
	pkt->tprot = protohdr;
<<<<<<< HEAD
	pkt->xt.thoff = thoff;
	pkt->xt.fragoff = frag_off;
}

static inline int __nft_set_pktinfo_ipv6_validate(struct nft_pktinfo *pkt,
						  struct sk_buff *skb)
=======
	pkt->thoff = thoff;
	pkt->fragoff = frag_off;
}

static inline int __nft_set_pktinfo_ipv6_validate(struct nft_pktinfo *pkt)
>>>>>>> upstream/android-13
{
#if IS_ENABLED(CONFIG_IPV6)
	unsigned int flags = IP6_FH_F_AUTH;
	struct ipv6hdr *ip6h, _ip6h;
	unsigned int thoff = 0;
	unsigned short frag_off;
	int protohdr;
	u32 pkt_len;

<<<<<<< HEAD
	ip6h = skb_header_pointer(skb, skb_network_offset(skb), sizeof(*ip6h),
				  &_ip6h);
=======
	ip6h = skb_header_pointer(pkt->skb, skb_network_offset(pkt->skb),
				  sizeof(*ip6h), &_ip6h);
>>>>>>> upstream/android-13
	if (!ip6h)
		return -1;

	if (ip6h->version != 6)
		return -1;

	pkt_len = ntohs(ip6h->payload_len);
<<<<<<< HEAD
	if (pkt_len + sizeof(*ip6h) > skb->len)
=======
	if (pkt_len + sizeof(*ip6h) > pkt->skb->len)
>>>>>>> upstream/android-13
		return -1;

	protohdr = ipv6_find_hdr(pkt->skb, &thoff, -1, &frag_off, &flags);
	if (protohdr < 0)
		return -1;

	pkt->tprot_set = true;
	pkt->tprot = protohdr;
<<<<<<< HEAD
	pkt->xt.thoff = thoff;
	pkt->xt.fragoff = frag_off;
=======
	pkt->thoff = thoff;
	pkt->fragoff = frag_off;
>>>>>>> upstream/android-13

	return 0;
#else
	return -1;
#endif
}

<<<<<<< HEAD
static inline void nft_set_pktinfo_ipv6_validate(struct nft_pktinfo *pkt,
						 struct sk_buff *skb)
{
	if (__nft_set_pktinfo_ipv6_validate(pkt, skb) < 0)
		nft_set_pktinfo_unspec(pkt, skb);
=======
static inline void nft_set_pktinfo_ipv6_validate(struct nft_pktinfo *pkt)
{
	if (__nft_set_pktinfo_ipv6_validate(pkt) < 0)
		nft_set_pktinfo_unspec(pkt);
}

static inline int nft_set_pktinfo_ipv6_ingress(struct nft_pktinfo *pkt)
{
#if IS_ENABLED(CONFIG_IPV6)
	unsigned int flags = IP6_FH_F_AUTH;
	unsigned short frag_off;
	unsigned int thoff = 0;
	struct inet6_dev *idev;
	struct ipv6hdr *ip6h;
	int protohdr;
	u32 pkt_len;

	if (!pskb_may_pull(pkt->skb, sizeof(*ip6h)))
		return -1;

	ip6h = ipv6_hdr(pkt->skb);
	if (ip6h->version != 6)
		goto inhdr_error;

	pkt_len = ntohs(ip6h->payload_len);
	if (pkt_len + sizeof(*ip6h) > pkt->skb->len) {
		idev = __in6_dev_get(nft_in(pkt));
		__IP6_INC_STATS(nft_net(pkt), idev, IPSTATS_MIB_INTRUNCATEDPKTS);
		return -1;
	}

	protohdr = ipv6_find_hdr(pkt->skb, &thoff, -1, &frag_off, &flags);
	if (protohdr < 0)
		goto inhdr_error;

	pkt->tprot_set = true;
	pkt->tprot = protohdr;
	pkt->thoff = thoff;
	pkt->fragoff = frag_off;

	return 0;

inhdr_error:
	idev = __in6_dev_get(nft_in(pkt));
	__IP6_INC_STATS(nft_net(pkt), idev, IPSTATS_MIB_INHDRERRORS);
	return -1;
#else
	return -1;
#endif
>>>>>>> upstream/android-13
}

#endif
