<<<<<<< HEAD
/*
 * RCU-based infrastructure for lightweight reader-writer locking
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * RCU-based infrastructure for lightweight reader-writer locking
 *
>>>>>>> upstream/android-13
 * Copyright (c) 2015, Red Hat, Inc.
 *
 * Author: Oleg Nesterov <oleg@redhat.com>
 */

#ifndef _LINUX_RCU_SYNC_H_
#define _LINUX_RCU_SYNC_H_

#include <linux/wait.h>
#include <linux/rcupdate.h>

<<<<<<< HEAD
enum rcu_sync_type { RCU_SYNC, RCU_SCHED_SYNC, RCU_BH_SYNC };

=======
>>>>>>> upstream/android-13
/* Structure to mediate between updaters and fastpath-using readers.  */
struct rcu_sync {
	int			gp_state;
	int			gp_count;
	wait_queue_head_t	gp_wait;

<<<<<<< HEAD
	int			cb_state;
	struct rcu_head		cb_head;

	enum rcu_sync_type	gp_type;
};

extern void rcu_sync_lockdep_assert(struct rcu_sync *);

=======
	struct rcu_head		cb_head;
};

>>>>>>> upstream/android-13
/**
 * rcu_sync_is_idle() - Are readers permitted to use their fastpaths?
 * @rsp: Pointer to rcu_sync structure to use for synchronization
 *
<<<<<<< HEAD
 * Returns true if readers are permitted to use their fastpaths.
 * Must be invoked within an RCU read-side critical section whose
 * flavor matches that of the rcu_sync struture.
 */
static inline bool rcu_sync_is_idle(struct rcu_sync *rsp)
{
#ifdef CONFIG_PROVE_RCU
	rcu_sync_lockdep_assert(rsp);
#endif
	return !rsp->gp_state; /* GP_IDLE */
}

extern void rcu_sync_init(struct rcu_sync *, enum rcu_sync_type);
=======
 * Returns true if readers are permitted to use their fastpaths.  Must be
 * invoked within some flavor of RCU read-side critical section.
 */
static inline bool rcu_sync_is_idle(struct rcu_sync *rsp)
{
	RCU_LOCKDEP_WARN(!rcu_read_lock_any_held(),
			 "suspicious rcu_sync_is_idle() usage");
	return !READ_ONCE(rsp->gp_state); /* GP_IDLE */
}

extern void rcu_sync_init(struct rcu_sync *);
>>>>>>> upstream/android-13
extern void rcu_sync_enter_start(struct rcu_sync *);
extern void rcu_sync_enter(struct rcu_sync *);
extern void rcu_sync_exit(struct rcu_sync *);
extern void rcu_sync_dtor(struct rcu_sync *);

<<<<<<< HEAD
#define __RCU_SYNC_INITIALIZER(name, type) {				\
		.gp_state = 0,						\
		.gp_count = 0,						\
		.gp_wait = __WAIT_QUEUE_HEAD_INITIALIZER(name.gp_wait),	\
		.cb_state = 0,						\
		.gp_type = type,					\
	}

#define	__DEFINE_RCU_SYNC(name, type)	\
	struct rcu_sync_struct name = __RCU_SYNC_INITIALIZER(name, type)

#define DEFINE_RCU_SYNC(name)		\
	__DEFINE_RCU_SYNC(name, RCU_SYNC)

#define DEFINE_RCU_SCHED_SYNC(name)	\
	__DEFINE_RCU_SYNC(name, RCU_SCHED_SYNC)

#define DEFINE_RCU_BH_SYNC(name)	\
	__DEFINE_RCU_SYNC(name, RCU_BH_SYNC)
=======
#define __RCU_SYNC_INITIALIZER(name) {					\
		.gp_state = 0,						\
		.gp_count = 0,						\
		.gp_wait = __WAIT_QUEUE_HEAD_INITIALIZER(name.gp_wait),	\
	}

#define	DEFINE_RCU_SYNC(name)	\
	struct rcu_sync name = __RCU_SYNC_INITIALIZER(name)
>>>>>>> upstream/android-13

#endif /* _LINUX_RCU_SYNC_H_ */
