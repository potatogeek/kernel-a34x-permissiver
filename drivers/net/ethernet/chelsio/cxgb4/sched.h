/*
 * This file is part of the Chelsio T4 Ethernet driver for Linux.
 *
 * Copyright (c) 2016 Chelsio Communications, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __CXGB4_SCHED_H
#define __CXGB4_SCHED_H

#include <linux/spinlock.h>
#include <linux/atomic.h>

#define SCHED_CLS_NONE 0xff

#define FW_SCHED_CLS_NONE 0xffffffff

/* Max rate that can be set to a scheduling class is 100 Gbps */
#define SCHED_MAX_RATE_KBPS 100000000U

enum {
	SCHED_STATE_ACTIVE,
	SCHED_STATE_UNUSED,
};

enum sched_fw_ops {
	SCHED_FW_OP_ADD,
<<<<<<< HEAD
=======
	SCHED_FW_OP_DEL,
>>>>>>> upstream/android-13
};

enum sched_bind_type {
	SCHED_QUEUE,
<<<<<<< HEAD
=======
	SCHED_FLOWC,
>>>>>>> upstream/android-13
};

struct sched_queue_entry {
	struct list_head list;
	unsigned int cntxt_id;
	struct ch_sched_queue param;
};

<<<<<<< HEAD
=======
struct sched_flowc_entry {
	struct list_head list;
	struct ch_sched_flowc param;
};

>>>>>>> upstream/android-13
struct sched_class {
	u8 state;
	u8 idx;
	struct ch_sched_params info;
<<<<<<< HEAD
	struct list_head queue_list;
	spinlock_t lock; /* Per class lock */
=======
	enum sched_bind_type bind_type;
	struct list_head entry_list;
>>>>>>> upstream/android-13
	atomic_t refcnt;
};

struct sched_table {      /* per port scheduling table */
	u8 sched_size;
<<<<<<< HEAD
	rwlock_t rw_lock; /* Table lock */
	struct sched_class tab[0];
=======
	struct sched_class tab[];
>>>>>>> upstream/android-13
};

static inline bool can_sched(struct net_device *dev)
{
	struct port_info *pi = netdev2pinfo(dev);

	return !pi->sched_tbl ? false : true;
}

static inline bool valid_class_id(struct net_device *dev, u8 class_id)
{
	struct port_info *pi = netdev2pinfo(dev);

	if ((class_id > pi->sched_tbl->sched_size - 1) &&
	    (class_id != SCHED_CLS_NONE))
		return false;

	return true;
}

<<<<<<< HEAD
=======
struct sched_class *cxgb4_sched_queue_lookup(struct net_device *dev,
					     struct ch_sched_queue *p);
>>>>>>> upstream/android-13
int cxgb4_sched_class_bind(struct net_device *dev, void *arg,
			   enum sched_bind_type type);
int cxgb4_sched_class_unbind(struct net_device *dev, void *arg,
			     enum sched_bind_type type);

struct sched_class *cxgb4_sched_class_alloc(struct net_device *dev,
					    struct ch_sched_params *p);
<<<<<<< HEAD
=======
void cxgb4_sched_class_free(struct net_device *dev, u8 classid);
>>>>>>> upstream/android-13

struct sched_table *t4_init_sched(unsigned int size);
void t4_cleanup_sched(struct adapter *adap);
#endif  /* __CXGB4_SCHED_H */
