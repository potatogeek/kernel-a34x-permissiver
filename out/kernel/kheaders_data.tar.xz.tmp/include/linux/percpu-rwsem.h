/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PERCPU_RWSEM_H
#define _LINUX_PERCPU_RWSEM_H

#include <linux/atomic.h>
#include <linux/rwsem.h>
#include <linux/percpu.h>
#include <linux/rcuwait.h>
#include <linux/rcu_sync.h>
#include <linux/lockdep.h>

struct percpu_rw_semaphore {
	struct rcu_sync		rss;
	unsigned int __percpu	*read_count;
	struct rw_semaphore	rw_sem; 
	struct rcuwait          writer; 
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

	preempt_disable();
	
	__this_cpu_inc(*sem->read_count);
	if (unlikely(!rcu_sync_is_idle(&sem->rss)))
		__percpu_down_read(sem, false); 
	barrier();
	
}

static inline void percpu_down_read(struct percpu_rw_semaphore *sem)
{
	percpu_down_read_preempt_disable(sem);
	preempt_enable();
}

static inline int percpu_down_read_trylock(struct percpu_rw_semaphore *sem)
{
	int ret = 1;

	preempt_disable();
	
	__this_cpu_inc(*sem->read_count);
	if (unlikely(!rcu_sync_is_idle(&sem->rss)))
		ret = __percpu_down_read(sem, true); 
	preempt_enable();
	

	if (ret)
		rwsem_acquire_read(&sem->rw_sem.dep_map, 0, 1, _RET_IP_);

	return ret;
}

static inline void percpu_up_read_preempt_enable(struct percpu_rw_semaphore *sem)
{
	
	barrier();
	
	if (likely(rcu_sync_is_idle(&sem->rss)))
		__this_cpu_dec(*sem->read_count);
	else
		__percpu_up_read(sem); 
	preempt_enable();

	rwsem_release(&sem->rw_sem.dep_map, 1, _RET_IP_);
}

static inline void percpu_up_read(struct percpu_rw_semaphore *sem)
{
	preempt_disable();
	percpu_up_read_preempt_enable(sem);
}

extern void percpu_down_write(struct percpu_rw_semaphore *);
extern void percpu_up_write(struct percpu_rw_semaphore *);

extern int __percpu_init_rwsem(struct percpu_rw_semaphore *,
				const char *, struct lock_class_key *);

extern void percpu_free_rwsem(struct percpu_rw_semaphore *);

#define percpu_init_rwsem(sem)					\
({								\
	static struct lock_class_key rwsem_key;			\
	__percpu_init_rwsem(sem, #sem, &rwsem_key);		\
})

#define percpu_rwsem_is_held(sem) lockdep_is_held(&(sem)->rw_sem)

#define percpu_rwsem_assert_held(sem)				\
	lockdep_assert_held(&(sem)->rw_sem)

static inline void percpu_rwsem_release(struct percpu_rw_semaphore *sem,
					bool read, unsigned long ip)
{
	lock_release(&sem->rw_sem.dep_map, 1, ip);
#ifdef CONFIG_RWSEM_SPIN_ON_OWNER
	if (!read)
		sem->rw_sem.owner = RWSEM_OWNER_UNKNOWN;
#endif
}

static inline void percpu_rwsem_acquire(struct percpu_rw_semaphore *sem,
					bool read, unsigned long ip)
{
	lock_acquire(&sem->rw_sem.dep_map, 0, 1, read, 1, NULL, ip);
#ifdef CONFIG_RWSEM_SPIN_ON_OWNER
	if (!read)
		sem->rw_sem.owner = current;
#endif
}

#endif
