/* SPDX-License-Identifier: GPL-2.0 */
/*
 * RT Mutexes: blocking mutual exclusion locks with PI support
 *
 * started by Ingo Molnar and Thomas Gleixner:
 *
 *  Copyright (C) 2004-2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 *  Copyright (C) 2006, Timesys Corp., Thomas Gleixner <tglx@timesys.com>
 *
 * This file contains the private data structure and API definitions.
 */

#ifndef __KERNEL_RTMUTEX_COMMON_H
#define __KERNEL_RTMUTEX_COMMON_H

<<<<<<< HEAD
=======
#include <linux/debug_locks.h>
>>>>>>> upstream/android-13
#include <linux/rtmutex.h>
#include <linux/sched/wake_q.h>

/*
 * This is the control structure for tasks blocked on a rt_mutex,
 * which is allocated on the kernel stack on of the blocked task.
 *
 * @tree_entry:		pi node to enqueue into the mutex waiters tree
 * @pi_tree_entry:	pi node to enqueue into the mutex owner waiters tree
 * @task:		task reference to the blocked task
<<<<<<< HEAD
 */
struct rt_mutex_waiter {
	struct rb_node          tree_entry;
	struct rb_node          pi_tree_entry;
	struct task_struct	*task;
	struct rt_mutex		*lock;
#ifdef CONFIG_DEBUG_RT_MUTEXES
	unsigned long		ip;
	struct pid		*deadlock_task_pid;
	struct rt_mutex		*deadlock_lock;
#endif
	int prio;
	u64 deadline;
};

/*
 * Various helpers to access the waiters-tree:
 */

#ifdef CONFIG_RT_MUTEXES

static inline int rt_mutex_has_waiters(struct rt_mutex *lock)
=======
 * @lock:		Pointer to the rt_mutex on which the waiter blocks
 * @wake_state:		Wakeup state to use (TASK_NORMAL or TASK_RTLOCK_WAIT)
 * @prio:		Priority of the waiter
 * @deadline:		Deadline of the waiter if applicable
 * @ww_ctx:		WW context pointer
 */
struct rt_mutex_waiter {
	struct rb_node		tree_entry;
	struct rb_node		pi_tree_entry;
	struct task_struct	*task;
	struct rt_mutex_base	*lock;
	unsigned int		wake_state;
	int			prio;
	u64			deadline;
	struct ww_acquire_ctx	*ww_ctx;
};

/**
 * rt_wake_q_head - Wrapper around regular wake_q_head to support
 *		    "sleeping" spinlocks on RT
 * @head:		The regular wake_q_head for sleeping lock variants
 * @rtlock_task:	Task pointer for RT lock (spin/rwlock) wakeups
 */
struct rt_wake_q_head {
	struct wake_q_head	head;
	struct task_struct	*rtlock_task;
};

#define DEFINE_RT_WAKE_Q(name)						\
	struct rt_wake_q_head name = {					\
		.head		= WAKE_Q_HEAD_INITIALIZER(name.head),	\
		.rtlock_task	= NULL,					\
	}

/*
 * PI-futex support (proxy locking functions, etc.):
 */
extern void rt_mutex_init_proxy_locked(struct rt_mutex_base *lock,
				       struct task_struct *proxy_owner);
extern void rt_mutex_proxy_unlock(struct rt_mutex_base *lock);
extern int __rt_mutex_start_proxy_lock(struct rt_mutex_base *lock,
				     struct rt_mutex_waiter *waiter,
				     struct task_struct *task);
extern int rt_mutex_start_proxy_lock(struct rt_mutex_base *lock,
				     struct rt_mutex_waiter *waiter,
				     struct task_struct *task);
extern int rt_mutex_wait_proxy_lock(struct rt_mutex_base *lock,
			       struct hrtimer_sleeper *to,
			       struct rt_mutex_waiter *waiter);
extern bool rt_mutex_cleanup_proxy_lock(struct rt_mutex_base *lock,
				 struct rt_mutex_waiter *waiter);

extern int rt_mutex_futex_trylock(struct rt_mutex_base *l);
extern int __rt_mutex_futex_trylock(struct rt_mutex_base *l);

extern void rt_mutex_futex_unlock(struct rt_mutex_base *lock);
extern bool __rt_mutex_futex_unlock(struct rt_mutex_base *lock,
				struct rt_wake_q_head *wqh);

extern void rt_mutex_postunlock(struct rt_wake_q_head *wqh);

/*
 * Must be guarded because this header is included from rcu/tree_plugin.h
 * unconditionally.
 */
#ifdef CONFIG_RT_MUTEXES
static inline int rt_mutex_has_waiters(struct rt_mutex_base *lock)
>>>>>>> upstream/android-13
{
	return !RB_EMPTY_ROOT(&lock->waiters.rb_root);
}

<<<<<<< HEAD
static inline struct rt_mutex_waiter *
rt_mutex_top_waiter(struct rt_mutex *lock)
=======
/*
 * Lockless speculative check whether @waiter is still the top waiter on
 * @lock. This is solely comparing pointers and not derefencing the
 * leftmost entry which might be about to vanish.
 */
static inline bool rt_mutex_waiter_is_top_waiter(struct rt_mutex_base *lock,
						 struct rt_mutex_waiter *waiter)
{
	struct rb_node *leftmost = rb_first_cached(&lock->waiters);

	return rb_entry(leftmost, struct rt_mutex_waiter, tree_entry) == waiter;
}

static inline struct rt_mutex_waiter *rt_mutex_top_waiter(struct rt_mutex_base *lock)
>>>>>>> upstream/android-13
{
	struct rb_node *leftmost = rb_first_cached(&lock->waiters);
	struct rt_mutex_waiter *w = NULL;

	if (leftmost) {
		w = rb_entry(leftmost, struct rt_mutex_waiter, tree_entry);
		BUG_ON(w->lock != lock);
	}
	return w;
}

static inline int task_has_pi_waiters(struct task_struct *p)
{
	return !RB_EMPTY_ROOT(&p->pi_waiters.rb_root);
}

<<<<<<< HEAD
static inline struct rt_mutex_waiter *
task_top_pi_waiter(struct task_struct *p)
{
	return rb_entry(p->pi_waiters.rb_leftmost,
			struct rt_mutex_waiter, pi_tree_entry);
}

#else

static inline int rt_mutex_has_waiters(struct rt_mutex *lock)
{
	return false;
}

static inline struct rt_mutex_waiter *
rt_mutex_top_waiter(struct rt_mutex *lock)
{
	return NULL;
}

static inline int task_has_pi_waiters(struct task_struct *p)
{
	return false;
}

static inline struct rt_mutex_waiter *
task_top_pi_waiter(struct task_struct *p)
{
	return NULL;
}

#endif

/*
 * lock->owner state tracking:
 */
#define RT_MUTEX_HAS_WAITERS	1UL

static inline struct task_struct *rt_mutex_owner(struct rt_mutex *lock)
=======
static inline struct rt_mutex_waiter *task_top_pi_waiter(struct task_struct *p)
{
	return rb_entry(p->pi_waiters.rb_leftmost, struct rt_mutex_waiter,
			pi_tree_entry);
}

#define RT_MUTEX_HAS_WAITERS	1UL

static inline struct task_struct *rt_mutex_owner(struct rt_mutex_base *lock)
>>>>>>> upstream/android-13
{
	unsigned long owner = (unsigned long) READ_ONCE(lock->owner);

	return (struct task_struct *) (owner & ~RT_MUTEX_HAS_WAITERS);
}

/*
 * Constants for rt mutex functions which have a selectable deadlock
 * detection.
 *
 * RT_MUTEX_MIN_CHAINWALK:	Stops the lock chain walk when there are
 *				no further PI adjustments to be made.
 *
 * RT_MUTEX_FULL_CHAINWALK:	Invoke deadlock detection with a full
 *				walk of the lock chain.
 */
enum rtmutex_chainwalk {
	RT_MUTEX_MIN_CHAINWALK,
	RT_MUTEX_FULL_CHAINWALK,
};

<<<<<<< HEAD
/*
 * PI-futex support (proxy locking functions, etc.):
 */
extern struct task_struct *rt_mutex_next_owner(struct rt_mutex *lock);
extern void rt_mutex_init_proxy_locked(struct rt_mutex *lock,
				       struct task_struct *proxy_owner);
extern void rt_mutex_proxy_unlock(struct rt_mutex *lock);
extern void rt_mutex_init_waiter(struct rt_mutex_waiter *waiter);
extern int __rt_mutex_start_proxy_lock(struct rt_mutex *lock,
				     struct rt_mutex_waiter *waiter,
				     struct task_struct *task);
extern int rt_mutex_start_proxy_lock(struct rt_mutex *lock,
				     struct rt_mutex_waiter *waiter,
				     struct task_struct *task);
extern int rt_mutex_wait_proxy_lock(struct rt_mutex *lock,
			       struct hrtimer_sleeper *to,
			       struct rt_mutex_waiter *waiter);
extern bool rt_mutex_cleanup_proxy_lock(struct rt_mutex *lock,
				 struct rt_mutex_waiter *waiter);

extern int rt_mutex_futex_trylock(struct rt_mutex *l);
extern int __rt_mutex_futex_trylock(struct rt_mutex *l);

extern void rt_mutex_futex_unlock(struct rt_mutex *lock);
extern bool __rt_mutex_futex_unlock(struct rt_mutex *lock,
				 struct wake_q_head *wqh);

extern void rt_mutex_postunlock(struct wake_q_head *wake_q);

#ifdef CONFIG_DEBUG_RT_MUTEXES
# include "rtmutex-debug.h"
#else
# include "rtmutex.h"
#endif
=======
static inline void __rt_mutex_base_init(struct rt_mutex_base *lock)
{
	raw_spin_lock_init(&lock->wait_lock);
	lock->waiters = RB_ROOT_CACHED;
	lock->owner = NULL;
}

/* Debug functions */
static inline void debug_rt_mutex_unlock(struct rt_mutex_base *lock)
{
	if (IS_ENABLED(CONFIG_DEBUG_RT_MUTEXES))
		DEBUG_LOCKS_WARN_ON(rt_mutex_owner(lock) != current);
}

static inline void debug_rt_mutex_proxy_unlock(struct rt_mutex_base *lock)
{
	if (IS_ENABLED(CONFIG_DEBUG_RT_MUTEXES))
		DEBUG_LOCKS_WARN_ON(!rt_mutex_owner(lock));
}

static inline void debug_rt_mutex_init_waiter(struct rt_mutex_waiter *waiter)
{
	if (IS_ENABLED(CONFIG_DEBUG_RT_MUTEXES))
		memset(waiter, 0x11, sizeof(*waiter));
}

static inline void debug_rt_mutex_free_waiter(struct rt_mutex_waiter *waiter)
{
	if (IS_ENABLED(CONFIG_DEBUG_RT_MUTEXES))
		memset(waiter, 0x22, sizeof(*waiter));
}

static inline void rt_mutex_init_waiter(struct rt_mutex_waiter *waiter)
{
	debug_rt_mutex_init_waiter(waiter);
	RB_CLEAR_NODE(&waiter->pi_tree_entry);
	RB_CLEAR_NODE(&waiter->tree_entry);
	waiter->wake_state = TASK_NORMAL;
	waiter->task = NULL;
}

static inline void rt_mutex_init_rtlock_waiter(struct rt_mutex_waiter *waiter)
{
	rt_mutex_init_waiter(waiter);
	waiter->wake_state = TASK_RTLOCK_WAIT;
}

#else /* CONFIG_RT_MUTEXES */
/* Used in rcu/tree_plugin.h */
static inline struct task_struct *rt_mutex_owner(struct rt_mutex_base *lock)
{
	return NULL;
}
#endif  /* !CONFIG_RT_MUTEXES */
>>>>>>> upstream/android-13

#endif
