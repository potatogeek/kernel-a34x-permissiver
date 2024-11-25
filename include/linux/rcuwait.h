/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RCUWAIT_H_
#define _LINUX_RCUWAIT_H_

#include <linux/rcupdate.h>
<<<<<<< HEAD

/*
 * rcuwait provides a way of blocking and waking up a single
 * task in an rcu-safe manner; where it is forbidden to use
 * after exit_notify(). task_struct is not properly rcu protected,
 * unless dealing with rcu-aware lists, ie: find_task_by_*().
 *
 * Alternatively we have task_rcu_dereference(), but the return
 * semantics have different implications which would break the
 * wakeup side. The only time @task is non-nil is when a user is
 * blocked (or checking if it needs to) on a condition, and reset
 * as soon as we know that the condition has succeeded and are
 * awoken.
 */
struct rcuwait {
	struct task_struct *task;
=======
#include <linux/sched/signal.h>

/*
 * rcuwait provides a way of blocking and waking up a single
 * task in an rcu-safe manner.
 *
 * The only time @task is non-nil is when a user is blocked (or
 * checking if it needs to) on a condition, and reset as soon as we
 * know that the condition has succeeded and are awoken.
 */
struct rcuwait {
	struct task_struct __rcu *task;
>>>>>>> upstream/android-13
};

#define __RCUWAIT_INITIALIZER(name)		\
	{ .task = NULL, }

static inline void rcuwait_init(struct rcuwait *w)
{
	w->task = NULL;
}

<<<<<<< HEAD
extern void rcuwait_wake_up(struct rcuwait *w);

/*
 * The caller is responsible for locking around rcuwait_wait_event(),
 * such that writes to @task are properly serialized.
 */
#define rcuwait_wait_event(w, condition)				\
({									\
	/*								\
	 * Complain if we are called after do_exit()/exit_notify(),     \
	 * as we cannot rely on the rcu critical region for the		\
	 * wakeup side.							\
	 */                                                             \
	WARN_ON(current->exit_state);                                   \
									\
	rcu_assign_pointer((w)->task, current);				\
=======
/*
 * Note: this provides no serialization and, just as with waitqueues,
 * requires care to estimate as to whether or not the wait is active.
 */
static inline int rcuwait_active(struct rcuwait *w)
{
	return !!rcu_access_pointer(w->task);
}

extern int rcuwait_wake_up(struct rcuwait *w);

/*
 * The caller is responsible for locking around rcuwait_wait_event(),
 * and [prepare_to/finish]_rcuwait() such that writes to @task are
 * properly serialized.
 */

static inline void prepare_to_rcuwait(struct rcuwait *w)
{
	rcu_assign_pointer(w->task, current);
}

static inline void finish_rcuwait(struct rcuwait *w)
{
        rcu_assign_pointer(w->task, NULL);
	__set_current_state(TASK_RUNNING);
}

#define rcuwait_wait_event(w, condition, state)				\
({									\
	int __ret = 0;							\
	prepare_to_rcuwait(w);						\
>>>>>>> upstream/android-13
	for (;;) {							\
		/*							\
		 * Implicit barrier (A) pairs with (B) in		\
		 * rcuwait_wake_up().					\
		 */							\
<<<<<<< HEAD
		set_current_state(TASK_UNINTERRUPTIBLE);		\
		if (condition)						\
			break;						\
									\
		schedule();						\
	}								\
									\
	WRITE_ONCE((w)->task, NULL);					\
	__set_current_state(TASK_RUNNING);				\
=======
		set_current_state(state);				\
		if (condition)						\
			break;						\
									\
		if (signal_pending_state(state, current)) {		\
			__ret = -EINTR;					\
			break;						\
		}							\
									\
		schedule();						\
	}								\
	finish_rcuwait(w);						\
	__ret;								\
>>>>>>> upstream/android-13
})

#endif /* _LINUX_RCUWAIT_H_ */
