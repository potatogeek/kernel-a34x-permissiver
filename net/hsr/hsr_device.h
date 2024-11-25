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
 * Author(s):
 *	2011-2014 Arvid Brodin, arvid.brodin@alten.se
 *
 * include file for HSR and PRP.
>>>>>>> upstream/android-13
 */

#ifndef __HSR_DEVICE_H
#define __HSR_DEVICE_H

#include <linux/netdevice.h>
#include "hsr_main.h"

<<<<<<< HEAD
void hsr_dev_setup(struct net_device *dev);
int hsr_dev_finalize(struct net_device *hsr_dev, struct net_device *slave[2],
		     unsigned char multicast_spec, u8 protocol_version);
void hsr_check_carrier_and_operstate(struct hsr_priv *hsr);
bool is_hsr_master(struct net_device *dev);
int hsr_get_max_mtu(struct hsr_priv *hsr);

=======
void hsr_del_ports(struct hsr_priv *hsr);
void hsr_dev_setup(struct net_device *dev);
int hsr_dev_finalize(struct net_device *hsr_dev, struct net_device *slave[2],
		     unsigned char multicast_spec, u8 protocol_version,
		     struct netlink_ext_ack *extack);
void hsr_check_carrier_and_operstate(struct hsr_priv *hsr);
int hsr_get_max_mtu(struct hsr_priv *hsr);
>>>>>>> upstream/android-13
#endif /* __HSR_DEVICE_H */
