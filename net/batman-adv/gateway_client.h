/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2009-2018  B.A.T.M.A.N. contributors:
 *
 * Marek Lindner
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
 * Marek Lindner
>>>>>>> upstream/android-13
 */

#ifndef _NET_BATMAN_ADV_GATEWAY_CLIENT_H_
#define _NET_BATMAN_ADV_GATEWAY_CLIENT_H_

#include "main.h"

<<<<<<< HEAD
#include <linux/types.h>

struct batadv_tvlv_gateway_data;
struct netlink_callback;
struct seq_file;
struct sk_buff;
=======
#include <linux/kref.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <uapi/linux/batadv_packet.h>
>>>>>>> upstream/android-13

void batadv_gw_check_client_stop(struct batadv_priv *bat_priv);
void batadv_gw_reselect(struct batadv_priv *bat_priv);
void batadv_gw_election(struct batadv_priv *bat_priv);
struct batadv_orig_node *
batadv_gw_get_selected_orig(struct batadv_priv *bat_priv);
void batadv_gw_check_election(struct batadv_priv *bat_priv,
			      struct batadv_orig_node *orig_node);
void batadv_gw_node_update(struct batadv_priv *bat_priv,
			   struct batadv_orig_node *orig_node,
			   struct batadv_tvlv_gateway_data *gateway);
void batadv_gw_node_delete(struct batadv_priv *bat_priv,
			   struct batadv_orig_node *orig_node);
void batadv_gw_node_free(struct batadv_priv *bat_priv);
<<<<<<< HEAD
void batadv_gw_node_put(struct batadv_gw_node *gw_node);
struct batadv_gw_node *
batadv_gw_get_selected_gw_node(struct batadv_priv *bat_priv);
int batadv_gw_client_seq_print_text(struct seq_file *seq, void *offset);
=======
void batadv_gw_node_release(struct kref *ref);
struct batadv_gw_node *
batadv_gw_get_selected_gw_node(struct batadv_priv *bat_priv);
>>>>>>> upstream/android-13
int batadv_gw_dump(struct sk_buff *msg, struct netlink_callback *cb);
bool batadv_gw_out_of_range(struct batadv_priv *bat_priv, struct sk_buff *skb);
enum batadv_dhcp_recipient
batadv_gw_dhcp_recipient_get(struct sk_buff *skb, unsigned int *header_len,
			     u8 *chaddr);
struct batadv_gw_node *batadv_gw_node_get(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig_node);

<<<<<<< HEAD
=======
/**
 * batadv_gw_node_put() - decrement the gw_node refcounter and possibly release
 *  it
 * @gw_node: gateway node to free
 */
static inline void batadv_gw_node_put(struct batadv_gw_node *gw_node)
{
	if (!gw_node)
		return;

	kref_put(&gw_node->refcount, batadv_gw_node_release);
}

>>>>>>> upstream/android-13
#endif /* _NET_BATMAN_ADV_GATEWAY_CLIENT_H_ */
