<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * arm64 callchain support
 *
 * Copyright (C) 2015 ARM Limited
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
#include <linux/perf_event.h>
#include <linux/uaccess.h>

<<<<<<< HEAD
=======
#include <asm/pointer_auth.h>
>>>>>>> upstream/android-13
#include <asm/stacktrace.h>

struct frame_tail {
	struct frame_tail	__user *fp;
	unsigned long		lr;
} __attribute__((packed));

/*
 * Get the return address for a single stackframe and return a pointer to the
 * next frame tail.
 */
static struct frame_tail __user *
user_backtrace(struct frame_tail __user *tail,
	       struct perf_callchain_entry_ctx *entry)
{
	struct frame_tail buftail;
	unsigned long err;
<<<<<<< HEAD

	/* Also check accessibility of one struct frame_tail beyond */
	if (!access_ok(VERIFY_READ, tail, sizeof(buftail)))
=======
	unsigned long lr;

	/* Also check accessibility of one struct frame_tail beyond */
	if (!access_ok(tail, sizeof(buftail)))
>>>>>>> upstream/android-13
		return NULL;

	pagefault_disable();
	err = __copy_from_user_inatomic(&buftail, tail, sizeof(buftail));
	pagefault_enable();

	if (err)
		return NULL;

<<<<<<< HEAD
	perf_callchain_store(entry, buftail.lr);
=======
	lr = ptrauth_strip_insn_pac(buftail.lr);

	perf_callchain_store(entry, lr);
>>>>>>> upstream/android-13

	/*
	 * Frame pointers should strictly progress back up the stack
	 * (towards higher addresses).
	 */
	if (tail >= buftail.fp)
		return NULL;

	return buftail.fp;
}

#ifdef CONFIG_COMPAT
/*
 * The registers we're interested in are at the end of the variable
 * length saved register structure. The fp points at the end of this
 * structure so the address of this struct is:
 * (struct compat_frame_tail *)(xxx->fp)-1
 *
 * This code has been adapted from the ARM OProfile support.
 */
struct compat_frame_tail {
	compat_uptr_t	fp; /* a (struct compat_frame_tail *) in compat mode */
	u32		sp;
	u32		lr;
} __attribute__((packed));

static struct compat_frame_tail __user *
compat_user_backtrace(struct compat_frame_tail __user *tail,
		      struct perf_callchain_entry_ctx *entry)
{
	struct compat_frame_tail buftail;
	unsigned long err;

	/* Also check accessibility of one struct frame_tail beyond */
<<<<<<< HEAD
	if (!access_ok(VERIFY_READ, tail, sizeof(buftail)))
=======
	if (!access_ok(tail, sizeof(buftail)))
>>>>>>> upstream/android-13
		return NULL;

	pagefault_disable();
	err = __copy_from_user_inatomic(&buftail, tail, sizeof(buftail));
	pagefault_enable();

	if (err)
		return NULL;

	perf_callchain_store(entry, buftail.lr);

	/*
	 * Frame pointers should strictly progress back up the stack
	 * (towards higher addresses).
	 */
	if (tail + 1 >= (struct compat_frame_tail __user *)
			compat_ptr(buftail.fp))
		return NULL;

	return (struct compat_frame_tail __user *)compat_ptr(buftail.fp) - 1;
}
#endif /* CONFIG_COMPAT */

void perf_callchain_user(struct perf_callchain_entry_ctx *entry,
			 struct pt_regs *regs)
{
<<<<<<< HEAD
	if (perf_guest_cbs && perf_guest_cbs->is_in_guest()) {
=======
	struct perf_guest_info_callbacks *guest_cbs = perf_get_guest_cbs();

	if (guest_cbs && guest_cbs->is_in_guest()) {
>>>>>>> upstream/android-13
		/* We don't support guest os callchain now */
		return;
	}

	perf_callchain_store(entry, regs->pc);

	if (!compat_user_mode(regs)) {
		/* AARCH64 mode */
		struct frame_tail __user *tail;

		tail = (struct frame_tail __user *)regs->regs[29];

		while (entry->nr < entry->max_stack &&
<<<<<<< HEAD
		       tail && !((unsigned long)tail & 0xf))
=======
		       tail && !((unsigned long)tail & 0x7))
>>>>>>> upstream/android-13
			tail = user_backtrace(tail, entry);
	} else {
#ifdef CONFIG_COMPAT
		/* AARCH32 compat mode */
		struct compat_frame_tail __user *tail;

		tail = (struct compat_frame_tail __user *)regs->compat_fp - 1;

		while ((entry->nr < entry->max_stack) &&
			tail && !((unsigned long)tail & 0x3))
			tail = compat_user_backtrace(tail, entry);
#endif
	}
}

/*
 * Gets called by walk_stackframe() for every stackframe. This will be called
 * whist unwinding the stackframe and is like a subroutine return so we use
 * the PC.
 */
<<<<<<< HEAD
static int callchain_trace(struct stackframe *frame, void *data)
{
	struct perf_callchain_entry_ctx *entry = data;
	perf_callchain_store(entry, frame->pc);
	return 0;
=======
static bool callchain_trace(void *data, unsigned long pc)
{
	struct perf_callchain_entry_ctx *entry = data;
	perf_callchain_store(entry, pc);
	return true;
>>>>>>> upstream/android-13
}

void perf_callchain_kernel(struct perf_callchain_entry_ctx *entry,
			   struct pt_regs *regs)
{
<<<<<<< HEAD
	struct stackframe frame;

	if (perf_guest_cbs && perf_guest_cbs->is_in_guest()) {
=======
	struct perf_guest_info_callbacks *guest_cbs = perf_get_guest_cbs();
	struct stackframe frame;

	if (guest_cbs && guest_cbs->is_in_guest()) {
>>>>>>> upstream/android-13
		/* We don't support guest os callchain now */
		return;
	}

<<<<<<< HEAD
	frame.fp = regs->regs[29];
	frame.pc = regs->pc;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame.graph = current->curr_ret_stack;
#endif

=======
	start_backtrace(&frame, regs->regs[29], regs->pc);
>>>>>>> upstream/android-13
	walk_stackframe(current, &frame, callchain_trace, entry);
}

unsigned long perf_instruction_pointer(struct pt_regs *regs)
{
<<<<<<< HEAD
	if (perf_guest_cbs && perf_guest_cbs->is_in_guest())
		return perf_guest_cbs->get_guest_ip();
=======
	struct perf_guest_info_callbacks *guest_cbs = perf_get_guest_cbs();

	if (guest_cbs && guest_cbs->is_in_guest())
		return guest_cbs->get_guest_ip();
>>>>>>> upstream/android-13

	return instruction_pointer(regs);
}

unsigned long perf_misc_flags(struct pt_regs *regs)
{
<<<<<<< HEAD
	int misc = 0;

	if (perf_guest_cbs && perf_guest_cbs->is_in_guest()) {
		if (perf_guest_cbs->is_user_mode())
=======
	struct perf_guest_info_callbacks *guest_cbs = perf_get_guest_cbs();
	int misc = 0;

	if (guest_cbs && guest_cbs->is_in_guest()) {
		if (guest_cbs->is_user_mode())
>>>>>>> upstream/android-13
			misc |= PERF_RECORD_MISC_GUEST_USER;
		else
			misc |= PERF_RECORD_MISC_GUEST_KERNEL;
	} else {
		if (user_mode(regs))
			misc |= PERF_RECORD_MISC_USER;
		else
			misc |= PERF_RECORD_MISC_KERNEL;
	}

	return misc;
}
