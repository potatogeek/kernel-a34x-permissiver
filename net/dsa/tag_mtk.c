<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Mediatek DSA Tag support
 * Copyright (C) 2017 Landen Chao <landen.chao@mediatek.com>
 *		      Sean Wang <sean.wang@mediatek.com>
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/etherdevice.h>
#include <linux/if_vlan.h>

#include "dsa_priv.h"

#define MTK_HDR_LEN		4
#define MTK_HDR_XMIT_UNTAGGED		0
#define MTK_HDR_XMIT_TAGGED_TPID_8100	1
#define MTK_HDR_XMIT_TAGGED_TPID_88A8	2
#define MTK_HDR_RECV_SOURCE_PORT_MASK	GENMASK(2, 0)
#define MTK_HDR_XMIT_DP_BIT_MASK	GENMASK(5, 0)
#define MTK_HDR_XMIT_SA_DIS		BIT(6)

static struct sk_buff *mtk_tag_xmit(struct sk_buff *skb,
				    struct net_device *dev)
{
	struct dsa_port *dp = dsa_slave_to_port(dev);
	u8 xmit_tpid;
	u8 *mtk_tag;
<<<<<<< HEAD
	unsigned char *dest = eth_hdr(skb)->h_dest;
	bool is_multicast_skb = is_multicast_ether_addr(dest) &&
				!is_broadcast_ether_addr(dest);
=======
>>>>>>> upstream/android-13

	/* Build the special tag after the MAC Source Address. If VLAN header
	 * is present, it's required that VLAN header and special tag is
	 * being combined. Only in this way we can allow the switch can parse
	 * the both special and VLAN tag at the same time and then look up VLAN
	 * table with VID.
	 */
	switch (skb->protocol) {
	case htons(ETH_P_8021Q):
		xmit_tpid = MTK_HDR_XMIT_TAGGED_TPID_8100;
		break;
	case htons(ETH_P_8021AD):
		xmit_tpid = MTK_HDR_XMIT_TAGGED_TPID_88A8;
		break;
	default:
<<<<<<< HEAD
		if (skb_cow_head(skb, MTK_HDR_LEN) < 0)
			return NULL;

		xmit_tpid = MTK_HDR_XMIT_UNTAGGED;
		skb_push(skb, MTK_HDR_LEN);
		memmove(skb->data, skb->data + MTK_HDR_LEN, 2 * ETH_ALEN);
	}

	mtk_tag = skb->data + 2 * ETH_ALEN;
=======
		xmit_tpid = MTK_HDR_XMIT_UNTAGGED;
		skb_push(skb, MTK_HDR_LEN);
		dsa_alloc_etype_header(skb, MTK_HDR_LEN);
	}

	mtk_tag = dsa_etype_header_pos_tx(skb);
>>>>>>> upstream/android-13

	/* Mark tag attribute on special tag insertion to notify hardware
	 * whether that's a combined special tag with 802.1Q header.
	 */
	mtk_tag[0] = xmit_tpid;
	mtk_tag[1] = (1 << dp->index) & MTK_HDR_XMIT_DP_BIT_MASK;

<<<<<<< HEAD
	/* Disable SA learning for multicast frames */
	if (unlikely(is_multicast_skb))
		mtk_tag[1] |= MTK_HDR_XMIT_SA_DIS;

=======
>>>>>>> upstream/android-13
	/* Tag control information is kept for 802.1Q */
	if (xmit_tpid == MTK_HDR_XMIT_UNTAGGED) {
		mtk_tag[2] = 0;
		mtk_tag[3] = 0;
	}

	return skb;
}

<<<<<<< HEAD
static struct sk_buff *mtk_tag_rcv(struct sk_buff *skb, struct net_device *dev,
				   struct packet_type *pt)
{
	int port;
	__be16 *phdr, hdr;
	unsigned char *dest = eth_hdr(skb)->h_dest;
	bool is_multicast_skb = is_multicast_ether_addr(dest) &&
				!is_broadcast_ether_addr(dest);
=======
static struct sk_buff *mtk_tag_rcv(struct sk_buff *skb, struct net_device *dev)
{
	u16 hdr;
	int port;
	__be16 *phdr;
>>>>>>> upstream/android-13

	if (unlikely(!pskb_may_pull(skb, MTK_HDR_LEN)))
		return NULL;

<<<<<<< HEAD
	/* The MTK header is added by the switch between src addr
	 * and ethertype at this point, skb->data points to 2 bytes
	 * after src addr so header should be 2 bytes right before.
	 */
	phdr = (__be16 *)(skb->data - 2);
=======
	phdr = dsa_etype_header_pos_rx(skb);
>>>>>>> upstream/android-13
	hdr = ntohs(*phdr);

	/* Remove MTK tag and recalculate checksum. */
	skb_pull_rcsum(skb, MTK_HDR_LEN);

<<<<<<< HEAD
	memmove(skb->data - ETH_HLEN,
		skb->data - ETH_HLEN - MTK_HDR_LEN,
		2 * ETH_ALEN);
=======
	dsa_strip_etype_header(skb, MTK_HDR_LEN);
>>>>>>> upstream/android-13

	/* Get source port information */
	port = (hdr & MTK_HDR_RECV_SOURCE_PORT_MASK);

	skb->dev = dsa_master_find_slave(dev, 0, port);
	if (!skb->dev)
		return NULL;

<<<<<<< HEAD
	/* Only unicast or broadcast frames are offloaded */
	if (likely(!is_multicast_skb))
		skb->offload_fwd_mark = 1;
=======
	dsa_default_offload_fwd_mark(skb);
>>>>>>> upstream/android-13

	return skb;
}

<<<<<<< HEAD
static int mtk_tag_flow_dissect(const struct sk_buff *skb, __be16 *proto,
				int *offset)
{
	*offset = 4;
	*proto = ((__be16 *)skb->data)[1];

	return 0;
}

const struct dsa_device_ops mtk_netdev_ops = {
	.xmit		= mtk_tag_xmit,
	.rcv		= mtk_tag_rcv,
	.flow_dissect	= mtk_tag_flow_dissect,
};
=======
static const struct dsa_device_ops mtk_netdev_ops = {
	.name		= "mtk",
	.proto		= DSA_TAG_PROTO_MTK,
	.xmit		= mtk_tag_xmit,
	.rcv		= mtk_tag_rcv,
	.needed_headroom = MTK_HDR_LEN,
};

MODULE_LICENSE("GPL");
MODULE_ALIAS_DSA_TAG_DRIVER(DSA_TAG_PROTO_MTK);

module_dsa_tag_driver(mtk_netdev_ops);
>>>>>>> upstream/android-13
