/*
 * Stack trace management functions
 *
 *  Copyright (C) 2006-2009 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 */
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <linux/stacktrace.h>
#include <linux/export.h>
#include <linux/uaccess.h>
#include <asm/stacktrace.h>
#include <asm/unwind.h>

<<<<<<< HEAD
static int save_stack_address(struct stack_trace *trace, unsigned long addr,
			      bool nosched)
{
	if (nosched && in_sched_functions(addr))
		return 0;

	if (trace->skip > 0) {
		trace->skip--;
		return 0;
	}

	if (trace->nr_entries >= trace->max_entries)
		return -1;

	trace->entries[trace->nr_entries++] = addr;
	return 0;
}

static void noinline __save_stack_trace(struct stack_trace *trace,
			       struct task_struct *task, struct pt_regs *regs,
			       bool nosched)
=======
void arch_stack_walk(stack_trace_consume_fn consume_entry, void *cookie,
		     struct task_struct *task, struct pt_regs *regs)
>>>>>>> upstream/android-13
{
	struct unwind_state state;
	unsigned long addr;

<<<<<<< HEAD
	if (regs)
		save_stack_address(trace, regs->ip, nosched);
=======
	if (regs && !consume_entry(cookie, regs->ip))
		return;
>>>>>>> upstream/android-13

	for (unwind_start(&state, task, regs, NULL); !unwind_done(&state);
	     unwind_next_frame(&state)) {
		addr = unwind_get_return_address(&state);
<<<<<<< HEAD
		if (!addr || save_stack_address(trace, addr, nosched))
			break;
	}

	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}

/*
 * Save stack-backtrace addresses into a stack_trace buffer.
 */
void save_stack_trace(struct stack_trace *trace)
{
	trace->skip++;
	__save_stack_trace(trace, current, NULL, false);
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_regs(struct pt_regs *regs, struct stack_trace *trace)
{
	__save_stack_trace(trace, current, regs, false);
}

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	if (!try_get_task_stack(tsk))
		return;

	if (tsk == current)
		trace->skip++;
	__save_stack_trace(trace, tsk, NULL, true);

	put_task_stack(tsk);
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);

#ifdef CONFIG_HAVE_RELIABLE_STACKTRACE

static int __always_inline
__save_stack_trace_reliable(struct stack_trace *trace,
			    struct task_struct *task)
=======
		if (!addr || !consume_entry(cookie, addr))
			break;
	}
}

int arch_stack_walk_reliable(stack_trace_consume_fn consume_entry,
			     void *cookie, struct task_struct *task)
>>>>>>> upstream/android-13
{
	struct unwind_state state;
	struct pt_regs *regs;
	unsigned long addr;

	for (unwind_start(&state, task, NULL, NULL);
	     !unwind_done(&state) && !unwind_error(&state);
	     unwind_next_frame(&state)) {

		regs = unwind_get_entry_regs(&state, NULL);
		if (regs) {
			/* Success path for user tasks */
			if (user_mode(regs))
<<<<<<< HEAD
				goto success;
=======
				return 0;
>>>>>>> upstream/android-13

			/*
			 * Kernel mode registers on the stack indicate an
			 * in-kernel interrupt or exception (e.g., preemption
			 * or a page fault), which can make frame pointers
			 * unreliable.
			 */
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
			if (IS_ENABLED(CONFIG_FRAME_POINTER))
				return -EINVAL;
		}

		addr = unwind_get_return_address(&state);

		/*
		 * A NULL or invalid return address probably means there's some
		 * generated code which __kernel_text_address() doesn't know
		 * about.
		 */
		if (!addr)
			return -EINVAL;

<<<<<<< HEAD
		if (save_stack_address(trace, addr, false))
=======
		if (!consume_entry(cookie, addr))
>>>>>>> upstream/android-13
			return -EINVAL;
	}

	/* Check for stack corruption */
	if (unwind_error(&state))
		return -EINVAL;

<<<<<<< HEAD
	/* Success path for non-user tasks, i.e. kthreads and idle tasks */
	if (!(task->flags & (PF_KTHREAD | PF_IDLE)))
		return -EINVAL;

success:
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;

	return 0;
}

/*
 * This function returns an error if it detects any unreliable features of the
 * stack.  Otherwise it guarantees that the stack trace is reliable.
 *
 * If the task is not 'current', the caller *must* ensure the task is inactive.
 */
int save_stack_trace_tsk_reliable(struct task_struct *tsk,
				  struct stack_trace *trace)
{
	int ret;

	/*
	 * If the task doesn't have a stack (e.g., a zombie), the stack is
	 * "reliably" empty.
	 */
	if (!try_get_task_stack(tsk))
		return 0;

	ret = __save_stack_trace_reliable(trace, tsk);

	put_task_stack(tsk);

	return ret;
}
#endif /* CONFIG_HAVE_RELIABLE_STACKTRACE */

=======
	return 0;
}

>>>>>>> upstream/android-13
/* Userspace stacktrace - based on kernel/trace/trace_sysprof.c */

struct stack_frame_user {
	const void __user	*next_fp;
	unsigned long		ret_addr;
};

static int
<<<<<<< HEAD
copy_stack_frame(const void __user *fp, struct stack_frame_user *frame)
{
	int ret;

	if (!access_ok(VERIFY_READ, fp, sizeof(*frame)))
=======
copy_stack_frame(const struct stack_frame_user __user *fp,
		 struct stack_frame_user *frame)
{
	int ret;

	if (__range_not_ok(fp, sizeof(*frame), TASK_SIZE))
>>>>>>> upstream/android-13
		return 0;

	ret = 1;
	pagefault_disable();
<<<<<<< HEAD
	if (__copy_from_user_inatomic(frame, fp, sizeof(*frame)))
=======
	if (__get_user(frame->next_fp, &fp->next_fp) ||
	    __get_user(frame->ret_addr, &fp->ret_addr))
>>>>>>> upstream/android-13
		ret = 0;
	pagefault_enable();

	return ret;
}

<<<<<<< HEAD
static inline void __save_stack_trace_user(struct stack_trace *trace)
{
	const struct pt_regs *regs = task_pt_regs(current);
	const void __user *fp = (const void __user *)regs->bp;

	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = regs->ip;

	while (trace->nr_entries < trace->max_entries) {
=======
void arch_stack_walk_user(stack_trace_consume_fn consume_entry, void *cookie,
			  const struct pt_regs *regs)
{
	const void __user *fp = (const void __user *)regs->bp;

	if (!consume_entry(cookie, regs->ip))
		return;

	while (1) {
>>>>>>> upstream/android-13
		struct stack_frame_user frame;

		frame.next_fp = NULL;
		frame.ret_addr = 0;
		if (!copy_stack_frame(fp, &frame))
			break;
		if ((unsigned long)fp < regs->sp)
			break;
<<<<<<< HEAD
		if (frame.ret_addr) {
			trace->entries[trace->nr_entries++] =
				frame.ret_addr;
		}
		if (fp == frame.next_fp)
=======
		if (!frame.ret_addr)
			break;
		if (!consume_entry(cookie, frame.ret_addr))
>>>>>>> upstream/android-13
			break;
		fp = frame.next_fp;
	}
}

<<<<<<< HEAD
void save_stack_trace_user(struct stack_trace *trace)
{
	/*
	 * Trace user stack if we are not a kernel thread
	 */
	if (current->mm) {
		__save_stack_trace_user(trace);
	}
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
=======
>>>>>>> upstream/android-13
