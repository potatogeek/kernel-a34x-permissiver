<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Tegra host1x Channel
 *
 * Copyright (c) 2010-2013, NVIDIA Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#ifndef __HOST1X_CHANNEL_H
#define __HOST1X_CHANNEL_H

#include <linux/io.h>
#include <linux/kref.h>

#include "cdma.h"

struct host1x;
struct host1x_channel;

struct host1x_channel_list {
	struct host1x_channel *channels;
	unsigned long *allocated_channels;
};

struct host1x_channel {
	struct kref refcount;
	unsigned int id;
	struct mutex submitlock;
	void __iomem *regs;
<<<<<<< HEAD
=======
	struct host1x_client *client;
>>>>>>> upstream/android-13
	struct device *dev;
	struct host1x_cdma cdma;
};

/* channel list operations */
int host1x_channel_list_init(struct host1x_channel_list *chlist,
			     unsigned int num_channels);
void host1x_channel_list_free(struct host1x_channel_list *chlist);
struct host1x_channel *host1x_channel_get_index(struct host1x *host,
						unsigned int index);

#endif
