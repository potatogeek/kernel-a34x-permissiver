/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_DEBUG_H
#define _LINUX_SCHED_DEBUG_H

/*
 * Various scheduler/task debugging interfaces:
 */

struct task_struct;
struct pid_namespace;

extern void dump_cpu_task(int cpu);

/*
 * Only dump TASK_* tasks. (0 for all tasks)
 */
<<<<<<< HEAD
extern void show_state_filter(unsigned long state_filter);
=======
extern void show_state_filter(unsigned int state_filter);
>>>>>>> upstream/android-13

static inline void show_state(void)
{
	show_state_filter(0);
}

struct pt_regs;

extern void show_regs(struct pt_regs *);
<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
extern void show_regs_auto_comment(struct pt_regs *regs, bool comm);
#endif
>>>>>>> upstream/android-13

/*
 * TASK is a pointer to the task whose backtrace we want to see (or NULL for current
 * task), SP is the stack pointer of the first frame that should be shown in the back
 * trace (or NULL if the entire call-chain of the task should be shown).
 */
<<<<<<< HEAD
extern void show_stack(struct task_struct *task, unsigned long *sp);

extern void sched_show_task(struct task_struct *p);

=======
extern void show_stack(struct task_struct *task, unsigned long *sp,
		       const char *loglvl);

extern void sched_show_task(struct task_struct *p);

#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
extern void show_stack_auto_comment(struct task_struct *task, unsigned long *sp);
extern void sched_show_task_auto_comment(struct task_struct *p);
#endif

>>>>>>> upstream/android-13
#ifdef CONFIG_SCHED_DEBUG
struct seq_file;
extern void proc_sched_show_task(struct task_struct *p,
				 struct pid_namespace *ns, struct seq_file *m);
extern void proc_sched_set_task(struct task_struct *p);
#endif

/* Attach to any functions which should be ignored in wchan output. */
<<<<<<< HEAD
#define __sched		__attribute__((__section__(".sched.text")))
=======
#define __sched		__section(".sched.text")
>>>>>>> upstream/android-13

/* Linker adds these: start and end of __sched functions */
extern char __sched_text_start[], __sched_text_end[];

/* Is this address in the __sched functions? */
extern int in_sched_functions(unsigned long addr);

#endif /* _LINUX_SCHED_DEBUG_H */
