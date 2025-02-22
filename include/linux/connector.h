<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * 	connector.h
 * 
 * 2004-2005 Copyright (c) Evgeniy Polyakov <zbr@ioremap.net>
 * All rights reserved.
<<<<<<< HEAD
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> upstream/android-13
 */
#ifndef __CONNECTOR_H
#define __CONNECTOR_H


#include <linux/refcount.h>

#include <linux/list.h>
#include <linux/workqueue.h>

#include <net/sock.h>
#include <uapi/linux/connector.h>

#define CN_CBQ_NAMELEN		32

struct cn_queue_dev {
	atomic_t refcnt;
	unsigned char name[CN_CBQ_NAMELEN];

	struct list_head queue_list;
	spinlock_t queue_lock;

	struct sock *nls;
};

struct cn_callback_id {
	unsigned char name[CN_CBQ_NAMELEN];
	struct cb_id id;
};

struct cn_callback_entry {
	struct list_head callback_entry;
	refcount_t refcnt;
	struct cn_queue_dev *pdev;

	struct cn_callback_id id;
	void (*callback) (struct cn_msg *, struct netlink_skb_parms *);

	u32 seq, group;
};

struct cn_dev {
	struct cb_id id;

	u32 seq, groups;
	struct sock *nls;
<<<<<<< HEAD
	void (*input) (struct sk_buff *skb);
=======
>>>>>>> upstream/android-13

	struct cn_queue_dev *cbdev;
};

<<<<<<< HEAD
int cn_add_callback(struct cb_id *id, const char *name,
		    void (*callback)(struct cn_msg *, struct netlink_skb_parms *));
void cn_del_callback(struct cb_id *);
int cn_netlink_send_mult(struct cn_msg *msg, u16 len, u32 portid, u32 group, gfp_t gfp_mask);
int cn_netlink_send(struct cn_msg *msg, u32 portid, u32 group, gfp_t gfp_mask);

int cn_queue_add_callback(struct cn_queue_dev *dev, const char *name,
			  struct cb_id *id,
			  void (*callback)(struct cn_msg *, struct netlink_skb_parms *));
void cn_queue_del_callback(struct cn_queue_dev *dev, struct cb_id *id);
=======
/**
 * cn_add_callback() - Registers new callback with connector core.
 *
 * @id:		unique connector's user identifier.
 *		It must be registered in connector.h for legal
 *		in-kernel users.
 * @name:	connector's callback symbolic name.
 * @callback:	connector's callback.
 * 		parameters are %cn_msg and the sender's credentials
 */
int cn_add_callback(const struct cb_id *id, const char *name,
		    void (*callback)(struct cn_msg *, struct netlink_skb_parms *));
/**
 * cn_del_callback() - Unregisters new callback with connector core.
 *
 * @id:		unique connector's user identifier.
 */
void cn_del_callback(const struct cb_id *id);


/**
 * cn_netlink_send_mult - Sends message to the specified groups.
 *
 * @msg: 	message header(with attached data).
 * @len:	Number of @msg to be sent.
 * @portid:	destination port.
 *		If non-zero the message will be sent to the given port,
 *		which should be set to the original sender.
 * @group:	destination group.
 * 		If @portid and @group is zero, then appropriate group will
 *		be searched through all registered connector users, and
 *		message will be delivered to the group which was created
 *		for user with the same ID as in @msg.
 *		If @group is not zero, then message will be delivered
 *		to the specified group.
 * @gfp_mask:	GFP mask.
 *
 * It can be safely called from softirq context, but may silently
 * fail under strong memory pressure.
 *
 * If there are no listeners for given group %-ESRCH can be returned.
 */
int cn_netlink_send_mult(struct cn_msg *msg, u16 len, u32 portid, u32 group, gfp_t gfp_mask);

/**
 * cn_netlink_send - Sends message to the specified groups.
 *
 * @msg:	message header(with attached data).
 * @portid:	destination port.
 *		If non-zero the message will be sent to the given port,
 *		which should be set to the original sender.
 * @group:	destination group.
 * 		If @portid and @group is zero, then appropriate group will
 *		be searched through all registered connector users, and
 *		message will be delivered to the group which was created
 *		for user with the same ID as in @msg.
 *		If @group is not zero, then message will be delivered
 *		to the specified group.
 * @gfp_mask:	GFP mask.
 *
 * It can be safely called from softirq context, but may silently
 * fail under strong memory pressure.
 *
 * If there are no listeners for given group %-ESRCH can be returned.
 */
int cn_netlink_send(struct cn_msg *msg, u32 portid, u32 group, gfp_t gfp_mask);

int cn_queue_add_callback(struct cn_queue_dev *dev, const char *name,
			  const struct cb_id *id,
			  void (*callback)(struct cn_msg *, struct netlink_skb_parms *));
void cn_queue_del_callback(struct cn_queue_dev *dev, const struct cb_id *id);
>>>>>>> upstream/android-13
void cn_queue_release_callback(struct cn_callback_entry *);

struct cn_queue_dev *cn_queue_alloc_dev(const char *name, struct sock *);
void cn_queue_free_dev(struct cn_queue_dev *dev);

<<<<<<< HEAD
int cn_cb_equal(struct cb_id *, struct cb_id *);
=======
int cn_cb_equal(const struct cb_id *, const struct cb_id *);
>>>>>>> upstream/android-13

#endif				/* __CONNECTOR_H */
