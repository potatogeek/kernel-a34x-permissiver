/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PERCPU_RWSEM_H
#define _LINUX_PERCPU_RWSEM_H

#include <linux/atomic.h>
<<<<<<< HEAD
#include <linux/rwsem.h>
#include <linux/percpu.h>
#include <linux/rcuwait.h>
#include <linux/rcu_sync.h>
#include <linux/lockdep.h>

struct percpu_rw_semaphore {
	struct rcu_sync		rss;
	unsigned int __percpu	*read_count;
	struct rw_semaphore	rw_sem; /* slowpath */
	struct rcuwait          writer; /* blocked writer */
	int			readers_block;
};

#define DEFINE_STATIC_PERCPU_RWSEM(name)				\
static DEFINE_PER_CPU(unsigned int, __percpu_rwsem_rc_##name);		\
static struct percpu_rw_semaphore name = {				\
	.rss = __RCU_SYNC_INITIALIZER(name.rss, RCU_SCHED_SYNC),	\
	.read_count = &__percpu_rwsem_rc_##name,			\
	.rw_sem = __RWSEM_INITIALIZER(name.rw_sem),			\
	.writer = __RCUWAIT_INITIALIZER(name.writer),			\
}

extern int __percpu_down_read(struct percpu_rw_semaphore *, int);
extern void __percpu_up_read(struct percpu_rw_semaphore *);

static inline void percpu_down_read_preempt_disable(struct percpu_rw_semaphore *sem)
{
	might_sleep();

	rwsem_acquire_read(&sem->rw_sem.dep_map, 0, 0, _RET_IP_);
=======
#include <linux/percpu.h>
#include <linux/rcuwait.h>
#include <linux/wait.h>
#include <linux/rcu_sync.h>
#include <linux/lockdep.h>

void _trace_android_vh_record_pcpu_rwsem_starttime(
		struct task_struct *tsk, unsigned long settime);

struct percpu_rw_semaphore {
	struct rcu_sync		rss;
	unsigned int __percpu	*read_count;
	struct rcuwait		writer;
	/*
	 * destroy_list_entry is used during object destruction when waiters
	 * can't be used, therefore reusing the same space.
	 */
	union {
		wait_queue_head_t	waiters;
		struct list_head	destroy_list_entry;
	};
	atomic_t		block;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};

#ifdef CONFIG_DEBUG_LOCK_ALLOC
#define __PERCPU_RWSEM_DEP_MAP_INIT(lockname)	.dep_map = { .name = #lockname },
#else
#define __PERCPU_RWSEM_DEP_MAP_INIT(lockname)
#endif

#define __DEFINE_PERCPU_RWSEM(name, is_static)				\
static DEFINE_PER_CPU(unsigned int, __percpu_rwsem_rc_##name);		\
is_static struct percpu_rw_semaphore name = {				\
	.rss = __RCU_SYNC_INITIALIZER(name.rss),			\
	.read_count = &__percpu_rwsem_rc_##name,			\
	.writer = __RCUWAIT_INITIALIZER(name.writer),			\
	.waiters = __WAIT_QUEUE_HEAD_INITIALIZER(name.waiters),		\
	.block = ATOMIC_INIT(0),					\
	__PERCPU_RWSEM_DEP_MAP_INIT(name)				\
}

#define DEFINE_PERCPU_RWSEM(name)		\
	__DEFINE_PERCPU_RWSEM(name, /* not static */)
#define DEFINE_STATIC_PERCPU_RWSEM(name)	\
	__DEFINE_PERCPU_RWSEM(name, static)

extern bool __percpu_down_read(struct percpu_rw_semaphore *, bool);

static inline void percpu_down_read(struct percpu_rw_semaphore *sem)
{
	might_sleep();

	rwsem_acquire_read(&sem->dep_map, 0, 0, _RET_IP_);
>>>>>>> upstream/android-13

	preempt_disable();
	/*
	 * We are in an RCU-sched read-side critical section, so the writer
	 * cannot both change sem->state from readers_fast and start checking
	 * counters while we are here. So if we see !sem->state, we know that
	 * the writer won't be checking until we're past the preempt_enable()
<<<<<<< HEAD
	 * and that one the synchronize_sched() is done, the writer will see
	 * anything we did within this RCU-sched read-size critical section.
	 */
	__this_cpu_inc(*sem->read_count);
	if (unlikely(!rcu_sync_is_idle(&sem->rss)))
		__percpu_down_read(sem, false); /* Unconditional memory barrier */
	barrier();
	/*
	 * The barrier() prevents the compiler from
	 * bleeding the critical section out.
	 */
}

static inline void percpu_down_read(struct percpu_rw_semaphore *sem)
{
	percpu_down_read_preempt_disable(sem);
	preempt_enable();
}

static inline int percpu_down_read_trylock(struct percpu_rw_semaphore *sem)
{
	int ret = 1;
=======
	 * and that once the synchronize_rcu() is done, the writer will see
	 * anything we did within this RCU-sched read-size critical section.
	 */
	if (likely(rcu_sync_is_idle(&sem->rss)))
		this_cpu_inc(*sem->read_count);
	else
		__percpu_down_read(sem, false); /* Unconditional memory barrier */
	/*
	 * The preempt_enable() prevents the compiler from
	 * bleeding the critical section out.
	 */
	preempt_enable();
	_trace_android_vh_record_pcpu_rwsem_starttime(current, jiffies);
}

static inline bool percpu_down_read_trylock(struct percpu_rw_semaphore *sem)
{
	bool ret = true;
>>>>>>> upstream/android-13

	preempt_disable();
	/*
	 * Same as in percpu_down_read().
	 */
<<<<<<< HEAD
	__this_cpu_inc(*sem->read_count);
	if (unlikely(!rcu_sync_is_idle(&sem->rss)))
=======
	if (likely(rcu_sync_is_idle(&sem->rss)))
		this_cpu_inc(*sem->read_count);
	else
>>>>>>> upstream/android-13
		ret = __percpu_down_read(sem, true); /* Unconditional memory barrier */
	preempt_enable();
	/*
	 * The barrier() from preempt_enable() prevents the compiler from
	 * bleeding the critical section out.
	 */

<<<<<<< HEAD
	if (ret)
		rwsem_acquire_read(&sem->rw_sem.dep_map, 0, 1, _RET_IP_);
=======
	if (ret) {
		_trace_android_vh_record_pcpu_rwsem_starttime(current, jiffies);
		rwsem_acquire_read(&sem->dep_map, 0, 1, _RET_IP_);
	}
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static inline void percpu_up_read_preempt_enable(struct percpu_rw_semaphore *sem)
{
	/*
	 * The barrier() prevents the compiler from
	 * bleeding the critical section out.
	 */
	barrier();
	/*
	 * Same as in percpu_down_read().
	 */
	if (likely(rcu_sync_is_idle(&sem->rss)))
		__this_cpu_dec(*sem->read_count);
	else
		__percpu_up_read(sem); /* Unconditional memory barrier */
	preempt_enable();

	rwsem_release(&sem->rw_sem.dep_map, 1, _RET_IP_);
}

static inline void percpu_up_read(struct percpu_rw_semaphore *sem)
{
	preempt_disable();
	percpu_up_read_preempt_enable(sem);
=======
static inline void percpu_up_read(struct percpu_rw_semaphore *sem)
{
	_trace_android_vh_record_pcpu_rwsem_starttime(current, 0);
	rwsem_release(&sem->dep_map, _RET_IP_);

	preempt_disable();
	/*
	 * Same as in percpu_down_read().
	 */
	if (likely(rcu_sync_is_idle(&sem->rss))) {
		this_cpu_dec(*sem->read_count);
	} else {
		/*
		 * slowpath; reader will only ever wake a single blocked
		 * writer.
		 */
		smp_mb(); /* B matches C */
		/*
		 * In other words, if they see our decrement (presumably to
		 * aggregate zero, as that is the only time it matters) they
		 * will also see our critical section.
		 */
		this_cpu_dec(*sem->read_count);
		rcuwait_wake_up(&sem->writer);
	}
	preempt_enable();
>>>>>>> upstream/android-13
}

extern void percpu_down_write(struct percpu_rw_semaphore *);
extern void percpu_up_write(struct percpu_rw_semaphore *);

extern int __percpu_init_rwsem(struct percpu_rw_semaphore *,
				const char *, struct lock_class_key *);

<<<<<<< HEAD
extern void percpu_free_rwsem(struct percpu_rw_semaphore *);

=======
/* Can't be called in atomic context. */
extern void percpu_free_rwsem(struct percpu_rw_semaphore *);

/* Invokes percpu_free_rwsem and frees the semaphore from a worker thread. */
extern void percpu_rwsem_async_destroy(struct percpu_rw_semaphore *sem);

>>>>>>> upstream/android-13
#define percpu_init_rwsem(sem)					\
({								\
	static struct lock_class_key rwsem_key;			\
	__percpu_init_rwsem(sem, #sem, &rwsem_key);		\
})

<<<<<<< HEAD
#define percpu_rwsem_is_held(sem) lockdep_is_held(&(sem)->rw_sem)

#define percpu_rwsem_assert_held(sem)				\
	lockdep_assert_held(&(sem)->rw_sem)
=======
#define percpu_rwsem_is_held(sem)	lockdep_is_held(sem)
#define percpu_rwsem_assert_held(sem)	lockdep_assert_held(sem)
>>>>>>> upstream/android-13

static inline void percpu_rwsem_release(struct percpu_rw_semaphore *sem,
					bool read, unsigned long ip)
{
<<<<<<< HEAD
	lock_release(&sem->rw_sem.dep_map, 1, ip);
#ifdef CONFIG_RWSEM_SPIN_ON_OWNER
	if (!read)
		sem->rw_sem.owner = RWSEM_OWNER_UNKNOWN;
#endif
=======
	lock_release(&sem->dep_map, ip);
>>>>>>> upstream/android-13
}

static inline void percpu_rwsem_acquire(struct percpu_rw_semaphore *sem,
					bool read, unsigned long ip)
{
<<<<<<< HEAD
	lock_acquire(&sem->rw_sem.dep_map, 0, 1, read, 1, NULL, ip);
#ifdef CONFIG_RWSEM_SPIN_ON_OWNER
	if (!read)
		sem->rw_sem.owner = current;
#endif
=======
	lock_acquire(&sem->dep_map, 0, 1, read, 1, NULL, ip);
>>>>>>> upstream/android-13
}

#endif
