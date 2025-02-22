/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_SCS_H
#define _LINUX_SCS_H

#include <linux/gfp.h>
#include <linux/sched.h>
#include <asm/page.h>

#ifdef CONFIG_SHADOW_CALL_STACK


#define SCS_SIZE	1024UL
#define GFP_SCS		(GFP_KERNEL | __GFP_ZERO)


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

#else 

#define task_scs(tsk)	NULL

static inline void task_set_scs(struct task_struct *tsk, void *s) {}
static inline void scs_init(void) {}
static inline void scs_task_reset(struct task_struct *tsk) {}
static inline int scs_prepare(struct task_struct *tsk, int node) { return 0; }
static inline bool scs_corrupted(struct task_struct *tsk) { return false; }
static inline void scs_release(struct task_struct *tsk) {}

#endif 

#endif 
