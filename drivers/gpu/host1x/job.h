<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Tegra host1x Job
 *
 * Copyright (c) 2011-2013, NVIDIA Corporation.
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

#ifndef __HOST1X_JOB_H
#define __HOST1X_JOB_H

<<<<<<< HEAD
=======
#include <linux/dma-direction.h>

>>>>>>> upstream/android-13
struct host1x_job_gather {
	unsigned int words;
	dma_addr_t base;
	struct host1x_bo *bo;
	unsigned int offset;
	bool handled;
};

<<<<<<< HEAD
struct host1x_job_unpin_data {
	struct host1x_bo *bo;
	struct sg_table *sgt;
	size_t size;
=======
struct host1x_job_wait {
	u32 id;
	u32 threshold;
	u32 next_class;
	bool relative;
};

struct host1x_job_cmd {
	bool is_wait;

	union {
		struct host1x_job_gather gather;
		struct host1x_job_wait wait;
	};
};

struct host1x_job_unpin_data {
	struct host1x_bo *bo;
	struct sg_table *sgt;
	struct device *dev;
	size_t size;
	enum dma_data_direction dir;
>>>>>>> upstream/android-13
};

/*
 * Dump contents of job to debug output.
 */
void host1x_job_dump(struct device *dev, struct host1x_job *job);

#endif
