<<<<<<< HEAD
/*
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __ASM_STACKTRACE_H
#define __ASM_STACKTRACE_H

#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/sched/task_stack.h>
<<<<<<< HEAD
=======
#include <linux/types.h>
>>>>>>> upstream/android-13

#include <asm/memory.h>
#include <asm/ptrace.h>
#include <asm/sdei.h>

<<<<<<< HEAD
struct stackframe {
	unsigned long fp;
	unsigned long pc;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	int graph;
#endif
};

=======
>>>>>>> upstream/android-13
enum stack_type {
	STACK_TYPE_UNKNOWN,
	STACK_TYPE_TASK,
	STACK_TYPE_IRQ,
	STACK_TYPE_OVERFLOW,
	STACK_TYPE_SDEI_NORMAL,
	STACK_TYPE_SDEI_CRITICAL,
<<<<<<< HEAD
=======
	__NR_STACK_TYPES
>>>>>>> upstream/android-13
};

struct stack_info {
	unsigned long low;
	unsigned long high;
	enum stack_type type;
};

<<<<<<< HEAD
extern int unwind_frame(struct task_struct *tsk, struct stackframe *frame);
extern void walk_stackframe(struct task_struct *tsk, struct stackframe *frame,
			    int (*fn)(struct stackframe *, void *), void *data);
extern void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk);
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
=======
/*
 * A snapshot of a frame record or fp/lr register values, along with some
 * accounting information necessary for robust unwinding.
 *
 * @fp:          The fp value in the frame record (or the real fp)
 * @pc:          The lr value in the frame record (or the real lr)
 *
 * @stacks_done: Stacks which have been entirely unwound, for which it is no
 *               longer valid to unwind to.
 *
 * @prev_fp:     The fp that pointed to this frame record, or a synthetic value
 *               of 0. This is used to ensure that within a stack, each
 *               subsequent frame record is at an increasing address.
 * @prev_type:   The type of stack this frame record was on, or a synthetic
 *               value of STACK_TYPE_UNKNOWN. This is used to detect a
 *               transition from one stack to another.
 *
 * @graph:       When FUNCTION_GRAPH_TRACER is selected, holds the index of a
 *               replacement lr value in the ftrace graph stack.
 */
struct stackframe {
	unsigned long fp;
	unsigned long pc;
	DECLARE_BITMAP(stacks_done, __NR_STACK_TYPES);
	unsigned long prev_fp;
	enum stack_type prev_type;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	int graph;
#endif
};

extern int unwind_frame(struct task_struct *tsk, struct stackframe *frame);
extern void walk_stackframe(struct task_struct *tsk, struct stackframe *frame,
			    bool (*fn)(void *, unsigned long), void *data);
extern void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk,
			   const char *loglvl);
#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
>>>>>>> upstream/android-13
extern void dump_backtrace_auto_comment(struct pt_regs *regs, struct task_struct *tsk);
#endif

DECLARE_PER_CPU(unsigned long *, irq_stack_ptr);

<<<<<<< HEAD
#ifdef CONFIG_SHADOW_CALL_STACK
DECLARE_PER_CPU(unsigned long *, irq_shadow_call_stack_ptr);
#endif

static inline bool on_irq_stack(unsigned long sp,
=======
static inline bool on_stack(unsigned long sp, unsigned long size,
			    unsigned long low, unsigned long high,
			    enum stack_type type, struct stack_info *info)
{
	if (!low)
		return false;

	if (sp < low || sp + size < sp || sp + size > high)
		return false;

	if (info) {
		info->low = low;
		info->high = high;
		info->type = type;
	}
	return true;
}

static inline bool on_irq_stack(unsigned long sp, unsigned long size,
>>>>>>> upstream/android-13
				struct stack_info *info)
{
	unsigned long low = (unsigned long)raw_cpu_read(irq_stack_ptr);
	unsigned long high = low + IRQ_STACK_SIZE;

<<<<<<< HEAD
	if (!low)
		return false;

	if (sp < low || sp >= high)
		return false;

	if (info) {
		info->low = low;
		info->high = high;
		info->type = STACK_TYPE_IRQ;
	}

	return true;
}

static inline bool on_task_stack(struct task_struct *tsk, unsigned long sp,
				struct stack_info *info)
=======
	return on_stack(sp, size, low, high, STACK_TYPE_IRQ, info);
}

static inline bool on_task_stack(const struct task_struct *tsk,
				 unsigned long sp, unsigned long size,
				 struct stack_info *info)
>>>>>>> upstream/android-13
{
	unsigned long low = (unsigned long)task_stack_page(tsk);
	unsigned long high = low + THREAD_SIZE;

<<<<<<< HEAD
	if (sp < low || sp >= high)
		return false;

	if (info) {
		info->low = low;
		info->high = high;
		info->type = STACK_TYPE_TASK;
	}

	return true;
=======
	return on_stack(sp, size, low, high, STACK_TYPE_TASK, info);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_VMAP_STACK
DECLARE_PER_CPU(unsigned long [OVERFLOW_STACK_SIZE/sizeof(long)], overflow_stack);

<<<<<<< HEAD
static inline bool on_overflow_stack(unsigned long sp,
=======
static inline bool on_overflow_stack(unsigned long sp, unsigned long size,
>>>>>>> upstream/android-13
				struct stack_info *info)
{
	unsigned long low = (unsigned long)raw_cpu_ptr(overflow_stack);
	unsigned long high = low + OVERFLOW_STACK_SIZE;

<<<<<<< HEAD
	if (sp < low || sp >= high)
		return false;

	if (info) {
		info->low = low;
		info->high = high;
		info->type = STACK_TYPE_OVERFLOW;
	}

	return true;
}
#else
static inline bool on_overflow_stack(unsigned long sp,
=======
	return on_stack(sp, size, low, high, STACK_TYPE_OVERFLOW, info);
}
#else
static inline bool on_overflow_stack(unsigned long sp, unsigned long size,
>>>>>>> upstream/android-13
			struct stack_info *info) { return false; }
#endif


/*
 * We can only safely access per-cpu stacks from current in a non-preemptible
 * context.
 */
<<<<<<< HEAD
static inline bool on_accessible_stack(struct task_struct *tsk,
					unsigned long sp,
					struct stack_info *info)
{
	if (on_task_stack(tsk, sp, info))
		return true;
	if (tsk != current || preemptible())
		return false;
	if (on_irq_stack(sp, info))
		return true;
	if (on_overflow_stack(sp, info))
		return true;
	if (on_sdei_stack(sp, info))
=======
static inline bool on_accessible_stack(const struct task_struct *tsk,
				       unsigned long sp, unsigned long size,
				       struct stack_info *info)
{
	if (info)
		info->type = STACK_TYPE_UNKNOWN;

	if (on_task_stack(tsk, sp, size, info))
		return true;
	if (tsk != current || preemptible())
		return false;
	if (on_irq_stack(sp, size, info))
		return true;
	if (on_overflow_stack(sp, size, info))
		return true;
	if (on_sdei_stack(sp, size, info))
>>>>>>> upstream/android-13
		return true;

	return false;
}

<<<<<<< HEAD
=======
void start_backtrace(struct stackframe *frame, unsigned long fp,
		     unsigned long pc);

>>>>>>> upstream/android-13
#endif	/* __ASM_STACKTRACE_H */
