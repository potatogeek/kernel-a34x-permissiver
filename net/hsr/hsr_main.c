<<<<<<< HEAD
/* Copyright 2011-2014 Autronica Fire and Security AS
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * Author(s):
 *	2011-2014 Arvid Brodin, arvid.brodin@alten.se
 */

#include <linux/netdevice.h>
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright 2011-2014 Autronica Fire and Security AS
 *
 * Author(s):
 *	2011-2014 Arvid Brodin, arvid.brodin@alten.se
 *
 * Event handling for HSR and PRP devices.
 */

#include <linux/netdevice.h>
#include <net/rtnetlink.h>
>>>>>>> upstream/android-13
#include <linux/rculist.h>
#include <linux/timer.h>
#include <linux/etherdevice.h>
#include "hsr_main.h"
#include "hsr_device.h"
#include "hsr_netlink.h"
#include "hsr_framereg.h"
#include "hsr_slave.h"

<<<<<<< HEAD
=======
static bool hsr_slave_empty(struct hsr_priv *hsr)
{
	struct hsr_port *port;

	hsr_for_each_port(hsr, port)
		if (port->type != HSR_PT_MASTER)
			return false;
	return true;
}
>>>>>>> upstream/android-13

static int hsr_netdev_notify(struct notifier_block *nb, unsigned long event,
			     void *ptr)
{
<<<<<<< HEAD
	struct net_device *dev;
	struct hsr_port *port, *master;
	struct hsr_priv *hsr;
=======
	struct hsr_port *port, *master;
	struct net_device *dev;
	struct hsr_priv *hsr;
	LIST_HEAD(list_kill);
>>>>>>> upstream/android-13
	int mtu_max;
	int res;

	dev = netdev_notifier_info_to_dev(ptr);
	port = hsr_port_get_rtnl(dev);
<<<<<<< HEAD
	if (port == NULL) {
=======
	if (!port) {
>>>>>>> upstream/android-13
		if (!is_hsr_master(dev))
			return NOTIFY_DONE;	/* Not an HSR device */
		hsr = netdev_priv(dev);
		port = hsr_port_get_hsr(hsr, HSR_PT_MASTER);
<<<<<<< HEAD
		if (port == NULL) {
=======
		if (!port) {
>>>>>>> upstream/android-13
			/* Resend of notification concerning removed device? */
			return NOTIFY_DONE;
		}
	} else {
		hsr = port->hsr;
	}

	switch (event) {
	case NETDEV_UP:		/* Administrative state DOWN */
	case NETDEV_DOWN:	/* Administrative state UP */
	case NETDEV_CHANGE:	/* Link (carrier) state changes */
		hsr_check_carrier_and_operstate(hsr);
		break;
<<<<<<< HEAD
=======
	case NETDEV_CHANGENAME:
		if (is_hsr_master(dev))
			hsr_debugfs_rename(dev);
		break;
>>>>>>> upstream/android-13
	case NETDEV_CHANGEADDR:
		if (port->type == HSR_PT_MASTER) {
			/* This should not happen since there's no
			 * ndo_set_mac_address() for HSR devices - i.e. not
			 * supported.
			 */
			break;
		}

		master = hsr_port_get_hsr(hsr, HSR_PT_MASTER);

		if (port->type == HSR_PT_SLAVE_A) {
			ether_addr_copy(master->dev->dev_addr, dev->dev_addr);
<<<<<<< HEAD
			call_netdevice_notifiers(NETDEV_CHANGEADDR, master->dev);
=======
			call_netdevice_notifiers(NETDEV_CHANGEADDR,
						 master->dev);
>>>>>>> upstream/android-13
		}

		/* Make sure we recognize frames from ourselves in hsr_rcv() */
		port = hsr_port_get_hsr(hsr, HSR_PT_SLAVE_B);
<<<<<<< HEAD
		res = hsr_create_self_node(&hsr->self_node_db,
=======
		res = hsr_create_self_node(hsr,
>>>>>>> upstream/android-13
					   master->dev->dev_addr,
					   port ?
						port->dev->dev_addr :
						master->dev->dev_addr);
		if (res)
			netdev_warn(master->dev,
				    "Could not update HSR node address.\n");
		break;
	case NETDEV_CHANGEMTU:
		if (port->type == HSR_PT_MASTER)
			break; /* Handled in ndo_change_mtu() */
		mtu_max = hsr_get_max_mtu(port->hsr);
		master = hsr_port_get_hsr(port->hsr, HSR_PT_MASTER);
		master->dev->mtu = mtu_max;
		break;
	case NETDEV_UNREGISTER:
<<<<<<< HEAD
		hsr_del_port(port);
=======
		if (!is_hsr_master(dev)) {
			master = hsr_port_get_hsr(port->hsr, HSR_PT_MASTER);
			hsr_del_port(port);
			if (hsr_slave_empty(master->hsr)) {
				const struct rtnl_link_ops *ops;

				ops = master->dev->rtnl_link_ops;
				ops->dellink(master->dev, &list_kill);
				unregister_netdevice_many(&list_kill);
			}
		}
>>>>>>> upstream/android-13
		break;
	case NETDEV_PRE_TYPE_CHANGE:
		/* HSR works only on Ethernet devices. Refuse slave to change
		 * its type.
		 */
		return NOTIFY_BAD;
	}

	return NOTIFY_DONE;
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
struct hsr_port *hsr_port_get_hsr(struct hsr_priv *hsr, enum hsr_port_type pt)
{
	struct hsr_port *port;

	hsr_for_each_port(hsr, port)
		if (port->type == pt)
			return port;
	return NULL;
}

<<<<<<< HEAD
=======
int hsr_get_version(struct net_device *dev, enum hsr_version *ver)
{
	struct hsr_priv *hsr;

	hsr = netdev_priv(dev);
	*ver = hsr->prot_version;

	return 0;
}
EXPORT_SYMBOL(hsr_get_version);

>>>>>>> upstream/android-13
static struct notifier_block hsr_nb = {
	.notifier_call = hsr_netdev_notify,	/* Slave event notifications */
};

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
static int __init hsr_init(void)
{
	int res;

	BUILD_BUG_ON(sizeof(struct hsr_tag) != HSR_HLEN);

	register_netdevice_notifier(&hsr_nb);
	res = hsr_netlink_init();

	return res;
}

static void __exit hsr_exit(void)
{
<<<<<<< HEAD
	unregister_netdevice_notifier(&hsr_nb);
	hsr_netlink_exit();
=======
	hsr_netlink_exit();
	hsr_debugfs_remove_root();
	unregister_netdevice_notifier(&hsr_nb);
>>>>>>> upstream/android-13
}

module_init(hsr_init);
module_exit(hsr_exit);
MODULE_LICENSE("GPL");
