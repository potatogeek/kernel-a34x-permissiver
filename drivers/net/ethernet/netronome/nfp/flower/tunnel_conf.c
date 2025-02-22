<<<<<<< HEAD
/*
 * Copyright (C) 2017 Netronome Systems, Inc.
 *
 * This software is dual licensed under the GNU General License Version 2,
 * June 1991 as shown in the file COPYING in the top-level directory of this
 * source tree or the BSD 2-Clause License provided below.  You have the
 * option to license this software under the complete terms of either license.
 *
 * The BSD 2-Clause License:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      1. Redistributions of source code must retain the above
 *         copyright notice, this list of conditions and the following
 *         disclaimer.
 *
 *      2. Redistributions in binary form must reproduce the above
 *         copyright notice, this list of conditions and the following
 *         disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
=======
// SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause)
/* Copyright (C) 2017-2018 Netronome Systems, Inc. */
>>>>>>> upstream/android-13

#include <linux/etherdevice.h>
#include <linux/inetdevice.h>
#include <net/netevent.h>
#include <linux/idr.h>
#include <net/dst_metadata.h>
#include <net/arp.h>

#include "cmsg.h"
#include "main.h"
#include "../nfp_net_repr.h"
#include "../nfp_net.h"

#define NFP_FL_MAX_ROUTES               32

<<<<<<< HEAD
=======
#define NFP_TUN_PRE_TUN_RULE_LIMIT	32
#define NFP_TUN_PRE_TUN_RULE_DEL	BIT(0)
#define NFP_TUN_PRE_TUN_IDX_BIT		BIT(3)
#define NFP_TUN_PRE_TUN_IPV6_BIT	BIT(7)

/**
 * struct nfp_tun_pre_tun_rule - rule matched before decap
 * @flags:		options for the rule offset
 * @port_idx:		index of destination MAC address for the rule
 * @vlan_tci:		VLAN info associated with MAC
 * @host_ctx_id:	stats context of rule to update
 */
struct nfp_tun_pre_tun_rule {
	__be32 flags;
	__be16 port_idx;
	__be16 vlan_tci;
	__be32 host_ctx_id;
};

>>>>>>> upstream/android-13
/**
 * struct nfp_tun_active_tuns - periodic message of active tunnels
 * @seq:		sequence number of the message
 * @count:		number of tunnels report in message
 * @flags:		options part of the request
 * @tun_info.ipv4:		dest IPv4 address of active route
 * @tun_info.egress_port:	port the encapsulated packet egressed
 * @tun_info.extra:		reserved for future use
 * @tun_info:		tunnels that have sent traffic in reported period
 */
struct nfp_tun_active_tuns {
	__be32 seq;
	__be32 count;
	__be32 flags;
	struct route_ip_info {
		__be32 ipv4;
		__be32 egress_port;
		__be32 extra[2];
	} tun_info[];
};

/**
<<<<<<< HEAD
=======
 * struct nfp_tun_active_tuns_v6 - periodic message of active IPv6 tunnels
 * @seq:		sequence number of the message
 * @count:		number of tunnels report in message
 * @flags:		options part of the request
 * @tun_info.ipv6:		dest IPv6 address of active route
 * @tun_info.egress_port:	port the encapsulated packet egressed
 * @tun_info.extra:		reserved for future use
 * @tun_info:		tunnels that have sent traffic in reported period
 */
struct nfp_tun_active_tuns_v6 {
	__be32 seq;
	__be32 count;
	__be32 flags;
	struct route_ip_info_v6 {
		struct in6_addr ipv6;
		__be32 egress_port;
		__be32 extra[2];
	} tun_info[];
};

/**
>>>>>>> upstream/android-13
 * struct nfp_tun_neigh - neighbour/route entry on the NFP
 * @dst_ipv4:	destination IPv4 address
 * @src_ipv4:	source IPv4 address
 * @dst_addr:	destination MAC address
 * @src_addr:	source MAC address
 * @port_id:	NFP port to output packet on - associated with source IPv4
 */
struct nfp_tun_neigh {
	__be32 dst_ipv4;
	__be32 src_ipv4;
	u8 dst_addr[ETH_ALEN];
	u8 src_addr[ETH_ALEN];
	__be32 port_id;
};

/**
<<<<<<< HEAD
=======
 * struct nfp_tun_neigh_v6 - neighbour/route entry on the NFP
 * @dst_ipv6:	destination IPv6 address
 * @src_ipv6:	source IPv6 address
 * @dst_addr:	destination MAC address
 * @src_addr:	source MAC address
 * @port_id:	NFP port to output packet on - associated with source IPv6
 */
struct nfp_tun_neigh_v6 {
	struct in6_addr dst_ipv6;
	struct in6_addr src_ipv6;
	u8 dst_addr[ETH_ALEN];
	u8 src_addr[ETH_ALEN];
	__be32 port_id;
};

/**
>>>>>>> upstream/android-13
 * struct nfp_tun_req_route_ipv4 - NFP requests a route/neighbour lookup
 * @ingress_port:	ingress port of packet that signalled request
 * @ipv4_addr:		destination ipv4 address for route
 * @reserved:		reserved for future use
 */
struct nfp_tun_req_route_ipv4 {
	__be32 ingress_port;
	__be32 ipv4_addr;
	__be32 reserved[2];
};

/**
<<<<<<< HEAD
 * struct nfp_ipv4_route_entry - routes that are offloaded to the NFP
 * @ipv4_addr:	destination of route
 * @list:	list pointer
 */
struct nfp_ipv4_route_entry {
	__be32 ipv4_addr;
	struct list_head list;
=======
 * struct nfp_tun_req_route_ipv6 - NFP requests an IPv6 route/neighbour lookup
 * @ingress_port:	ingress port of packet that signalled request
 * @ipv6_addr:		destination ipv6 address for route
 */
struct nfp_tun_req_route_ipv6 {
	__be32 ingress_port;
	struct in6_addr ipv6_addr;
};

/**
 * struct nfp_offloaded_route - routes that are offloaded to the NFP
 * @list:	list pointer
 * @ip_add:	destination of route - can be IPv4 or IPv6
 */
struct nfp_offloaded_route {
	struct list_head list;
	u8 ip_add[];
>>>>>>> upstream/android-13
};

#define NFP_FL_IPV4_ADDRS_MAX        32

/**
 * struct nfp_tun_ipv4_addr - set the IP address list on the NFP
 * @count:	number of IPs populated in the array
 * @ipv4_addr:	array of IPV4_ADDRS_MAX 32 bit IPv4 addresses
 */
struct nfp_tun_ipv4_addr {
	__be32 count;
	__be32 ipv4_addr[NFP_FL_IPV4_ADDRS_MAX];
};

/**
 * struct nfp_ipv4_addr_entry - cached IPv4 addresses
 * @ipv4_addr:	IP address
 * @ref_count:	number of rules currently using this IP
 * @list:	list pointer
 */
struct nfp_ipv4_addr_entry {
	__be32 ipv4_addr;
	int ref_count;
	struct list_head list;
};

<<<<<<< HEAD
/**
 * struct nfp_tun_mac_addr - configure MAC address of tunnel EP on NFP
 * @reserved:	reserved for future use
 * @count:	number of MAC addresses in the message
 * @addresses.index:	index of MAC address in the lookup table
 * @addresses.addr:	interface MAC address
 * @addresses:	series of MACs to offload
 */
struct nfp_tun_mac_addr {
	__be16 reserved;
	__be16 count;
	struct index_mac_addr {
		__be16 index;
		u8 addr[ETH_ALEN];
	} addresses[];
};

/**
 * struct nfp_tun_mac_offload_entry - list of MACs to offload
 * @index:	index of MAC address for offloading
 * @addr:	interface MAC address
 * @list:	list pointer
 */
struct nfp_tun_mac_offload_entry {
	__be16 index;
	u8 addr[ETH_ALEN];
	struct list_head list;
=======
#define NFP_FL_IPV6_ADDRS_MAX        4

/**
 * struct nfp_tun_ipv6_addr - set the IP address list on the NFP
 * @count:	number of IPs populated in the array
 * @ipv6_addr:	array of IPV6_ADDRS_MAX 128 bit IPv6 addresses
 */
struct nfp_tun_ipv6_addr {
	__be32 count;
	struct in6_addr ipv6_addr[NFP_FL_IPV6_ADDRS_MAX];
};

#define NFP_TUN_MAC_OFFLOAD_DEL_FLAG	0x2

/**
 * struct nfp_tun_mac_addr_offload - configure MAC address of tunnel EP on NFP
 * @flags:	MAC address offload options
 * @count:	number of MAC addresses in the message (should be 1)
 * @index:	index of MAC address in the lookup table
 * @addr:	interface MAC address
 */
struct nfp_tun_mac_addr_offload {
	__be16 flags;
	__be16 count;
	__be16 index;
	u8 addr[ETH_ALEN];
};

enum nfp_flower_mac_offload_cmd {
	NFP_TUNNEL_MAC_OFFLOAD_ADD =		0,
	NFP_TUNNEL_MAC_OFFLOAD_DEL =		1,
	NFP_TUNNEL_MAC_OFFLOAD_MOD =		2,
>>>>>>> upstream/android-13
};

#define NFP_MAX_MAC_INDEX       0xff

/**
<<<<<<< HEAD
 * struct nfp_tun_mac_non_nfp_idx - converts non NFP netdev ifindex to 8-bit id
 * @ifindex:	netdev ifindex of the device
 * @index:	index of netdevs mac on NFP
 * @list:	list pointer
 */
struct nfp_tun_mac_non_nfp_idx {
	int ifindex;
	u8 index;
	struct list_head list;
=======
 * struct nfp_tun_offloaded_mac - hashtable entry for an offloaded MAC
 * @ht_node:		Hashtable entry
 * @addr:		Offloaded MAC address
 * @index:		Offloaded index for given MAC address
 * @ref_count:		Number of devs using this MAC address
 * @repr_list:		List of reprs sharing this MAC address
 * @bridge_count:	Number of bridge/internal devs with MAC
 */
struct nfp_tun_offloaded_mac {
	struct rhash_head ht_node;
	u8 addr[ETH_ALEN];
	u16 index;
	int ref_count;
	struct list_head repr_list;
	int bridge_count;
};

static const struct rhashtable_params offloaded_macs_params = {
	.key_offset	= offsetof(struct nfp_tun_offloaded_mac, addr),
	.head_offset	= offsetof(struct nfp_tun_offloaded_mac, ht_node),
	.key_len	= ETH_ALEN,
	.automatic_shrinking	= true,
>>>>>>> upstream/android-13
};

void nfp_tunnel_keep_alive(struct nfp_app *app, struct sk_buff *skb)
{
	struct nfp_tun_active_tuns *payload;
	struct net_device *netdev;
	int count, i, pay_len;
	struct neighbour *n;
	__be32 ipv4_addr;
	u32 port;

	payload = nfp_flower_cmsg_get_data(skb);
	count = be32_to_cpu(payload->count);
	if (count > NFP_FL_MAX_ROUTES) {
		nfp_flower_cmsg_warn(app, "Tunnel keep-alive request exceeds max routes.\n");
		return;
	}

	pay_len = nfp_flower_cmsg_get_data_len(skb);
<<<<<<< HEAD
	if (pay_len != sizeof(struct nfp_tun_active_tuns) +
	    sizeof(struct route_ip_info) * count) {
=======
	if (pay_len != struct_size(payload, tun_info, count)) {
>>>>>>> upstream/android-13
		nfp_flower_cmsg_warn(app, "Corruption in tunnel keep-alive message.\n");
		return;
	}

	rcu_read_lock();
	for (i = 0; i < count; i++) {
		ipv4_addr = payload->tun_info[i].ipv4;
		port = be32_to_cpu(payload->tun_info[i].egress_port);
<<<<<<< HEAD
		netdev = nfp_app_repr_get(app, port);
=======
		netdev = nfp_app_dev_get(app, port, NULL);
>>>>>>> upstream/android-13
		if (!netdev)
			continue;

		n = neigh_lookup(&arp_tbl, &ipv4_addr, netdev);
		if (!n)
			continue;

		/* Update the used timestamp of neighbour */
		neigh_event_send(n, NULL);
		neigh_release(n);
	}
	rcu_read_unlock();
}

<<<<<<< HEAD
static bool nfp_tun_is_netdev_to_offload(struct net_device *netdev)
{
	if (!netdev->rtnl_link_ops)
		return false;
	if (!strcmp(netdev->rtnl_link_ops->kind, "openvswitch"))
		return true;
	if (!strcmp(netdev->rtnl_link_ops->kind, "vxlan"))
		return true;

	return false;
=======
void nfp_tunnel_keep_alive_v6(struct nfp_app *app, struct sk_buff *skb)
{
#if IS_ENABLED(CONFIG_IPV6)
	struct nfp_tun_active_tuns_v6 *payload;
	struct net_device *netdev;
	int count, i, pay_len;
	struct neighbour *n;
	void *ipv6_add;
	u32 port;

	payload = nfp_flower_cmsg_get_data(skb);
	count = be32_to_cpu(payload->count);
	if (count > NFP_FL_IPV6_ADDRS_MAX) {
		nfp_flower_cmsg_warn(app, "IPv6 tunnel keep-alive request exceeds max routes.\n");
		return;
	}

	pay_len = nfp_flower_cmsg_get_data_len(skb);
	if (pay_len != struct_size(payload, tun_info, count)) {
		nfp_flower_cmsg_warn(app, "Corruption in tunnel keep-alive message.\n");
		return;
	}

	rcu_read_lock();
	for (i = 0; i < count; i++) {
		ipv6_add = &payload->tun_info[i].ipv6;
		port = be32_to_cpu(payload->tun_info[i].egress_port);
		netdev = nfp_app_dev_get(app, port, NULL);
		if (!netdev)
			continue;

		n = neigh_lookup(&nd_tbl, ipv6_add, netdev);
		if (!n)
			continue;

		/* Update the used timestamp of neighbour */
		neigh_event_send(n, NULL);
		neigh_release(n);
	}
	rcu_read_unlock();
#endif
>>>>>>> upstream/android-13
}

static int
nfp_flower_xmit_tun_conf(struct nfp_app *app, u8 mtype, u16 plen, void *pdata,
			 gfp_t flag)
{
	struct sk_buff *skb;
	unsigned char *msg;

	skb = nfp_flower_cmsg_alloc(app, plen, mtype, flag);
	if (!skb)
		return -ENOMEM;

	msg = nfp_flower_cmsg_get_data(skb);
	memcpy(msg, pdata, nfp_flower_cmsg_get_data_len(skb));

	nfp_ctrl_tx(app->ctrl, skb);
	return 0;
}

<<<<<<< HEAD
static bool nfp_tun_has_route(struct nfp_app *app, __be32 ipv4_addr)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv4_route_entry *entry;
	struct list_head *ptr, *storage;

	spin_lock_bh(&priv->nfp_neigh_off_lock);
	list_for_each_safe(ptr, storage, &priv->nfp_neigh_off_list) {
		entry = list_entry(ptr, struct nfp_ipv4_route_entry, list);
		if (entry->ipv4_addr == ipv4_addr) {
			spin_unlock_bh(&priv->nfp_neigh_off_lock);
			return true;
		}
	}
	spin_unlock_bh(&priv->nfp_neigh_off_lock);
	return false;
}

static void nfp_tun_add_route_to_cache(struct nfp_app *app, __be32 ipv4_addr)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv4_route_entry *entry;
	struct list_head *ptr, *storage;

	spin_lock_bh(&priv->nfp_neigh_off_lock);
	list_for_each_safe(ptr, storage, &priv->nfp_neigh_off_list) {
		entry = list_entry(ptr, struct nfp_ipv4_route_entry, list);
		if (entry->ipv4_addr == ipv4_addr) {
			spin_unlock_bh(&priv->nfp_neigh_off_lock);
			return;
		}
	}
	entry = kmalloc(sizeof(*entry), GFP_ATOMIC);
	if (!entry) {
		spin_unlock_bh(&priv->nfp_neigh_off_lock);
		nfp_flower_cmsg_warn(app, "Mem error when storing new route.\n");
		return;
	}

	entry->ipv4_addr = ipv4_addr;
	list_add_tail(&entry->list, &priv->nfp_neigh_off_list);
	spin_unlock_bh(&priv->nfp_neigh_off_lock);
}

static void nfp_tun_del_route_from_cache(struct nfp_app *app, __be32 ipv4_addr)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv4_route_entry *entry;
	struct list_head *ptr, *storage;

	spin_lock_bh(&priv->nfp_neigh_off_lock);
	list_for_each_safe(ptr, storage, &priv->nfp_neigh_off_list) {
		entry = list_entry(ptr, struct nfp_ipv4_route_entry, list);
		if (entry->ipv4_addr == ipv4_addr) {
=======
static bool
__nfp_tun_has_route(struct list_head *route_list, spinlock_t *list_lock,
		    void *add, int add_len)
{
	struct nfp_offloaded_route *entry;

	spin_lock_bh(list_lock);
	list_for_each_entry(entry, route_list, list)
		if (!memcmp(entry->ip_add, add, add_len)) {
			spin_unlock_bh(list_lock);
			return true;
		}
	spin_unlock_bh(list_lock);
	return false;
}

static int
__nfp_tun_add_route_to_cache(struct list_head *route_list,
			     spinlock_t *list_lock, void *add, int add_len)
{
	struct nfp_offloaded_route *entry;

	spin_lock_bh(list_lock);
	list_for_each_entry(entry, route_list, list)
		if (!memcmp(entry->ip_add, add, add_len)) {
			spin_unlock_bh(list_lock);
			return 0;
		}

	entry = kmalloc(sizeof(*entry) + add_len, GFP_ATOMIC);
	if (!entry) {
		spin_unlock_bh(list_lock);
		return -ENOMEM;
	}

	memcpy(entry->ip_add, add, add_len);
	list_add_tail(&entry->list, route_list);
	spin_unlock_bh(list_lock);

	return 0;
}

static void
__nfp_tun_del_route_from_cache(struct list_head *route_list,
			       spinlock_t *list_lock, void *add, int add_len)
{
	struct nfp_offloaded_route *entry;

	spin_lock_bh(list_lock);
	list_for_each_entry(entry, route_list, list)
		if (!memcmp(entry->ip_add, add, add_len)) {
>>>>>>> upstream/android-13
			list_del(&entry->list);
			kfree(entry);
			break;
		}
<<<<<<< HEAD
	}
	spin_unlock_bh(&priv->nfp_neigh_off_lock);
}

static void
nfp_tun_write_neigh(struct net_device *netdev, struct nfp_app *app,
		    struct flowi4 *flow, struct neighbour *neigh, gfp_t flag)
{
	struct nfp_tun_neigh payload;

	/* Only offload representor IPv4s for now. */
	if (!nfp_netdev_is_nfp_repr(netdev))
=======
	spin_unlock_bh(list_lock);
}

static bool nfp_tun_has_route_v4(struct nfp_app *app, __be32 *ipv4_addr)
{
	struct nfp_flower_priv *priv = app->priv;

	return __nfp_tun_has_route(&priv->tun.neigh_off_list_v4,
				   &priv->tun.neigh_off_lock_v4, ipv4_addr,
				   sizeof(*ipv4_addr));
}

static bool
nfp_tun_has_route_v6(struct nfp_app *app, struct in6_addr *ipv6_addr)
{
	struct nfp_flower_priv *priv = app->priv;

	return __nfp_tun_has_route(&priv->tun.neigh_off_list_v6,
				   &priv->tun.neigh_off_lock_v6, ipv6_addr,
				   sizeof(*ipv6_addr));
}

static void
nfp_tun_add_route_to_cache_v4(struct nfp_app *app, __be32 *ipv4_addr)
{
	struct nfp_flower_priv *priv = app->priv;

	__nfp_tun_add_route_to_cache(&priv->tun.neigh_off_list_v4,
				     &priv->tun.neigh_off_lock_v4, ipv4_addr,
				     sizeof(*ipv4_addr));
}

static void
nfp_tun_add_route_to_cache_v6(struct nfp_app *app, struct in6_addr *ipv6_addr)
{
	struct nfp_flower_priv *priv = app->priv;

	__nfp_tun_add_route_to_cache(&priv->tun.neigh_off_list_v6,
				     &priv->tun.neigh_off_lock_v6, ipv6_addr,
				     sizeof(*ipv6_addr));
}

static void
nfp_tun_del_route_from_cache_v4(struct nfp_app *app, __be32 *ipv4_addr)
{
	struct nfp_flower_priv *priv = app->priv;

	__nfp_tun_del_route_from_cache(&priv->tun.neigh_off_list_v4,
				       &priv->tun.neigh_off_lock_v4, ipv4_addr,
				       sizeof(*ipv4_addr));
}

static void
nfp_tun_del_route_from_cache_v6(struct nfp_app *app, struct in6_addr *ipv6_addr)
{
	struct nfp_flower_priv *priv = app->priv;

	__nfp_tun_del_route_from_cache(&priv->tun.neigh_off_list_v6,
				       &priv->tun.neigh_off_lock_v6, ipv6_addr,
				       sizeof(*ipv6_addr));
}

static void
nfp_tun_write_neigh_v4(struct net_device *netdev, struct nfp_app *app,
		       struct flowi4 *flow, struct neighbour *neigh, gfp_t flag)
{
	struct nfp_tun_neigh payload;
	u32 port_id;

	port_id = nfp_flower_get_port_id_from_netdev(app, netdev);
	if (!port_id)
>>>>>>> upstream/android-13
		return;

	memset(&payload, 0, sizeof(struct nfp_tun_neigh));
	payload.dst_ipv4 = flow->daddr;

	/* If entry has expired send dst IP with all other fields 0. */
	if (!(neigh->nud_state & NUD_VALID) || neigh->dead) {
<<<<<<< HEAD
		nfp_tun_del_route_from_cache(app, payload.dst_ipv4);
=======
		nfp_tun_del_route_from_cache_v4(app, &payload.dst_ipv4);
>>>>>>> upstream/android-13
		/* Trigger ARP to verify invalid neighbour state. */
		neigh_event_send(neigh, NULL);
		goto send_msg;
	}

	/* Have a valid neighbour so populate rest of entry. */
	payload.src_ipv4 = flow->saddr;
	ether_addr_copy(payload.src_addr, netdev->dev_addr);
	neigh_ha_snapshot(payload.dst_addr, neigh, netdev);
<<<<<<< HEAD
	payload.port_id = cpu_to_be32(nfp_repr_get_port_id(netdev));
	/* Add destination of new route to NFP cache. */
	nfp_tun_add_route_to_cache(app, payload.dst_ipv4);
=======
	payload.port_id = cpu_to_be32(port_id);
	/* Add destination of new route to NFP cache. */
	nfp_tun_add_route_to_cache_v4(app, &payload.dst_ipv4);
>>>>>>> upstream/android-13

send_msg:
	nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_TUN_NEIGH,
				 sizeof(struct nfp_tun_neigh),
				 (unsigned char *)&payload, flag);
}

<<<<<<< HEAD
=======
static void
nfp_tun_write_neigh_v6(struct net_device *netdev, struct nfp_app *app,
		       struct flowi6 *flow, struct neighbour *neigh, gfp_t flag)
{
	struct nfp_tun_neigh_v6 payload;
	u32 port_id;

	port_id = nfp_flower_get_port_id_from_netdev(app, netdev);
	if (!port_id)
		return;

	memset(&payload, 0, sizeof(struct nfp_tun_neigh_v6));
	payload.dst_ipv6 = flow->daddr;

	/* If entry has expired send dst IP with all other fields 0. */
	if (!(neigh->nud_state & NUD_VALID) || neigh->dead) {
		nfp_tun_del_route_from_cache_v6(app, &payload.dst_ipv6);
		/* Trigger probe to verify invalid neighbour state. */
		neigh_event_send(neigh, NULL);
		goto send_msg;
	}

	/* Have a valid neighbour so populate rest of entry. */
	payload.src_ipv6 = flow->saddr;
	ether_addr_copy(payload.src_addr, netdev->dev_addr);
	neigh_ha_snapshot(payload.dst_addr, neigh, netdev);
	payload.port_id = cpu_to_be32(port_id);
	/* Add destination of new route to NFP cache. */
	nfp_tun_add_route_to_cache_v6(app, &payload.dst_ipv6);

send_msg:
	nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_TUN_NEIGH_V6,
				 sizeof(struct nfp_tun_neigh_v6),
				 (unsigned char *)&payload, flag);
}

>>>>>>> upstream/android-13
static int
nfp_tun_neigh_event_handler(struct notifier_block *nb, unsigned long event,
			    void *ptr)
{
	struct nfp_flower_priv *app_priv;
	struct netevent_redirect *redir;
<<<<<<< HEAD
	struct flowi4 flow = {};
	struct neighbour *n;
	struct nfp_app *app;
	struct rtable *rt;
=======
	struct flowi4 flow4 = {};
	struct flowi6 flow6 = {};
	struct neighbour *n;
	struct nfp_app *app;
	struct rtable *rt;
	bool ipv6 = false;
>>>>>>> upstream/android-13
	int err;

	switch (event) {
	case NETEVENT_REDIRECT:
		redir = (struct netevent_redirect *)ptr;
		n = redir->neigh;
		break;
	case NETEVENT_NEIGH_UPDATE:
		n = (struct neighbour *)ptr;
		break;
	default:
		return NOTIFY_DONE;
	}

<<<<<<< HEAD
	flow.daddr = *(__be32 *)n->primary_key;

	/* Only concerned with route changes for representors. */
	if (!nfp_netdev_is_nfp_repr(n->dev))
		return NOTIFY_DONE;

	app_priv = container_of(nb, struct nfp_flower_priv, nfp_tun_neigh_nb);
	app = app_priv->app;

	/* Only concerned with changes to routes already added to NFP. */
	if (!nfp_tun_has_route(app, flow.daddr))
		return NOTIFY_DONE;

#if IS_ENABLED(CONFIG_INET)
	/* Do a route lookup to populate flow data. */
	rt = ip_route_output_key(dev_net(n->dev), &flow);
	err = PTR_ERR_OR_ZERO(rt);
	if (err)
		return NOTIFY_DONE;

	ip_rt_put(rt);
#else
	return NOTIFY_DONE;
#endif

	flow.flowi4_proto = IPPROTO_UDP;
	nfp_tun_write_neigh(n->dev, app, &flow, n, GFP_ATOMIC);
=======
	if (n->tbl->family == AF_INET6)
		ipv6 = true;

	if (ipv6)
		flow6.daddr = *(struct in6_addr *)n->primary_key;
	else
		flow4.daddr = *(__be32 *)n->primary_key;

	app_priv = container_of(nb, struct nfp_flower_priv, tun.neigh_nb);
	app = app_priv->app;

	if (!nfp_netdev_is_nfp_repr(n->dev) &&
	    !nfp_flower_internal_port_can_offload(app, n->dev))
		return NOTIFY_DONE;

	/* Only concerned with changes to routes already added to NFP. */
	if ((ipv6 && !nfp_tun_has_route_v6(app, &flow6.daddr)) ||
	    (!ipv6 && !nfp_tun_has_route_v4(app, &flow4.daddr)))
		return NOTIFY_DONE;

#if IS_ENABLED(CONFIG_INET)
	if (ipv6) {
#if IS_ENABLED(CONFIG_IPV6)
		struct dst_entry *dst;

		dst = ipv6_stub->ipv6_dst_lookup_flow(dev_net(n->dev), NULL,
						      &flow6, NULL);
		if (IS_ERR(dst))
			return NOTIFY_DONE;

		dst_release(dst);
		flow6.flowi6_proto = IPPROTO_UDP;
		nfp_tun_write_neigh_v6(n->dev, app, &flow6, n, GFP_ATOMIC);
#else
		return NOTIFY_DONE;
#endif /* CONFIG_IPV6 */
	} else {
		/* Do a route lookup to populate flow data. */
		rt = ip_route_output_key(dev_net(n->dev), &flow4);
		err = PTR_ERR_OR_ZERO(rt);
		if (err)
			return NOTIFY_DONE;

		ip_rt_put(rt);

		flow4.flowi4_proto = IPPROTO_UDP;
		nfp_tun_write_neigh_v4(n->dev, app, &flow4, n, GFP_ATOMIC);
	}
#else
	return NOTIFY_DONE;
#endif /* CONFIG_INET */
>>>>>>> upstream/android-13

	return NOTIFY_OK;
}

<<<<<<< HEAD
void nfp_tunnel_request_route(struct nfp_app *app, struct sk_buff *skb)
=======
void nfp_tunnel_request_route_v4(struct nfp_app *app, struct sk_buff *skb)
>>>>>>> upstream/android-13
{
	struct nfp_tun_req_route_ipv4 *payload;
	struct net_device *netdev;
	struct flowi4 flow = {};
	struct neighbour *n;
	struct rtable *rt;
	int err;

	payload = nfp_flower_cmsg_get_data(skb);

	rcu_read_lock();
<<<<<<< HEAD
	netdev = nfp_app_repr_get(app, be32_to_cpu(payload->ingress_port));
=======
	netdev = nfp_app_dev_get(app, be32_to_cpu(payload->ingress_port), NULL);
>>>>>>> upstream/android-13
	if (!netdev)
		goto fail_rcu_unlock;

	flow.daddr = payload->ipv4_addr;
	flow.flowi4_proto = IPPROTO_UDP;

#if IS_ENABLED(CONFIG_INET)
	/* Do a route lookup on same namespace as ingress port. */
	rt = ip_route_output_key(dev_net(netdev), &flow);
	err = PTR_ERR_OR_ZERO(rt);
	if (err)
		goto fail_rcu_unlock;
#else
	goto fail_rcu_unlock;
#endif

	/* Get the neighbour entry for the lookup */
	n = dst_neigh_lookup(&rt->dst, &flow.daddr);
	ip_rt_put(rt);
	if (!n)
		goto fail_rcu_unlock;
<<<<<<< HEAD
	nfp_tun_write_neigh(n->dev, app, &flow, n, GFP_ATOMIC);
=======
	nfp_tun_write_neigh_v4(n->dev, app, &flow, n, GFP_ATOMIC);
>>>>>>> upstream/android-13
	neigh_release(n);
	rcu_read_unlock();
	return;

fail_rcu_unlock:
	rcu_read_unlock();
	nfp_flower_cmsg_warn(app, "Requested route not found.\n");
}

<<<<<<< HEAD
=======
void nfp_tunnel_request_route_v6(struct nfp_app *app, struct sk_buff *skb)
{
	struct nfp_tun_req_route_ipv6 *payload;
	struct net_device *netdev;
	struct flowi6 flow = {};
	struct dst_entry *dst;
	struct neighbour *n;

	payload = nfp_flower_cmsg_get_data(skb);

	rcu_read_lock();
	netdev = nfp_app_dev_get(app, be32_to_cpu(payload->ingress_port), NULL);
	if (!netdev)
		goto fail_rcu_unlock;

	flow.daddr = payload->ipv6_addr;
	flow.flowi6_proto = IPPROTO_UDP;

#if IS_ENABLED(CONFIG_INET) && IS_ENABLED(CONFIG_IPV6)
	dst = ipv6_stub->ipv6_dst_lookup_flow(dev_net(netdev), NULL, &flow,
					      NULL);
	if (IS_ERR(dst))
		goto fail_rcu_unlock;
#else
	goto fail_rcu_unlock;
#endif

	n = dst_neigh_lookup(dst, &flow.daddr);
	dst_release(dst);
	if (!n)
		goto fail_rcu_unlock;

	nfp_tun_write_neigh_v6(n->dev, app, &flow, n, GFP_ATOMIC);
	neigh_release(n);
	rcu_read_unlock();
	return;

fail_rcu_unlock:
	rcu_read_unlock();
	nfp_flower_cmsg_warn(app, "Requested IPv6 route not found.\n");
}

>>>>>>> upstream/android-13
static void nfp_tun_write_ipv4_list(struct nfp_app *app)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv4_addr_entry *entry;
	struct nfp_tun_ipv4_addr payload;
	struct list_head *ptr, *storage;
	int count;

	memset(&payload, 0, sizeof(struct nfp_tun_ipv4_addr));
<<<<<<< HEAD
	mutex_lock(&priv->nfp_ipv4_off_lock);
	count = 0;
	list_for_each_safe(ptr, storage, &priv->nfp_ipv4_off_list) {
		if (count >= NFP_FL_IPV4_ADDRS_MAX) {
			mutex_unlock(&priv->nfp_ipv4_off_lock);
=======
	mutex_lock(&priv->tun.ipv4_off_lock);
	count = 0;
	list_for_each_safe(ptr, storage, &priv->tun.ipv4_off_list) {
		if (count >= NFP_FL_IPV4_ADDRS_MAX) {
			mutex_unlock(&priv->tun.ipv4_off_lock);
>>>>>>> upstream/android-13
			nfp_flower_cmsg_warn(app, "IPv4 offload exceeds limit.\n");
			return;
		}
		entry = list_entry(ptr, struct nfp_ipv4_addr_entry, list);
		payload.ipv4_addr[count++] = entry->ipv4_addr;
	}
	payload.count = cpu_to_be32(count);
<<<<<<< HEAD
	mutex_unlock(&priv->nfp_ipv4_off_lock);
=======
	mutex_unlock(&priv->tun.ipv4_off_lock);
>>>>>>> upstream/android-13

	nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_TUN_IPS,
				 sizeof(struct nfp_tun_ipv4_addr),
				 &payload, GFP_KERNEL);
}

void nfp_tunnel_add_ipv4_off(struct nfp_app *app, __be32 ipv4)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv4_addr_entry *entry;
	struct list_head *ptr, *storage;

<<<<<<< HEAD
	mutex_lock(&priv->nfp_ipv4_off_lock);
	list_for_each_safe(ptr, storage, &priv->nfp_ipv4_off_list) {
		entry = list_entry(ptr, struct nfp_ipv4_addr_entry, list);
		if (entry->ipv4_addr == ipv4) {
			entry->ref_count++;
			mutex_unlock(&priv->nfp_ipv4_off_lock);
=======
	mutex_lock(&priv->tun.ipv4_off_lock);
	list_for_each_safe(ptr, storage, &priv->tun.ipv4_off_list) {
		entry = list_entry(ptr, struct nfp_ipv4_addr_entry, list);
		if (entry->ipv4_addr == ipv4) {
			entry->ref_count++;
			mutex_unlock(&priv->tun.ipv4_off_lock);
>>>>>>> upstream/android-13
			return;
		}
	}

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
<<<<<<< HEAD
		mutex_unlock(&priv->nfp_ipv4_off_lock);
=======
		mutex_unlock(&priv->tun.ipv4_off_lock);
>>>>>>> upstream/android-13
		nfp_flower_cmsg_warn(app, "Mem error when offloading IP address.\n");
		return;
	}
	entry->ipv4_addr = ipv4;
	entry->ref_count = 1;
<<<<<<< HEAD
	list_add_tail(&entry->list, &priv->nfp_ipv4_off_list);
	mutex_unlock(&priv->nfp_ipv4_off_lock);
=======
	list_add_tail(&entry->list, &priv->tun.ipv4_off_list);
	mutex_unlock(&priv->tun.ipv4_off_lock);
>>>>>>> upstream/android-13

	nfp_tun_write_ipv4_list(app);
}

void nfp_tunnel_del_ipv4_off(struct nfp_app *app, __be32 ipv4)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv4_addr_entry *entry;
	struct list_head *ptr, *storage;

<<<<<<< HEAD
	mutex_lock(&priv->nfp_ipv4_off_lock);
	list_for_each_safe(ptr, storage, &priv->nfp_ipv4_off_list) {
=======
	mutex_lock(&priv->tun.ipv4_off_lock);
	list_for_each_safe(ptr, storage, &priv->tun.ipv4_off_list) {
>>>>>>> upstream/android-13
		entry = list_entry(ptr, struct nfp_ipv4_addr_entry, list);
		if (entry->ipv4_addr == ipv4) {
			entry->ref_count--;
			if (!entry->ref_count) {
				list_del(&entry->list);
				kfree(entry);
			}
			break;
		}
	}
<<<<<<< HEAD
	mutex_unlock(&priv->nfp_ipv4_off_lock);
=======
	mutex_unlock(&priv->tun.ipv4_off_lock);
>>>>>>> upstream/android-13

	nfp_tun_write_ipv4_list(app);
}

<<<<<<< HEAD
void nfp_tunnel_write_macs(struct nfp_app *app)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_tun_mac_offload_entry *entry;
	struct nfp_tun_mac_addr *payload;
	struct list_head *ptr, *storage;
	int mac_count, err, pay_size;

	mutex_lock(&priv->nfp_mac_off_lock);
	if (!priv->nfp_mac_off_count) {
		mutex_unlock(&priv->nfp_mac_off_lock);
		return;
	}

	pay_size = sizeof(struct nfp_tun_mac_addr) +
		   sizeof(struct index_mac_addr) * priv->nfp_mac_off_count;

	payload = kzalloc(pay_size, GFP_KERNEL);
	if (!payload) {
		mutex_unlock(&priv->nfp_mac_off_lock);
		return;
	}

	payload->count = cpu_to_be16(priv->nfp_mac_off_count);

	mac_count = 0;
	list_for_each_safe(ptr, storage, &priv->nfp_mac_off_list) {
		entry = list_entry(ptr, struct nfp_tun_mac_offload_entry,
				   list);
		payload->addresses[mac_count].index = entry->index;
		ether_addr_copy(payload->addresses[mac_count].addr,
				entry->addr);
		mac_count++;
	}

	err = nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_TUN_MAC,
				       pay_size, payload, GFP_KERNEL);

	kfree(payload);

	if (err) {
		mutex_unlock(&priv->nfp_mac_off_lock);
		/* Write failed so retain list for future retry. */
		return;
	}

	/* If list was successfully offloaded, flush it. */
	list_for_each_safe(ptr, storage, &priv->nfp_mac_off_list) {
		entry = list_entry(ptr, struct nfp_tun_mac_offload_entry,
				   list);
		list_del(&entry->list);
		kfree(entry);
	}

	priv->nfp_mac_off_count = 0;
	mutex_unlock(&priv->nfp_mac_off_lock);
}

static int nfp_tun_get_mac_idx(struct nfp_app *app, int ifindex)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_tun_mac_non_nfp_idx *entry;
	struct list_head *ptr, *storage;
	int idx;

	mutex_lock(&priv->nfp_mac_index_lock);
	list_for_each_safe(ptr, storage, &priv->nfp_mac_index_list) {
		entry = list_entry(ptr, struct nfp_tun_mac_non_nfp_idx, list);
		if (entry->ifindex == ifindex) {
			idx = entry->index;
			mutex_unlock(&priv->nfp_mac_index_lock);
			return idx;
		}
	}

	idx = ida_simple_get(&priv->nfp_mac_off_ids, 0,
			     NFP_MAX_MAC_INDEX, GFP_KERNEL);
	if (idx < 0) {
		mutex_unlock(&priv->nfp_mac_index_lock);
		return idx;
	}

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		mutex_unlock(&priv->nfp_mac_index_lock);
		return -ENOMEM;
	}
	entry->ifindex = ifindex;
	entry->index = idx;
	list_add_tail(&entry->list, &priv->nfp_mac_index_list);
	mutex_unlock(&priv->nfp_mac_index_lock);

	return idx;
}

static void nfp_tun_del_mac_idx(struct nfp_app *app, int ifindex)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_tun_mac_non_nfp_idx *entry;
	struct list_head *ptr, *storage;

	mutex_lock(&priv->nfp_mac_index_lock);
	list_for_each_safe(ptr, storage, &priv->nfp_mac_index_list) {
		entry = list_entry(ptr, struct nfp_tun_mac_non_nfp_idx, list);
		if (entry->ifindex == ifindex) {
			ida_simple_remove(&priv->nfp_mac_off_ids,
					  entry->index);
			list_del(&entry->list);
			kfree(entry);
			break;
		}
	}
	mutex_unlock(&priv->nfp_mac_index_lock);
}

static void nfp_tun_add_to_mac_offload_list(struct net_device *netdev,
					    struct nfp_app *app)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_tun_mac_offload_entry *entry;
	u16 nfp_mac_idx;
	int port = 0;

	/* Check if MAC should be offloaded. */
	if (!is_valid_ether_addr(netdev->dev_addr))
		return;

	if (nfp_netdev_is_nfp_repr(netdev))
		port = nfp_repr_get_port_id(netdev);
	else if (!nfp_tun_is_netdev_to_offload(netdev))
		return;

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		nfp_flower_cmsg_warn(app, "Mem fail when offloading MAC.\n");
		return;
	}

	if (FIELD_GET(NFP_FLOWER_CMSG_PORT_TYPE, port) ==
	    NFP_FLOWER_CMSG_PORT_TYPE_PHYS_PORT) {
		nfp_mac_idx = port << 8 | NFP_FLOWER_CMSG_PORT_TYPE_PHYS_PORT;
	} else if (FIELD_GET(NFP_FLOWER_CMSG_PORT_TYPE, port) ==
		   NFP_FLOWER_CMSG_PORT_TYPE_PCIE_PORT) {
		port = FIELD_GET(NFP_FLOWER_CMSG_PORT_VNIC, port);
		nfp_mac_idx = port << 8 | NFP_FLOWER_CMSG_PORT_TYPE_PCIE_PORT;
	} else {
		/* Must assign our own unique 8-bit index. */
		int idx = nfp_tun_get_mac_idx(app, netdev->ifindex);

		if (idx < 0) {
			nfp_flower_cmsg_warn(app, "Can't assign non-repr MAC index.\n");
			kfree(entry);
			return;
		}
		nfp_mac_idx = idx << 8 | NFP_FLOWER_CMSG_PORT_TYPE_OTHER_PORT;
	}

	entry->index = cpu_to_be16(nfp_mac_idx);
	ether_addr_copy(entry->addr, netdev->dev_addr);

	mutex_lock(&priv->nfp_mac_off_lock);
	priv->nfp_mac_off_count++;
	list_add_tail(&entry->list, &priv->nfp_mac_off_list);
	mutex_unlock(&priv->nfp_mac_off_lock);
}

static int nfp_tun_mac_event_handler(struct notifier_block *nb,
				     unsigned long event, void *ptr)
{
	struct nfp_flower_priv *app_priv;
	struct net_device *netdev;
	struct nfp_app *app;

	if (event == NETDEV_DOWN || event == NETDEV_UNREGISTER) {
		app_priv = container_of(nb, struct nfp_flower_priv,
					nfp_tun_mac_nb);
		app = app_priv->app;
		netdev = netdev_notifier_info_to_dev(ptr);

		/* If non-nfp netdev then free its offload index. */
		if (nfp_tun_is_netdev_to_offload(netdev))
			nfp_tun_del_mac_idx(app, netdev->ifindex);
	} else if (event == NETDEV_UP || event == NETDEV_CHANGEADDR ||
		   event == NETDEV_REGISTER) {
		app_priv = container_of(nb, struct nfp_flower_priv,
					nfp_tun_mac_nb);
		app = app_priv->app;
		netdev = netdev_notifier_info_to_dev(ptr);

		nfp_tun_add_to_mac_offload_list(netdev, app);

		/* Force a list write to keep NFP up to date. */
		nfp_tunnel_write_macs(app);
=======
static void nfp_tun_write_ipv6_list(struct nfp_app *app)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv6_addr_entry *entry;
	struct nfp_tun_ipv6_addr payload;
	int count = 0;

	memset(&payload, 0, sizeof(struct nfp_tun_ipv6_addr));
	mutex_lock(&priv->tun.ipv6_off_lock);
	list_for_each_entry(entry, &priv->tun.ipv6_off_list, list) {
		if (count >= NFP_FL_IPV6_ADDRS_MAX) {
			nfp_flower_cmsg_warn(app, "Too many IPv6 tunnel endpoint addresses, some cannot be offloaded.\n");
			break;
		}
		payload.ipv6_addr[count++] = entry->ipv6_addr;
	}
	mutex_unlock(&priv->tun.ipv6_off_lock);
	payload.count = cpu_to_be32(count);

	nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_TUN_IPS_V6,
				 sizeof(struct nfp_tun_ipv6_addr),
				 &payload, GFP_KERNEL);
}

struct nfp_ipv6_addr_entry *
nfp_tunnel_add_ipv6_off(struct nfp_app *app, struct in6_addr *ipv6)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv6_addr_entry *entry;

	mutex_lock(&priv->tun.ipv6_off_lock);
	list_for_each_entry(entry, &priv->tun.ipv6_off_list, list)
		if (!memcmp(&entry->ipv6_addr, ipv6, sizeof(*ipv6))) {
			entry->ref_count++;
			mutex_unlock(&priv->tun.ipv6_off_lock);
			return entry;
		}

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		mutex_unlock(&priv->tun.ipv6_off_lock);
		nfp_flower_cmsg_warn(app, "Mem error when offloading IP address.\n");
		return NULL;
	}
	entry->ipv6_addr = *ipv6;
	entry->ref_count = 1;
	list_add_tail(&entry->list, &priv->tun.ipv6_off_list);
	mutex_unlock(&priv->tun.ipv6_off_lock);

	nfp_tun_write_ipv6_list(app);

	return entry;
}

void
nfp_tunnel_put_ipv6_off(struct nfp_app *app, struct nfp_ipv6_addr_entry *entry)
{
	struct nfp_flower_priv *priv = app->priv;
	bool freed = false;

	mutex_lock(&priv->tun.ipv6_off_lock);
	if (!--entry->ref_count) {
		list_del(&entry->list);
		kfree(entry);
		freed = true;
	}
	mutex_unlock(&priv->tun.ipv6_off_lock);

	if (freed)
		nfp_tun_write_ipv6_list(app);
}

static int
__nfp_tunnel_offload_mac(struct nfp_app *app, u8 *mac, u16 idx, bool del)
{
	struct nfp_tun_mac_addr_offload payload;

	memset(&payload, 0, sizeof(payload));

	if (del)
		payload.flags = cpu_to_be16(NFP_TUN_MAC_OFFLOAD_DEL_FLAG);

	/* FW supports multiple MACs per cmsg but restrict to single. */
	payload.count = cpu_to_be16(1);
	payload.index = cpu_to_be16(idx);
	ether_addr_copy(payload.addr, mac);

	return nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_TUN_MAC,
					sizeof(struct nfp_tun_mac_addr_offload),
					&payload, GFP_KERNEL);
}

static bool nfp_tunnel_port_is_phy_repr(int port)
{
	if (FIELD_GET(NFP_FLOWER_CMSG_PORT_TYPE, port) ==
	    NFP_FLOWER_CMSG_PORT_TYPE_PHYS_PORT)
		return true;

	return false;
}

static u16 nfp_tunnel_get_mac_idx_from_phy_port_id(int port)
{
	return port << 8 | NFP_FLOWER_CMSG_PORT_TYPE_PHYS_PORT;
}

static u16 nfp_tunnel_get_global_mac_idx_from_ida(int id)
{
	return id << 8 | NFP_FLOWER_CMSG_PORT_TYPE_OTHER_PORT;
}

static int nfp_tunnel_get_ida_from_global_mac_idx(u16 nfp_mac_idx)
{
	return nfp_mac_idx >> 8;
}

static bool nfp_tunnel_is_mac_idx_global(u16 nfp_mac_idx)
{
	return (nfp_mac_idx & 0xff) == NFP_FLOWER_CMSG_PORT_TYPE_OTHER_PORT;
}

static struct nfp_tun_offloaded_mac *
nfp_tunnel_lookup_offloaded_macs(struct nfp_app *app, u8 *mac)
{
	struct nfp_flower_priv *priv = app->priv;

	return rhashtable_lookup_fast(&priv->tun.offloaded_macs, mac,
				      offloaded_macs_params);
}

static void
nfp_tunnel_offloaded_macs_inc_ref_and_link(struct nfp_tun_offloaded_mac *entry,
					   struct net_device *netdev, bool mod)
{
	if (nfp_netdev_is_nfp_repr(netdev)) {
		struct nfp_flower_repr_priv *repr_priv;
		struct nfp_repr *repr;

		repr = netdev_priv(netdev);
		repr_priv = repr->app_priv;

		/* If modifing MAC, remove repr from old list first. */
		if (mod)
			list_del(&repr_priv->mac_list);

		list_add_tail(&repr_priv->mac_list, &entry->repr_list);
	} else if (nfp_flower_is_supported_bridge(netdev)) {
		entry->bridge_count++;
	}

	entry->ref_count++;
}

static int
nfp_tunnel_add_shared_mac(struct nfp_app *app, struct net_device *netdev,
			  int port, bool mod)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_tun_offloaded_mac *entry;
	int ida_idx = -1, err;
	u16 nfp_mac_idx = 0;

	entry = nfp_tunnel_lookup_offloaded_macs(app, netdev->dev_addr);
	if (entry && nfp_tunnel_is_mac_idx_global(entry->index)) {
		if (entry->bridge_count ||
		    !nfp_flower_is_supported_bridge(netdev)) {
			nfp_tunnel_offloaded_macs_inc_ref_and_link(entry,
								   netdev, mod);
			return 0;
		}

		/* MAC is global but matches need to go to pre_tun table. */
		nfp_mac_idx = entry->index | NFP_TUN_PRE_TUN_IDX_BIT;
	}

	if (!nfp_mac_idx) {
		/* Assign a global index if non-repr or MAC is now shared. */
		if (entry || !port) {
			ida_idx = ida_simple_get(&priv->tun.mac_off_ids, 0,
						 NFP_MAX_MAC_INDEX, GFP_KERNEL);
			if (ida_idx < 0)
				return ida_idx;

			nfp_mac_idx =
				nfp_tunnel_get_global_mac_idx_from_ida(ida_idx);

			if (nfp_flower_is_supported_bridge(netdev))
				nfp_mac_idx |= NFP_TUN_PRE_TUN_IDX_BIT;

		} else {
			nfp_mac_idx =
				nfp_tunnel_get_mac_idx_from_phy_port_id(port);
		}
	}

	if (!entry) {
		entry = kzalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry) {
			err = -ENOMEM;
			goto err_free_ida;
		}

		ether_addr_copy(entry->addr, netdev->dev_addr);
		INIT_LIST_HEAD(&entry->repr_list);

		if (rhashtable_insert_fast(&priv->tun.offloaded_macs,
					   &entry->ht_node,
					   offloaded_macs_params)) {
			err = -ENOMEM;
			goto err_free_entry;
		}
	}

	err = __nfp_tunnel_offload_mac(app, netdev->dev_addr,
				       nfp_mac_idx, false);
	if (err) {
		/* If not shared then free. */
		if (!entry->ref_count)
			goto err_remove_hash;
		goto err_free_ida;
	}

	entry->index = nfp_mac_idx;
	nfp_tunnel_offloaded_macs_inc_ref_and_link(entry, netdev, mod);

	return 0;

err_remove_hash:
	rhashtable_remove_fast(&priv->tun.offloaded_macs, &entry->ht_node,
			       offloaded_macs_params);
err_free_entry:
	kfree(entry);
err_free_ida:
	if (ida_idx != -1)
		ida_simple_remove(&priv->tun.mac_off_ids, ida_idx);

	return err;
}

static int
nfp_tunnel_del_shared_mac(struct nfp_app *app, struct net_device *netdev,
			  u8 *mac, bool mod)
{
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_flower_repr_priv *repr_priv;
	struct nfp_tun_offloaded_mac *entry;
	struct nfp_repr *repr;
	u16 nfp_mac_idx;
	int ida_idx;

	entry = nfp_tunnel_lookup_offloaded_macs(app, mac);
	if (!entry)
		return 0;

	entry->ref_count--;
	/* If del is part of a mod then mac_list is still in use elsewheree. */
	if (nfp_netdev_is_nfp_repr(netdev) && !mod) {
		repr = netdev_priv(netdev);
		repr_priv = repr->app_priv;
		list_del(&repr_priv->mac_list);
	}

	if (nfp_flower_is_supported_bridge(netdev)) {
		entry->bridge_count--;

		if (!entry->bridge_count && entry->ref_count) {
			nfp_mac_idx = entry->index & ~NFP_TUN_PRE_TUN_IDX_BIT;
			if (__nfp_tunnel_offload_mac(app, mac, nfp_mac_idx,
						     false)) {
				nfp_flower_cmsg_warn(app, "MAC offload index revert failed on %s.\n",
						     netdev_name(netdev));
				return 0;
			}

			entry->index = nfp_mac_idx;
			return 0;
		}
	}

	/* If MAC is now used by 1 repr set the offloaded MAC index to port. */
	if (entry->ref_count == 1 && list_is_singular(&entry->repr_list)) {
		int port, err;

		repr_priv = list_first_entry(&entry->repr_list,
					     struct nfp_flower_repr_priv,
					     mac_list);
		repr = repr_priv->nfp_repr;
		port = nfp_repr_get_port_id(repr->netdev);
		nfp_mac_idx = nfp_tunnel_get_mac_idx_from_phy_port_id(port);
		err = __nfp_tunnel_offload_mac(app, mac, nfp_mac_idx, false);
		if (err) {
			nfp_flower_cmsg_warn(app, "MAC offload index revert failed on %s.\n",
					     netdev_name(netdev));
			return 0;
		}

		ida_idx = nfp_tunnel_get_ida_from_global_mac_idx(entry->index);
		ida_simple_remove(&priv->tun.mac_off_ids, ida_idx);
		entry->index = nfp_mac_idx;
		return 0;
	}

	if (entry->ref_count)
		return 0;

	WARN_ON_ONCE(rhashtable_remove_fast(&priv->tun.offloaded_macs,
					    &entry->ht_node,
					    offloaded_macs_params));

	if (nfp_flower_is_supported_bridge(netdev))
		nfp_mac_idx = entry->index & ~NFP_TUN_PRE_TUN_IDX_BIT;
	else
		nfp_mac_idx = entry->index;

	/* If MAC has global ID then extract and free the ida entry. */
	if (nfp_tunnel_is_mac_idx_global(nfp_mac_idx)) {
		ida_idx = nfp_tunnel_get_ida_from_global_mac_idx(entry->index);
		ida_simple_remove(&priv->tun.mac_off_ids, ida_idx);
	}

	kfree(entry);

	return __nfp_tunnel_offload_mac(app, mac, 0, true);
}

static int
nfp_tunnel_offload_mac(struct nfp_app *app, struct net_device *netdev,
		       enum nfp_flower_mac_offload_cmd cmd)
{
	struct nfp_flower_non_repr_priv *nr_priv = NULL;
	bool non_repr = false, *mac_offloaded;
	u8 *off_mac = NULL;
	int err, port = 0;

	if (nfp_netdev_is_nfp_repr(netdev)) {
		struct nfp_flower_repr_priv *repr_priv;
		struct nfp_repr *repr;

		repr = netdev_priv(netdev);
		if (repr->app != app)
			return 0;

		repr_priv = repr->app_priv;
		if (repr_priv->on_bridge)
			return 0;

		mac_offloaded = &repr_priv->mac_offloaded;
		off_mac = &repr_priv->offloaded_mac_addr[0];
		port = nfp_repr_get_port_id(netdev);
		if (!nfp_tunnel_port_is_phy_repr(port))
			return 0;
	} else if (nfp_fl_is_netdev_to_offload(netdev)) {
		nr_priv = nfp_flower_non_repr_priv_get(app, netdev);
		if (!nr_priv)
			return -ENOMEM;

		mac_offloaded = &nr_priv->mac_offloaded;
		off_mac = &nr_priv->offloaded_mac_addr[0];
		non_repr = true;
	} else {
		return 0;
	}

	if (!is_valid_ether_addr(netdev->dev_addr)) {
		err = -EINVAL;
		goto err_put_non_repr_priv;
	}

	if (cmd == NFP_TUNNEL_MAC_OFFLOAD_MOD && !*mac_offloaded)
		cmd = NFP_TUNNEL_MAC_OFFLOAD_ADD;

	switch (cmd) {
	case NFP_TUNNEL_MAC_OFFLOAD_ADD:
		err = nfp_tunnel_add_shared_mac(app, netdev, port, false);
		if (err)
			goto err_put_non_repr_priv;

		if (non_repr)
			__nfp_flower_non_repr_priv_get(nr_priv);

		*mac_offloaded = true;
		ether_addr_copy(off_mac, netdev->dev_addr);
		break;
	case NFP_TUNNEL_MAC_OFFLOAD_DEL:
		/* Only attempt delete if add was successful. */
		if (!*mac_offloaded)
			break;

		if (non_repr)
			__nfp_flower_non_repr_priv_put(nr_priv);

		*mac_offloaded = false;

		err = nfp_tunnel_del_shared_mac(app, netdev, netdev->dev_addr,
						false);
		if (err)
			goto err_put_non_repr_priv;

		break;
	case NFP_TUNNEL_MAC_OFFLOAD_MOD:
		/* Ignore if changing to the same address. */
		if (ether_addr_equal(netdev->dev_addr, off_mac))
			break;

		err = nfp_tunnel_add_shared_mac(app, netdev, port, true);
		if (err)
			goto err_put_non_repr_priv;

		/* Delete the previous MAC address. */
		err = nfp_tunnel_del_shared_mac(app, netdev, off_mac, true);
		if (err)
			nfp_flower_cmsg_warn(app, "Failed to remove offload of replaced MAC addr on %s.\n",
					     netdev_name(netdev));

		ether_addr_copy(off_mac, netdev->dev_addr);
		break;
	default:
		err = -EINVAL;
		goto err_put_non_repr_priv;
	}

	if (non_repr)
		__nfp_flower_non_repr_priv_put(nr_priv);

	return 0;

err_put_non_repr_priv:
	if (non_repr)
		__nfp_flower_non_repr_priv_put(nr_priv);

	return err;
}

int nfp_tunnel_mac_event_handler(struct nfp_app *app,
				 struct net_device *netdev,
				 unsigned long event, void *ptr)
{
	int err;

	if (event == NETDEV_DOWN) {
		err = nfp_tunnel_offload_mac(app, netdev,
					     NFP_TUNNEL_MAC_OFFLOAD_DEL);
		if (err)
			nfp_flower_cmsg_warn(app, "Failed to delete offload MAC on %s.\n",
					     netdev_name(netdev));
	} else if (event == NETDEV_UP) {
		err = nfp_tunnel_offload_mac(app, netdev,
					     NFP_TUNNEL_MAC_OFFLOAD_ADD);
		if (err)
			nfp_flower_cmsg_warn(app, "Failed to offload MAC on %s.\n",
					     netdev_name(netdev));
	} else if (event == NETDEV_CHANGEADDR) {
		/* Only offload addr change if netdev is already up. */
		if (!(netdev->flags & IFF_UP))
			return NOTIFY_OK;

		err = nfp_tunnel_offload_mac(app, netdev,
					     NFP_TUNNEL_MAC_OFFLOAD_MOD);
		if (err)
			nfp_flower_cmsg_warn(app, "Failed to offload MAC change on %s.\n",
					     netdev_name(netdev));
	} else if (event == NETDEV_CHANGEUPPER) {
		/* If a repr is attached to a bridge then tunnel packets
		 * entering the physical port are directed through the bridge
		 * datapath and cannot be directly detunneled. Therefore,
		 * associated offloaded MACs and indexes should not be used
		 * by fw for detunneling.
		 */
		struct netdev_notifier_changeupper_info *info = ptr;
		struct net_device *upper = info->upper_dev;
		struct nfp_flower_repr_priv *repr_priv;
		struct nfp_repr *repr;

		if (!nfp_netdev_is_nfp_repr(netdev) ||
		    !nfp_flower_is_supported_bridge(upper))
			return NOTIFY_OK;

		repr = netdev_priv(netdev);
		if (repr->app != app)
			return NOTIFY_OK;

		repr_priv = repr->app_priv;

		if (info->linking) {
			if (nfp_tunnel_offload_mac(app, netdev,
						   NFP_TUNNEL_MAC_OFFLOAD_DEL))
				nfp_flower_cmsg_warn(app, "Failed to delete offloaded MAC on %s.\n",
						     netdev_name(netdev));
			repr_priv->on_bridge = true;
		} else {
			repr_priv->on_bridge = false;

			if (!(netdev->flags & IFF_UP))
				return NOTIFY_OK;

			if (nfp_tunnel_offload_mac(app, netdev,
						   NFP_TUNNEL_MAC_OFFLOAD_ADD))
				nfp_flower_cmsg_warn(app, "Failed to offload MAC on %s.\n",
						     netdev_name(netdev));
		}
>>>>>>> upstream/android-13
	}
	return NOTIFY_OK;
}

<<<<<<< HEAD
int nfp_tunnel_config_start(struct nfp_app *app)
{
	struct nfp_flower_priv *priv = app->priv;
	struct net_device *netdev;
	int err;

	/* Initialise priv data for MAC offloading. */
	priv->nfp_mac_off_count = 0;
	mutex_init(&priv->nfp_mac_off_lock);
	INIT_LIST_HEAD(&priv->nfp_mac_off_list);
	priv->nfp_tun_mac_nb.notifier_call = nfp_tun_mac_event_handler;
	mutex_init(&priv->nfp_mac_index_lock);
	INIT_LIST_HEAD(&priv->nfp_mac_index_list);
	ida_init(&priv->nfp_mac_off_ids);

	/* Initialise priv data for IPv4 offloading. */
	mutex_init(&priv->nfp_ipv4_off_lock);
	INIT_LIST_HEAD(&priv->nfp_ipv4_off_list);

	/* Initialise priv data for neighbour offloading. */
	spin_lock_init(&priv->nfp_neigh_off_lock);
	INIT_LIST_HEAD(&priv->nfp_neigh_off_list);
	priv->nfp_tun_neigh_nb.notifier_call = nfp_tun_neigh_event_handler;

	err = register_netdevice_notifier(&priv->nfp_tun_mac_nb);
	if (err)
		goto err_free_mac_ida;

	err = register_netevent_notifier(&priv->nfp_tun_neigh_nb);
	if (err)
		goto err_unreg_mac_nb;

	/* Parse netdevs already registered for MACs that need offloaded. */
	rtnl_lock();
	for_each_netdev(&init_net, netdev)
		nfp_tun_add_to_mac_offload_list(netdev, app);
	rtnl_unlock();

	return 0;

err_unreg_mac_nb:
	unregister_netdevice_notifier(&priv->nfp_tun_mac_nb);
err_free_mac_ida:
	ida_destroy(&priv->nfp_mac_off_ids);
	return err;
=======
int nfp_flower_xmit_pre_tun_flow(struct nfp_app *app,
				 struct nfp_fl_payload *flow)
{
	struct nfp_flower_priv *app_priv = app->priv;
	struct nfp_tun_offloaded_mac *mac_entry;
	struct nfp_flower_meta_tci *key_meta;
	struct nfp_tun_pre_tun_rule payload;
	struct net_device *internal_dev;
	int err;

	if (app_priv->pre_tun_rule_cnt == NFP_TUN_PRE_TUN_RULE_LIMIT)
		return -ENOSPC;

	memset(&payload, 0, sizeof(struct nfp_tun_pre_tun_rule));

	internal_dev = flow->pre_tun_rule.dev;
	payload.vlan_tci = flow->pre_tun_rule.vlan_tci;
	payload.host_ctx_id = flow->meta.host_ctx_id;

	/* Lookup MAC index for the pre-tunnel rule egress device.
	 * Note that because the device is always an internal port, it will
	 * have a constant global index so does not need to be tracked.
	 */
	mac_entry = nfp_tunnel_lookup_offloaded_macs(app,
						     internal_dev->dev_addr);
	if (!mac_entry)
		return -ENOENT;

	/* Set/clear IPV6 bit. cpu_to_be16() swap will lead to MSB being
	 * set/clear for port_idx.
	 */
	key_meta = (struct nfp_flower_meta_tci *)flow->unmasked_data;
	if (key_meta->nfp_flow_key_layer & NFP_FLOWER_LAYER_IPV6)
		mac_entry->index |= NFP_TUN_PRE_TUN_IPV6_BIT;
	else
		mac_entry->index &= ~NFP_TUN_PRE_TUN_IPV6_BIT;

	payload.port_idx = cpu_to_be16(mac_entry->index);

	/* Copy mac id and vlan to flow - dev may not exist at delete time. */
	flow->pre_tun_rule.vlan_tci = payload.vlan_tci;
	flow->pre_tun_rule.port_idx = payload.port_idx;

	err = nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_PRE_TUN_RULE,
				       sizeof(struct nfp_tun_pre_tun_rule),
				       (unsigned char *)&payload, GFP_KERNEL);
	if (err)
		return err;

	app_priv->pre_tun_rule_cnt++;

	return 0;
}

int nfp_flower_xmit_pre_tun_del_flow(struct nfp_app *app,
				     struct nfp_fl_payload *flow)
{
	struct nfp_flower_priv *app_priv = app->priv;
	struct nfp_tun_pre_tun_rule payload;
	u32 tmp_flags = 0;
	int err;

	memset(&payload, 0, sizeof(struct nfp_tun_pre_tun_rule));

	tmp_flags |= NFP_TUN_PRE_TUN_RULE_DEL;
	payload.flags = cpu_to_be32(tmp_flags);
	payload.vlan_tci = flow->pre_tun_rule.vlan_tci;
	payload.port_idx = flow->pre_tun_rule.port_idx;

	err = nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_PRE_TUN_RULE,
				       sizeof(struct nfp_tun_pre_tun_rule),
				       (unsigned char *)&payload, GFP_KERNEL);
	if (err)
		return err;

	app_priv->pre_tun_rule_cnt--;

	return 0;
}

int nfp_tunnel_config_start(struct nfp_app *app)
{
	struct nfp_flower_priv *priv = app->priv;
	int err;

	/* Initialise rhash for MAC offload tracking. */
	err = rhashtable_init(&priv->tun.offloaded_macs,
			      &offloaded_macs_params);
	if (err)
		return err;

	ida_init(&priv->tun.mac_off_ids);

	/* Initialise priv data for IPv4/v6 offloading. */
	mutex_init(&priv->tun.ipv4_off_lock);
	INIT_LIST_HEAD(&priv->tun.ipv4_off_list);
	mutex_init(&priv->tun.ipv6_off_lock);
	INIT_LIST_HEAD(&priv->tun.ipv6_off_list);

	/* Initialise priv data for neighbour offloading. */
	spin_lock_init(&priv->tun.neigh_off_lock_v4);
	INIT_LIST_HEAD(&priv->tun.neigh_off_list_v4);
	spin_lock_init(&priv->tun.neigh_off_lock_v6);
	INIT_LIST_HEAD(&priv->tun.neigh_off_list_v6);
	priv->tun.neigh_nb.notifier_call = nfp_tun_neigh_event_handler;

	err = register_netevent_notifier(&priv->tun.neigh_nb);
	if (err) {
		rhashtable_free_and_destroy(&priv->tun.offloaded_macs,
					    nfp_check_rhashtable_empty, NULL);
		return err;
	}

	return 0;
>>>>>>> upstream/android-13
}

void nfp_tunnel_config_stop(struct nfp_app *app)
{
<<<<<<< HEAD
	struct nfp_tun_mac_offload_entry *mac_entry;
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv4_route_entry *route_entry;
	struct nfp_tun_mac_non_nfp_idx *mac_idx;
	struct nfp_ipv4_addr_entry *ip_entry;
	struct list_head *ptr, *storage;

	unregister_netdevice_notifier(&priv->nfp_tun_mac_nb);
	unregister_netevent_notifier(&priv->nfp_tun_neigh_nb);

	/* Free any memory that may be occupied by MAC list. */
	list_for_each_safe(ptr, storage, &priv->nfp_mac_off_list) {
		mac_entry = list_entry(ptr, struct nfp_tun_mac_offload_entry,
				       list);
		list_del(&mac_entry->list);
		kfree(mac_entry);
	}

	/* Free any memory that may be occupied by MAC index list. */
	list_for_each_safe(ptr, storage, &priv->nfp_mac_index_list) {
		mac_idx = list_entry(ptr, struct nfp_tun_mac_non_nfp_idx,
				     list);
		list_del(&mac_idx->list);
		kfree(mac_idx);
	}

	ida_destroy(&priv->nfp_mac_off_ids);

	/* Free any memory that may be occupied by ipv4 list. */
	list_for_each_safe(ptr, storage, &priv->nfp_ipv4_off_list) {
=======
	struct nfp_offloaded_route *route_entry, *temp;
	struct nfp_flower_priv *priv = app->priv;
	struct nfp_ipv4_addr_entry *ip_entry;
	struct nfp_tun_neigh_v6 ipv6_route;
	struct nfp_tun_neigh ipv4_route;
	struct list_head *ptr, *storage;

	unregister_netevent_notifier(&priv->tun.neigh_nb);

	ida_destroy(&priv->tun.mac_off_ids);

	/* Free any memory that may be occupied by ipv4 list. */
	list_for_each_safe(ptr, storage, &priv->tun.ipv4_off_list) {
>>>>>>> upstream/android-13
		ip_entry = list_entry(ptr, struct nfp_ipv4_addr_entry, list);
		list_del(&ip_entry->list);
		kfree(ip_entry);
	}

<<<<<<< HEAD
	/* Free any memory that may be occupied by the route list. */
	list_for_each_safe(ptr, storage, &priv->nfp_neigh_off_list) {
		route_entry = list_entry(ptr, struct nfp_ipv4_route_entry,
					 list);
		list_del(&route_entry->list);
		kfree(route_entry);
	}
=======
	mutex_destroy(&priv->tun.ipv6_off_lock);

	/* Free memory in the route list and remove entries from fw cache. */
	list_for_each_entry_safe(route_entry, temp,
				 &priv->tun.neigh_off_list_v4, list) {
		memset(&ipv4_route, 0, sizeof(ipv4_route));
		memcpy(&ipv4_route.dst_ipv4, &route_entry->ip_add,
		       sizeof(ipv4_route.dst_ipv4));
		list_del(&route_entry->list);
		kfree(route_entry);

		nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_TUN_NEIGH,
					 sizeof(struct nfp_tun_neigh),
					 (unsigned char *)&ipv4_route,
					 GFP_KERNEL);
	}

	list_for_each_entry_safe(route_entry, temp,
				 &priv->tun.neigh_off_list_v6, list) {
		memset(&ipv6_route, 0, sizeof(ipv6_route));
		memcpy(&ipv6_route.dst_ipv6, &route_entry->ip_add,
		       sizeof(ipv6_route.dst_ipv6));
		list_del(&route_entry->list);
		kfree(route_entry);

		nfp_flower_xmit_tun_conf(app, NFP_FLOWER_CMSG_TYPE_TUN_NEIGH_V6,
					 sizeof(struct nfp_tun_neigh),
					 (unsigned char *)&ipv6_route,
					 GFP_KERNEL);
	}

	/* Destroy rhash. Entries should be cleaned on netdev notifier unreg. */
	rhashtable_free_and_destroy(&priv->tun.offloaded_macs,
				    nfp_check_rhashtable_empty, NULL);
>>>>>>> upstream/android-13
}
