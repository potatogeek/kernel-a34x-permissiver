<<<<<<< HEAD
/*
 * Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef __SOC_TEGRA_BPMP_H
#define __SOC_TEGRA_BPMP_H

#include <linux/mailbox_client.h>
#include <linux/pm_domain.h>
#include <linux/reset-controller.h>
#include <linux/semaphore.h>
#include <linux/types.h>

#include <soc/tegra/bpmp-abi.h>

struct tegra_bpmp_clk;
<<<<<<< HEAD
=======
struct tegra_bpmp_ops;
>>>>>>> upstream/android-13

struct tegra_bpmp_soc {
	struct {
		struct {
			unsigned int offset;
			unsigned int count;
			unsigned int timeout;
		} cpu_tx, thread, cpu_rx;
	} channels;
<<<<<<< HEAD
=======

	const struct tegra_bpmp_ops *ops;
>>>>>>> upstream/android-13
	unsigned int num_resets;
};

struct tegra_bpmp_mb_data {
	u32 code;
	u32 flags;
	u8 data[MSG_DATA_MIN_SZ];
} __packed;

struct tegra_bpmp_channel {
	struct tegra_bpmp *bpmp;
	struct tegra_bpmp_mb_data *ib;
	struct tegra_bpmp_mb_data *ob;
	struct completion completion;
	struct tegra_ivc *ivc;
<<<<<<< HEAD
=======
	unsigned int index;
>>>>>>> upstream/android-13
};

typedef void (*tegra_bpmp_mrq_handler_t)(unsigned int mrq,
					 struct tegra_bpmp_channel *channel,
					 void *data);

struct tegra_bpmp_mrq {
	struct list_head list;
	unsigned int mrq;
	tegra_bpmp_mrq_handler_t handler;
	void *data;
};

struct tegra_bpmp {
	const struct tegra_bpmp_soc *soc;
	struct device *dev;
<<<<<<< HEAD

	struct {
		struct gen_pool *pool;
		dma_addr_t phys;
		void *virt;
	} tx, rx;
=======
	void *priv;
>>>>>>> upstream/android-13

	struct {
		struct mbox_client client;
		struct mbox_chan *channel;
	} mbox;

	spinlock_t atomic_tx_lock;
	struct tegra_bpmp_channel *tx_channel, *rx_channel, *threaded_channels;

	struct {
		unsigned long *allocated;
		unsigned long *busy;
		unsigned int count;
		struct semaphore lock;
	} threaded;

	struct list_head mrqs;
	spinlock_t lock;

	struct tegra_bpmp_clk **clocks;
	unsigned int num_clocks;

	struct reset_controller_dev rstc;

	struct genpd_onecell_data genpd;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_mirror;
#endif
};

struct tegra_bpmp_message {
	unsigned int mrq;

	struct {
		const void *data;
		size_t size;
	} tx;

	struct {
		void *data;
		size_t size;
		int ret;
	} rx;
};

#if IS_ENABLED(CONFIG_TEGRA_BPMP)
struct tegra_bpmp *tegra_bpmp_get(struct device *dev);
void tegra_bpmp_put(struct tegra_bpmp *bpmp);
int tegra_bpmp_transfer_atomic(struct tegra_bpmp *bpmp,
			       struct tegra_bpmp_message *msg);
int tegra_bpmp_transfer(struct tegra_bpmp *bpmp,
			struct tegra_bpmp_message *msg);
void tegra_bpmp_mrq_return(struct tegra_bpmp_channel *channel, int code,
			   const void *data, size_t size);

int tegra_bpmp_request_mrq(struct tegra_bpmp *bpmp, unsigned int mrq,
			   tegra_bpmp_mrq_handler_t handler, void *data);
void tegra_bpmp_free_mrq(struct tegra_bpmp *bpmp, unsigned int mrq,
			 void *data);
<<<<<<< HEAD
=======
bool tegra_bpmp_mrq_is_supported(struct tegra_bpmp *bpmp, unsigned int mrq);
>>>>>>> upstream/android-13
#else
static inline struct tegra_bpmp *tegra_bpmp_get(struct device *dev)
{
	return ERR_PTR(-ENOTSUPP);
}
static inline void tegra_bpmp_put(struct tegra_bpmp *bpmp)
{
}
static inline int tegra_bpmp_transfer_atomic(struct tegra_bpmp *bpmp,
					     struct tegra_bpmp_message *msg)
{
	return -ENOTSUPP;
}
static inline int tegra_bpmp_transfer(struct tegra_bpmp *bpmp,
				      struct tegra_bpmp_message *msg)
{
	return -ENOTSUPP;
}
static inline void tegra_bpmp_mrq_return(struct tegra_bpmp_channel *channel,
					 int code, const void *data,
					 size_t size)
{
}

static inline int tegra_bpmp_request_mrq(struct tegra_bpmp *bpmp,
					 unsigned int mrq,
					 tegra_bpmp_mrq_handler_t handler,
					 void *data)
{
	return -ENOTSUPP;
}
static inline void tegra_bpmp_free_mrq(struct tegra_bpmp *bpmp,
				       unsigned int mrq, void *data)
{
}
<<<<<<< HEAD
#endif

=======

static inline bool tegra_bpmp_mrq_is_supported(struct tegra_bpmp *bpmp,
					      unsigned int mrq)
{
	return false;
}
#endif

void tegra_bpmp_handle_rx(struct tegra_bpmp *bpmp);

>>>>>>> upstream/android-13
#if IS_ENABLED(CONFIG_CLK_TEGRA_BPMP)
int tegra_bpmp_init_clocks(struct tegra_bpmp *bpmp);
#else
static inline int tegra_bpmp_init_clocks(struct tegra_bpmp *bpmp)
{
	return 0;
}
#endif

#if IS_ENABLED(CONFIG_RESET_TEGRA_BPMP)
int tegra_bpmp_init_resets(struct tegra_bpmp *bpmp);
#else
static inline int tegra_bpmp_init_resets(struct tegra_bpmp *bpmp)
{
	return 0;
}
#endif

#if IS_ENABLED(CONFIG_SOC_TEGRA_POWERGATE_BPMP)
int tegra_bpmp_init_powergates(struct tegra_bpmp *bpmp);
#else
static inline int tegra_bpmp_init_powergates(struct tegra_bpmp *bpmp)
{
	return 0;
}
#endif

#if IS_ENABLED(CONFIG_DEBUG_FS)
int tegra_bpmp_init_debugfs(struct tegra_bpmp *bpmp);
#else
static inline int tegra_bpmp_init_debugfs(struct tegra_bpmp *bpmp)
{
	return 0;
}
#endif


#endif /* __SOC_TEGRA_BPMP_H */
