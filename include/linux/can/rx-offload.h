<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/can/rx-offload.h
 *
 * Copyright (c) 2014 David Jander, Protonic Holland
 * Copyright (c) 2014-2017 Pengutronix, Marc Kleine-Budde <kernel@pengutronix.de>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef _CAN_RX_OFFLOAD_H
#define _CAN_RX_OFFLOAD_H

#include <linux/netdevice.h>
#include <linux/can.h>

struct can_rx_offload {
	struct net_device *dev;

<<<<<<< HEAD
	unsigned int (*mailbox_read)(struct can_rx_offload *offload, struct can_frame *cf,
				     u32 *timestamp, unsigned int mb);

	struct sk_buff_head skb_queue;
=======
	struct sk_buff *(*mailbox_read)(struct can_rx_offload *offload,
					unsigned int mb, u32 *timestamp,
					bool drop);

	struct sk_buff_head skb_queue;
	struct sk_buff_head skb_irq_queue;
>>>>>>> upstream/android-13
	u32 skb_queue_len_max;

	unsigned int mb_first;
	unsigned int mb_last;

	struct napi_struct napi;

	bool inc;
};

<<<<<<< HEAD
int can_rx_offload_add_timestamp(struct net_device *dev, struct can_rx_offload *offload);
int can_rx_offload_add_fifo(struct net_device *dev, struct can_rx_offload *offload, unsigned int weight);
int can_rx_offload_irq_offload_timestamp(struct can_rx_offload *offload, u64 reg);
=======
int can_rx_offload_add_timestamp(struct net_device *dev,
				 struct can_rx_offload *offload);
int can_rx_offload_add_fifo(struct net_device *dev,
			    struct can_rx_offload *offload,
			    unsigned int weight);
int can_rx_offload_add_manual(struct net_device *dev,
			      struct can_rx_offload *offload,
			      unsigned int weight);
int can_rx_offload_irq_offload_timestamp(struct can_rx_offload *offload,
					 u64 reg);
>>>>>>> upstream/android-13
int can_rx_offload_irq_offload_fifo(struct can_rx_offload *offload);
int can_rx_offload_queue_sorted(struct can_rx_offload *offload,
				struct sk_buff *skb, u32 timestamp);
unsigned int can_rx_offload_get_echo_skb(struct can_rx_offload *offload,
<<<<<<< HEAD
					 unsigned int idx, u32 timestamp);
int can_rx_offload_queue_tail(struct can_rx_offload *offload,
			      struct sk_buff *skb);
void can_rx_offload_reset(struct can_rx_offload *offload);
void can_rx_offload_del(struct can_rx_offload *offload);
void can_rx_offload_enable(struct can_rx_offload *offload);

static inline void can_rx_offload_schedule(struct can_rx_offload *offload)
{
	napi_schedule(&offload->napi);
}

=======
					 unsigned int idx, u32 timestamp,
					 unsigned int *frame_len_ptr);
int can_rx_offload_queue_tail(struct can_rx_offload *offload,
			      struct sk_buff *skb);
void can_rx_offload_irq_finish(struct can_rx_offload *offload);
void can_rx_offload_threaded_irq_finish(struct can_rx_offload *offload);
void can_rx_offload_del(struct can_rx_offload *offload);
void can_rx_offload_enable(struct can_rx_offload *offload);

>>>>>>> upstream/android-13
static inline void can_rx_offload_disable(struct can_rx_offload *offload)
{
	napi_disable(&offload->napi);
}

#endif /* !_CAN_RX_OFFLOAD_H */
