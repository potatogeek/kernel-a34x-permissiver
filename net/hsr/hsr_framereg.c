<<<<<<< HEAD
/* Copyright 2011-2014 Autronica Fire and Security AS
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright 2011-2014 Autronica Fire and Security AS
 *
>>>>>>> upstream/android-13
 * Author(s):
 *	2011-2014 Arvid Brodin, arvid.brodin@alten.se
 *
 * The HSR spec says never to forward the same frame twice on the same
 * interface. A frame is identified by its source MAC address and its HSR
 * sequence number. This code keeps track of senders and their sequence numbers
 * to allow filtering of duplicate frames, and to detect HSR ring errors.
<<<<<<< HEAD
=======
 * Same code handles filtering of duplicates for PRP as well.
>>>>>>> upstream/android-13
 */

#include <linux/if_ether.h>
#include <linux/etherdevice.h>
#include <linux/slab.h>
#include <linux/rculist.h>
#include "hsr_main.h"
#include "hsr_framereg.h"
#include "hsr_netlink.h"

<<<<<<< HEAD

struct hsr_node {
	struct list_head	mac_list;
	unsigned char		MacAddressA[ETH_ALEN];
	unsigned char		MacAddressB[ETH_ALEN];
	/* Local slave through which AddrB frames are received from this node */
	enum hsr_port_type	AddrB_port;
	unsigned long		time_in[HSR_PT_PORTS];
	bool			time_in_stale[HSR_PT_PORTS];
	u16			seq_out[HSR_PT_PORTS];
	struct rcu_head		rcu_head;
};


/*	TODO: use hash lists for mac addresses (linux/jhash.h)?    */


=======
/*	TODO: use hash lists for mac addresses (linux/jhash.h)?    */

>>>>>>> upstream/android-13
/* seq_nr_after(a, b) - return true if a is after (higher in sequence than) b,
 * false otherwise.
 */
static bool seq_nr_after(u16 a, u16 b)
{
	/* Remove inconsistency where
	 * seq_nr_after(a, b) == seq_nr_before(a, b)
	 */
<<<<<<< HEAD
	if ((int) b - a == 32768)
		return false;

	return (((s16) (b - a)) < 0);
}
#define seq_nr_before(a, b)		seq_nr_after((b), (a))
#define seq_nr_after_or_eq(a, b)	(!seq_nr_before((a), (b)))
#define seq_nr_before_or_eq(a, b)	(!seq_nr_after((a), (b)))

=======
	if ((int)b - a == 32768)
		return false;

	return (((s16)(b - a)) < 0);
}

#define seq_nr_before(a, b)		seq_nr_after((b), (a))
#define seq_nr_before_or_eq(a, b)	(!seq_nr_after((a), (b)))
>>>>>>> upstream/android-13

bool hsr_addr_is_self(struct hsr_priv *hsr, unsigned char *addr)
{
	struct hsr_node *node;

	node = list_first_or_null_rcu(&hsr->self_node_db, struct hsr_node,
				      mac_list);
	if (!node) {
		WARN_ONCE(1, "HSR: No self node\n");
		return false;
	}

<<<<<<< HEAD
	if (ether_addr_equal(addr, node->MacAddressA))
		return true;
	if (ether_addr_equal(addr, node->MacAddressB))
=======
	if (ether_addr_equal(addr, node->macaddress_A))
		return true;
	if (ether_addr_equal(addr, node->macaddress_B))
>>>>>>> upstream/android-13
		return true;

	return false;
}

/* Search for mac entry. Caller must hold rcu read lock.
 */
<<<<<<< HEAD
static struct hsr_node *find_node_by_AddrA(struct list_head *node_db,
					   const unsigned char addr[ETH_ALEN])
=======
static struct hsr_node *find_node_by_addr_A(struct list_head *node_db,
					    const unsigned char addr[ETH_ALEN])
>>>>>>> upstream/android-13
{
	struct hsr_node *node;

	list_for_each_entry_rcu(node, node_db, mac_list) {
<<<<<<< HEAD
		if (ether_addr_equal(node->MacAddressA, addr))
=======
		if (ether_addr_equal(node->macaddress_A, addr))
>>>>>>> upstream/android-13
			return node;
	}

	return NULL;
}

<<<<<<< HEAD

/* Helper for device init; the self_node_db is used in hsr_rcv() to recognize
 * frames from self that's been looped over the HSR ring.
 */
int hsr_create_self_node(struct list_head *self_node_db,
			 unsigned char addr_a[ETH_ALEN],
			 unsigned char addr_b[ETH_ALEN])
{
=======
/* Helper for device init; the self_node_db is used in hsr_rcv() to recognize
 * frames from self that's been looped over the HSR ring.
 */
int hsr_create_self_node(struct hsr_priv *hsr,
			 unsigned char addr_a[ETH_ALEN],
			 unsigned char addr_b[ETH_ALEN])
{
	struct list_head *self_node_db = &hsr->self_node_db;
>>>>>>> upstream/android-13
	struct hsr_node *node, *oldnode;

	node = kmalloc(sizeof(*node), GFP_KERNEL);
	if (!node)
		return -ENOMEM;

<<<<<<< HEAD
	ether_addr_copy(node->MacAddressA, addr_a);
	ether_addr_copy(node->MacAddressB, addr_b);

	rcu_read_lock();
	oldnode = list_first_or_null_rcu(self_node_db,
						struct hsr_node, mac_list);
	if (oldnode) {
		list_replace_rcu(&oldnode->mac_list, &node->mac_list);
		rcu_read_unlock();
		synchronize_rcu();
		kfree(oldnode);
	} else {
		rcu_read_unlock();
		list_add_tail_rcu(&node->mac_list, self_node_db);
=======
	ether_addr_copy(node->macaddress_A, addr_a);
	ether_addr_copy(node->macaddress_B, addr_b);

	spin_lock_bh(&hsr->list_lock);
	oldnode = list_first_or_null_rcu(self_node_db,
					 struct hsr_node, mac_list);
	if (oldnode) {
		list_replace_rcu(&oldnode->mac_list, &node->mac_list);
		spin_unlock_bh(&hsr->list_lock);
		kfree_rcu(oldnode, rcu_head);
	} else {
		list_add_tail_rcu(&node->mac_list, self_node_db);
		spin_unlock_bh(&hsr->list_lock);
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
void hsr_del_node(struct list_head *self_node_db)
{
	struct hsr_node *node;

	rcu_read_lock();
	node = list_first_or_null_rcu(self_node_db, struct hsr_node, mac_list);
	rcu_read_unlock();
	if (node) {
		list_del_rcu(&node->mac_list);
		kfree(node);
	}
}

/* Allocate an hsr_node and add it to node_db. 'addr' is the node's AddressA;
 * seq_out is used to initialize filtering of outgoing duplicate frames
 * originating from the newly added node.
 */
struct hsr_node *hsr_add_node(struct list_head *node_db, unsigned char addr[],
			      u16 seq_out)
{
	struct hsr_node *node;
	unsigned long now;
	int i;

	node = kzalloc(sizeof(*node), GFP_ATOMIC);
	if (!node)
		return NULL;

	ether_addr_copy(node->MacAddressA, addr);
=======
void hsr_del_self_node(struct hsr_priv *hsr)
{
	struct list_head *self_node_db = &hsr->self_node_db;
	struct hsr_node *node;

	spin_lock_bh(&hsr->list_lock);
	node = list_first_or_null_rcu(self_node_db, struct hsr_node, mac_list);
	if (node) {
		list_del_rcu(&node->mac_list);
		kfree_rcu(node, rcu_head);
	}
	spin_unlock_bh(&hsr->list_lock);
}

void hsr_del_nodes(struct list_head *node_db)
{
	struct hsr_node *node;
	struct hsr_node *tmp;

	list_for_each_entry_safe(node, tmp, node_db, mac_list)
		kfree(node);
}

void prp_handle_san_frame(bool san, enum hsr_port_type port,
			  struct hsr_node *node)
{
	/* Mark if the SAN node is over LAN_A or LAN_B */
	if (port == HSR_PT_SLAVE_A) {
		node->san_a = true;
		return;
	}

	if (port == HSR_PT_SLAVE_B)
		node->san_b = true;
}

/* Allocate an hsr_node and add it to node_db. 'addr' is the node's address_A;
 * seq_out is used to initialize filtering of outgoing duplicate frames
 * originating from the newly added node.
 */
static struct hsr_node *hsr_add_node(struct hsr_priv *hsr,
				     struct list_head *node_db,
				     unsigned char addr[],
				     u16 seq_out, bool san,
				     enum hsr_port_type rx_port)
{
	struct hsr_node *new_node, *node;
	unsigned long now;
	int i;

	new_node = kzalloc(sizeof(*new_node), GFP_ATOMIC);
	if (!new_node)
		return NULL;

	ether_addr_copy(new_node->macaddress_A, addr);
>>>>>>> upstream/android-13

	/* We are only interested in time diffs here, so use current jiffies
	 * as initialization. (0 could trigger an spurious ring error warning).
	 */
	now = jiffies;
<<<<<<< HEAD
	for (i = 0; i < HSR_PT_PORTS; i++)
		node->time_in[i] = now;
	for (i = 0; i < HSR_PT_PORTS; i++)
		node->seq_out[i] = seq_out;

	list_add_tail_rcu(&node->mac_list, node_db);

	return node;
}

/* Get the hsr_node from which 'skb' was sent.
 */
struct hsr_node *hsr_get_node(struct hsr_port *port, struct sk_buff *skb,
			      bool is_sup)
{
	struct list_head *node_db = &port->hsr->node_db;
	struct hsr_node *node;
	struct ethhdr *ethhdr;
=======
	for (i = 0; i < HSR_PT_PORTS; i++) {
		new_node->time_in[i] = now;
		new_node->time_out[i] = now;
	}
	for (i = 0; i < HSR_PT_PORTS; i++)
		new_node->seq_out[i] = seq_out;

	if (san && hsr->proto_ops->handle_san_frame)
		hsr->proto_ops->handle_san_frame(san, rx_port, new_node);

	spin_lock_bh(&hsr->list_lock);
	list_for_each_entry_rcu(node, node_db, mac_list,
				lockdep_is_held(&hsr->list_lock)) {
		if (ether_addr_equal(node->macaddress_A, addr))
			goto out;
		if (ether_addr_equal(node->macaddress_B, addr))
			goto out;
	}
	list_add_tail_rcu(&new_node->mac_list, node_db);
	spin_unlock_bh(&hsr->list_lock);
	return new_node;
out:
	spin_unlock_bh(&hsr->list_lock);
	kfree(new_node);
	return node;
}

void prp_update_san_info(struct hsr_node *node, bool is_sup)
{
	if (!is_sup)
		return;

	node->san_a = false;
	node->san_b = false;
}

/* Get the hsr_node from which 'skb' was sent.
 */
struct hsr_node *hsr_get_node(struct hsr_port *port, struct list_head *node_db,
			      struct sk_buff *skb, bool is_sup,
			      enum hsr_port_type rx_port)
{
	struct hsr_priv *hsr = port->hsr;
	struct hsr_node *node;
	struct ethhdr *ethhdr;
	struct prp_rct *rct;
	bool san = false;
>>>>>>> upstream/android-13
	u16 seq_out;

	if (!skb_mac_header_was_set(skb))
		return NULL;

<<<<<<< HEAD
	ethhdr = (struct ethhdr *) skb_mac_header(skb);

	list_for_each_entry_rcu(node, node_db, mac_list) {
		if (ether_addr_equal(node->MacAddressA, ethhdr->h_source))
			return node;
		if (ether_addr_equal(node->MacAddressB, ethhdr->h_source))
			return node;
	}

	/* Everyone may create a node entry, connected node to a HSR device. */

	if (ethhdr->h_proto == htons(ETH_P_PRP)
			|| ethhdr->h_proto == htons(ETH_P_HSR)) {
=======
	ethhdr = (struct ethhdr *)skb_mac_header(skb);

	list_for_each_entry_rcu(node, node_db, mac_list) {
		if (ether_addr_equal(node->macaddress_A, ethhdr->h_source)) {
			if (hsr->proto_ops->update_san_info)
				hsr->proto_ops->update_san_info(node, is_sup);
			return node;
		}
		if (ether_addr_equal(node->macaddress_B, ethhdr->h_source)) {
			if (hsr->proto_ops->update_san_info)
				hsr->proto_ops->update_san_info(node, is_sup);
			return node;
		}
	}

	/* Everyone may create a node entry, connected node to a HSR/PRP
	 * device.
	 */
	if (ethhdr->h_proto == htons(ETH_P_PRP) ||
	    ethhdr->h_proto == htons(ETH_P_HSR)) {
>>>>>>> upstream/android-13
		/* Use the existing sequence_nr from the tag as starting point
		 * for filtering duplicate frames.
		 */
		seq_out = hsr_get_skb_sequence_nr(skb) - 1;
	} else {
<<<<<<< HEAD
		/* this is called also for frames from master port and
		 * so warn only for non master ports
		 */
		if (port->type != HSR_PT_MASTER)
			WARN_ONCE(1, "%s: Non-HSR frame\n", __func__);
		seq_out = HSR_SEQNR_START;
	}

	return hsr_add_node(node_db, ethhdr->h_source, seq_out);
}

/* Use the Supervision frame's info about an eventual MacAddressB for merging
 * nodes that has previously had their MacAddressB registered as a separate
 * node.
 */
void hsr_handle_sup_frame(struct sk_buff *skb, struct hsr_node *node_curr,
			  struct hsr_port *port_rcv)
{
	struct ethhdr *ethhdr;
	struct hsr_node *node_real;
	struct hsr_sup_payload *hsr_sp;
	struct list_head *node_db;
	int i;

	ethhdr = (struct ethhdr *) skb_mac_header(skb);
=======
		rct = skb_get_PRP_rct(skb);
		if (rct && prp_check_lsdu_size(skb, rct, is_sup)) {
			seq_out = prp_get_skb_sequence_nr(rct);
		} else {
			if (rx_port != HSR_PT_MASTER)
				san = true;
			seq_out = HSR_SEQNR_START;
		}
	}

	return hsr_add_node(hsr, node_db, ethhdr->h_source, seq_out,
			    san, rx_port);
}

/* Use the Supervision frame's info about an eventual macaddress_B for merging
 * nodes that has previously had their macaddress_B registered as a separate
 * node.
 */
void hsr_handle_sup_frame(struct hsr_frame_info *frame)
{
	struct hsr_node *node_curr = frame->node_src;
	struct hsr_port *port_rcv = frame->port_rcv;
	struct hsr_priv *hsr = port_rcv->hsr;
	struct hsr_sup_payload *hsr_sp;
	struct hsr_node *node_real;
	struct sk_buff *skb = NULL;
	struct list_head *node_db;
	struct ethhdr *ethhdr;
	int i;

	/* Here either frame->skb_hsr or frame->skb_prp should be
	 * valid as supervision frame always will have protocol
	 * header info.
	 */
	if (frame->skb_hsr)
		skb = frame->skb_hsr;
	else if (frame->skb_prp)
		skb = frame->skb_prp;
	else if (frame->skb_std)
		skb = frame->skb_std;
	if (!skb)
		return;

	ethhdr = (struct ethhdr *)skb_mac_header(skb);
>>>>>>> upstream/android-13

	/* Leave the ethernet header. */
	skb_pull(skb, sizeof(struct ethhdr));

	/* And leave the HSR tag. */
	if (ethhdr->h_proto == htons(ETH_P_HSR))
		skb_pull(skb, sizeof(struct hsr_tag));

	/* And leave the HSR sup tag. */
	skb_pull(skb, sizeof(struct hsr_sup_tag));

<<<<<<< HEAD
	hsr_sp = (struct hsr_sup_payload *) skb->data;

	/* Merge node_curr (registered on MacAddressB) into node_real */
	node_db = &port_rcv->hsr->node_db;
	node_real = find_node_by_AddrA(node_db, hsr_sp->MacAddressA);
	if (!node_real)
		/* No frame received from AddrA of this node yet */
		node_real = hsr_add_node(node_db, hsr_sp->MacAddressA,
					 HSR_SEQNR_START - 1);
=======
	hsr_sp = (struct hsr_sup_payload *)skb->data;

	/* Merge node_curr (registered on macaddress_B) into node_real */
	node_db = &port_rcv->hsr->node_db;
	node_real = find_node_by_addr_A(node_db, hsr_sp->macaddress_A);
	if (!node_real)
		/* No frame received from AddrA of this node yet */
		node_real = hsr_add_node(hsr, node_db, hsr_sp->macaddress_A,
					 HSR_SEQNR_START - 1, true,
					 port_rcv->type);
>>>>>>> upstream/android-13
	if (!node_real)
		goto done; /* No mem */
	if (node_real == node_curr)
		/* Node has already been merged */
		goto done;

<<<<<<< HEAD
	ether_addr_copy(node_real->MacAddressB, ethhdr->h_source);
=======
	ether_addr_copy(node_real->macaddress_B, ethhdr->h_source);
>>>>>>> upstream/android-13
	for (i = 0; i < HSR_PT_PORTS; i++) {
		if (!node_curr->time_in_stale[i] &&
		    time_after(node_curr->time_in[i], node_real->time_in[i])) {
			node_real->time_in[i] = node_curr->time_in[i];
<<<<<<< HEAD
			node_real->time_in_stale[i] = node_curr->time_in_stale[i];
=======
			node_real->time_in_stale[i] =
						node_curr->time_in_stale[i];
>>>>>>> upstream/android-13
		}
		if (seq_nr_after(node_curr->seq_out[i], node_real->seq_out[i]))
			node_real->seq_out[i] = node_curr->seq_out[i];
	}
<<<<<<< HEAD
	node_real->AddrB_port = port_rcv->type;

	list_del_rcu(&node_curr->mac_list);
	kfree_rcu(node_curr, rcu_head);

done:
	skb_push(skb, sizeof(struct hsrv1_ethhdr_sp));
}


/* 'skb' is a frame meant for this host, that is to be passed to upper layers.
 *
 * If the frame was sent by a node's B interface, replace the source
 * address with that node's "official" address (MacAddressA) so that upper
=======
	node_real->addr_B_port = port_rcv->type;

	spin_lock_bh(&hsr->list_lock);
	list_del_rcu(&node_curr->mac_list);
	spin_unlock_bh(&hsr->list_lock);
	kfree_rcu(node_curr, rcu_head);

done:
	/* PRP uses v0 header */
	if (ethhdr->h_proto == htons(ETH_P_HSR))
		skb_push(skb, sizeof(struct hsrv1_ethhdr_sp));
	else
		skb_push(skb, sizeof(struct hsrv0_ethhdr_sp));
}

/* 'skb' is a frame meant for this host, that is to be passed to upper layers.
 *
 * If the frame was sent by a node's B interface, replace the source
 * address with that node's "official" address (macaddress_A) so that upper
>>>>>>> upstream/android-13
 * layers recognize where it came from.
 */
void hsr_addr_subst_source(struct hsr_node *node, struct sk_buff *skb)
{
	if (!skb_mac_header_was_set(skb)) {
		WARN_ONCE(1, "%s: Mac header not set\n", __func__);
		return;
	}

<<<<<<< HEAD
	memcpy(&eth_hdr(skb)->h_source, node->MacAddressA, ETH_ALEN);
=======
	memcpy(&eth_hdr(skb)->h_source, node->macaddress_A, ETH_ALEN);
>>>>>>> upstream/android-13
}

/* 'skb' is a frame meant for another host.
 * 'port' is the outgoing interface
 *
 * Substitute the target (dest) MAC address if necessary, so the it matches the
 * recipient interface MAC address, regardless of whether that is the
 * recipient's A or B interface.
 * This is needed to keep the packets flowing through switches that learn on
 * which "side" the different interfaces are.
 */
void hsr_addr_subst_dest(struct hsr_node *node_src, struct sk_buff *skb,
			 struct hsr_port *port)
{
	struct hsr_node *node_dst;

	if (!skb_mac_header_was_set(skb)) {
		WARN_ONCE(1, "%s: Mac header not set\n", __func__);
		return;
	}

	if (!is_unicast_ether_addr(eth_hdr(skb)->h_dest))
		return;

<<<<<<< HEAD
	node_dst = find_node_by_AddrA(&port->hsr->node_db, eth_hdr(skb)->h_dest);
=======
	node_dst = find_node_by_addr_A(&port->hsr->node_db,
				       eth_hdr(skb)->h_dest);
>>>>>>> upstream/android-13
	if (!node_dst) {
		if (net_ratelimit())
			netdev_err(skb->dev, "%s: Unknown node\n", __func__);
		return;
	}
<<<<<<< HEAD
	if (port->type != node_dst->AddrB_port)
		return;

	ether_addr_copy(eth_hdr(skb)->h_dest, node_dst->MacAddressB);
}


=======
	if (port->type != node_dst->addr_B_port)
		return;

	if (is_valid_ether_addr(node_dst->macaddress_B))
		ether_addr_copy(eth_hdr(skb)->h_dest, node_dst->macaddress_B);
}

>>>>>>> upstream/android-13
void hsr_register_frame_in(struct hsr_node *node, struct hsr_port *port,
			   u16 sequence_nr)
{
	/* Don't register incoming frames without a valid sequence number. This
	 * ensures entries of restarted nodes gets pruned so that they can
	 * re-register and resume communications.
	 */
<<<<<<< HEAD
	if (seq_nr_before(sequence_nr, node->seq_out[port->type]))
=======
	if (!(port->dev->features & NETIF_F_HW_HSR_TAG_RM) &&
	    seq_nr_before(sequence_nr, node->seq_out[port->type]))
>>>>>>> upstream/android-13
		return;

	node->time_in[port->type] = jiffies;
	node->time_in_stale[port->type] = false;
}

/* 'skb' is a HSR Ethernet frame (with a HSR tag inserted), with a valid
 * ethhdr->h_source address and skb->mac_header set.
 *
 * Return:
 *	 1 if frame can be shown to have been sent recently on this interface,
 *	 0 otherwise, or
 *	 negative error code on error
 */
int hsr_register_frame_out(struct hsr_port *port, struct hsr_node *node,
			   u16 sequence_nr)
{
<<<<<<< HEAD
	if (seq_nr_before_or_eq(sequence_nr, node->seq_out[port->type]))
		return 1;

=======
	if (seq_nr_before_or_eq(sequence_nr, node->seq_out[port->type]) &&
	    time_is_after_jiffies(node->time_out[port->type] +
	    msecs_to_jiffies(HSR_ENTRY_FORGET_TIME)))
		return 1;

	node->time_out[port->type] = jiffies;
>>>>>>> upstream/android-13
	node->seq_out[port->type] = sequence_nr;
	return 0;
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
static struct hsr_port *get_late_port(struct hsr_priv *hsr,
				      struct hsr_node *node)
{
	if (node->time_in_stale[HSR_PT_SLAVE_A])
		return hsr_port_get_hsr(hsr, HSR_PT_SLAVE_A);
	if (node->time_in_stale[HSR_PT_SLAVE_B])
		return hsr_port_get_hsr(hsr, HSR_PT_SLAVE_B);

	if (time_after(node->time_in[HSR_PT_SLAVE_B],
		       node->time_in[HSR_PT_SLAVE_A] +
					msecs_to_jiffies(MAX_SLAVE_DIFF)))
		return hsr_port_get_hsr(hsr, HSR_PT_SLAVE_A);
	if (time_after(node->time_in[HSR_PT_SLAVE_A],
		       node->time_in[HSR_PT_SLAVE_B] +
					msecs_to_jiffies(MAX_SLAVE_DIFF)))
		return hsr_port_get_hsr(hsr, HSR_PT_SLAVE_B);

	return NULL;
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
/* Remove stale sequence_nr records. Called by timer every
 * HSR_LIFE_CHECK_INTERVAL (two seconds or so).
 */
void hsr_prune_nodes(struct timer_list *t)
{
	struct hsr_priv *hsr = from_timer(hsr, t, prune_timer);
	struct hsr_node *node;
<<<<<<< HEAD
=======
	struct hsr_node *tmp;
>>>>>>> upstream/android-13
	struct hsr_port *port;
	unsigned long timestamp;
	unsigned long time_a, time_b;

<<<<<<< HEAD
	rcu_read_lock();
	list_for_each_entry_rcu(node, &hsr->node_db, mac_list) {
=======
	spin_lock_bh(&hsr->list_lock);
	list_for_each_entry_safe(node, tmp, &hsr->node_db, mac_list) {
		/* Don't prune own node. Neither time_in[HSR_PT_SLAVE_A]
		 * nor time_in[HSR_PT_SLAVE_B], will ever be updated for
		 * the master port. Thus the master node will be repeatedly
		 * pruned leading to packet loss.
		 */
		if (hsr_addr_is_self(hsr, node->macaddress_A))
			continue;

>>>>>>> upstream/android-13
		/* Shorthand */
		time_a = node->time_in[HSR_PT_SLAVE_A];
		time_b = node->time_in[HSR_PT_SLAVE_B];

		/* Check for timestamps old enough to risk wrap-around */
<<<<<<< HEAD
		if (time_after(jiffies, time_a + MAX_JIFFY_OFFSET/2))
			node->time_in_stale[HSR_PT_SLAVE_A] = true;
		if (time_after(jiffies, time_b + MAX_JIFFY_OFFSET/2))
=======
		if (time_after(jiffies, time_a + MAX_JIFFY_OFFSET / 2))
			node->time_in_stale[HSR_PT_SLAVE_A] = true;
		if (time_after(jiffies, time_b + MAX_JIFFY_OFFSET / 2))
>>>>>>> upstream/android-13
			node->time_in_stale[HSR_PT_SLAVE_B] = true;

		/* Get age of newest frame from node.
		 * At least one time_in is OK here; nodes get pruned long
		 * before both time_ins can get stale
		 */
		timestamp = time_a;
		if (node->time_in_stale[HSR_PT_SLAVE_A] ||
		    (!node->time_in_stale[HSR_PT_SLAVE_B] &&
		    time_after(time_b, time_a)))
			timestamp = time_b;

		/* Warn of ring error only as long as we get frames at all */
		if (time_is_after_jiffies(timestamp +
<<<<<<< HEAD
					msecs_to_jiffies(1.5*MAX_SLAVE_DIFF))) {
			rcu_read_lock();
			port = get_late_port(hsr, node);
			if (port != NULL)
				hsr_nl_ringerror(hsr, node->MacAddressA, port);
=======
				msecs_to_jiffies(1.5 * MAX_SLAVE_DIFF))) {
			rcu_read_lock();
			port = get_late_port(hsr, node);
			if (port)
				hsr_nl_ringerror(hsr, node->macaddress_A, port);
>>>>>>> upstream/android-13
			rcu_read_unlock();
		}

		/* Prune old entries */
		if (time_is_before_jiffies(timestamp +
<<<<<<< HEAD
					msecs_to_jiffies(HSR_NODE_FORGET_TIME))) {
			hsr_nl_nodedown(hsr, node->MacAddressA);
=======
				msecs_to_jiffies(HSR_NODE_FORGET_TIME))) {
			hsr_nl_nodedown(hsr, node->macaddress_A);
>>>>>>> upstream/android-13
			list_del_rcu(&node->mac_list);
			/* Note that we need to free this entry later: */
			kfree_rcu(node, rcu_head);
		}
	}
<<<<<<< HEAD
	rcu_read_unlock();
}

=======
	spin_unlock_bh(&hsr->list_lock);

	/* Restart timer */
	mod_timer(&hsr->prune_timer,
		  jiffies + msecs_to_jiffies(PRUNE_PERIOD));
}
>>>>>>> upstream/android-13

void *hsr_get_next_node(struct hsr_priv *hsr, void *_pos,
			unsigned char addr[ETH_ALEN])
{
	struct hsr_node *node;

	if (!_pos) {
		node = list_first_or_null_rcu(&hsr->node_db,
					      struct hsr_node, mac_list);
		if (node)
<<<<<<< HEAD
			ether_addr_copy(addr, node->MacAddressA);
=======
			ether_addr_copy(addr, node->macaddress_A);
>>>>>>> upstream/android-13
		return node;
	}

	node = _pos;
	list_for_each_entry_continue_rcu(node, &hsr->node_db, mac_list) {
<<<<<<< HEAD
		ether_addr_copy(addr, node->MacAddressA);
=======
		ether_addr_copy(addr, node->macaddress_A);
>>>>>>> upstream/android-13
		return node;
	}

	return NULL;
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
int hsr_get_node_data(struct hsr_priv *hsr,
		      const unsigned char *addr,
		      unsigned char addr_b[ETH_ALEN],
		      unsigned int *addr_b_ifindex,
		      int *if1_age,
		      u16 *if1_seq,
		      int *if2_age,
		      u16 *if2_seq)
{
	struct hsr_node *node;
	struct hsr_port *port;
	unsigned long tdiff;

<<<<<<< HEAD
	node = find_node_by_AddrA(&hsr->node_db, addr);
	if (!node)
		return -ENOENT;

	ether_addr_copy(addr_b, node->MacAddressB);
=======
	node = find_node_by_addr_A(&hsr->node_db, addr);
	if (!node)
		return -ENOENT;

	ether_addr_copy(addr_b, node->macaddress_B);
>>>>>>> upstream/android-13

	tdiff = jiffies - node->time_in[HSR_PT_SLAVE_A];
	if (node->time_in_stale[HSR_PT_SLAVE_A])
		*if1_age = INT_MAX;
#if HZ <= MSEC_PER_SEC
	else if (tdiff > msecs_to_jiffies(INT_MAX))
		*if1_age = INT_MAX;
#endif
	else
		*if1_age = jiffies_to_msecs(tdiff);

	tdiff = jiffies - node->time_in[HSR_PT_SLAVE_B];
	if (node->time_in_stale[HSR_PT_SLAVE_B])
		*if2_age = INT_MAX;
#if HZ <= MSEC_PER_SEC
	else if (tdiff > msecs_to_jiffies(INT_MAX))
		*if2_age = INT_MAX;
#endif
	else
		*if2_age = jiffies_to_msecs(tdiff);

	/* Present sequence numbers as if they were incoming on interface */
	*if1_seq = node->seq_out[HSR_PT_SLAVE_B];
	*if2_seq = node->seq_out[HSR_PT_SLAVE_A];

<<<<<<< HEAD
	if (node->AddrB_port != HSR_PT_NONE) {
		port = hsr_port_get_hsr(hsr, node->AddrB_port);
=======
	if (node->addr_B_port != HSR_PT_NONE) {
		port = hsr_port_get_hsr(hsr, node->addr_B_port);
>>>>>>> upstream/android-13
		*addr_b_ifindex = port->dev->ifindex;
	} else {
		*addr_b_ifindex = -1;
	}

	return 0;
}
