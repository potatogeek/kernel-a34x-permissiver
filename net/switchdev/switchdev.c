<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * net/switchdev/switchdev.c - Switch device API
 * Copyright (c) 2014-2015 Jiri Pirko <jiri@resnulli.us>
 * Copyright (c) 2014-2015 Scott Feldman <sfeldma@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if_bridge.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/if_vlan.h>
#include <linux/rtnetlink.h>
#include <net/switchdev.h>

<<<<<<< HEAD
/**
 *	switchdev_trans_item_enqueue - Enqueue data item to transaction queue
 *
 *	@trans: transaction
 *	@data: pointer to data being queued
 *	@destructor: data destructor
 *	@tritem: transaction item being queued
 *
 *	Enqeueue data item to transaction queue. tritem is typically placed in
 *	cointainter pointed at by data pointer. Destructor is called on
 *	transaction abort and after successful commit phase in case
 *	the caller did not dequeue the item before.
 */
void switchdev_trans_item_enqueue(struct switchdev_trans *trans,
				  void *data, void (*destructor)(void const *),
				  struct switchdev_trans_item *tritem)
{
	tritem->data = data;
	tritem->destructor = destructor;
	list_add_tail(&tritem->list, &trans->item_list);
}
EXPORT_SYMBOL_GPL(switchdev_trans_item_enqueue);

static struct switchdev_trans_item *
__switchdev_trans_item_dequeue(struct switchdev_trans *trans)
{
	struct switchdev_trans_item *tritem;

	if (list_empty(&trans->item_list))
		return NULL;
	tritem = list_first_entry(&trans->item_list,
				  struct switchdev_trans_item, list);
	list_del(&tritem->list);
	return tritem;
}

/**
 *	switchdev_trans_item_dequeue - Dequeue data item from transaction queue
 *
 *	@trans: transaction
 */
void *switchdev_trans_item_dequeue(struct switchdev_trans *trans)
{
	struct switchdev_trans_item *tritem;

	tritem = __switchdev_trans_item_dequeue(trans);
	BUG_ON(!tritem);
	return tritem->data;
}
EXPORT_SYMBOL_GPL(switchdev_trans_item_dequeue);

static void switchdev_trans_init(struct switchdev_trans *trans)
{
	INIT_LIST_HEAD(&trans->item_list);
}

static void switchdev_trans_items_destroy(struct switchdev_trans *trans)
{
	struct switchdev_trans_item *tritem;

	while ((tritem = __switchdev_trans_item_dequeue(trans)))
		tritem->destructor(tritem->data);
}

static void switchdev_trans_items_warn_destroy(struct net_device *dev,
					       struct switchdev_trans *trans)
{
	WARN(!list_empty(&trans->item_list), "%s: transaction item queue is not empty.\n",
	     dev->name);
	switchdev_trans_items_destroy(trans);
}

=======
>>>>>>> upstream/android-13
static LIST_HEAD(deferred);
static DEFINE_SPINLOCK(deferred_lock);

typedef void switchdev_deferred_func_t(struct net_device *dev,
				       const void *data);

struct switchdev_deferred_item {
	struct list_head list;
	struct net_device *dev;
	switchdev_deferred_func_t *func;
<<<<<<< HEAD
	unsigned long data[0];
=======
	unsigned long data[];
>>>>>>> upstream/android-13
};

static struct switchdev_deferred_item *switchdev_deferred_dequeue(void)
{
	struct switchdev_deferred_item *dfitem;

	spin_lock_bh(&deferred_lock);
	if (list_empty(&deferred)) {
		dfitem = NULL;
		goto unlock;
	}
	dfitem = list_first_entry(&deferred,
				  struct switchdev_deferred_item, list);
	list_del(&dfitem->list);
unlock:
	spin_unlock_bh(&deferred_lock);
	return dfitem;
}

/**
 *	switchdev_deferred_process - Process ops in deferred queue
 *
 *	Called to flush the ops currently queued in deferred ops queue.
 *	rtnl_lock must be held.
 */
void switchdev_deferred_process(void)
{
	struct switchdev_deferred_item *dfitem;

	ASSERT_RTNL();

	while ((dfitem = switchdev_deferred_dequeue())) {
		dfitem->func(dfitem->dev, dfitem->data);
		dev_put(dfitem->dev);
		kfree(dfitem);
	}
}
EXPORT_SYMBOL_GPL(switchdev_deferred_process);

static void switchdev_deferred_process_work(struct work_struct *work)
{
	rtnl_lock();
	switchdev_deferred_process();
	rtnl_unlock();
}

static DECLARE_WORK(deferred_process_work, switchdev_deferred_process_work);

static int switchdev_deferred_enqueue(struct net_device *dev,
				      const void *data, size_t data_len,
				      switchdev_deferred_func_t *func)
{
	struct switchdev_deferred_item *dfitem;

	dfitem = kmalloc(sizeof(*dfitem) + data_len, GFP_ATOMIC);
	if (!dfitem)
		return -ENOMEM;
	dfitem->dev = dev;
	dfitem->func = func;
	memcpy(dfitem->data, data, data_len);
	dev_hold(dev);
	spin_lock_bh(&deferred_lock);
	list_add_tail(&dfitem->list, &deferred);
	spin_unlock_bh(&deferred_lock);
	schedule_work(&deferred_process_work);
	return 0;
}

<<<<<<< HEAD
/**
 *	switchdev_port_attr_get - Get port attribute
 *
 *	@dev: port device
 *	@attr: attribute to get
 */
int switchdev_port_attr_get(struct net_device *dev, struct switchdev_attr *attr)
{
	const struct switchdev_ops *ops = dev->switchdev_ops;
	struct net_device *lower_dev;
	struct list_head *iter;
	struct switchdev_attr first = {
		.id = SWITCHDEV_ATTR_ID_UNDEFINED
	};
	int err = -EOPNOTSUPP;

	if (ops && ops->switchdev_port_attr_get)
		return ops->switchdev_port_attr_get(dev, attr);

	if (attr->flags & SWITCHDEV_F_NO_RECURSE)
		return err;

	/* Switch device port(s) may be stacked under
	 * bond/team/vlan dev, so recurse down to get attr on
	 * each port.  Return -ENODATA if attr values don't
	 * compare across ports.
	 */

	netdev_for_each_lower_dev(dev, lower_dev, iter) {
		err = switchdev_port_attr_get(lower_dev, attr);
		if (err)
			break;
		if (first.id == SWITCHDEV_ATTR_ID_UNDEFINED)
			first = *attr;
		else if (memcmp(&first, attr, sizeof(*attr)))
			return -ENODATA;
	}

	return err;
}
EXPORT_SYMBOL_GPL(switchdev_port_attr_get);

static int __switchdev_port_attr_set(struct net_device *dev,
				     const struct switchdev_attr *attr,
				     struct switchdev_trans *trans)
{
	const struct switchdev_ops *ops = dev->switchdev_ops;
	struct net_device *lower_dev;
	struct list_head *iter;
	int err = -EOPNOTSUPP;

	if (ops && ops->switchdev_port_attr_set) {
		err = ops->switchdev_port_attr_set(dev, attr, trans);
		goto done;
	}

	if (attr->flags & SWITCHDEV_F_NO_RECURSE)
		goto done;

	/* Switch device port(s) may be stacked under
	 * bond/team/vlan dev, so recurse down to set attr on
	 * each port.
	 */

	netdev_for_each_lower_dev(dev, lower_dev, iter) {
		err = __switchdev_port_attr_set(lower_dev, attr, trans);
		if (err)
			break;
	}

done:
	if (err == -EOPNOTSUPP && attr->flags & SWITCHDEV_F_SKIP_EOPNOTSUPP)
		err = 0;

	return err;
}

static int switchdev_port_attr_set_now(struct net_device *dev,
				       const struct switchdev_attr *attr)
{
	struct switchdev_trans trans;
	int err;

	switchdev_trans_init(&trans);

	/* Phase I: prepare for attr set. Driver/device should fail
	 * here if there are going to be issues in the commit phase,
	 * such as lack of resources or support.  The driver/device
	 * should reserve resources needed for the commit phase here,
	 * but should not commit the attr.
	 */

	trans.ph_prepare = true;
	err = __switchdev_port_attr_set(dev, attr, &trans);
	if (err) {
		/* Prepare phase failed: abort the transaction.  Any
		 * resources reserved in the prepare phase are
		 * released.
		 */

		if (err != -EOPNOTSUPP)
			switchdev_trans_items_destroy(&trans);

		return err;
	}

	/* Phase II: commit attr set.  This cannot fail as a fault
	 * of driver/device.  If it does, it's a bug in the driver/device
	 * because the driver said everythings was OK in phase I.
	 */

	trans.ph_prepare = false;
	err = __switchdev_port_attr_set(dev, attr, &trans);
	WARN(err, "%s: Commit of attribute (id=%d) failed.\n",
	     dev->name, attr->id);
	switchdev_trans_items_warn_destroy(dev, &trans);

	return err;
=======
static int switchdev_port_attr_notify(enum switchdev_notifier_type nt,
				      struct net_device *dev,
				      const struct switchdev_attr *attr,
				      struct netlink_ext_ack *extack)
{
	int err;
	int rc;

	struct switchdev_notifier_port_attr_info attr_info = {
		.attr = attr,
		.handled = false,
	};

	rc = call_switchdev_blocking_notifiers(nt, dev,
					       &attr_info.info, extack);
	err = notifier_to_errno(rc);
	if (err) {
		WARN_ON(!attr_info.handled);
		return err;
	}

	if (!attr_info.handled)
		return -EOPNOTSUPP;

	return 0;
}

static int switchdev_port_attr_set_now(struct net_device *dev,
				       const struct switchdev_attr *attr,
				       struct netlink_ext_ack *extack)
{
	return switchdev_port_attr_notify(SWITCHDEV_PORT_ATTR_SET, dev, attr,
					  extack);
>>>>>>> upstream/android-13
}

static void switchdev_port_attr_set_deferred(struct net_device *dev,
					     const void *data)
{
	const struct switchdev_attr *attr = data;
	int err;

<<<<<<< HEAD
	err = switchdev_port_attr_set_now(dev, attr);
=======
	err = switchdev_port_attr_set_now(dev, attr, NULL);
>>>>>>> upstream/android-13
	if (err && err != -EOPNOTSUPP)
		netdev_err(dev, "failed (err=%d) to set attribute (id=%d)\n",
			   err, attr->id);
	if (attr->complete)
		attr->complete(dev, err, attr->complete_priv);
}

static int switchdev_port_attr_set_defer(struct net_device *dev,
					 const struct switchdev_attr *attr)
{
	return switchdev_deferred_enqueue(dev, attr, sizeof(*attr),
					  switchdev_port_attr_set_deferred);
}

/**
 *	switchdev_port_attr_set - Set port attribute
 *
 *	@dev: port device
 *	@attr: attribute to set
<<<<<<< HEAD
 *
 *	Use a 2-phase prepare-commit transaction model to ensure
 *	system is not left in a partially updated state due to
 *	failure from driver/device.
=======
 *	@extack: netlink extended ack, for error message propagation
>>>>>>> upstream/android-13
 *
 *	rtnl_lock must be held and must not be in atomic section,
 *	in case SWITCHDEV_F_DEFER flag is not set.
 */
int switchdev_port_attr_set(struct net_device *dev,
<<<<<<< HEAD
			    const struct switchdev_attr *attr)
=======
			    const struct switchdev_attr *attr,
			    struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	if (attr->flags & SWITCHDEV_F_DEFER)
		return switchdev_port_attr_set_defer(dev, attr);
	ASSERT_RTNL();
<<<<<<< HEAD
	return switchdev_port_attr_set_now(dev, attr);
=======
	return switchdev_port_attr_set_now(dev, attr, extack);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(switchdev_port_attr_set);

static size_t switchdev_obj_size(const struct switchdev_obj *obj)
{
	switch (obj->id) {
	case SWITCHDEV_OBJ_ID_PORT_VLAN:
		return sizeof(struct switchdev_obj_port_vlan);
	case SWITCHDEV_OBJ_ID_PORT_MDB:
		return sizeof(struct switchdev_obj_port_mdb);
	case SWITCHDEV_OBJ_ID_HOST_MDB:
		return sizeof(struct switchdev_obj_port_mdb);
	default:
		BUG();
	}
	return 0;
}

<<<<<<< HEAD
static int __switchdev_port_obj_add(struct net_device *dev,
				    const struct switchdev_obj *obj,
				    struct switchdev_trans *trans)
{
	const struct switchdev_ops *ops = dev->switchdev_ops;
	struct net_device *lower_dev;
	struct list_head *iter;
	int err = -EOPNOTSUPP;

	if (ops && ops->switchdev_port_obj_add)
		return ops->switchdev_port_obj_add(dev, obj, trans);

	/* Switch device port(s) may be stacked under
	 * bond/team/vlan dev, so recurse down to add object on
	 * each port.
	 */

	netdev_for_each_lower_dev(dev, lower_dev, iter) {
		err = __switchdev_port_obj_add(lower_dev, obj, trans);
		if (err)
			break;
	}

	return err;
}

static int switchdev_port_obj_add_now(struct net_device *dev,
				      const struct switchdev_obj *obj)
{
	struct switchdev_trans trans;
	int err;

	ASSERT_RTNL();

	switchdev_trans_init(&trans);

	/* Phase I: prepare for obj add. Driver/device should fail
	 * here if there are going to be issues in the commit phase,
	 * such as lack of resources or support.  The driver/device
	 * should reserve resources needed for the commit phase here,
	 * but should not commit the obj.
	 */

	trans.ph_prepare = true;
	err = __switchdev_port_obj_add(dev, obj, &trans);
	if (err) {
		/* Prepare phase failed: abort the transaction.  Any
		 * resources reserved in the prepare phase are
		 * released.
		 */

		if (err != -EOPNOTSUPP)
			switchdev_trans_items_destroy(&trans);

		return err;
	}

	/* Phase II: commit obj add.  This cannot fail as a fault
	 * of driver/device.  If it does, it's a bug in the driver/device
	 * because the driver said everythings was OK in phase I.
	 */

	trans.ph_prepare = false;
	err = __switchdev_port_obj_add(dev, obj, &trans);
	WARN(err, "%s: Commit of object (id=%d) failed.\n", dev->name, obj->id);
	switchdev_trans_items_warn_destroy(dev, &trans);

	return err;
=======
static int switchdev_port_obj_notify(enum switchdev_notifier_type nt,
				     struct net_device *dev,
				     const struct switchdev_obj *obj,
				     struct netlink_ext_ack *extack)
{
	int rc;
	int err;

	struct switchdev_notifier_port_obj_info obj_info = {
		.obj = obj,
		.handled = false,
	};

	rc = call_switchdev_blocking_notifiers(nt, dev, &obj_info.info, extack);
	err = notifier_to_errno(rc);
	if (err) {
		WARN_ON(!obj_info.handled);
		return err;
	}
	if (!obj_info.handled)
		return -EOPNOTSUPP;
	return 0;
>>>>>>> upstream/android-13
}

static void switchdev_port_obj_add_deferred(struct net_device *dev,
					    const void *data)
{
	const struct switchdev_obj *obj = data;
	int err;

<<<<<<< HEAD
	err = switchdev_port_obj_add_now(dev, obj);
=======
	ASSERT_RTNL();
	err = switchdev_port_obj_notify(SWITCHDEV_PORT_OBJ_ADD,
					dev, obj, NULL);
>>>>>>> upstream/android-13
	if (err && err != -EOPNOTSUPP)
		netdev_err(dev, "failed (err=%d) to add object (id=%d)\n",
			   err, obj->id);
	if (obj->complete)
		obj->complete(dev, err, obj->complete_priv);
}

static int switchdev_port_obj_add_defer(struct net_device *dev,
					const struct switchdev_obj *obj)
{
	return switchdev_deferred_enqueue(dev, obj, switchdev_obj_size(obj),
					  switchdev_port_obj_add_deferred);
}

/**
 *	switchdev_port_obj_add - Add port object
 *
 *	@dev: port device
<<<<<<< HEAD
 *	@id: object ID
 *	@obj: object to add
 *
 *	Use a 2-phase prepare-commit transaction model to ensure
 *	system is not left in a partially updated state due to
 *	failure from driver/device.
=======
 *	@obj: object to add
 *	@extack: netlink extended ack
>>>>>>> upstream/android-13
 *
 *	rtnl_lock must be held and must not be in atomic section,
 *	in case SWITCHDEV_F_DEFER flag is not set.
 */
int switchdev_port_obj_add(struct net_device *dev,
<<<<<<< HEAD
			   const struct switchdev_obj *obj)
=======
			   const struct switchdev_obj *obj,
			   struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	if (obj->flags & SWITCHDEV_F_DEFER)
		return switchdev_port_obj_add_defer(dev, obj);
	ASSERT_RTNL();
<<<<<<< HEAD
	return switchdev_port_obj_add_now(dev, obj);
=======
	return switchdev_port_obj_notify(SWITCHDEV_PORT_OBJ_ADD,
					 dev, obj, extack);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(switchdev_port_obj_add);

static int switchdev_port_obj_del_now(struct net_device *dev,
				      const struct switchdev_obj *obj)
{
<<<<<<< HEAD
	const struct switchdev_ops *ops = dev->switchdev_ops;
	struct net_device *lower_dev;
	struct list_head *iter;
	int err = -EOPNOTSUPP;

	if (ops && ops->switchdev_port_obj_del)
		return ops->switchdev_port_obj_del(dev, obj);

	/* Switch device port(s) may be stacked under
	 * bond/team/vlan dev, so recurse down to delete object on
	 * each port.
	 */

	netdev_for_each_lower_dev(dev, lower_dev, iter) {
		err = switchdev_port_obj_del_now(lower_dev, obj);
		if (err)
			break;
	}

	return err;
=======
	return switchdev_port_obj_notify(SWITCHDEV_PORT_OBJ_DEL,
					 dev, obj, NULL);
>>>>>>> upstream/android-13
}

static void switchdev_port_obj_del_deferred(struct net_device *dev,
					    const void *data)
{
	const struct switchdev_obj *obj = data;
	int err;

	err = switchdev_port_obj_del_now(dev, obj);
	if (err && err != -EOPNOTSUPP)
		netdev_err(dev, "failed (err=%d) to del object (id=%d)\n",
			   err, obj->id);
	if (obj->complete)
		obj->complete(dev, err, obj->complete_priv);
}

static int switchdev_port_obj_del_defer(struct net_device *dev,
					const struct switchdev_obj *obj)
{
	return switchdev_deferred_enqueue(dev, obj, switchdev_obj_size(obj),
					  switchdev_port_obj_del_deferred);
}

/**
 *	switchdev_port_obj_del - Delete port object
 *
 *	@dev: port device
<<<<<<< HEAD
 *	@id: object ID
=======
>>>>>>> upstream/android-13
 *	@obj: object to delete
 *
 *	rtnl_lock must be held and must not be in atomic section,
 *	in case SWITCHDEV_F_DEFER flag is not set.
 */
int switchdev_port_obj_del(struct net_device *dev,
			   const struct switchdev_obj *obj)
{
	if (obj->flags & SWITCHDEV_F_DEFER)
		return switchdev_port_obj_del_defer(dev, obj);
	ASSERT_RTNL();
	return switchdev_port_obj_del_now(dev, obj);
}
EXPORT_SYMBOL_GPL(switchdev_port_obj_del);

static ATOMIC_NOTIFIER_HEAD(switchdev_notif_chain);
<<<<<<< HEAD
=======
static BLOCKING_NOTIFIER_HEAD(switchdev_blocking_notif_chain);
>>>>>>> upstream/android-13

/**
 *	register_switchdev_notifier - Register notifier
 *	@nb: notifier_block
 *
 *	Register switch device notifier.
 */
int register_switchdev_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&switchdev_notif_chain, nb);
}
EXPORT_SYMBOL_GPL(register_switchdev_notifier);

/**
 *	unregister_switchdev_notifier - Unregister notifier
 *	@nb: notifier_block
 *
 *	Unregister switch device notifier.
 */
int unregister_switchdev_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&switchdev_notif_chain, nb);
}
EXPORT_SYMBOL_GPL(unregister_switchdev_notifier);

/**
 *	call_switchdev_notifiers - Call notifiers
 *	@val: value passed unmodified to notifier function
 *	@dev: port device
 *	@info: notifier information data
<<<<<<< HEAD
 *
 *	Call all network notifier blocks.
 */
int call_switchdev_notifiers(unsigned long val, struct net_device *dev,
			     struct switchdev_notifier_info *info)
{
	info->dev = dev;
=======
 *	@extack: netlink extended ack
 *	Call all network notifier blocks.
 */
int call_switchdev_notifiers(unsigned long val, struct net_device *dev,
			     struct switchdev_notifier_info *info,
			     struct netlink_ext_ack *extack)
{
	info->dev = dev;
	info->extack = extack;
>>>>>>> upstream/android-13
	return atomic_notifier_call_chain(&switchdev_notif_chain, val, info);
}
EXPORT_SYMBOL_GPL(call_switchdev_notifiers);

<<<<<<< HEAD
bool switchdev_port_same_parent_id(struct net_device *a,
				   struct net_device *b)
{
	struct switchdev_attr a_attr = {
		.orig_dev = a,
		.id = SWITCHDEV_ATTR_ID_PORT_PARENT_ID,
	};
	struct switchdev_attr b_attr = {
		.orig_dev = b,
		.id = SWITCHDEV_ATTR_ID_PORT_PARENT_ID,
	};

	if (switchdev_port_attr_get(a, &a_attr) ||
	    switchdev_port_attr_get(b, &b_attr))
		return false;

	return netdev_phys_item_id_same(&a_attr.u.ppid, &b_attr.u.ppid);
}
EXPORT_SYMBOL_GPL(switchdev_port_same_parent_id);
=======
int register_switchdev_blocking_notifier(struct notifier_block *nb)
{
	struct blocking_notifier_head *chain = &switchdev_blocking_notif_chain;

	return blocking_notifier_chain_register(chain, nb);
}
EXPORT_SYMBOL_GPL(register_switchdev_blocking_notifier);

int unregister_switchdev_blocking_notifier(struct notifier_block *nb)
{
	struct blocking_notifier_head *chain = &switchdev_blocking_notif_chain;

	return blocking_notifier_chain_unregister(chain, nb);
}
EXPORT_SYMBOL_GPL(unregister_switchdev_blocking_notifier);

int call_switchdev_blocking_notifiers(unsigned long val, struct net_device *dev,
				      struct switchdev_notifier_info *info,
				      struct netlink_ext_ack *extack)
{
	info->dev = dev;
	info->extack = extack;
	return blocking_notifier_call_chain(&switchdev_blocking_notif_chain,
					    val, info);
}
EXPORT_SYMBOL_GPL(call_switchdev_blocking_notifiers);

struct switchdev_nested_priv {
	bool (*check_cb)(const struct net_device *dev);
	bool (*foreign_dev_check_cb)(const struct net_device *dev,
				     const struct net_device *foreign_dev);
	const struct net_device *dev;
	struct net_device *lower_dev;
};

static int switchdev_lower_dev_walk(struct net_device *lower_dev,
				    struct netdev_nested_priv *priv)
{
	struct switchdev_nested_priv *switchdev_priv = priv->data;
	bool (*foreign_dev_check_cb)(const struct net_device *dev,
				     const struct net_device *foreign_dev);
	bool (*check_cb)(const struct net_device *dev);
	const struct net_device *dev;

	check_cb = switchdev_priv->check_cb;
	foreign_dev_check_cb = switchdev_priv->foreign_dev_check_cb;
	dev = switchdev_priv->dev;

	if (check_cb(lower_dev) && !foreign_dev_check_cb(lower_dev, dev)) {
		switchdev_priv->lower_dev = lower_dev;
		return 1;
	}

	return 0;
}

static struct net_device *
switchdev_lower_dev_find(struct net_device *dev,
			 bool (*check_cb)(const struct net_device *dev),
			 bool (*foreign_dev_check_cb)(const struct net_device *dev,
						      const struct net_device *foreign_dev))
{
	struct switchdev_nested_priv switchdev_priv = {
		.check_cb = check_cb,
		.foreign_dev_check_cb = foreign_dev_check_cb,
		.dev = dev,
		.lower_dev = NULL,
	};
	struct netdev_nested_priv priv = {
		.data = &switchdev_priv,
	};

	netdev_walk_all_lower_dev_rcu(dev, switchdev_lower_dev_walk, &priv);

	return switchdev_priv.lower_dev;
}

static int __switchdev_handle_fdb_add_to_device(struct net_device *dev,
		const struct net_device *orig_dev,
		const struct switchdev_notifier_fdb_info *fdb_info,
		bool (*check_cb)(const struct net_device *dev),
		bool (*foreign_dev_check_cb)(const struct net_device *dev,
					     const struct net_device *foreign_dev),
		int (*add_cb)(struct net_device *dev,
			      const struct net_device *orig_dev, const void *ctx,
			      const struct switchdev_notifier_fdb_info *fdb_info),
		int (*lag_add_cb)(struct net_device *dev,
				  const struct net_device *orig_dev, const void *ctx,
				  const struct switchdev_notifier_fdb_info *fdb_info))
{
	const struct switchdev_notifier_info *info = &fdb_info->info;
	struct net_device *br, *lower_dev;
	struct list_head *iter;
	int err = -EOPNOTSUPP;

	if (check_cb(dev))
		return add_cb(dev, orig_dev, info->ctx, fdb_info);

	if (netif_is_lag_master(dev)) {
		if (!switchdev_lower_dev_find(dev, check_cb, foreign_dev_check_cb))
			goto maybe_bridged_with_us;

		/* This is a LAG interface that we offload */
		if (!lag_add_cb)
			return -EOPNOTSUPP;

		return lag_add_cb(dev, orig_dev, info->ctx, fdb_info);
	}

	/* Recurse through lower interfaces in case the FDB entry is pointing
	 * towards a bridge device.
	 */
	if (netif_is_bridge_master(dev)) {
		if (!switchdev_lower_dev_find(dev, check_cb, foreign_dev_check_cb))
			return 0;

		/* This is a bridge interface that we offload */
		netdev_for_each_lower_dev(dev, lower_dev, iter) {
			/* Do not propagate FDB entries across bridges */
			if (netif_is_bridge_master(lower_dev))
				continue;

			/* Bridge ports might be either us, or LAG interfaces
			 * that we offload.
			 */
			if (!check_cb(lower_dev) &&
			    !switchdev_lower_dev_find(lower_dev, check_cb,
						      foreign_dev_check_cb))
				continue;

			err = __switchdev_handle_fdb_add_to_device(lower_dev, orig_dev,
								   fdb_info, check_cb,
								   foreign_dev_check_cb,
								   add_cb, lag_add_cb);
			if (err && err != -EOPNOTSUPP)
				return err;
		}

		return 0;
	}

maybe_bridged_with_us:
	/* Event is neither on a bridge nor a LAG. Check whether it is on an
	 * interface that is in a bridge with us.
	 */
	br = netdev_master_upper_dev_get_rcu(dev);
	if (!br || !netif_is_bridge_master(br))
		return 0;

	if (!switchdev_lower_dev_find(br, check_cb, foreign_dev_check_cb))
		return 0;

	return __switchdev_handle_fdb_add_to_device(br, orig_dev, fdb_info,
						    check_cb, foreign_dev_check_cb,
						    add_cb, lag_add_cb);
}

int switchdev_handle_fdb_add_to_device(struct net_device *dev,
		const struct switchdev_notifier_fdb_info *fdb_info,
		bool (*check_cb)(const struct net_device *dev),
		bool (*foreign_dev_check_cb)(const struct net_device *dev,
					     const struct net_device *foreign_dev),
		int (*add_cb)(struct net_device *dev,
			      const struct net_device *orig_dev, const void *ctx,
			      const struct switchdev_notifier_fdb_info *fdb_info),
		int (*lag_add_cb)(struct net_device *dev,
				  const struct net_device *orig_dev, const void *ctx,
				  const struct switchdev_notifier_fdb_info *fdb_info))
{
	int err;

	err = __switchdev_handle_fdb_add_to_device(dev, dev, fdb_info,
						   check_cb,
						   foreign_dev_check_cb,
						   add_cb, lag_add_cb);
	if (err == -EOPNOTSUPP)
		err = 0;

	return err;
}
EXPORT_SYMBOL_GPL(switchdev_handle_fdb_add_to_device);

static int __switchdev_handle_fdb_del_to_device(struct net_device *dev,
		const struct net_device *orig_dev,
		const struct switchdev_notifier_fdb_info *fdb_info,
		bool (*check_cb)(const struct net_device *dev),
		bool (*foreign_dev_check_cb)(const struct net_device *dev,
					     const struct net_device *foreign_dev),
		int (*del_cb)(struct net_device *dev,
			      const struct net_device *orig_dev, const void *ctx,
			      const struct switchdev_notifier_fdb_info *fdb_info),
		int (*lag_del_cb)(struct net_device *dev,
				  const struct net_device *orig_dev, const void *ctx,
				  const struct switchdev_notifier_fdb_info *fdb_info))
{
	const struct switchdev_notifier_info *info = &fdb_info->info;
	struct net_device *br, *lower_dev;
	struct list_head *iter;
	int err = -EOPNOTSUPP;

	if (check_cb(dev))
		return del_cb(dev, orig_dev, info->ctx, fdb_info);

	if (netif_is_lag_master(dev)) {
		if (!switchdev_lower_dev_find(dev, check_cb, foreign_dev_check_cb))
			goto maybe_bridged_with_us;

		/* This is a LAG interface that we offload */
		if (!lag_del_cb)
			return -EOPNOTSUPP;

		return lag_del_cb(dev, orig_dev, info->ctx, fdb_info);
	}

	/* Recurse through lower interfaces in case the FDB entry is pointing
	 * towards a bridge device.
	 */
	if (netif_is_bridge_master(dev)) {
		if (!switchdev_lower_dev_find(dev, check_cb, foreign_dev_check_cb))
			return 0;

		/* This is a bridge interface that we offload */
		netdev_for_each_lower_dev(dev, lower_dev, iter) {
			/* Do not propagate FDB entries across bridges */
			if (netif_is_bridge_master(lower_dev))
				continue;

			/* Bridge ports might be either us, or LAG interfaces
			 * that we offload.
			 */
			if (!check_cb(lower_dev) &&
			    !switchdev_lower_dev_find(lower_dev, check_cb,
						      foreign_dev_check_cb))
				continue;

			err = __switchdev_handle_fdb_del_to_device(lower_dev, orig_dev,
								   fdb_info, check_cb,
								   foreign_dev_check_cb,
								   del_cb, lag_del_cb);
			if (err && err != -EOPNOTSUPP)
				return err;
		}

		return 0;
	}

maybe_bridged_with_us:
	/* Event is neither on a bridge nor a LAG. Check whether it is on an
	 * interface that is in a bridge with us.
	 */
	br = netdev_master_upper_dev_get_rcu(dev);
	if (!br || !netif_is_bridge_master(br))
		return 0;

	if (!switchdev_lower_dev_find(br, check_cb, foreign_dev_check_cb))
		return 0;

	return __switchdev_handle_fdb_del_to_device(br, orig_dev, fdb_info,
						    check_cb, foreign_dev_check_cb,
						    del_cb, lag_del_cb);
}

int switchdev_handle_fdb_del_to_device(struct net_device *dev,
		const struct switchdev_notifier_fdb_info *fdb_info,
		bool (*check_cb)(const struct net_device *dev),
		bool (*foreign_dev_check_cb)(const struct net_device *dev,
					     const struct net_device *foreign_dev),
		int (*del_cb)(struct net_device *dev,
			      const struct net_device *orig_dev, const void *ctx,
			      const struct switchdev_notifier_fdb_info *fdb_info),
		int (*lag_del_cb)(struct net_device *dev,
				  const struct net_device *orig_dev, const void *ctx,
				  const struct switchdev_notifier_fdb_info *fdb_info))
{
	int err;

	err = __switchdev_handle_fdb_del_to_device(dev, dev, fdb_info,
						   check_cb,
						   foreign_dev_check_cb,
						   del_cb, lag_del_cb);
	if (err == -EOPNOTSUPP)
		err = 0;

	return err;
}
EXPORT_SYMBOL_GPL(switchdev_handle_fdb_del_to_device);

static int __switchdev_handle_port_obj_add(struct net_device *dev,
			struct switchdev_notifier_port_obj_info *port_obj_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*add_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_obj *obj,
				      struct netlink_ext_ack *extack))
{
	struct switchdev_notifier_info *info = &port_obj_info->info;
	struct netlink_ext_ack *extack;
	struct net_device *lower_dev;
	struct list_head *iter;
	int err = -EOPNOTSUPP;

	extack = switchdev_notifier_info_to_extack(info);

	if (check_cb(dev)) {
		err = add_cb(dev, info->ctx, port_obj_info->obj, extack);
		if (err != -EOPNOTSUPP)
			port_obj_info->handled = true;
		return err;
	}

	/* Switch ports might be stacked under e.g. a LAG. Ignore the
	 * unsupported devices, another driver might be able to handle them. But
	 * propagate to the callers any hard errors.
	 *
	 * If the driver does its own bookkeeping of stacked ports, it's not
	 * necessary to go through this helper.
	 */
	netdev_for_each_lower_dev(dev, lower_dev, iter) {
		if (netif_is_bridge_master(lower_dev))
			continue;

		err = __switchdev_handle_port_obj_add(lower_dev, port_obj_info,
						      check_cb, add_cb);
		if (err && err != -EOPNOTSUPP)
			return err;
	}

	return err;
}

int switchdev_handle_port_obj_add(struct net_device *dev,
			struct switchdev_notifier_port_obj_info *port_obj_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*add_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_obj *obj,
				      struct netlink_ext_ack *extack))
{
	int err;

	err = __switchdev_handle_port_obj_add(dev, port_obj_info, check_cb,
					      add_cb);
	if (err == -EOPNOTSUPP)
		err = 0;
	return err;
}
EXPORT_SYMBOL_GPL(switchdev_handle_port_obj_add);

static int __switchdev_handle_port_obj_del(struct net_device *dev,
			struct switchdev_notifier_port_obj_info *port_obj_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*del_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_obj *obj))
{
	struct switchdev_notifier_info *info = &port_obj_info->info;
	struct net_device *lower_dev;
	struct list_head *iter;
	int err = -EOPNOTSUPP;

	if (check_cb(dev)) {
		err = del_cb(dev, info->ctx, port_obj_info->obj);
		if (err != -EOPNOTSUPP)
			port_obj_info->handled = true;
		return err;
	}

	/* Switch ports might be stacked under e.g. a LAG. Ignore the
	 * unsupported devices, another driver might be able to handle them. But
	 * propagate to the callers any hard errors.
	 *
	 * If the driver does its own bookkeeping of stacked ports, it's not
	 * necessary to go through this helper.
	 */
	netdev_for_each_lower_dev(dev, lower_dev, iter) {
		if (netif_is_bridge_master(lower_dev))
			continue;

		err = __switchdev_handle_port_obj_del(lower_dev, port_obj_info,
						      check_cb, del_cb);
		if (err && err != -EOPNOTSUPP)
			return err;
	}

	return err;
}

int switchdev_handle_port_obj_del(struct net_device *dev,
			struct switchdev_notifier_port_obj_info *port_obj_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*del_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_obj *obj))
{
	int err;

	err = __switchdev_handle_port_obj_del(dev, port_obj_info, check_cb,
					      del_cb);
	if (err == -EOPNOTSUPP)
		err = 0;
	return err;
}
EXPORT_SYMBOL_GPL(switchdev_handle_port_obj_del);

static int __switchdev_handle_port_attr_set(struct net_device *dev,
			struct switchdev_notifier_port_attr_info *port_attr_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*set_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_attr *attr,
				      struct netlink_ext_ack *extack))
{
	struct switchdev_notifier_info *info = &port_attr_info->info;
	struct netlink_ext_ack *extack;
	struct net_device *lower_dev;
	struct list_head *iter;
	int err = -EOPNOTSUPP;

	extack = switchdev_notifier_info_to_extack(info);

	if (check_cb(dev)) {
		err = set_cb(dev, info->ctx, port_attr_info->attr, extack);
		if (err != -EOPNOTSUPP)
			port_attr_info->handled = true;
		return err;
	}

	/* Switch ports might be stacked under e.g. a LAG. Ignore the
	 * unsupported devices, another driver might be able to handle them. But
	 * propagate to the callers any hard errors.
	 *
	 * If the driver does its own bookkeeping of stacked ports, it's not
	 * necessary to go through this helper.
	 */
	netdev_for_each_lower_dev(dev, lower_dev, iter) {
		if (netif_is_bridge_master(lower_dev))
			continue;

		err = __switchdev_handle_port_attr_set(lower_dev, port_attr_info,
						       check_cb, set_cb);
		if (err && err != -EOPNOTSUPP)
			return err;
	}

	return err;
}

int switchdev_handle_port_attr_set(struct net_device *dev,
			struct switchdev_notifier_port_attr_info *port_attr_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*set_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_attr *attr,
				      struct netlink_ext_ack *extack))
{
	int err;

	err = __switchdev_handle_port_attr_set(dev, port_attr_info, check_cb,
					       set_cb);
	if (err == -EOPNOTSUPP)
		err = 0;
	return err;
}
EXPORT_SYMBOL_GPL(switchdev_handle_port_attr_set);

int switchdev_bridge_port_offload(struct net_device *brport_dev,
				  struct net_device *dev, const void *ctx,
				  struct notifier_block *atomic_nb,
				  struct notifier_block *blocking_nb,
				  bool tx_fwd_offload,
				  struct netlink_ext_ack *extack)
{
	struct switchdev_notifier_brport_info brport_info = {
		.brport = {
			.dev = dev,
			.ctx = ctx,
			.atomic_nb = atomic_nb,
			.blocking_nb = blocking_nb,
			.tx_fwd_offload = tx_fwd_offload,
		},
	};
	int err;

	ASSERT_RTNL();

	err = call_switchdev_blocking_notifiers(SWITCHDEV_BRPORT_OFFLOADED,
						brport_dev, &brport_info.info,
						extack);
	return notifier_to_errno(err);
}
EXPORT_SYMBOL_GPL(switchdev_bridge_port_offload);

void switchdev_bridge_port_unoffload(struct net_device *brport_dev,
				     const void *ctx,
				     struct notifier_block *atomic_nb,
				     struct notifier_block *blocking_nb)
{
	struct switchdev_notifier_brport_info brport_info = {
		.brport = {
			.ctx = ctx,
			.atomic_nb = atomic_nb,
			.blocking_nb = blocking_nb,
		},
	};

	ASSERT_RTNL();

	call_switchdev_blocking_notifiers(SWITCHDEV_BRPORT_UNOFFLOADED,
					  brport_dev, &brport_info.info,
					  NULL);
}
EXPORT_SYMBOL_GPL(switchdev_bridge_port_unoffload);
>>>>>>> upstream/android-13
