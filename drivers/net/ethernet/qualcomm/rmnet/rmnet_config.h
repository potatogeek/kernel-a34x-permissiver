<<<<<<< HEAD
/* Copyright (c) 2013-2014, 2016-2018 The Linux Foundation. All rights reserved.
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
 * RMNET Data configuration engine
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2013-2014, 2016-2018, 2021 The Linux Foundation.
 * All rights reserved.
 *
 * RMNET Data configuration engine
>>>>>>> upstream/android-13
 */

#include <linux/skbuff.h>
#include <net/gro_cells.h>

#ifndef _RMNET_CONFIG_H_
#define _RMNET_CONFIG_H_

#define RMNET_MAX_LOGICAL_EP 255

struct rmnet_endpoint {
	u8 mux_id;
	struct net_device *egress_dev;
	struct hlist_node hlnode;
};

/* One instance of this structure is instantiated for each real_dev associated
 * with rmnet.
 */
struct rmnet_port {
	struct net_device *dev;
	u32 data_format;
	u8 nr_rmnet_devs;
	u8 rmnet_mode;
	struct hlist_head muxed_ep[RMNET_MAX_LOGICAL_EP];
	struct net_device *bridge_ep;
	struct net_device *rmnet_dev;
};

extern struct rtnl_link_ops rmnet_link_ops;

struct rmnet_vnd_stats {
	u64 rx_pkts;
	u64 rx_bytes;
	u64 tx_pkts;
	u64 tx_bytes;
	u32 tx_drops;
};

struct rmnet_pcpu_stats {
	struct rmnet_vnd_stats stats;
	struct u64_stats_sync syncp;
};

struct rmnet_priv_stats {
	u64 csum_ok;
<<<<<<< HEAD
=======
	u64 csum_ip4_header_bad;
>>>>>>> upstream/android-13
	u64 csum_valid_unset;
	u64 csum_validation_failed;
	u64 csum_err_bad_buffer;
	u64 csum_err_invalid_ip_version;
	u64 csum_err_invalid_transport;
	u64 csum_fragmented_pkt;
	u64 csum_skipped;
	u64 csum_sw;
<<<<<<< HEAD
=======
	u64 csum_hw;
>>>>>>> upstream/android-13
};

struct rmnet_priv {
	u8 mux_id;
	struct net_device *real_dev;
	struct rmnet_pcpu_stats __percpu *pcpu_stats;
	struct gro_cells gro_cells;
	struct rmnet_priv_stats stats;
};

struct rmnet_port *rmnet_get_port_rcu(struct net_device *real_dev);
struct rmnet_endpoint *rmnet_get_endpoint(struct rmnet_port *port, u8 mux_id);
int rmnet_add_bridge(struct net_device *rmnet_dev,
		     struct net_device *slave_dev,
		     struct netlink_ext_ack *extack);
int rmnet_del_bridge(struct net_device *rmnet_dev,
		     struct net_device *slave_dev);
<<<<<<< HEAD
=======
struct rmnet_port*
rmnet_get_port_rtnl(const struct net_device *real_dev);
>>>>>>> upstream/android-13
#endif /* _RMNET_CONFIG_H_ */
