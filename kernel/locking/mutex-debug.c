/*
<<<<<<< HEAD
 * kernel/mutex-debug.c
 *
=======
>>>>>>> upstream/android-13
 * Debugging code for mutexes
 *
 * Started by Ingo Molnar:
 *
 *  Copyright (C) 2004, 2005, 2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 *
 * lock debugging, locking tree, deadlock detection started by:
 *
 *  Copyright (C) 2004, LynuxWorks, Inc., Igor Manyilov, Bill Huey
 *  Released under the General Public License (GPL).
 */
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/poison.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/kallsyms.h>
#include <linux/interrupt.h>
#include <linux/debug_locks.h>

<<<<<<< HEAD
#include "mutex-debug.h"
=======
#include "mutex.h"
>>>>>>> upstream/android-13

/*
 * Must be called with lock->wait_lock held.
 */
void debug_mutex_lock_common(struct mutex *lock, struct mutex_waiter *waiter)
{
	memset(waiter, MUTEX_DEBUG_INIT, sizeof(*waiter));
	waiter->magic = waiter;
	INIT_LIST_HEAD(&waiter->list);
<<<<<<< HEAD
=======
	waiter->ww_ctx = MUTEX_POISON_WW_CTX;
>>>>>>> upstream/android-13
}

void debug_mutex_wake_waiter(struct mutex *lock, struct mutex_waiter *waiter)
{
<<<<<<< HEAD
	SMP_DEBUG_LOCKS_WARN_ON(!spin_is_locked(&lock->wait_lock));
=======
	lockdep_assert_held(&lock->wait_lock);
>>>>>>> upstream/android-13
	DEBUG_LOCKS_WARN_ON(list_empty(&lock->wait_list));
	DEBUG_LOCKS_WARN_ON(waiter->magic != waiter);
	DEBUG_LOCKS_WARN_ON(list_empty(&waiter->list));
}

void debug_mutex_free_waiter(struct mutex_waiter *waiter)
{
	DEBUG_LOCKS_WARN_ON(!list_empty(&waiter->list));
	memset(waiter, MUTEX_DEBUG_FREE, sizeof(*waiter));
}

void debug_mutex_add_waiter(struct mutex *lock, struct mutex_waiter *waiter,
			    struct task_struct *task)
{
<<<<<<< HEAD
	SMP_DEBUG_LOCKS_WARN_ON(!spin_is_locked(&lock->wait_lock));
=======
	lockdep_assert_held(&lock->wait_lock);
>>>>>>> upstream/android-13

	/* Mark the current thread as blocked on the lock: */
	task->blocked_on = waiter;
}

<<<<<<< HEAD
void mutex_remove_waiter(struct mutex *lock, struct mutex_waiter *waiter,
=======
void debug_mutex_remove_waiter(struct mutex *lock, struct mutex_waiter *waiter,
>>>>>>> upstream/android-13
			 struct task_struct *task)
{
	DEBUG_LOCKS_WARN_ON(list_empty(&waiter->list));
	DEBUG_LOCKS_WARN_ON(waiter->task != task);
	DEBUG_LOCKS_WARN_ON(task->blocked_on != waiter);
	task->blocked_on = NULL;

<<<<<<< HEAD
	list_del_init(&waiter->list);
=======
	INIT_LIST_HEAD(&waiter->list);
>>>>>>> upstream/android-13
	waiter->task = NULL;
}

void debug_mutex_unlock(struct mutex *lock)
{
	if (likely(debug_locks)) {
		DEBUG_LOCKS_WARN_ON(lock->magic != lock);
		DEBUG_LOCKS_WARN_ON(!lock->wait_list.prev && !lock->wait_list.next);
	}
}

void debug_mutex_init(struct mutex *lock, const char *name,
		      struct lock_class_key *key)
{
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	/*
	 * Make sure we are not reinitializing a held lock:
	 */
	debug_check_no_locks_freed((void *)lock, sizeof(*lock));
<<<<<<< HEAD
	lockdep_init_map(&lock->dep_map, name, key, 0);
=======
	lockdep_init_map_wait(&lock->dep_map, name, key, 0, LD_WAIT_SLEEP);
>>>>>>> upstream/android-13
#endif
	lock->magic = lock;
}

/***
 * mutex_destroy - mark a mutex unusable
 * @lock: the mutex to be destroyed
 *
 * This function marks the mutex uninitialized, and any subsequent
 * use of the mutex is forbidden. The mutex must not be locked when
 * this function is called.
 */
void mutex_destroy(struct mutex *lock)
{
	DEBUG_LOCKS_WARN_ON(mutex_is_locked(lock));
	lock->magic = NULL;
}

EXPORT_SYMBOL_GPL(mutex_destroy);
