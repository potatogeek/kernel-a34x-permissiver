<<<<<<< HEAD
/*
 * Huawei HiNIC PCI Express Linux driver
 * Copyright(c) 2017 Huawei Technologies Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Huawei HiNIC PCI Express Linux driver
 * Copyright(c) 2017 Huawei Technologies Co., Ltd
>>>>>>> upstream/android-13
 */

#ifndef HINIC_RX_H
#define HINIC_RX_H

#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/u64_stats_sync.h>
#include <linux/interrupt.h>

#include "hinic_hw_qp.h"

<<<<<<< HEAD
struct hinic_rxq_stats {
	u64                     pkts;
	u64                     bytes;

=======
#define HINIC_RX_CSUM_OFFLOAD_EN	0xFFF
#define HINIC_RX_CSUM_HW_CHECK_NONE	BIT(7)
#define HINIC_RX_CSUM_IPSU_OTHER_ERR	BIT(8)

struct hinic_rxq_stats {
	u64                     pkts;
	u64                     bytes;
	u64			errors;
	u64			csum_errors;
	u64			other_errors;
	u64			alloc_skb_err;
>>>>>>> upstream/android-13
	struct u64_stats_sync   syncp;
};

struct hinic_rxq {
	struct net_device       *netdev;
	struct hinic_rq         *rq;

	struct hinic_rxq_stats  rxq_stats;

	char                    *irq_name;
<<<<<<< HEAD

	struct tasklet_struct   rx_task;
=======
	u16			buf_len;
	u32			rx_buff_shift;
>>>>>>> upstream/android-13

	struct napi_struct      napi;
};

void hinic_rxq_clean_stats(struct hinic_rxq *rxq);

void hinic_rxq_get_stats(struct hinic_rxq *rxq, struct hinic_rxq_stats *stats);

int hinic_init_rxq(struct hinic_rxq *rxq, struct hinic_rq *rq,
		   struct net_device *netdev);

void hinic_clean_rxq(struct hinic_rxq *rxq);

#endif
