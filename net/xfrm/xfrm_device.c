<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * xfrm_device.c - IPsec device offloading code.
 *
 * Copyright (c) 2015 secunet Security Networks AG
 *
 * Author:
 * Steffen Klassert <steffen.klassert@secunet.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <net/dst.h>
#include <net/xfrm.h>
#include <linux/notifier.h>

#ifdef CONFIG_XFRM_OFFLOAD
<<<<<<< HEAD
=======
static void __xfrm_transport_prep(struct xfrm_state *x, struct sk_buff *skb,
				  unsigned int hsize)
{
	struct xfrm_offload *xo = xfrm_offload(skb);

	skb_reset_mac_len(skb);
	if (xo->flags & XFRM_GSO_SEGMENT)
		skb->transport_header -= x->props.header_len;

	pskb_pull(skb, skb_transport_offset(skb) + x->props.header_len);
}

static void __xfrm_mode_tunnel_prep(struct xfrm_state *x, struct sk_buff *skb,
				    unsigned int hsize)

{
	struct xfrm_offload *xo = xfrm_offload(skb);

	if (xo->flags & XFRM_GSO_SEGMENT)
		skb->transport_header = skb->network_header + hsize;

	skb_reset_mac_len(skb);
	pskb_pull(skb, skb->mac_len + x->props.header_len);
}

static void __xfrm_mode_beet_prep(struct xfrm_state *x, struct sk_buff *skb,
				  unsigned int hsize)
{
	struct xfrm_offload *xo = xfrm_offload(skb);
	int phlen = 0;

	if (xo->flags & XFRM_GSO_SEGMENT)
		skb->transport_header = skb->network_header + hsize;

	skb_reset_mac_len(skb);
	if (x->sel.family != AF_INET6) {
		phlen = IPV4_BEET_PHMAXLEN;
		if (x->outer_mode.family == AF_INET6)
			phlen += sizeof(struct ipv6hdr) - sizeof(struct iphdr);
	}

	pskb_pull(skb, skb->mac_len + hsize + (x->props.header_len - phlen));
}

/* Adjust pointers into the packet when IPsec is done at layer2 */
static void xfrm_outer_mode_prep(struct xfrm_state *x, struct sk_buff *skb)
{
	switch (x->outer_mode.encap) {
	case XFRM_MODE_TUNNEL:
		if (x->outer_mode.family == AF_INET)
			return __xfrm_mode_tunnel_prep(x, skb,
						       sizeof(struct iphdr));
		if (x->outer_mode.family == AF_INET6)
			return __xfrm_mode_tunnel_prep(x, skb,
						       sizeof(struct ipv6hdr));
		break;
	case XFRM_MODE_TRANSPORT:
		if (x->outer_mode.family == AF_INET)
			return __xfrm_transport_prep(x, skb,
						     sizeof(struct iphdr));
		if (x->outer_mode.family == AF_INET6)
			return __xfrm_transport_prep(x, skb,
						     sizeof(struct ipv6hdr));
		break;
	case XFRM_MODE_BEET:
		if (x->outer_mode.family == AF_INET)
			return __xfrm_mode_beet_prep(x, skb,
						     sizeof(struct iphdr));
		if (x->outer_mode.family == AF_INET6)
			return __xfrm_mode_beet_prep(x, skb,
						     sizeof(struct ipv6hdr));
		break;
	case XFRM_MODE_ROUTEOPTIMIZATION:
	case XFRM_MODE_IN_TRIGGER:
		break;
	}
}

>>>>>>> upstream/android-13
struct sk_buff *validate_xmit_xfrm(struct sk_buff *skb, netdev_features_t features, bool *again)
{
	int err;
	unsigned long flags;
	struct xfrm_state *x;
<<<<<<< HEAD
	struct sk_buff *skb2;
	struct softnet_data *sd;
	netdev_features_t esp_features = features;
	struct xfrm_offload *xo = xfrm_offload(skb);
=======
	struct softnet_data *sd;
	struct sk_buff *skb2, *nskb, *pskb = NULL;
	netdev_features_t esp_features = features;
	struct xfrm_offload *xo = xfrm_offload(skb);
	struct net_device *dev = skb->dev;
	struct sec_path *sp;
>>>>>>> upstream/android-13

	if (!xo || (xo->flags & XFRM_XMIT))
		return skb;

	if (!(features & NETIF_F_HW_ESP))
		esp_features = features & ~(NETIF_F_SG | NETIF_F_CSUM_MASK);

<<<<<<< HEAD
	x = skb->sp->xvec[skb->sp->len - 1];
	if (xo->flags & XFRM_GRO || x->xso.flags & XFRM_OFFLOAD_INBOUND)
		return skb;

=======
	sp = skb_sec_path(skb);
	x = sp->xvec[sp->len - 1];
	if (xo->flags & XFRM_GRO || x->xso.flags & XFRM_OFFLOAD_INBOUND)
		return skb;

	/* This skb was already validated on the upper/virtual dev */
	if ((x->xso.dev != dev) && (x->xso.real_dev == dev))
		return skb;

>>>>>>> upstream/android-13
	local_irq_save(flags);
	sd = this_cpu_ptr(&softnet_data);
	err = !skb_queue_empty(&sd->xfrm_backlog);
	local_irq_restore(flags);

	if (err) {
		*again = true;
		return skb;
	}

<<<<<<< HEAD
	xo->flags |= XFRM_XMIT;

	if (skb_is_gso(skb)) {
		struct net_device *dev = skb->dev;

		if (unlikely(x->xso.dev != dev)) {
			struct sk_buff *segs;

			/* Packet got rerouted, fixup features and segment it. */
			esp_features = esp_features & ~(NETIF_F_HW_ESP
							| NETIF_F_GSO_ESP);

			segs = skb_gso_segment(skb, esp_features);
			if (IS_ERR(segs)) {
				kfree_skb(skb);
				atomic_long_inc(&dev->tx_dropped);
				return NULL;
			} else {
				consume_skb(skb);
				skb = segs;
			}
=======
	if (skb_is_gso(skb) && unlikely(x->xso.dev != dev)) {
		struct sk_buff *segs;

		/* Packet got rerouted, fixup features and segment it. */
		esp_features = esp_features & ~(NETIF_F_HW_ESP | NETIF_F_GSO_ESP);

		segs = skb_gso_segment(skb, esp_features);
		if (IS_ERR(segs)) {
			kfree_skb(skb);
			atomic_long_inc(&dev->tx_dropped);
			return NULL;
		} else {
			consume_skb(skb);
			skb = segs;
>>>>>>> upstream/android-13
		}
	}

	if (!skb->next) {
<<<<<<< HEAD
		x->outer_mode->xmit(x, skb);
=======
		esp_features |= skb->dev->gso_partial_features;
		xfrm_outer_mode_prep(x, skb);
>>>>>>> upstream/android-13

		xo->flags |= XFRM_DEV_RESUME;

		err = x->type_offload->xmit(x, skb, esp_features);
		if (err) {
			if (err == -EINPROGRESS)
				return NULL;

			XFRM_INC_STATS(xs_net(x), LINUX_MIB_XFRMOUTSTATEPROTOERROR);
			kfree_skb(skb);
			return NULL;
		}

		skb_push(skb, skb->data - skb_mac_header(skb));

		return skb;
	}

<<<<<<< HEAD
	skb2 = skb;

	do {
		struct sk_buff *nskb = skb2->next;
		skb2->next = NULL;
=======
	skb_list_walk_safe(skb, skb2, nskb) {
		esp_features |= skb->dev->gso_partial_features;
		skb_mark_not_on_list(skb2);
>>>>>>> upstream/android-13

		xo = xfrm_offload(skb2);
		xo->flags |= XFRM_DEV_RESUME;

<<<<<<< HEAD
		x->outer_mode->xmit(x, skb2);
=======
		xfrm_outer_mode_prep(x, skb2);
>>>>>>> upstream/android-13

		err = x->type_offload->xmit(x, skb2, esp_features);
		if (!err) {
			skb2->next = nskb;
		} else if (err != -EINPROGRESS) {
			XFRM_INC_STATS(xs_net(x), LINUX_MIB_XFRMOUTSTATEPROTOERROR);
			skb2->next = nskb;
			kfree_skb_list(skb2);
			return NULL;
		} else {
			if (skb == skb2)
				skb = nskb;
<<<<<<< HEAD

			if (!skb)
				return NULL;

			goto skip_push;
		}

		skb_push(skb2, skb2->data - skb_mac_header(skb2));

skip_push:
		skb2 = nskb;
	} while (skb2);
=======
			else
				pskb->next = nskb;

			continue;
		}

		skb_push(skb2, skb2->data - skb_mac_header(skb2));
		pskb = skb2;
	}
>>>>>>> upstream/android-13

	return skb;
}
EXPORT_SYMBOL_GPL(validate_xmit_xfrm);

int xfrm_dev_state_add(struct net *net, struct xfrm_state *x,
		       struct xfrm_user_offload *xuo)
{
	int err;
	struct dst_entry *dst;
	struct net_device *dev;
	struct xfrm_state_offload *xso = &x->xso;
	xfrm_address_t *saddr;
	xfrm_address_t *daddr;

	if (!x->type_offload)
		return -EINVAL;

	/* We don't yet support UDP encapsulation and TFC padding. */
	if (x->encap || x->tfcpad)
		return -EINVAL;

<<<<<<< HEAD
=======
	if (xuo->flags & ~(XFRM_OFFLOAD_IPV6 | XFRM_OFFLOAD_INBOUND))
		return -EINVAL;

>>>>>>> upstream/android-13
	dev = dev_get_by_index(net, xuo->ifindex);
	if (!dev) {
		if (!(xuo->flags & XFRM_OFFLOAD_INBOUND)) {
			saddr = &x->props.saddr;
			daddr = &x->id.daddr;
		} else {
			saddr = &x->id.daddr;
			daddr = &x->props.saddr;
		}

		dst = __xfrm_dst_lookup(net, 0, 0, saddr, daddr,
					x->props.family,
					xfrm_smark_get(0, x));
		if (IS_ERR(dst))
			return 0;

		dev = dst->dev;

		dev_hold(dev);
		dst_release(dst);
	}

	if (!dev->xfrmdev_ops || !dev->xfrmdev_ops->xdo_dev_state_add) {
		xso->dev = NULL;
		dev_put(dev);
		return 0;
	}

	if (x->props.flags & XFRM_STATE_ESN &&
	    !dev->xfrmdev_ops->xdo_dev_state_advance_esn) {
		xso->dev = NULL;
		dev_put(dev);
		return -EINVAL;
	}

	xso->dev = dev;
<<<<<<< HEAD
	xso->num_exthdrs = 1;
	xso->flags = xuo->flags;

	err = dev->xfrmdev_ops->xdo_dev_state_add(x);
	if (err) {
		xso->dev = NULL;
		dev_put(dev);
		return err;
=======
	xso->real_dev = dev;
	xso->num_exthdrs = 1;
	/* Don't forward bit that is not implemented */
	xso->flags = xuo->flags & ~XFRM_OFFLOAD_IPV6;

	err = dev->xfrmdev_ops->xdo_dev_state_add(x);
	if (err) {
		xso->num_exthdrs = 0;
		xso->flags = 0;
		xso->dev = NULL;
		xso->real_dev = NULL;
		dev_put(dev);

		if (err != -EOPNOTSUPP)
			return err;
>>>>>>> upstream/android-13
	}

	return 0;
}
EXPORT_SYMBOL_GPL(xfrm_dev_state_add);

bool xfrm_dev_offload_ok(struct sk_buff *skb, struct xfrm_state *x)
{
	int mtu;
	struct dst_entry *dst = skb_dst(skb);
	struct xfrm_dst *xdst = (struct xfrm_dst *)dst;
	struct net_device *dev = x->xso.dev;

	if (!x->type_offload || x->encap)
		return false;

	if ((!dev || (dev == xfrm_dst_path(dst)->dev)) &&
<<<<<<< HEAD
	    (!xdst->child->xfrm && x->type->get_mtu)) {
		mtu = x->type->get_mtu(x, xdst->child_mtu_cached);

=======
	    (!xdst->child->xfrm)) {
		mtu = xfrm_state_mtu(x, xdst->child_mtu_cached);
>>>>>>> upstream/android-13
		if (skb->len <= mtu)
			goto ok;

		if (skb_is_gso(skb) && skb_gso_validate_network_len(skb, mtu))
			goto ok;
	}

	return false;

ok:
	if (dev && dev->xfrmdev_ops && dev->xfrmdev_ops->xdo_dev_offload_ok)
		return x->xso.dev->xfrmdev_ops->xdo_dev_offload_ok(skb, x);

	return true;
}
EXPORT_SYMBOL_GPL(xfrm_dev_offload_ok);

void xfrm_dev_resume(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	int ret = NETDEV_TX_BUSY;
	struct netdev_queue *txq;
	struct softnet_data *sd;
	unsigned long flags;

	rcu_read_lock();
<<<<<<< HEAD
	txq = netdev_pick_tx(dev, skb, NULL);
=======
	txq = netdev_core_pick_tx(dev, skb, NULL);
>>>>>>> upstream/android-13

	HARD_TX_LOCK(dev, txq, smp_processor_id());
	if (!netif_xmit_frozen_or_stopped(txq))
		skb = dev_hard_start_xmit(skb, dev, txq, &ret);
	HARD_TX_UNLOCK(dev, txq);

	if (!dev_xmit_complete(ret)) {
		local_irq_save(flags);
		sd = this_cpu_ptr(&softnet_data);
		skb_queue_tail(&sd->xfrm_backlog, skb);
		raise_softirq_irqoff(NET_TX_SOFTIRQ);
		local_irq_restore(flags);
	}
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(xfrm_dev_resume);

void xfrm_dev_backlog(struct softnet_data *sd)
{
	struct sk_buff_head *xfrm_backlog = &sd->xfrm_backlog;
	struct sk_buff_head list;
	struct sk_buff *skb;

	if (skb_queue_empty(xfrm_backlog))
		return;

	__skb_queue_head_init(&list);

	spin_lock(&xfrm_backlog->lock);
	skb_queue_splice_init(xfrm_backlog, &list);
	spin_unlock(&xfrm_backlog->lock);

	while (!skb_queue_empty(&list)) {
		skb = __skb_dequeue(&list);
		xfrm_dev_resume(skb);
	}

}
#endif

static int xfrm_api_check(struct net_device *dev)
{
#ifdef CONFIG_XFRM_OFFLOAD
	if ((dev->features & NETIF_F_HW_ESP_TX_CSUM) &&
	    !(dev->features & NETIF_F_HW_ESP))
		return NOTIFY_BAD;

	if ((dev->features & NETIF_F_HW_ESP) &&
	    (!(dev->xfrmdev_ops &&
	       dev->xfrmdev_ops->xdo_dev_state_add &&
	       dev->xfrmdev_ops->xdo_dev_state_delete)))
		return NOTIFY_BAD;
#else
	if (dev->features & (NETIF_F_HW_ESP | NETIF_F_HW_ESP_TX_CSUM))
		return NOTIFY_BAD;
#endif

	return NOTIFY_DONE;
}

static int xfrm_dev_register(struct net_device *dev)
{
	return xfrm_api_check(dev);
}

static int xfrm_dev_feat_change(struct net_device *dev)
{
	return xfrm_api_check(dev);
}

static int xfrm_dev_down(struct net_device *dev)
{
	if (dev->features & NETIF_F_HW_ESP)
		xfrm_dev_state_flush(dev_net(dev), dev, true);

	return NOTIFY_DONE;
}

static int xfrm_dev_event(struct notifier_block *this, unsigned long event, void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);

	switch (event) {
	case NETDEV_REGISTER:
		return xfrm_dev_register(dev);

	case NETDEV_FEAT_CHANGE:
		return xfrm_dev_feat_change(dev);

	case NETDEV_DOWN:
	case NETDEV_UNREGISTER:
		return xfrm_dev_down(dev);
	}
	return NOTIFY_DONE;
}

static struct notifier_block xfrm_dev_notifier = {
	.notifier_call	= xfrm_dev_event,
};

void __init xfrm_dev_init(void)
{
	register_netdevice_notifier(&xfrm_dev_notifier);
}
