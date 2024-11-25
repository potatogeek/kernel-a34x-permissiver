<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Stack tracing support
 *
 * Copyright (C) 2012 ARM Ltd.
<<<<<<< HEAD
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
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <linux/stacktrace.h>
<<<<<<< HEAD

#include <asm/irq.h>
=======
#include <linux/sec_debug.h>

#include <asm/irq.h>
#include <asm/pointer_auth.h>
>>>>>>> upstream/android-13
#include <asm/stack_pointer.h>
#include <asm/stacktrace.h>

/*
 * AArch64 PCS assigns the frame pointer to x29.
 *
 * A simple function prologue looks like this:
 * 	sub	sp, sp, #0x10
 *   	stp	x29, x30, [sp]
 *	mov	x29, sp
 *
 * A simple function epilogue looks like this:
 *	mov	sp, x29
 *	ldp	x29, x30, [sp]
 *	add	sp, sp, #0x10
 */
<<<<<<< HEAD
int notrace unwind_frame(struct task_struct *tsk, struct stackframe *frame)
{
	unsigned long fp = frame->fp;

	if (fp & 0xf)
		return -EINVAL;
=======


notrace void start_backtrace(struct stackframe *frame, unsigned long fp,
		     unsigned long pc)
{
	frame->fp = fp;
	frame->pc = pc;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame->graph = 0;
#endif

	/*
	 * Prime the first unwind.
	 *
	 * In unwind_frame() we'll check that the FP points to a valid stack,
	 * which can't be STACK_TYPE_UNKNOWN, and the first unwind will be
	 * treated as a transition to whichever stack that happens to be. The
	 * prev_fp value won't be used, but we set it to 0 such that it is
	 * definitely not an accessible stack address.
	 */
	bitmap_zero(frame->stacks_done, __NR_STACK_TYPES);
	frame->prev_fp = 0;
	frame->prev_type = STACK_TYPE_UNKNOWN;
}
NOKPROBE_SYMBOL(start_backtrace);
EXPORT_SYMBOL_GPL(start_backtrace);

/*
 * Unwind from one frame record (A) to the next frame record (B).
 *
 * We terminate early if the location of B indicates a malformed chain of frame
 * records (e.g. a cycle), determined based on the location and fp value of A
 * and the location (but not the fp value) of B.
 */
int notrace unwind_frame(struct task_struct *tsk, struct stackframe *frame)
{
	unsigned long fp = frame->fp;
	struct stack_info info;
>>>>>>> upstream/android-13

	if (!tsk)
		tsk = current;

<<<<<<< HEAD
	if (!on_accessible_stack(tsk, fp, NULL))
		return -EINVAL;

	frame->fp = READ_ONCE_NOCHECK(*(unsigned long *)(fp));
	frame->pc = READ_ONCE_NOCHECK(*(unsigned long *)(fp + 8));

#ifdef CONFIG_SEC_DEBUG_LIMIT_BACKTRACE
	if (frame->fp == fp)
		return -EINVAL;
#endif

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	if (tsk->ret_stack &&
			(frame->pc == (unsigned long)return_to_handler)) {
		if (WARN_ON_ONCE(frame->graph == -1))
			return -EINVAL;
		if (frame->graph < -1)
			frame->graph += FTRACE_NOTRACE_DEPTH;

=======
	/* Final frame; nothing to unwind */
	if (fp == (unsigned long)task_pt_regs(tsk)->stackframe)
		return -ENOENT;

	if (fp & 0x7)
		return -EINVAL;

	if (!on_accessible_stack(tsk, fp, 16, &info))
		return -EINVAL;

	if (test_bit(info.type, frame->stacks_done))
		return -EINVAL;

	/*
	 * As stacks grow downward, any valid record on the same stack must be
	 * at a strictly higher address than the prior record.
	 *
	 * Stacks can nest in several valid orders, e.g.
	 *
	 * TASK -> IRQ -> OVERFLOW -> SDEI_NORMAL
	 * TASK -> SDEI_NORMAL -> SDEI_CRITICAL -> OVERFLOW
	 *
	 * ... but the nesting itself is strict. Once we transition from one
	 * stack to another, it's never valid to unwind back to that first
	 * stack.
	 */
	if (info.type == frame->prev_type) {
		if (fp <= frame->prev_fp)
			return -EINVAL;
	} else {
		set_bit(frame->prev_type, frame->stacks_done);
	}

	/*
	 * Record this frame record's values and location. The prev_fp and
	 * prev_type are only meaningful to the next unwind_frame() invocation.
	 */
	frame->fp = READ_ONCE_NOCHECK(*(unsigned long *)(fp));
	frame->pc = READ_ONCE_NOCHECK(*(unsigned long *)(fp + 8));
	frame->prev_fp = fp;
	frame->prev_type = info.type;

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	if (tsk->ret_stack &&
		(ptrauth_strip_insn_pac(frame->pc) == (unsigned long)return_to_handler)) {
		struct ftrace_ret_stack *ret_stack;
>>>>>>> upstream/android-13
		/*
		 * This is a case where function graph tracer has
		 * modified a return address (LR) in a stack frame
		 * to hook a function return.
		 * So replace it to an original value.
		 */
<<<<<<< HEAD
		frame->pc = tsk->ret_stack[frame->graph--].ret;
	}
#endif /* CONFIG_FUNCTION_GRAPH_TRACER */

	/*
	 * Frames created upon entry from EL0 have NULL FP and PC values, so
	 * don't bother reporting these. Frames created by __noreturn functions
	 * might have a valid FP even if PC is bogus, so only terminate where
	 * both are NULL.
	 */
	if (!frame->fp && !frame->pc)
		return -EINVAL;
=======
		ret_stack = ftrace_graph_get_ret_stack(tsk, frame->graph++);
		if (WARN_ON_ONCE(!ret_stack))
			return -EINVAL;
		frame->pc = ret_stack->ret;
	}
#endif /* CONFIG_FUNCTION_GRAPH_TRACER */

	frame->pc = ptrauth_strip_insn_pac(frame->pc);
>>>>>>> upstream/android-13

	return 0;
}
NOKPROBE_SYMBOL(unwind_frame);

void notrace walk_stackframe(struct task_struct *tsk, struct stackframe *frame,
<<<<<<< HEAD
		     int (*fn)(struct stackframe *, void *), void *data)
=======
			     bool (*fn)(void *, unsigned long), void *data)
>>>>>>> upstream/android-13
{
	while (1) {
		int ret;

<<<<<<< HEAD
		if (fn(frame, data))
=======
		if (!fn(data, frame->pc))
>>>>>>> upstream/android-13
			break;
		ret = unwind_frame(tsk, frame);
		if (ret < 0)
			break;
	}
}
NOKPROBE_SYMBOL(walk_stackframe);

<<<<<<< HEAD
#ifdef CONFIG_STACKTRACE
struct stack_trace_data {
	struct stack_trace *trace;
	unsigned int no_sched_functions;
	unsigned int skip;
};

static int save_trace(struct stackframe *frame, void *d)
{
	struct stack_trace_data *data = d;
	struct stack_trace *trace = data->trace;
	unsigned long addr = frame->pc;

	if (data->no_sched_functions && in_sched_functions(addr))
		return 0;
	if (data->skip) {
		data->skip--;
		return 0;
	}

	trace->entries[trace->nr_entries++] = addr;

	return trace->nr_entries >= trace->max_entries;
}

void save_stack_trace_regs(struct pt_regs *regs, struct stack_trace *trace)
{
	struct stack_trace_data data;
	struct stackframe frame;

	data.trace = trace;
	data.skip = trace->skip;
	data.no_sched_functions = 0;

	frame.fp = regs->regs[29];
	frame.pc = regs->pc;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame.graph = current->curr_ret_stack;
#endif

	walk_stackframe(current, &frame, save_trace, &data);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}

static noinline void __save_stack_trace(struct task_struct *tsk,
	struct stack_trace *trace, unsigned int nosched)
{
	struct stack_trace_data data;
	struct stackframe frame;
=======
static void dump_backtrace_entry(unsigned long where, const char *loglvl)
{
	printk("%s %pSb\n", loglvl, (void *)where);
}

#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
static void dump_backtrace_entry_auto_comment(unsigned long where)
{
	pr_auto(ASL2, " %pSb\n", (void *)where);
}
#endif

void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk,
		    const char *loglvl)
{
	struct stackframe frame;
	int skip = 0;

	pr_debug("%s(regs = %p tsk = %p)\n", __func__, regs, tsk);

	if (regs) {
		if (user_mode(regs))
			return;
		skip = 1;
	}

	if (!tsk)
		tsk = current;
>>>>>>> upstream/android-13

	if (!try_get_task_stack(tsk))
		return;

<<<<<<< HEAD
	data.trace = trace;
	data.skip = trace->skip;
	data.no_sched_functions = nosched;

	if (tsk != current) {
		frame.fp = thread_saved_fp(tsk);
		frame.pc = thread_saved_pc(tsk);
	} else {
		/* We don't want this function nor the caller */
		data.skip += 2;
		frame.fp = (unsigned long)__builtin_frame_address(0);
		frame.pc = (unsigned long)__save_stack_trace;
	}
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame.graph = tsk->curr_ret_stack;
#endif

	walk_stackframe(tsk, &frame, save_trace, &data);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;

	put_task_stack(tsk);
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	__save_stack_trace(tsk, trace, 1);
}

void save_stack_trace(struct stack_trace *trace)
{
	__save_stack_trace(current, trace, 0);
}

EXPORT_SYMBOL_GPL(save_stack_trace);
=======
	if (tsk == current) {
		start_backtrace(&frame,
				(unsigned long)__builtin_frame_address(0),
				(unsigned long)dump_backtrace);
	} else {
		/*
		 * task blocked in __switch_to
		 */
		start_backtrace(&frame,
				thread_saved_fp(tsk),
				thread_saved_pc(tsk));
	}

	printk("%sCall trace:\n", loglvl);
	do {
		/* skip until specified stack frame */
		if (!skip) {
			dump_backtrace_entry(frame.pc, loglvl);
		} else if (frame.fp == regs->regs[29]) {
			skip = 0;
			/*
			 * Mostly, this is the case where this function is
			 * called in panic/abort. As exception handler's
			 * stack frame does not contain the corresponding pc
			 * at which an exception has taken place, use regs->pc
			 * instead.
			 */
			dump_backtrace_entry(regs->pc, loglvl);
		}
	} while (!unwind_frame(tsk, &frame));

	put_task_stack(tsk);
}
EXPORT_SYMBOL_GPL(dump_backtrace);

#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
void dump_backtrace_auto_comment(struct pt_regs *regs, struct task_struct *tsk)
{
	struct stackframe frame;
	int skip = 0;

	pr_debug("%s(regs = %p tsk = %p)\n", __func__, regs, tsk);

	if (regs) {
		if (user_mode(regs))
			return;
		skip = 1;
	}

	if (!tsk)
		tsk = current;

	if (!try_get_task_stack(tsk))
		return;

	if (tsk == current) {
		start_backtrace(&frame,
				(unsigned long)__builtin_frame_address(0),
				(unsigned long)dump_backtrace_auto_comment);
	} else {
		/*
		 * task blocked in __switch_to
		 */
		start_backtrace(&frame,
				thread_saved_fp(tsk),
				thread_saved_pc(tsk));
	}

	pr_auto_once(2);
	pr_auto(ASL2, "Call trace:\n");
	do {
		/* skip until specified stack frame */
		if (!skip) {
			dump_backtrace_entry_auto_comment(frame.pc);
		} else if (frame.fp == regs->regs[29]) {
			skip = 0;
			/*
			 * Mostly, this is the case where this function is
			 * called in panic/abort. As exception handler's
			 * stack frame does not contain the corresponding pc
			 * at which an exception has taken place, use regs->pc
			 * instead.
			 */
			dump_backtrace_entry_auto_comment(regs->pc);
		}
	} while (!unwind_frame(tsk, &frame));

	put_task_stack(tsk);
}

void show_stack_auto_comment(struct task_struct *tsk, unsigned long *sp)
{
	dump_backtrace_auto_comment(NULL, tsk);
	barrier();
}
#endif /* CONFIG_SEC_DEBUG_AUTO_COMMENT */

void show_stack(struct task_struct *tsk, unsigned long *sp, const char *loglvl)
{
	dump_backtrace(NULL, tsk, loglvl);
	barrier();
}

#ifdef CONFIG_STACKTRACE

noinline notrace void arch_stack_walk(stack_trace_consume_fn consume_entry,
			      void *cookie, struct task_struct *task,
			      struct pt_regs *regs)
{
	struct stackframe frame;

	if (regs)
		start_backtrace(&frame, regs->regs[29], regs->pc);
	else if (task == current)
		start_backtrace(&frame,
				(unsigned long)__builtin_frame_address(1),
				(unsigned long)__builtin_return_address(0));
	else
		start_backtrace(&frame, thread_saved_fp(task),
				thread_saved_pc(task));

	walk_stackframe(task, &frame, consume_entry, cookie);
}

>>>>>>> upstream/android-13
#endif
