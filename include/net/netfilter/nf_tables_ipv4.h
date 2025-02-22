/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_TABLES_IPV4_H_
#define _NF_TABLES_IPV4_H_

#include <net/netfilter/nf_tables.h>
#include <net/ip.h>

<<<<<<< HEAD
static inline void nft_set_pktinfo_ipv4(struct nft_pktinfo *pkt,
					struct sk_buff *skb)
=======
static inline void nft_set_pktinfo_ipv4(struct nft_pktinfo *pkt)
>>>>>>> upstream/android-13
{
	struct iphdr *ip;

	ip = ip_hdr(pkt->skb);
	pkt->tprot_set = true;
	pkt->tprot = ip->protocol;
<<<<<<< HEAD
	pkt->xt.thoff = ip_hdrlen(pkt->skb);
	pkt->xt.fragoff = ntohs(ip->frag_off) & IP_OFFSET;
}

static inline int __nft_set_pktinfo_ipv4_validate(struct nft_pktinfo *pkt,
						  struct sk_buff *skb)
=======
	pkt->thoff = ip_hdrlen(pkt->skb);
	pkt->fragoff = ntohs(ip->frag_off) & IP_OFFSET;
}

static inline int __nft_set_pktinfo_ipv4_validate(struct nft_pktinfo *pkt)
>>>>>>> upstream/android-13
{
	struct iphdr *iph, _iph;
	u32 len, thoff;

<<<<<<< HEAD
	iph = skb_header_pointer(skb, skb_network_offset(skb), sizeof(*iph),
				 &_iph);
=======
	iph = skb_header_pointer(pkt->skb, skb_network_offset(pkt->skb),
				 sizeof(*iph), &_iph);
>>>>>>> upstream/android-13
	if (!iph)
		return -1;

	if (iph->ihl < 5 || iph->version != 4)
		return -1;

	len = ntohs(iph->tot_len);
	thoff = iph->ihl * 4;
<<<<<<< HEAD
	if (skb->len < len)
=======
	if (pkt->skb->len < len)
>>>>>>> upstream/android-13
		return -1;
	else if (len < thoff)
		return -1;

	pkt->tprot_set = true;
	pkt->tprot = iph->protocol;
<<<<<<< HEAD
	pkt->xt.thoff = thoff;
	pkt->xt.fragoff = ntohs(iph->frag_off) & IP_OFFSET;
=======
	pkt->thoff = thoff;
	pkt->fragoff = ntohs(iph->frag_off) & IP_OFFSET;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static inline void nft_set_pktinfo_ipv4_validate(struct nft_pktinfo *pkt,
						 struct sk_buff *skb)
{
	if (__nft_set_pktinfo_ipv4_validate(pkt, skb) < 0)
		nft_set_pktinfo_unspec(pkt, skb);
}

=======
static inline void nft_set_pktinfo_ipv4_validate(struct nft_pktinfo *pkt)
{
	if (__nft_set_pktinfo_ipv4_validate(pkt) < 0)
		nft_set_pktinfo_unspec(pkt);
}

static inline int nft_set_pktinfo_ipv4_ingress(struct nft_pktinfo *pkt)
{
	struct iphdr *iph;
	u32 len, thoff;

	if (!pskb_may_pull(pkt->skb, sizeof(*iph)))
		return -1;

	iph = ip_hdr(pkt->skb);
	if (iph->ihl < 5 || iph->version != 4)
		goto inhdr_error;

	len = ntohs(iph->tot_len);
	thoff = iph->ihl * 4;
	if (pkt->skb->len < len) {
		__IP_INC_STATS(nft_net(pkt), IPSTATS_MIB_INTRUNCATEDPKTS);
		return -1;
	} else if (len < thoff) {
		goto inhdr_error;
	}

	pkt->tprot_set = true;
	pkt->tprot = iph->protocol;
	pkt->thoff = thoff;
	pkt->fragoff = ntohs(iph->frag_off) & IP_OFFSET;

	return 0;

inhdr_error:
	__IP_INC_STATS(nft_net(pkt), IPSTATS_MIB_INHDRERRORS);
	return -1;
}
>>>>>>> upstream/android-13
#endif
