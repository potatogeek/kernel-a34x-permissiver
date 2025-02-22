<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Handling of a single switch chip, part of a switch fabric
 *
 * Copyright (c) 2017 Savoir-faire Linux Inc.
 *	Vivien Didelot <vivien.didelot@savoirfairelinux.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/netdevice.h>
#include <linux/notifier.h>
=======
 */

#include <linux/if_bridge.h>
#include <linux/netdevice.h>
#include <linux/notifier.h>
#include <linux/if_vlan.h>
>>>>>>> upstream/android-13
#include <net/switchdev.h>

#include "dsa_priv.h"

static unsigned int dsa_switch_fastest_ageing_time(struct dsa_switch *ds,
						   unsigned int ageing_time)
{
	int i;

	for (i = 0; i < ds->num_ports; ++i) {
<<<<<<< HEAD
		struct dsa_port *dp = &ds->ports[i];
=======
		struct dsa_port *dp = dsa_to_port(ds, i);
>>>>>>> upstream/android-13

		if (dp->ageing_time && dp->ageing_time < ageing_time)
			ageing_time = dp->ageing_time;
	}

	return ageing_time;
}

static int dsa_switch_ageing_time(struct dsa_switch *ds,
				  struct dsa_notifier_ageing_time_info *info)
{
	unsigned int ageing_time = info->ageing_time;
<<<<<<< HEAD
	struct switchdev_trans *trans = info->trans;

	if (switchdev_trans_ph_prepare(trans)) {
		if (ds->ageing_time_min && ageing_time < ds->ageing_time_min)
			return -ERANGE;
		if (ds->ageing_time_max && ageing_time > ds->ageing_time_max)
			return -ERANGE;
		return 0;
	}
=======

	if (ds->ageing_time_min && ageing_time < ds->ageing_time_min)
		return -ERANGE;

	if (ds->ageing_time_max && ageing_time > ds->ageing_time_max)
		return -ERANGE;
>>>>>>> upstream/android-13

	/* Program the fastest ageing time in case of multiple bridges */
	ageing_time = dsa_switch_fastest_ageing_time(ds, ageing_time);

	if (ds->ops->set_ageing_time)
		return ds->ops->set_ageing_time(ds, ageing_time);

	return 0;
}

<<<<<<< HEAD
static int dsa_switch_bridge_join(struct dsa_switch *ds,
				  struct dsa_notifier_bridge_info *info)
{
	if (ds->index == info->sw_index && ds->ops->port_bridge_join)
		return ds->ops->port_bridge_join(ds, info->port, info->br);

	if (ds->index != info->sw_index && ds->ops->crosschip_bridge_join)
		return ds->ops->crosschip_bridge_join(ds, info->sw_index,
						      info->port, info->br);

	return 0;
=======
static bool dsa_switch_mtu_match(struct dsa_switch *ds, int port,
				 struct dsa_notifier_mtu_info *info)
{
	if (ds->index == info->sw_index && port == info->port)
		return true;

	/* Do not propagate to other switches in the tree if the notifier was
	 * targeted for a single switch.
	 */
	if (info->targeted_match)
		return false;

	if (dsa_is_dsa_port(ds, port) || dsa_is_cpu_port(ds, port))
		return true;

	return false;
}

static int dsa_switch_mtu(struct dsa_switch *ds,
			  struct dsa_notifier_mtu_info *info)
{
	int port, ret;

	if (!ds->ops->port_change_mtu)
		return -EOPNOTSUPP;

	for (port = 0; port < ds->num_ports; port++) {
		if (dsa_switch_mtu_match(ds, port, info)) {
			ret = ds->ops->port_change_mtu(ds, port, info->mtu);
			if (ret)
				return ret;
		}
	}

	return 0;
}

static int dsa_switch_bridge_join(struct dsa_switch *ds,
				  struct dsa_notifier_bridge_info *info)
{
	struct dsa_switch_tree *dst = ds->dst;
	int err;

	if (dst->index == info->tree_index && ds->index == info->sw_index) {
		if (!ds->ops->port_bridge_join)
			return -EOPNOTSUPP;

		err = ds->ops->port_bridge_join(ds, info->port, info->br);
		if (err)
			return err;
	}

	if ((dst->index != info->tree_index || ds->index != info->sw_index) &&
	    ds->ops->crosschip_bridge_join) {
		err = ds->ops->crosschip_bridge_join(ds, info->tree_index,
						     info->sw_index,
						     info->port, info->br);
		if (err)
			return err;
	}

	return dsa_tag_8021q_bridge_join(ds, info);
>>>>>>> upstream/android-13
}

static int dsa_switch_bridge_leave(struct dsa_switch *ds,
				   struct dsa_notifier_bridge_info *info)
{
<<<<<<< HEAD
	if (ds->index == info->sw_index && ds->ops->port_bridge_leave)
		ds->ops->port_bridge_leave(ds, info->port, info->br);

	if (ds->index != info->sw_index && ds->ops->crosschip_bridge_leave)
		ds->ops->crosschip_bridge_leave(ds, info->sw_index, info->port,
						info->br);

	return 0;
}

=======
	struct dsa_switch_tree *dst = ds->dst;
	struct netlink_ext_ack extack = {0};
	bool change_vlan_filtering = false;
	bool vlan_filtering;
	int err, port;

	if (dst->index == info->tree_index && ds->index == info->sw_index &&
	    ds->ops->port_bridge_leave)
		ds->ops->port_bridge_leave(ds, info->port, info->br);

	if ((dst->index != info->tree_index || ds->index != info->sw_index) &&
	    ds->ops->crosschip_bridge_leave)
		ds->ops->crosschip_bridge_leave(ds, info->tree_index,
						info->sw_index, info->port,
						info->br);

	if (ds->needs_standalone_vlan_filtering && !br_vlan_enabled(info->br)) {
		change_vlan_filtering = true;
		vlan_filtering = true;
	} else if (!ds->needs_standalone_vlan_filtering &&
		   br_vlan_enabled(info->br)) {
		change_vlan_filtering = true;
		vlan_filtering = false;
	}

	/* If the bridge was vlan_filtering, the bridge core doesn't trigger an
	 * event for changing vlan_filtering setting upon slave ports leaving
	 * it. That is a good thing, because that lets us handle it and also
	 * handle the case where the switch's vlan_filtering setting is global
	 * (not per port). When that happens, the correct moment to trigger the
	 * vlan_filtering callback is only when the last port leaves the last
	 * VLAN-aware bridge.
	 */
	if (change_vlan_filtering && ds->vlan_filtering_is_global) {
		for (port = 0; port < ds->num_ports; port++) {
			struct net_device *bridge_dev;

			bridge_dev = dsa_to_port(ds, port)->bridge_dev;

			if (bridge_dev && br_vlan_enabled(bridge_dev)) {
				change_vlan_filtering = false;
				break;
			}
		}
	}

	if (change_vlan_filtering) {
		err = dsa_port_vlan_filtering(dsa_to_port(ds, info->port),
					      vlan_filtering, &extack);
		if (extack._msg)
			dev_err(ds->dev, "port %d: %s\n", info->port,
				extack._msg);
		if (err && err != -EOPNOTSUPP)
			return err;
	}

	return dsa_tag_8021q_bridge_leave(ds, info);
}

/* Matches for all upstream-facing ports (the CPU port and all upstream-facing
 * DSA links) that sit between the targeted port on which the notifier was
 * emitted and its dedicated CPU port.
 */
static bool dsa_switch_host_address_match(struct dsa_switch *ds, int port,
					  int info_sw_index, int info_port)
{
	struct dsa_port *targeted_dp, *cpu_dp;
	struct dsa_switch *targeted_ds;

	targeted_ds = dsa_switch_find(ds->dst->index, info_sw_index);
	targeted_dp = dsa_to_port(targeted_ds, info_port);
	cpu_dp = targeted_dp->cpu_dp;

	if (dsa_switch_is_upstream_of(ds, targeted_ds))
		return port == dsa_towards_port(ds, cpu_dp->ds->index,
						cpu_dp->index);

	return false;
}

static struct dsa_mac_addr *dsa_mac_addr_find(struct list_head *addr_list,
					      const unsigned char *addr,
					      u16 vid)
{
	struct dsa_mac_addr *a;

	list_for_each_entry(a, addr_list, list)
		if (ether_addr_equal(a->addr, addr) && a->vid == vid)
			return a;

	return NULL;
}

static int dsa_switch_do_mdb_add(struct dsa_switch *ds, int port,
				 const struct switchdev_obj_port_mdb *mdb)
{
	struct dsa_port *dp = dsa_to_port(ds, port);
	struct dsa_mac_addr *a;
	int err;

	/* No need to bother with refcounting for user ports */
	if (!(dsa_port_is_cpu(dp) || dsa_port_is_dsa(dp)))
		return ds->ops->port_mdb_add(ds, port, mdb);

	a = dsa_mac_addr_find(&dp->mdbs, mdb->addr, mdb->vid);
	if (a) {
		refcount_inc(&a->refcount);
		return 0;
	}

	a = kzalloc(sizeof(*a), GFP_KERNEL);
	if (!a)
		return -ENOMEM;

	err = ds->ops->port_mdb_add(ds, port, mdb);
	if (err) {
		kfree(a);
		return err;
	}

	ether_addr_copy(a->addr, mdb->addr);
	a->vid = mdb->vid;
	refcount_set(&a->refcount, 1);
	list_add_tail(&a->list, &dp->mdbs);

	return 0;
}

static int dsa_switch_do_mdb_del(struct dsa_switch *ds, int port,
				 const struct switchdev_obj_port_mdb *mdb)
{
	struct dsa_port *dp = dsa_to_port(ds, port);
	struct dsa_mac_addr *a;
	int err;

	/* No need to bother with refcounting for user ports */
	if (!(dsa_port_is_cpu(dp) || dsa_port_is_dsa(dp)))
		return ds->ops->port_mdb_del(ds, port, mdb);

	a = dsa_mac_addr_find(&dp->mdbs, mdb->addr, mdb->vid);
	if (!a)
		return -ENOENT;

	if (!refcount_dec_and_test(&a->refcount))
		return 0;

	err = ds->ops->port_mdb_del(ds, port, mdb);
	if (err) {
		refcount_set(&a->refcount, 1);
		return err;
	}

	list_del(&a->list);
	kfree(a);

	return 0;
}

static int dsa_switch_do_fdb_add(struct dsa_switch *ds, int port,
				 const unsigned char *addr, u16 vid)
{
	struct dsa_port *dp = dsa_to_port(ds, port);
	struct dsa_mac_addr *a;
	int err;

	/* No need to bother with refcounting for user ports */
	if (!(dsa_port_is_cpu(dp) || dsa_port_is_dsa(dp)))
		return ds->ops->port_fdb_add(ds, port, addr, vid);

	a = dsa_mac_addr_find(&dp->fdbs, addr, vid);
	if (a) {
		refcount_inc(&a->refcount);
		return 0;
	}

	a = kzalloc(sizeof(*a), GFP_KERNEL);
	if (!a)
		return -ENOMEM;

	err = ds->ops->port_fdb_add(ds, port, addr, vid);
	if (err) {
		kfree(a);
		return err;
	}

	ether_addr_copy(a->addr, addr);
	a->vid = vid;
	refcount_set(&a->refcount, 1);
	list_add_tail(&a->list, &dp->fdbs);

	return 0;
}

static int dsa_switch_do_fdb_del(struct dsa_switch *ds, int port,
				 const unsigned char *addr, u16 vid)
{
	struct dsa_port *dp = dsa_to_port(ds, port);
	struct dsa_mac_addr *a;
	int err;

	/* No need to bother with refcounting for user ports */
	if (!(dsa_port_is_cpu(dp) || dsa_port_is_dsa(dp)))
		return ds->ops->port_fdb_del(ds, port, addr, vid);

	a = dsa_mac_addr_find(&dp->fdbs, addr, vid);
	if (!a)
		return -ENOENT;

	if (!refcount_dec_and_test(&a->refcount))
		return 0;

	err = ds->ops->port_fdb_del(ds, port, addr, vid);
	if (err) {
		refcount_set(&a->refcount, 1);
		return err;
	}

	list_del(&a->list);
	kfree(a);

	return 0;
}

static int dsa_switch_host_fdb_add(struct dsa_switch *ds,
				   struct dsa_notifier_fdb_info *info)
{
	int err = 0;
	int port;

	if (!ds->ops->port_fdb_add)
		return -EOPNOTSUPP;

	for (port = 0; port < ds->num_ports; port++) {
		if (dsa_switch_host_address_match(ds, port, info->sw_index,
						  info->port)) {
			err = dsa_switch_do_fdb_add(ds, port, info->addr,
						    info->vid);
			if (err)
				break;
		}
	}

	return err;
}

static int dsa_switch_host_fdb_del(struct dsa_switch *ds,
				   struct dsa_notifier_fdb_info *info)
{
	int err = 0;
	int port;

	if (!ds->ops->port_fdb_del)
		return -EOPNOTSUPP;

	for (port = 0; port < ds->num_ports; port++) {
		if (dsa_switch_host_address_match(ds, port, info->sw_index,
						  info->port)) {
			err = dsa_switch_do_fdb_del(ds, port, info->addr,
						    info->vid);
			if (err)
				break;
		}
	}

	return err;
}

>>>>>>> upstream/android-13
static int dsa_switch_fdb_add(struct dsa_switch *ds,
			      struct dsa_notifier_fdb_info *info)
{
	int port = dsa_towards_port(ds, info->sw_index, info->port);

	if (!ds->ops->port_fdb_add)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return ds->ops->port_fdb_add(ds, port, info->addr, info->vid);
=======
	return dsa_switch_do_fdb_add(ds, port, info->addr, info->vid);
>>>>>>> upstream/android-13
}

static int dsa_switch_fdb_del(struct dsa_switch *ds,
			      struct dsa_notifier_fdb_info *info)
{
	int port = dsa_towards_port(ds, info->sw_index, info->port);

	if (!ds->ops->port_fdb_del)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return ds->ops->port_fdb_del(ds, port, info->addr, info->vid);
}

static int
dsa_switch_mdb_prepare_bitmap(struct dsa_switch *ds,
			      const struct switchdev_obj_port_mdb *mdb,
			      const unsigned long *bitmap)
{
	int port, err;

	if (!ds->ops->port_mdb_prepare || !ds->ops->port_mdb_add)
		return -EOPNOTSUPP;

	for_each_set_bit(port, bitmap, ds->num_ports) {
		err = ds->ops->port_mdb_prepare(ds, port, mdb);
		if (err)
			return err;
	}
=======
	return dsa_switch_do_fdb_del(ds, port, info->addr, info->vid);
}

static int dsa_switch_hsr_join(struct dsa_switch *ds,
			       struct dsa_notifier_hsr_info *info)
{
	if (ds->index == info->sw_index && ds->ops->port_hsr_join)
		return ds->ops->port_hsr_join(ds, info->port, info->hsr);

	return -EOPNOTSUPP;
}

static int dsa_switch_hsr_leave(struct dsa_switch *ds,
				struct dsa_notifier_hsr_info *info)
{
	if (ds->index == info->sw_index && ds->ops->port_hsr_leave)
		return ds->ops->port_hsr_leave(ds, info->port, info->hsr);

	return -EOPNOTSUPP;
}

static int dsa_switch_lag_change(struct dsa_switch *ds,
				 struct dsa_notifier_lag_info *info)
{
	if (ds->index == info->sw_index && ds->ops->port_lag_change)
		return ds->ops->port_lag_change(ds, info->port);

	if (ds->index != info->sw_index && ds->ops->crosschip_lag_change)
		return ds->ops->crosschip_lag_change(ds, info->sw_index,
						     info->port);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void dsa_switch_mdb_add_bitmap(struct dsa_switch *ds,
				      const struct switchdev_obj_port_mdb *mdb,
				      const unsigned long *bitmap)
{
	int port;

	if (!ds->ops->port_mdb_add)
		return;

	for_each_set_bit(port, bitmap, ds->num_ports)
		ds->ops->port_mdb_add(ds, port, mdb);
=======
static int dsa_switch_lag_join(struct dsa_switch *ds,
			       struct dsa_notifier_lag_info *info)
{
	if (ds->index == info->sw_index && ds->ops->port_lag_join)
		return ds->ops->port_lag_join(ds, info->port, info->lag,
					      info->info);

	if (ds->index != info->sw_index && ds->ops->crosschip_lag_join)
		return ds->ops->crosschip_lag_join(ds, info->sw_index,
						   info->port, info->lag,
						   info->info);

	return -EOPNOTSUPP;
}

static int dsa_switch_lag_leave(struct dsa_switch *ds,
				struct dsa_notifier_lag_info *info)
{
	if (ds->index == info->sw_index && ds->ops->port_lag_leave)
		return ds->ops->port_lag_leave(ds, info->port, info->lag);

	if (ds->index != info->sw_index && ds->ops->crosschip_lag_leave)
		return ds->ops->crosschip_lag_leave(ds, info->sw_index,
						    info->port, info->lag);

	return -EOPNOTSUPP;
>>>>>>> upstream/android-13
}

static int dsa_switch_mdb_add(struct dsa_switch *ds,
			      struct dsa_notifier_mdb_info *info)
{
<<<<<<< HEAD
	const struct switchdev_obj_port_mdb *mdb = info->mdb;
	struct switchdev_trans *trans = info->trans;
	int port;

	/* Build a mask of Multicast group members */
	bitmap_zero(ds->bitmap, ds->num_ports);
	if (ds->index == info->sw_index)
		set_bit(info->port, ds->bitmap);
	for (port = 0; port < ds->num_ports; port++)
		if (dsa_is_dsa_port(ds, port))
			set_bit(port, ds->bitmap);

	if (switchdev_trans_ph_prepare(trans))
		return dsa_switch_mdb_prepare_bitmap(ds, mdb, ds->bitmap);

	dsa_switch_mdb_add_bitmap(ds, mdb, ds->bitmap);

	return 0;
=======
	int port = dsa_towards_port(ds, info->sw_index, info->port);

	if (!ds->ops->port_mdb_add)
		return -EOPNOTSUPP;

	return dsa_switch_do_mdb_add(ds, port, info->mdb);
>>>>>>> upstream/android-13
}

static int dsa_switch_mdb_del(struct dsa_switch *ds,
			      struct dsa_notifier_mdb_info *info)
{
<<<<<<< HEAD
	const struct switchdev_obj_port_mdb *mdb = info->mdb;
=======
	int port = dsa_towards_port(ds, info->sw_index, info->port);
>>>>>>> upstream/android-13

	if (!ds->ops->port_mdb_del)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (ds->index == info->sw_index)
		return ds->ops->port_mdb_del(ds, info->port, mdb);

	return 0;
}

static int
dsa_switch_vlan_prepare_bitmap(struct dsa_switch *ds,
			       const struct switchdev_obj_port_vlan *vlan,
			       const unsigned long *bitmap)
{
	int port, err;

	if (!ds->ops->port_vlan_prepare || !ds->ops->port_vlan_add)
		return -EOPNOTSUPP;

	for_each_set_bit(port, bitmap, ds->num_ports) {
		err = ds->ops->port_vlan_prepare(ds, port, vlan);
		if (err)
			return err;
	}

	return 0;
}

static void
dsa_switch_vlan_add_bitmap(struct dsa_switch *ds,
			   const struct switchdev_obj_port_vlan *vlan,
			   const unsigned long *bitmap)
{
	int port;

	for_each_set_bit(port, bitmap, ds->num_ports)
		ds->ops->port_vlan_add(ds, port, vlan);
=======
	return dsa_switch_do_mdb_del(ds, port, info->mdb);
}

static int dsa_switch_host_mdb_add(struct dsa_switch *ds,
				   struct dsa_notifier_mdb_info *info)
{
	int err = 0;
	int port;

	if (!ds->ops->port_mdb_add)
		return -EOPNOTSUPP;

	for (port = 0; port < ds->num_ports; port++) {
		if (dsa_switch_host_address_match(ds, port, info->sw_index,
						  info->port)) {
			err = dsa_switch_do_mdb_add(ds, port, info->mdb);
			if (err)
				break;
		}
	}

	return err;
}

static int dsa_switch_host_mdb_del(struct dsa_switch *ds,
				   struct dsa_notifier_mdb_info *info)
{
	int err = 0;
	int port;

	if (!ds->ops->port_mdb_del)
		return -EOPNOTSUPP;

	for (port = 0; port < ds->num_ports; port++) {
		if (dsa_switch_host_address_match(ds, port, info->sw_index,
						  info->port)) {
			err = dsa_switch_do_mdb_del(ds, port, info->mdb);
			if (err)
				break;
		}
	}

	return err;
}

static bool dsa_switch_vlan_match(struct dsa_switch *ds, int port,
				  struct dsa_notifier_vlan_info *info)
{
	if (ds->index == info->sw_index && port == info->port)
		return true;

	if (dsa_is_dsa_port(ds, port))
		return true;

	return false;
>>>>>>> upstream/android-13
}

static int dsa_switch_vlan_add(struct dsa_switch *ds,
			       struct dsa_notifier_vlan_info *info)
{
<<<<<<< HEAD
	const struct switchdev_obj_port_vlan *vlan = info->vlan;
	struct switchdev_trans *trans = info->trans;
	int port;

	/* Build a mask of VLAN members */
	bitmap_zero(ds->bitmap, ds->num_ports);
	if (ds->index == info->sw_index)
		set_bit(info->port, ds->bitmap);
	for (port = 0; port < ds->num_ports; port++)
		if (dsa_is_cpu_port(ds, port) || dsa_is_dsa_port(ds, port))
			set_bit(port, ds->bitmap);

	if (switchdev_trans_ph_prepare(trans))
		return dsa_switch_vlan_prepare_bitmap(ds, vlan, ds->bitmap);

	dsa_switch_vlan_add_bitmap(ds, vlan, ds->bitmap);
=======
	int port, err;

	if (!ds->ops->port_vlan_add)
		return -EOPNOTSUPP;

	for (port = 0; port < ds->num_ports; port++) {
		if (dsa_switch_vlan_match(ds, port, info)) {
			err = ds->ops->port_vlan_add(ds, port, info->vlan,
						     info->extack);
			if (err)
				return err;
		}
	}
>>>>>>> upstream/android-13

	return 0;
}

static int dsa_switch_vlan_del(struct dsa_switch *ds,
			       struct dsa_notifier_vlan_info *info)
{
<<<<<<< HEAD
	const struct switchdev_obj_port_vlan *vlan = info->vlan;

=======
>>>>>>> upstream/android-13
	if (!ds->ops->port_vlan_del)
		return -EOPNOTSUPP;

	if (ds->index == info->sw_index)
<<<<<<< HEAD
		return ds->ops->port_vlan_del(ds, info->port, vlan);
=======
		return ds->ops->port_vlan_del(ds, info->port, info->vlan);

	/* Do not deprogram the DSA links as they may be used as conduit
	 * for other VLAN members in the fabric.
	 */
	return 0;
}

static int dsa_switch_change_tag_proto(struct dsa_switch *ds,
				       struct dsa_notifier_tag_proto_info *info)
{
	const struct dsa_device_ops *tag_ops = info->tag_ops;
	int port, err;

	if (!ds->ops->change_tag_protocol)
		return -EOPNOTSUPP;

	ASSERT_RTNL();

	for (port = 0; port < ds->num_ports; port++) {
		if (!dsa_is_cpu_port(ds, port))
			continue;

		err = ds->ops->change_tag_protocol(ds, port, tag_ops->proto);
		if (err)
			return err;

		dsa_port_set_tag_protocol(dsa_to_port(ds, port), tag_ops);
	}

	/* Now that changing the tag protocol can no longer fail, let's update
	 * the remaining bits which are "duplicated for faster access", and the
	 * bits that depend on the tagger, such as the MTU.
	 */
	for (port = 0; port < ds->num_ports; port++) {
		if (dsa_is_user_port(ds, port)) {
			struct net_device *slave;

			slave = dsa_to_port(ds, port)->slave;
			dsa_slave_setup_tagger(slave);

			/* rtnl_mutex is held in dsa_tree_change_tag_proto */
			dsa_slave_change_mtu(slave, slave->mtu);
		}
	}

	return 0;
}

static int dsa_switch_mrp_add(struct dsa_switch *ds,
			      struct dsa_notifier_mrp_info *info)
{
	if (!ds->ops->port_mrp_add)
		return -EOPNOTSUPP;

	if (ds->index == info->sw_index)
		return ds->ops->port_mrp_add(ds, info->port, info->mrp);

	return 0;
}

static int dsa_switch_mrp_del(struct dsa_switch *ds,
			      struct dsa_notifier_mrp_info *info)
{
	if (!ds->ops->port_mrp_del)
		return -EOPNOTSUPP;

	if (ds->index == info->sw_index)
		return ds->ops->port_mrp_del(ds, info->port, info->mrp);

	return 0;
}

static int
dsa_switch_mrp_add_ring_role(struct dsa_switch *ds,
			     struct dsa_notifier_mrp_ring_role_info *info)
{
	if (!ds->ops->port_mrp_add_ring_role)
		return -EOPNOTSUPP;

	if (ds->index == info->sw_index)
		return ds->ops->port_mrp_add_ring_role(ds, info->port,
						       info->mrp);

	return 0;
}

static int
dsa_switch_mrp_del_ring_role(struct dsa_switch *ds,
			     struct dsa_notifier_mrp_ring_role_info *info)
{
	if (!ds->ops->port_mrp_del_ring_role)
		return -EOPNOTSUPP;

	if (ds->index == info->sw_index)
		return ds->ops->port_mrp_del_ring_role(ds, info->port,
						       info->mrp);
>>>>>>> upstream/android-13

	return 0;
}

static int dsa_switch_event(struct notifier_block *nb,
			    unsigned long event, void *info)
{
	struct dsa_switch *ds = container_of(nb, struct dsa_switch, nb);
	int err;

	switch (event) {
	case DSA_NOTIFIER_AGEING_TIME:
		err = dsa_switch_ageing_time(ds, info);
		break;
	case DSA_NOTIFIER_BRIDGE_JOIN:
		err = dsa_switch_bridge_join(ds, info);
		break;
	case DSA_NOTIFIER_BRIDGE_LEAVE:
		err = dsa_switch_bridge_leave(ds, info);
		break;
	case DSA_NOTIFIER_FDB_ADD:
		err = dsa_switch_fdb_add(ds, info);
		break;
	case DSA_NOTIFIER_FDB_DEL:
		err = dsa_switch_fdb_del(ds, info);
		break;
<<<<<<< HEAD
=======
	case DSA_NOTIFIER_HOST_FDB_ADD:
		err = dsa_switch_host_fdb_add(ds, info);
		break;
	case DSA_NOTIFIER_HOST_FDB_DEL:
		err = dsa_switch_host_fdb_del(ds, info);
		break;
	case DSA_NOTIFIER_HSR_JOIN:
		err = dsa_switch_hsr_join(ds, info);
		break;
	case DSA_NOTIFIER_HSR_LEAVE:
		err = dsa_switch_hsr_leave(ds, info);
		break;
	case DSA_NOTIFIER_LAG_CHANGE:
		err = dsa_switch_lag_change(ds, info);
		break;
	case DSA_NOTIFIER_LAG_JOIN:
		err = dsa_switch_lag_join(ds, info);
		break;
	case DSA_NOTIFIER_LAG_LEAVE:
		err = dsa_switch_lag_leave(ds, info);
		break;
>>>>>>> upstream/android-13
	case DSA_NOTIFIER_MDB_ADD:
		err = dsa_switch_mdb_add(ds, info);
		break;
	case DSA_NOTIFIER_MDB_DEL:
		err = dsa_switch_mdb_del(ds, info);
		break;
<<<<<<< HEAD
=======
	case DSA_NOTIFIER_HOST_MDB_ADD:
		err = dsa_switch_host_mdb_add(ds, info);
		break;
	case DSA_NOTIFIER_HOST_MDB_DEL:
		err = dsa_switch_host_mdb_del(ds, info);
		break;
>>>>>>> upstream/android-13
	case DSA_NOTIFIER_VLAN_ADD:
		err = dsa_switch_vlan_add(ds, info);
		break;
	case DSA_NOTIFIER_VLAN_DEL:
		err = dsa_switch_vlan_del(ds, info);
		break;
<<<<<<< HEAD
=======
	case DSA_NOTIFIER_MTU:
		err = dsa_switch_mtu(ds, info);
		break;
	case DSA_NOTIFIER_TAG_PROTO:
		err = dsa_switch_change_tag_proto(ds, info);
		break;
	case DSA_NOTIFIER_MRP_ADD:
		err = dsa_switch_mrp_add(ds, info);
		break;
	case DSA_NOTIFIER_MRP_DEL:
		err = dsa_switch_mrp_del(ds, info);
		break;
	case DSA_NOTIFIER_MRP_ADD_RING_ROLE:
		err = dsa_switch_mrp_add_ring_role(ds, info);
		break;
	case DSA_NOTIFIER_MRP_DEL_RING_ROLE:
		err = dsa_switch_mrp_del_ring_role(ds, info);
		break;
	case DSA_NOTIFIER_TAG_8021Q_VLAN_ADD:
		err = dsa_switch_tag_8021q_vlan_add(ds, info);
		break;
	case DSA_NOTIFIER_TAG_8021Q_VLAN_DEL:
		err = dsa_switch_tag_8021q_vlan_del(ds, info);
		break;
>>>>>>> upstream/android-13
	default:
		err = -EOPNOTSUPP;
		break;
	}

<<<<<<< HEAD
	/* Non-switchdev operations cannot be rolled back. If a DSA driver
	 * returns an error during the chained call, switch chips may be in an
	 * inconsistent state.
	 */
=======
>>>>>>> upstream/android-13
	if (err)
		dev_dbg(ds->dev, "breaking chain for DSA event %lu (%d)\n",
			event, err);

	return notifier_from_errno(err);
}

int dsa_switch_register_notifier(struct dsa_switch *ds)
{
	ds->nb.notifier_call = dsa_switch_event;

	return raw_notifier_chain_register(&ds->dst->nh, &ds->nb);
}

void dsa_switch_unregister_notifier(struct dsa_switch *ds)
{
	int err;

	err = raw_notifier_chain_unregister(&ds->dst->nh, &ds->nb);
	if (err)
		dev_err(ds->dev, "failed to unregister notifier (%d)\n", err);
}
