<<<<<<< HEAD
/* Copyright (c) 2013-2018, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * RMNET Data MAP protocol
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2013-2018, 2021, The Linux Foundation. All rights reserved.
 *
 * RMNET Data MAP protocol
>>>>>>> upstream/android-13
 */

#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <net/ip6_checksum.h>
<<<<<<< HEAD
=======
#include <linux/bitfield.h>
>>>>>>> upstream/android-13
#include "rmnet_config.h"
#include "rmnet_map.h"
#include "rmnet_private.h"

#define RMNET_MAP_DEAGGR_SPACING  64
#define RMNET_MAP_DEAGGR_HEADROOM (RMNET_MAP_DEAGGR_SPACING / 2)

static __sum16 *rmnet_map_get_csum_field(unsigned char protocol,
					 const void *txporthdr)
{
<<<<<<< HEAD
	__sum16 *check = NULL;

	switch (protocol) {
	case IPPROTO_TCP:
		check = &(((struct tcphdr *)txporthdr)->check);
		break;

	case IPPROTO_UDP:
		check = &(((struct udphdr *)txporthdr)->check);
		break;

	default:
		check = NULL;
		break;
	}

	return check;
=======
	if (protocol == IPPROTO_TCP)
		return &((struct tcphdr *)txporthdr)->check;

	if (protocol == IPPROTO_UDP)
		return &((struct udphdr *)txporthdr)->check;

	return NULL;
>>>>>>> upstream/android-13
}

static int
rmnet_map_ipv4_dl_csum_trailer(struct sk_buff *skb,
			       struct rmnet_map_dl_csum_trailer *csum_trailer,
			       struct rmnet_priv *priv)
{
<<<<<<< HEAD
	__sum16 *csum_field, csum_temp, pseudo_csum, hdr_csum, ip_payload_csum;
	u16 csum_value, csum_value_final;
	struct iphdr *ip4h;
	void *txporthdr;
	__be16 addend;

	ip4h = (struct iphdr *)(skb->data);
	if ((ntohs(ip4h->frag_off) & IP_MF) ||
	    ((ntohs(ip4h->frag_off) & IP_OFFSET) > 0)) {
=======
	struct iphdr *ip4h = (struct iphdr *)skb->data;
	void *txporthdr = skb->data + ip4h->ihl * 4;
	__sum16 *csum_field, pseudo_csum;
	__sum16 ip_payload_csum;

	/* Computing the checksum over just the IPv4 header--including its
	 * checksum field--should yield 0.  If it doesn't, the IP header
	 * is bad, so return an error and let the IP layer drop it.
	 */
	if (ip_fast_csum(ip4h, ip4h->ihl)) {
		priv->stats.csum_ip4_header_bad++;
		return -EINVAL;
	}

	/* We don't support checksum offload on IPv4 fragments */
	if (ip_is_fragment(ip4h)) {
>>>>>>> upstream/android-13
		priv->stats.csum_fragmented_pkt++;
		return -EOPNOTSUPP;
	}

<<<<<<< HEAD
	txporthdr = skb->data + ip4h->ihl * 4;

	csum_field = rmnet_map_get_csum_field(ip4h->protocol, txporthdr);

=======
	/* Checksum offload is only supported for UDP and TCP protocols */
	csum_field = rmnet_map_get_csum_field(ip4h->protocol, txporthdr);
>>>>>>> upstream/android-13
	if (!csum_field) {
		priv->stats.csum_err_invalid_transport++;
		return -EPROTONOSUPPORT;
	}

<<<<<<< HEAD
	/* RFC 768 - Skip IPv4 UDP packets where sender checksum field is 0 */
	if (*csum_field == 0 && ip4h->protocol == IPPROTO_UDP) {
=======
	/* RFC 768: UDP checksum is optional for IPv4, and is 0 if unused */
	if (!*csum_field && ip4h->protocol == IPPROTO_UDP) {
>>>>>>> upstream/android-13
		priv->stats.csum_skipped++;
		return 0;
	}

<<<<<<< HEAD
	csum_value = ~ntohs(csum_trailer->csum_value);
	hdr_csum = ~ip_fast_csum(ip4h, (int)ip4h->ihl);
	ip_payload_csum = csum16_sub((__force __sum16)csum_value,
				     (__force __be16)hdr_csum);

	pseudo_csum = ~csum_tcpudp_magic(ip4h->saddr, ip4h->daddr,
					 ntohs(ip4h->tot_len) - ip4h->ihl * 4,
					 ip4h->protocol, 0);
	addend = (__force __be16)ntohs((__force __be16)pseudo_csum);
	pseudo_csum = csum16_add(ip_payload_csum, addend);

	addend = (__force __be16)ntohs((__force __be16)*csum_field);
	csum_temp = ~csum16_sub(pseudo_csum, addend);
	csum_value_final = (__force u16)csum_temp;

	if (unlikely(csum_value_final == 0)) {
		switch (ip4h->protocol) {
		case IPPROTO_UDP:
			/* RFC 768 - DL4 1's complement rule for UDP csum 0 */
			csum_value_final = ~csum_value_final;
			break;

		case IPPROTO_TCP:
			/* DL4 Non-RFC compliant TCP checksum found */
			if (*csum_field == (__force __sum16)0xFFFF)
				csum_value_final = ~csum_value_final;
			break;
		}
	}

	if (csum_value_final == ntohs((__force __be16)*csum_field)) {
		priv->stats.csum_ok++;
		return 0;
	} else {
		priv->stats.csum_validation_failed++;
		return -EINVAL;
	}
=======
	/* The checksum value in the trailer is computed over the entire
	 * IP packet, including the IP header and payload.  To derive the
	 * transport checksum from this, we first subract the contribution
	 * of the IP header from the trailer checksum.  We then add the
	 * checksum computed over the pseudo header.
	 *
	 * We verified above that the IP header contributes zero to the
	 * trailer checksum.  Therefore the checksum in the trailer is
	 * just the checksum computed over the IP payload.

	 * If the IP payload arrives intact, adding the pseudo header
	 * checksum to the IP payload checksum will yield 0xffff (negative
	 * zero).  This means the trailer checksum and the pseudo checksum
	 * are additive inverses of each other.  Put another way, the
	 * message passes the checksum test if the trailer checksum value
	 * is the negated pseudo header checksum.
	 *
	 * Knowing this, we don't even need to examine the transport
	 * header checksum value; it is already accounted for in the
	 * checksum value found in the trailer.
	 */
	ip_payload_csum = csum_trailer->csum_value;

	pseudo_csum = csum_tcpudp_magic(ip4h->saddr, ip4h->daddr,
					ntohs(ip4h->tot_len) - ip4h->ihl * 4,
					ip4h->protocol, 0);

	/* The cast is required to ensure only the low 16 bits are examined */
	if (ip_payload_csum != (__sum16)~pseudo_csum) {
		priv->stats.csum_validation_failed++;
		return -EINVAL;
	}

	priv->stats.csum_ok++;
	return 0;
>>>>>>> upstream/android-13
}

#if IS_ENABLED(CONFIG_IPV6)
static int
rmnet_map_ipv6_dl_csum_trailer(struct sk_buff *skb,
			       struct rmnet_map_dl_csum_trailer *csum_trailer,
			       struct rmnet_priv *priv)
{
<<<<<<< HEAD
	__sum16 *csum_field, ip6_payload_csum, pseudo_csum, csum_temp;
	u16 csum_value, csum_value_final;
	__be16 ip6_hdr_csum, addend;
	struct ipv6hdr *ip6h;
	void *txporthdr;
	u32 length;

	ip6h = (struct ipv6hdr *)(skb->data);

	txporthdr = skb->data + sizeof(struct ipv6hdr);
	csum_field = rmnet_map_get_csum_field(ip6h->nexthdr, txporthdr);

=======
	struct ipv6hdr *ip6h = (struct ipv6hdr *)skb->data;
	void *txporthdr = skb->data + sizeof(*ip6h);
	__sum16 *csum_field, pseudo_csum;
	__sum16 ip6_payload_csum;
	__be16 ip_header_csum;

	/* Checksum offload is only supported for UDP and TCP protocols;
	 * the packet cannot include any IPv6 extension headers
	 */
	csum_field = rmnet_map_get_csum_field(ip6h->nexthdr, txporthdr);
>>>>>>> upstream/android-13
	if (!csum_field) {
		priv->stats.csum_err_invalid_transport++;
		return -EPROTONOSUPPORT;
	}

<<<<<<< HEAD
	csum_value = ~ntohs(csum_trailer->csum_value);
	ip6_hdr_csum = (__force __be16)
			~ntohs((__force __be16)ip_compute_csum(ip6h,
			       (int)(txporthdr - (void *)(skb->data))));
	ip6_payload_csum = csum16_sub((__force __sum16)csum_value,
				      ip6_hdr_csum);

	length = (ip6h->nexthdr == IPPROTO_UDP) ?
		 ntohs(((struct udphdr *)txporthdr)->len) :
		 ntohs(ip6h->payload_len);
	pseudo_csum = ~(csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
			     length, ip6h->nexthdr, 0));
	addend = (__force __be16)ntohs((__force __be16)pseudo_csum);
	pseudo_csum = csum16_add(ip6_payload_csum, addend);

	addend = (__force __be16)ntohs((__force __be16)*csum_field);
	csum_temp = ~csum16_sub(pseudo_csum, addend);
	csum_value_final = (__force u16)csum_temp;

	if (unlikely(csum_value_final == 0)) {
		switch (ip6h->nexthdr) {
		case IPPROTO_UDP:
			/* RFC 2460 section 8.1
			 * DL6 One's complement rule for UDP checksum 0
			 */
			csum_value_final = ~csum_value_final;
			break;

		case IPPROTO_TCP:
			/* DL6 Non-RFC compliant TCP checksum found */
			if (*csum_field == (__force __sum16)0xFFFF)
				csum_value_final = ~csum_value_final;
			break;
		}
	}

	if (csum_value_final == ntohs((__force __be16)*csum_field)) {
		priv->stats.csum_ok++;
		return 0;
	} else {
		priv->stats.csum_validation_failed++;
		return -EINVAL;
	}
}
#endif

static void rmnet_map_complement_ipv4_txporthdr_csum_field(void *iphdr)
{
	struct iphdr *ip4h = (struct iphdr *)iphdr;
	void *txphdr;
	u16 *csum;

	txphdr = iphdr + ip4h->ihl * 4;
=======
	/* The checksum value in the trailer is computed over the entire
	 * IP packet, including the IP header and payload.  To derive the
	 * transport checksum from this, we first subract the contribution
	 * of the IP header from the trailer checksum.  We then add the
	 * checksum computed over the pseudo header.
	 */
	ip_header_csum = (__force __be16)ip_fast_csum(ip6h, sizeof(*ip6h) / 4);
	ip6_payload_csum = csum16_sub(csum_trailer->csum_value, ip_header_csum);

	pseudo_csum = csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
				      ntohs(ip6h->payload_len),
				      ip6h->nexthdr, 0);

	/* It's sufficient to compare the IP payload checksum with the
	 * negated pseudo checksum to determine whether the packet
	 * checksum was good.  (See further explanation in comments
	 * in rmnet_map_ipv4_dl_csum_trailer()).
	 *
	 * The cast is required to ensure only the low 16 bits are
	 * examined.
	 */
	if (ip6_payload_csum != (__sum16)~pseudo_csum) {
		priv->stats.csum_validation_failed++;
		return -EINVAL;
	}

	priv->stats.csum_ok++;
	return 0;
}
#else
static int
rmnet_map_ipv6_dl_csum_trailer(struct sk_buff *skb,
			       struct rmnet_map_dl_csum_trailer *csum_trailer,
			       struct rmnet_priv *priv)
{
	return 0;
}
#endif

static void rmnet_map_complement_ipv4_txporthdr_csum_field(struct iphdr *ip4h)
{
	void *txphdr;
	u16 *csum;

	txphdr = (void *)ip4h + ip4h->ihl * 4;
>>>>>>> upstream/android-13

	if (ip4h->protocol == IPPROTO_TCP || ip4h->protocol == IPPROTO_UDP) {
		csum = (u16 *)rmnet_map_get_csum_field(ip4h->protocol, txphdr);
		*csum = ~(*csum);
	}
}

static void
<<<<<<< HEAD
rmnet_map_ipv4_ul_csum_header(void *iphdr,
			      struct rmnet_map_ul_csum_header *ul_header,
			      struct sk_buff *skb)
{
	struct iphdr *ip4h = (struct iphdr *)iphdr;
	__be16 *hdr = (__be16 *)ul_header, offset;

	offset = htons((__force u16)(skb_transport_header(skb) -
				     (unsigned char *)iphdr));
	ul_header->csum_start_offset = offset;
	ul_header->csum_insert_offset = skb->csum_offset;
	ul_header->csum_enabled = 1;
	if (ip4h->protocol == IPPROTO_UDP)
		ul_header->udp_ind = 1;
	else
		ul_header->udp_ind = 0;

	/* Changing remaining fields to network order */
	hdr++;
	*hdr = htons((__force u16)*hdr);
=======
rmnet_map_ipv4_ul_csum_header(struct iphdr *iphdr,
			      struct rmnet_map_ul_csum_header *ul_header,
			      struct sk_buff *skb)
{
	u16 val;

	val = MAP_CSUM_UL_ENABLED_FLAG;
	if (iphdr->protocol == IPPROTO_UDP)
		val |= MAP_CSUM_UL_UDP_FLAG;
	val |= skb->csum_offset & MAP_CSUM_UL_OFFSET_MASK;

	ul_header->csum_start_offset = htons(skb_network_header_len(skb));
	ul_header->csum_info = htons(val);
>>>>>>> upstream/android-13

	skb->ip_summed = CHECKSUM_NONE;

	rmnet_map_complement_ipv4_txporthdr_csum_field(iphdr);
}

#if IS_ENABLED(CONFIG_IPV6)
<<<<<<< HEAD
static void rmnet_map_complement_ipv6_txporthdr_csum_field(void *ip6hdr)
{
	struct ipv6hdr *ip6h = (struct ipv6hdr *)ip6hdr;
	void *txphdr;
	u16 *csum;

	txphdr = ip6hdr + sizeof(struct ipv6hdr);
=======
static void
rmnet_map_complement_ipv6_txporthdr_csum_field(struct ipv6hdr *ip6h)
{
	void *txphdr;
	u16 *csum;

	txphdr = ip6h + 1;
>>>>>>> upstream/android-13

	if (ip6h->nexthdr == IPPROTO_TCP || ip6h->nexthdr == IPPROTO_UDP) {
		csum = (u16 *)rmnet_map_get_csum_field(ip6h->nexthdr, txphdr);
		*csum = ~(*csum);
	}
}

static void
<<<<<<< HEAD
=======
rmnet_map_ipv6_ul_csum_header(struct ipv6hdr *ipv6hdr,
			      struct rmnet_map_ul_csum_header *ul_header,
			      struct sk_buff *skb)
{
	u16 val;

	val = MAP_CSUM_UL_ENABLED_FLAG;
	if (ipv6hdr->nexthdr == IPPROTO_UDP)
		val |= MAP_CSUM_UL_UDP_FLAG;
	val |= skb->csum_offset & MAP_CSUM_UL_OFFSET_MASK;

	ul_header->csum_start_offset = htons(skb_network_header_len(skb));
	ul_header->csum_info = htons(val);

	skb->ip_summed = CHECKSUM_NONE;

	rmnet_map_complement_ipv6_txporthdr_csum_field(ipv6hdr);
}
#else
static void
>>>>>>> upstream/android-13
rmnet_map_ipv6_ul_csum_header(void *ip6hdr,
			      struct rmnet_map_ul_csum_header *ul_header,
			      struct sk_buff *skb)
{
<<<<<<< HEAD
	struct ipv6hdr *ip6h = (struct ipv6hdr *)ip6hdr;
	__be16 *hdr = (__be16 *)ul_header, offset;

	offset = htons((__force u16)(skb_transport_header(skb) -
				     (unsigned char *)ip6hdr));
	ul_header->csum_start_offset = offset;
	ul_header->csum_insert_offset = skb->csum_offset;
	ul_header->csum_enabled = 1;

	if (ip6h->nexthdr == IPPROTO_UDP)
		ul_header->udp_ind = 1;
	else
		ul_header->udp_ind = 0;

	/* Changing remaining fields to network order */
	hdr++;
	*hdr = htons((__force u16)*hdr);

	skb->ip_summed = CHECKSUM_NONE;

	rmnet_map_complement_ipv6_txporthdr_csum_field(ip6hdr);
}
#endif

=======
}
#endif

static void rmnet_map_v5_checksum_uplink_packet(struct sk_buff *skb,
						struct rmnet_port *port,
						struct net_device *orig_dev)
{
	struct rmnet_priv *priv = netdev_priv(orig_dev);
	struct rmnet_map_v5_csum_header *ul_header;

	ul_header = skb_push(skb, sizeof(*ul_header));
	memset(ul_header, 0, sizeof(*ul_header));
	ul_header->header_info = u8_encode_bits(RMNET_MAP_HEADER_TYPE_CSUM_OFFLOAD,
						MAPV5_HDRINFO_HDR_TYPE_FMASK);

	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		void *iph = ip_hdr(skb);
		__sum16 *check;
		void *trans;
		u8 proto;

		if (skb->protocol == htons(ETH_P_IP)) {
			u16 ip_len = ((struct iphdr *)iph)->ihl * 4;

			proto = ((struct iphdr *)iph)->protocol;
			trans = iph + ip_len;
		} else if (IS_ENABLED(CONFIG_IPV6) &&
			   skb->protocol == htons(ETH_P_IPV6)) {
			u16 ip_len = sizeof(struct ipv6hdr);

			proto = ((struct ipv6hdr *)iph)->nexthdr;
			trans = iph + ip_len;
		} else {
			priv->stats.csum_err_invalid_ip_version++;
			goto sw_csum;
		}

		check = rmnet_map_get_csum_field(proto, trans);
		if (check) {
			skb->ip_summed = CHECKSUM_NONE;
			/* Ask for checksum offloading */
			ul_header->csum_info |= MAPV5_CSUMINFO_VALID_FLAG;
			priv->stats.csum_hw++;
			return;
		}
	}

sw_csum:
	priv->stats.csum_sw++;
}

>>>>>>> upstream/android-13
/* Adds MAP header to front of skb->data
 * Padding is calculated and set appropriately in MAP header. Mux ID is
 * initialized to 0.
 */
struct rmnet_map_header *rmnet_map_add_map_header(struct sk_buff *skb,
<<<<<<< HEAD
						  int hdrlen, int pad)
=======
						  int hdrlen,
						  struct rmnet_port *port,
						  int pad)
>>>>>>> upstream/android-13
{
	struct rmnet_map_header *map_header;
	u32 padding, map_datalen;
	u8 *padbytes;

	map_datalen = skb->len - hdrlen;
	map_header = (struct rmnet_map_header *)
			skb_push(skb, sizeof(struct rmnet_map_header));
	memset(map_header, 0, sizeof(struct rmnet_map_header));

<<<<<<< HEAD
=======
	/* Set next_hdr bit for csum offload packets */
	if (port->data_format & RMNET_FLAGS_EGRESS_MAP_CKSUMV5)
		map_header->flags |= MAP_NEXT_HEADER_FLAG;

>>>>>>> upstream/android-13
	if (pad == RMNET_MAP_NO_PAD_BYTES) {
		map_header->pkt_len = htons(map_datalen);
		return map_header;
	}

<<<<<<< HEAD
=======
	BUILD_BUG_ON(MAP_PAD_LEN_MASK < 3);
>>>>>>> upstream/android-13
	padding = ALIGN(map_datalen, 4) - map_datalen;

	if (padding == 0)
		goto done;

	if (skb_tailroom(skb) < padding)
		return NULL;

	padbytes = (u8 *)skb_put(skb, padding);
	memset(padbytes, 0, padding);

done:
	map_header->pkt_len = htons(map_datalen + padding);
<<<<<<< HEAD
	map_header->pad_len = padding & 0x3F;
=======
	/* This is a data packet, so the CMD bit is 0 */
	map_header->flags = padding & MAP_PAD_LEN_MASK;
>>>>>>> upstream/android-13

	return map_header;
}

/* Deaggregates a single packet
 * A whole new buffer is allocated for each portion of an aggregated frame.
 * Caller should keep calling deaggregate() on the source skb until 0 is
 * returned, indicating that there are no more packets to deaggregate. Caller
 * is responsible for freeing the original skb.
 */
struct sk_buff *rmnet_map_deaggregate(struct sk_buff *skb,
				      struct rmnet_port *port)
{
<<<<<<< HEAD
	struct rmnet_map_header *maph;
	struct sk_buff *skbn;
=======
	struct rmnet_map_v5_csum_header *next_hdr = NULL;
	struct rmnet_map_header *maph;
	void *data = skb->data;
	struct sk_buff *skbn;
	u8 nexthdr_type;
>>>>>>> upstream/android-13
	u32 packet_len;

	if (skb->len == 0)
		return NULL;

	maph = (struct rmnet_map_header *)skb->data;
<<<<<<< HEAD
	packet_len = ntohs(maph->pkt_len) + sizeof(struct rmnet_map_header);

	if (port->data_format & RMNET_FLAGS_INGRESS_MAP_CKSUMV4)
		packet_len += sizeof(struct rmnet_map_dl_csum_trailer);
=======
	packet_len = ntohs(maph->pkt_len) + sizeof(*maph);

	if (port->data_format & RMNET_FLAGS_INGRESS_MAP_CKSUMV4) {
		packet_len += sizeof(struct rmnet_map_dl_csum_trailer);
	} else if (port->data_format & RMNET_FLAGS_INGRESS_MAP_CKSUMV5) {
		if (!(maph->flags & MAP_CMD_FLAG)) {
			packet_len += sizeof(*next_hdr);
			if (maph->flags & MAP_NEXT_HEADER_FLAG)
				next_hdr = data + sizeof(*maph);
			else
				/* Mapv5 data pkt without csum hdr is invalid */
				return NULL;
		}
	}
>>>>>>> upstream/android-13

	if (((int)skb->len - (int)packet_len) < 0)
		return NULL;

	/* Some hardware can send us empty frames. Catch them */
<<<<<<< HEAD
	if (ntohs(maph->pkt_len) == 0)
		return NULL;

=======
	if (!maph->pkt_len)
		return NULL;

	if (next_hdr) {
		nexthdr_type = u8_get_bits(next_hdr->header_info,
					   MAPV5_HDRINFO_HDR_TYPE_FMASK);
		if (nexthdr_type != RMNET_MAP_HEADER_TYPE_CSUM_OFFLOAD)
			return NULL;
	}

>>>>>>> upstream/android-13
	skbn = alloc_skb(packet_len + RMNET_MAP_DEAGGR_SPACING, GFP_ATOMIC);
	if (!skbn)
		return NULL;

	skb_reserve(skbn, RMNET_MAP_DEAGGR_HEADROOM);
	skb_put(skbn, packet_len);
	memcpy(skbn->data, skb->data, packet_len);
	skb_pull(skb, packet_len);

	return skbn;
}

/* Validates packet checksums. Function takes a pointer to
 * the beginning of a buffer which contains the IP payload +
 * padding + checksum trailer.
 * Only IPv4 and IPv6 are supported along with TCP & UDP.
 * Fragmented or tunneled packets are not supported.
 */
int rmnet_map_checksum_downlink_packet(struct sk_buff *skb, u16 len)
{
	struct rmnet_priv *priv = netdev_priv(skb->dev);
	struct rmnet_map_dl_csum_trailer *csum_trailer;

	if (unlikely(!(skb->dev->features & NETIF_F_RXCSUM))) {
		priv->stats.csum_sw++;
		return -EOPNOTSUPP;
	}

	csum_trailer = (struct rmnet_map_dl_csum_trailer *)(skb->data + len);

<<<<<<< HEAD
	if (!csum_trailer->valid) {
=======
	if (!(csum_trailer->flags & MAP_CSUM_DL_VALID_FLAG)) {
>>>>>>> upstream/android-13
		priv->stats.csum_valid_unset++;
		return -EINVAL;
	}

<<<<<<< HEAD
	if (skb->protocol == htons(ETH_P_IP)) {
		return rmnet_map_ipv4_dl_csum_trailer(skb, csum_trailer, priv);
	} else if (skb->protocol == htons(ETH_P_IPV6)) {
#if IS_ENABLED(CONFIG_IPV6)
		return rmnet_map_ipv6_dl_csum_trailer(skb, csum_trailer, priv);
#else
		priv->stats.csum_err_invalid_ip_version++;
		return -EPROTONOSUPPORT;
#endif
	} else {
		priv->stats.csum_err_invalid_ip_version++;
		return -EPROTONOSUPPORT;
	}

	return 0;
}

/* Generates UL checksum meta info header for IPv4 and IPv6 over TCP and UDP
 * packets that are supported for UL checksum offload.
 */
void rmnet_map_checksum_uplink_packet(struct sk_buff *skb,
				      struct net_device *orig_dev)
=======
	if (skb->protocol == htons(ETH_P_IP))
		return rmnet_map_ipv4_dl_csum_trailer(skb, csum_trailer, priv);

	if (IS_ENABLED(CONFIG_IPV6) && skb->protocol == htons(ETH_P_IPV6))
		return rmnet_map_ipv6_dl_csum_trailer(skb, csum_trailer, priv);

	priv->stats.csum_err_invalid_ip_version++;

	return -EPROTONOSUPPORT;
}

static void rmnet_map_v4_checksum_uplink_packet(struct sk_buff *skb,
						struct net_device *orig_dev)
>>>>>>> upstream/android-13
{
	struct rmnet_priv *priv = netdev_priv(orig_dev);
	struct rmnet_map_ul_csum_header *ul_header;
	void *iphdr;

	ul_header = (struct rmnet_map_ul_csum_header *)
		    skb_push(skb, sizeof(struct rmnet_map_ul_csum_header));

	if (unlikely(!(orig_dev->features &
		     (NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM))))
		goto sw_csum;

<<<<<<< HEAD
	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		iphdr = (char *)ul_header +
			sizeof(struct rmnet_map_ul_csum_header);

		if (skb->protocol == htons(ETH_P_IP)) {
			rmnet_map_ipv4_ul_csum_header(iphdr, ul_header, skb);
			return;
		} else if (skb->protocol == htons(ETH_P_IPV6)) {
#if IS_ENABLED(CONFIG_IPV6)
			rmnet_map_ipv6_ul_csum_header(iphdr, ul_header, skb);
			return;
#else
			priv->stats.csum_err_invalid_ip_version++;
			goto sw_csum;
#endif
		} else {
			priv->stats.csum_err_invalid_ip_version++;
		}
	}

sw_csum:
	ul_header->csum_start_offset = 0;
	ul_header->csum_insert_offset = 0;
	ul_header->csum_enabled = 0;
	ul_header->udp_ind = 0;

	priv->stats.csum_sw++;
}
=======
	if (skb->ip_summed != CHECKSUM_PARTIAL)
		goto sw_csum;

	iphdr = (char *)ul_header +
		sizeof(struct rmnet_map_ul_csum_header);

	if (skb->protocol == htons(ETH_P_IP)) {
		rmnet_map_ipv4_ul_csum_header(iphdr, ul_header, skb);
		priv->stats.csum_hw++;
		return;
	}

	if (IS_ENABLED(CONFIG_IPV6) && skb->protocol == htons(ETH_P_IPV6)) {
		rmnet_map_ipv6_ul_csum_header(iphdr, ul_header, skb);
		priv->stats.csum_hw++;
		return;
	}

	priv->stats.csum_err_invalid_ip_version++;

sw_csum:
	memset(ul_header, 0, sizeof(*ul_header));

	priv->stats.csum_sw++;
}

/* Generates UL checksum meta info header for IPv4 and IPv6 over TCP and UDP
 * packets that are supported for UL checksum offload.
 */
void rmnet_map_checksum_uplink_packet(struct sk_buff *skb,
				      struct rmnet_port *port,
				      struct net_device *orig_dev,
				      int csum_type)
{
	switch (csum_type) {
	case RMNET_FLAGS_EGRESS_MAP_CKSUMV4:
		rmnet_map_v4_checksum_uplink_packet(skb, orig_dev);
		break;
	case RMNET_FLAGS_EGRESS_MAP_CKSUMV5:
		rmnet_map_v5_checksum_uplink_packet(skb, port, orig_dev);
		break;
	default:
		break;
	}
}

/* Process a MAPv5 packet header */
int rmnet_map_process_next_hdr_packet(struct sk_buff *skb,
				      u16 len)
{
	struct rmnet_priv *priv = netdev_priv(skb->dev);
	struct rmnet_map_v5_csum_header *next_hdr;
	u8 nexthdr_type;

	next_hdr = (struct rmnet_map_v5_csum_header *)(skb->data +
			sizeof(struct rmnet_map_header));

	nexthdr_type = u8_get_bits(next_hdr->header_info,
				   MAPV5_HDRINFO_HDR_TYPE_FMASK);

	if (nexthdr_type != RMNET_MAP_HEADER_TYPE_CSUM_OFFLOAD)
		return -EINVAL;

	if (unlikely(!(skb->dev->features & NETIF_F_RXCSUM))) {
		priv->stats.csum_sw++;
	} else if (next_hdr->csum_info & MAPV5_CSUMINFO_VALID_FLAG) {
		priv->stats.csum_ok++;
		skb->ip_summed = CHECKSUM_UNNECESSARY;
	} else {
		priv->stats.csum_valid_unset++;
	}

	/* Pull csum v5 header */
	skb_pull(skb, sizeof(*next_hdr));

	return 0;
}
>>>>>>> upstream/android-13
