// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
/* Copyright (C) 2014-2018  B.A.T.M.A.N. contributors:
 *
 * Linus Lüssing
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
=======
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Linus Lüssing
>>>>>>> upstream/android-13
 */

#include "multicast.h"
#include "main.h"

#include <linux/atomic.h>
#include <linux/bitops.h>
#include <linux/bug.h>
#include <linux/byteorder/generic.h>
#include <linux/errno.h>
#include <linux/etherdevice.h>
#include <linux/gfp.h>
#include <linux/icmpv6.h>
#include <linux/if_bridge.h>
#include <linux/if_ether.h>
#include <linux/igmp.h>
#include <linux/in.h>
#include <linux/in6.h>
<<<<<<< HEAD
=======
#include <linux/inetdevice.h>
>>>>>>> upstream/android-13
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/kref.h>
#include <linux/list.h>
#include <linux/lockdep.h>
#include <linux/netdevice.h>
#include <linux/netlink.h>
#include <linux/printk.h>
#include <linux/rculist.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD
#include <linux/seq_file.h>
=======
>>>>>>> upstream/android-13
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/stddef.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <net/addrconf.h>
#include <net/genetlink.h>
#include <net/if_inet6.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/netlink.h>
#include <net/sock.h>
#include <uapi/linux/batadv_packet.h>
#include <uapi/linux/batman_adv.h>

<<<<<<< HEAD
=======
#include "bridge_loop_avoidance.h"
>>>>>>> upstream/android-13
#include "hard-interface.h"
#include "hash.h"
#include "log.h"
#include "netlink.h"
<<<<<<< HEAD
=======
#include "send.h"
>>>>>>> upstream/android-13
#include "soft-interface.h"
#include "translation-table.h"
#include "tvlv.h"

static void batadv_mcast_mla_update(struct work_struct *work);

/**
 * batadv_mcast_start_timer() - schedule the multicast periodic worker
 * @bat_priv: the bat priv with all the soft interface information
 */
static void batadv_mcast_start_timer(struct batadv_priv *bat_priv)
{
	queue_delayed_work(batadv_event_workqueue, &bat_priv->mcast.work,
			   msecs_to_jiffies(BATADV_MCAST_WORK_PERIOD));
}

/**
 * batadv_mcast_get_bridge() - get the bridge on top of the softif if it exists
 * @soft_iface: netdev struct of the mesh interface
 *
 * If the given soft interface has a bridge on top then the refcount
 * of the according net device is increased.
 *
 * Return: NULL if no such bridge exists. Otherwise the net device of the
 * bridge.
 */
static struct net_device *batadv_mcast_get_bridge(struct net_device *soft_iface)
{
	struct net_device *upper = soft_iface;

	rcu_read_lock();
	do {
		upper = netdev_master_upper_dev_get_rcu(upper);
	} while (upper && !(upper->priv_flags & IFF_EBRIDGE));

<<<<<<< HEAD
	if (upper)
		dev_hold(upper);
=======
	dev_hold(upper);
>>>>>>> upstream/android-13
	rcu_read_unlock();

	return upper;
}

/**
<<<<<<< HEAD
 * batadv_mcast_addr_is_ipv4() - check if multicast MAC is IPv4
 * @addr: the MAC address to check
 *
 * Return: True, if MAC address is one reserved for IPv4 multicast, false
 * otherwise.
 */
static bool batadv_mcast_addr_is_ipv4(const u8 *addr)
{
	static const u8 prefix[] = {0x01, 0x00, 0x5E};

	return memcmp(prefix, addr, sizeof(prefix)) == 0;
}

/**
 * batadv_mcast_addr_is_ipv6() - check if multicast MAC is IPv6
 * @addr: the MAC address to check
 *
 * Return: True, if MAC address is one reserved for IPv6 multicast, false
 * otherwise.
 */
static bool batadv_mcast_addr_is_ipv6(const u8 *addr)
{
	static const u8 prefix[] = {0x33, 0x33};

	return memcmp(prefix, addr, sizeof(prefix)) == 0;
}

/**
 * batadv_mcast_mla_softif_get() - get softif multicast listeners
 * @bat_priv: the bat priv with all the soft interface information
 * @dev: the device to collect multicast addresses from
 * @mcast_list: a list to put found addresses into
 *
 * Collects multicast addresses of multicast listeners residing
 * on this kernel on the given soft interface, dev, in
 * the given mcast_list. In general, multicast listeners provided by
 * your multicast receiving applications run directly on this node.
 *
 * If there is a bridge interface on top of dev, collects from that one
 * instead. Just like with IP addresses and routes, multicast listeners
 * will(/should) register to the bridge interface instead of an
 * enslaved bat0.
 *
 * Return: -ENOMEM on memory allocation error or the number of
 * items added to the mcast_list otherwise.
 */
static int batadv_mcast_mla_softif_get(struct batadv_priv *bat_priv,
				       struct net_device *dev,
				       struct hlist_head *mcast_list)
{
	bool all_ipv4 = bat_priv->mcast.flags & BATADV_MCAST_WANT_ALL_IPV4;
	bool all_ipv6 = bat_priv->mcast.flags & BATADV_MCAST_WANT_ALL_IPV6;
	struct net_device *bridge = batadv_mcast_get_bridge(dev);
	struct netdev_hw_addr *mc_list_entry;
	struct batadv_hw_addr *new;
	int ret = 0;

	netif_addr_lock_bh(bridge ? bridge : dev);
	netdev_for_each_mc_addr(mc_list_entry, bridge ? bridge : dev) {
		if (all_ipv4 && batadv_mcast_addr_is_ipv4(mc_list_entry->addr))
			continue;

		if (all_ipv6 && batadv_mcast_addr_is_ipv6(mc_list_entry->addr))
			continue;

		new = kmalloc(sizeof(*new), GFP_ATOMIC);
		if (!new) {
			ret = -ENOMEM;
			break;
		}

		ether_addr_copy(new->addr, mc_list_entry->addr);
		hlist_add_head(&new->list, mcast_list);
		ret++;
	}
	netif_addr_unlock_bh(bridge ? bridge : dev);

	if (bridge)
		dev_put(bridge);

	return ret;
=======
 * batadv_mcast_mla_rtr_flags_softif_get_ipv4() - get mcast router flags from
 *  node for IPv4
 * @dev: the interface to check
 *
 * Checks the presence of an IPv4 multicast router on this node.
 *
 * Caller needs to hold rcu read lock.
 *
 * Return: BATADV_NO_FLAGS if present, BATADV_MCAST_WANT_NO_RTR4 otherwise.
 */
static u8 batadv_mcast_mla_rtr_flags_softif_get_ipv4(struct net_device *dev)
{
	struct in_device *in_dev = __in_dev_get_rcu(dev);

	if (in_dev && IN_DEV_MFORWARD(in_dev))
		return BATADV_NO_FLAGS;
	else
		return BATADV_MCAST_WANT_NO_RTR4;
}

/**
 * batadv_mcast_mla_rtr_flags_softif_get_ipv6() - get mcast router flags from
 *  node for IPv6
 * @dev: the interface to check
 *
 * Checks the presence of an IPv6 multicast router on this node.
 *
 * Caller needs to hold rcu read lock.
 *
 * Return: BATADV_NO_FLAGS if present, BATADV_MCAST_WANT_NO_RTR6 otherwise.
 */
#if IS_ENABLED(CONFIG_IPV6_MROUTE)
static u8 batadv_mcast_mla_rtr_flags_softif_get_ipv6(struct net_device *dev)
{
	struct inet6_dev *in6_dev = __in6_dev_get(dev);

	if (in6_dev && atomic_read(&in6_dev->cnf.mc_forwarding))
		return BATADV_NO_FLAGS;
	else
		return BATADV_MCAST_WANT_NO_RTR6;
}
#else
static inline u8
batadv_mcast_mla_rtr_flags_softif_get_ipv6(struct net_device *dev)
{
	return BATADV_MCAST_WANT_NO_RTR6;
}
#endif

/**
 * batadv_mcast_mla_rtr_flags_softif_get() - get mcast router flags from node
 * @bat_priv: the bat priv with all the soft interface information
 * @bridge: bridge interface on top of the soft_iface if present,
 *  otherwise pass NULL
 *
 * Checks the presence of IPv4 and IPv6 multicast routers on this
 * node.
 *
 * Return:
 *	BATADV_NO_FLAGS: Both an IPv4 and IPv6 multicast router is present
 *	BATADV_MCAST_WANT_NO_RTR4: No IPv4 multicast router is present
 *	BATADV_MCAST_WANT_NO_RTR6: No IPv6 multicast router is present
 *	The former two OR'd: no multicast router is present
 */
static u8 batadv_mcast_mla_rtr_flags_softif_get(struct batadv_priv *bat_priv,
						struct net_device *bridge)
{
	struct net_device *dev = bridge ? bridge : bat_priv->soft_iface;
	u8 flags = BATADV_NO_FLAGS;

	rcu_read_lock();

	flags |= batadv_mcast_mla_rtr_flags_softif_get_ipv4(dev);
	flags |= batadv_mcast_mla_rtr_flags_softif_get_ipv6(dev);

	rcu_read_unlock();

	return flags;
}

/**
 * batadv_mcast_mla_rtr_flags_bridge_get() - get mcast router flags from bridge
 * @bat_priv: the bat priv with all the soft interface information
 * @bridge: bridge interface on top of the soft_iface if present,
 *  otherwise pass NULL
 *
 * Checks the presence of IPv4 and IPv6 multicast routers behind a bridge.
 *
 * Return:
 *	BATADV_NO_FLAGS: Both an IPv4 and IPv6 multicast router is present
 *	BATADV_MCAST_WANT_NO_RTR4: No IPv4 multicast router is present
 *	BATADV_MCAST_WANT_NO_RTR6: No IPv6 multicast router is present
 *	The former two OR'd: no multicast router is present
 */
static u8 batadv_mcast_mla_rtr_flags_bridge_get(struct batadv_priv *bat_priv,
						struct net_device *bridge)
{
	struct net_device *dev = bat_priv->soft_iface;
	u8 flags = BATADV_NO_FLAGS;

	if (!bridge)
		return BATADV_MCAST_WANT_NO_RTR4 | BATADV_MCAST_WANT_NO_RTR6;

	if (!br_multicast_has_router_adjacent(dev, ETH_P_IP))
		flags |= BATADV_MCAST_WANT_NO_RTR4;
	if (!br_multicast_has_router_adjacent(dev, ETH_P_IPV6))
		flags |= BATADV_MCAST_WANT_NO_RTR6;

	return flags;
}

/**
 * batadv_mcast_mla_rtr_flags_get() - get multicast router flags
 * @bat_priv: the bat priv with all the soft interface information
 * @bridge: bridge interface on top of the soft_iface if present,
 *  otherwise pass NULL
 *
 * Checks the presence of IPv4 and IPv6 multicast routers on this
 * node or behind its bridge.
 *
 * Return:
 *	BATADV_NO_FLAGS: Both an IPv4 and IPv6 multicast router is present
 *	BATADV_MCAST_WANT_NO_RTR4: No IPv4 multicast router is present
 *	BATADV_MCAST_WANT_NO_RTR6: No IPv6 multicast router is present
 *	The former two OR'd: no multicast router is present
 */
static u8 batadv_mcast_mla_rtr_flags_get(struct batadv_priv *bat_priv,
					 struct net_device *bridge)
{
	u8 flags = BATADV_MCAST_WANT_NO_RTR4 | BATADV_MCAST_WANT_NO_RTR6;

	flags &= batadv_mcast_mla_rtr_flags_softif_get(bat_priv, bridge);
	flags &= batadv_mcast_mla_rtr_flags_bridge_get(bat_priv, bridge);

	return flags;
}

/**
 * batadv_mcast_mla_flags_get() - get the new multicast flags
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: A set of flags for the current/next TVLV, querier and
 * bridge state.
 */
static struct batadv_mcast_mla_flags
batadv_mcast_mla_flags_get(struct batadv_priv *bat_priv)
{
	struct net_device *dev = bat_priv->soft_iface;
	struct batadv_mcast_querier_state *qr4, *qr6;
	struct batadv_mcast_mla_flags mla_flags;
	struct net_device *bridge;

	bridge = batadv_mcast_get_bridge(dev);

	memset(&mla_flags, 0, sizeof(mla_flags));
	mla_flags.enabled = 1;
	mla_flags.tvlv_flags |= batadv_mcast_mla_rtr_flags_get(bat_priv,
							       bridge);

	if (!bridge)
		return mla_flags;

	dev_put(bridge);

	mla_flags.bridged = 1;
	qr4 = &mla_flags.querier_ipv4;
	qr6 = &mla_flags.querier_ipv6;

	if (!IS_ENABLED(CONFIG_BRIDGE_IGMP_SNOOPING))
		pr_warn_once("No bridge IGMP snooping compiled - multicast optimizations disabled\n");

	qr4->exists = br_multicast_has_querier_anywhere(dev, ETH_P_IP);
	qr4->shadowing = br_multicast_has_querier_adjacent(dev, ETH_P_IP);

	qr6->exists = br_multicast_has_querier_anywhere(dev, ETH_P_IPV6);
	qr6->shadowing = br_multicast_has_querier_adjacent(dev, ETH_P_IPV6);

	mla_flags.tvlv_flags |= BATADV_MCAST_WANT_ALL_UNSNOOPABLES;

	/* 1) If no querier exists at all, then multicast listeners on
	 *    our local TT clients behind the bridge will keep silent.
	 * 2) If the selected querier is on one of our local TT clients,
	 *    behind the bridge, then this querier might shadow multicast
	 *    listeners on our local TT clients, behind this bridge.
	 *
	 * In both cases, we will signalize other batman nodes that
	 * we need all multicast traffic of the according protocol.
	 */
	if (!qr4->exists || qr4->shadowing) {
		mla_flags.tvlv_flags |= BATADV_MCAST_WANT_ALL_IPV4;
		mla_flags.tvlv_flags &= ~BATADV_MCAST_WANT_NO_RTR4;
	}

	if (!qr6->exists || qr6->shadowing) {
		mla_flags.tvlv_flags |= BATADV_MCAST_WANT_ALL_IPV6;
		mla_flags.tvlv_flags &= ~BATADV_MCAST_WANT_NO_RTR6;
	}

	return mla_flags;
>>>>>>> upstream/android-13
}

/**
 * batadv_mcast_mla_is_duplicate() - check whether an address is in a list
 * @mcast_addr: the multicast address to check
 * @mcast_list: the list with multicast addresses to search in
 *
 * Return: true if the given address is already in the given list.
 * Otherwise returns false.
 */
static bool batadv_mcast_mla_is_duplicate(u8 *mcast_addr,
					  struct hlist_head *mcast_list)
{
	struct batadv_hw_addr *mcast_entry;

	hlist_for_each_entry(mcast_entry, mcast_list, list)
		if (batadv_compare_eth(mcast_entry->addr, mcast_addr))
			return true;

	return false;
}

/**
<<<<<<< HEAD
=======
 * batadv_mcast_mla_softif_get_ipv4() - get softif IPv4 multicast listeners
 * @dev: the device to collect multicast addresses from
 * @mcast_list: a list to put found addresses into
 * @flags: flags indicating the new multicast state
 *
 * Collects multicast addresses of IPv4 multicast listeners residing
 * on this kernel on the given soft interface, dev, in
 * the given mcast_list. In general, multicast listeners provided by
 * your multicast receiving applications run directly on this node.
 *
 * Return: -ENOMEM on memory allocation error or the number of
 * items added to the mcast_list otherwise.
 */
static int
batadv_mcast_mla_softif_get_ipv4(struct net_device *dev,
				 struct hlist_head *mcast_list,
				 struct batadv_mcast_mla_flags *flags)
{
	struct batadv_hw_addr *new;
	struct in_device *in_dev;
	u8 mcast_addr[ETH_ALEN];
	struct ip_mc_list *pmc;
	int ret = 0;

	if (flags->tvlv_flags & BATADV_MCAST_WANT_ALL_IPV4)
		return 0;

	rcu_read_lock();

	in_dev = __in_dev_get_rcu(dev);
	if (!in_dev) {
		rcu_read_unlock();
		return 0;
	}

	for (pmc = rcu_dereference(in_dev->mc_list); pmc;
	     pmc = rcu_dereference(pmc->next_rcu)) {
		if (flags->tvlv_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES &&
		    ipv4_is_local_multicast(pmc->multiaddr))
			continue;

		if (!(flags->tvlv_flags & BATADV_MCAST_WANT_NO_RTR4) &&
		    !ipv4_is_local_multicast(pmc->multiaddr))
			continue;

		ip_eth_mc_map(pmc->multiaddr, mcast_addr);

		if (batadv_mcast_mla_is_duplicate(mcast_addr, mcast_list))
			continue;

		new = kmalloc(sizeof(*new), GFP_ATOMIC);
		if (!new) {
			ret = -ENOMEM;
			break;
		}

		ether_addr_copy(new->addr, mcast_addr);
		hlist_add_head(&new->list, mcast_list);
		ret++;
	}
	rcu_read_unlock();

	return ret;
}

/**
 * batadv_mcast_mla_softif_get_ipv6() - get softif IPv6 multicast listeners
 * @dev: the device to collect multicast addresses from
 * @mcast_list: a list to put found addresses into
 * @flags: flags indicating the new multicast state
 *
 * Collects multicast addresses of IPv6 multicast listeners residing
 * on this kernel on the given soft interface, dev, in
 * the given mcast_list. In general, multicast listeners provided by
 * your multicast receiving applications run directly on this node.
 *
 * Return: -ENOMEM on memory allocation error or the number of
 * items added to the mcast_list otherwise.
 */
#if IS_ENABLED(CONFIG_IPV6)
static int
batadv_mcast_mla_softif_get_ipv6(struct net_device *dev,
				 struct hlist_head *mcast_list,
				 struct batadv_mcast_mla_flags *flags)
{
	struct batadv_hw_addr *new;
	struct inet6_dev *in6_dev;
	u8 mcast_addr[ETH_ALEN];
	struct ifmcaddr6 *pmc6;
	int ret = 0;

	if (flags->tvlv_flags & BATADV_MCAST_WANT_ALL_IPV6)
		return 0;

	rcu_read_lock();

	in6_dev = __in6_dev_get(dev);
	if (!in6_dev) {
		rcu_read_unlock();
		return 0;
	}

	for (pmc6 = rcu_dereference(in6_dev->mc_list);
	     pmc6;
	     pmc6 = rcu_dereference(pmc6->next)) {
		if (IPV6_ADDR_MC_SCOPE(&pmc6->mca_addr) <
		    IPV6_ADDR_SCOPE_LINKLOCAL)
			continue;

		if (flags->tvlv_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES &&
		    ipv6_addr_is_ll_all_nodes(&pmc6->mca_addr))
			continue;

		if (!(flags->tvlv_flags & BATADV_MCAST_WANT_NO_RTR6) &&
		    IPV6_ADDR_MC_SCOPE(&pmc6->mca_addr) >
		    IPV6_ADDR_SCOPE_LINKLOCAL)
			continue;

		ipv6_eth_mc_map(&pmc6->mca_addr, mcast_addr);

		if (batadv_mcast_mla_is_duplicate(mcast_addr, mcast_list))
			continue;

		new = kmalloc(sizeof(*new), GFP_ATOMIC);
		if (!new) {
			ret = -ENOMEM;
			break;
		}

		ether_addr_copy(new->addr, mcast_addr);
		hlist_add_head(&new->list, mcast_list);
		ret++;
	}
	rcu_read_unlock();

	return ret;
}
#else
static inline int
batadv_mcast_mla_softif_get_ipv6(struct net_device *dev,
				 struct hlist_head *mcast_list,
				 struct batadv_mcast_mla_flags *flags)
{
	return 0;
}
#endif

/**
 * batadv_mcast_mla_softif_get() - get softif multicast listeners
 * @dev: the device to collect multicast addresses from
 * @mcast_list: a list to put found addresses into
 * @flags: flags indicating the new multicast state
 *
 * Collects multicast addresses of multicast listeners residing
 * on this kernel on the given soft interface, dev, in
 * the given mcast_list. In general, multicast listeners provided by
 * your multicast receiving applications run directly on this node.
 *
 * If there is a bridge interface on top of dev, collect from that one
 * instead. Just like with IP addresses and routes, multicast listeners
 * will(/should) register to the bridge interface instead of an
 * enslaved bat0.
 *
 * Return: -ENOMEM on memory allocation error or the number of
 * items added to the mcast_list otherwise.
 */
static int
batadv_mcast_mla_softif_get(struct net_device *dev,
			    struct hlist_head *mcast_list,
			    struct batadv_mcast_mla_flags *flags)
{
	struct net_device *bridge = batadv_mcast_get_bridge(dev);
	int ret4, ret6 = 0;

	if (bridge)
		dev = bridge;

	ret4 = batadv_mcast_mla_softif_get_ipv4(dev, mcast_list, flags);
	if (ret4 < 0)
		goto out;

	ret6 = batadv_mcast_mla_softif_get_ipv6(dev, mcast_list, flags);
	if (ret6 < 0) {
		ret4 = 0;
		goto out;
	}

out:
	dev_put(bridge);

	return ret4 + ret6;
}

/**
>>>>>>> upstream/android-13
 * batadv_mcast_mla_br_addr_cpy() - copy a bridge multicast address
 * @dst: destination to write to - a multicast MAC address
 * @src: source to read from - a multicast IP address
 *
 * Converts a given multicast IPv4/IPv6 address from a bridge
 * to its matching multicast MAC address and copies it into the given
 * destination buffer.
 *
 * Caller needs to make sure the destination buffer can hold
 * at least ETH_ALEN bytes.
 */
static void batadv_mcast_mla_br_addr_cpy(char *dst, const struct br_ip *src)
{
	if (src->proto == htons(ETH_P_IP))
<<<<<<< HEAD
		ip_eth_mc_map(src->u.ip4, dst);
#if IS_ENABLED(CONFIG_IPV6)
	else if (src->proto == htons(ETH_P_IPV6))
		ipv6_eth_mc_map(&src->u.ip6, dst);
=======
		ip_eth_mc_map(src->dst.ip4, dst);
#if IS_ENABLED(CONFIG_IPV6)
	else if (src->proto == htons(ETH_P_IPV6))
		ipv6_eth_mc_map(&src->dst.ip6, dst);
>>>>>>> upstream/android-13
#endif
	else
		eth_zero_addr(dst);
}

/**
 * batadv_mcast_mla_bridge_get() - get bridged-in multicast listeners
<<<<<<< HEAD
 * @bat_priv: the bat priv with all the soft interface information
 * @dev: a bridge slave whose bridge to collect multicast addresses from
 * @mcast_list: a list to put found addresses into
=======
 * @dev: a bridge slave whose bridge to collect multicast addresses from
 * @mcast_list: a list to put found addresses into
 * @flags: flags indicating the new multicast state
>>>>>>> upstream/android-13
 *
 * Collects multicast addresses of multicast listeners residing
 * on foreign, non-mesh devices which we gave access to our mesh via
 * a bridge on top of the given soft interface, dev, in the given
 * mcast_list.
 *
 * Return: -ENOMEM on memory allocation error or the number of
 * items added to the mcast_list otherwise.
 */
<<<<<<< HEAD
static int batadv_mcast_mla_bridge_get(struct batadv_priv *bat_priv,
				       struct net_device *dev,
				       struct hlist_head *mcast_list)
{
	struct list_head bridge_mcast_list = LIST_HEAD_INIT(bridge_mcast_list);
	bool all_ipv4 = bat_priv->mcast.flags & BATADV_MCAST_WANT_ALL_IPV4;
	bool all_ipv6 = bat_priv->mcast.flags & BATADV_MCAST_WANT_ALL_IPV6;
	struct br_ip_list *br_ip_entry, *tmp;
=======
static int batadv_mcast_mla_bridge_get(struct net_device *dev,
				       struct hlist_head *mcast_list,
				       struct batadv_mcast_mla_flags *flags)
{
	struct list_head bridge_mcast_list = LIST_HEAD_INIT(bridge_mcast_list);
	struct br_ip_list *br_ip_entry, *tmp;
	u8 tvlv_flags = flags->tvlv_flags;
>>>>>>> upstream/android-13
	struct batadv_hw_addr *new;
	u8 mcast_addr[ETH_ALEN];
	int ret;

	/* we don't need to detect these devices/listeners, the IGMP/MLD
	 * snooping code of the Linux bridge already does that for us
	 */
	ret = br_multicast_list_adjacent(dev, &bridge_mcast_list);
	if (ret < 0)
		goto out;

	list_for_each_entry(br_ip_entry, &bridge_mcast_list, list) {
<<<<<<< HEAD
		if (all_ipv4 && br_ip_entry->addr.proto == htons(ETH_P_IP))
			continue;

		if (all_ipv6 && br_ip_entry->addr.proto == htons(ETH_P_IPV6))
			continue;
=======
		if (br_ip_entry->addr.proto == htons(ETH_P_IP)) {
			if (tvlv_flags & BATADV_MCAST_WANT_ALL_IPV4)
				continue;

			if (tvlv_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES &&
			    ipv4_is_local_multicast(br_ip_entry->addr.dst.ip4))
				continue;

			if (!(tvlv_flags & BATADV_MCAST_WANT_NO_RTR4) &&
			    !ipv4_is_local_multicast(br_ip_entry->addr.dst.ip4))
				continue;
		}

#if IS_ENABLED(CONFIG_IPV6)
		if (br_ip_entry->addr.proto == htons(ETH_P_IPV6)) {
			if (tvlv_flags & BATADV_MCAST_WANT_ALL_IPV6)
				continue;

			if (tvlv_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES &&
			    ipv6_addr_is_ll_all_nodes(&br_ip_entry->addr.dst.ip6))
				continue;

			if (!(tvlv_flags & BATADV_MCAST_WANT_NO_RTR6) &&
			    IPV6_ADDR_MC_SCOPE(&br_ip_entry->addr.dst.ip6) >
			    IPV6_ADDR_SCOPE_LINKLOCAL)
				continue;
		}
#endif
>>>>>>> upstream/android-13

		batadv_mcast_mla_br_addr_cpy(mcast_addr, &br_ip_entry->addr);
		if (batadv_mcast_mla_is_duplicate(mcast_addr, mcast_list))
			continue;

		new = kmalloc(sizeof(*new), GFP_ATOMIC);
		if (!new) {
			ret = -ENOMEM;
			break;
		}

		ether_addr_copy(new->addr, mcast_addr);
		hlist_add_head(&new->list, mcast_list);
	}

out:
	list_for_each_entry_safe(br_ip_entry, tmp, &bridge_mcast_list, list) {
		list_del(&br_ip_entry->list);
		kfree(br_ip_entry);
	}

	return ret;
}

/**
 * batadv_mcast_mla_list_free() - free a list of multicast addresses
 * @mcast_list: the list to free
 *
 * Removes and frees all items in the given mcast_list.
 */
static void batadv_mcast_mla_list_free(struct hlist_head *mcast_list)
{
	struct batadv_hw_addr *mcast_entry;
	struct hlist_node *tmp;

	hlist_for_each_entry_safe(mcast_entry, tmp, mcast_list, list) {
		hlist_del(&mcast_entry->list);
		kfree(mcast_entry);
	}
}

/**
 * batadv_mcast_mla_tt_retract() - clean up multicast listener announcements
 * @bat_priv: the bat priv with all the soft interface information
 * @mcast_list: a list of addresses which should _not_ be removed
 *
 * Retracts the announcement of any multicast listener from the
 * translation table except the ones listed in the given mcast_list.
 *
 * If mcast_list is NULL then all are retracted.
 */
static void batadv_mcast_mla_tt_retract(struct batadv_priv *bat_priv,
					struct hlist_head *mcast_list)
{
	struct batadv_hw_addr *mcast_entry;
	struct hlist_node *tmp;

	hlist_for_each_entry_safe(mcast_entry, tmp, &bat_priv->mcast.mla_list,
				  list) {
		if (mcast_list &&
		    batadv_mcast_mla_is_duplicate(mcast_entry->addr,
						  mcast_list))
			continue;

		batadv_tt_local_remove(bat_priv, mcast_entry->addr,
				       BATADV_NO_FLAGS,
				       "mcast TT outdated", false);

		hlist_del(&mcast_entry->list);
		kfree(mcast_entry);
	}
}

/**
 * batadv_mcast_mla_tt_add() - add multicast listener announcements
 * @bat_priv: the bat priv with all the soft interface information
 * @mcast_list: a list of addresses which are going to get added
 *
 * Adds multicast listener announcements from the given mcast_list to the
 * translation table if they have not been added yet.
 */
static void batadv_mcast_mla_tt_add(struct batadv_priv *bat_priv,
				    struct hlist_head *mcast_list)
{
	struct batadv_hw_addr *mcast_entry;
	struct hlist_node *tmp;

	if (!mcast_list)
		return;

	hlist_for_each_entry_safe(mcast_entry, tmp, mcast_list, list) {
		if (batadv_mcast_mla_is_duplicate(mcast_entry->addr,
						  &bat_priv->mcast.mla_list))
			continue;

		if (!batadv_tt_local_add(bat_priv->soft_iface,
					 mcast_entry->addr, BATADV_NO_FLAGS,
					 BATADV_NULL_IFINDEX, BATADV_NO_MARK))
			continue;

		hlist_del(&mcast_entry->list);
		hlist_add_head(&mcast_entry->list, &bat_priv->mcast.mla_list);
	}
}

/**
<<<<<<< HEAD
 * batadv_mcast_has_bridge() - check whether the soft-iface is bridged
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Checks whether there is a bridge on top of our soft interface.
 *
 * Return: true if there is a bridge, false otherwise.
 */
static bool batadv_mcast_has_bridge(struct batadv_priv *bat_priv)
{
	struct net_device *upper = bat_priv->soft_iface;

	rcu_read_lock();
	do {
		upper = netdev_master_upper_dev_get_rcu(upper);
	} while (upper && !(upper->priv_flags & IFF_EBRIDGE));
	rcu_read_unlock();

	return upper;
}

/**
=======
>>>>>>> upstream/android-13
 * batadv_mcast_querier_log() - debug output regarding the querier status on
 *  link
 * @bat_priv: the bat priv with all the soft interface information
 * @str_proto: a string for the querier protocol (e.g. "IGMP" or "MLD")
 * @old_state: the previous querier state on our link
 * @new_state: the new querier state on our link
 *
 * Outputs debug messages to the logging facility with log level 'mcast'
 * regarding changes to the querier status on the link which are relevant
 * to our multicast optimizations.
 *
 * Usually this is about whether a querier appeared or vanished in
 * our mesh or whether the querier is in the suboptimal position of being
 * behind our local bridge segment: Snooping switches will directly
 * forward listener reports to the querier, therefore batman-adv and
 * the bridge will potentially not see these listeners - the querier is
 * potentially shadowing listeners from us then.
 *
 * This is only interesting for nodes with a bridge on top of their
 * soft interface.
 */
static void
batadv_mcast_querier_log(struct batadv_priv *bat_priv, char *str_proto,
			 struct batadv_mcast_querier_state *old_state,
			 struct batadv_mcast_querier_state *new_state)
{
	if (!old_state->exists && new_state->exists)
		batadv_info(bat_priv->soft_iface, "%s Querier appeared\n",
			    str_proto);
	else if (old_state->exists && !new_state->exists)
		batadv_info(bat_priv->soft_iface,
			    "%s Querier disappeared - multicast optimizations disabled\n",
			    str_proto);
<<<<<<< HEAD
	else if (!bat_priv->mcast.bridged && !new_state->exists)
=======
	else if (!bat_priv->mcast.mla_flags.bridged && !new_state->exists)
>>>>>>> upstream/android-13
		batadv_info(bat_priv->soft_iface,
			    "No %s Querier present - multicast optimizations disabled\n",
			    str_proto);

	if (new_state->exists) {
		if ((!old_state->shadowing && new_state->shadowing) ||
		    (!old_state->exists && new_state->shadowing))
			batadv_dbg(BATADV_DBG_MCAST, bat_priv,
				   "%s Querier is behind our bridged segment: Might shadow listeners\n",
				   str_proto);
		else if (old_state->shadowing && !new_state->shadowing)
			batadv_dbg(BATADV_DBG_MCAST, bat_priv,
				   "%s Querier is not behind our bridged segment\n",
				   str_proto);
	}
}

/**
 * batadv_mcast_bridge_log() - debug output for topology changes in bridged
 *  setups
 * @bat_priv: the bat priv with all the soft interface information
<<<<<<< HEAD
 * @bridged: a flag about whether the soft interface is currently bridged or not
 * @querier_ipv4: (maybe) new status of a potential, selected IGMP querier
 * @querier_ipv6: (maybe) new status of a potential, selected MLD querier
=======
 * @new_flags: flags indicating the new multicast state
>>>>>>> upstream/android-13
 *
 * If no bridges are ever used on this node, then this function does nothing.
 *
 * Otherwise this function outputs debug information to the 'mcast' log level
 * which might be relevant to our multicast optimizations.
 *
 * More precisely, it outputs information when a bridge interface is added or
 * removed from a soft interface. And when a bridge is present, it further
 * outputs information about the querier state which is relevant for the
 * multicast flags this node is going to set.
 */
static void
<<<<<<< HEAD
batadv_mcast_bridge_log(struct batadv_priv *bat_priv, bool bridged,
			struct batadv_mcast_querier_state *querier_ipv4,
			struct batadv_mcast_querier_state *querier_ipv6)
{
	if (!bat_priv->mcast.bridged && bridged)
		batadv_dbg(BATADV_DBG_MCAST, bat_priv,
			   "Bridge added: Setting Unsnoopables(U)-flag\n");
	else if (bat_priv->mcast.bridged && !bridged)
		batadv_dbg(BATADV_DBG_MCAST, bat_priv,
			   "Bridge removed: Unsetting Unsnoopables(U)-flag\n");

	if (bridged) {
		batadv_mcast_querier_log(bat_priv, "IGMP",
					 &bat_priv->mcast.querier_ipv4,
					 querier_ipv4);
		batadv_mcast_querier_log(bat_priv, "MLD",
					 &bat_priv->mcast.querier_ipv6,
					 querier_ipv6);
=======
batadv_mcast_bridge_log(struct batadv_priv *bat_priv,
			struct batadv_mcast_mla_flags *new_flags)
{
	struct batadv_mcast_mla_flags *old_flags = &bat_priv->mcast.mla_flags;

	if (!old_flags->bridged && new_flags->bridged)
		batadv_dbg(BATADV_DBG_MCAST, bat_priv,
			   "Bridge added: Setting Unsnoopables(U)-flag\n");
	else if (old_flags->bridged && !new_flags->bridged)
		batadv_dbg(BATADV_DBG_MCAST, bat_priv,
			   "Bridge removed: Unsetting Unsnoopables(U)-flag\n");

	if (new_flags->bridged) {
		batadv_mcast_querier_log(bat_priv, "IGMP",
					 &old_flags->querier_ipv4,
					 &new_flags->querier_ipv4);
		batadv_mcast_querier_log(bat_priv, "MLD",
					 &old_flags->querier_ipv6,
					 &new_flags->querier_ipv6);
>>>>>>> upstream/android-13
	}
}

/**
<<<<<<< HEAD
 * batadv_mcast_flags_logs() - output debug information about mcast flag changes
 * @bat_priv: the bat priv with all the soft interface information
 * @flags: flags indicating the new multicast state
 *
 * Whenever the multicast flags this nodes announces changes (@mcast_flags vs.
 * bat_priv->mcast.flags), this notifies userspace via the 'mcast' log level.
 */
static void batadv_mcast_flags_log(struct batadv_priv *bat_priv, u8 flags)
{
	u8 old_flags = bat_priv->mcast.flags;
	char str_old_flags[] = "[...]";

	sprintf(str_old_flags, "[%c%c%c]",
		(old_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES) ? 'U' : '.',
		(old_flags & BATADV_MCAST_WANT_ALL_IPV4) ? '4' : '.',
		(old_flags & BATADV_MCAST_WANT_ALL_IPV6) ? '6' : '.');

	batadv_dbg(BATADV_DBG_MCAST, bat_priv,
		   "Changing multicast flags from '%s' to '[%c%c%c]'\n",
		   bat_priv->mcast.enabled ? str_old_flags : "<undefined>",
		   (flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES) ? 'U' : '.',
		   (flags & BATADV_MCAST_WANT_ALL_IPV4) ? '4' : '.',
		   (flags & BATADV_MCAST_WANT_ALL_IPV6) ? '6' : '.');
}

/**
 * batadv_mcast_mla_tvlv_update() - update multicast tvlv
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Updates the own multicast tvlv with our current multicast related settings,
 * capabilities and inabilities.
 *
 * Return: false if we want all IPv4 && IPv6 multicast traffic and true
 * otherwise.
 */
static bool batadv_mcast_mla_tvlv_update(struct batadv_priv *bat_priv)
{
	struct batadv_tvlv_mcast_data mcast_data;
	struct batadv_mcast_querier_state querier4 = {false, false};
	struct batadv_mcast_querier_state querier6 = {false, false};
	struct net_device *dev = bat_priv->soft_iface;
	bool bridged;

	mcast_data.flags = BATADV_NO_FLAGS;
	memset(mcast_data.reserved, 0, sizeof(mcast_data.reserved));

	bridged = batadv_mcast_has_bridge(bat_priv);
	if (!bridged)
		goto update;

	if (!IS_ENABLED(CONFIG_BRIDGE_IGMP_SNOOPING))
		pr_warn_once("No bridge IGMP snooping compiled - multicast optimizations disabled\n");

	querier4.exists = br_multicast_has_querier_anywhere(dev, ETH_P_IP);
	querier4.shadowing = br_multicast_has_querier_adjacent(dev, ETH_P_IP);

	querier6.exists = br_multicast_has_querier_anywhere(dev, ETH_P_IPV6);
	querier6.shadowing = br_multicast_has_querier_adjacent(dev, ETH_P_IPV6);

	mcast_data.flags |= BATADV_MCAST_WANT_ALL_UNSNOOPABLES;

	/* 1) If no querier exists at all, then multicast listeners on
	 *    our local TT clients behind the bridge will keep silent.
	 * 2) If the selected querier is on one of our local TT clients,
	 *    behind the bridge, then this querier might shadow multicast
	 *    listeners on our local TT clients, behind this bridge.
	 *
	 * In both cases, we will signalize other batman nodes that
	 * we need all multicast traffic of the according protocol.
	 */
	if (!querier4.exists || querier4.shadowing)
		mcast_data.flags |= BATADV_MCAST_WANT_ALL_IPV4;

	if (!querier6.exists || querier6.shadowing)
		mcast_data.flags |= BATADV_MCAST_WANT_ALL_IPV6;

update:
	batadv_mcast_bridge_log(bat_priv, bridged, &querier4, &querier6);

	bat_priv->mcast.querier_ipv4.exists = querier4.exists;
	bat_priv->mcast.querier_ipv4.shadowing = querier4.shadowing;

	bat_priv->mcast.querier_ipv6.exists = querier6.exists;
	bat_priv->mcast.querier_ipv6.shadowing = querier6.shadowing;

	bat_priv->mcast.bridged = bridged;

	if (!bat_priv->mcast.enabled ||
	    mcast_data.flags != bat_priv->mcast.flags) {
		batadv_mcast_flags_log(bat_priv, mcast_data.flags);
		batadv_tvlv_container_register(bat_priv, BATADV_TVLV_MCAST, 2,
					       &mcast_data, sizeof(mcast_data));
		bat_priv->mcast.flags = mcast_data.flags;
		bat_priv->mcast.enabled = true;
	}

	return !(mcast_data.flags & BATADV_MCAST_WANT_ALL_IPV4 &&
		 mcast_data.flags & BATADV_MCAST_WANT_ALL_IPV6);
=======
 * batadv_mcast_flags_log() - output debug information about mcast flag changes
 * @bat_priv: the bat priv with all the soft interface information
 * @flags: TVLV flags indicating the new multicast state
 *
 * Whenever the multicast TVLV flags this node announces change, this function
 * should be used to notify userspace about the change.
 */
static void batadv_mcast_flags_log(struct batadv_priv *bat_priv, u8 flags)
{
	bool old_enabled = bat_priv->mcast.mla_flags.enabled;
	u8 old_flags = bat_priv->mcast.mla_flags.tvlv_flags;
	char str_old_flags[] = "[.... . ]";

	sprintf(str_old_flags, "[%c%c%c%s%s]",
		(old_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES) ? 'U' : '.',
		(old_flags & BATADV_MCAST_WANT_ALL_IPV4) ? '4' : '.',
		(old_flags & BATADV_MCAST_WANT_ALL_IPV6) ? '6' : '.',
		!(old_flags & BATADV_MCAST_WANT_NO_RTR4) ? "R4" : ". ",
		!(old_flags & BATADV_MCAST_WANT_NO_RTR6) ? "R6" : ". ");

	batadv_dbg(BATADV_DBG_MCAST, bat_priv,
		   "Changing multicast flags from '%s' to '[%c%c%c%s%s]'\n",
		   old_enabled ? str_old_flags : "<undefined>",
		   (flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES) ? 'U' : '.',
		   (flags & BATADV_MCAST_WANT_ALL_IPV4) ? '4' : '.',
		   (flags & BATADV_MCAST_WANT_ALL_IPV6) ? '6' : '.',
		   !(flags & BATADV_MCAST_WANT_NO_RTR4) ? "R4" : ". ",
		   !(flags & BATADV_MCAST_WANT_NO_RTR6) ? "R6" : ". ");
}

/**
 * batadv_mcast_mla_flags_update() - update multicast flags
 * @bat_priv: the bat priv with all the soft interface information
 * @flags: flags indicating the new multicast state
 *
 * Updates the own multicast tvlv with our current multicast related settings,
 * capabilities and inabilities.
 */
static void
batadv_mcast_mla_flags_update(struct batadv_priv *bat_priv,
			      struct batadv_mcast_mla_flags *flags)
{
	struct batadv_tvlv_mcast_data mcast_data;

	if (!memcmp(flags, &bat_priv->mcast.mla_flags, sizeof(*flags)))
		return;

	batadv_mcast_bridge_log(bat_priv, flags);
	batadv_mcast_flags_log(bat_priv, flags->tvlv_flags);

	mcast_data.flags = flags->tvlv_flags;
	memset(mcast_data.reserved, 0, sizeof(mcast_data.reserved));

	batadv_tvlv_container_register(bat_priv, BATADV_TVLV_MCAST, 2,
				       &mcast_data, sizeof(mcast_data));

	bat_priv->mcast.mla_flags = *flags;
>>>>>>> upstream/android-13
}

/**
 * __batadv_mcast_mla_update() - update the own MLAs
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Updates the own multicast listener announcements in the translation
 * table as well as the own, announced multicast tvlv container.
 *
 * Note that non-conflicting reads and writes to bat_priv->mcast.mla_list
 * in batadv_mcast_mla_tt_retract() and batadv_mcast_mla_tt_add() are
 * ensured by the non-parallel execution of the worker this function
 * belongs to.
 */
static void __batadv_mcast_mla_update(struct batadv_priv *bat_priv)
{
	struct net_device *soft_iface = bat_priv->soft_iface;
	struct hlist_head mcast_list = HLIST_HEAD_INIT;
<<<<<<< HEAD
	int ret;

	if (!batadv_mcast_mla_tvlv_update(bat_priv))
		goto update;

	ret = batadv_mcast_mla_softif_get(bat_priv, soft_iface, &mcast_list);
	if (ret < 0)
		goto out;

	ret = batadv_mcast_mla_bridge_get(bat_priv, soft_iface, &mcast_list);
	if (ret < 0)
		goto out;

update:
	batadv_mcast_mla_tt_retract(bat_priv, &mcast_list);
	batadv_mcast_mla_tt_add(bat_priv, &mcast_list);
=======
	struct batadv_mcast_mla_flags flags;
	int ret;

	flags = batadv_mcast_mla_flags_get(bat_priv);

	ret = batadv_mcast_mla_softif_get(soft_iface, &mcast_list, &flags);
	if (ret < 0)
		goto out;

	ret = batadv_mcast_mla_bridge_get(soft_iface, &mcast_list, &flags);
	if (ret < 0)
		goto out;

	spin_lock(&bat_priv->mcast.mla_lock);
	batadv_mcast_mla_tt_retract(bat_priv, &mcast_list);
	batadv_mcast_mla_tt_add(bat_priv, &mcast_list);
	batadv_mcast_mla_flags_update(bat_priv, &flags);
	spin_unlock(&bat_priv->mcast.mla_lock);
>>>>>>> upstream/android-13

out:
	batadv_mcast_mla_list_free(&mcast_list);
}

/**
 * batadv_mcast_mla_update() - update the own MLAs
 * @work: kernel work struct
 *
 * Updates the own multicast listener announcements in the translation
 * table as well as the own, announced multicast tvlv container.
 *
 * In the end, reschedules the work timer.
 */
static void batadv_mcast_mla_update(struct work_struct *work)
{
	struct delayed_work *delayed_work;
	struct batadv_priv_mcast *priv_mcast;
	struct batadv_priv *bat_priv;

	delayed_work = to_delayed_work(work);
	priv_mcast = container_of(delayed_work, struct batadv_priv_mcast, work);
	bat_priv = container_of(priv_mcast, struct batadv_priv, mcast);

<<<<<<< HEAD
	spin_lock(&bat_priv->mcast.mla_lock);
	__batadv_mcast_mla_update(bat_priv);
	spin_unlock(&bat_priv->mcast.mla_lock);

=======
	__batadv_mcast_mla_update(bat_priv);
>>>>>>> upstream/android-13
	batadv_mcast_start_timer(bat_priv);
}

/**
 * batadv_mcast_is_report_ipv4() - check for IGMP reports
 * @skb: the ethernet frame destined for the mesh
 *
 * This call might reallocate skb data.
 *
 * Checks whether the given frame is a valid IGMP report.
 *
 * Return: If so then true, otherwise false.
 */
static bool batadv_mcast_is_report_ipv4(struct sk_buff *skb)
{
<<<<<<< HEAD
	if (ip_mc_check_igmp(skb, NULL) < 0)
=======
	if (ip_mc_check_igmp(skb) < 0)
>>>>>>> upstream/android-13
		return false;

	switch (igmp_hdr(skb)->type) {
	case IGMP_HOST_MEMBERSHIP_REPORT:
	case IGMPV2_HOST_MEMBERSHIP_REPORT:
	case IGMPV3_HOST_MEMBERSHIP_REPORT:
		return true;
	}

	return false;
}

/**
 * batadv_mcast_forw_mode_check_ipv4() - check for optimized forwarding
 *  potential
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the IPv4 packet to check
 * @is_unsnoopable: stores whether the destination is snoopable
<<<<<<< HEAD
=======
 * @is_routable: stores whether the destination is routable
>>>>>>> upstream/android-13
 *
 * Checks whether the given IPv4 packet has the potential to be forwarded with a
 * mode more optimal than classic flooding.
 *
 * Return: If so then 0. Otherwise -EINVAL or -ENOMEM in case of memory
 * allocation failure.
 */
static int batadv_mcast_forw_mode_check_ipv4(struct batadv_priv *bat_priv,
					     struct sk_buff *skb,
<<<<<<< HEAD
					     bool *is_unsnoopable)
=======
					     bool *is_unsnoopable,
					     int *is_routable)
>>>>>>> upstream/android-13
{
	struct iphdr *iphdr;

	/* We might fail due to out-of-memory -> drop it */
	if (!pskb_may_pull(skb, sizeof(struct ethhdr) + sizeof(*iphdr)))
		return -ENOMEM;

	if (batadv_mcast_is_report_ipv4(skb))
		return -EINVAL;

	iphdr = ip_hdr(skb);

<<<<<<< HEAD
	/* TODO: Implement Multicast Router Discovery (RFC4286),
	 * then allow scope > link local, too
	 */
	if (!ipv4_is_local_multicast(iphdr->daddr))
		return -EINVAL;

	/* link-local multicast listeners behind a bridge are
	 * not snoopable (see RFC4541, section 2.1.2.2)
	 */
	*is_unsnoopable = true;
=======
	/* link-local multicast listeners behind a bridge are
	 * not snoopable (see RFC4541, section 2.1.2.2)
	 */
	if (ipv4_is_local_multicast(iphdr->daddr))
		*is_unsnoopable = true;
	else
		*is_routable = ETH_P_IP;
>>>>>>> upstream/android-13

	return 0;
}

/**
 * batadv_mcast_is_report_ipv6() - check for MLD reports
 * @skb: the ethernet frame destined for the mesh
 *
 * This call might reallocate skb data.
 *
 * Checks whether the given frame is a valid MLD report.
 *
 * Return: If so then true, otherwise false.
 */
static bool batadv_mcast_is_report_ipv6(struct sk_buff *skb)
{
<<<<<<< HEAD
	if (ipv6_mc_check_mld(skb, NULL) < 0)
=======
	if (ipv6_mc_check_mld(skb) < 0)
>>>>>>> upstream/android-13
		return false;

	switch (icmp6_hdr(skb)->icmp6_type) {
	case ICMPV6_MGM_REPORT:
	case ICMPV6_MLD2_REPORT:
		return true;
	}

	return false;
}

/**
 * batadv_mcast_forw_mode_check_ipv6() - check for optimized forwarding
 *  potential
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the IPv6 packet to check
 * @is_unsnoopable: stores whether the destination is snoopable
<<<<<<< HEAD
=======
 * @is_routable: stores whether the destination is routable
>>>>>>> upstream/android-13
 *
 * Checks whether the given IPv6 packet has the potential to be forwarded with a
 * mode more optimal than classic flooding.
 *
 * Return: If so then 0. Otherwise -EINVAL is or -ENOMEM if we are out of memory
 */
static int batadv_mcast_forw_mode_check_ipv6(struct batadv_priv *bat_priv,
					     struct sk_buff *skb,
<<<<<<< HEAD
					     bool *is_unsnoopable)
=======
					     bool *is_unsnoopable,
					     int *is_routable)
>>>>>>> upstream/android-13
{
	struct ipv6hdr *ip6hdr;

	/* We might fail due to out-of-memory -> drop it */
	if (!pskb_may_pull(skb, sizeof(struct ethhdr) + sizeof(*ip6hdr)))
		return -ENOMEM;

	if (batadv_mcast_is_report_ipv6(skb))
		return -EINVAL;

	ip6hdr = ipv6_hdr(skb);

<<<<<<< HEAD
	/* TODO: Implement Multicast Router Discovery (RFC4286),
	 * then allow scope > link local, too
	 */
	if (IPV6_ADDR_MC_SCOPE(&ip6hdr->daddr) != IPV6_ADDR_SCOPE_LINKLOCAL)
=======
	if (IPV6_ADDR_MC_SCOPE(&ip6hdr->daddr) < IPV6_ADDR_SCOPE_LINKLOCAL)
>>>>>>> upstream/android-13
		return -EINVAL;

	/* link-local-all-nodes multicast listeners behind a bridge are
	 * not snoopable (see RFC4541, section 3, paragraph 3)
	 */
	if (ipv6_addr_is_ll_all_nodes(&ip6hdr->daddr))
		*is_unsnoopable = true;
<<<<<<< HEAD
=======
	else if (IPV6_ADDR_MC_SCOPE(&ip6hdr->daddr) > IPV6_ADDR_SCOPE_LINKLOCAL)
		*is_routable = ETH_P_IPV6;
>>>>>>> upstream/android-13

	return 0;
}

/**
 * batadv_mcast_forw_mode_check() - check for optimized forwarding potential
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast frame to check
 * @is_unsnoopable: stores whether the destination is snoopable
<<<<<<< HEAD
=======
 * @is_routable: stores whether the destination is routable
>>>>>>> upstream/android-13
 *
 * Checks whether the given multicast ethernet frame has the potential to be
 * forwarded with a mode more optimal than classic flooding.
 *
 * Return: If so then 0. Otherwise -EINVAL is or -ENOMEM if we are out of memory
 */
static int batadv_mcast_forw_mode_check(struct batadv_priv *bat_priv,
					struct sk_buff *skb,
<<<<<<< HEAD
					bool *is_unsnoopable)
=======
					bool *is_unsnoopable,
					int *is_routable)
>>>>>>> upstream/android-13
{
	struct ethhdr *ethhdr = eth_hdr(skb);

	if (!atomic_read(&bat_priv->multicast_mode))
		return -EINVAL;

	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_IP:
		return batadv_mcast_forw_mode_check_ipv4(bat_priv, skb,
<<<<<<< HEAD
							 is_unsnoopable);
=======
							 is_unsnoopable,
							 is_routable);
>>>>>>> upstream/android-13
	case ETH_P_IPV6:
		if (!IS_ENABLED(CONFIG_IPV6))
			return -EINVAL;

		return batadv_mcast_forw_mode_check_ipv6(bat_priv, skb,
<<<<<<< HEAD
							 is_unsnoopable);
=======
							 is_unsnoopable,
							 is_routable);
>>>>>>> upstream/android-13
	default:
		return -EINVAL;
	}
}

/**
 * batadv_mcast_forw_want_all_ip_count() - count nodes with unspecific mcast
 *  interest
 * @bat_priv: the bat priv with all the soft interface information
 * @ethhdr: ethernet header of a packet
 *
 * Return: the number of nodes which want all IPv4 multicast traffic if the
 * given ethhdr is from an IPv4 packet or the number of nodes which want all
 * IPv6 traffic if it matches an IPv6 packet.
 */
static int batadv_mcast_forw_want_all_ip_count(struct batadv_priv *bat_priv,
					       struct ethhdr *ethhdr)
{
	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_IP:
		return atomic_read(&bat_priv->mcast.num_want_all_ipv4);
	case ETH_P_IPV6:
		return atomic_read(&bat_priv->mcast.num_want_all_ipv6);
	default:
		/* we shouldn't be here... */
		return 0;
	}
}

/**
<<<<<<< HEAD
=======
 * batadv_mcast_forw_rtr_count() - count nodes with a multicast router
 * @bat_priv: the bat priv with all the soft interface information
 * @protocol: the ethernet protocol type to count multicast routers for
 *
 * Return: the number of nodes which want all routable IPv4 multicast traffic
 * if the protocol is ETH_P_IP or the number of nodes which want all routable
 * IPv6 traffic if the protocol is ETH_P_IPV6. Otherwise returns 0.
 */

static int batadv_mcast_forw_rtr_count(struct batadv_priv *bat_priv,
				       int protocol)
{
	switch (protocol) {
	case ETH_P_IP:
		return atomic_read(&bat_priv->mcast.num_want_all_rtr4);
	case ETH_P_IPV6:
		return atomic_read(&bat_priv->mcast.num_want_all_rtr6);
	default:
		return 0;
	}
}

/**
>>>>>>> upstream/android-13
 * batadv_mcast_forw_tt_node_get() - get a multicast tt node
 * @bat_priv: the bat priv with all the soft interface information
 * @ethhdr: the ether header containing the multicast destination
 *
 * Return: an orig_node matching the multicast address provided by ethhdr
 * via a translation table lookup. This increases the returned nodes refcount.
 */
static struct batadv_orig_node *
batadv_mcast_forw_tt_node_get(struct batadv_priv *bat_priv,
			      struct ethhdr *ethhdr)
{
	return batadv_transtable_search(bat_priv, NULL, ethhdr->h_dest,
					BATADV_NO_FLAGS);
}

/**
 * batadv_mcast_forw_ipv4_node_get() - get a node with an ipv4 flag
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: an orig_node which has the BATADV_MCAST_WANT_ALL_IPV4 flag set and
 * increases its refcount.
 */
static struct batadv_orig_node *
batadv_mcast_forw_ipv4_node_get(struct batadv_priv *bat_priv)
{
	struct batadv_orig_node *tmp_orig_node, *orig_node = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(tmp_orig_node,
				 &bat_priv->mcast.want_all_ipv4_list,
				 mcast_want_all_ipv4_node) {
		if (!kref_get_unless_zero(&tmp_orig_node->refcount))
			continue;

		orig_node = tmp_orig_node;
		break;
	}
	rcu_read_unlock();

	return orig_node;
}

/**
 * batadv_mcast_forw_ipv6_node_get() - get a node with an ipv6 flag
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: an orig_node which has the BATADV_MCAST_WANT_ALL_IPV6 flag set
 * and increases its refcount.
 */
static struct batadv_orig_node *
batadv_mcast_forw_ipv6_node_get(struct batadv_priv *bat_priv)
{
	struct batadv_orig_node *tmp_orig_node, *orig_node = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(tmp_orig_node,
				 &bat_priv->mcast.want_all_ipv6_list,
				 mcast_want_all_ipv6_node) {
		if (!kref_get_unless_zero(&tmp_orig_node->refcount))
			continue;

		orig_node = tmp_orig_node;
		break;
	}
	rcu_read_unlock();

	return orig_node;
}

/**
 * batadv_mcast_forw_ip_node_get() - get a node with an ipv4/ipv6 flag
 * @bat_priv: the bat priv with all the soft interface information
 * @ethhdr: an ethernet header to determine the protocol family from
 *
 * Return: an orig_node which has the BATADV_MCAST_WANT_ALL_IPV4 or
<<<<<<< HEAD
 * BATADV_MCAST_WANT_ALL_IPV6 flag, depending on the provided ethhdr, set and
=======
 * BATADV_MCAST_WANT_ALL_IPV6 flag, depending on the provided ethhdr, sets and
>>>>>>> upstream/android-13
 * increases its refcount.
 */
static struct batadv_orig_node *
batadv_mcast_forw_ip_node_get(struct batadv_priv *bat_priv,
			      struct ethhdr *ethhdr)
{
	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_IP:
		return batadv_mcast_forw_ipv4_node_get(bat_priv);
	case ETH_P_IPV6:
		return batadv_mcast_forw_ipv6_node_get(bat_priv);
	default:
		/* we shouldn't be here... */
		return NULL;
	}
}

/**
 * batadv_mcast_forw_unsnoop_node_get() - get a node with an unsnoopable flag
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: an orig_node which has the BATADV_MCAST_WANT_ALL_UNSNOOPABLES flag
 * set and increases its refcount.
 */
static struct batadv_orig_node *
batadv_mcast_forw_unsnoop_node_get(struct batadv_priv *bat_priv)
{
	struct batadv_orig_node *tmp_orig_node, *orig_node = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(tmp_orig_node,
				 &bat_priv->mcast.want_all_unsnoopables_list,
				 mcast_want_all_unsnoopables_node) {
		if (!kref_get_unless_zero(&tmp_orig_node->refcount))
			continue;

		orig_node = tmp_orig_node;
		break;
	}
	rcu_read_unlock();

	return orig_node;
}

/**
<<<<<<< HEAD
=======
 * batadv_mcast_forw_rtr4_node_get() - get a node with an ipv4 mcast router flag
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: an orig_node which has the BATADV_MCAST_WANT_NO_RTR4 flag unset and
 * increases its refcount.
 */
static struct batadv_orig_node *
batadv_mcast_forw_rtr4_node_get(struct batadv_priv *bat_priv)
{
	struct batadv_orig_node *tmp_orig_node, *orig_node = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(tmp_orig_node,
				 &bat_priv->mcast.want_all_rtr4_list,
				 mcast_want_all_rtr4_node) {
		if (!kref_get_unless_zero(&tmp_orig_node->refcount))
			continue;

		orig_node = tmp_orig_node;
		break;
	}
	rcu_read_unlock();

	return orig_node;
}

/**
 * batadv_mcast_forw_rtr6_node_get() - get a node with an ipv6 mcast router flag
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: an orig_node which has the BATADV_MCAST_WANT_NO_RTR6 flag unset
 * and increases its refcount.
 */
static struct batadv_orig_node *
batadv_mcast_forw_rtr6_node_get(struct batadv_priv *bat_priv)
{
	struct batadv_orig_node *tmp_orig_node, *orig_node = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(tmp_orig_node,
				 &bat_priv->mcast.want_all_rtr6_list,
				 mcast_want_all_rtr6_node) {
		if (!kref_get_unless_zero(&tmp_orig_node->refcount))
			continue;

		orig_node = tmp_orig_node;
		break;
	}
	rcu_read_unlock();

	return orig_node;
}

/**
 * batadv_mcast_forw_rtr_node_get() - get a node with an ipv4/ipv6 router flag
 * @bat_priv: the bat priv with all the soft interface information
 * @ethhdr: an ethernet header to determine the protocol family from
 *
 * Return: an orig_node which has no BATADV_MCAST_WANT_NO_RTR4 or
 * BATADV_MCAST_WANT_NO_RTR6 flag, depending on the provided ethhdr, set and
 * increases its refcount.
 */
static struct batadv_orig_node *
batadv_mcast_forw_rtr_node_get(struct batadv_priv *bat_priv,
			       struct ethhdr *ethhdr)
{
	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_IP:
		return batadv_mcast_forw_rtr4_node_get(bat_priv);
	case ETH_P_IPV6:
		return batadv_mcast_forw_rtr6_node_get(bat_priv);
	default:
		/* we shouldn't be here... */
		return NULL;
	}
}

/**
>>>>>>> upstream/android-13
 * batadv_mcast_forw_mode() - check on how to forward a multicast packet
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: The multicast packet to check
 * @orig: an originator to be set to forward the skb to
<<<<<<< HEAD
=======
 * @is_routable: stores whether the destination is routable
>>>>>>> upstream/android-13
 *
 * Return: the forwarding mode as enum batadv_forw_mode and in case of
 * BATADV_FORW_SINGLE set the orig to the single originator the skb
 * should be forwarded to.
 */
enum batadv_forw_mode
batadv_mcast_forw_mode(struct batadv_priv *bat_priv, struct sk_buff *skb,
<<<<<<< HEAD
		       struct batadv_orig_node **orig)
{
	int ret, tt_count, ip_count, unsnoop_count, total_count;
	bool is_unsnoopable = false;
	struct ethhdr *ethhdr;

	ret = batadv_mcast_forw_mode_check(bat_priv, skb, &is_unsnoopable);
=======
		       struct batadv_orig_node **orig, int *is_routable)
{
	int ret, tt_count, ip_count, unsnoop_count, total_count;
	bool is_unsnoopable = false;
	unsigned int mcast_fanout;
	struct ethhdr *ethhdr;
	int rtr_count = 0;

	ret = batadv_mcast_forw_mode_check(bat_priv, skb, &is_unsnoopable,
					   is_routable);
>>>>>>> upstream/android-13
	if (ret == -ENOMEM)
		return BATADV_FORW_NONE;
	else if (ret < 0)
		return BATADV_FORW_ALL;

	ethhdr = eth_hdr(skb);

	tt_count = batadv_tt_global_hash_count(bat_priv, ethhdr->h_dest,
					       BATADV_NO_FLAGS);
	ip_count = batadv_mcast_forw_want_all_ip_count(bat_priv, ethhdr);
	unsnoop_count = !is_unsnoopable ? 0 :
			atomic_read(&bat_priv->mcast.num_want_all_unsnoopables);
<<<<<<< HEAD

	total_count = tt_count + ip_count + unsnoop_count;
=======
	rtr_count = batadv_mcast_forw_rtr_count(bat_priv, *is_routable);

	total_count = tt_count + ip_count + unsnoop_count + rtr_count;
>>>>>>> upstream/android-13

	switch (total_count) {
	case 1:
		if (tt_count)
			*orig = batadv_mcast_forw_tt_node_get(bat_priv, ethhdr);
		else if (ip_count)
			*orig = batadv_mcast_forw_ip_node_get(bat_priv, ethhdr);
		else if (unsnoop_count)
			*orig = batadv_mcast_forw_unsnoop_node_get(bat_priv);
<<<<<<< HEAD
=======
		else if (rtr_count)
			*orig = batadv_mcast_forw_rtr_node_get(bat_priv,
							       ethhdr);
>>>>>>> upstream/android-13

		if (*orig)
			return BATADV_FORW_SINGLE;

<<<<<<< HEAD
		/* fall through */
	case 0:
		return BATADV_FORW_NONE;
	default:
		return BATADV_FORW_ALL;
	}
=======
		fallthrough;
	case 0:
		return BATADV_FORW_NONE;
	default:
		mcast_fanout = atomic_read(&bat_priv->multicast_fanout);

		if (!unsnoop_count && total_count <= mcast_fanout)
			return BATADV_FORW_SOME;
	}

	return BATADV_FORW_ALL;
}

/**
 * batadv_mcast_forw_send_orig() - send a multicast packet to an originator
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast packet to send
 * @vid: the vlan identifier
 * @orig_node: the originator to send the packet to
 *
 * Return: NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int batadv_mcast_forw_send_orig(struct batadv_priv *bat_priv,
				struct sk_buff *skb,
				unsigned short vid,
				struct batadv_orig_node *orig_node)
{
	/* Avoid sending multicast-in-unicast packets to other BLA
	 * gateways - they already got the frame from the LAN side
	 * we share with them.
	 * TODO: Refactor to take BLA into account earlier, to avoid
	 * reducing the mcast_fanout count.
	 */
	if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig, vid)) {
		dev_kfree_skb(skb);
		return NET_XMIT_SUCCESS;
	}

	return batadv_send_skb_unicast(bat_priv, skb, BATADV_UNICAST, 0,
				       orig_node, vid);
}

/**
 * batadv_mcast_forw_tt() - forwards a packet to multicast listeners
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast packet to transmit
 * @vid: the vlan identifier
 *
 * Sends copies of a frame with multicast destination to any multicast
 * listener registered in the translation table. A transmission is performed
 * via a batman-adv unicast packet for each such destination node.
 *
 * Return: NET_XMIT_DROP on memory allocation failure, NET_XMIT_SUCCESS
 * otherwise.
 */
static int
batadv_mcast_forw_tt(struct batadv_priv *bat_priv, struct sk_buff *skb,
		     unsigned short vid)
{
	int ret = NET_XMIT_SUCCESS;
	struct sk_buff *newskb;

	struct batadv_tt_orig_list_entry *orig_entry;

	struct batadv_tt_global_entry *tt_global;
	const u8 *addr = eth_hdr(skb)->h_dest;

	tt_global = batadv_tt_global_hash_find(bat_priv, addr, vid);
	if (!tt_global)
		goto out;

	rcu_read_lock();
	hlist_for_each_entry_rcu(orig_entry, &tt_global->orig_list, list) {
		newskb = skb_copy(skb, GFP_ATOMIC);
		if (!newskb) {
			ret = NET_XMIT_DROP;
			break;
		}

		batadv_mcast_forw_send_orig(bat_priv, newskb, vid,
					    orig_entry->orig_node);
	}
	rcu_read_unlock();

	batadv_tt_global_entry_put(tt_global);

out:
	return ret;
}

/**
 * batadv_mcast_forw_want_all_ipv4() - forward to nodes with want-all-ipv4
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast packet to transmit
 * @vid: the vlan identifier
 *
 * Sends copies of a frame with multicast destination to any node with a
 * BATADV_MCAST_WANT_ALL_IPV4 flag set. A transmission is performed via a
 * batman-adv unicast packet for each such destination node.
 *
 * Return: NET_XMIT_DROP on memory allocation failure, NET_XMIT_SUCCESS
 * otherwise.
 */
static int
batadv_mcast_forw_want_all_ipv4(struct batadv_priv *bat_priv,
				struct sk_buff *skb, unsigned short vid)
{
	struct batadv_orig_node *orig_node;
	int ret = NET_XMIT_SUCCESS;
	struct sk_buff *newskb;

	rcu_read_lock();
	hlist_for_each_entry_rcu(orig_node,
				 &bat_priv->mcast.want_all_ipv4_list,
				 mcast_want_all_ipv4_node) {
		newskb = skb_copy(skb, GFP_ATOMIC);
		if (!newskb) {
			ret = NET_XMIT_DROP;
			break;
		}

		batadv_mcast_forw_send_orig(bat_priv, newskb, vid, orig_node);
	}
	rcu_read_unlock();
	return ret;
}

/**
 * batadv_mcast_forw_want_all_ipv6() - forward to nodes with want-all-ipv6
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: The multicast packet to transmit
 * @vid: the vlan identifier
 *
 * Sends copies of a frame with multicast destination to any node with a
 * BATADV_MCAST_WANT_ALL_IPV6 flag set. A transmission is performed via a
 * batman-adv unicast packet for each such destination node.
 *
 * Return: NET_XMIT_DROP on memory allocation failure, NET_XMIT_SUCCESS
 * otherwise.
 */
static int
batadv_mcast_forw_want_all_ipv6(struct batadv_priv *bat_priv,
				struct sk_buff *skb, unsigned short vid)
{
	struct batadv_orig_node *orig_node;
	int ret = NET_XMIT_SUCCESS;
	struct sk_buff *newskb;

	rcu_read_lock();
	hlist_for_each_entry_rcu(orig_node,
				 &bat_priv->mcast.want_all_ipv6_list,
				 mcast_want_all_ipv6_node) {
		newskb = skb_copy(skb, GFP_ATOMIC);
		if (!newskb) {
			ret = NET_XMIT_DROP;
			break;
		}

		batadv_mcast_forw_send_orig(bat_priv, newskb, vid, orig_node);
	}
	rcu_read_unlock();
	return ret;
}

/**
 * batadv_mcast_forw_want_all() - forward packet to nodes in a want-all list
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast packet to transmit
 * @vid: the vlan identifier
 *
 * Sends copies of a frame with multicast destination to any node with a
 * BATADV_MCAST_WANT_ALL_IPV4 or BATADV_MCAST_WANT_ALL_IPV6 flag set. A
 * transmission is performed via a batman-adv unicast packet for each such
 * destination node.
 *
 * Return: NET_XMIT_DROP on memory allocation failure or if the protocol family
 * is neither IPv4 nor IPv6. NET_XMIT_SUCCESS otherwise.
 */
static int
batadv_mcast_forw_want_all(struct batadv_priv *bat_priv,
			   struct sk_buff *skb, unsigned short vid)
{
	switch (ntohs(eth_hdr(skb)->h_proto)) {
	case ETH_P_IP:
		return batadv_mcast_forw_want_all_ipv4(bat_priv, skb, vid);
	case ETH_P_IPV6:
		return batadv_mcast_forw_want_all_ipv6(bat_priv, skb, vid);
	default:
		/* we shouldn't be here... */
		return NET_XMIT_DROP;
	}
}

/**
 * batadv_mcast_forw_want_all_rtr4() - forward to nodes with want-all-rtr4
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast packet to transmit
 * @vid: the vlan identifier
 *
 * Sends copies of a frame with multicast destination to any node with a
 * BATADV_MCAST_WANT_NO_RTR4 flag unset. A transmission is performed via a
 * batman-adv unicast packet for each such destination node.
 *
 * Return: NET_XMIT_DROP on memory allocation failure, NET_XMIT_SUCCESS
 * otherwise.
 */
static int
batadv_mcast_forw_want_all_rtr4(struct batadv_priv *bat_priv,
				struct sk_buff *skb, unsigned short vid)
{
	struct batadv_orig_node *orig_node;
	int ret = NET_XMIT_SUCCESS;
	struct sk_buff *newskb;

	rcu_read_lock();
	hlist_for_each_entry_rcu(orig_node,
				 &bat_priv->mcast.want_all_rtr4_list,
				 mcast_want_all_rtr4_node) {
		newskb = skb_copy(skb, GFP_ATOMIC);
		if (!newskb) {
			ret = NET_XMIT_DROP;
			break;
		}

		batadv_mcast_forw_send_orig(bat_priv, newskb, vid, orig_node);
	}
	rcu_read_unlock();
	return ret;
}

/**
 * batadv_mcast_forw_want_all_rtr6() - forward to nodes with want-all-rtr6
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: The multicast packet to transmit
 * @vid: the vlan identifier
 *
 * Sends copies of a frame with multicast destination to any node with a
 * BATADV_MCAST_WANT_NO_RTR6 flag unset. A transmission is performed via a
 * batman-adv unicast packet for each such destination node.
 *
 * Return: NET_XMIT_DROP on memory allocation failure, NET_XMIT_SUCCESS
 * otherwise.
 */
static int
batadv_mcast_forw_want_all_rtr6(struct batadv_priv *bat_priv,
				struct sk_buff *skb, unsigned short vid)
{
	struct batadv_orig_node *orig_node;
	int ret = NET_XMIT_SUCCESS;
	struct sk_buff *newskb;

	rcu_read_lock();
	hlist_for_each_entry_rcu(orig_node,
				 &bat_priv->mcast.want_all_rtr6_list,
				 mcast_want_all_rtr6_node) {
		newskb = skb_copy(skb, GFP_ATOMIC);
		if (!newskb) {
			ret = NET_XMIT_DROP;
			break;
		}

		batadv_mcast_forw_send_orig(bat_priv, newskb, vid, orig_node);
	}
	rcu_read_unlock();
	return ret;
}

/**
 * batadv_mcast_forw_want_rtr() - forward packet to nodes in a want-all-rtr list
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast packet to transmit
 * @vid: the vlan identifier
 *
 * Sends copies of a frame with multicast destination to any node with a
 * BATADV_MCAST_WANT_NO_RTR4 or BATADV_MCAST_WANT_NO_RTR6 flag unset. A
 * transmission is performed via a batman-adv unicast packet for each such
 * destination node.
 *
 * Return: NET_XMIT_DROP on memory allocation failure or if the protocol family
 * is neither IPv4 nor IPv6. NET_XMIT_SUCCESS otherwise.
 */
static int
batadv_mcast_forw_want_rtr(struct batadv_priv *bat_priv,
			   struct sk_buff *skb, unsigned short vid)
{
	switch (ntohs(eth_hdr(skb)->h_proto)) {
	case ETH_P_IP:
		return batadv_mcast_forw_want_all_rtr4(bat_priv, skb, vid);
	case ETH_P_IPV6:
		return batadv_mcast_forw_want_all_rtr6(bat_priv, skb, vid);
	default:
		/* we shouldn't be here... */
		return NET_XMIT_DROP;
	}
}

/**
 * batadv_mcast_forw_send() - send packet to any detected multicast recipient
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the multicast packet to transmit
 * @vid: the vlan identifier
 * @is_routable: stores whether the destination is routable
 *
 * Sends copies of a frame with multicast destination to any node that signaled
 * interest in it, that is either via the translation table or the according
 * want-all flags. A transmission is performed via a batman-adv unicast packet
 * for each such destination node.
 *
 * The given skb is consumed/freed.
 *
 * Return: NET_XMIT_DROP on memory allocation failure or if the protocol family
 * is neither IPv4 nor IPv6. NET_XMIT_SUCCESS otherwise.
 */
int batadv_mcast_forw_send(struct batadv_priv *bat_priv, struct sk_buff *skb,
			   unsigned short vid, int is_routable)
{
	int ret;

	ret = batadv_mcast_forw_tt(bat_priv, skb, vid);
	if (ret != NET_XMIT_SUCCESS) {
		kfree_skb(skb);
		return ret;
	}

	ret = batadv_mcast_forw_want_all(bat_priv, skb, vid);
	if (ret != NET_XMIT_SUCCESS) {
		kfree_skb(skb);
		return ret;
	}

	if (!is_routable)
		goto skip_mc_router;

	ret = batadv_mcast_forw_want_rtr(bat_priv, skb, vid);
	if (ret != NET_XMIT_SUCCESS) {
		kfree_skb(skb);
		return ret;
	}

skip_mc_router:
	consume_skb(skb);
	return ret;
>>>>>>> upstream/android-13
}

/**
 * batadv_mcast_want_unsnoop_update() - update unsnoop counter and list
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node which multicast state might have changed of
 * @mcast_flags: flags indicating the new multicast state
 *
 * If the BATADV_MCAST_WANT_ALL_UNSNOOPABLES flag of this originator,
<<<<<<< HEAD
 * orig, has toggled then this method updates counter and list accordingly.
=======
 * orig, has toggled then this method updates the counter and the list
 * accordingly.
>>>>>>> upstream/android-13
 *
 * Caller needs to hold orig->mcast_handler_lock.
 */
static void batadv_mcast_want_unsnoop_update(struct batadv_priv *bat_priv,
					     struct batadv_orig_node *orig,
					     u8 mcast_flags)
{
	struct hlist_node *node = &orig->mcast_want_all_unsnoopables_node;
	struct hlist_head *head = &bat_priv->mcast.want_all_unsnoopables_list;

	lockdep_assert_held(&orig->mcast_handler_lock);

	/* switched from flag unset to set */
	if (mcast_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES &&
	    !(orig->mcast_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES)) {
		atomic_inc(&bat_priv->mcast.num_want_all_unsnoopables);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(!hlist_unhashed(node));

		hlist_add_head_rcu(node, head);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	/* switched from flag set to unset */
	} else if (!(mcast_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES) &&
		   orig->mcast_flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES) {
		atomic_dec(&bat_priv->mcast.num_want_all_unsnoopables);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(hlist_unhashed(node));

		hlist_del_init_rcu(node);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	}
}

/**
 * batadv_mcast_want_ipv4_update() - update want-all-ipv4 counter and list
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node which multicast state might have changed of
 * @mcast_flags: flags indicating the new multicast state
 *
 * If the BATADV_MCAST_WANT_ALL_IPV4 flag of this originator, orig, has
<<<<<<< HEAD
 * toggled then this method updates counter and list accordingly.
=======
 * toggled then this method updates the counter and the list accordingly.
>>>>>>> upstream/android-13
 *
 * Caller needs to hold orig->mcast_handler_lock.
 */
static void batadv_mcast_want_ipv4_update(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  u8 mcast_flags)
{
	struct hlist_node *node = &orig->mcast_want_all_ipv4_node;
	struct hlist_head *head = &bat_priv->mcast.want_all_ipv4_list;

	lockdep_assert_held(&orig->mcast_handler_lock);

	/* switched from flag unset to set */
	if (mcast_flags & BATADV_MCAST_WANT_ALL_IPV4 &&
	    !(orig->mcast_flags & BATADV_MCAST_WANT_ALL_IPV4)) {
		atomic_inc(&bat_priv->mcast.num_want_all_ipv4);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(!hlist_unhashed(node));

		hlist_add_head_rcu(node, head);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	/* switched from flag set to unset */
	} else if (!(mcast_flags & BATADV_MCAST_WANT_ALL_IPV4) &&
		   orig->mcast_flags & BATADV_MCAST_WANT_ALL_IPV4) {
		atomic_dec(&bat_priv->mcast.num_want_all_ipv4);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(hlist_unhashed(node));

		hlist_del_init_rcu(node);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	}
}

/**
 * batadv_mcast_want_ipv6_update() - update want-all-ipv6 counter and list
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node which multicast state might have changed of
 * @mcast_flags: flags indicating the new multicast state
 *
 * If the BATADV_MCAST_WANT_ALL_IPV6 flag of this originator, orig, has
<<<<<<< HEAD
 * toggled then this method updates counter and list accordingly.
=======
 * toggled then this method updates the counter and the list accordingly.
>>>>>>> upstream/android-13
 *
 * Caller needs to hold orig->mcast_handler_lock.
 */
static void batadv_mcast_want_ipv6_update(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  u8 mcast_flags)
{
	struct hlist_node *node = &orig->mcast_want_all_ipv6_node;
	struct hlist_head *head = &bat_priv->mcast.want_all_ipv6_list;

	lockdep_assert_held(&orig->mcast_handler_lock);

	/* switched from flag unset to set */
	if (mcast_flags & BATADV_MCAST_WANT_ALL_IPV6 &&
	    !(orig->mcast_flags & BATADV_MCAST_WANT_ALL_IPV6)) {
		atomic_inc(&bat_priv->mcast.num_want_all_ipv6);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(!hlist_unhashed(node));

		hlist_add_head_rcu(node, head);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	/* switched from flag set to unset */
	} else if (!(mcast_flags & BATADV_MCAST_WANT_ALL_IPV6) &&
		   orig->mcast_flags & BATADV_MCAST_WANT_ALL_IPV6) {
		atomic_dec(&bat_priv->mcast.num_want_all_ipv6);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(hlist_unhashed(node));

		hlist_del_init_rcu(node);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	}
}

/**
<<<<<<< HEAD
=======
 * batadv_mcast_want_rtr4_update() - update want-all-rtr4 counter and list
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node which multicast state might have changed of
 * @mcast_flags: flags indicating the new multicast state
 *
 * If the BATADV_MCAST_WANT_NO_RTR4 flag of this originator, orig, has
 * toggled then this method updates the counter and the list accordingly.
 *
 * Caller needs to hold orig->mcast_handler_lock.
 */
static void batadv_mcast_want_rtr4_update(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  u8 mcast_flags)
{
	struct hlist_node *node = &orig->mcast_want_all_rtr4_node;
	struct hlist_head *head = &bat_priv->mcast.want_all_rtr4_list;

	lockdep_assert_held(&orig->mcast_handler_lock);

	/* switched from flag set to unset */
	if (!(mcast_flags & BATADV_MCAST_WANT_NO_RTR4) &&
	    orig->mcast_flags & BATADV_MCAST_WANT_NO_RTR4) {
		atomic_inc(&bat_priv->mcast.num_want_all_rtr4);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(!hlist_unhashed(node));

		hlist_add_head_rcu(node, head);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	/* switched from flag unset to set */
	} else if (mcast_flags & BATADV_MCAST_WANT_NO_RTR4 &&
		   !(orig->mcast_flags & BATADV_MCAST_WANT_NO_RTR4)) {
		atomic_dec(&bat_priv->mcast.num_want_all_rtr4);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(hlist_unhashed(node));

		hlist_del_init_rcu(node);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	}
}

/**
 * batadv_mcast_want_rtr6_update() - update want-all-rtr6 counter and list
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node which multicast state might have changed of
 * @mcast_flags: flags indicating the new multicast state
 *
 * If the BATADV_MCAST_WANT_NO_RTR6 flag of this originator, orig, has
 * toggled then this method updates the counter and the list accordingly.
 *
 * Caller needs to hold orig->mcast_handler_lock.
 */
static void batadv_mcast_want_rtr6_update(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  u8 mcast_flags)
{
	struct hlist_node *node = &orig->mcast_want_all_rtr6_node;
	struct hlist_head *head = &bat_priv->mcast.want_all_rtr6_list;

	lockdep_assert_held(&orig->mcast_handler_lock);

	/* switched from flag set to unset */
	if (!(mcast_flags & BATADV_MCAST_WANT_NO_RTR6) &&
	    orig->mcast_flags & BATADV_MCAST_WANT_NO_RTR6) {
		atomic_inc(&bat_priv->mcast.num_want_all_rtr6);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(!hlist_unhashed(node));

		hlist_add_head_rcu(node, head);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	/* switched from flag unset to set */
	} else if (mcast_flags & BATADV_MCAST_WANT_NO_RTR6 &&
		   !(orig->mcast_flags & BATADV_MCAST_WANT_NO_RTR6)) {
		atomic_dec(&bat_priv->mcast.num_want_all_rtr6);

		spin_lock_bh(&bat_priv->mcast.want_lists_lock);
		/* flag checks above + mcast_handler_lock prevents this */
		WARN_ON(hlist_unhashed(node));

		hlist_del_init_rcu(node);
		spin_unlock_bh(&bat_priv->mcast.want_lists_lock);
	}
}

/**
 * batadv_mcast_tvlv_flags_get() - get multicast flags from an OGM TVLV
 * @enabled: whether the originator has multicast TVLV support enabled
 * @tvlv_value: tvlv buffer containing the multicast flags
 * @tvlv_value_len: tvlv buffer length
 *
 * Return: multicast flags for the given tvlv buffer
 */
static u8
batadv_mcast_tvlv_flags_get(bool enabled, void *tvlv_value, u16 tvlv_value_len)
{
	u8 mcast_flags = BATADV_NO_FLAGS;

	if (enabled && tvlv_value && tvlv_value_len >= sizeof(mcast_flags))
		mcast_flags = *(u8 *)tvlv_value;

	if (!enabled) {
		mcast_flags |= BATADV_MCAST_WANT_ALL_IPV4;
		mcast_flags |= BATADV_MCAST_WANT_ALL_IPV6;
	}

	/* remove redundant flags to avoid sending duplicate packets later */
	if (mcast_flags & BATADV_MCAST_WANT_ALL_IPV4)
		mcast_flags |= BATADV_MCAST_WANT_NO_RTR4;

	if (mcast_flags & BATADV_MCAST_WANT_ALL_IPV6)
		mcast_flags |= BATADV_MCAST_WANT_NO_RTR6;

	return mcast_flags;
}

/**
>>>>>>> upstream/android-13
 * batadv_mcast_tvlv_ogm_handler() - process incoming multicast tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @flags: flags indicating the tvlv state (see batadv_tvlv_handler_flags)
 * @tvlv_value: tvlv buffer containing the multicast data
 * @tvlv_value_len: tvlv buffer length
 */
static void batadv_mcast_tvlv_ogm_handler(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  u8 flags,
					  void *tvlv_value,
					  u16 tvlv_value_len)
{
	bool orig_mcast_enabled = !(flags & BATADV_TVLV_HANDLER_OGM_CIFNOTFND);
<<<<<<< HEAD
	u8 mcast_flags = BATADV_NO_FLAGS;

	if (orig_mcast_enabled && tvlv_value &&
	    tvlv_value_len >= sizeof(mcast_flags))
		mcast_flags = *(u8 *)tvlv_value;

	if (!orig_mcast_enabled) {
		mcast_flags |= BATADV_MCAST_WANT_ALL_IPV4;
		mcast_flags |= BATADV_MCAST_WANT_ALL_IPV6;
	}
=======
	u8 mcast_flags;

	mcast_flags = batadv_mcast_tvlv_flags_get(orig_mcast_enabled,
						  tvlv_value, tvlv_value_len);
>>>>>>> upstream/android-13

	spin_lock_bh(&orig->mcast_handler_lock);

	if (orig_mcast_enabled &&
	    !test_bit(BATADV_ORIG_CAPA_HAS_MCAST, &orig->capabilities)) {
		set_bit(BATADV_ORIG_CAPA_HAS_MCAST, &orig->capabilities);
	} else if (!orig_mcast_enabled &&
		   test_bit(BATADV_ORIG_CAPA_HAS_MCAST, &orig->capabilities)) {
		clear_bit(BATADV_ORIG_CAPA_HAS_MCAST, &orig->capabilities);
	}

	set_bit(BATADV_ORIG_CAPA_HAS_MCAST, &orig->capa_initialized);

	batadv_mcast_want_unsnoop_update(bat_priv, orig, mcast_flags);
	batadv_mcast_want_ipv4_update(bat_priv, orig, mcast_flags);
	batadv_mcast_want_ipv6_update(bat_priv, orig, mcast_flags);
<<<<<<< HEAD
=======
	batadv_mcast_want_rtr4_update(bat_priv, orig, mcast_flags);
	batadv_mcast_want_rtr6_update(bat_priv, orig, mcast_flags);
>>>>>>> upstream/android-13

	orig->mcast_flags = mcast_flags;
	spin_unlock_bh(&orig->mcast_handler_lock);
}

/**
 * batadv_mcast_init() - initialize the multicast optimizations structures
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_mcast_init(struct batadv_priv *bat_priv)
{
	batadv_tvlv_handler_register(bat_priv, batadv_mcast_tvlv_ogm_handler,
				     NULL, BATADV_TVLV_MCAST, 2,
				     BATADV_TVLV_HANDLER_OGM_CIFNOTFND);

	INIT_DELAYED_WORK(&bat_priv->mcast.work, batadv_mcast_mla_update);
	batadv_mcast_start_timer(bat_priv);
}

<<<<<<< HEAD
#ifdef CONFIG_BATMAN_ADV_DEBUGFS
/**
 * batadv_mcast_flags_print_header() - print own mcast flags to debugfs table
 * @bat_priv: the bat priv with all the soft interface information
 * @seq: debugfs table seq_file struct
 *
 * Prints our own multicast flags including a more specific reason why
 * they are set, that is prints the bridge and querier state too, to
 * the debugfs table specified via @seq.
 */
static void batadv_mcast_flags_print_header(struct batadv_priv *bat_priv,
					    struct seq_file *seq)
{
	u8 flags = bat_priv->mcast.flags;
	char querier4, querier6, shadowing4, shadowing6;
	bool bridged = bat_priv->mcast.bridged;

	if (bridged) {
		querier4 = bat_priv->mcast.querier_ipv4.exists ? '.' : '4';
		querier6 = bat_priv->mcast.querier_ipv6.exists ? '.' : '6';
		shadowing4 = bat_priv->mcast.querier_ipv4.shadowing ? '4' : '.';
		shadowing6 = bat_priv->mcast.querier_ipv6.shadowing ? '6' : '.';
	} else {
		querier4 = '?';
		querier6 = '?';
		shadowing4 = '?';
		shadowing6 = '?';
	}

	seq_printf(seq, "Multicast flags (own flags: [%c%c%c])\n",
		   (flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES) ? 'U' : '.',
		   (flags & BATADV_MCAST_WANT_ALL_IPV4) ? '4' : '.',
		   (flags & BATADV_MCAST_WANT_ALL_IPV6) ? '6' : '.');
	seq_printf(seq, "* Bridged [U]\t\t\t\t%c\n", bridged ? 'U' : '.');
	seq_printf(seq, "* No IGMP/MLD Querier [4/6]:\t\t%c/%c\n",
		   querier4, querier6);
	seq_printf(seq, "* Shadowing IGMP/MLD Querier [4/6]:\t%c/%c\n",
		   shadowing4, shadowing6);
	seq_puts(seq, "-------------------------------------------\n");
	seq_printf(seq, "       %-10s %s\n", "Originator", "Flags");
}

/**
 * batadv_mcast_flags_seq_print_text() - print the mcast flags of other nodes
 * @seq: seq file to print on
 * @offset: not used
 *
 * This prints a table of (primary) originators and their according
 * multicast flags, including (in the header) our own.
 *
 * Return: always 0
 */
int batadv_mcast_flags_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	struct batadv_hard_iface *primary_if;
	struct batadv_hashtable *hash = bat_priv->orig_hash;
	struct batadv_orig_node *orig_node;
	struct hlist_head *head;
	u8 flags;
	u32 i;

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		return 0;

	batadv_mcast_flags_print_header(bat_priv, seq);

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(orig_node, head, hash_entry) {
			if (!test_bit(BATADV_ORIG_CAPA_HAS_MCAST,
				      &orig_node->capa_initialized))
				continue;

			if (!test_bit(BATADV_ORIG_CAPA_HAS_MCAST,
				      &orig_node->capabilities)) {
				seq_printf(seq, "%pM -\n", orig_node->orig);
				continue;
			}

			flags = orig_node->mcast_flags;

			seq_printf(seq, "%pM [%c%c%c]\n", orig_node->orig,
				   (flags & BATADV_MCAST_WANT_ALL_UNSNOOPABLES)
				   ? 'U' : '.',
				   (flags & BATADV_MCAST_WANT_ALL_IPV4)
				   ? '4' : '.',
				   (flags & BATADV_MCAST_WANT_ALL_IPV6)
				   ? '6' : '.');
		}
		rcu_read_unlock();
	}

	batadv_hardif_put(primary_if);

	return 0;
}
#endif

=======
>>>>>>> upstream/android-13
/**
 * batadv_mcast_mesh_info_put() - put multicast info into a netlink message
 * @msg: buffer for the message
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: 0 or error code.
 */
int batadv_mcast_mesh_info_put(struct sk_buff *msg,
			       struct batadv_priv *bat_priv)
{
<<<<<<< HEAD
	u32 flags = bat_priv->mcast.flags;
	u32 flags_priv = BATADV_NO_FLAGS;

	if (bat_priv->mcast.bridged) {
		flags_priv |= BATADV_MCAST_FLAGS_BRIDGED;

		if (bat_priv->mcast.querier_ipv4.exists)
			flags_priv |= BATADV_MCAST_FLAGS_QUERIER_IPV4_EXISTS;
		if (bat_priv->mcast.querier_ipv6.exists)
			flags_priv |= BATADV_MCAST_FLAGS_QUERIER_IPV6_EXISTS;
		if (bat_priv->mcast.querier_ipv4.shadowing)
			flags_priv |= BATADV_MCAST_FLAGS_QUERIER_IPV4_SHADOWING;
		if (bat_priv->mcast.querier_ipv6.shadowing)
=======
	u32 flags = bat_priv->mcast.mla_flags.tvlv_flags;
	u32 flags_priv = BATADV_NO_FLAGS;

	if (bat_priv->mcast.mla_flags.bridged) {
		flags_priv |= BATADV_MCAST_FLAGS_BRIDGED;

		if (bat_priv->mcast.mla_flags.querier_ipv4.exists)
			flags_priv |= BATADV_MCAST_FLAGS_QUERIER_IPV4_EXISTS;
		if (bat_priv->mcast.mla_flags.querier_ipv6.exists)
			flags_priv |= BATADV_MCAST_FLAGS_QUERIER_IPV6_EXISTS;
		if (bat_priv->mcast.mla_flags.querier_ipv4.shadowing)
			flags_priv |= BATADV_MCAST_FLAGS_QUERIER_IPV4_SHADOWING;
		if (bat_priv->mcast.mla_flags.querier_ipv6.shadowing)
>>>>>>> upstream/android-13
			flags_priv |= BATADV_MCAST_FLAGS_QUERIER_IPV6_SHADOWING;
	}

	if (nla_put_u32(msg, BATADV_ATTR_MCAST_FLAGS, flags) ||
	    nla_put_u32(msg, BATADV_ATTR_MCAST_FLAGS_PRIV, flags_priv))
		return -EMSGSIZE;

	return 0;
}

/**
 * batadv_mcast_flags_dump_entry() - dump one entry of the multicast flags table
 *  to a netlink socket
 * @msg: buffer for the message
 * @portid: netlink port
<<<<<<< HEAD
 * @seq: Sequence number of netlink message
=======
 * @cb: Control block containing additional options
>>>>>>> upstream/android-13
 * @orig_node: originator to dump the multicast flags of
 *
 * Return: 0 or error code.
 */
static int
<<<<<<< HEAD
batadv_mcast_flags_dump_entry(struct sk_buff *msg, u32 portid, u32 seq,
=======
batadv_mcast_flags_dump_entry(struct sk_buff *msg, u32 portid,
			      struct netlink_callback *cb,
>>>>>>> upstream/android-13
			      struct batadv_orig_node *orig_node)
{
	void *hdr;

<<<<<<< HEAD
	hdr = genlmsg_put(msg, portid, seq, &batadv_netlink_family,
			  NLM_F_MULTI, BATADV_CMD_GET_MCAST_FLAGS);
	if (!hdr)
		return -ENOBUFS;

=======
	hdr = genlmsg_put(msg, portid, cb->nlh->nlmsg_seq,
			  &batadv_netlink_family, NLM_F_MULTI,
			  BATADV_CMD_GET_MCAST_FLAGS);
	if (!hdr)
		return -ENOBUFS;

	genl_dump_check_consistent(cb, hdr);

>>>>>>> upstream/android-13
	if (nla_put(msg, BATADV_ATTR_ORIG_ADDRESS, ETH_ALEN,
		    orig_node->orig)) {
		genlmsg_cancel(msg, hdr);
		return -EMSGSIZE;
	}

	if (test_bit(BATADV_ORIG_CAPA_HAS_MCAST,
		     &orig_node->capabilities)) {
		if (nla_put_u32(msg, BATADV_ATTR_MCAST_FLAGS,
				orig_node->mcast_flags)) {
			genlmsg_cancel(msg, hdr);
			return -EMSGSIZE;
		}
	}

	genlmsg_end(msg, hdr);
	return 0;
}

/**
 * batadv_mcast_flags_dump_bucket() - dump one bucket of the multicast flags
 *  table to a netlink socket
 * @msg: buffer for the message
 * @portid: netlink port
<<<<<<< HEAD
 * @seq: Sequence number of netlink message
 * @head: bucket to dump
=======
 * @cb: Control block containing additional options
 * @hash: hash to dump
 * @bucket: bucket index to dump
>>>>>>> upstream/android-13
 * @idx_skip: How many entries to skip
 *
 * Return: 0 or error code.
 */
static int
<<<<<<< HEAD
batadv_mcast_flags_dump_bucket(struct sk_buff *msg, u32 portid, u32 seq,
			       struct hlist_head *head, long *idx_skip)
=======
batadv_mcast_flags_dump_bucket(struct sk_buff *msg, u32 portid,
			       struct netlink_callback *cb,
			       struct batadv_hashtable *hash,
			       unsigned int bucket, long *idx_skip)
>>>>>>> upstream/android-13
{
	struct batadv_orig_node *orig_node;
	long idx = 0;

<<<<<<< HEAD
	rcu_read_lock();
	hlist_for_each_entry_rcu(orig_node, head, hash_entry) {
=======
	spin_lock_bh(&hash->list_locks[bucket]);
	cb->seq = atomic_read(&hash->generation) << 1 | 1;

	hlist_for_each_entry(orig_node, &hash->table[bucket], hash_entry) {
>>>>>>> upstream/android-13
		if (!test_bit(BATADV_ORIG_CAPA_HAS_MCAST,
			      &orig_node->capa_initialized))
			continue;

		if (idx < *idx_skip)
			goto skip;

<<<<<<< HEAD
		if (batadv_mcast_flags_dump_entry(msg, portid, seq,
						  orig_node)) {
			rcu_read_unlock();
=======
		if (batadv_mcast_flags_dump_entry(msg, portid, cb, orig_node)) {
			spin_unlock_bh(&hash->list_locks[bucket]);
>>>>>>> upstream/android-13
			*idx_skip = idx;

			return -EMSGSIZE;
		}

skip:
		idx++;
	}
<<<<<<< HEAD
	rcu_read_unlock();
=======
	spin_unlock_bh(&hash->list_locks[bucket]);
>>>>>>> upstream/android-13

	return 0;
}

/**
 * __batadv_mcast_flags_dump() - dump multicast flags table to a netlink socket
 * @msg: buffer for the message
 * @portid: netlink port
<<<<<<< HEAD
 * @seq: Sequence number of netlink message
=======
 * @cb: Control block containing additional options
>>>>>>> upstream/android-13
 * @bat_priv: the bat priv with all the soft interface information
 * @bucket: current bucket to dump
 * @idx: index in current bucket to the next entry to dump
 *
 * Return: 0 or error code.
 */
static int
<<<<<<< HEAD
__batadv_mcast_flags_dump(struct sk_buff *msg, u32 portid, u32 seq,
=======
__batadv_mcast_flags_dump(struct sk_buff *msg, u32 portid,
			  struct netlink_callback *cb,
>>>>>>> upstream/android-13
			  struct batadv_priv *bat_priv, long *bucket, long *idx)
{
	struct batadv_hashtable *hash = bat_priv->orig_hash;
	long bucket_tmp = *bucket;
<<<<<<< HEAD
	struct hlist_head *head;
	long idx_tmp = *idx;

	while (bucket_tmp < hash->size) {
		head = &hash->table[bucket_tmp];

		if (batadv_mcast_flags_dump_bucket(msg, portid, seq, head,
						   &idx_tmp))
=======
	long idx_tmp = *idx;

	while (bucket_tmp < hash->size) {
		if (batadv_mcast_flags_dump_bucket(msg, portid, cb, hash,
						   bucket_tmp, &idx_tmp))
>>>>>>> upstream/android-13
			break;

		bucket_tmp++;
		idx_tmp = 0;
	}

	*bucket = bucket_tmp;
	*idx = idx_tmp;

	return msg->len;
}

/**
 * batadv_mcast_netlink_get_primary() - get primary interface from netlink
 *  callback
 * @cb: netlink callback structure
 * @primary_if: the primary interface pointer to return the result in
 *
 * Return: 0 or error code.
 */
static int
batadv_mcast_netlink_get_primary(struct netlink_callback *cb,
				 struct batadv_hard_iface **primary_if)
{
	struct batadv_hard_iface *hard_iface = NULL;
	struct net *net = sock_net(cb->skb->sk);
	struct net_device *soft_iface;
	struct batadv_priv *bat_priv;
	int ifindex;
	int ret = 0;

	ifindex = batadv_netlink_get_ifindex(cb->nlh, BATADV_ATTR_MESH_IFINDEX);
	if (!ifindex)
		return -EINVAL;

	soft_iface = dev_get_by_index(net, ifindex);
	if (!soft_iface || !batadv_softif_is_valid(soft_iface)) {
		ret = -ENODEV;
		goto out;
	}

	bat_priv = netdev_priv(soft_iface);

	hard_iface = batadv_primary_if_get_selected(bat_priv);
	if (!hard_iface || hard_iface->if_status != BATADV_IF_ACTIVE) {
		ret = -ENOENT;
		goto out;
	}

out:
<<<<<<< HEAD
	if (soft_iface)
		dev_put(soft_iface);

	if (!ret && primary_if)
		*primary_if = hard_iface;
	else if (hard_iface)
=======
	dev_put(soft_iface);

	if (!ret && primary_if)
		*primary_if = hard_iface;
	else
>>>>>>> upstream/android-13
		batadv_hardif_put(hard_iface);

	return ret;
}

/**
 * batadv_mcast_flags_dump() - dump multicast flags table to a netlink socket
 * @msg: buffer for the message
 * @cb: callback structure containing arguments
 *
 * Return: message length.
 */
int batadv_mcast_flags_dump(struct sk_buff *msg, struct netlink_callback *cb)
{
	struct batadv_hard_iface *primary_if = NULL;
	int portid = NETLINK_CB(cb->skb).portid;
	struct batadv_priv *bat_priv;
	long *bucket = &cb->args[0];
	long *idx = &cb->args[1];
	int ret;

	ret = batadv_mcast_netlink_get_primary(cb, &primary_if);
	if (ret)
		return ret;

	bat_priv = netdev_priv(primary_if->soft_iface);
<<<<<<< HEAD
	ret = __batadv_mcast_flags_dump(msg, portid, cb->nlh->nlmsg_seq,
					bat_priv, bucket, idx);
=======
	ret = __batadv_mcast_flags_dump(msg, portid, cb, bat_priv, bucket, idx);
>>>>>>> upstream/android-13

	batadv_hardif_put(primary_if);
	return ret;
}

/**
 * batadv_mcast_free() - free the multicast optimizations structures
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_mcast_free(struct batadv_priv *bat_priv)
{
	cancel_delayed_work_sync(&bat_priv->mcast.work);

	batadv_tvlv_container_unregister(bat_priv, BATADV_TVLV_MCAST, 2);
	batadv_tvlv_handler_unregister(bat_priv, BATADV_TVLV_MCAST, 2);

	/* safely calling outside of worker, as worker was canceled above */
	batadv_mcast_mla_tt_retract(bat_priv, NULL);
}

/**
 * batadv_mcast_purge_orig() - reset originator global mcast state modifications
 * @orig: the originator which is going to get purged
 */
void batadv_mcast_purge_orig(struct batadv_orig_node *orig)
{
	struct batadv_priv *bat_priv = orig->bat_priv;

	spin_lock_bh(&orig->mcast_handler_lock);

	batadv_mcast_want_unsnoop_update(bat_priv, orig, BATADV_NO_FLAGS);
	batadv_mcast_want_ipv4_update(bat_priv, orig, BATADV_NO_FLAGS);
	batadv_mcast_want_ipv6_update(bat_priv, orig, BATADV_NO_FLAGS);
<<<<<<< HEAD
=======
	batadv_mcast_want_rtr4_update(bat_priv, orig,
				      BATADV_MCAST_WANT_NO_RTR4);
	batadv_mcast_want_rtr6_update(bat_priv, orig,
				      BATADV_MCAST_WANT_NO_RTR6);
>>>>>>> upstream/android-13

	spin_unlock_bh(&orig->mcast_handler_lock);
}
