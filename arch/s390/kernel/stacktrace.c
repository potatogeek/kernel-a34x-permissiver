// SPDX-License-Identifier: GPL-2.0
/*
 * Stack trace management functions
 *
 *  Copyright IBM Corp. 2006
 *  Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>
 */

<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/stacktrace.h>
#include <linux/kallsyms.h>
#include <linux/export.h>

static int __save_address(void *data, unsigned long address, int nosched)
{
	struct stack_trace *trace = data;

	if (nosched && in_sched_functions(address))
		return 0;
	if (trace->skip > 0) {
		trace->skip--;
		return 0;
	}
	if (trace->nr_entries < trace->max_entries) {
		trace->entries[trace->nr_entries++] = address;
		return 0;
	}
	return 1;
}

static int save_address(void *data, unsigned long address, int reliable)
{
	return __save_address(data, address, 0);
}

static int save_address_nosched(void *data, unsigned long address, int reliable)
{
	return __save_address(data, address, 1);
}

void save_stack_trace(struct stack_trace *trace)
{
	unsigned long sp;

	sp = current_stack_pointer();
	dump_trace(save_address, trace, NULL, sp);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	unsigned long sp;

	sp = tsk->thread.ksp;
	if (tsk == current)
		sp = current_stack_pointer();
	dump_trace(save_address_nosched, trace, tsk, sp);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);

void save_stack_trace_regs(struct pt_regs *regs, struct stack_trace *trace)
{
	unsigned long sp;

	sp = kernel_stack_pointer(regs);
	dump_trace(save_address, trace, NULL, sp);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace_regs);
=======
#include <linux/stacktrace.h>
#include <asm/stacktrace.h>
#include <asm/unwind.h>
#include <asm/kprobes.h>

void arch_stack_walk(stack_trace_consume_fn consume_entry, void *cookie,
		     struct task_struct *task, struct pt_regs *regs)
{
	struct unwind_state state;
	unsigned long addr;

	unwind_for_each_frame(&state, task, regs, 0) {
		addr = unwind_get_return_address(&state);
		if (!addr || !consume_entry(cookie, addr))
			break;
	}
}

int arch_stack_walk_reliable(stack_trace_consume_fn consume_entry,
			     void *cookie, struct task_struct *task)
{
	struct unwind_state state;
	unsigned long addr;

	unwind_for_each_frame(&state, task, NULL, 0) {
		if (state.stack_info.type != STACK_TYPE_TASK)
			return -EINVAL;

		if (state.regs)
			return -EINVAL;

		addr = unwind_get_return_address(&state);
		if (!addr)
			return -EINVAL;

#ifdef CONFIG_KPROBES
		/*
		 * Mark stacktraces with kretprobed functions on them
		 * as unreliable.
		 */
		if (state.ip == (unsigned long)kretprobe_trampoline)
			return -EINVAL;
#endif

		if (!consume_entry(cookie, addr))
			return -EINVAL;
	}

	/* Check for stack corruption */
	if (unwind_error(&state))
		return -EINVAL;
	return 0;
}
>>>>>>> upstream/android-13
