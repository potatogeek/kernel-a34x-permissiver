<<<<<<< HEAD
/*
 * Copyright (c) 2007-2011 Nicira, Inc.
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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2007-2011 Nicira, Inc.
>>>>>>> upstream/android-13
 */

#ifndef VPORT_NETDEV_H
#define VPORT_NETDEV_H 1

#include <linux/netdevice.h>
#include <linux/rcupdate.h>

#include "vport.h"

struct vport *ovs_netdev_get_vport(struct net_device *dev);

struct vport *ovs_netdev_link(struct vport *vport, const char *name);
void ovs_netdev_detach_dev(struct vport *);

int __init ovs_netdev_init(void);
void ovs_netdev_exit(void);

void ovs_netdev_tunnel_destroy(struct vport *vport);
#endif /* vport_netdev.h */
