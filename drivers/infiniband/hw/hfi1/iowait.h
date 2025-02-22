<<<<<<< HEAD
#ifndef _HFI1_IOWAIT_H
#define _HFI1_IOWAIT_H
/*
 * Copyright(c) 2015, 2016 Intel Corporation.
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <linux/list.h>
#include <linux/workqueue.h>
=======
/* SPDX-License-Identifier: GPL-2.0 or BSD-3-Clause */
/*
 * Copyright(c) 2015 - 2018 Intel Corporation.
 */

#ifndef _HFI1_IOWAIT_H
#define _HFI1_IOWAIT_H

#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
>>>>>>> upstream/android-13
#include <linux/sched.h>

#include "sdma_txreq.h"

/*
 * typedef (*restart_t)() - restart callback
 * @work: pointer to work structure
 */
typedef void (*restart_t)(struct work_struct *work);

<<<<<<< HEAD
struct sdma_txreq;
struct sdma_engine;
/**
 * struct iowait - linkage for delayed progress/waiting
 * @list: used to add/insert into QP/PQ wait lists
 * @lock: uses to record the list head lock
=======
#define IOWAIT_PENDING_IB  0x0
#define IOWAIT_PENDING_TID 0x1

/*
 * A QP can have multiple Send Engines (SEs).
 *
 * The current use case is for supporting a TID RDMA
 * packet build/xmit mechanism independent from verbs.
 */
#define IOWAIT_SES 2
#define IOWAIT_IB_SE 0
#define IOWAIT_TID_SE 1

struct sdma_txreq;
struct sdma_engine;
/**
 * @iowork: the work struct
 * @tx_head: list of prebuilt packets
 * @iow: the parent iowait structure
 *
 * This structure is the work item (process) specific
 * details associated with the each of the two SEs of the
 * QP.
 *
 * The workstruct and the queued TXs are unique to each
 * SE.
 */
struct iowait;
struct iowait_work {
	struct work_struct iowork;
	struct list_head tx_head;
	struct iowait *iow;
};

/**
 * @list: used to add/insert into QP/PQ wait lists
>>>>>>> upstream/android-13
 * @tx_head: overflow list of sdma_txreq's
 * @sleep: no space callback
 * @wakeup: space callback wakeup
 * @sdma_drained: sdma count drained
<<<<<<< HEAD
=======
 * @init_priority: callback to manipulate priority
 * @lock: lock protected head of wait queue
>>>>>>> upstream/android-13
 * @iowork: workqueue overhead
 * @wait_dma: wait for sdma_busy == 0
 * @wait_pio: wait for pio_busy == 0
 * @sdma_busy: # of packets in flight
 * @count: total number of descriptors in tx_head'ed list
 * @tx_limit: limit for overflow queuing
 * @tx_count: number of tx entry's in tx_head'ed list
<<<<<<< HEAD
=======
 * @flags: wait flags (one per QP)
 * @wait: SE array for multiple legs
>>>>>>> upstream/android-13
 *
 * This is to be embedded in user's state structure
 * (QP or PQ).
 *
 * The sleep and wakeup members are a
 * bit misnamed.   They do not strictly
 * speaking sleep or wake up, but they
 * are callbacks for the ULP to implement
 * what ever queuing/dequeuing of
 * the embedded iowait and its containing struct
<<<<<<< HEAD
 * when a resource shortage like SDMA ring space is seen.
 *
 * Both potentially have locks help
 * so sleeping is not allowed.
=======
 * when a resource shortage like SDMA ring space
 * or PIO credit space is seen.
 *
 * Both potentially have locks help
 * so sleeping is not allowed and it is not
 * supported to submit txreqs from the wakeup
 * call directly because of lock conflicts.
>>>>>>> upstream/android-13
 *
 * The wait_dma member along with the iow
 *
 * The lock field is used by waiters to record
 * the seqlock_t that guards the list head.
 * Waiters explicity know that, but the destroy
 * code that unwaits QPs does not.
 */
<<<<<<< HEAD

struct iowait {
	struct list_head list;
	struct list_head tx_head;
	int (*sleep)(
		struct sdma_engine *sde,
		struct iowait *wait,
=======
struct iowait {
	struct list_head list;
	int (*sleep)(
		struct sdma_engine *sde,
		struct iowait_work *wait,
>>>>>>> upstream/android-13
		struct sdma_txreq *tx,
		uint seq,
		bool pkts_sent
		);
	void (*wakeup)(struct iowait *wait, int reason);
	void (*sdma_drained)(struct iowait *wait);
<<<<<<< HEAD
	seqlock_t *lock;
	struct work_struct iowork;
=======
	void (*init_priority)(struct iowait *wait);
	seqlock_t *lock;
>>>>>>> upstream/android-13
	wait_queue_head_t wait_dma;
	wait_queue_head_t wait_pio;
	atomic_t sdma_busy;
	atomic_t pio_busy;
	u32 count;
	u32 tx_limit;
	u32 tx_count;
	u8 starved_cnt;
<<<<<<< HEAD
=======
	u8 priority;
	unsigned long flags;
	struct iowait_work wait[IOWAIT_SES];
>>>>>>> upstream/android-13
};

#define SDMA_AVAIL_REASON 0

<<<<<<< HEAD
/**
 * iowait_init() - initialize wait structure
 * @wait: wait struct to initialize
 * @tx_limit: limit for overflow queuing
 * @func: restart function for workqueue
 * @sleep: sleep function for no space
 * @resume: wakeup function for no space
 *
 * This function initializes the iowait
 * structure embedded in the QP or PQ.
 *
 */

static inline void iowait_init(
	struct iowait *wait,
	u32 tx_limit,
	void (*func)(struct work_struct *work),
	int (*sleep)(
		struct sdma_engine *sde,
		struct iowait *wait,
		struct sdma_txreq *tx,
		uint seq,
		bool pkts_sent),
	void (*wakeup)(struct iowait *wait, int reason),
	void (*sdma_drained)(struct iowait *wait))
{
	wait->count = 0;
	wait->lock = NULL;
	INIT_LIST_HEAD(&wait->list);
	INIT_LIST_HEAD(&wait->tx_head);
	INIT_WORK(&wait->iowork, func);
	init_waitqueue_head(&wait->wait_dma);
	init_waitqueue_head(&wait->wait_pio);
	atomic_set(&wait->sdma_busy, 0);
	atomic_set(&wait->pio_busy, 0);
	wait->tx_limit = tx_limit;
	wait->sleep = sleep;
	wait->wakeup = wakeup;
	wait->sdma_drained = sdma_drained;
}

/**
 * iowait_schedule() - initialize wait structure
=======
void iowait_set_flag(struct iowait *wait, u32 flag);
bool iowait_flag_set(struct iowait *wait, u32 flag);
void iowait_clear_flag(struct iowait *wait, u32 flag);

void iowait_init(struct iowait *wait, u32 tx_limit,
		 void (*func)(struct work_struct *work),
		 void (*tidfunc)(struct work_struct *work),
		 int (*sleep)(struct sdma_engine *sde,
			      struct iowait_work *wait,
			      struct sdma_txreq *tx,
			      uint seq,
			      bool pkts_sent),
		 void (*wakeup)(struct iowait *wait, int reason),
		 void (*sdma_drained)(struct iowait *wait),
		 void (*init_priority)(struct iowait *wait));

/**
 * iowait_schedule() - schedule the default send engine work
>>>>>>> upstream/android-13
 * @wait: wait struct to schedule
 * @wq: workqueue for schedule
 * @cpu: cpu
 */
<<<<<<< HEAD
static inline void iowait_schedule(
	struct iowait *wait,
	struct workqueue_struct *wq,
	int cpu)
{
	queue_work_on(cpu, wq, &wait->iowork);
=======
static inline bool iowait_schedule(struct iowait *wait,
				   struct workqueue_struct *wq, int cpu)
{
	return !!queue_work_on(cpu, wq, &wait->wait[IOWAIT_IB_SE].iowork);
}

/**
 * iowait_tid_schedule - schedule the tid SE
 * @wait: the iowait structure
 * @wq: the work queue
 * @cpu: the cpu
 */
static inline bool iowait_tid_schedule(struct iowait *wait,
				       struct workqueue_struct *wq, int cpu)
{
	return !!queue_work_on(cpu, wq, &wait->wait[IOWAIT_TID_SE].iowork);
>>>>>>> upstream/android-13
}

/**
 * iowait_sdma_drain() - wait for DMAs to drain
 *
 * @wait: iowait structure
 *
 * This will delay until the iowait sdmas have
 * completed.
 */
static inline void iowait_sdma_drain(struct iowait *wait)
{
	wait_event(wait->wait_dma, !atomic_read(&wait->sdma_busy));
}

/**
 * iowait_sdma_pending() - return sdma pending count
 *
 * @wait: iowait structure
 *
 */
static inline int iowait_sdma_pending(struct iowait *wait)
{
	return atomic_read(&wait->sdma_busy);
}

/**
 * iowait_sdma_inc - note sdma io pending
 * @wait: iowait structure
 */
static inline void iowait_sdma_inc(struct iowait *wait)
{
	atomic_inc(&wait->sdma_busy);
}

/**
 * iowait_sdma_add - add count to pending
 * @wait: iowait structure
 */
static inline void iowait_sdma_add(struct iowait *wait, int count)
{
	atomic_add(count, &wait->sdma_busy);
}

/**
 * iowait_sdma_dec - note sdma complete
 * @wait: iowait structure
 */
static inline int iowait_sdma_dec(struct iowait *wait)
{
<<<<<<< HEAD
=======
	if (!wait)
		return 0;
>>>>>>> upstream/android-13
	return atomic_dec_and_test(&wait->sdma_busy);
}

/**
 * iowait_pio_drain() - wait for pios to drain
 *
 * @wait: iowait structure
 *
 * This will delay until the iowait pios have
 * completed.
 */
static inline void iowait_pio_drain(struct iowait *wait)
{
	wait_event_timeout(wait->wait_pio,
			   !atomic_read(&wait->pio_busy),
			   HZ);
}

/**
 * iowait_pio_pending() - return pio pending count
 *
 * @wait: iowait structure
 *
 */
static inline int iowait_pio_pending(struct iowait *wait)
{
	return atomic_read(&wait->pio_busy);
}

/**
 * iowait_pio_inc - note pio pending
 * @wait: iowait structure
 */
static inline void iowait_pio_inc(struct iowait *wait)
{
	atomic_inc(&wait->pio_busy);
}

/**
<<<<<<< HEAD
 * iowait_sdma_dec - note pio complete
=======
 * iowait_pio_dec - note pio complete
>>>>>>> upstream/android-13
 * @wait: iowait structure
 */
static inline int iowait_pio_dec(struct iowait *wait)
{
<<<<<<< HEAD
=======
	if (!wait)
		return 0;
>>>>>>> upstream/android-13
	return atomic_dec_and_test(&wait->pio_busy);
}

/**
 * iowait_drain_wakeup() - trigger iowait_drain() waiter
 *
 * @wait: iowait structure
 *
 * This will trigger any waiters.
 */
static inline void iowait_drain_wakeup(struct iowait *wait)
{
	wake_up(&wait->wait_dma);
	wake_up(&wait->wait_pio);
	if (wait->sdma_drained)
		wait->sdma_drained(wait);
}

/**
 * iowait_get_txhead() - get packet off of iowait list
 *
<<<<<<< HEAD
 * @wait wait struture
 */
static inline struct sdma_txreq *iowait_get_txhead(struct iowait *wait)
=======
 * @wait: iowait_work structure
 */
static inline struct sdma_txreq *iowait_get_txhead(struct iowait_work *wait)
>>>>>>> upstream/android-13
{
	struct sdma_txreq *tx = NULL;

	if (!list_empty(&wait->tx_head)) {
		tx = list_first_entry(
			&wait->tx_head,
			struct sdma_txreq,
			list);
		list_del_init(&tx->list);
	}
	return tx;
}

<<<<<<< HEAD
=======
static inline u16 iowait_get_desc(struct iowait_work *w)
{
	u16 num_desc = 0;
	struct sdma_txreq *tx = NULL;

	if (!list_empty(&w->tx_head)) {
		tx = list_first_entry(&w->tx_head, struct sdma_txreq,
				      list);
		num_desc = tx->num_desc;
		if (tx->flags & SDMA_TXREQ_F_VIP)
			w->iow->priority++;
	}
	return num_desc;
}

static inline u32 iowait_get_all_desc(struct iowait *w)
{
	u32 num_desc = 0;

	num_desc = iowait_get_desc(&w->wait[IOWAIT_IB_SE]);
	num_desc += iowait_get_desc(&w->wait[IOWAIT_TID_SE]);
	return num_desc;
}

static inline void iowait_update_priority(struct iowait_work *w)
{
	struct sdma_txreq *tx = NULL;

	if (!list_empty(&w->tx_head)) {
		tx = list_first_entry(&w->tx_head, struct sdma_txreq,
				      list);
		if (tx->flags & SDMA_TXREQ_F_VIP)
			w->iow->priority++;
	}
}

static inline void iowait_update_all_priority(struct iowait *w)
{
	iowait_update_priority(&w->wait[IOWAIT_IB_SE]);
	iowait_update_priority(&w->wait[IOWAIT_TID_SE]);
}

static inline void iowait_init_priority(struct iowait *w)
{
	w->priority = 0;
	if (w->init_priority)
		w->init_priority(w);
}

static inline void iowait_get_priority(struct iowait *w)
{
	iowait_init_priority(w);
	iowait_update_all_priority(w);
}

>>>>>>> upstream/android-13
/**
 * iowait_queue - Put the iowait on a wait queue
 * @pkts_sent: have some packets been sent before queuing?
 * @w: the iowait struct
 * @wait_head: the wait queue
 *
 * This function is called to insert an iowait struct into a
<<<<<<< HEAD
 * wait queue after a resource (eg, sdma decriptor or pio
=======
 * wait queue after a resource (eg, sdma descriptor or pio
>>>>>>> upstream/android-13
 * buffer) is run out.
 */
static inline void iowait_queue(bool pkts_sent, struct iowait *w,
				struct list_head *wait_head)
{
	/*
	 * To play fair, insert the iowait at the tail of the wait queue if it
	 * has already sent some packets; Otherwise, put it at the head.
<<<<<<< HEAD
	 */
	if (pkts_sent) {
		list_add_tail(&w->list, wait_head);
		w->starved_cnt = 0;
	} else {
		list_add(&w->list, wait_head);
		w->starved_cnt++;
	}
=======
	 * However, if it has priority packets to send, also put it at the
	 * head.
	 */
	if (pkts_sent)
		w->starved_cnt = 0;
	else
		w->starved_cnt++;

	if (w->priority > 0 || !pkts_sent)
		list_add(&w->list, wait_head);
	else
		list_add_tail(&w->list, wait_head);
>>>>>>> upstream/android-13
}

/**
 * iowait_starve_clear - clear the wait queue's starve count
 * @pkts_sent: have some packets been sent?
 * @w: the iowait struct
 *
 * This function is called to clear the starve count. If no
 * packets have been sent, the starve count will not be cleared.
 */
static inline void iowait_starve_clear(bool pkts_sent, struct iowait *w)
{
	if (pkts_sent)
		w->starved_cnt = 0;
}

<<<<<<< HEAD
/**
 * iowait_starve_find_max - Find the maximum of the starve count
 * @w: the iowait struct
 * @max: a variable containing the max starve count
 * @idx: the index of the current iowait in an array
 * @max_idx: a variable containing the array index for the
 *         iowait entry that has the max starve count
 *
 * This function is called to compare the starve count of a
 * given iowait with the given max starve count. The max starve
 * count and the index will be updated if the iowait's start
 * count is larger.
 */
static inline void iowait_starve_find_max(struct iowait *w, u8 *max,
					  uint idx, uint *max_idx)
{
	if (w->starved_cnt > *max) {
		*max = w->starved_cnt;
		*max_idx = idx;
	}
}

/**
 * iowait_packet_queued() - determine if a packet is already built
 * @wait: the wait structure
 */
static inline bool iowait_packet_queued(struct iowait *wait)
=======
/* Update the top priority index */
uint iowait_priority_update_top(struct iowait *w,
				struct iowait *top,
				uint idx, uint top_idx);

/**
 * iowait_packet_queued() - determine if a packet is queued
 * @wait: the iowait_work structure
 */
static inline bool iowait_packet_queued(struct iowait_work *wait)
>>>>>>> upstream/android-13
{
	return !list_empty(&wait->tx_head);
}

<<<<<<< HEAD
=======
/**
 * inc_wait_count - increment wait counts
 * @w: the log work struct
 * @n: the count
 */
static inline void iowait_inc_wait_count(struct iowait_work *w, u16 n)
{
	if (!w)
		return;
	w->iow->tx_count++;
	w->iow->count += n;
}

/**
 * iowait_get_tid_work - return iowait_work for tid SE
 * @w: the iowait struct
 */
static inline struct iowait_work *iowait_get_tid_work(struct iowait *w)
{
	return &w->wait[IOWAIT_TID_SE];
}

/**
 * iowait_get_ib_work - return iowait_work for ib SE
 * @w: the iowait struct
 */
static inline struct iowait_work *iowait_get_ib_work(struct iowait *w)
{
	return &w->wait[IOWAIT_IB_SE];
}

/**
 * iowait_ioww_to_iow - return iowait given iowait_work
 * @w: the iowait_work struct
 */
static inline struct iowait *iowait_ioww_to_iow(struct iowait_work *w)
{
	if (likely(w))
		return w->iow;
	return NULL;
}

void iowait_cancel_work(struct iowait *w);
int iowait_set_work_flag(struct iowait_work *w);

>>>>>>> upstream/android-13
#endif
