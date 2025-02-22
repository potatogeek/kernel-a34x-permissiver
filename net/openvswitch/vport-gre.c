<<<<<<< HEAD
/*
 * Copyright (c) 2007-2014 Nicira, Inc.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2007-2014 Nicira, Inc.
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/if.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/if_tunnel.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/in_route.h>
#include <linux/inetdevice.h>
#include <linux/jhash.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/rculist.h>
#include <net/route.h>
#include <net/xfrm.h>

#include <net/icmp.h>
#include <net/ip.h>
#include <net/ip_tunnels.h>
#include <net/gre.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include <net/protocol.h>

#include "datapath.h"
#include "vport.h"
#include "vport-netdev.h"

static struct vport_ops ovs_gre_vport_ops;

static struct vport *gre_tnl_create(const struct vport_parms *parms)
{
	struct net *net = ovs_dp_get_net(parms->dp);
	struct net_device *dev;
	struct vport *vport;
	int err;

	vport = ovs_vport_alloc(0, &ovs_gre_vport_ops, parms);
	if (IS_ERR(vport))
		return vport;

	rtnl_lock();
	dev = gretap_fb_dev_create(net, parms->name, NET_NAME_USER);
	if (IS_ERR(dev)) {
		rtnl_unlock();
		ovs_vport_free(vport);
		return ERR_CAST(dev);
	}

<<<<<<< HEAD
	err = dev_change_flags(dev, dev->flags | IFF_UP);
=======
	err = dev_change_flags(dev, dev->flags | IFF_UP, NULL);
>>>>>>> upstream/android-13
	if (err < 0) {
		rtnl_delete_link(dev);
		rtnl_unlock();
		ovs_vport_free(vport);
		return ERR_PTR(err);
	}

	rtnl_unlock();
	return vport;
}

static struct vport *gre_create(const struct vport_parms *parms)
{
	struct vport *vport;

	vport = gre_tnl_create(parms);
	if (IS_ERR(vport))
		return vport;

	return ovs_netdev_link(vport, parms->name);
}

static struct vport_ops ovs_gre_vport_ops = {
	.type		= OVS_VPORT_TYPE_GRE,
	.create		= gre_create,
	.send		= dev_queue_xmit,
	.destroy	= ovs_netdev_tunnel_destroy,
};

static int __init ovs_gre_tnl_init(void)
{
	return ovs_vport_ops_register(&ovs_gre_vport_ops);
}

static void __exit ovs_gre_tnl_exit(void)
{
	ovs_vport_ops_unregister(&ovs_gre_vport_ops);
}

module_init(ovs_gre_tnl_init);
module_exit(ovs_gre_tnl_exit);

MODULE_DESCRIPTION("OVS: GRE switching port");
MODULE_LICENSE("GPL");
MODULE_ALIAS("vport-type-3");
