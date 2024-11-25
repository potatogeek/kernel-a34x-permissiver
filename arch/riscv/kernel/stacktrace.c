<<<<<<< HEAD
/*
 * Copyright (C) 2008 ARM Limited
 * Copyright (C) 2014 Regents of the University of California
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2008 ARM Limited
 * Copyright (C) 2014 Regents of the University of California
>>>>>>> upstream/android-13
 */

#include <linux/export.h>
#include <linux/kallsyms.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <linux/stacktrace.h>
#include <linux/ftrace.h>

<<<<<<< HEAD
#ifdef CONFIG_FRAME_POINTER

struct stackframe {
	unsigned long fp;
	unsigned long ra;
};

static void notrace walk_stackframe(struct task_struct *task,
	struct pt_regs *regs, bool (*fn)(unsigned long, void *), void *arg)
{
	unsigned long fp, sp, pc;

	if (regs) {
		fp = GET_FP(regs);
		sp = GET_USP(regs);
		pc = GET_IP(regs);
	} else if (task == NULL || task == current) {
		const register unsigned long current_sp __asm__ ("sp");
		fp = (unsigned long)__builtin_frame_address(0);
		sp = current_sp;
=======
#include <asm/stacktrace.h>

register unsigned long sp_in_global __asm__("sp");

#ifdef CONFIG_FRAME_POINTER

void notrace walk_stackframe(struct task_struct *task, struct pt_regs *regs,
			     bool (*fn)(void *, unsigned long), void *arg)
{
	unsigned long fp, sp, pc;
	int level = 0;

	if (regs) {
		fp = frame_pointer(regs);
		sp = user_stack_pointer(regs);
		pc = instruction_pointer(regs);
	} else if (task == NULL || task == current) {
		fp = (unsigned long)__builtin_frame_address(0);
		sp = sp_in_global;
>>>>>>> upstream/android-13
		pc = (unsigned long)walk_stackframe;
	} else {
		/* task blocked in __switch_to */
		fp = task->thread.s[0];
		sp = task->thread.sp;
		pc = task->thread.ra;
	}

	for (;;) {
		unsigned long low, high;
		struct stackframe *frame;

<<<<<<< HEAD
		if (unlikely(!__kernel_text_address(pc) || fn(pc, arg)))
=======
		if (unlikely(!__kernel_text_address(pc) || (level++ >= 1 && !fn(arg, pc))))
>>>>>>> upstream/android-13
			break;

		/* Validate frame pointer */
		low = sp + sizeof(struct stackframe);
		high = ALIGN(sp, THREAD_SIZE);
		if (unlikely(fp < low || fp > high || fp & 0x7))
			break;
		/* Unwind stack frame */
		frame = (struct stackframe *)fp - 1;
		sp = fp;
<<<<<<< HEAD
		fp = frame->fp;
#ifdef HAVE_FUNCTION_GRAPH_RET_ADDR_PTR
		pc = ftrace_graph_ret_addr(current, NULL, frame->ra,
					   (unsigned long *)(fp - 8));
#else
		pc = frame->ra - 0x4;
#endif
=======
		if (regs && (regs->epc == pc) && (frame->fp & 0x7)) {
			fp = frame->ra;
			pc = regs->ra;
		} else {
			fp = frame->fp;
			pc = ftrace_graph_ret_addr(current, NULL, frame->ra,
						   (unsigned long *)(fp - 8));
		}

>>>>>>> upstream/android-13
	}
}

#else /* !CONFIG_FRAME_POINTER */

void notrace walk_stackframe(struct task_struct *task,
<<<<<<< HEAD
	struct pt_regs *regs, bool (*fn)(unsigned long, void *), void *arg)
=======
	struct pt_regs *regs, bool (*fn)(void *, unsigned long), void *arg)
>>>>>>> upstream/android-13
{
	unsigned long sp, pc;
	unsigned long *ksp;

	if (regs) {
<<<<<<< HEAD
		sp = GET_USP(regs);
		pc = GET_IP(regs);
	} else if (task == NULL || task == current) {
		const register unsigned long current_sp __asm__ ("sp");
		sp = current_sp;
=======
		sp = user_stack_pointer(regs);
		pc = instruction_pointer(regs);
	} else if (task == NULL || task == current) {
		sp = sp_in_global;
>>>>>>> upstream/android-13
		pc = (unsigned long)walk_stackframe;
	} else {
		/* task blocked in __switch_to */
		sp = task->thread.sp;
		pc = task->thread.ra;
	}

	if (unlikely(sp & 0x7))
		return;

	ksp = (unsigned long *)sp;
	while (!kstack_end(ksp)) {
<<<<<<< HEAD
		if (__kernel_text_address(pc) && unlikely(fn(pc, arg)))
=======
		if (__kernel_text_address(pc) && unlikely(!fn(arg, pc)))
>>>>>>> upstream/android-13
			break;
		pc = (*ksp++) - 0x4;
	}
}

#endif /* CONFIG_FRAME_POINTER */

<<<<<<< HEAD

static bool print_trace_address(unsigned long pc, void *arg)
{
	print_ip_sym(pc);
	return false;
}

void show_stack(struct task_struct *task, unsigned long *sp)
{
	pr_cont("Call Trace:\n");
	walk_stackframe(task, NULL, print_trace_address, NULL);
}


static bool save_wchan(unsigned long pc, void *arg)
=======
static bool print_trace_address(void *arg, unsigned long pc)
{
	const char *loglvl = arg;

	print_ip_sym(loglvl, pc);
	return true;
}

noinline void dump_backtrace(struct pt_regs *regs, struct task_struct *task,
		    const char *loglvl)
{
	walk_stackframe(task, regs, print_trace_address, (void *)loglvl);
}

void show_stack(struct task_struct *task, unsigned long *sp, const char *loglvl)
{
	pr_cont("%sCall Trace:\n", loglvl);
	dump_backtrace(NULL, task, loglvl);
}

static bool save_wchan(void *arg, unsigned long pc)
>>>>>>> upstream/android-13
{
	if (!in_sched_functions(pc)) {
		unsigned long *p = arg;
		*p = pc;
<<<<<<< HEAD
		return true;
	}
	return false;
=======
		return false;
	}
	return true;
>>>>>>> upstream/android-13
}

unsigned long get_wchan(struct task_struct *task)
{
	unsigned long pc = 0;

<<<<<<< HEAD
	if (likely(task && task != current && task->state != TASK_RUNNING))
		walk_stackframe(task, NULL, save_wchan, &pc);
	return pc;
}


#ifdef CONFIG_STACKTRACE

static bool __save_trace(unsigned long pc, void *arg, bool nosched)
{
	struct stack_trace *trace = arg;

	if (unlikely(nosched && in_sched_functions(pc)))
		return false;
	if (unlikely(trace->skip > 0)) {
		trace->skip--;
		return false;
	}

	trace->entries[trace->nr_entries++] = pc;
	return (trace->nr_entries >= trace->max_entries);
}

static bool save_trace(unsigned long pc, void *arg)
{
	return __save_trace(pc, arg, false);
}

/*
 * Save stack-backtrace addresses into a stack_trace buffer.
 */
void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	walk_stackframe(tsk, NULL, save_trace, trace);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);

void save_stack_trace(struct stack_trace *trace)
{
	save_stack_trace_tsk(NULL, trace);
}
EXPORT_SYMBOL_GPL(save_stack_trace);

=======
	if (likely(task && task != current && !task_is_running(task))) {
		if (!try_get_task_stack(task))
			return 0;
		walk_stackframe(task, NULL, save_wchan, &pc);
		put_task_stack(task);
	}
	return pc;
}

#ifdef CONFIG_STACKTRACE

noinline void arch_stack_walk(stack_trace_consume_fn consume_entry, void *cookie,
		     struct task_struct *task, struct pt_regs *regs)
{
	walk_stackframe(task, regs, consume_entry, cookie);
}

>>>>>>> upstream/android-13
#endif /* CONFIG_STACKTRACE */
