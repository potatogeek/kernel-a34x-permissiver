<<<<<<< HEAD
/*
 * net/dsa/tag_ksz.c - Microchip KSZ Switch tag format handling
 * Copyright (c) 2017 Microchip Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * net/dsa/tag_ksz.c - Microchip KSZ Switch tag format handling
 * Copyright (c) 2017 Microchip Technology
>>>>>>> upstream/android-13
 */

#include <linux/etherdevice.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <net/dsa.h>
#include "dsa_priv.h"

<<<<<<< HEAD
/* For Ingress (Host -> KSZ), 2 bytes are added before FCS.
 * ---------------------------------------------------------------------------
 * DA(6bytes)|SA(6bytes)|....|Data(nbytes)|tag0(1byte)|tag1(1byte)|FCS(4bytes)
 * ---------------------------------------------------------------------------
 * tag0 : Prioritization (not used now)
 * tag1 : each bit represents port (eg, 0x01=port1, 0x02=port2, 0x10=port5)
 *
 * For Egress (KSZ -> Host), 1 byte is added before FCS.
=======
/* Typically only one byte is used for tail tag. */
#define KSZ_EGRESS_TAG_LEN		1
#define KSZ_INGRESS_TAG_LEN		1

static struct sk_buff *ksz_common_rcv(struct sk_buff *skb,
				      struct net_device *dev,
				      unsigned int port, unsigned int len)
{
	skb->dev = dsa_master_find_slave(dev, 0, port);
	if (!skb->dev)
		return NULL;

	pskb_trim_rcsum(skb, skb->len - len);

	dsa_default_offload_fwd_mark(skb);

	return skb;
}

/*
 * For Ingress (Host -> KSZ8795), 1 byte is added before FCS.
 * ---------------------------------------------------------------------------
 * DA(6bytes)|SA(6bytes)|....|Data(nbytes)|tag(1byte)|FCS(4bytes)
 * ---------------------------------------------------------------------------
 * tag : each bit represents port (eg, 0x01=port1, 0x02=port2, 0x10=port5)
 *
 * For Egress (KSZ8795 -> Host), 1 byte is added before FCS.
>>>>>>> upstream/android-13
 * ---------------------------------------------------------------------------
 * DA(6bytes)|SA(6bytes)|....|Data(nbytes)|tag0(1byte)|FCS(4bytes)
 * ---------------------------------------------------------------------------
 * tag0 : zero-based value represents port
 *	  (eg, 0x00=port1, 0x02=port3, 0x06=port7)
 */

<<<<<<< HEAD
#define	KSZ_INGRESS_TAG_LEN	2
#define	KSZ_EGRESS_TAG_LEN	1

static struct sk_buff *ksz_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct dsa_port *dp = dsa_slave_to_port(dev);
	struct sk_buff *nskb;
	int padlen;
	u8 *tag;

	padlen = (skb->len >= ETH_ZLEN) ? 0 : ETH_ZLEN - skb->len;

	if (skb_tailroom(skb) >= padlen + KSZ_INGRESS_TAG_LEN) {
		/* Let dsa_slave_xmit() free skb */
		if (__skb_put_padto(skb, skb->len + padlen, false))
			return NULL;

		nskb = skb;
	} else {
		nskb = alloc_skb(NET_IP_ALIGN + skb->len +
				 padlen + KSZ_INGRESS_TAG_LEN, GFP_ATOMIC);
		if (!nskb)
			return NULL;
		skb_reserve(nskb, NET_IP_ALIGN);

		skb_reset_mac_header(nskb);
		skb_set_network_header(nskb,
				       skb_network_header(skb) - skb->head);
		skb_set_transport_header(nskb,
					 skb_transport_header(skb) - skb->head);
		skb_copy_and_csum_dev(skb, skb_put(nskb, skb->len));

		/* Let skb_put_padto() free nskb, and let dsa_slave_xmit() free
		 * skb
		 */
		if (skb_put_padto(nskb, nskb->len + padlen))
			return NULL;

		consume_skb(skb);
	}

	tag = skb_put(nskb, KSZ_INGRESS_TAG_LEN);
	tag[0] = 0;
	tag[1] = 1 << dp->index; /* destination port */

	return nskb;
}

static struct sk_buff *ksz_rcv(struct sk_buff *skb, struct net_device *dev,
			       struct packet_type *pt)
{
	u8 *tag;
	int source_port;

	tag = skb_tail_pointer(skb) - KSZ_EGRESS_TAG_LEN;

	source_port = tag[0] & 7;

	skb->dev = dsa_master_find_slave(dev, 0, source_port);
	if (!skb->dev)
		return NULL;

	pskb_trim_rcsum(skb, skb->len - KSZ_EGRESS_TAG_LEN);
=======
#define KSZ8795_TAIL_TAG_OVERRIDE	BIT(6)
#define KSZ8795_TAIL_TAG_LOOKUP		BIT(7)

static struct sk_buff *ksz8795_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct dsa_port *dp = dsa_slave_to_port(dev);
	u8 *tag;
	u8 *addr;

	if (skb->ip_summed == CHECKSUM_PARTIAL && skb_checksum_help(skb))
		return NULL;

	/* Tag encoding */
	tag = skb_put(skb, KSZ_INGRESS_TAG_LEN);
	addr = skb_mac_header(skb);

	*tag = 1 << dp->index;
	if (is_link_local_ether_addr(addr))
		*tag |= KSZ8795_TAIL_TAG_OVERRIDE;
>>>>>>> upstream/android-13

	return skb;
}

<<<<<<< HEAD
const struct dsa_device_ops ksz_netdev_ops = {
	.xmit	= ksz_xmit,
	.rcv	= ksz_rcv,
};
=======
static struct sk_buff *ksz8795_rcv(struct sk_buff *skb, struct net_device *dev)
{
	u8 *tag = skb_tail_pointer(skb) - KSZ_EGRESS_TAG_LEN;

	return ksz_common_rcv(skb, dev, tag[0] & 7, KSZ_EGRESS_TAG_LEN);
}

static const struct dsa_device_ops ksz8795_netdev_ops = {
	.name	= "ksz8795",
	.proto	= DSA_TAG_PROTO_KSZ8795,
	.xmit	= ksz8795_xmit,
	.rcv	= ksz8795_rcv,
	.needed_tailroom = KSZ_INGRESS_TAG_LEN,
};

DSA_TAG_DRIVER(ksz8795_netdev_ops);
MODULE_ALIAS_DSA_TAG_DRIVER(DSA_TAG_PROTO_KSZ8795);

/*
 * For Ingress (Host -> KSZ9477), 2 bytes are added before FCS.
 * ---------------------------------------------------------------------------
 * DA(6bytes)|SA(6bytes)|....|Data(nbytes)|tag0(1byte)|tag1(1byte)|FCS(4bytes)
 * ---------------------------------------------------------------------------
 * tag0 : Prioritization (not used now)
 * tag1 : each bit represents port (eg, 0x01=port1, 0x02=port2, 0x10=port5)
 *
 * For Egress (KSZ9477 -> Host), 1 byte is added before FCS.
 * ---------------------------------------------------------------------------
 * DA(6bytes)|SA(6bytes)|....|Data(nbytes)|tag0(1byte)|FCS(4bytes)
 * ---------------------------------------------------------------------------
 * tag0 : zero-based value represents port
 *	  (eg, 0x00=port1, 0x02=port3, 0x06=port7)
 */

#define KSZ9477_INGRESS_TAG_LEN		2
#define KSZ9477_PTP_TAG_LEN		4
#define KSZ9477_PTP_TAG_INDICATION	0x80

#define KSZ9477_TAIL_TAG_OVERRIDE	BIT(9)
#define KSZ9477_TAIL_TAG_LOOKUP		BIT(10)

static struct sk_buff *ksz9477_xmit(struct sk_buff *skb,
				    struct net_device *dev)
{
	struct dsa_port *dp = dsa_slave_to_port(dev);
	__be16 *tag;
	u8 *addr;
	u16 val;

	if (skb->ip_summed == CHECKSUM_PARTIAL && skb_checksum_help(skb))
		return NULL;

	/* Tag encoding */
	tag = skb_put(skb, KSZ9477_INGRESS_TAG_LEN);
	addr = skb_mac_header(skb);

	val = BIT(dp->index);

	if (is_link_local_ether_addr(addr))
		val |= KSZ9477_TAIL_TAG_OVERRIDE;

	*tag = cpu_to_be16(val);

	return skb;
}

static struct sk_buff *ksz9477_rcv(struct sk_buff *skb, struct net_device *dev)
{
	/* Tag decoding */
	u8 *tag = skb_tail_pointer(skb) - KSZ_EGRESS_TAG_LEN;
	unsigned int port = tag[0] & 7;
	unsigned int len = KSZ_EGRESS_TAG_LEN;

	/* Extra 4-bytes PTP timestamp */
	if (tag[0] & KSZ9477_PTP_TAG_INDICATION)
		len += KSZ9477_PTP_TAG_LEN;

	return ksz_common_rcv(skb, dev, port, len);
}

static const struct dsa_device_ops ksz9477_netdev_ops = {
	.name	= "ksz9477",
	.proto	= DSA_TAG_PROTO_KSZ9477,
	.xmit	= ksz9477_xmit,
	.rcv	= ksz9477_rcv,
	.needed_tailroom = KSZ9477_INGRESS_TAG_LEN,
};

DSA_TAG_DRIVER(ksz9477_netdev_ops);
MODULE_ALIAS_DSA_TAG_DRIVER(DSA_TAG_PROTO_KSZ9477);

#define KSZ9893_TAIL_TAG_OVERRIDE	BIT(5)
#define KSZ9893_TAIL_TAG_LOOKUP		BIT(6)

static struct sk_buff *ksz9893_xmit(struct sk_buff *skb,
				    struct net_device *dev)
{
	struct dsa_port *dp = dsa_slave_to_port(dev);
	u8 *addr;
	u8 *tag;

	if (skb->ip_summed == CHECKSUM_PARTIAL && skb_checksum_help(skb))
		return NULL;

	/* Tag encoding */
	tag = skb_put(skb, KSZ_INGRESS_TAG_LEN);
	addr = skb_mac_header(skb);

	*tag = BIT(dp->index);

	if (is_link_local_ether_addr(addr))
		*tag |= KSZ9893_TAIL_TAG_OVERRIDE;

	return skb;
}

static const struct dsa_device_ops ksz9893_netdev_ops = {
	.name	= "ksz9893",
	.proto	= DSA_TAG_PROTO_KSZ9893,
	.xmit	= ksz9893_xmit,
	.rcv	= ksz9477_rcv,
	.needed_tailroom = KSZ_INGRESS_TAG_LEN,
};

DSA_TAG_DRIVER(ksz9893_netdev_ops);
MODULE_ALIAS_DSA_TAG_DRIVER(DSA_TAG_PROTO_KSZ9893);

static struct dsa_tag_driver *dsa_tag_driver_array[] = {
	&DSA_TAG_DRIVER_NAME(ksz8795_netdev_ops),
	&DSA_TAG_DRIVER_NAME(ksz9477_netdev_ops),
	&DSA_TAG_DRIVER_NAME(ksz9893_netdev_ops),
};

module_dsa_tag_drivers(dsa_tag_driver_array);

MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
