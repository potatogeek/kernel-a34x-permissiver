/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2007-2018  B.A.T.M.A.N. contributors:
 *
 * Marek Lindner, Simon Wunderlich
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
 * Marek Lindner, Simon Wunderlich
>>>>>>> upstream/android-13
 */

#ifndef _NET_BATMAN_ADV_ORIGINATOR_H_
#define _NET_BATMAN_ADV_ORIGINATOR_H_

#include "main.h"

#include <linux/compiler.h>
#include <linux/if_ether.h>
#include <linux/jhash.h>
<<<<<<< HEAD
#include <linux/types.h>

struct netlink_callback;
struct seq_file;
struct sk_buff;

=======
#include <linux/kref.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/types.h>

>>>>>>> upstream/android-13
bool batadv_compare_orig(const struct hlist_node *node, const void *data2);
int batadv_originator_init(struct batadv_priv *bat_priv);
void batadv_originator_free(struct batadv_priv *bat_priv);
void batadv_purge_orig_ref(struct batadv_priv *bat_priv);
<<<<<<< HEAD
void batadv_orig_node_put(struct batadv_orig_node *orig_node);
=======
void batadv_orig_node_release(struct kref *ref);
>>>>>>> upstream/android-13
struct batadv_orig_node *batadv_orig_node_new(struct batadv_priv *bat_priv,
					      const u8 *addr);
struct batadv_hardif_neigh_node *
batadv_hardif_neigh_get(const struct batadv_hard_iface *hard_iface,
			const u8 *neigh_addr);
<<<<<<< HEAD
void
batadv_hardif_neigh_put(struct batadv_hardif_neigh_node *hardif_neigh);
=======
void batadv_hardif_neigh_release(struct kref *ref);
>>>>>>> upstream/android-13
struct batadv_neigh_node *
batadv_neigh_node_get_or_create(struct batadv_orig_node *orig_node,
				struct batadv_hard_iface *hard_iface,
				const u8 *neigh_addr);
<<<<<<< HEAD
void batadv_neigh_node_put(struct batadv_neigh_node *neigh_node);
=======
void batadv_neigh_node_release(struct kref *ref);
>>>>>>> upstream/android-13
struct batadv_neigh_node *
batadv_orig_router_get(struct batadv_orig_node *orig_node,
		       const struct batadv_hard_iface *if_outgoing);
struct batadv_neigh_ifinfo *
batadv_neigh_ifinfo_new(struct batadv_neigh_node *neigh,
			struct batadv_hard_iface *if_outgoing);
struct batadv_neigh_ifinfo *
batadv_neigh_ifinfo_get(struct batadv_neigh_node *neigh,
			struct batadv_hard_iface *if_outgoing);
<<<<<<< HEAD
void batadv_neigh_ifinfo_put(struct batadv_neigh_ifinfo *neigh_ifinfo);

int batadv_hardif_neigh_dump(struct sk_buff *msg, struct netlink_callback *cb);
int batadv_hardif_neigh_seq_print_text(struct seq_file *seq, void *offset);
=======
void batadv_neigh_ifinfo_release(struct kref *ref);

int batadv_hardif_neigh_dump(struct sk_buff *msg, struct netlink_callback *cb);
>>>>>>> upstream/android-13

struct batadv_orig_ifinfo *
batadv_orig_ifinfo_get(struct batadv_orig_node *orig_node,
		       struct batadv_hard_iface *if_outgoing);
struct batadv_orig_ifinfo *
batadv_orig_ifinfo_new(struct batadv_orig_node *orig_node,
		       struct batadv_hard_iface *if_outgoing);
<<<<<<< HEAD
void batadv_orig_ifinfo_put(struct batadv_orig_ifinfo *orig_ifinfo);

int batadv_orig_seq_print_text(struct seq_file *seq, void *offset);
int batadv_orig_dump(struct sk_buff *msg, struct netlink_callback *cb);
int batadv_orig_hardif_seq_print_text(struct seq_file *seq, void *offset);
int batadv_orig_hash_add_if(struct batadv_hard_iface *hard_iface,
			    unsigned int max_if_num);
int batadv_orig_hash_del_if(struct batadv_hard_iface *hard_iface,
			    unsigned int max_if_num);
=======
void batadv_orig_ifinfo_release(struct kref *ref);

int batadv_orig_dump(struct sk_buff *msg, struct netlink_callback *cb);
>>>>>>> upstream/android-13
struct batadv_orig_node_vlan *
batadv_orig_node_vlan_new(struct batadv_orig_node *orig_node,
			  unsigned short vid);
struct batadv_orig_node_vlan *
batadv_orig_node_vlan_get(struct batadv_orig_node *orig_node,
			  unsigned short vid);
<<<<<<< HEAD
void batadv_orig_node_vlan_put(struct batadv_orig_node_vlan *orig_vlan);
=======
void batadv_orig_node_vlan_release(struct kref *ref);
>>>>>>> upstream/android-13

/**
 * batadv_choose_orig() - Return the index of the orig entry in the hash table
 * @data: mac address of the originator node
 * @size: the size of the hash table
 *
 * Return: the hash index where the object represented by @data should be
 * stored at.
 */
static inline u32 batadv_choose_orig(const void *data, u32 size)
{
	u32 hash = 0;

	hash = jhash(data, ETH_ALEN, hash);
	return hash % size;
}

struct batadv_orig_node *
batadv_orig_hash_find(struct batadv_priv *bat_priv, const void *data);

<<<<<<< HEAD
=======
/**
 * batadv_orig_node_vlan_put() - decrement the refcounter and possibly release
 *  the originator-vlan object
 * @orig_vlan: the originator-vlan object to release
 */
static inline void
batadv_orig_node_vlan_put(struct batadv_orig_node_vlan *orig_vlan)
{
	if (!orig_vlan)
		return;

	kref_put(&orig_vlan->refcount, batadv_orig_node_vlan_release);
}

/**
 * batadv_neigh_ifinfo_put() - decrement the refcounter and possibly release
 *  the neigh_ifinfo
 * @neigh_ifinfo: the neigh_ifinfo object to release
 */
static inline void
batadv_neigh_ifinfo_put(struct batadv_neigh_ifinfo *neigh_ifinfo)
{
	if (!neigh_ifinfo)
		return;

	kref_put(&neigh_ifinfo->refcount, batadv_neigh_ifinfo_release);
}

/**
 * batadv_hardif_neigh_put() - decrement the hardif neighbors refcounter
 *  and possibly release it
 * @hardif_neigh: hardif neigh neighbor to free
 */
static inline void
batadv_hardif_neigh_put(struct batadv_hardif_neigh_node *hardif_neigh)
{
	if (!hardif_neigh)
		return;

	kref_put(&hardif_neigh->refcount, batadv_hardif_neigh_release);
}

/**
 * batadv_neigh_node_put() - decrement the neighbors refcounter and possibly
 *  release it
 * @neigh_node: neigh neighbor to free
 */
static inline void batadv_neigh_node_put(struct batadv_neigh_node *neigh_node)
{
	if (!neigh_node)
		return;

	kref_put(&neigh_node->refcount, batadv_neigh_node_release);
}

/**
 * batadv_orig_ifinfo_put() - decrement the refcounter and possibly release
 *  the orig_ifinfo
 * @orig_ifinfo: the orig_ifinfo object to release
 */
static inline void
batadv_orig_ifinfo_put(struct batadv_orig_ifinfo *orig_ifinfo)
{
	if (!orig_ifinfo)
		return;

	kref_put(&orig_ifinfo->refcount, batadv_orig_ifinfo_release);
}

/**
 * batadv_orig_node_put() - decrement the orig node refcounter and possibly
 *  release it
 * @orig_node: the orig node to free
 */
static inline void batadv_orig_node_put(struct batadv_orig_node *orig_node)
{
	if (!orig_node)
		return;

	kref_put(&orig_node->refcount, batadv_orig_node_release);
}

>>>>>>> upstream/android-13
#endif /* _NET_BATMAN_ADV_ORIGINATOR_H_ */
