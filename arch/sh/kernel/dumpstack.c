<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 2000, 2001, 2002 Andi Kleen, SuSE Labs
 *  Copyright (C) 2009  Matt Fleming
 *  Copyright (C) 2002 - 2012  Paul Mundt
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/kallsyms.h>
#include <linux/ftrace.h>
#include <linux/debug_locks.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <linux/kdebug.h>
#include <linux/export.h>
#include <linux/uaccess.h>
#include <asm/unwinder.h>
#include <asm/stacktrace.h>

<<<<<<< HEAD
void dump_mem(const char *str, unsigned long bottom, unsigned long top)
=======
void dump_mem(const char *str, const char *loglvl, unsigned long bottom,
	      unsigned long top)
>>>>>>> upstream/android-13
{
	unsigned long p;
	int i;

<<<<<<< HEAD
	printk("%s(0x%08lx to 0x%08lx)\n", str, bottom, top);

	for (p = bottom & ~31; p < top; ) {
		printk("%04lx: ", p & 0xffff);
=======
	printk("%s%s(0x%08lx to 0x%08lx)\n", loglvl, str, bottom, top);

	for (p = bottom & ~31; p < top; ) {
		printk("%s%04lx: ", loglvl,  p & 0xffff);
>>>>>>> upstream/android-13

		for (i = 0; i < 8; i++, p += 4) {
			unsigned int val;

			if (p < bottom || p >= top)
<<<<<<< HEAD
				printk("         ");
			else {
				if (__get_user(val, (unsigned int __user *)p)) {
					printk("\n");
					return;
				}
				printk("%08x ", val);
			}
		}
		printk("\n");
=======
				pr_cont("         ");
			else {
				if (__get_user(val, (unsigned int __user *)p)) {
					pr_cont("\n");
					return;
				}
				pr_cont("%08x ", val);
			}
		}
		pr_cont("\n");
>>>>>>> upstream/android-13
	}
}

void printk_address(unsigned long address, int reliable)
{
<<<<<<< HEAD
	printk(" [<%p>] %s%pS\n", (void *) address,
			reliable ? "" : "? ", (void *) address);
=======
	pr_cont(" [<%px>] %s%pS\n", (void *) address,
		reliable ? "" : "? ", (void *) address);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
static void
print_ftrace_graph_addr(unsigned long addr, void *data,
			const struct stacktrace_ops *ops,
			struct thread_info *tinfo, int *graph)
{
	struct task_struct *task = tinfo->task;
<<<<<<< HEAD
	unsigned long ret_addr;
	int index = task->curr_ret_stack;
=======
	struct ftrace_ret_stack *ret_stack;
	unsigned long ret_addr;
>>>>>>> upstream/android-13

	if (addr != (unsigned long)return_to_handler)
		return;

<<<<<<< HEAD
	if (!task->ret_stack || index < *graph)
		return;

	index -= *graph;
	ret_addr = task->ret_stack[index].ret;
=======
	if (!task->ret_stack)
		return;

	ret_stack = ftrace_graph_get_ret_stack(task, *graph);
	if (!ret_stack)
		return;

	ret_addr = ret_stack->ret;
>>>>>>> upstream/android-13

	ops->address(data, ret_addr, 1);

	(*graph)++;
}
#else
static inline void
print_ftrace_graph_addr(unsigned long addr, void *data,
			const struct stacktrace_ops *ops,
			struct thread_info *tinfo, int *graph)
{ }
#endif

void
stack_reader_dump(struct task_struct *task, struct pt_regs *regs,
		  unsigned long *sp, const struct stacktrace_ops *ops,
		  void *data)
{
	struct thread_info *context;
	int graph = 0;

	context = (struct thread_info *)
		((unsigned long)sp & (~(THREAD_SIZE - 1)));

	while (!kstack_end(sp)) {
		unsigned long addr = *sp++;

		if (__kernel_text_address(addr)) {
			ops->address(data, addr, 1);

			print_ftrace_graph_addr(addr, data, ops,
						context, &graph);
		}
	}
}

<<<<<<< HEAD
static int print_trace_stack(void *data, char *name)
{
	printk("%s <%s> ", (char *)data, name);
	return 0;
}

=======
>>>>>>> upstream/android-13
/*
 * Print one address/symbol entries per line.
 */
static void print_trace_address(void *data, unsigned long addr, int reliable)
{
	printk("%s", (char *)data);
	printk_address(addr, reliable);
}

static const struct stacktrace_ops print_trace_ops = {
<<<<<<< HEAD
	.stack = print_trace_stack,
=======
>>>>>>> upstream/android-13
	.address = print_trace_address,
};

void show_trace(struct task_struct *tsk, unsigned long *sp,
<<<<<<< HEAD
		struct pt_regs *regs)
=======
		struct pt_regs *regs, const char *loglvl)
>>>>>>> upstream/android-13
{
	if (regs && user_mode(regs))
		return;

<<<<<<< HEAD
	printk("\nCall trace:\n");

	unwind_stack(tsk, regs, sp, &print_trace_ops, "");

	printk("\n");
=======
	printk("%s\nCall trace:\n", loglvl);

	unwind_stack(tsk, regs, sp, &print_trace_ops, (void *)loglvl);

	pr_cont("\n");
>>>>>>> upstream/android-13

	if (!tsk)
		tsk = current;

	debug_show_held_locks(tsk);
}

<<<<<<< HEAD
void show_stack(struct task_struct *tsk, unsigned long *sp)
=======
void show_stack(struct task_struct *tsk, unsigned long *sp, const char *loglvl)
>>>>>>> upstream/android-13
{
	unsigned long stack;

	if (!tsk)
		tsk = current;
	if (tsk == current)
		sp = (unsigned long *)current_stack_pointer;
	else
		sp = (unsigned long *)tsk->thread.sp;

	stack = (unsigned long)sp;
<<<<<<< HEAD
	dump_mem("Stack: ", stack, THREAD_SIZE +
		 (unsigned long)task_stack_page(tsk));
	show_trace(tsk, sp, NULL);
=======
	dump_mem("Stack: ", loglvl, stack, THREAD_SIZE +
		 (unsigned long)task_stack_page(tsk));
	show_trace(tsk, sp, NULL, loglvl);
>>>>>>> upstream/android-13
}
