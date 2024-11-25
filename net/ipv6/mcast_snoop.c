<<<<<<< HEAD
/* Copyright (C) 2010: YOSHIFUJI Hideaki <yoshfuji@linux-ipv6.org>
 * Copyright (C) 2015: Linus Lüssing <linus.luessing@c0d3.blue>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (C) 2010: YOSHIFUJI Hideaki <yoshfuji@linux-ipv6.org>
 * Copyright (C) 2015: Linus Lüssing <linus.luessing@c0d3.blue>
 *
>>>>>>> upstream/android-13
 * Based on the MLD support added to br_multicast.c by YOSHIFUJI Hideaki.
 */

#include <linux/skbuff.h>
#include <net/ipv6.h>
#include <net/mld.h>
#include <net/addrconf.h>
#include <net/ip6_checksum.h>

static int ipv6_mc_check_ip6hdr(struct sk_buff *skb)
{
	const struct ipv6hdr *ip6h;
	unsigned int len;
	unsigned int offset = skb_network_offset(skb) + sizeof(*ip6h);

	if (!pskb_may_pull(skb, offset))
		return -EINVAL;

	ip6h = ipv6_hdr(skb);

	if (ip6h->version != 6)
		return -EINVAL;

	len = offset + ntohs(ip6h->payload_len);
	if (skb->len < len || len <= offset)
		return -EINVAL;

<<<<<<< HEAD
=======
	skb_set_transport_header(skb, offset);

>>>>>>> upstream/android-13
	return 0;
}

static int ipv6_mc_check_exthdrs(struct sk_buff *skb)
{
	const struct ipv6hdr *ip6h;
	int offset;
	u8 nexthdr;
	__be16 frag_off;

	ip6h = ipv6_hdr(skb);

	if (ip6h->nexthdr != IPPROTO_HOPOPTS)
		return -ENOMSG;

	nexthdr = ip6h->nexthdr;
	offset = skb_network_offset(skb) + sizeof(*ip6h);
	offset = ipv6_skip_exthdr(skb, offset, &nexthdr, &frag_off);

	if (offset < 0)
		return -EINVAL;

	if (nexthdr != IPPROTO_ICMPV6)
		return -ENOMSG;

	skb_set_transport_header(skb, offset);

	return 0;
}

static int ipv6_mc_check_mld_reportv2(struct sk_buff *skb)
{
	unsigned int len = skb_transport_offset(skb);

	len += sizeof(struct mld2_report);

<<<<<<< HEAD
	return pskb_may_pull(skb, len) ? 0 : -EINVAL;
=======
	return ipv6_mc_may_pull(skb, len) ? 0 : -EINVAL;
>>>>>>> upstream/android-13
}

static int ipv6_mc_check_mld_query(struct sk_buff *skb)
{
<<<<<<< HEAD
	struct mld_msg *mld;
	unsigned int len = skb_transport_offset(skb);
=======
	unsigned int transport_len = ipv6_transport_len(skb);
	struct mld_msg *mld;
	unsigned int len;
>>>>>>> upstream/android-13

	/* RFC2710+RFC3810 (MLDv1+MLDv2) require link-local source addresses */
	if (!(ipv6_addr_type(&ipv6_hdr(skb)->saddr) & IPV6_ADDR_LINKLOCAL))
		return -EINVAL;

<<<<<<< HEAD
	len += sizeof(struct mld_msg);
	if (skb->len < len)
		return -EINVAL;

	/* MLDv1? */
	if (skb->len != len) {
		/* or MLDv2? */
		len += sizeof(struct mld2_query) - sizeof(struct mld_msg);
		if (skb->len < len || !pskb_may_pull(skb, len))
=======
	/* MLDv1? */
	if (transport_len != sizeof(struct mld_msg)) {
		/* or MLDv2? */
		if (transport_len < sizeof(struct mld2_query))
			return -EINVAL;

		len = skb_transport_offset(skb) + sizeof(struct mld2_query);
		if (!ipv6_mc_may_pull(skb, len))
>>>>>>> upstream/android-13
			return -EINVAL;
	}

	mld = (struct mld_msg *)skb_transport_header(skb);

	/* RFC2710+RFC3810 (MLDv1+MLDv2) require the multicast link layer
	 * all-nodes destination address (ff02::1) for general queries
	 */
	if (ipv6_addr_any(&mld->mld_mca) &&
	    !ipv6_addr_is_ll_all_nodes(&ipv6_hdr(skb)->daddr))
		return -EINVAL;

	return 0;
}

static int ipv6_mc_check_mld_msg(struct sk_buff *skb)
{
<<<<<<< HEAD
	struct mld_msg *mld = (struct mld_msg *)skb_transport_header(skb);
=======
	unsigned int len = skb_transport_offset(skb) + sizeof(struct mld_msg);
	struct mld_msg *mld;

	if (!ipv6_mc_may_pull(skb, len))
		return -ENODATA;

	mld = (struct mld_msg *)skb_transport_header(skb);
>>>>>>> upstream/android-13

	switch (mld->mld_type) {
	case ICMPV6_MGM_REDUCTION:
	case ICMPV6_MGM_REPORT:
<<<<<<< HEAD
		/* fall through */
=======
>>>>>>> upstream/android-13
		return 0;
	case ICMPV6_MLD2_REPORT:
		return ipv6_mc_check_mld_reportv2(skb);
	case ICMPV6_MGM_QUERY:
		return ipv6_mc_check_mld_query(skb);
	default:
<<<<<<< HEAD
		return -ENOMSG;
=======
		return -ENODATA;
>>>>>>> upstream/android-13
	}
}

static inline __sum16 ipv6_mc_validate_checksum(struct sk_buff *skb)
{
	return skb_checksum_validate(skb, IPPROTO_ICMPV6, ip6_compute_pseudo);
}

<<<<<<< HEAD
static int __ipv6_mc_check_mld(struct sk_buff *skb,
			       struct sk_buff **skb_trimmed)

{
	struct sk_buff *skb_chk = NULL;
	unsigned int transport_len;
	unsigned int len = skb_transport_offset(skb) + sizeof(struct mld_msg);
	int ret = -EINVAL;

	transport_len = ntohs(ipv6_hdr(skb)->payload_len);
	transport_len -= skb_transport_offset(skb) - sizeof(struct ipv6hdr);
=======
static int ipv6_mc_check_icmpv6(struct sk_buff *skb)
{
	unsigned int len = skb_transport_offset(skb) + sizeof(struct icmp6hdr);
	unsigned int transport_len = ipv6_transport_len(skb);
	struct sk_buff *skb_chk;

	if (!ipv6_mc_may_pull(skb, len))
		return -EINVAL;
>>>>>>> upstream/android-13

	skb_chk = skb_checksum_trimmed(skb, transport_len,
				       ipv6_mc_validate_checksum);
	if (!skb_chk)
<<<<<<< HEAD
		goto err;

	if (!pskb_may_pull(skb_chk, len))
		goto err;

	ret = ipv6_mc_check_mld_msg(skb_chk);
	if (ret)
		goto err;

	if (skb_trimmed)
		*skb_trimmed = skb_chk;
	/* free now unneeded clone */
	else if (skb_chk != skb)
		kfree_skb(skb_chk);

	ret = 0;

err:
	if (ret && skb_chk && skb_chk != skb)
		kfree_skb(skb_chk);

	return ret;
=======
		return -EINVAL;

	if (skb_chk != skb)
		kfree_skb(skb_chk);

	return 0;
>>>>>>> upstream/android-13
}

/**
 * ipv6_mc_check_mld - checks whether this is a sane MLD packet
 * @skb: the skb to validate
<<<<<<< HEAD
 * @skb_trimmed: to store an skb pointer trimmed to IPv6 packet tail (optional)
=======
>>>>>>> upstream/android-13
 *
 * Checks whether an IPv6 packet is a valid MLD packet. If so sets
 * skb transport header accordingly and returns zero.
 *
 * -EINVAL: A broken packet was detected, i.e. it violates some internet
 *  standard
<<<<<<< HEAD
 * -ENOMSG: IP header validation succeeded but it is not an MLD packet.
 * -ENOMEM: A memory allocation failure happened.
 *
 * Optionally, an skb pointer might be provided via skb_trimmed (or set it
 * to NULL): After parsing an MLD packet successfully it will point to
 * an skb which has its tail aligned to the IP packet end. This might
 * either be the originally provided skb or a trimmed, cloned version if
 * the skb frame had data beyond the IP packet. A cloned skb allows us
 * to leave the original skb and its full frame unchanged (which might be
 * desirable for layer 2 frame jugglers).
 *
 * Caller needs to set the skb network header and free any returned skb if it
 * differs from the provided skb.
 */
int ipv6_mc_check_mld(struct sk_buff *skb, struct sk_buff **skb_trimmed)
=======
 * -ENOMSG: IP header validation succeeded but it is not an ICMPv6 packet
 *  with a hop-by-hop option.
 * -ENODATA: IP+ICMPv6 header with hop-by-hop option validation succeeded
 *  but it is not an MLD packet.
 * -ENOMEM: A memory allocation failure happened.
 *
 * Caller needs to set the skb network header and free any returned skb if it
 * differs from the provided skb.
 */
int ipv6_mc_check_mld(struct sk_buff *skb)
>>>>>>> upstream/android-13
{
	int ret;

	ret = ipv6_mc_check_ip6hdr(skb);
	if (ret < 0)
		return ret;

	ret = ipv6_mc_check_exthdrs(skb);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	return __ipv6_mc_check_mld(skb, skb_trimmed);
=======
	ret = ipv6_mc_check_icmpv6(skb);
	if (ret < 0)
		return ret;

	return ipv6_mc_check_mld_msg(skb);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(ipv6_mc_check_mld);
