/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2011-2018  B.A.T.M.A.N. contributors:
 *
 * Antonio Quartulli
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
 * Antonio Quartulli
>>>>>>> upstream/android-13
 */

#ifndef _NET_BATMAN_ADV_DISTRIBUTED_ARP_TABLE_H_
#define _NET_BATMAN_ADV_DISTRIBUTED_ARP_TABLE_H_

#include "main.h"

#include <linux/compiler.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
=======
#include <linux/netlink.h>
#include <linux/skbuff.h>
>>>>>>> upstream/android-13
#include <linux/types.h>
#include <uapi/linux/batadv_packet.h>

#include "originator.h"

<<<<<<< HEAD
struct netlink_callback;
struct seq_file;
struct sk_buff;

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_BATMAN_ADV_DAT

/* BATADV_DAT_ADDR_MAX - maximum address value in the DHT space */
#define BATADV_DAT_ADDR_MAX ((batadv_dat_addr_t)~(batadv_dat_addr_t)0)

void batadv_dat_status_update(struct net_device *net_dev);
bool batadv_dat_snoop_outgoing_arp_request(struct batadv_priv *bat_priv,
					   struct sk_buff *skb);
bool batadv_dat_snoop_incoming_arp_request(struct batadv_priv *bat_priv,
					   struct sk_buff *skb, int hdr_size);
void batadv_dat_snoop_outgoing_arp_reply(struct batadv_priv *bat_priv,
					 struct sk_buff *skb);
bool batadv_dat_snoop_incoming_arp_reply(struct batadv_priv *bat_priv,
					 struct sk_buff *skb, int hdr_size);
<<<<<<< HEAD
=======
void batadv_dat_snoop_outgoing_dhcp_ack(struct batadv_priv *bat_priv,
					struct sk_buff *skb,
					__be16 proto,
					unsigned short vid);
void batadv_dat_snoop_incoming_dhcp_ack(struct batadv_priv *bat_priv,
					struct sk_buff *skb, int hdr_size);
>>>>>>> upstream/android-13
bool batadv_dat_drop_broadcast_packet(struct batadv_priv *bat_priv,
				      struct batadv_forw_packet *forw_packet);

/**
 * batadv_dat_init_orig_node_addr() - assign a DAT address to the orig_node
 * @orig_node: the node to assign the DAT address to
 */
static inline void
batadv_dat_init_orig_node_addr(struct batadv_orig_node *orig_node)
{
	u32 addr;

	addr = batadv_choose_orig(orig_node->orig, BATADV_DAT_ADDR_MAX);
	orig_node->dat_addr = (batadv_dat_addr_t)addr;
}

/**
 * batadv_dat_init_own_addr() - assign a DAT address to the node itself
 * @bat_priv: the bat priv with all the soft interface information
 * @primary_if: a pointer to the primary interface
 */
static inline void
batadv_dat_init_own_addr(struct batadv_priv *bat_priv,
			 struct batadv_hard_iface *primary_if)
{
	u32 addr;

	addr = batadv_choose_orig(primary_if->net_dev->dev_addr,
				  BATADV_DAT_ADDR_MAX);

	bat_priv->dat.addr = (batadv_dat_addr_t)addr;
}

int batadv_dat_init(struct batadv_priv *bat_priv);
void batadv_dat_free(struct batadv_priv *bat_priv);
<<<<<<< HEAD
int batadv_dat_cache_seq_print_text(struct seq_file *seq, void *offset);
=======
>>>>>>> upstream/android-13
int batadv_dat_cache_dump(struct sk_buff *msg, struct netlink_callback *cb);

/**
 * batadv_dat_inc_counter() - increment the correct DAT packet counter
 * @bat_priv: the bat priv with all the soft interface information
 * @subtype: the 4addr subtype of the packet to be counted
 *
 * Updates the ethtool statistics for the received packet if it is a DAT subtype
 */
static inline void batadv_dat_inc_counter(struct batadv_priv *bat_priv,
					  u8 subtype)
{
	switch (subtype) {
	case BATADV_P_DAT_DHT_GET:
		batadv_inc_counter(bat_priv,
				   BATADV_CNT_DAT_GET_RX);
		break;
	case BATADV_P_DAT_DHT_PUT:
		batadv_inc_counter(bat_priv,
				   BATADV_CNT_DAT_PUT_RX);
		break;
	}
}

#else

static inline void batadv_dat_status_update(struct net_device *net_dev)
{
}

static inline bool
batadv_dat_snoop_outgoing_arp_request(struct batadv_priv *bat_priv,
				      struct sk_buff *skb)
{
	return false;
}

static inline bool
batadv_dat_snoop_incoming_arp_request(struct batadv_priv *bat_priv,
				      struct sk_buff *skb, int hdr_size)
{
	return false;
}

static inline bool
batadv_dat_snoop_outgoing_arp_reply(struct batadv_priv *bat_priv,
				    struct sk_buff *skb)
{
	return false;
}

static inline bool
batadv_dat_snoop_incoming_arp_reply(struct batadv_priv *bat_priv,
				    struct sk_buff *skb, int hdr_size)
{
	return false;
}

<<<<<<< HEAD
=======
static inline void
batadv_dat_snoop_outgoing_dhcp_ack(struct batadv_priv *bat_priv,
				   struct sk_buff *skb, __be16 proto,
				   unsigned short vid)
{
}

static inline void
batadv_dat_snoop_incoming_dhcp_ack(struct batadv_priv *bat_priv,
				   struct sk_buff *skb, int hdr_size)
{
}

>>>>>>> upstream/android-13
static inline bool
batadv_dat_drop_broadcast_packet(struct batadv_priv *bat_priv,
				 struct batadv_forw_packet *forw_packet)
{
	return false;
}

static inline void
batadv_dat_init_orig_node_addr(struct batadv_orig_node *orig_node)
{
}

static inline void batadv_dat_init_own_addr(struct batadv_priv *bat_priv,
					    struct batadv_hard_iface *iface)
{
}

<<<<<<< HEAD
static inline void batadv_arp_change_timeout(struct net_device *soft_iface,
					     const char *name)
{
}

=======
>>>>>>> upstream/android-13
static inline int batadv_dat_init(struct batadv_priv *bat_priv)
{
	return 0;
}

static inline void batadv_dat_free(struct batadv_priv *bat_priv)
{
}

static inline int
batadv_dat_cache_dump(struct sk_buff *msg, struct netlink_callback *cb)
{
	return -EOPNOTSUPP;
}

static inline void batadv_dat_inc_counter(struct batadv_priv *bat_priv,
					  u8 subtype)
{
}

#endif /* CONFIG_BATMAN_ADV_DAT */

#endif /* _NET_BATMAN_ADV_DISTRIBUTED_ARP_TABLE_H_ */
