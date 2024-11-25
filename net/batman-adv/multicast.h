/* SPDX-License-Identifier: GPL-2.0 */
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

#ifndef _NET_BATMAN_ADV_MULTICAST_H_
#define _NET_BATMAN_ADV_MULTICAST_H_

#include "main.h"

<<<<<<< HEAD
struct netlink_callback;
struct seq_file;
struct sk_buff;
=======
#include <linux/netlink.h>
#include <linux/skbuff.h>
>>>>>>> upstream/android-13

/**
 * enum batadv_forw_mode - the way a packet should be forwarded as
 */
enum batadv_forw_mode {
	/**
	 * @BATADV_FORW_ALL: forward the packet to all nodes (currently via
	 *  classic flooding)
	 */
	BATADV_FORW_ALL,

	/**
<<<<<<< HEAD
=======
	 * @BATADV_FORW_SOME: forward the packet to some nodes (currently via
	 *  a multicast-to-unicast conversion and the BATMAN unicast routing
	 *  protocol)
	 */
	BATADV_FORW_SOME,

	/**
>>>>>>> upstream/android-13
	 * @BATADV_FORW_SINGLE: forward the packet to a single node (currently
	 *  via the BATMAN unicast routing protocol)
	 */
	BATADV_FORW_SINGLE,

	/** @BATADV_FORW_NONE: don't forward, drop it */
	BATADV_FORW_NONE,
};

#ifdef CONFIG_BATMAN_ADV_MCAST

enum batadv_forw_mode
batadv_mcast_forw_mode(struct batadv_priv *bat_priv, struct sk_buff *skb,
<<<<<<< HEAD
		       struct batadv_orig_node **mcast_single_orig);

void batadv_mcast_init(struct batadv_priv *bat_priv);

int batadv_mcast_flags_seq_print_text(struct seq_file *seq, void *offset);

=======
		       struct batadv_orig_node **mcast_single_orig,
		       int *is_routable);

int batadv_mcast_forw_send_orig(struct batadv_priv *bat_priv,
				struct sk_buff *skb,
				unsigned short vid,
				struct batadv_orig_node *orig_node);

int batadv_mcast_forw_send(struct batadv_priv *bat_priv, struct sk_buff *skb,
			   unsigned short vid, int is_routable);

void batadv_mcast_init(struct batadv_priv *bat_priv);

>>>>>>> upstream/android-13
int batadv_mcast_mesh_info_put(struct sk_buff *msg,
			       struct batadv_priv *bat_priv);

int batadv_mcast_flags_dump(struct sk_buff *msg, struct netlink_callback *cb);

void batadv_mcast_free(struct batadv_priv *bat_priv);

void batadv_mcast_purge_orig(struct batadv_orig_node *orig_node);

#else

static inline enum batadv_forw_mode
batadv_mcast_forw_mode(struct batadv_priv *bat_priv, struct sk_buff *skb,
<<<<<<< HEAD
		       struct batadv_orig_node **mcast_single_orig)
=======
		       struct batadv_orig_node **mcast_single_orig,
		       int *is_routable)
>>>>>>> upstream/android-13
{
	return BATADV_FORW_ALL;
}

<<<<<<< HEAD
=======
static inline int
batadv_mcast_forw_send_orig(struct batadv_priv *bat_priv,
			    struct sk_buff *skb,
			    unsigned short vid,
			    struct batadv_orig_node *orig_node)
{
	kfree_skb(skb);
	return NET_XMIT_DROP;
}

static inline int
batadv_mcast_forw_send(struct batadv_priv *bat_priv, struct sk_buff *skb,
		       unsigned short vid, int is_routable)
{
	kfree_skb(skb);
	return NET_XMIT_DROP;
}

>>>>>>> upstream/android-13
static inline int batadv_mcast_init(struct batadv_priv *bat_priv)
{
	return 0;
}

static inline int
batadv_mcast_mesh_info_put(struct sk_buff *msg, struct batadv_priv *bat_priv)
{
	return 0;
}

static inline int batadv_mcast_flags_dump(struct sk_buff *msg,
					  struct netlink_callback *cb)
{
	return -EOPNOTSUPP;
}

static inline void batadv_mcast_free(struct batadv_priv *bat_priv)
{
}

static inline void batadv_mcast_purge_orig(struct batadv_orig_node *orig_node)
{
}

#endif /* CONFIG_BATMAN_ADV_MCAST */

#endif /* _NET_BATMAN_ADV_MULTICAST_H_ */
