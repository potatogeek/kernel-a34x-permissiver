<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * net/dsa/dsa.c - Hardware switch handling
 * Copyright (c) 2008-2009 Marvell Semiconductor
 * Copyright (c) 2013 Florian Fainelli <florian@openwrt.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/device.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_mdio.h>
#include <linux/of_platform.h>
#include <linux/of_net.h>
#include <linux/netdevice.h>
#include <linux/sysfs.h>
#include <linux/phy_fixed.h>
#include <linux/ptp_classify.h>
#include <linux/etherdevice.h>

#include "dsa_priv.h"

<<<<<<< HEAD
=======
static LIST_HEAD(dsa_tag_drivers_list);
static DEFINE_MUTEX(dsa_tag_drivers_lock);

>>>>>>> upstream/android-13
static struct sk_buff *dsa_slave_notag_xmit(struct sk_buff *skb,
					    struct net_device *dev)
{
	/* Just return the original SKB */
	return skb;
}

static const struct dsa_device_ops none_ops = {
<<<<<<< HEAD
=======
	.name	= "none",
	.proto	= DSA_TAG_PROTO_NONE,
>>>>>>> upstream/android-13
	.xmit	= dsa_slave_notag_xmit,
	.rcv	= NULL,
};

<<<<<<< HEAD
const struct dsa_device_ops *dsa_device_ops[DSA_TAG_LAST] = {
#ifdef CONFIG_NET_DSA_TAG_BRCM
	[DSA_TAG_PROTO_BRCM] = &brcm_netdev_ops,
#endif
#ifdef CONFIG_NET_DSA_TAG_BRCM_PREPEND
	[DSA_TAG_PROTO_BRCM_PREPEND] = &brcm_prepend_netdev_ops,
#endif
#ifdef CONFIG_NET_DSA_TAG_DSA
	[DSA_TAG_PROTO_DSA] = &dsa_netdev_ops,
#endif
#ifdef CONFIG_NET_DSA_TAG_EDSA
	[DSA_TAG_PROTO_EDSA] = &edsa_netdev_ops,
#endif
#ifdef CONFIG_NET_DSA_TAG_KSZ
	[DSA_TAG_PROTO_KSZ] = &ksz_netdev_ops,
#endif
#ifdef CONFIG_NET_DSA_TAG_LAN9303
	[DSA_TAG_PROTO_LAN9303] = &lan9303_netdev_ops,
#endif
#ifdef CONFIG_NET_DSA_TAG_MTK
	[DSA_TAG_PROTO_MTK] = &mtk_netdev_ops,
#endif
#ifdef CONFIG_NET_DSA_TAG_QCA
	[DSA_TAG_PROTO_QCA] = &qca_netdev_ops,
#endif
#ifdef CONFIG_NET_DSA_TAG_TRAILER
	[DSA_TAG_PROTO_TRAILER] = &trailer_netdev_ops,
#endif
	[DSA_TAG_PROTO_NONE] = &none_ops,
};

const struct dsa_device_ops *dsa_resolve_tag_protocol(int tag_protocol)
{
	const struct dsa_device_ops *ops;

	if (tag_protocol >= DSA_TAG_LAST)
		return ERR_PTR(-EINVAL);
	ops = dsa_device_ops[tag_protocol];

	if (!ops)
		return ERR_PTR(-ENOPROTOOPT);
=======
DSA_TAG_DRIVER(none_ops);

static void dsa_tag_driver_register(struct dsa_tag_driver *dsa_tag_driver,
				    struct module *owner)
{
	dsa_tag_driver->owner = owner;

	mutex_lock(&dsa_tag_drivers_lock);
	list_add_tail(&dsa_tag_driver->list, &dsa_tag_drivers_list);
	mutex_unlock(&dsa_tag_drivers_lock);
}

void dsa_tag_drivers_register(struct dsa_tag_driver *dsa_tag_driver_array[],
			      unsigned int count, struct module *owner)
{
	unsigned int i;

	for (i = 0; i < count; i++)
		dsa_tag_driver_register(dsa_tag_driver_array[i], owner);
}

static void dsa_tag_driver_unregister(struct dsa_tag_driver *dsa_tag_driver)
{
	mutex_lock(&dsa_tag_drivers_lock);
	list_del(&dsa_tag_driver->list);
	mutex_unlock(&dsa_tag_drivers_lock);
}
EXPORT_SYMBOL_GPL(dsa_tag_drivers_register);

void dsa_tag_drivers_unregister(struct dsa_tag_driver *dsa_tag_driver_array[],
				unsigned int count)
{
	unsigned int i;

	for (i = 0; i < count; i++)
		dsa_tag_driver_unregister(dsa_tag_driver_array[i]);
}
EXPORT_SYMBOL_GPL(dsa_tag_drivers_unregister);

const char *dsa_tag_protocol_to_str(const struct dsa_device_ops *ops)
{
	return ops->name;
};

/* Function takes a reference on the module owning the tagger,
 * so dsa_tag_driver_put must be called afterwards.
 */
const struct dsa_device_ops *dsa_find_tagger_by_name(const char *buf)
{
	const struct dsa_device_ops *ops = ERR_PTR(-ENOPROTOOPT);
	struct dsa_tag_driver *dsa_tag_driver;

	mutex_lock(&dsa_tag_drivers_lock);
	list_for_each_entry(dsa_tag_driver, &dsa_tag_drivers_list, list) {
		const struct dsa_device_ops *tmp = dsa_tag_driver->ops;

		if (!sysfs_streq(buf, tmp->name))
			continue;

		if (!try_module_get(dsa_tag_driver->owner))
			break;

		ops = tmp;
		break;
	}
	mutex_unlock(&dsa_tag_drivers_lock);
>>>>>>> upstream/android-13

	return ops;
}

<<<<<<< HEAD
=======
const struct dsa_device_ops *dsa_tag_driver_get(int tag_protocol)
{
	struct dsa_tag_driver *dsa_tag_driver;
	const struct dsa_device_ops *ops;
	bool found = false;

	request_module("%s%d", DSA_TAG_DRIVER_ALIAS, tag_protocol);

	mutex_lock(&dsa_tag_drivers_lock);
	list_for_each_entry(dsa_tag_driver, &dsa_tag_drivers_list, list) {
		ops = dsa_tag_driver->ops;
		if (ops->proto == tag_protocol) {
			found = true;
			break;
		}
	}

	if (found) {
		if (!try_module_get(dsa_tag_driver->owner))
			ops = ERR_PTR(-ENOPROTOOPT);
	} else {
		ops = ERR_PTR(-ENOPROTOOPT);
	}

	mutex_unlock(&dsa_tag_drivers_lock);

	return ops;
}

void dsa_tag_driver_put(const struct dsa_device_ops *ops)
{
	struct dsa_tag_driver *dsa_tag_driver;

	mutex_lock(&dsa_tag_drivers_lock);
	list_for_each_entry(dsa_tag_driver, &dsa_tag_drivers_list, list) {
		if (dsa_tag_driver->ops == ops) {
			module_put(dsa_tag_driver->owner);
			break;
		}
	}
	mutex_unlock(&dsa_tag_drivers_lock);
}

>>>>>>> upstream/android-13
static int dev_is_class(struct device *dev, void *class)
{
	if (dev->class != NULL && !strcmp(dev->class->name, class))
		return 1;

	return 0;
}

static struct device *dev_find_class(struct device *parent, char *class)
{
	if (dev_is_class(parent, class)) {
		get_device(parent);
		return parent;
	}

	return device_find_child(parent, class, dev_is_class);
}

struct net_device *dsa_dev_to_net_device(struct device *dev)
{
	struct device *d;

	d = dev_find_class(dev, "net");
	if (d != NULL) {
		struct net_device *nd;

		nd = to_net_dev(d);
		dev_hold(nd);
		put_device(d);

		return nd;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(dsa_dev_to_net_device);

/* Determine if we should defer delivery of skb until we have a rx timestamp.
 *
 * Called from dsa_switch_rcv. For now, this will only work if tagging is
 * enabled on the switch. Normally the MAC driver would retrieve the hardware
 * timestamp when it reads the packet out of the hardware. However in a DSA
 * switch, the DSA driver owning the interface to which the packet is
 * delivered is never notified unless we do so here.
 */
static bool dsa_skb_defer_rx_timestamp(struct dsa_slave_priv *p,
				       struct sk_buff *skb)
{
	struct dsa_switch *ds = p->dp->ds;
	unsigned int type;

	if (skb_headroom(skb) < ETH_HLEN)
		return false;

	__skb_push(skb, ETH_HLEN);

	type = ptp_classify_raw(skb);

	__skb_pull(skb, ETH_HLEN);

	if (type == PTP_CLASS_NONE)
		return false;

	if (likely(ds->ops->port_rxtstamp))
		return ds->ops->port_rxtstamp(ds, p->dp->index, skb, type);

	return false;
}

static int dsa_switch_rcv(struct sk_buff *skb, struct net_device *dev,
			  struct packet_type *pt, struct net_device *unused)
{
	struct dsa_port *cpu_dp = dev->dsa_ptr;
	struct sk_buff *nskb = NULL;
<<<<<<< HEAD
	struct pcpu_sw_netstats *s;
=======
>>>>>>> upstream/android-13
	struct dsa_slave_priv *p;

	if (unlikely(!cpu_dp)) {
		kfree_skb(skb);
		return 0;
	}

	skb = skb_unshare(skb, GFP_ATOMIC);
	if (!skb)
		return 0;

<<<<<<< HEAD
	nskb = cpu_dp->rcv(skb, dev, pt);
=======
	nskb = cpu_dp->rcv(skb, dev);
>>>>>>> upstream/android-13
	if (!nskb) {
		kfree_skb(skb);
		return 0;
	}

	skb = nskb;
<<<<<<< HEAD
	p = netdev_priv(skb->dev);
=======
>>>>>>> upstream/android-13
	skb_push(skb, ETH_HLEN);
	skb->pkt_type = PACKET_HOST;
	skb->protocol = eth_type_trans(skb, skb->dev);

<<<<<<< HEAD
	s = this_cpu_ptr(p->stats64);
	u64_stats_update_begin(&s->syncp);
	s->rx_packets++;
	s->rx_bytes += skb->len;
	u64_stats_update_end(&s->syncp);
=======
	if (unlikely(!dsa_slave_dev_check(skb->dev))) {
		/* Packet is to be injected directly on an upper
		 * device, e.g. a team/bond, so skip all DSA-port
		 * specific actions.
		 */
		netif_rx(skb);
		return 0;
	}

	p = netdev_priv(skb->dev);

	if (unlikely(cpu_dp->ds->untag_bridge_pvid)) {
		nskb = dsa_untag_bridge_pvid(skb);
		if (!nskb) {
			kfree_skb(skb);
			return 0;
		}
		skb = nskb;
	}

	dev_sw_netstats_rx_add(skb->dev, skb->len);
>>>>>>> upstream/android-13

	if (dsa_skb_defer_rx_timestamp(p, skb))
		return 0;

	gro_cells_receive(&p->gcells, skb);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static bool dsa_is_port_initialized(struct dsa_switch *ds, int p)
{
<<<<<<< HEAD
	return dsa_is_user_port(ds, p) && ds->ports[p].slave;
=======
	const struct dsa_port *dp = dsa_to_port(ds, p);

	return dp->type == DSA_PORT_TYPE_USER && dp->slave;
>>>>>>> upstream/android-13
}

int dsa_switch_suspend(struct dsa_switch *ds)
{
	int i, ret = 0;

	/* Suspend slave network devices */
	for (i = 0; i < ds->num_ports; i++) {
		if (!dsa_is_port_initialized(ds, i))
			continue;

<<<<<<< HEAD
		ret = dsa_slave_suspend(ds->ports[i].slave);
=======
		ret = dsa_slave_suspend(dsa_to_port(ds, i)->slave);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

	if (ds->ops->suspend)
		ret = ds->ops->suspend(ds);

	return ret;
}
EXPORT_SYMBOL_GPL(dsa_switch_suspend);

int dsa_switch_resume(struct dsa_switch *ds)
{
	int i, ret = 0;

	if (ds->ops->resume)
		ret = ds->ops->resume(ds);

	if (ret)
		return ret;

	/* Resume slave network devices */
	for (i = 0; i < ds->num_ports; i++) {
		if (!dsa_is_port_initialized(ds, i))
			continue;

<<<<<<< HEAD
		ret = dsa_slave_resume(ds->ports[i].slave);
=======
		ret = dsa_slave_resume(dsa_to_port(ds, i)->slave);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(dsa_switch_resume);
#endif

static struct packet_type dsa_pack_type __read_mostly = {
	.type	= cpu_to_be16(ETH_P_XDSA),
	.func	= dsa_switch_rcv,
};

static struct workqueue_struct *dsa_owq;

bool dsa_schedule_work(struct work_struct *work)
{
	return queue_work(dsa_owq, work);
}

<<<<<<< HEAD
static ATOMIC_NOTIFIER_HEAD(dsa_notif_chain);

int register_dsa_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&dsa_notif_chain, nb);
}
EXPORT_SYMBOL_GPL(register_dsa_notifier);

int unregister_dsa_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&dsa_notif_chain, nb);
}
EXPORT_SYMBOL_GPL(unregister_dsa_notifier);

int call_dsa_notifiers(unsigned long val, struct net_device *dev,
		       struct dsa_notifier_info *info)
{
	info->dev = dev;
	return atomic_notifier_call_chain(&dsa_notif_chain, val, info);
}
EXPORT_SYMBOL_GPL(call_dsa_notifiers);
=======
void dsa_flush_workqueue(void)
{
	flush_workqueue(dsa_owq);
}

int dsa_devlink_param_get(struct devlink *dl, u32 id,
			  struct devlink_param_gset_ctx *ctx)
{
	struct dsa_switch *ds = dsa_devlink_to_ds(dl);

	if (!ds->ops->devlink_param_get)
		return -EOPNOTSUPP;

	return ds->ops->devlink_param_get(ds, id, ctx);
}
EXPORT_SYMBOL_GPL(dsa_devlink_param_get);

int dsa_devlink_param_set(struct devlink *dl, u32 id,
			  struct devlink_param_gset_ctx *ctx)
{
	struct dsa_switch *ds = dsa_devlink_to_ds(dl);

	if (!ds->ops->devlink_param_set)
		return -EOPNOTSUPP;

	return ds->ops->devlink_param_set(ds, id, ctx);
}
EXPORT_SYMBOL_GPL(dsa_devlink_param_set);

int dsa_devlink_params_register(struct dsa_switch *ds,
				const struct devlink_param *params,
				size_t params_count)
{
	return devlink_params_register(ds->devlink, params, params_count);
}
EXPORT_SYMBOL_GPL(dsa_devlink_params_register);

void dsa_devlink_params_unregister(struct dsa_switch *ds,
				   const struct devlink_param *params,
				   size_t params_count)
{
	devlink_params_unregister(ds->devlink, params, params_count);
}
EXPORT_SYMBOL_GPL(dsa_devlink_params_unregister);

int dsa_devlink_resource_register(struct dsa_switch *ds,
				  const char *resource_name,
				  u64 resource_size,
				  u64 resource_id,
				  u64 parent_resource_id,
				  const struct devlink_resource_size_params *size_params)
{
	return devlink_resource_register(ds->devlink, resource_name,
					 resource_size, resource_id,
					 parent_resource_id,
					 size_params);
}
EXPORT_SYMBOL_GPL(dsa_devlink_resource_register);

void dsa_devlink_resources_unregister(struct dsa_switch *ds)
{
	devlink_resources_unregister(ds->devlink, NULL);
}
EXPORT_SYMBOL_GPL(dsa_devlink_resources_unregister);

void dsa_devlink_resource_occ_get_register(struct dsa_switch *ds,
					   u64 resource_id,
					   devlink_resource_occ_get_t *occ_get,
					   void *occ_get_priv)
{
	return devlink_resource_occ_get_register(ds->devlink, resource_id,
						 occ_get, occ_get_priv);
}
EXPORT_SYMBOL_GPL(dsa_devlink_resource_occ_get_register);

void dsa_devlink_resource_occ_get_unregister(struct dsa_switch *ds,
					     u64 resource_id)
{
	devlink_resource_occ_get_unregister(ds->devlink, resource_id);
}
EXPORT_SYMBOL_GPL(dsa_devlink_resource_occ_get_unregister);

struct devlink_region *
dsa_devlink_region_create(struct dsa_switch *ds,
			  const struct devlink_region_ops *ops,
			  u32 region_max_snapshots, u64 region_size)
{
	return devlink_region_create(ds->devlink, ops, region_max_snapshots,
				     region_size);
}
EXPORT_SYMBOL_GPL(dsa_devlink_region_create);

struct devlink_region *
dsa_devlink_port_region_create(struct dsa_switch *ds,
			       int port,
			       const struct devlink_port_region_ops *ops,
			       u32 region_max_snapshots, u64 region_size)
{
	struct dsa_port *dp = dsa_to_port(ds, port);

	return devlink_port_region_create(&dp->devlink_port, ops,
					  region_max_snapshots,
					  region_size);
}
EXPORT_SYMBOL_GPL(dsa_devlink_port_region_create);

void dsa_devlink_region_destroy(struct devlink_region *region)
{
	devlink_region_destroy(region);
}
EXPORT_SYMBOL_GPL(dsa_devlink_region_destroy);

struct dsa_port *dsa_port_from_netdev(struct net_device *netdev)
{
	if (!netdev || !dsa_slave_dev_check(netdev))
		return ERR_PTR(-ENODEV);

	return dsa_slave_to_port(netdev);
}
EXPORT_SYMBOL_GPL(dsa_port_from_netdev);
>>>>>>> upstream/android-13

static int __init dsa_init_module(void)
{
	int rc;

	dsa_owq = alloc_ordered_workqueue("dsa_ordered",
					  WQ_MEM_RECLAIM);
	if (!dsa_owq)
		return -ENOMEM;

	rc = dsa_slave_register_notifier();
	if (rc)
		goto register_notifier_fail;

<<<<<<< HEAD
	rc = dsa_legacy_register();
	if (rc)
		goto legacy_register_fail;

	dev_add_pack(&dsa_pack_type);

	return 0;

legacy_register_fail:
	dsa_slave_unregister_notifier();
=======
	dev_add_pack(&dsa_pack_type);

	dsa_tag_driver_register(&DSA_TAG_DRIVER_NAME(none_ops),
				THIS_MODULE);

	return 0;

>>>>>>> upstream/android-13
register_notifier_fail:
	destroy_workqueue(dsa_owq);

	return rc;
}
module_init(dsa_init_module);

static void __exit dsa_cleanup_module(void)
{
<<<<<<< HEAD
	dsa_slave_unregister_notifier();
	dev_remove_pack(&dsa_pack_type);
	dsa_legacy_unregister();
=======
	dsa_tag_driver_unregister(&DSA_TAG_DRIVER_NAME(none_ops));

	dsa_slave_unregister_notifier();
	dev_remove_pack(&dsa_pack_type);
>>>>>>> upstream/android-13
	destroy_workqueue(dsa_owq);
}
module_exit(dsa_cleanup_module);

MODULE_AUTHOR("Lennert Buytenhek <buytenh@wantstofly.org>");
MODULE_DESCRIPTION("Driver for Distributed Switch Architecture switch chips");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:dsa");
