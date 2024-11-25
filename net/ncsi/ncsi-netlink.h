<<<<<<< HEAD
/*
 * Copyright Samuel Mendoza-Jonas, IBM Corporation 2018.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright Samuel Mendoza-Jonas, IBM Corporation 2018.
>>>>>>> upstream/android-13
 */

#ifndef __NCSI_NETLINK_H__
#define __NCSI_NETLINK_H__

#include <linux/netdevice.h>

#include "internal.h"

<<<<<<< HEAD
=======
int ncsi_send_netlink_rsp(struct ncsi_request *nr,
			  struct ncsi_package *np,
			  struct ncsi_channel *nc);
int ncsi_send_netlink_timeout(struct ncsi_request *nr,
			      struct ncsi_package *np,
			      struct ncsi_channel *nc);
int ncsi_send_netlink_err(struct net_device *dev,
			  u32 snd_seq,
			  u32 snd_portid,
			  struct nlmsghdr *nlhdr,
			  int err);

>>>>>>> upstream/android-13
#endif /* __NCSI_NETLINK_H__ */
