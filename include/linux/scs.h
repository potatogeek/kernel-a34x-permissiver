/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Shadow Call Stack support.
 *
 * Copyright (C) 2019 Google LLC
 */

#ifndef _LINUX_SCS_H
#define _LINUX_SCS_H

#include <linux/gfp.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <asm/page.h>

#ifdef CONFIG_SHADOW_CALL_STACK

/*
 * In testing, 1 KiB shadow stack size (i.e. 128 stack frames on a 64-bit
 * architecture) provided ~40% safety margin on stack usage while keeping
 * memory allocation overhead reasonable.
 */
#define SCS_SIZE	1024UL
#define GFP_SCS		(GFP_KERNEL | __GFP_ZERO)

/*
 * A random number outside the kernel's virtual address space to mark the
 * end of the shadow stack.
 */
#define SCS_END_MAGIC	0xaf0194819b1635f6UL

#define task_scs(tsk)	(task_thread_info(tsk)->shadow_call_stack)

static inline void task_set_scs(struct task_struct *tsk, void *s)
{
	task_scs(tsk) = s;
}

extern void scs_init(void);
extern void scs_task_reset(struct task_struct *tsk);
extern int scs_prepare(struct task_struct *tsk, int node);
extern bool scs_corrupted(struct task_struct *tsk);
extern void scs_release(struct task_struct *tsk);

#else /* CONFIG_SHADOW_CALL_STACK */

#define task_scs(tsk)	NULL

static inline void task_set_scs(struct task_struct *tsk, void *s) {}
static inline void scs_init(void) {}
static inline void scs_task_reset(struct task_struct *tsk) {}
static inline int scs_prepare(struct task_struct *tsk, int node) { return 0; }
static inline bool scs_corrupted(struct task_struct *tsk) { return false; }
static inline void scs_release(struct task_struct *tsk) {}
=======
#include <linux/poison.h>
#include <linux/sched.h>
#include <linux/sizes.h>

#ifdef CONFIG_SHADOW_CALL_STACK

#define SCS_ORDER		0
#define SCS_SIZE		(PAGE_SIZE << SCS_ORDER)
#define GFP_SCS			(GFP_KERNEL | __GFP_ZERO)

/* An illegal pointer value to mark the end of the shadow stack. */
#define SCS_END_MAGIC		(0x5f6UL + POISON_POINTER_DELTA)

#define task_scs(tsk)		(task_thread_info(tsk)->scs_base)
#define task_scs_sp(tsk)	(task_thread_info(tsk)->scs_sp)

void *scs_alloc(int node);
void scs_free(void *s);
void scs_init(void);
int scs_prepare(struct task_struct *tsk, int node);
void scs_release(struct task_struct *tsk);

static inline void scs_task_reset(struct task_struct *tsk)
{
	/*
	 * Reset the shadow stack to the base address in case the task
	 * is reused.
	 */
	task_scs_sp(tsk) = task_scs(tsk);
}

static inline unsigned long *__scs_magic(void *s)
{
	return (unsigned long *)(s + SCS_SIZE) - 1;
}

static inline bool task_scs_end_corrupted(struct task_struct *tsk)
{
	unsigned long *magic = __scs_magic(task_scs(tsk));
	unsigned long sz = task_scs_sp(tsk) - task_scs(tsk);

	return sz >= SCS_SIZE - 1 || READ_ONCE_NOCHECK(*magic) != SCS_END_MAGIC;
}

#else /* CONFIG_SHADOW_CALL_STACK */

static inline void *scs_alloc(int node) { return NULL; }
static inline void scs_free(void *s) {}
static inline void scs_init(void) {}
static inline void scs_task_reset(struct task_struct *tsk) {}
static inline int scs_prepare(struct task_struct *tsk, int node) { return 0; }
static inline void scs_release(struct task_struct *tsk) {}
static inline bool task_scs_end_corrupted(struct task_struct *tsk) { return false; }
>>>>>>> upstream/android-13

#endif /* CONFIG_SHADOW_CALL_STACK */

#endif /* _LINUX_SCS_H */
