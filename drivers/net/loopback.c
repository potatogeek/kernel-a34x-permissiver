<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Pseudo-driver for the loopback interface.
 *
 * Version:	@(#)loopback.c	1.0.4b	08/16/93
 *
 * Authors:	Ross Biro
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *		Donald Becker, <becker@scyld.com>
 *
 *		Alan Cox	:	Fixed oddments for NET3.014
 *		Alan Cox	:	Rejig for NET3.029 snap #3
 *		Alan Cox	:	Fixed NET3.029 bugs and sped up
 *		Larry McVoy	:	Tiny tweak to double performance
 *		Alan Cox	:	Backed out LMV's tweak - the linux mm
 *					can't take it...
 *              Michael Griffith:       Don't bother computing the checksums
 *                                      on packets received on the loopback
 *                                      interface.
 *		Alexey Kuznetsov:	Potential hang under some extreme
 *					cases removed.
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/in.h>

#include <linux/uaccess.h>
#include <linux/io.h>

#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/ethtool.h>
#include <net/sock.h>
#include <net/checksum.h>
#include <linux/if_ether.h>	/* For the statistics structure. */
#include <linux/if_arp.h>	/* For ARPHRD_ETHER */
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/percpu.h>
#include <linux/net_tstamp.h>
#include <net/net_namespace.h>
#include <linux/u64_stats_sync.h>

<<<<<<< HEAD
struct pcpu_lstats {
	u64			packets;
	u64			bytes;
	struct u64_stats_sync	syncp;
};
=======
/* blackhole_netdev - a device used for dsts that are marked expired!
 * This is global device (instead of per-net-ns) since it's not needed
 * to be per-ns and gets initialized at boot time.
 */
struct net_device *blackhole_netdev;
EXPORT_SYMBOL(blackhole_netdev);
>>>>>>> upstream/android-13

/* The higher levels take care of making this non-reentrant (it's
 * called with bh's disabled).
 */
static netdev_tx_t loopback_xmit(struct sk_buff *skb,
				 struct net_device *dev)
{
<<<<<<< HEAD
	struct pcpu_lstats *lb_stats;
=======
>>>>>>> upstream/android-13
	int len;

	skb_tx_timestamp(skb);

	/* do not fool net_timestamp_check() with various clock bases */
	skb->tstamp = 0;

	skb_orphan(skb);

	/* Before queueing this packet to netif_rx(),
	 * make sure dst is refcounted.
	 */
	skb_dst_force(skb);

	skb->protocol = eth_type_trans(skb, dev);

<<<<<<< HEAD
	/* it's OK to use per_cpu_ptr() because BHs are off */
	lb_stats = this_cpu_ptr(dev->lstats);

	len = skb->len;
	if (likely(netif_rx(skb) == NET_RX_SUCCESS)) {
		u64_stats_update_begin(&lb_stats->syncp);
		lb_stats->bytes += len;
		lb_stats->packets++;
		u64_stats_update_end(&lb_stats->syncp);
	}
=======
	len = skb->len;
	if (likely(netif_rx(skb) == NET_RX_SUCCESS))
		dev_lstats_add(dev, len);
>>>>>>> upstream/android-13

	return NETDEV_TX_OK;
}

<<<<<<< HEAD
static void loopback_get_stats64(struct net_device *dev,
				 struct rtnl_link_stats64 *stats)
{
	u64 bytes = 0;
	u64 packets = 0;
	int i;

=======
void dev_lstats_read(struct net_device *dev, u64 *packets, u64 *bytes)
{
	int i;

	*packets = 0;
	*bytes = 0;

>>>>>>> upstream/android-13
	for_each_possible_cpu(i) {
		const struct pcpu_lstats *lb_stats;
		u64 tbytes, tpackets;
		unsigned int start;

		lb_stats = per_cpu_ptr(dev->lstats, i);
		do {
			start = u64_stats_fetch_begin_irq(&lb_stats->syncp);
<<<<<<< HEAD
			tbytes = lb_stats->bytes;
			tpackets = lb_stats->packets;
		} while (u64_stats_fetch_retry_irq(&lb_stats->syncp, start));
		bytes   += tbytes;
		packets += tpackets;
	}
=======
			tpackets = u64_stats_read(&lb_stats->packets);
			tbytes = u64_stats_read(&lb_stats->bytes);
		} while (u64_stats_fetch_retry_irq(&lb_stats->syncp, start));
		*bytes   += tbytes;
		*packets += tpackets;
	}
}
EXPORT_SYMBOL(dev_lstats_read);

static void loopback_get_stats64(struct net_device *dev,
				 struct rtnl_link_stats64 *stats)
{
	u64 packets, bytes;

	dev_lstats_read(dev, &packets, &bytes);

>>>>>>> upstream/android-13
	stats->rx_packets = packets;
	stats->tx_packets = packets;
	stats->rx_bytes   = bytes;
	stats->tx_bytes   = bytes;
}

static u32 always_on(struct net_device *dev)
{
	return 1;
}

<<<<<<< HEAD
static int loopback_get_ts_info(struct net_device *netdev,
				struct ethtool_ts_info *ts_info)
{
	ts_info->so_timestamping = SOF_TIMESTAMPING_TX_SOFTWARE |
				   SOF_TIMESTAMPING_RX_SOFTWARE |
				   SOF_TIMESTAMPING_SOFTWARE;

	ts_info->phc_index = -1;

	return 0;
};

static const struct ethtool_ops loopback_ethtool_ops = {
	.get_link		= always_on,
	.get_ts_info		= loopback_get_ts_info,
=======
static const struct ethtool_ops loopback_ethtool_ops = {
	.get_link		= always_on,
	.get_ts_info		= ethtool_op_get_ts_info,
>>>>>>> upstream/android-13
};

static int loopback_dev_init(struct net_device *dev)
{
	dev->lstats = netdev_alloc_pcpu_stats(struct pcpu_lstats);
	if (!dev->lstats)
		return -ENOMEM;
	return 0;
}

static void loopback_dev_free(struct net_device *dev)
{
	dev_net(dev)->loopback_dev = NULL;
	free_percpu(dev->lstats);
}

static const struct net_device_ops loopback_ops = {
	.ndo_init        = loopback_dev_init,
	.ndo_start_xmit  = loopback_xmit,
	.ndo_get_stats64 = loopback_get_stats64,
	.ndo_set_mac_address = eth_mac_addr,
};

<<<<<<< HEAD
/* The loopback device is special. There is only one instance
 * per network namespace.
 */
static void loopback_setup(struct net_device *dev)
{
	dev->mtu		= 64 * 1024;
=======
static void gen_lo_setup(struct net_device *dev,
			 unsigned int mtu,
			 const struct ethtool_ops *eth_ops,
			 const struct header_ops *hdr_ops,
			 const struct net_device_ops *dev_ops,
			 void (*dev_destructor)(struct net_device *dev))
{
	dev->mtu		= mtu;
>>>>>>> upstream/android-13
	dev->hard_header_len	= ETH_HLEN;	/* 14	*/
	dev->min_header_len	= ETH_HLEN;	/* 14	*/
	dev->addr_len		= ETH_ALEN;	/* 6	*/
	dev->type		= ARPHRD_LOOPBACK;	/* 0x0001*/
	dev->flags		= IFF_LOOPBACK;
	dev->priv_flags		|= IFF_LIVE_ADDR_CHANGE | IFF_NO_QUEUE;
	netif_keep_dst(dev);
	dev->hw_features	= NETIF_F_GSO_SOFTWARE;
	dev->features		= NETIF_F_SG | NETIF_F_FRAGLIST
		| NETIF_F_GSO_SOFTWARE
		| NETIF_F_HW_CSUM
		| NETIF_F_RXCSUM
		| NETIF_F_SCTP_CRC
		| NETIF_F_HIGHDMA
		| NETIF_F_LLTX
		| NETIF_F_NETNS_LOCAL
		| NETIF_F_VLAN_CHALLENGED
		| NETIF_F_LOOPBACK;
<<<<<<< HEAD
	dev->ethtool_ops	= &loopback_ethtool_ops;
	dev->header_ops		= &eth_header_ops;
	dev->netdev_ops		= &loopback_ops;
	dev->needs_free_netdev	= true;
	dev->priv_destructor	= loopback_dev_free;
=======
	dev->ethtool_ops	= eth_ops;
	dev->header_ops		= hdr_ops;
	dev->netdev_ops		= dev_ops;
	dev->needs_free_netdev	= true;
	dev->priv_destructor	= dev_destructor;
}

/* The loopback device is special. There is only one instance
 * per network namespace.
 */
static void loopback_setup(struct net_device *dev)
{
	gen_lo_setup(dev, (64 * 1024), &loopback_ethtool_ops, &eth_header_ops,
		     &loopback_ops, loopback_dev_free);
>>>>>>> upstream/android-13
}

/* Setup and register the loopback device. */
static __net_init int loopback_net_init(struct net *net)
{
	struct net_device *dev;
	int err;

	err = -ENOMEM;
	dev = alloc_netdev(0, "lo", NET_NAME_UNKNOWN, loopback_setup);
	if (!dev)
		goto out;

	dev_net_set(dev, net);
	err = register_netdev(dev);
	if (err)
		goto out_free_netdev;

	BUG_ON(dev->ifindex != LOOPBACK_IFINDEX);
	net->loopback_dev = dev;
	return 0;

out_free_netdev:
	free_netdev(dev);
out:
	if (net_eq(net, &init_net))
		panic("loopback: Failed to register netdevice: %d\n", err);
	return err;
}

/* Registered in net/core/dev.c */
struct pernet_operations __net_initdata loopback_net_ops = {
	.init = loopback_net_init,
};
<<<<<<< HEAD
=======

/* blackhole netdevice */
static netdev_tx_t blackhole_netdev_xmit(struct sk_buff *skb,
					 struct net_device *dev)
{
	kfree_skb(skb);
	net_warn_ratelimited("%s(): Dropping skb.\n", __func__);
	return NETDEV_TX_OK;
}

static const struct net_device_ops blackhole_netdev_ops = {
	.ndo_start_xmit = blackhole_netdev_xmit,
};

/* This is a dst-dummy device used specifically for invalidated
 * DSTs and unlike loopback, this is not per-ns.
 */
static void blackhole_netdev_setup(struct net_device *dev)
{
	gen_lo_setup(dev, ETH_MIN_MTU, NULL, NULL, &blackhole_netdev_ops, NULL);
}

/* Setup and register the blackhole_netdev. */
static int __init blackhole_netdev_init(void)
{
	blackhole_netdev = alloc_netdev(0, "blackhole_dev", NET_NAME_UNKNOWN,
					blackhole_netdev_setup);
	if (!blackhole_netdev)
		return -ENOMEM;

	rtnl_lock();
	dev_init_scheduler(blackhole_netdev);
	dev_activate(blackhole_netdev);
	rtnl_unlock();

	blackhole_netdev->flags |= IFF_UP | IFF_RUNNING;
	dev_net_set(blackhole_netdev, &init_net);

	return 0;
}

device_initcall(blackhole_netdev_init);
>>>>>>> upstream/android-13
