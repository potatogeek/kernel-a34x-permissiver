/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_PREEMPT_H
#define __ASM_PREEMPT_H

#include <linux/thread_info.h>

#define PREEMPT_ENABLED	(0)

static __always_inline int preempt_count(void)
{
	return READ_ONCE(current_thread_info()->preempt_count);
}

static __always_inline volatile int *preempt_count_ptr(void)
{
	return &current_thread_info()->preempt_count;
}

static __always_inline void preempt_count_set(int pc)
{
	*preempt_count_ptr() = pc;
}


#define init_task_preempt_count(p) do { \
	task_thread_info(p)->preempt_count = FORK_PREEMPT_COUNT; \
} while (0)

#define init_idle_preempt_count(p, cpu) do { \
	task_thread_info(p)->preempt_count = PREEMPT_ENABLED; \
} while (0)

static __always_inline void set_preempt_need_resched(void)
{
}

static __always_inline void clear_preempt_need_resched(void)
{
}

static __always_inline bool test_preempt_need_resched(void)
{
	return false;
}



static __always_inline void __preempt_count_add(int val)
{
	*preempt_count_ptr() += val;
}

static __always_inline void __preempt_count_sub(int val)
{
	*preempt_count_ptr() -= val;
}

static __always_inline bool __preempt_count_dec_and_test(void)
{
	
	return !--*preempt_count_ptr() && tif_need_resched();
}


static __always_inline bool should_resched(int preempt_offset)
{
	return unlikely(preempt_count() == preempt_offset &&
			tif_need_resched());
}

#ifdef CONFIG_PREEMPT
extern asmlinkage void preempt_schedule(void);
#define __preempt_schedule() preempt_schedule()
extern asmlinkage void preempt_schedule_notrace(void);
#define __preempt_schedule_notrace() preempt_schedule_notrace()
#endif 

#endif 
