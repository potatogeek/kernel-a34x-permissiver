<<<<<<< HEAD
/* Copyright (c) 2013-2017, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * RMNET Data Virtual Network Device APIs
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2013-2017, The Linux Foundation. All rights reserved.
 *
 * RMNET Data Virtual Network Device APIs
>>>>>>> upstream/android-13
 */

#ifndef _RMNET_VND_H_
#define _RMNET_VND_H_

int rmnet_vnd_do_flow_control(struct net_device *dev, int enable);
int rmnet_vnd_newlink(u8 id, struct net_device *rmnet_dev,
		      struct rmnet_port *port,
		      struct net_device *real_dev,
<<<<<<< HEAD
		      struct rmnet_endpoint *ep);
=======
		      struct rmnet_endpoint *ep,
		      struct netlink_ext_ack *extack);
>>>>>>> upstream/android-13
int rmnet_vnd_dellink(u8 id, struct rmnet_port *port,
		      struct rmnet_endpoint *ep);
void rmnet_vnd_rx_fixup(struct sk_buff *skb, struct net_device *dev);
void rmnet_vnd_tx_fixup(struct sk_buff *skb, struct net_device *dev);
void rmnet_vnd_setup(struct net_device *dev);
<<<<<<< HEAD
=======
int rmnet_vnd_validate_real_dev_mtu(struct net_device *real_dev);
int rmnet_vnd_update_dev_mtu(struct rmnet_port *port,
			     struct net_device *real_dev);
>>>>>>> upstream/android-13
#endif /* _RMNET_VND_H_ */
