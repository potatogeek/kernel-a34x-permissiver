<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Intel SST generic IPC Support
 *
 * Copyright (C) 2015, Intel Corporation. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef __SST_GENERIC_IPC_H
#define __SST_GENERIC_IPC_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

<<<<<<< HEAD
#define IPC_MAX_MAILBOX_BYTES	256

struct ipc_message {
	struct list_head list;
	u64 header;

	/* direction wrt host CPU */
	char *tx_data;
	size_t tx_size;
	char *rx_data;
	size_t rx_size;
=======
struct sst_ipc_message {
	u64 header;
	void *data;
	size_t size;
};

struct ipc_message {
	struct list_head list;
	struct sst_ipc_message tx;
	struct sst_ipc_message rx;
>>>>>>> upstream/android-13

	wait_queue_head_t waitq;
	bool pending;
	bool complete;
	bool wait;
	int errno;
};

struct sst_generic_ipc;
<<<<<<< HEAD
=======
struct sst_dsp;
>>>>>>> upstream/android-13

struct sst_plat_ipc_ops {
	void (*tx_msg)(struct sst_generic_ipc *, struct ipc_message *);
	void (*shim_dbg)(struct sst_generic_ipc *, const char *);
	void (*tx_data_copy)(struct ipc_message *, char *, size_t);
	u64  (*reply_msg_match)(u64 header, u64 *mask);
	bool (*is_dsp_busy)(struct sst_dsp *dsp);
	int (*check_dsp_lp_on)(struct sst_dsp *dsp, bool state);
};

/* SST generic IPC data */
struct sst_generic_ipc {
	struct device *dev;
	struct sst_dsp *dsp;

	/* IPC messaging */
	struct list_head tx_list;
	struct list_head rx_list;
	struct list_head empty_list;
	wait_queue_head_t wait_txq;
	struct task_struct *tx_thread;
	struct work_struct kwork;
	bool pending;
	struct ipc_message *msg;
	int tx_data_max_size;
	int rx_data_max_size;

	struct sst_plat_ipc_ops ops;
};

<<<<<<< HEAD
int sst_ipc_tx_message_wait(struct sst_generic_ipc *ipc, u64 header,
	void *tx_data, size_t tx_bytes, void *rx_data, size_t rx_bytes);

int sst_ipc_tx_message_nowait(struct sst_generic_ipc *ipc, u64 header,
	void *tx_data, size_t tx_bytes);

int sst_ipc_tx_message_nopm(struct sst_generic_ipc *ipc, u64 header,
	void *tx_data, size_t tx_bytes, void *rx_data, size_t rx_bytes);
=======
int sst_ipc_tx_message_wait(struct sst_generic_ipc *ipc,
	struct sst_ipc_message request, struct sst_ipc_message *reply);

int sst_ipc_tx_message_nowait(struct sst_generic_ipc *ipc,
	struct sst_ipc_message request);

int sst_ipc_tx_message_nopm(struct sst_generic_ipc *ipc,
	struct sst_ipc_message request, struct sst_ipc_message *reply);
>>>>>>> upstream/android-13

struct ipc_message *sst_ipc_reply_find_msg(struct sst_generic_ipc *ipc,
	u64 header);

void sst_ipc_tx_msg_reply_complete(struct sst_generic_ipc *ipc,
	struct ipc_message *msg);

<<<<<<< HEAD
void sst_ipc_drop_all(struct sst_generic_ipc *ipc);
=======
>>>>>>> upstream/android-13
int sst_ipc_init(struct sst_generic_ipc *ipc);
void sst_ipc_fini(struct sst_generic_ipc *ipc);

#endif
