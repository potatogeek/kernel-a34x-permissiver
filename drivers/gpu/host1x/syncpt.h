<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Tegra host1x Syncpoints
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

#ifndef __HOST1X_SYNCPT_H
#define __HOST1X_SYNCPT_H

#include <linux/atomic.h>
#include <linux/host1x.h>
#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/kref.h>
>>>>>>> upstream/android-13
#include <linux/sched.h>

#include "intr.h"

struct host1x;

/* Reserved for replacing an expired wait with a NOP */
#define HOST1X_SYNCPT_RESERVED			0

struct host1x_syncpt_base {
	unsigned int id;
	bool requested;
};

struct host1x_syncpt {
<<<<<<< HEAD
=======
	struct kref ref;

>>>>>>> upstream/android-13
	unsigned int id;
	atomic_t min_val;
	atomic_t max_val;
	u32 base_val;
	const char *name;
	bool client_managed;
	struct host1x *host;
<<<<<<< HEAD
	struct host1x_client *client;
=======
>>>>>>> upstream/android-13
	struct host1x_syncpt_base *base;

	/* interrupt data */
	struct host1x_syncpt_intr intr;
<<<<<<< HEAD
=======

	/*
	 * If a submission incrementing this syncpoint fails, lock it so that
	 * further submission cannot be made until application has handled the
	 * failure.
	 */
	bool locked;
>>>>>>> upstream/android-13
};

/* Initialize sync point array  */
int host1x_syncpt_init(struct host1x *host);

/*  Free sync point array */
void host1x_syncpt_deinit(struct host1x *host);

/* Return number of sync point supported. */
unsigned int host1x_syncpt_nb_pts(struct host1x *host);

/* Return number of wait bases supported. */
unsigned int host1x_syncpt_nb_bases(struct host1x *host);

/* Return number of mlocks supported. */
unsigned int host1x_syncpt_nb_mlocks(struct host1x *host);

/*
 * Check sync point sanity. If max is larger than min, there have too many
 * sync point increments.
 *
 * Client managed sync point are not tracked.
 * */
static inline bool host1x_syncpt_check_max(struct host1x_syncpt *sp, u32 real)
{
	u32 max;
	if (sp->client_managed)
		return true;
	max = host1x_syncpt_read_max(sp);
	return (s32)(max - real) >= 0;
}

/* Return true if sync point is client managed. */
static inline bool host1x_syncpt_client_managed(struct host1x_syncpt *sp)
{
	return sp->client_managed;
}

/*
 * Returns true if syncpoint min == max, which means that there are no
 * outstanding operations.
 */
static inline bool host1x_syncpt_idle(struct host1x_syncpt *sp)
{
	int min, max;
	smp_rmb();
	min = atomic_read(&sp->min_val);
	max = atomic_read(&sp->max_val);
	return (min == max);
}

/* Load current value from hardware to the shadow register. */
u32 host1x_syncpt_load(struct host1x_syncpt *sp);

/* Check if the given syncpoint value has already passed */
bool host1x_syncpt_is_expired(struct host1x_syncpt *sp, u32 thresh);

/* Save host1x sync point state into shadow registers. */
void host1x_syncpt_save(struct host1x *host);

/* Reset host1x sync point state from shadow registers. */
void host1x_syncpt_restore(struct host1x *host);

/* Read current wait base value into shadow register and return it. */
u32 host1x_syncpt_load_wait_base(struct host1x_syncpt *sp);

/* Indicate future operations by incrementing the sync point max. */
u32 host1x_syncpt_incr_max(struct host1x_syncpt *sp, u32 incrs);

/* Check if sync point id is valid. */
static inline int host1x_syncpt_is_valid(struct host1x_syncpt *sp)
{
	return sp->id < host1x_syncpt_nb_pts(sp->host);
}

<<<<<<< HEAD
=======
static inline void host1x_syncpt_set_locked(struct host1x_syncpt *sp)
{
	sp->locked = true;
}

>>>>>>> upstream/android-13
#endif
