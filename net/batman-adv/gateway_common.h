/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2009-2018  B.A.T.M.A.N. contributors:
 *
 * Marek Lindner
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
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Marek Lindner
>>>>>>> upstream/android-13
 */

#ifndef _NET_BATMAN_ADV_GATEWAY_COMMON_H_
#define _NET_BATMAN_ADV_GATEWAY_COMMON_H_

#include "main.h"

<<<<<<< HEAD
#include <linux/types.h>

struct net_device;

enum batadv_gw_modes {
	BATADV_GW_MODE_OFF,
	BATADV_GW_MODE_CLIENT,
	BATADV_GW_MODE_SERVER,
};

=======
#include <linux/netdevice.h>
#include <linux/types.h>

>>>>>>> upstream/android-13
/**
 * enum batadv_bandwidth_units - bandwidth unit types
 */
enum batadv_bandwidth_units {
	/** @BATADV_BW_UNIT_KBIT: unit type kbit */
	BATADV_BW_UNIT_KBIT,

	/** @BATADV_BW_UNIT_MBIT: unit type mbit */
	BATADV_BW_UNIT_MBIT,
};

#define BATADV_GW_MODE_OFF_NAME	"off"
#define BATADV_GW_MODE_CLIENT_NAME	"client"
#define BATADV_GW_MODE_SERVER_NAME	"server"

ssize_t batadv_gw_bandwidth_set(struct net_device *net_dev, char *buff,
				size_t count);
void batadv_gw_tvlv_container_update(struct batadv_priv *bat_priv);
void batadv_gw_init(struct batadv_priv *bat_priv);
void batadv_gw_free(struct batadv_priv *bat_priv);
bool batadv_parse_throughput(struct net_device *net_dev, char *buff,
			     const char *description, u32 *throughput);

#endif /* _NET_BATMAN_ADV_GATEWAY_COMMON_H_ */
