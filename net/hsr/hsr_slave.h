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
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright 2011-2014 Autronica Fire and Security AS
 *
 *	2011-2014 Arvid Brodin, arvid.brodin@alten.se
 *
 * include file for HSR and PRP.
>>>>>>> upstream/android-13
 */

#ifndef __HSR_SLAVE_H
#define __HSR_SLAVE_H

#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
#include "hsr_main.h"

int hsr_add_port(struct hsr_priv *hsr, struct net_device *dev,
<<<<<<< HEAD
		 enum hsr_port_type pt);
=======
		 enum hsr_port_type pt, struct netlink_ext_ack *extack);
>>>>>>> upstream/android-13
void hsr_del_port(struct hsr_port *port);
bool hsr_port_exists(const struct net_device *dev);

static inline struct hsr_port *hsr_port_get_rtnl(const struct net_device *dev)
{
	ASSERT_RTNL();
	return hsr_port_exists(dev) ?
				rtnl_dereference(dev->rx_handler_data) : NULL;
}

static inline struct hsr_port *hsr_port_get_rcu(const struct net_device *dev)
{
	return hsr_port_exists(dev) ?
				rcu_dereference(dev->rx_handler_data) : NULL;
}

<<<<<<< HEAD
=======
bool hsr_invalid_dan_ingress_frame(__be16 protocol);

>>>>>>> upstream/android-13
#endif /* __HSR_SLAVE_H */
