<<<<<<< HEAD
/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/etherdevice.h>

#include "dsa_priv.h"

#define QCA_HDR_LEN	2
#define QCA_HDR_VERSION	0x2

#define QCA_HDR_RECV_VERSION_MASK	GENMASK(15, 14)
#define QCA_HDR_RECV_VERSION_S		14
#define QCA_HDR_RECV_PRIORITY_MASK	GENMASK(13, 11)
#define QCA_HDR_RECV_PRIORITY_S		11
#define QCA_HDR_RECV_TYPE_MASK		GENMASK(10, 6)
#define QCA_HDR_RECV_TYPE_S		6
#define QCA_HDR_RECV_FRAME_IS_TAGGED	BIT(3)
#define QCA_HDR_RECV_SOURCE_PORT_MASK	GENMASK(2, 0)

#define QCA_HDR_XMIT_VERSION_MASK	GENMASK(15, 14)
#define QCA_HDR_XMIT_VERSION_S		14
#define QCA_HDR_XMIT_PRIORITY_MASK	GENMASK(13, 11)
#define QCA_HDR_XMIT_PRIORITY_S		11
#define QCA_HDR_XMIT_CONTROL_MASK	GENMASK(10, 8)
#define QCA_HDR_XMIT_CONTROL_S		8
#define QCA_HDR_XMIT_FROM_CPU		BIT(7)
#define QCA_HDR_XMIT_DP_BIT_MASK	GENMASK(6, 0)

static struct sk_buff *qca_tag_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct dsa_port *dp = dsa_slave_to_port(dev);
<<<<<<< HEAD
	u16 *phdr, hdr;

	if (skb_cow_head(skb, QCA_HDR_LEN) < 0)
		return NULL;

	skb_push(skb, QCA_HDR_LEN);

	memmove(skb->data, skb->data + QCA_HDR_LEN, 2 * ETH_ALEN);
	phdr = (u16 *)(skb->data + 2 * ETH_ALEN);
=======
	__be16 *phdr;
	u16 hdr;

	skb_push(skb, QCA_HDR_LEN);

	dsa_alloc_etype_header(skb, QCA_HDR_LEN);
	phdr = dsa_etype_header_pos_tx(skb);
>>>>>>> upstream/android-13

	/* Set the version field, and set destination port information */
	hdr = QCA_HDR_VERSION << QCA_HDR_XMIT_VERSION_S |
		QCA_HDR_XMIT_FROM_CPU | BIT(dp->index);

	*phdr = htons(hdr);

	return skb;
}

<<<<<<< HEAD
static struct sk_buff *qca_tag_rcv(struct sk_buff *skb, struct net_device *dev,
				   struct packet_type *pt)
{
	u8 ver;
	int port;
	__be16 *phdr, hdr;
=======
static struct sk_buff *qca_tag_rcv(struct sk_buff *skb, struct net_device *dev)
{
	u8 ver;
	u16  hdr;
	int port;
	__be16 *phdr;
>>>>>>> upstream/android-13

	if (unlikely(!pskb_may_pull(skb, QCA_HDR_LEN)))
		return NULL;

<<<<<<< HEAD
	/* The QCA header is added by the switch between src addr and Ethertype
	 * At this point, skb->data points to ethertype so header should be
	 * right before
	 */
	phdr = (__be16 *)(skb->data - 2);
=======
	phdr = dsa_etype_header_pos_rx(skb);
>>>>>>> upstream/android-13
	hdr = ntohs(*phdr);

	/* Make sure the version is correct */
	ver = (hdr & QCA_HDR_RECV_VERSION_MASK) >> QCA_HDR_RECV_VERSION_S;
	if (unlikely(ver != QCA_HDR_VERSION))
		return NULL;

	/* Remove QCA tag and recalculate checksum */
	skb_pull_rcsum(skb, QCA_HDR_LEN);
<<<<<<< HEAD
	memmove(skb->data - ETH_HLEN, skb->data - ETH_HLEN - QCA_HDR_LEN,
		ETH_HLEN - QCA_HDR_LEN);
=======
	dsa_strip_etype_header(skb, QCA_HDR_LEN);
>>>>>>> upstream/android-13

	/* Get source port information */
	port = (hdr & QCA_HDR_RECV_SOURCE_PORT_MASK);

	skb->dev = dsa_master_find_slave(dev, 0, port);
	if (!skb->dev)
		return NULL;

	return skb;
}

<<<<<<< HEAD
const struct dsa_device_ops qca_netdev_ops = {
	.xmit	= qca_tag_xmit,
	.rcv	= qca_tag_rcv,
};
=======
static const struct dsa_device_ops qca_netdev_ops = {
	.name	= "qca",
	.proto	= DSA_TAG_PROTO_QCA,
	.xmit	= qca_tag_xmit,
	.rcv	= qca_tag_rcv,
	.needed_headroom = QCA_HDR_LEN,
};

MODULE_LICENSE("GPL");
MODULE_ALIAS_DSA_TAG_DRIVER(DSA_TAG_PROTO_QCA);

module_dsa_tag_driver(qca_netdev_ops);
>>>>>>> upstream/android-13
